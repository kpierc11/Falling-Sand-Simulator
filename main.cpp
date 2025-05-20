
#include <iostream>
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>


Uint64 currentFrameTime = SDL_GetTicks();
Uint64 previousFrameTime = currentFrameTime;

bool mouseDown = false;
bool mouseUp = true;


static void checkCollision(std::vector<SDL_FRect>* vector, SDL_FRect currentSquare)
{

};

int main(int argc, char* argv[])
{
	SDL_Window* window;                    // Declare a pointer
	bool done = false;
	SDL_Renderer* renderer;

	SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL3

	// Create an application window with the following settings:
	window = SDL_CreateWindow(
		"An SDL3 window",                  // window title
		1500,                               // width, in pixels
		600,                               // height, in pixels
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE           // flags - see below
	);

	// Check that the window was successfully created
	if (window == NULL) {
		// In the case that the window could not be made...
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	renderer = SDL_CreateRenderer(window, NULL);




	// Check that the window was successfully created
	if (renderer == NULL) {
		// In the case that the window could not be made...
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create renderer: %s\n", SDL_GetError());
		return 1;
	}


	std::vector<SDL_FRect> rects;


	/*for (int i = 0; i < 100; i++)
	{
		SDL_FRect rect;
		rect.h = 20;
		rect.w = 20;
		rect.x = 50;
		rect.y = 50;
		rects.push_back(rect);
	}*/

	while (!done) {
		SDL_Event event;

		currentFrameTime = SDL_GetTicks();

		double deltaTime = static_cast<double>(currentFrameTime - previousFrameTime) / SDL_GetPerformanceFrequency();

		previousFrameTime = currentFrameTime;

		//std::cout << "Delta time (ms): " << deltaTime * 1000.0 << std::endl;


		//std::cout << timePassedBetweenFrame << std::endl;


		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);


		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		//SDL_RenderLine(renderer, 1.0f, 1.0f, 1400.0f, 1);

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



		if (mouseDown)
		{
			float currentX = 0.0f;
			float currentY = 0.0f;
			SDL_GetMouseState(&currentX, &currentY);

			//std::cout << currentX << currentY << std::endl;

			SDL_FRect rect{};
			rect.h = 15;
			rect.w = 15;
			rect.x = currentX;
			rect.y = currentY;

			rects.push_back(rect);
		}




		for (int i = 0; i < rects.size(); i++)
		{

			float currentX = 0.0f;
			float currentY = 0.0f;
			SDL_GetMouseState(&currentX, &currentY);


			//Check and make sure the sand doesn't go past the bottom of the screen
			if (rects[i].y < 600 - rects[i].h)
			{
				rects[i].y + 20.0f;
				rects[i].y += .5f;	
			}


			// Draw a green rectangle
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			SDL_RenderRect(renderer, &rects[i]);
		}


		// Show everything on screen
		SDL_RenderPresent(renderer);


		/*SDL_RenderRect(renderer, &rect);
		rect.x += 1.0f;*/

	}

	// Close and destroy the window
	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();
	return 0;
}




void checkCollision() {



}
