module;
#include <SDL3/SDL.h>
export module Particle;

struct Color
{
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;
};

export class Particle
{
    SDL_FRect rect{};
    Color color;
    bool isShowing{false};
    void ShiftParticleDown(int index);
    void ShiftParticleLeftOrRight(int index);
};