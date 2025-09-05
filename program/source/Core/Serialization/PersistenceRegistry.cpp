#include "Core/Serialization/PersistenceRegistry.hpp"

#include <string>

namespace core::serialization
{
	void PersistenceRegistry::SerializeComponents(const ecs::Entity& entity, ecs::EntityRegistry& registry, nlohmann::json& jsonComponents)
	{
		for (auto& s : m_ComponentSerializers)
		{
            if (registry.HasComponent(s.Tidx, entity)) 
            {
                jsonComponents[s.Name] = s.Serialize(entity, registry);
            }
        }
	}

	void PersistenceRegistry::DeserializeComponents(const ecs::Entity& entity, ecs::EntityRegistry& registry, nlohmann::json& jsonComponents)
	{
		for (auto& s : m_ComponentSerializers)
		{
        	systems::LogManager::Log(systems::LogType::EDebug, s.Name);

            if (registry.HasComponent(s.Tidx, entity)) 
            {
            	systems::LogManager::Log(systems::LogType::EDebug, "Founded: ", s.Name);

                jsonComponents[s.Name] = s.Serialize(entity, registry);
            }
        }
	}
}