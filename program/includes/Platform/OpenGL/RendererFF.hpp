#pragma once

#include "Core/Renderer/Renderer.hpp"
#include "Core/Renderer/RenderCommand.hpp"

#include <cstdint>
#include <vector>

// Cabeçalhos do OpenGL 1.x
#if defined(_Win32)
	#include <windows.h>
#endif

// Para utilização do GL correto no ambiente SDL
#include <SDL2/SDL_opengl.h> 

namespace platform
{
	/**
	 *  RendererFF: implementação de Renderer usando o OpenGL 1.x (fixed-function)
	 *  Mantém BeginFrame/Flush/EndFrame como na API moderna, mas internamente usa
	 * matriz de projeção/modelview e client-side arrays. 
	 */
	class RendererFF : public core::renderer::Renderer
	{
	public:
		RendererFF();
		~RendererFF();

		void BeginFrame(const core::renderer::RendererFrameInfo& info) override;
		void Flush(const core::renderer::RenderQueue& queue) override;
		void EndFrame() override;

	private:
		void setOrtho(int w, int h); // proj ORTHO 2D top-left
		void applyModel2D(float x, float y, float sx, float sy, float rot, float z);
		void setColor4(const std::array<float, 4>& color);
		void bindTexture(uint32_t texID);
		GLenum castPrimitive(core::renderer::PrimitiveType ptype) const;

		unsigned int LoadTexture(const char* texPath);
		int GetTextureWidth(unsigned int texId) const;
		int GetTextureHeight(unsigned int texId) const;

	private:
		int m_VPW=0, m_VPH=0;

		std::vector<core::renderer::ImageInfo> m_TextureSlots;
	};
}