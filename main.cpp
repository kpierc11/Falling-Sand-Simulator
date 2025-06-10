
#include <iostream>
#include "SandSimulator.h"

bool mouseDown = false;
bool mouseUp = true;


int main(int argc, char* argv[])
{
	bool done = false;

	SandSimulator sandSimulation;

	sandSimulation.InitSandGrid();


	while (!done) {

		//Set initial draw color
		SDL_SetRenderDrawColor(sandSimulation.GetRenderer(), 0, 0, 0, 255);

		//Clear back buffer
		SDL_RenderClear(sandSimulation.GetRenderer());


		SDL_SetRenderDrawColor(sandSimulation.GetRenderer(), 0, 0, 255, 255);



		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				done = true;
			}

			if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
			{
				mouseDown = true;
			}
			if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
			{
				mouseDown = false;
			}
		}



		sandSimulation.DrawGrid();
		sandSimulation.UpdateGrid();


		//Set the grid value where the mouse is clicked to 1. 
		if (mouseDown)
		{
			//Set grid values
			float mouseX = 0.0f;
			float mouseY = 0.0f;
			SDL_GetMouseState(&mouseX, &mouseY);

			int gridY = static_cast<int>(mouseY / sandSimulation.GetSandSize());
			int gridX = static_cast<int>(mouseX / sandSimulation.GetSandSize());

			for (auto& sandParticle : sandSimulation.mGrid)
			{

				if (sandParticle.rect.x / sandSimulation.GetSandSize() == gridX && sandParticle.rect.y / sandSimulation.GetSandSize() == gridY && !sandParticle.isShowing)
				{
					sandParticle.isShowing = 1;
				}

			}

		}

		// Show everything on screen
		SDL_RenderPresent(sandSimulation.GetRenderer());

	}

	// Close and destroy the window
	SDL_DestroyWindow(sandSimulation.GetWindow());

	// Clean up
	SDL_Quit();

	return 0;
}
