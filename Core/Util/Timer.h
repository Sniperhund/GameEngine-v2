#pragma once
#include "../Util/Debug.h"
#include <string>

#include <chrono>

namespace GameEngine {
	class Timer {
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	public:
		Timer() {
			Reset();
		}

		void Reset() {
			m_Start = std::chrono::high_resolution_clock::now();
		}

		float Elapsed() {
			return std::chrono::duration_cast<std::chrono::nanoseconds>
				(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
		}

		float ElapsedMillis() {
			return Elapsed() * 1000.0f;
		}
	};

	class ScopedTimer {
	private:
		std::string m_Name;
		Timer m_Timer;
	public:
		ScopedTimer(const std::string& name) : m_Name(name) {}
		~ScopedTimer() {
			float time = m_Timer.ElapsedMillis();
			DebugLog("[TIMER] " << m_Name << " - " << time << "ms");
		}
	};
}