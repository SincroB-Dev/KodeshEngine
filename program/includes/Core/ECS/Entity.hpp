#pragma once

#include "Core/Utils/UniqueIDGen.hpp"

namespace core
{
	namespace ecs
	{
		struct Entity : utils::UniqueID 
		{
			using UniqueID::UniqueID;

			Entity(const UniqueID& uid): UniqueID(uid) {}
		};
	}
}