#pragma once

#include "Core/Renderer/RenderCommand.hpp"
#include "Core/Renderer/RenderQueue.hpp"

#include "Core/Utils/Matrix4x4.hpp"

namespace core
{
	namespace renderer
	{
		struct RendererFrameInfo
		{
			int ViewportW, ViewportH;
			mathutils::Matrix4x4 ViewProjection;
		};

		class Renderer
		{
		public:
			virtual ~Renderer() = default;

			virtual void BeginFrame(const RendererFrameInfo& info) = 0;
			virtual void Flush(const RenderQueue& queue) = 0;
			virtual void EndFrame() = 0;

			// helper para criar SortKey (texture/z)
			static uint32_t MakeSortKey(uint32_t tex, float z)
			{
				// texture (24bits) | z invertido (24bits)
				// simplificado: quantiza z para 24 bits [0..1]
				uint32_t zq = (uint32_t)((1.0f - z) * 16777215.0f);
				return ((uint32_t)(tex & 0xFFFFFF) << 24) | ((uint32_t)zq);
			}

			virtual unsigned int LoadTexture(const char* texPath) = 0;
			virtual int GetTextureWidth(unsigned int texId) const = 0;
			virtual int GetTextureHeight(unsigned int texId) const = 0;
		};
	}
}