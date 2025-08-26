#include "Core/Input/InputManager.hpp"

#include "Core/Events/WindowEvent.hpp"
#include "Core/Events/MouseEvent.hpp"
#include "Core/Events/KeyboardEvent.hpp"

namespace core
{
	namespace input
	{
		void InputManager::RegisterEventsOnDispatcher(events::EventDispatcher& dispatcher)
		{
			// key events
			dispatcher.Register<events::KeyPressedEvent>([this](events::Event& e) {
				auto& ke = static_cast<events::KeyPressedEvent&>(e);
				m_Keys[ke.GetKeyCode()].Press();
			});

			dispatcher.Register<events::KeyReleasedEvent>([this](events::Event& e) {
				auto& ke = static_cast<events::KeyReleasedEvent&>(e);
				m_Keys[ke.GetKeyCode()].Release();
			});

			// mouse events
			dispatcher.Register<events::MouseButtonPressedEvent>([this](events::Event& e) {
				auto& me = static_cast<events::MouseButtonPressedEvent&>(e);
				m_MouseButtons[me.GetButton()] = true;
			});

			dispatcher.Register<events::MouseButtonReleasedEvent>([this](events::Event& e) {
				auto& me = static_cast<events::MouseButtonReleasedEvent&>(e);
				m_MouseButtons[me.GetButton()] = false;
			});

			dispatcher.Register<events::MouseMovedEvent>([this](events::Event& e) {
				auto& me = static_cast<events::MouseMovedEvent&>(e);
				if (e.Handled)
				m_MouseX = me.GetX();
				m_MouseY = me.GetY();
			});

			dispatcher.Register<events::MouseScrolledEvent>([this](events::Event& e) {
				auto& me = static_cast<events::MouseScrolledEvent&>(e);
				m_ScrollX = me.GetXOffset();
				m_ScrollY = me.GetYOffset();
			});
		}

		void InputManager::Update()
		{
			for (auto& key : m_Keys)
			{
				key.second.Update();
			}
		}

		// [TECLADO]
		bool InputManager::IsKeyPressed(int keycode) const
		{
			auto it = m_Keys.find(keycode);
			return it != m_Keys.end() && it->second.IsPressed();
		}

		bool InputManager::IsKeyHeld(int keycode) const
		{
			auto it = m_Keys.find(keycode);
			return it != m_Keys.end() && it->second.IsHeld();
		}

		bool InputManager::IsKeyReleased(int keycode) const
		{
			auto it = m_Keys.find(keycode);
			return it != m_Keys.end() && it->second.IsReleased();
		}

		bool InputManager::IsKeyIdle(int keycode) const
		{
			auto it = m_Keys.find(keycode);
			return it != m_Keys.end() && it->second.IsIdle();
		}

		KeyStateEnum InputManager::GetState(int keycode) const
		{
			auto it = m_Keys.find(keycode);
			return it != m_Keys.end() ? it->second.GetState() : KeyStateEnum::Idle;
		}

		KeyState* InputManager::GetKey(int keycode)
		{
			auto it = m_Keys.find(keycode);
			return it != m_Keys.end() ? &it->second : nullptr;
		}

		bool InputManager::IsMouseButtonPressed(int button) const
		{
			auto it = m_MouseButtons.find(button);
			return it != m_MouseButtons.end() ? it->second : false;
		}
	}
}