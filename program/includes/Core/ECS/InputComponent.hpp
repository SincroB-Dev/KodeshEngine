#pragma once

#include "Core/ECS/IComponent.hpp"
#include "Core/Input/InputManager.hpp"
#include "Core/Events/Event.hpp"
#include "Core/Input/KeyState.hpp"

#include <string>
#include <vector>
#include <functional>

namespace core
{
	namespace ecs
	{
		using ComponentCallbackFn = std::function<void(EntityRegistry& entities, double&/*deltatime*/)>;

		struct InputAction
		{
			int keycode; // Tecla que ativa a action
			input::KeyStateEnum status; // Status que ativa a action
			std::vector<ComponentCallbackFn> callbacks; // Lista de callbacks que a action vai executar
		};

		class InputComponent : public IComponent
		{
		public:
			std::unordered_map<std::string, InputAction> actions;

			inline void AddAction(
				const std::string& name, const int& keycode,
				ComponentCallbackFn callback, 
				input::KeyStateEnum status
			)
			{
				actions[name] = InputAction{keycode, status};
				actions[name].callbacks.push_back(callback);
			}

			inline void RemoveAction(const std::string& name)
			{
				actions.erase(name);
			}

			InputComponent() {}
		};
	}
}