#include "Core/Utils/Timer.hpp"

namespace core
{
	namespace utils
	{
		void Timer::Reset()
		{
			m_StartTime = Clock::now();
			m_LastTime = m_StartTime;
			m_DeltaTime = 0.0;
			m_Paused = false;
		}

		void Timer::Pause()
		{
			if (!m_Paused)
			{
				m_PauseTime = Clock::now();
				m_Paused = true;
			}
		}

		void Timer::Resume()
		{
			if (m_Paused)
			{
				auto now = Clock::now();

				m_StartTime += (now - m_PauseTime);
				m_LastTime += (now - m_PauseTime);

				m_Paused = false;
			}
		}

		void Timer::Tick()
		{
			if (m_Paused)
			{
				m_DeltaTime = 0.0;
				return;
			}

			auto now = Clock::now();
			m_DeltaTime = std::chrono::duration<double>(now - m_LastTime).count();
			m_LastTime = now;
		}

		double Timer::DeltaTime() const
		{
			return m_DeltaTime;
		}

		double Timer::ElapsedTime() const
		{
			if (m_Paused)
			{
				return std::chrono::duration<double>(m_PauseTime - m_StartTime).count();
			}
			return std::chrono::duration<double>(Clock::now() - m_StartTime).count();
		}
	}
}