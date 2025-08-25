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

			    for (auto& component : pool)
			    {
			        for (auto& action : component.second.actions)
			        {
			            if (input.GetState(action.second.keycode) == action.second.status)
			            {
			            	for (auto& callback : action.second.callbacks)
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