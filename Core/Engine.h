#pragma once
#include "Renderer.h"
#include <vector>
#include <functional>

namespace GameEngine {
	class Engine
	{
	private:
		Renderer* m_Renderer;

		std::vector<std::function<void()>> m_GameLoops;
	public:
		Engine(Renderer* renderer) : m_Renderer(renderer) {}

		void Init();
	};
}
