#pragma once

#include "Core/ECS/Systems/ISystem.hpp"

#include "Core/ECS/LifetimeComponent.hpp"

#include "Core/ECS/Entity.hpp"

namespace core
{
	namespace ecs
	{
		/**
		 * @brief: Sistema responsável por atualizar determinados componentes,
		 * inicialmente pensado para disparar ticks e remover objetos.
		 */
		class GlobalSystem : public ISystem
		{
		public:
			GlobalSystem() {};

			void Update(EntityRegistry& registry, double deltaTime)
			{
				auto& lifetimes = registry.GetPool<LifetimeComponent>();

				for (auto& [e, lifetime] : lifetimes)
				{
					lifetime.timer.Tick();

					if (!lifetime.dead && lifetime.timer.ElapsedTime() >= lifetime.lifetime)
					{
						lifetime.timer.Reset();
						lifetime.timer.Pause();

						registry.DestroyEntity(Entity{e});

						lifetime.dead = true;
						break;
					}
				}
			}
		};
	}
}