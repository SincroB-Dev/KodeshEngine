#pragma once

#include "Core/Systems/UILayerManager.hpp"

#include "Core/Serialization/PersistenceRegistry.hpp"

#include <nlohmann/json.hpp>

namespace core::serialization::persistence
{
	template<>
	inline nlohmann::json SerializeSystem<systems::UILayerManager>(const systems::UILayerManager& sm)
	{
		nlohmann::json workspace;

	    return workspace;
	}

	template<>
	inline void DeserializeSystem<systems::UILayerManager>(systems::UILayerManager* uilayer, const nlohmann::json &data)
	{}
}