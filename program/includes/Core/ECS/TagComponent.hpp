#pragma once

#include "Core/ECS/IComponent.hpp"

#include <string>

namespace core
{
	namespace ecs
	{
		class TagComponent : public IComponent
		{
		public:
			std::string tag;
			std::string name; // Deve ser único na listagem de componentes;

			TagComponent(std::string name, std::string tag)
				: tag(tag), name(name) {}
		};
	}
}