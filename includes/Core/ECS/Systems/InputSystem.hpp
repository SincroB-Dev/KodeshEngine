#pragma once

#include "Core/ECS/Systems/ISystem.hpp"

#include "Core/ECS/InputComponent.hpp"

#include "Core/Input/InputManager.hpp"

#include <iostream>

namespace core
{
	namespace ecs
	{
		/**
		 * @brief: Este sistema é responsável por ativar inputs, chamando pilhas de
		 * callbacks na pilha de componentes do tipo input. 
		 */
		class InputSystem : public ISystem
		{
		public:
			inline InputSystem(input::InputManager& input) 
				: input(input) {}

			inline void Update(EntityRegistry& registry, double deltaTime) override
			{
				auto& pool = registry.GetPool<InputComponent>();

			    for (auto& [entity, component] : pool)
			    {
			        for (auto& [name, action] : component.actions)
			        {
			            if (input.GetState(action.keycode) == action.status)
			            {
			            	for (auto& callback : action.callbacks)
			            	{
			                	callback(deltaTime);
			            	}
			            }
			        }
			    }
			}

		private:
			input::InputManager& input;
		};
	}
}