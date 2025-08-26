#include "Core/Input/KeyState.hpp"

namespace core
{
	namespace input
	{
		// Helper para depuração.
		std::ostream& operator << (std::ostream& os, KeyStateEnum state)
		{
			switch (state)
			{
				case KeyStateEnum::Idle: return os << "Idle";
				case KeyStateEnum::Pressed: return os << "Pressed";
				case KeyStateEnum::Held: return os << "Held";
				case KeyStateEnum::Released: return os << "Released";
			}
			return os << "Unknown";
		}

		void KeyState::Press()
		{
			if (m_State == KeyStateEnum::Idle || m_State == KeyStateEnum::Released)
			{
				m_State = KeyStateEnum::Pressed;
			}
			// se já estava held ou just pressed, continua
			else
			{
				m_State = KeyStateEnum::Held;
			}
		}

		void KeyState::Release()
		{
			if (m_State == KeyStateEnum::Held || m_State == KeyStateEnum::Pressed)
			{
				m_State = KeyStateEnum::Released;
			}
			// se já estava held ou justpressed, continua
			else
			{
				m_State = KeyStateEnum::Idle;
			}
		}

		void KeyState::Update()
		{
			// avança estados para o proximo frame
			if (m_State == KeyStateEnum::Pressed)
			{
				m_State = KeyStateEnum::Held;
			}
			else if (m_State == KeyStateEnum::Released)
			{
				m_State = KeyStateEnum::Idle;
			}
		}

		bool KeyState::IsPressed() const
		{
			return m_State == KeyStateEnum::Pressed;
		}

		bool KeyState::IsHeld() const
		{
			return m_State == KeyStateEnum::Held;
		}

		bool KeyState::IsReleased() const
		{
			return m_State == KeyStateEnum::Released;
		}

		bool KeyState::IsIdle() const
		{
			return m_State == KeyStateEnum::Idle;
		}

		KeyStateEnum KeyState::GetState() const
		{
			return m_State;
		}
	}
}