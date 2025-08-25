#pragma once

#include "Core/Utils/UniqueIDGen.hpp"

#include <string>
#include <functional>

namespace editor
{
	namespace ui
	{
		/**
		 * Abstração para utilização em interfaces 
		 */
		class UIComponent
		{
		public:
			UIComponent(core::utils::UniqueID uid)
				: UID(uid) {}
			virtual ~UIComponent() = default;

			const core::utils::UniqueID UID;

			virtual void Render() = 0;
		};
	}
}