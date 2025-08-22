#pragma once

#include <unordered_map>
#include <vector>
#include <typeindex>
#include <memory>
#include <cassert>
#include <queue>

#include "Core/ECS/Entity.hpp"
#include "Core/ECS/IComponent.hpp"
#include "Core/ECS/ComponentStorage.hpp"
#include "Core/Utils/UniqueIDGen.hpp"

namespace core
{
	namespace ecs
	{
		class EntityRegistry
		{
		public:
			EntityRegistry() {};

			// Adiciona um componente por tipo
			template<typename T, typename ...Args>
			T& AddComponent(Entity e, Args&&... args)
			{
				m_ComponentStorage.AddComponent<T>(e.ID, std::forward<Args>(args)...);
			}

			// Captura um componente por tipo
			template<typename T>
			T* GetComponent(Entity e)
			{
				return m_ComponentStorage.GetComponent<T>(e.ID);
			}

			// Verifica se a entidade possui um componente
			template<typename T>
			bool HasComponent(Entity e)
			{
				return m_ComponentStorage.HasComponent<T>(e.ID);
			}

			// Remove o componente
			template<typename T>
			void RemoveComponent(Entity e)
			{
				m_ComponentStorage.RemoveComponent<T>(e.ID);
			}

			// Consulta de pool
			template<typename T>
			inline std::unordered_map<uint32_t, T>& GetPool()
			{
				return m_ComponentStorage.GetPool<T>(); 
			}

			// Cria uma nova entidade, reutilizando o ID se possível
			ecs::Entity CreateEntity();

			// Consulta entities
			const std::vector<Entity>& GetEntities() const;

			// Marca a entidade como destruída e libera o ID
			void DestroyEntity(const ecs::Entity& e);

			// Verifica se a entidade ainda é válida
			bool IsAlive(const ecs::Entity e) const;

		private:
			std::vector<Entity> m_Entities;

			utils::UniqueIDGen m_NextID;
			ComponentStorage m_ComponentStorage;
		};
	}
}