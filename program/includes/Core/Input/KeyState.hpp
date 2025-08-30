#pragma once

#include <ostream>
#include <iostream>
#include <array>

#include "Core/Utils/Metadata.hpp"

namespace core::input
{
	enum class KeyStateEnum
	{
		Idle     = 0, // Não pressionada
		Pressed  = 1, // Pressoonada agora
		Held     = 2, // Mantida
		Released = 3  // Acabou de soltar
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

namespace core
{
	/**
	 * @brief Registro manual de metadados.
	 **/
	template<>
	struct EnumRegistry<input::KeyStateEnum> {
	    static constexpr std::array<EnumEntry, 4> entries = {{
	        {"Idle", static_cast<int>(input::KeyStateEnum::Idle)},
	        {"Pressed", static_cast<int>(input::KeyStateEnum::Pressed)},
	        {"Held", static_cast<int>(input::KeyStateEnum::Held)},
	        {"Released", static_cast<int>(input::KeyStateEnum::Released)}
	    }};
	};

	static constexpr auto KeyStateEnumDescriptor = 
		MakeEnumDescriptor<input::KeyStateEnum>("KeyStateEnum");
}