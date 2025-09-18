#include <iostream>
#include "SandSimulator.hpp"

int main()
{
	SandSimulator sandSimulation;

	bool simulationInitialized = sandSimulation.InitSandGrid();

	if (simulationInitialized)
	{
		sandSimulation.SimulationLoop();
	}

	sandSimulation.EndSimulation();

	return 0;
}
