#pragma once

#include <functional>
#include <unordered_map>
#include <typeindex>
#include <nlohmann/json.hpp>

#include "Core/ECS/Entity.hpp"
#include "Core/ECS/EntityRegistry.hpp"

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
	using ComponentDeserializeFn = std::function<void(const ecs::Entity&, ecs::EntityRegistry&, nlohmann::json&)>;

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

	class ComponentsRegistry
	{
		// Construtor privado.
		ComponentsRegistry() {};

	public:
		/**
		 * @brief Como um registrador que deve ser o mesmo desde o inicio até o fim do programa,
		 * 		  então ele é um singletron forçado. 
		 **/
		static inline ComponentsRegistry& Instance()
		{
			static ComponentsRegistry csr;
			// Reserva 8 slots para entrada de componentes, futuramente aumentar para diminuir interações,
			// considerando que o uso de vectors está cada vez maior no software.
			csr.m_Serializers.reserve(8);
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
		 * @brief Armazena componentes serializados em um json object.
		 **/
		void SerializeComponents(const ecs::Entity& entity, ecs::EntityRegistry& registry, nlohmann::json& jsonComponents);

		/**
		 * @brief Deserializa json objects em componentes, e os linka diretamente ao entity.
		 **/
		void DeserializeComponents(const ecs::Entity& entity, ecs::EntityRegistry& registry, nlohmann::json& jsonComponents);

	private:
		std::vector<ComponentSerializerEntry> m_Serializers;
	};

	template<typename T>
	void ComponentsRegistry::RegisterComponent(const std::string& name)
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
	        [](const ecs::Entity& entity, ecs::EntityRegistry& registry, nlohmann::json& data)
	    	{
	            registry.AddComponent<T>(entity, persistence::DeserializeComponent<T>(data));
	        }
		};

    	m_Serializers.push_back(std::move(Entry));
	}
}