#include "SandSimulator.h"

SandSimulator::SandSimulator()
{

	mSandSize = 6;
	mScreenWidth = 1000;
	mScreenHeight = 1000;
	mRows = mScreenHeight / mSandSize;
	mColumns = mScreenWidth / mSandSize;
	mWindow = NULL;
	mRenderer = NULL;
	mRandomNum = 0;


	for (int i = 0; i < mRows; i++) {
		for (int j = 0; j < mColumns; j++) {
			// Seed the random number generator
			std::random_device rd;
			std::mt19937 gen(rd());

			// Define the range
			std::uniform_int_distribution<> distrib(1, 4);

			int randomNum = distrib(gen);

			SandParticle particle;
			particle.rect.h = mSandSize;
			particle.rect.w = mSandSize;
			particle.rect.x = j * mSandSize;
			particle.rect.y = i * mSandSize;



			if (randomNum == 1)
			{
				particle.r = 125;
				particle.g = 102;
				particle.b = 8;
				particle.a = 255;
			}

			if (randomNum == 2)
			{
				particle.r = 154;
				particle.g = 125;
				particle.b = 10;
				particle.a = 255;
			}

			if (randomNum == 3)
			{
				particle.r = 183;
				particle.g = 149;
				particle.b = 11;
				particle.a = 255;
			}

			if (randomNum == 4)
			{
				particle.r = 212;
				particle.g = 172;
				particle.b = 13;
				particle.a = 255;
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
		"An SDL3 window",
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

void SandSimulator::DrawGrid()
{
	// Seed the random number generator
	std::random_device rd;
	std::mt19937 gen(rd());

	// Define the range
	std::uniform_int_distribution<> distrib(1, 4);

	mRandomNum = distrib(gen);

	//Render Grid 
	for (auto sandParticle = mGrid.begin(); sandParticle != mGrid.end(); ++sandParticle)
	{
		if (sandParticle->isShowing && sandParticle != mGrid.end())
		{

			SDL_SetRenderDrawColor(mRenderer, sandParticle->r, sandParticle->g, sandParticle->b, sandParticle->a);
			SDL_RenderFillRect(mRenderer, &sandParticle->rect);
		}


	}
}

void SandSimulator::UpdateGrid()
{

	for (int i = mColumns * mRows - 1; i > 0; i--)
	{
		if (mGrid[i].isShowing && i + mRows < mColumns * mRows - 1)
		{
			if (mGrid[i + mRows].isShowing == 0)
			{
				mGrid[i].isShowing = 0;
				mGrid[i + mRows].isShowing = 1;

			}
			else if (mGrid[i + mRows].isShowing)
			{
				//shift left
				if (mRandomNum == 1 && mGrid[(i + mRows) - 1].isShowing == 0)
				{
					mGrid[i].isShowing = 0;
					mGrid[i + mRows - 1].isShowing = 1;
				}

				//shift right
				if (mRandomNum == 2 && mGrid[(i + mRows) + 1].isShowing == 0)
				{
					mGrid[i].isShowing = 0;
					mGrid[i + mRows + 1].isShowing = 1;
				}
			}
		}

	}

}
