
#include <iostream>
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <thread>
#include <chrono>
#include <random>


Uint64 currentFrameTime = SDL_GetTicks();
Uint64 previousFrameTime = currentFrameTime;

bool mouseDown = false;
bool mouseUp = true;

constexpr float CELLSIZE = 6;
constexpr float SCREENWIDTH = 600;
constexpr float SCREENHEIGHT = 600;
constexpr int ROWS = SCREENHEIGHT / CELLSIZE;
constexpr int COLUMNS = SCREENWIDTH / CELLSIZE;

int grid[ROWS][COLUMNS] = {};

int main(int argc, char* argv[])
{
	SDL_Window* window;
	bool done = false;
	SDL_Renderer* renderer;

	// Initialize SDL3
	SDL_Init(SDL_INIT_VIDEO);

	// Create an application window with the following settings:
	window = SDL_CreateWindow(
		"An SDL3 window",
		SCREENWIDTH,
		SCREENHEIGHT,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);

	// Check that the window was successfully created
	if (window == NULL) {

		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	renderer = SDL_CreateRenderer(window, NULL);

	// Check that the window was successfully created
	if (renderer == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create renderer: %s\n", SDL_GetError());
		return 1;
	}


	std::vector<SDL_FRect> rects;


	//Create default grid
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			grid[i][j] = 0;
		}
	}

	while (!done) {
		SDL_Event event;

		/*	currentFrameTime = SDL_GetTicks();

			double deltaTime = static_cast<double>(currentFrameTime - previousFrameTime) / SDL_GetPerformanceFrequency();

			previousFrameTime = currentFrameTime;*/


			// Seed the random number generator
		std::random_device rd;
		std::mt19937 gen(rd());

		// Define the range
		std::uniform_int_distribution<> distrib(1, 2);

		int randomNum = distrib(gen);


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
			if (event.type == SDL_EVENT_WINDOW_RESIZED)
			{
				int* width{};
				int* height{};
				SDL_GetWindowSize(window, width, height);


			}
		}

		//Set the grid value where the mouse is clicked to 1. 
		if (mouseDown)
		{
			//Set grid values
			float mouseX = 0.0f;
			float mouseY = 0.0f;
			SDL_GetMouseState(&mouseX, &mouseY);

			int gridY = static_cast<int>(mouseY / CELLSIZE);
			int gridX = static_cast<int>(mouseX / CELLSIZE);

			if (gridY > 0 && gridX < COLUMNS && gridX > 0)
			{
				int mouseGridPos = grid[gridY][gridX];

				if (mouseGridPos == 0)
				{
					grid[static_cast<int>(mouseY / CELLSIZE)][static_cast<int>(mouseX / CELLSIZE)] = randomNum;
				}
			}

		}

		//Render inital grid
		for (int i = 0; i < ROWS; i++) {
			for (int j = 0; j < COLUMNS; j++) {

				//Grid layout with blank rects
				/*SDL_FRect rect{};
				rect.h = CELLSIZE;
				rect.w = CELLSIZE;
				rect.x = j * CELLSIZE;
				rect.y = i * CELLSIZE;

				SDL_SetRenderDrawColor(renderer, 46, 134, 193, 255);
				SDL_RenderRect(renderer, &rect);*/


				//Render each cell that should have a sand particle. 
				if (grid[i][j] > 0)
				{

					SDL_FRect rect{};
					rect.h = CELLSIZE;
					rect.w = CELLSIZE;
					rect.x = j * CELLSIZE;
					rect.y = i * CELLSIZE;


					if (grid[i][j] == 1)
					{
						SDL_SetRenderDrawColor(renderer, 194, 178, 128, 255);
					}

					if (grid[i][j] == 2)
					{
						SDL_SetRenderDrawColor(renderer, 240, 230, 140, 255);
					}

					SDL_RenderFillRect(renderer, &rect);

				}
			}

		}


		//Activate gravity on cells that have a 1 value. 
		for (int i = ROWS - 1; i > 0; i--) {
			for (int j = COLUMNS - 1; j > 0; j--) {

				//Have each sand particle move to the cell below to simulate gravity.  
				if (grid[i][j] > 0 && i + 1 != COLUMNS)
				{
					if (grid[i + 1][j] == 0 && i + 1 < COLUMNS)
					{
						int temp = grid[i][j];
						grid[i][j] = grid[i + 1][j];
						grid[i + 1][j] = temp;
					}
					else if (grid[i + 1][j] > 0)
					{
						//shift left
						if (randomNum == 1 && grid[i + 1][j - 1] == 0)
						{
							int temp = grid[i][j];
							grid[i][j] = grid[i + 1][j - 1];
							grid[i + 1][j - 1] = temp;
						}

						//shift right
						if (randomNum == 2 && grid[i + 1][j + 1] == 0)
						{
							int temp = grid[i][j];
							grid[i][j] = grid[i + 1][j + 1];
							grid[i + 1][j + 1] = temp;

						}

					}

				}
			}

		}

		// Show everything on screen
		SDL_RenderPresent(renderer);

	}

	// Close and destroy the window
	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();
	return 0;
}
