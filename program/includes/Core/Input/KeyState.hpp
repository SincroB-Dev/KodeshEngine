#pragma once

#include <ostream>
#include <iostream>

namespace core
{
	namespace input
	{
		enum class KeyStateEnum
		{
			Idle = 0,    // Não pressionada
			Pressed, // Pressoonada agora
			Held,    // Mantida
			Released // Acabou de soltar
		};

		class KeyState
		{
		public:
			KeyState() : m_State(KeyStateEnum::Idle) {}

			void Press();
			void Release();
			void Update();

			bool IsPressed() const;
			bool IsHeld() const;
			bool IsReleased() const;
			bool IsIdle() const;

			KeyStateEnum GetState() const;

		private:
			KeyStateEnum m_State;
		};
	}
}