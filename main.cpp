
#include <iostream>
#include "SandSimulator.h"


int main(int argc, char* argv[])
{
	SandSimulator sandSimulation;

	bool simulationInitialized = sandSimulation.InitSandGrid();

	if(simulationInitialized)
	{
		sandSimulation.SimulationLoop();
	}

	// Close and destroy the window
	SDL_DestroyWindow(sandSimulation.GetWindow());


	// Clean up
	SDL_Quit();

	return 0;
}
