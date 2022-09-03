#include <iostream>

#include "Engine.h"

using namespace GameEngine;

void Loop() {
	glClearColor(0.2, 0.2, 0.25, 1.0);
}

int main()
{
	Renderer renderer(RendererInformation(800, 600, "Game Engine"));
	Engine engine(&renderer);

	engine.AddGameLoopCallback(Loop);
	engine.StartGameLoop();
}