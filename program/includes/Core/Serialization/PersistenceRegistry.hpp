#pragma once

#include <functional>
#include <unordered_map>
#include <typeindex>
#include <nlohmann/json.hpp>

#include "Core/ECS/Entity.hpp"
#include "Core/ECS/EntityRegistry.hpp"
#include "Core/Input/InputManager.hpp"
#include "Core/Events/EventDispatcher.hpp"
#include "Core/Systems/ISystem.hpp"

#include "Core/Serialization/Persistence/JsonComponents.hpp"

#include "Core/Helpers/LogManager.hpp"

namespace core::serialization
{
	/**
	 * @brief Recebe entity e o registry. 
	 **/
	using ComponentSerializeFn   = std::function<nlohmann::json(const ecs::Entity&, ecs::EntityRegistry&)>;

	/**
	 * @brief Recebe entity, registry e json com dados. 
	 **/
	using ComponentDeserializeFn = std::function<void(const ecs::Entity&, ecs::EntityRegistry&, const nlohmann::json&)>;

	/**
	 * @brief Utilização específica para deserializar sistemas.
	 **/
	using DeserializeSystemFn = std::function<std::unique_ptr
		<systems::ISystem> (
			events::EventDispatcher&,
			input::InputManager&,
			const nlohmann::json&
		)
	>;

	/**
	 * Entrada de serializadores.
	 **/
	struct ComponentSerializerEntry
	{
	    std::string Name;
	    std::type_index Tidx;

	    ComponentSerializeFn Serialize;
	    ComponentDeserializeFn Deserialize;
	};

	class PersistenceRegistry
	{
		// Construtor privado.
		PersistenceRegistry() {};

	public:
		/**
		 * @brief Como um registrador que deve ser o mesmo desde o inicio até o fim do programa,
		 * 		  então ele é um singletron forçado. 
		 **/
		static inline PersistenceRegistry& Instance()
		{
			static PersistenceRegistry csr;
			// Reserva 8 slots para entrada de componentes, futuramente aumentar para diminuir interações,
			// considerando que o uso de vectors está cada vez maior no software.
			csr.m_ComponentSerializers.reserve(8);

			// Reserva 4 slots para entrada de sistemas, o software ainda conta com dois sistemas,
			// porém, futuramente se encaixarão outros como AudioManager, e PhysicsManager.
			// csr.m_SystemsDeserializers.reserve(4);

			return csr;
		}

		/**
		 * @brief Método para registro de serializador e deserializador de componentes, muita atenção ao inserir
		 *        componentes no registrador, pois não está sendo feita verificação de chaves existentes, o que
		 *        pode afetar resultados em deserialização de componentes. (em caso de duplicidades)
		 **/
		template<typename T>
		void RegisterComponent(const std::string& name);

		/**
		 * @brief Método para registro de deserializador de sistemas, lembrando que é 1 registro por sistema.
		 **/
		template<typename T, typename... Args>
		void RegisterSystem(const std::string& name, Args&&... args);

		/**
		 * @brief Armazena componentes serializados em um json object.
		 **/
		void SerializeComponents(const ecs::Entity& entity, ecs::EntityRegistry& registry, nlohmann::json& jsonComponents);

		/**
		 * @brief Deserializa json objects em componentes, e os linka diretamente ao entity.
		 **/
		void DeserializeComponents(const ecs::Entity& entity, ecs::EntityRegistry& registry, const nlohmann::json& jsonComponents);

		/**
		 * @brief Deserializa json objects em sistemas, em uma listagem. 
		 **/
		std::unique_ptr<systems::ISystem> DeserializeSystem(const std::string& name, events::EventDispatcher& dispatcher, input::InputManager& input, const nlohmann::json& j);

	private:
		/**
		 * @brief Armazena entradas de serialização/deserialização de componentes ECS. 
		 **/
		std::vector<ComponentSerializerEntry> m_ComponentSerializers;

		/**
		 * @brief Armazena os deserializadores dos sistemas, todo sistema tem um deserializador,
		 *        nem todo deserializador é obrigado a ser utilizado, todo sistema pode ser salvo
		 *        /ou não, porém, todo sistema salvo deve ser carregado!
		 **/
		std::unordered_map<std::string, DeserializeSystemFn> m_SystemsDeserializers;
	};

	template<typename T>
	void PersistenceRegistry::RegisterComponent(const std::string& name)
	{
		ComponentSerializerEntry Entry{
			name, typeid(T),

			// Serialização
			[](const ecs::Entity& entity, ecs::EntityRegistry& registry)
			{
	            auto& comp = *registry.GetComponent<T>(entity);
	            return persistence::SerializeComponent(comp);
	        },

	        // Deserialização
	        [](const ecs::Entity& entity, ecs::EntityRegistry& registry, const nlohmann::json& data)
	    	{
	            registry.AddComponent<T>(entity, persistence::DeserializeComponent<T>(data));
	        }
		};

    	m_ComponentSerializers.push_back(std::move(Entry));
	}

	template<typename T, typename... Args>
	void PersistenceRegistry::RegisterSystem(const std::string& name, Args&&... args)
	{
	    m_SystemsDeserializers[name] =
	        [targs = std::make_tuple(std::forward<Args>(args)...)]
	        (events::EventDispatcher& dispatcher, input::InputManager& input, const nlohmann::json &j) -> std::unique_ptr<systems::ISystem>
		    {
		    	return std::apply(
	                [&](auto&&... unpacked)
			        {
			            // Cria o sistema
			            auto sys = std::make_unique<T>(dispatcher, input, std::forward<decltype(unpacked)>(unpacked)...);

			            // Utiliza a função especializada para carregar os dados.
			            persistence::DeserializeSystem<T>(sys.get(), j);

			            return sys;
			        }, targs
		        );
		    };
	}
}