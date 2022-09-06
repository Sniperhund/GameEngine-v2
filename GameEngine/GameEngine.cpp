#include <iostream>

#define STB_IMAGE_IMPLEMENTATION

#include "Engine.h"
#include "Timer.h"
#include "Debug.h"

using namespace GameEngine;

Renderer renderer(RendererInformation(800, 600, "Game Engine"));

void Loop() {
	glClearColor(0.2, 0.2, 0.25, 1.0);
}

int main()
{
	Engine engine(&renderer);

	engine.AddGameLoopCallback(Loop);
	engine.StartGameLoop();
}