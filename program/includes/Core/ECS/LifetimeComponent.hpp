#pragma once

#include "Core/ECS/IComponent.hpp"
#include "Core/Utils/Timer.hpp"

namespace core
{
	namespace ecs
	{
		/**
		 * @brief: Tempo de vida de componente, geralmente utilizado para eliminar ele
		 * passado um periodo. 
		 */
		class LifetimeComponent : IComponent
		{
		public:
			utils::Timer timer;
			double lifetime;
			bool dead = false;

			LifetimeComponent(double lifetime): lifetime(lifetime) {}

			LifetimeComponent(const LifetimeComponent& lf): LifetimeComponent(lf.lifetime) {}
		};
	}
}