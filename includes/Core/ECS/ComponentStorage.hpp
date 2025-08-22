#pragma once

#include <unordered_map>
#include <vector>
#include <typeindex>
#include <memory>
#include <cassert>
#include <queue>

#include "Core/ECS/Entity.hpp"
#include "Core/ECS/IComponent.hpp"

namespace core
{
	namespace ecs
	{
		/**
		 * Abstrações para facilitar a remoção sem conhecimento de componentes em sí. 
		 */
		struct IPool
		{
		    virtual ~IPool() = default;
		    virtual void Erase(uint32_t id) = 0;
		};

		template<typename T>
		struct ComponentPool : IPool
		{
		    std::unordered_map<uint32_t, T> data;

		    void Erase(uint32_t id) override
		    {
		        data.erase(id);
		    }
		};

		/**
		 *	@brief: Responsável por armazenar e administrar as pools de componentes,
		 * separada de EntityRegistry para facilitar manutenção
		 */
		class ComponentStorage
		{
		public:
			// Adiciona um componente por tipo
			template<typename T, typename ...Args>
			T& AddComponent(uint32_t id, Args&&... args);

			// Captura um componente por tipo
			template<typename T>
			T* GetComponent(uint32_t id);

			// Verifica se a entidade possui um componente
			template<typename T>
			bool HasComponent(uint32_t id);

			// Remove o componente
			template<typename T>
			void RemoveComponent(uint32_t id);

			// Consulta de pool
			template<typename T>
			std::unordered_map<uint32_t, T>& GetPool();

			inline void RemoveIDFromPools(uint32_t id)
			{
			    for (auto& [type, poolPtr] : m_ComponentPools)
			    {
			        poolPtr->Erase(id);
			    }
			}

		private:
			// Um pool de componentes por tipo
			std::unordered_map<std::type_index, std::shared_ptr<IPool>> m_ComponentPools;
		};

		// Adiciona um componente por tipo
		template<typename T, typename... Args>
		T& ComponentStorage::AddComponent(uint32_t id, Args&&... args)
		{
			auto& pool = GetPool<T>();
		    auto [it, inserted] = pool.try_emplace(id, std::forward<Args>(args)...);
		    if (!inserted)
		    {
		    	std::runtime_error("Não foi possível incluir componente: " + std::string(typeid(T).name()));
		    }
		    return it->second;
		}

		// Captura um componente por tipo
		template<typename T>
		inline T* ComponentStorage::GetComponent(uint32_t id)
		{
			auto& pool = GetPool<T>();
			auto it = pool.find(id);
			if (it != pool.end())
			{
				return &it->second;
			}
			return nullptr;
		}

		// Verifica se a entidade possui um componente
		template<typename T>
		inline bool ComponentStorage::HasComponent(uint32_t id)
		{
			auto& pool = GetPool<T>();
			return pool.find(id) != pool.end();
		}

		// Remove o componente
		template<typename T>
		inline void ComponentStorage::RemoveComponent(uint32_t id)
		{
			auto& pool = GetPool<T>();
			pool.erase(id);
		}

		template<typename T>
		inline std::unordered_map<uint32_t, T>& ComponentStorage::GetPool()
		{
		    std::type_index typeIndex(typeid(T));
		    if (m_ComponentPools.find(typeIndex) == m_ComponentPools.end())
		    {
		        m_ComponentPools[typeIndex] = std::make_shared<ComponentPool<T>>();
		    }

		    auto pool = std::static_pointer_cast<ComponentPool<T>>(m_ComponentPools[typeIndex]);
		    return pool->data;
		}
	}
}