#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>
#include <chrono>
#include <random>


struct SandParticle
{
	SDL_FRect rect{};
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;
	bool isShowing;
	bool isWater;
};


class SandSimulator {

public:

	SandSimulator();

	~SandSimulator();

	std::vector<SandParticle>mGrid;

	bool InitSandGrid();
	void SimulationLoop();

	SDL_Renderer* GetRenderer() {
		return mRenderer;
	}
	SDL_Window* GetWindow() {
		return mWindow;
	}
	int GetColumns() const {
		return mColumns;
	}
	int GetRows() const {
		return mRows;
	}

	float GetSandSize() const {
		return mSandSize;
	}

private:
	void DrawGrid();
	void RenderParticles();
	void UpdateParticlePositions();
	void HandleInput();
	void ShiftParticleDown(int index);
	void ShiftParticleLeftOrRight(int index);
	void ShiftWaterParticle(int index);


private:
	float mSandSize;
	int mScreenWidth;
	int mScreenHeight;
	int mRows;
	int mColumns;
	SDL_Window* mWindow;
	bool mDone = false;
	SDL_Renderer* mRenderer;
	int mRandomNum;
	bool mMouseDown;
	std::mt19937 mRng;
	std::uniform_int_distribution<> mDistrib;
	SDL_FRect mMouseArea;
	int mMouseAreaSize; 

};

