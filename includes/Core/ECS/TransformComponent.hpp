#pragma once

#include "Core/ECS/IComponent.hpp"

#include "Core/Utils/Vector.hpp"

namespace core
{
	namespace ecs
	{
		class TransformComponent : public IComponent
		{
		public:
			mathutils::Vector position;
			mathutils::Vector scale;
			float rotation;

			TransformComponent(mathutils::Vector position, mathutils::Vector scale, float rotation)
				: position(position), scale(scale), rotation(rotation) {}
		};
	}
}