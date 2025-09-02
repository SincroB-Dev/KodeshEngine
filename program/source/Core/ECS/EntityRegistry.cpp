#include "Core/ECS/EntityRegistry.hpp"

namespace core
{
	namespace ecs
	{
		// Cria uma nova entidade, reutilizando o ID se possível
		ecs::Entity EntityRegistry::CreateEntity()
		{
			m_Entities.push_back(m_NextID.CreateUniqueID());
			return m_Entities.back();
		}

		// Marca a entidade como destruída e libera o ID
		void EntityRegistry::DestroyEntity(ecs::Entity e)
		{
			if (m_NextID.DestroyID(e))
			{
				// Livra o objeto das pools
			    m_ComponentStorage.RemoveIDFromPools(e.ID);
			}
		}

		const std::vector<Entity>& EntityRegistry::GetEntities() const
		{
			return m_Entities;
		}

		EntityRegistry EntityRegistry::GetCopy() const 
        {
            return EntityRegistry(m_Entities, m_ComponentStorage.GetCopy(), m_NextID);
        }
	}
}