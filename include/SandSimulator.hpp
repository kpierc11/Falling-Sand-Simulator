#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>
#include <chrono>
#include <random>
#include "imgui.h"

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

struct SandParticle
{
	
};

class SandSimulator
{

public:
	SandSimulator();

	~SandSimulator();

	std::vector<Particle> mGrid;

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

private:
	void HandleInput();
	void UpdateParticles();
	void Render();
	void ShiftParticleDown(int index);
	void ShiftParticleLeftOrRight(int index);

private:
	Uint64 mCurrentFrameTime;
	Uint64 mPreviousFrameTime;
	float mSandSize;
	int mScreenWidth;
	int mScreenHeight;
	int mRows;
	int mColumns;
	SDL_Window *mWindow;
	bool mDone = false;
	SDL_Renderer *mRenderer;
	int mRandomNum;
	bool mMouseDown;
	std::mt19937 mRng;
	std::uniform_int_distribution<> mDistrib;
	SDL_FRect mMouseArea;
	int mMouseAreaSize;
	int mAmountShowingOnGrid;
	ImGuiIO mIO;
};
