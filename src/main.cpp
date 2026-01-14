#include <emscripten.h>
#include "SandSimulator.hpp"

SandSimulator sandSimulation;

static void mainloop(void) /* this will run often, possibly at the monitor's refresh rate */
{

	if (sandSimulation.isSimulationOver())
	{
		sandSimulation.EndSimulation();
#ifdef __EMSCRIPTEN__
		emscripten_cancel_main_loop(); /* this should "kill" the app. */
#else
		exit(0);
#endif
	}

	sandSimulation.SimulationLoop();
}

int main()
{
	bool simulationInitialized = sandSimulation.InitSandGrid();
	if (simulationInitialized)
	{
#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop(mainloop, 0, 1);
#else
		mainloop();
#endif
	}
}
