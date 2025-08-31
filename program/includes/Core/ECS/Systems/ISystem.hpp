#pragma once

#include "Core/ECS/EntityRegistry.hpp"

namespace core
{
	namespace ecs
	{
		class ISystem
		{
		public:
			virtual ~ISystem() = default;
			virtual void Update(EntityRegistry& registry, double deltaTime) = 0; 
		};
	}
}