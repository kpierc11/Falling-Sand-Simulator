
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

constexpr float CELLSIZE = 5;
constexpr float SCREENWIDTH = 1500;
constexpr float SCREENHEIGHT = 600;
constexpr int ROWS = SCREENHEIGHT / CELLSIZE;
constexpr int COLUMNS = SCREENWIDTH / CELLSIZE;

int grid[ROWS][COLUMNS] = {};


static void checkCollision(std::vector<SDL_FRect>* vector, SDL_FRect currentSquare)
{

};

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

	grid[20][0] = 1;

	while (!done) {
		SDL_Event event;

		currentFrameTime = SDL_GetTicks();

		double deltaTime = static_cast<double>(currentFrameTime - previousFrameTime) / SDL_GetPerformanceFrequency();

		previousFrameTime = currentFrameTime;

		//std::cout << "Delta time (ms): " << deltaTime * 1000.0 << std::endl;


		//std::cout << timePassedBetweenFrame << std::endl;

		// Seed the random number generator
		std::random_device rd;
		std::mt19937 gen(rd());

		// Define the range
		std::uniform_int_distribution<> distrib(1, 2);


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



		if (mouseDown)
		{
			//Set grid values

			float mouseX = 0.0f;
			float mouseY = 0.0f;
			SDL_GetMouseState(&mouseX, &mouseY);
			std::cout << "MouseX:" << mouseX / CELLSIZE << std::endl;
			std::cout << "MouseY:" << mouseY / CELLSIZE << std::endl;



			grid[static_cast<int>(mouseY / CELLSIZE)][static_cast<int>(mouseX / CELLSIZE)] = 1;
			grid[static_cast<int>((mouseY / CELLSIZE))][static_cast<int>((mouseX / CELLSIZE) + 1)] = 1;
			grid[static_cast<int>((mouseY / CELLSIZE))][static_cast<int>((mouseX / CELLSIZE) - 1)] = 1;
			grid[static_cast<int>((mouseY / CELLSIZE) + 1)][static_cast<int>((mouseX / CELLSIZE))] = 1;

		}

		float mouseX = 0.0f;
		float mouseY = 0.0f;
		SDL_GetMouseState(&mouseX, &mouseY);

		SDL_FRect rect{};
		rect.h = CELLSIZE;
		rect.w = CELLSIZE;
		rect.x = mouseX;
		rect.y = mouseY;
		SDL_SetRenderDrawColor(renderer, 205, 92, 92, 255);
		SDL_RenderFillRect(renderer, &rect);

		for (int i = 0; i < ROWS; i++) {
			for (int j = 0; j < COLUMNS; j++) {

				

				////Grid layout with blank rects
				//SDL_FRect rect{};
				//rect.h = CELLSIZE;
				//rect.w = CELLSIZE;
				//rect.x = j * CELLSIZE;
				//rect.y = i * CELLSIZE;

				//SDL_SetRenderDrawColor(renderer, 46, 134, 193, 255);
				//SDL_RenderRect(renderer, &rect);

				//Render each cell that should have a sand particle. 
				if (grid[i][j] == 1)
				{
					SDL_FRect rect{};
					rect.h = CELLSIZE;
					rect.w = CELLSIZE;
					rect.x = j * CELLSIZE;
					rect.y = i * CELLSIZE;

					if (grid[i + 1][j] == 0)
					{
						grid[i + 1][j] = 1;
						grid[i][j] = 0;
					}



					SDL_SetRenderDrawColor(renderer, 205, 92, 92, 255);
					SDL_RenderFillRect(renderer, &rect);

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




void checkCollision() {



}
