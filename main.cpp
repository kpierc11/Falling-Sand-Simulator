
#include <iostream>
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>


int main(int argc, char* argv[])
{
	SDL_Window* window;                    // Declare a pointer
	bool done = false;
	SDL_Renderer* renderer;

	SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL3

	// Create an application window with the following settings:
	window = SDL_CreateWindow(
		"An SDL3 window",                  // window title
		1440,                               // width, in pixels
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


	for(int i = 0; i < 100; i++)
	{
		SDL_FRect rect;
		rect.h = 20;
		rect.w = 20;
		rect.x = 50;
		rect.y = 50;
		rects.push_back(rect);
	}

	SDL_FRect rect;
	rect.h = 20;
	rect.w = 20;
	rect.x = 50;
	rect.y = 50;

	while (!done) {
		SDL_Event event;
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_RenderLine(renderer, 1.0f, 1.0f, 1400.00f, 1);
		SDL_RenderRect(renderer, &rect);

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				done = true;
			}

			if(event.type == SDL_EVENT_MOUSE_MOTION)
			{

				float currentX = 0.0f;
				float currentY = 0.0f;
				SDL_GetMouseState(&currentX, &currentY);

				std::cout << currentX << currentY << std::endl;
				
				/*for(auto square: rects)
				{

					square.
				}


				rect.x = currentX;
				rect.y = currentY;*/
			
			
			}
		}



		/*for (auto &square : rects)
		{
			square.y -= 5.0f;
			SDL_RenderRect(renderer, &square);
		}*/

		SDL_RenderRect(renderer, &rect);
		rect.x += 1.0f;

		SDL_RenderPresent(renderer);
	}

	// Close and destroy the window
	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();
	return 0;
}
