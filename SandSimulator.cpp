#include "SandSimulator.h"

SandSimulator::SandSimulator() :
	mSandSize(6),
	mScreenWidth(1280),
	mScreenHeight(1000),
	mRows(mScreenHeight / mSandSize),
	mColumns(mScreenWidth / mSandSize),
	mWindow(nullptr),
	mRenderer(nullptr),
	mRandomNum(0),
	mDone(false),
	mMouseDown(false),
	mRng(std::mt19937(std::random_device{}())),
	mDistrib(std::uniform_int_distribution<>(1, 4)),
	mMouseArea({}),
	mMouseAreaSize(20)
{

	for (int i = 0; i < mRows; i++) {
		for (int j = 0; j < mColumns; j++) {


			mRandomNum = mDistrib(mRng);


			SandParticle particle;
			particle.rect.h = mSandSize;
			particle.rect.w = mSandSize;
			particle.rect.x = j * mSandSize;
			particle.rect.y = i * mSandSize;

			// Mark index 200 as water
			if (i > mRows - 100)
			{
				particle.isWater = 1;

			}

			// Color water particles
			if (particle.isWater == 1)
			{
				particle.r = 26;
				particle.g = 67;
				particle.b = 96;
				particle.a = 255;
				switch (mRandomNum)
				{
					case 1:
					particle.r = 26;
					particle.g = 67;
					particle.b = 96;
					particle.a = 255;
					break;
					case 2:
					particle.r = 41;
					particle.g = 128;
					particle.b = 185;
					particle.a = 255;
					break;
					case 3:
					particle.r = 41;
					particle.g = 128;
					particle.b = 185;
					particle.a = 255;
					break;
					case 4:
					particle.r = 26;
					particle.g = 67;
					particle.b = 96;
					particle.a = 255;
					break;
				}

			}
			else {
				switch (mRandomNum)
				{
					case 1:
					particle.r = 125;
					particle.g = 102;
					particle.b = 8;
					particle.a = 255;
					break;
					case 2:
					particle.r = 154;
					particle.g = 125;
					particle.b = 10;
					particle.a = 255;
					break;
					case 3:
					particle.r = 183;
					particle.g = 149;
					particle.b = 11;
					particle.a = 255;
					break;
					case 4:
					particle.r = 212;
					particle.g = 172;
					particle.b = 13;
					particle.a = 255;
					break;
				}

			}

			particle.isShowing = 0;

			mGrid.push_back(particle);
		}
	}

}


SandSimulator::~SandSimulator()
{


}

bool SandSimulator::InitSandGrid()
{

	// Initialize SDL3
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	// Create an application window with the following settings:
	mWindow = SDL_CreateWindow(
		"Sand Simulator",
		mScreenWidth,
		mScreenHeight,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);

	// Check that the window was successfully created
	if (mWindow == NULL) {

		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	mRenderer = SDL_CreateRenderer(mWindow, NULL);

	// Check that the window was successfully created
	if (mRenderer == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create renderer: %s\n", SDL_GetError());
		return 1;
	}
}

void SandSimulator::SimulationLoop()
{

	while (!mDone)
	{
		//Set initial draw color
		SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);

		//Clear back buffer
		SDL_RenderClear(mRenderer);

		SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);


		HandleInput();
		DrawGrid();

		// Show everything on screen
		SDL_RenderPresent(GetRenderer());
	}


}

void SandSimulator::DrawGrid()
{

	RenderParticles();
	UpdateParticlePositions();

}

void SandSimulator::RenderParticles()
{
	mRandomNum = mDistrib(mRng);

	//Render Grid 
	for (auto sandParticle = mGrid.begin(); sandParticle != mGrid.end(); ++sandParticle)
	{
		if (sandParticle->isShowing)
		{
			// Render the particle
			SDL_SetRenderDrawColor(mRenderer, sandParticle->r, sandParticle->g, sandParticle->b, sandParticle->a);
			SDL_RenderFillRect(mRenderer, &sandParticle->rect);
		}
	}

}


void SandSimulator::UpdateParticlePositions()
{
	for (int i = mColumns * mRows - 1; i > 0; i--)
	{
		if (mGrid[i].isShowing && i + mColumns < mColumns * mRows - 1)
		{
			if (mGrid[i + mColumns].isShowing == 0)
			{
				ShiftParticleDown(i);
			}
			else if (mGrid[i + mColumns].isShowing)
			{
				ShiftParticleLeftOrRight(i);
			}

			if (mGrid[i].isWater == 1)
			{
				ShiftWaterParticle(i);
			}

		}

	}

}

void SandSimulator::HandleInput()
{

	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			mDone = true;
		}

		if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
		{
			mMouseDown = true;
		}
		if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
		{
			mMouseDown = false;
		}
		if (event.type == SDL_EVENT_MOUSE_WHEEL)
		{

			std::cout << "Mouse Scrolled bitch. " << std::endl;
			std::cout << event.wheel.y << std::endl;
			if (event.wheel.y == 1)
			{

				mMouseAreaSize -= 20;

			}
			else if (event.wheel.y == -1)
			{

				mMouseAreaSize += 20;
			}

		}
	}

	//Set the grid value where the mouse is clicked to 1. 
	if (mMouseDown)
	{
		//Set grid values
		float mouseX = 0.0f;
		float mouseY = 0.0f;
		SDL_GetMouseState(&mouseX, &mouseY);

		const int gridX = static_cast<int>(mouseX / GetSandSize());
		const int gridY = static_cast<int>(mouseY / GetSandSize());

		std::cout << "Mouse X: " << gridX << std::endl;
		std::cout << "Mouse Y:" << gridY << std::endl;

		mMouseArea.h = mMouseAreaSize;
		mMouseArea.w = mMouseAreaSize;
		mMouseArea.x = mouseX - (mMouseArea.h / 2);
		mMouseArea.y = mouseY - (mMouseArea.w / 2);

		std::cout << "Mouse Area X: " << mMouseArea.x << std::endl;
		std::cout << "Mouse Area Y:" << mMouseArea.y << std::endl;

		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
		SDL_RenderRect(mRenderer, &mMouseArea);

		for (auto& sandParticle : mGrid)
		{

			float particleX = sandParticle.rect.x / GetSandSize();
			float particleY = sandParticle.rect.y / GetSandSize();

			if (particleX == mMouseArea.x && particleY == mMouseArea.y && !sandParticle.isShowing)
			{
				sandParticle.isShowing = 1;
			}

		}

	}
}

void SandSimulator::ShiftParticleDown(int index)
{
	if (mGrid[index + mColumns].isShowing == 0)
	{
		mGrid[index].isShowing = 0;
		mGrid[index + mColumns].isShowing = 1;
	}
}

void SandSimulator::ShiftParticleLeftOrRight(int index)
{
	//shift left
	if (mRandomNum == 1 && mGrid[(index + mColumns) - 1].isShowing == 0)
	{
		mGrid[index].isShowing = 0;
		mGrid[index + mColumns - 1].isShowing = 1;
	}

	//shift right
	if (mRandomNum == 2 && mGrid[(index + mColumns) + 1].isShowing == 0)
	{
		mGrid[index].isShowing = 0;
		mGrid[index + mColumns + 1].isShowing = 1;
	}
}


void SandSimulator::ShiftWaterParticle(int index)
{
	//shift right and up and down
	//if (mGrid[(index + mColumns) + 1].isShowing == 0)
	//{
	/*mGrid[index].isShowing = 0;
	mGrid[index + mColumns - 1].isShowing = 1;*/

	/*mGrid[index].isShowing = 0;
	mGrid[index + 1].isShowing = 1;*/

	//}
}
