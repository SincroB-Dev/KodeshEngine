#pragma once

#include "Core/Systems/UILayerManager.hpp"

#include "Core/Serialization/SerializerRegistry/ComponentsRegistry.hpp"

#include <nlohmann/json.hpp>

namespace core::serialization::persistence
{
	template<>
	inline nlohmann::json SerializeSystem<systems::UILayerManager>(const systems::UILayerManager& sm)
	{
		nlohmann::json workspace;

	    return workspace;
	}
}