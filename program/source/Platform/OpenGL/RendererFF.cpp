#include "Platform/OpenGL/RendererFF.hpp"
#include "Core/Helpers/LogManager.hpp"

#include <algorithm>
#include <vector>
#include <iostream>
#include <cstring>

#include "Core/Maths/Vector.hpp"

extern "C" {
    #define STB_IMAGE_IMPLEMENTATION
    #define STB_IMAGE_STATIC
    #include <stb_image/stb_image.h>
}

// Utilização de vetores, para facilitar.
using namespace core::mathutils;
using namespace core::renderer;
using namespace core::systems;

namespace platform
{
	RendererFF::RendererFF()
	{
		// estado fixo básico
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	RendererFF::~RendererFF()
	{
		// nada (client-size não precisam de delete)
	}

	void RendererFF::setOrtho(int w, int h)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, w, h, 0, -1, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		m_VPW = w; m_VPH = h;
	}

	void RendererFF::setColor4(const std::array<float, 4>& c)
	{
		glColor4f(c[0], c[1], c[2], c[3]);
	}

	void RendererFF::applyModel2D(float x, float y, float sx, float sy, float rot, float z)
	{
		glTranslatef(x, y, z);
		glScalef(sx, sy, 0.0f);
		glRotatef(rot, 0.0f, 0.0f, 1.0f);
	}

	void RendererFF::bindTexture(uint32_t texID)
	{
		if (texID)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texID);
		}
		else
		{
			glDisable(GL_TEXTURE_2D);
		}
	}

	void RendererFF::BeginFrame(const RendererFrameInfo& info)
	{
		float worldHeight = 5.0f;
		float aspect = (float)info.ViewportW / (float)info.ViewportH;
		float worldWidth = worldHeight * aspect;

		// info.ViewProjection é ignorado no FF, a projeção aqui é setada via setOrtho
		glViewport(0, 0, info.ViewportW, info.ViewportH);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// glOrtho(0, info.ViewportW, info.ViewportH, 0, -1, 1); // Y invertido
		glOrtho(-worldWidth, worldWidth, -worldHeight, worldHeight, -1, 1); // Y invertido
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	GLenum RendererFF::castPrimitive(PrimitiveType ptype) const
	{
		switch (ptype)
		{
			case PrimitiveType::Triangles:
				return GL_TRIANGLES;

			case PrimitiveType::Lines:
				return GL_LINES;

			case PrimitiveType::TriangleFan:
				return GL_TRIANGLE_FAN;

			case PrimitiveType::Quads:
				return GL_QUADS;

			case PrimitiveType::Polygon:
				return GL_POLYGON;
		}	

		return GL_QUADS;
	}

	void RendererFF::Flush(const RenderQueue& queue)
	{
		// copia & ordena por sortKey (texture/z)
		std::vector<RenderCommand> cmds = queue.Commands();
		std::sort(cmds.begin(), cmds.end(), [](const auto& a, const auto& b){
			return a.sortKey < b.sortKey;
		});

		for (const auto& cmd : cmds)
		{
			glPushMatrix();

			if (cmd.textureID > 0)
			{
				bindTexture(cmd.textureID);
			}

			setColor4(cmd.color);
			applyModel2D(cmd.x, cmd.y, cmd.sx, cmd.sy, cmd.rot, cmd.z);

			glBegin(castPrimitive(cmd.primitive));
			{
				for (const auto& v : cmd.vertices)
				{
					glTexCoord2f(v.uv.x, v.uv.y);
					glVertex2f(v.vertex.x, v.vertex.y);
				}
			}

			glEnd();
			glPopMatrix();
		}

		// Limpeza de cor
		glColor4f(1, 1, 1, 1);
	}

	void RendererFF::EndFrame()
	{
		// Sem trabalho aqui, o swap é feito pelo SDL
	}

	unsigned int RendererFF::LoadTexture(const char* texPath)
	{
		core::renderer::ImageInfo image;

	    unsigned char* data = stbi_load(texPath, &image.width, &image.height, &image.channels, 4); // força RGBA
	    if (!data)
	    {
	        LogManager::Log(LogType::Error, "Não foi possível carregar a imagem: ", texPath);
	        return 0;
	    }

	    glGenTextures(1, &image.texId);
	    glBindTexture(GL_TEXTURE_2D, image.texId);

	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	    glTexImage2D(
	        GL_TEXTURE_2D, 0, GL_RGBA,
	        image.width, image.height, 0,
	        GL_RGBA, GL_UNSIGNED_BYTE, data
	    );

	    m_TextureSlots.push_back(image);

	    stbi_image_free(data);
	    return image.texId;
	}

	int RendererFF::GetTextureWidth(unsigned int texID) const
	{
		for (auto tex : m_TextureSlots)
			if (tex.texId == texID)
				return tex.width;

		return 0;
	}

	int RendererFF::GetTextureHeight(unsigned int texID) const
	{
		for (auto tex : m_TextureSlots)
			if (tex.texId == texID)
				return tex.height;

		return 0;
	}
}