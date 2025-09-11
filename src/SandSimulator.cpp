#include "SandSimulator.hpp"
#include "cmath"

SandSimulator::SandSimulator() : mSandSize(8),
								 mScreenWidth(1280),
								 mScreenHeight(1000),
								 mRows(static_cast<int>(mScreenHeight / mSandSize)),
								 mColumns(static_cast<int>(mScreenWidth / mSandSize)),
								 mWindow(nullptr),
								 mDone(false),
								 mRenderer(nullptr),
								 mRandomNum(0),
								 mMouseDown(false),
								 mRng(std::mt19937(std::random_device{}())),
								 mDistrib(std::uniform_int_distribution<>(1, 6)),
								 mMouseArea({}),
								 mMouseAreaSize(20)
{

	for (int i = 0; i < mRows; i++)
	{
		for (int j = 0; j < mColumns; j++)
		{

			mRandomNum = mDistrib(mRng);

			SandParticle particle;
			particle.rect.h = mSandSize;
			particle.rect.w = mSandSize;
			particle.rect.x = static_cast<float>(j * mSandSize);
			particle.rect.y = static_cast<float>(i * mSandSize);

			switch (mRandomNum)
			{
			case 1:
				particle.r = 150;
				particle.g = 114;
				particle.b = 22;
				particle.a = 255;
				break;
			case 2:
				particle.r = 161;
				particle.g = 130;
				particle.b = 50;
				particle.a = 255;
				break;
			case 3:
				particle.r = 177;
				particle.g = 153;
				particle.b = 87;
				particle.a = 255;
				break;
			case 4:
				particle.r = 192;
				particle.g = 173;
				particle.b = 121;
				particle.a = 255;
				break;
			case 5:
				particle.r = 205;
				particle.g = 190;
				particle.b = 144;
				particle.a = 255;
				break;
			case 6:
				particle.r = 228;
				particle.g = 214;
				particle.b = 172;
				particle.a = 255;
				break;
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
		SDL_WINDOW_OPENGL);

	// Check that the window was successfully created
	if (mWindow == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
		return 0;
	}

	mRenderer = SDL_CreateRenderer(mWindow, NULL);

	// Check that the window was successfully created
	if (mRenderer == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create renderer: %s\n", SDL_GetError());
		return 0;
	}

	return 1;
}

void SandSimulator::SimulationLoop()
{

	while (!mDone)
	{
		// Set initial draw color
		SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);

		// Clear back buffer
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

	// Render Grid
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
		}
	}
}

void SandSimulator::HandleInput()
{

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_QUIT)
		{
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

	// Set the grid value where the mouse is clicked to 1.
	if (mMouseDown)
	{
		// Set grid values
		float mouseX = 0.0f;
		float mouseY = 0.0f;
		SDL_GetMouseState(&mouseX, &mouseY);

		const int gridX = static_cast<int>(mouseX / GetSandSize());
		const int gridY = static_cast<int>(mouseY / GetSandSize());

		mMouseArea.h = static_cast<float>(mMouseAreaSize);
		mMouseArea.w = static_cast<float>(mMouseAreaSize);
		mMouseArea.x = mouseX - (mMouseArea.h / 2);
		mMouseArea.y = mouseY - (mMouseArea.w / 2);

		// int mouseAreaGridX = static_cast<int>(mMouseArea.x / GetSandSize());
		// int mouseAreaGridY = static_cast<int>(mMouseArea.y / GetSandSize());

		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
		SDL_RenderRect(mRenderer, &mMouseArea);

		for (auto &sandParticle : mGrid)
		{

			float particleX = sandParticle.rect.x / GetSandSize();
			float particleY = sandParticle.rect.y / GetSandSize();

			if (particleX == gridX && particleY == gridY && !sandParticle.isShowing)
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
	// shift left
	if (mRandomNum == 1 && mGrid[(index + mColumns) - 1].isShowing == 0)
	{
		mGrid[index].isShowing = 0;
		mGrid[index + mColumns - 1].isShowing = 1;
	}

	// shift right
	if (mRandomNum == 2 && mGrid[(index + mColumns) + 1].isShowing == 0)
	{
		mGrid[index].isShowing = 0;
		mGrid[index + mColumns + 1].isShowing = 1;
	}
}
