#pragma once

#include "World.h"
class Simulation
{
public:
	Simulation(bool fullscreen);
	~Simulation();
	bool initRenderer(bool fullacreen);

	void setup();
	void generateWorld();
	Camera* camera;

	void updateRender();

	const Renderer* getRenderer() { return renderer; };
	bool running = true;
	void toggleFullscreen() { 
		renderer->toggleFullscreen(); 
		};
	void manageInputs();
	void update();
	void updateLogic();
	World* world;
private:

	Renderer* renderer;

};

