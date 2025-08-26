#pragma once

#include "Core/ECS/IComponent.hpp"
#include "Core/Utils/VertexInfo.hpp"
#include "Core/Utils/Color.hpp"

#include "Core/Renderer/RenderCommand.hpp"

#include <string>

namespace core
{
	namespace ecs
	{
		class ShapeComponent : public IComponent
		{
		public:
			// Identificadores
			uint32_t textureID;    // 0 = sem textura

			// Cor em RGBA
			utils::Color color;

			renderer::PrimitiveType primitive;

			std::vector<utils::VertexInfo> vertices;

			// Para ordenação dos objetos que estão a frente
			float orderZ;

			ShapeComponent(
				const uint32_t&                texID, 
				utils::Color                   color,
				renderer::PrimitiveType        primitive, 
				std::vector<utils::VertexInfo> vertices, 
				float                          orderZ
			) : textureID(texID), color(color),
				primitive(primitive), vertices(vertices), orderZ(orderZ) {}
		};
	}
}