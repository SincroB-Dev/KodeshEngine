#pragma once

#include "Core/Renderer/RenderCommand.hpp"

#include <vector>

namespace core
{
	namespace renderer
	{
		class RenderQueue
		{
		public:
			inline void Clear() { m_Commands.clear(); }
			inline void Submit(const RenderCommand& command) { m_Commands.push_back(command); }

			const std::vector<RenderCommand>& Commands() const { return m_Commands; }
			std::vector<RenderCommand>& Commands() { return m_Commands; }

		private:
			std::vector<RenderCommand> m_Commands;
		};
	}
}