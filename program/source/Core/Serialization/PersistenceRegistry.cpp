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

	void PersistenceRegistry::DeserializeComponents(const ecs::Entity& entity, ecs::EntityRegistry& registry, const nlohmann::json& jsonComponents)
	{
		for (auto [key, value] : jsonComponents.items())
		{
			for (auto& s : m_ComponentSerializers)
			{
	            if (!value.is_null() && std::string(key) == s.Name) 
	            {
	            	s.Deserialize(entity, registry, value);
	            }
	        }
		}
	}

	std::unique_ptr<systems::ISystem> PersistenceRegistry::DeserializeSystem(const std::string& name, events::EventDispatcher& dispatcher, input::InputManager& input, const nlohmann::json& j)
	{
		if (m_SystemsDeserializers.count(name))
		{
			return m_SystemsDeserializers[name](dispatcher, input, j);
		}

		return nullptr;
	}
}