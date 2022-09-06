#include <iostream>

#define STB_IMAGE_IMPLEMENTATION

#include "Engine.h"
#include "Timer.h"
#include "Debug.h"

using namespace GameEngine;

Renderer renderer(RendererInformation(800, 600, "Game Engine"));

class Test1 : public UILayer {
	void Start() override {
		title = "Something else";
	}

	void Update() override {
		if (!active) return;

		ImGui::Text("Hello");
	}
};

void Loop() {
	glClearColor(0.2, 0.2, 0.25, 1.0);
}

int main()
{
	Engine engine(&renderer);

	UILayer* test1 = new Test1();
	engine.AddLayer(test1);
	engine.AddGameLoopCallback(Loop);
	engine.StartGameLoop();
}