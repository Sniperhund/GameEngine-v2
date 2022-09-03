#include <iostream>

#include "Engine.h"

using namespace GameEngine;

int main()
{
	Renderer renderer(RendererInformation(800, 600, "Game Engine"));
	Engine engine(&renderer);

	while (1);
}