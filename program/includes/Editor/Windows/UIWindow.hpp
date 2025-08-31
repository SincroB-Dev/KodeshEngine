#pragma once

#include <string>

namespace editor
{
	namespace windows
	{
		class UIWindow
		{
		public:
			inline UIWindow(const std::string& name)
				: m_Name(name), m_IsOpen(true) {}

			virtual void OnImGuiRender() = 0;

			inline virtual bool IsOpen() const final { return m_IsOpen; }
			inline virtual void Close() final { m_IsOpen = false; }
			inline virtual const std::string& GetName() const final { return m_Name; }

		protected:
			std::string m_Name;
			bool m_IsOpen;
		};
	}
}