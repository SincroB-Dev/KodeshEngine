#pragma once

#include "Core/Events/EventDispatcher.hpp"
#include "Core/Input/KeyState.hpp"

#include <unordered_map>

namespace core::input
{
	class InputManager
	{
	public:
		InputManager()
			: m_MouseX(0.0f), m_MouseY(0.0f), m_ScrollX(0.0f), m_ScrollY(0.0f)
		{}

		void Update();

		/**
		 * @brief Registra seus eventos no dispatcher de eventos, ele deve ser o ultimo a ser chamado
		 * 		  pois quando estiver na engine, o UI deve ser a primeira a consumir os eventos.
		 **/
		void RegisterEventsOnDispatcher(events::EventDispatcher& dispatcher);

		// [TECLADO]
		bool IsKeyPressed(int keycode) const;
		bool IsKeyHeld(int keycode) const;
		bool IsKeyReleased(int keycode) const;
		bool IsKeyIdle(int keycode) const;

		KeyStateEnum GetState(int keycode) const;
		KeyState* GetKey(int keycode);

		// [MOUSE]
		bool IsMouseButtonPressed(int button) const;

		inline float GetMouseX() const { return m_MouseX; }
		inline float GetMouseY() const { return m_MouseY; }

		inline float GetScrollX() const { return m_ScrollX; }
		inline float GetScrollY() const { return m_ScrollY; }

		inline void ResetScroll() { m_ScrollX = 0.0f; m_ScrollY = 0.0f; }

	private:
		std::unordered_map<int, KeyState> m_Keys;
		std::unordered_map<int, bool> m_MouseButtons;

		float m_MouseX, m_MouseY;
		float m_ScrollX, m_ScrollY;
	};
}