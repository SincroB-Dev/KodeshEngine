#pragma once

#include <chrono>

namespace core
{
	namespace utils
	{
		class Timer
		{
		public:
			inline Timer() { Reset(); }

			// Reseta o timer
			void Reset();

			// Pausa o contador
			void Pause();

			// Resume o contador
			void Resume();

			// Atualiza o deltaTime
			void Tick();

			// Delta time em segundos
			double ElapsedTime() const;

			// Tempo total desde o reset em segundos
			double DeltaTime() const;

		private:
			using Clock = std::chrono::high_resolution_clock;

			std::chrono::time_point<Clock> m_StartTime;
			std::chrono::time_point<Clock> m_LastTime;
			std::chrono::time_point<Clock> m_PauseTime;

			double m_DeltaTime;
			bool m_Paused;
		};
	}
}