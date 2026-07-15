
module;
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>
#include <chrono>
#include <random>
#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"
#include "cmath"

export module SandSimulator;

import Particle;

bool show_demo_window = true;
bool show_another_window = true;
bool water = false;
bool filledSand = true;
bool hollowSquares = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

export class SandSimulator
{

public:
	SandSimulator();

	~SandSimulator();

	bool InitSandGrid();
	void SimulationLoop();
	void EndSimulation();

	SDL_Renderer *GetRenderer()
	{
		return mRenderer;
	}
	SDL_Window *GetWindow()
	{
		return mWindow;
	}
	int GetColumns() const
	{
		return mColumns;
	}
	int GetRows() const
	{
		return mRows;
	}

	int GetSandSize() const
	{
		return mSandSize;
	}

	bool isSimulationOver() const { return mDone; }

private:
	void HandleInput();
	void UpdateParticles();
	void Render();
	void RebuildGrid();

private:
	Uint64 mCurrentFrameTime;

	int mScreenWidth;
	int mScreenHeight;
	int mRows;
	int mColumns;
	int mMouseAreaSize;
	int mRandomNum;
	int mSandSize;

	bool mMouseDown;
	bool mDone = false;

	SDL_Window *mWindow;
	SDL_Renderer *mRenderer;
	SDL_FRect mMouseArea;

	std::vector<Particle> mGrid;
	std::mt19937 mRng;
	std::uniform_int_distribution<> mDistrib;

	ImGuiIO mIO;
};

SandSimulator::SandSimulator() : mCurrentFrameTime(SDL_GetTicks()),
								 mScreenWidth(1000),
								 mScreenHeight(800),
								 mRows(0),
								 mColumns(0),
								 mMouseAreaSize(20),
								 mRandomNum(0),
								 mSandSize(8),
								 mMouseDown(false),
								 mDone(false),
								 mWindow(nullptr),
								 mRenderer(nullptr),
								 mMouseArea({}),
								 mGrid({}),
								 mRng(std::mt19937(std::random_device{}())),
								 mDistrib(std::uniform_int_distribution<>(0, 5)),
								 mIO()
{
}

SandSimulator::~SandSimulator()
{
}

bool SandSimulator::InitSandGrid()
{

	// Initialize SDL3
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	// float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());

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

	// Setup sand grid
	mRows = mScreenHeight / mSandSize;
	mColumns = mScreenWidth / mSandSize;

	mGrid.reserve(mRows * mColumns);

	for (int i = 0; i < mRows; i++)
	{
		for (int j = 0; j < mColumns; j++)
		{
			Particle particle;

			particle.mRect = {
				static_cast<float>(j * mSandSize),
				static_cast<float>(i * mSandSize),
				static_cast<float>(mSandSize),
				static_cast<float>(mSandSize)};

			particle.mIsShowing = false;

			mGrid.push_back(particle);
		}
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	mIO = ImGui::GetIO();
	(void)mIO;
	mIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	mIO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// ImGui::StyleColorsLight();

	// Setup scaling
	ImGuiStyle &style = ImGui::GetStyle();
	// style.ScaleAllSizes(main_scale); // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
	// style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

	// Setup Platform/Renderer backends
	ImGui_ImplSDL3_InitForSDLRenderer(mWindow, mRenderer);
	ImGui_ImplSDLRenderer3_Init(mRenderer);

	return 1;
}

void SandSimulator::SimulationLoop()
{
	// Start the Dear ImGui frame
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	HandleInput();
	UpdateParticles();

	static int counter = 0;

	ImGui::Begin("Sand Simulator Settings");

	ImGui::Text("Can change the color and particle style");
	ImGui::Checkbox("Sand", &filledSand);
	ImGui::Checkbox("Water", &water);
	ImGui::Checkbox("Hollow Squares", &hollowSquares);

	static int prevSandSize = mSandSize;
	ImGui::SliderInt("Sand Size", &mSandSize, 1, 20);
	if (mSandSize != prevSandSize)
	{
		RebuildGrid();
		prevSandSize = mSandSize;
	} // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::ColorEdit3("clear color", (float *)&clear_color);

	ImGui::End();

	Render();
}

void SandSimulator::HandleInput()
{

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL3_ProcessEvent(&event);

		if (mIO.WantCaptureMouse)
		{
			break;
		}

		if (event.type == SDL_EVENT_QUIT)
		{
			mDone = true;
		}

		if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (!mIO.WantCaptureMouse)
			{
				mMouseDown = true;
			};
		}
		if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
		{
			if (!mIO.WantCaptureMouse)
			{
				mMouseDown = false;
			}
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

		if (event.type == SDL_EVENT_WINDOW_RESIZED)
		{

			int newWidth = event.window.data1;
			int newHeight = event.window.data2;
		}
	}

	if (!mIO.WantCaptureMouse && mMouseDown)
	{
		// Set grid values
		float mouseX = 0.0f;
		float mouseY = 0.0f;
		SDL_GetMouseState(&mouseX, &mouseY);

		const int gridX = static_cast<int>(mouseX / GetSandSize());
		const int gridY = static_cast<int>(mouseY / GetSandSize());

		if (gridX >= 0 && gridX < mColumns &&
			gridY >= 0 && gridY < mRows)
		{
			int index = gridY * mColumns + gridX;
			Particle &sandParticle = mGrid[index];

			if (!sandParticle.mIsShowing && filledSand)
			{
				sandParticle.mType = Type::Sand;
				sandParticle.mIsShowing = true;
				sandParticle.mColor = sandColors[mDistrib(mRng)];
			}

			if (!sandParticle.mIsShowing && water)
			{
				sandParticle.mType = Type::Water;
				sandParticle.mIsShowing = true;
				sandParticle.mColor = waterColors[0];
			}
		}
	}
}

void SandSimulator::UpdateParticles()
{
	for (int i = mColumns * mRows - 1; i > 0; i--)
	{
		mGrid[i].Update(mGrid, i, mColumns, mRows);
	}
}

void SandSimulator::Render()
{
	// Set initial draw color
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);

	// Clear back buffer
	SDL_RenderClear(mRenderer);

	mRows = mScreenHeight / mSandSize;
	mColumns = mScreenWidth / mSandSize;

	// Render Grid
	for (Particle &sandParticle : mGrid)
	{
		if (!sandParticle.mIsShowing)
			continue;

		SDL_SetRenderDrawColor(mRenderer, sandParticle.mColor.r, sandParticle.mColor.g, sandParticle.mColor.b, sandParticle.mColor.a);
		SDL_RenderFillRect(mRenderer, &sandParticle.mRect);
	}

	// IM Gui Render
	ImGui::Render();
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), mRenderer);

	// Show everything on screen
	SDL_RenderPresent(mRenderer);
}

void SandSimulator::RebuildGrid()
{
	mRows = mScreenHeight / mSandSize;
	mColumns = mScreenWidth / mSandSize;

	mGrid.clear();

	for (int i = 0; i < mRows; i++)
	{
		for (int j = 0; j < mColumns; j++)
		{
			Particle particle;

			particle.mRect = {
				static_cast<float>(j * mSandSize),
				static_cast<float>(i * mSandSize),
				static_cast<float>(mSandSize),
				static_cast<float>(mSandSize)};

			particle.mIsShowing = false;

			mGrid.push_back(particle);
		}
	}
}

void SandSimulator::EndSimulation()
{

	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}