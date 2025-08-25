#pragma once

#include <cstdint>
#include <array>
#include <vector>

#include "Core/Utils/VertexInfo.hpp"

namespace core
{
	namespace renderer
	{
		enum class PrimitiveType
		{
			Triangles,
			Lines,
			TriangleFan,
			Quads,
			Polygon
		};

		struct ImageInfo
		{
			unsigned int texId;
			int width, height;
			int channels;
		};

		struct RenderCommand
		{
			PrimitiveType primitive;

			// Identificadores
			uint32_t textureID;    // 0 = sem textura

			// Transform 2D
			float x, y;
			float sx, sy;
			float rot;

			// Z para ordenação (UI > cena)
			float z;

			// Cor em RGBA
			std::array<float, 4> color;

			std::vector<utils::VertexInfo> vertices;

			// Batch key (para sort estável por shader/tex)
			uint32_t sortKey; // Preenchido pelo Renderer
		};
	}
}