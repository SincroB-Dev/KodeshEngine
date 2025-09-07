#include "Core/ECS/ComponentStorage.hpp"
#include "Core/ECS/IComponent.hpp"

namespace core::ecs
{
	bool ComponentStorage::HasComponent(std::type_index tidx, uint32_t id)
    {
        auto it = m_ComponentPools.find(tidx);

        if (it != m_ComponentPools.end())
        {
            auto* pool = static_cast<ComponentPool<IComponent>*>(it->second.get());
            return pool->data.find(id) != pool->data.end(); 
        }
        
        return false;
    }
}