#pragma once

#include "Core/Serialization/PersistenceFwd.hpp"

#include "Core/ECS/TransformComponent.hpp"
#include "Core/ECS/ShapeComponent.hpp"
#include "Core/ECS/TagComponent.hpp"
#include "Core/ECS/InputComponent.hpp"
#include "Core/ECS/LifetimeComponent.hpp"

#include <nlohmann/json.hpp>

#include "Core/Renderer/Renderer.hpp"

#include "Core/Maths/VertexInfo.hpp"
#include "Core/Maths/Vector.hpp"
#include "Core/Maths/Color.hpp"

namespace core::serialization::persistence
{
	//--------------------------------
	// TransformComponent
	//--------------------------------

	template<>
	inline nlohmann::json SerializeComponent<ecs::TransformComponent>(const ecs::TransformComponent& c)
	{
	    return { 
	    	{ "position", {c.position.x, c.position.y} }, 
	    	{ "scale", {c.scale.x, c.scale.y} },
			{ "rotation", c.rotation } 
		};
	}

	template<>
	inline ecs::TransformComponent DeserializeComponent<ecs::TransformComponent>(const nlohmann::json& data)
	{
	    return ecs::TransformComponent(
	    	mathutils::Vector(data["position"][0], data["position"][1]),
	    	mathutils::Vector(data["scale"][0], data["scale"][1]),
	    	data["rotation"]
    	);
	}

	//--------------------------------
	// ShapeComponent
	//--------------------------------

	template<>
	inline nlohmann::json SerializeComponent<ecs::ShapeComponent>(const ecs::ShapeComponent& c)
	{
		nlohmann::json vertices;

		for (auto& v : c.vertices)
		{
			vertices["vertex"].push_back({v.vertex.x, v.vertex.y});
			vertices["uv"].push_back({v.uv.x, v.uv.y});
		}

	    return { 
	    	{ "textureID", c.textureID }, 
	    	{ "orderZ", c.orderZ },
	    	{ "color", {c.color.r, c.color.g, c.color.b, c.color.a} },
	    	{ "vertices", vertices },
	    	{ "primitive", static_cast<int>(c.primitive) }
	    };
	}

	template<>
	inline ecs::ShapeComponent DeserializeComponent<ecs::ShapeComponent>(const nlohmann::json& data)
	{
		std::vector<utils::VertexInfo> vertices;
		nlohmann::json jsonVertices = data["vertices"];

		for (size_t i = 0; i < jsonVertices["vertex"].size(); i++)
		{
			vertices.push_back(utils::VertexInfo{
				// vertices
				mathutils::Vector(jsonVertices["vertex"][i][0], jsonVertices["vertex"][i][1]),

				// uv
				i < jsonVertices["uv"].size()
				? mathutils::Vector(jsonVertices["uv"][i][0], jsonVertices["uv"][i][1])
				: mathutils::Vector()
			});
		}

	    return ecs::ShapeComponent(
		    data["textureID"],
		    utils::Color(data["color"][0], data["color"][1], data["color"][2], data["color"][3]),
		    static_cast<renderer::PrimitiveType>(data["primitive"]),
		    vertices,
		    data["orderZ"]
	    );
	}

	//--------------------------------
	// TagComponent
	//--------------------------------

	template<>
	inline nlohmann::json SerializeComponent<ecs::TagComponent>(const ecs::TagComponent& c)
	{
	    return { 
	    	{ "name", c.name }, 
	    	{ "tag", c.tag }
		};
	}

	template<>
	inline ecs::TagComponent DeserializeComponent<ecs::TagComponent>(const nlohmann::json& data)
	{
	    return ecs::TagComponent(
	    	data["name"],
	    	data["tag"]
    	);
	}

	//--------------------------------
	// LifetimeComponent
	//--------------------------------
	
	template<>
	inline nlohmann::json SerializeComponent<ecs::LifetimeComponent>(const ecs::LifetimeComponent& c)
	{
	    return { 
	    	{ "lifetime", c.lifetime }
		};
	}

	template<>
	inline ecs::LifetimeComponent DeserializeComponent<ecs::LifetimeComponent>(const nlohmann::json& data)
	{
	    return ecs::LifetimeComponent(data["lifetime"]);
	}

	//--------------------------------
	// InputComponent
	//--------------------------------
	
	template<>
	inline nlohmann::json SerializeComponent<ecs::InputComponent>(const ecs::InputComponent& c)
	{
	    return {};
	}

	template<>
	inline ecs::InputComponent DeserializeComponent<ecs::InputComponent>(const nlohmann::json& data)
	{
	    return ecs::InputComponent();
	}
}