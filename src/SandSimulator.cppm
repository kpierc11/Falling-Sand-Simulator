
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


bool show_demo_window = true;
bool show_another_window = true;
bool points = false;
bool filledSand = true;
bool hollowSquares = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

struct Color
{
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;
};

struct Particle
{
	SDL_FRect rect{};
	Color color;
	bool isShowing{false};
	void ShiftParticleDown(int index);
	void ShiftParticleLeftOrRight(int index);
};

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
	void ShiftParticleDown(int index);
	void ShiftParticleLeftOrRight(int index);

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
	std::vector<Particle> mParticles;
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
								 mParticles({}),
								 mRng(std::mt19937(std::random_device{}())),
								 mDistrib(std::uniform_int_distribution<>(1, 6)),
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
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

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

	static const Color sandColors[] = {
		{150, 114, 22, 255},
		{161, 130, 50, 255},
		{177, 153, 87, 255},
		{192, 173, 121, 255},
		{205, 190, 144, 255},
		{228, 214, 172, 255}};

	mGrid.reserve(mRows * mColumns);

	for (int i = 0; i < mRows; i++)
	{
		for (int j = 0; j < mColumns; j++)
		{
			Particle particle;

			particle.rect = {
				static_cast<float>(j * mSandSize),
				static_cast<float>(i * mSandSize),
				static_cast<float>(mSandSize),
				static_cast<float>(mSandSize)};

			int colorIndex = mDistrib(mRng) - 1;
			particle.color = sandColors[colorIndex];

			particle.color = sandColors[colorIndex];

			particle.isShowing = false;

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

	// mCurrentFrameTime = SDL_GetTicks();

	// float deltaTime = (mCurrentFrameTime - mPreviousFrameTime) / 1000.0f;
	// mPreviousFrameTime = mCurrentFrameTime;

	// Start the Dear ImGui frame
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	HandleInput();
	UpdateParticles();

	static int counter = 0;

	ImGui::Begin("Sand Simulator Settings");

	ImGui::Text("Can change the color and particle style"); 
	ImGui::Checkbox("Filled Sand", &filledSand);
	ImGui::Checkbox("Points", &points);
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

			if (!sandParticle.isShowing)
			{
				sandParticle.isShowing = true;
			}
		}
	}
}

void SandSimulator::UpdateParticles()
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
		if (!sandParticle.isShowing)
			continue;

		SDL_SetRenderDrawColor(mRenderer, sandParticle.color.r, sandParticle.color.g, sandParticle.color.b, sandParticle.color.a);
		// SDL_SetRenderDrawColor(mRenderer, static_cast<Uint8>(clear_color.x * 255.0f), static_cast<Uint8>(clear_color.y * 255.0f), static_cast<Uint8>(clear_color.z * 255.0f), static_cast<Uint8>(clear_color.w * 255.0f));

		if (filledSand)
		{
			SDL_RenderFillRect(mRenderer, &sandParticle.rect);
		}
		else if (hollowSquares)
		{
			SDL_RenderRect(mRenderer, &sandParticle.rect);
		}
		else if (points)
		{
			SDL_RenderPoint(mRenderer, sandParticle.rect.x, sandParticle.rect.y);
		}
		else
		{
			SDL_RenderFillRect(mRenderer, &sandParticle.rect);
		}
	}

	// IM Gui Render
	ImGui::Render();
	// SDL_SetRenderScale(mRenderer, mIO.DisplayFramebufferScale.x, mIO.DisplayFramebufferScale.y);
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), mRenderer);

	// Show everything on screen
	SDL_RenderPresent(mRenderer);
}

void SandSimulator::RebuildGrid()
{
	mRows = mScreenHeight / mSandSize;
	mColumns = mScreenWidth / mSandSize;

	mGrid.clear();

	static const Color sandColors[] = {
		{150, 114, 22, 255},
		{161, 130, 50, 255},
		{177, 153, 87, 255},
		{192, 173, 121, 255},
		{205, 190, 144, 255},
		{228, 214, 172, 255}};

	for (int i = 0; i < mRows; i++)
	{
		for (int j = 0; j < mColumns; j++)
		{
			Particle particle;

			particle.rect = {
				static_cast<float>(j * mSandSize),
				static_cast<float>(i * mSandSize),
				static_cast<float>(mSandSize),
				static_cast<float>(mSandSize)};

			int colorIndex = mDistrib(mRng) - 1;
			particle.color = sandColors[colorIndex];
			particle.isShowing = false;

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
	if (mDistrib(mRng) == 1 && mGrid[(index + mColumns) - 1].isShowing == 0)
	{
		mGrid[index].isShowing = 0;
		mGrid[index + mColumns - 1].isShowing = 1;
	}

	// shift right
	if (mDistrib(mRng) == 2 && mGrid[(index + mColumns) + 1].isShowing == 0)
	{
		mGrid[index].isShowing = 0;
		mGrid[index + mColumns + 1].isShowing = 1;
	}
}