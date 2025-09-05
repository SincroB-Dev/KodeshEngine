#include "Core/Serialization/SerializerRegistry/ComponentsRegistry.hpp"

#include <string>

namespace core::serialization
{
	void ComponentsRegistry::SerializeComponents(const ecs::Entity& entity, ecs::EntityRegistry& registry, nlohmann::json& jsonComponents)
	{
		for (auto& s : m_Serializers)
		{
            if (registry.HasComponent(s.Tidx, entity)) 
            {
                jsonComponents[s.Name] = s.Serialize(entity, registry);
            }
        }
	}

	void ComponentsRegistry::DeserializeComponents(const ecs::Entity& entity, ecs::EntityRegistry& registry, nlohmann::json& jsonComponents)
	{
		for (auto& s : m_Serializers)
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