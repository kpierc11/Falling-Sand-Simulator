module;
#include <SDL3/SDL.h>
#include <cmath>
export module Particle;
import std;

std::random_device rd;

std::mt19937 gen(rd());

std::uniform_int_distribution<int> distrib(1, 2);

int gasCounter = 0;

export enum class Type : std::uint8_t {
    Sand,
    Water,
    Bubbles
};

export struct Color
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
};

export const Color sandColors[] = {
    {101, 67, 33, 255},   // #654321 - Deep Wet Sand
    {140, 98, 57, 255},   // #8C6239 - Golden Dune
    {174, 132, 82, 255},  // #AE8452 - Desert Tan
    {205, 170, 125, 255}, // #CDAA7D - Coastal Beige
    {229, 204, 163, 255}, // #E5CCA3 - Sunlit Sand
    {245, 232, 196, 255}  // #F5E8C4 - Seashell Cream
};

export const Color waterColors[] = {
    // {3, 48, 94, 255},     // #03045e
    // {2, 62, 138, 255},    // #023e8a
    {0, 119, 182, 255}, // #0077b6
    // {0, 150, 199, 255},   // #0096c7
    // {0, 180, 216, 255},   // #00b4d8
    // {72, 202, 228, 255},  // #48cae4
    // {144, 224, 239, 255}, // #90e0ef
    // {173, 232, 244, 255}, // #ade8f4
    // {202, 240, 248, 255}  // #caf0f8
};

export const Color bubbleColors[] = {
    // {3, 48, 94, 255},     // #03045e
    // {2, 62, 138, 255},    // #023e8a
    //{0, 119, 182, 255},   // #0077b6
    // {0, 150, 199, 255},   // #0096c7
    // {0, 180, 216, 255},   // #00b4d8
    // {72, 202, 228, 255},  // #48cae4
    // {144, 224, 239, 255}, // #90e0ef
    {173, 232, 244, 255}, // #ade8f4
    // {202, 240, 248, 255}  // #caf0f8
};

export class Particle
{
public:
    Particle();
    ~Particle();

    SDL_FRect mRect{};
    Color mColor;
    Type mType;
    bool mIsShowing;
    void Update(std::vector<Particle> &mGrid, int i, int columns, int rows);
    void MoveSand(int index, std::vector<Particle> &mGrid, int columns);
    void MoveWater(int index, std::vector<Particle> &mGrid, int columns);
    void MoveBubbles(int index, std::vector<Particle> &mGrid, int columns);
};

Particle::Particle() : mRect({}), mColor({}), mType(Type::Sand), mIsShowing(false)
{
}

Particle::~Particle()
{
}

void Particle::Update(std::vector<Particle> &mGrid, int i, int columns, int rows)
{

    if (mGrid[i].mIsShowing && i + columns < columns * rows - 1 && i - columns > 0)
    {
        if (mGrid[i].mType == Type::Sand)
        {
            MoveSand(i, mGrid, columns);
        }

        if (mGrid[i].mType == Type::Water)
        {

            MoveWater(i, mGrid, columns);
        }

        if (mGrid[i].mType == Type::Bubbles)
        {
           MoveBubbles(i, mGrid, columns);
        }
    }
}

void Particle::MoveSand(int index, std::vector<Particle> &mGrid, int columns)
{

    if (mGrid[index + columns].mIsShowing == 0)
    {
        mGrid[index].mIsShowing = 0;
        mGrid[index + columns].mIsShowing = 1;
        mGrid[index + columns].mType = mGrid[index].mType;
        mGrid[index + columns].mColor = mGrid[index].mColor;
    }
    else if (mGrid[index + columns].mIsShowing)
    {
        // shift left
        if (distrib(gen) == 1 && mGrid[(index + columns) - 1].mIsShowing == 0)
        {
            mGrid[index].mIsShowing = 0;
            mGrid[index + columns - 1].mIsShowing = 1;
            mGrid[index + columns - 1].mType = mGrid[index].mType;
            mGrid[index + columns - 1].mColor = mGrid[index].mColor;
        }

        // shift right
        if (distrib(gen) == 2 && mGrid[(index + columns) + 1].mIsShowing == 0)
        {
            mGrid[index].mIsShowing = 0;
            mGrid[index + columns + 1].mIsShowing = 1;
            mGrid[index + columns + 1].mType = mGrid[index].mType;
            mGrid[index + columns + 1].mColor = mGrid[index].mColor;
        }
    }
}

void Particle::MoveWater(int index, std::vector<Particle> &mGrid, int columns)
{

    if (mGrid[index + columns].mIsShowing == 0)
    {
        mGrid[index].mIsShowing = 0;
        mGrid[index + columns].mIsShowing = 1;
        mGrid[index + columns].mType = mGrid[index].mType;
        mGrid[index + columns].mColor = mGrid[index].mColor;
    }
    else
    {
        if (distrib(gen) == 1 && mGrid[(index + columns) - 1].mIsShowing == 0)
        {
            mGrid[index].mIsShowing = 0;
            mGrid[index + columns - 1].mIsShowing = 1;
            mGrid[index + columns - 1].mType = mGrid[index].mType;
            mGrid[index + columns - 1].mColor = mGrid[index].mColor;
        }

        // shift down right
        else if (distrib(gen) == 2 && mGrid[(index + columns) + 1].mIsShowing == 0)
        {
            mGrid[index].mIsShowing = 0;
            mGrid[index + columns + 1].mIsShowing = 1;
            mGrid[index + columns + 1].mType = mGrid[index].mType;
            mGrid[index + columns + 1].mColor = mGrid[index].mColor;
        }

        // Shiftleft
        else if (distrib(gen) == 1 && mGrid[(index - 1)].mIsShowing == 0)
        {
            mGrid[index].mIsShowing = 0;
            mGrid[index - 1].mIsShowing = 1;
            mGrid[index - 1].mType = mGrid[index].mType;
            mGrid[index - 1].mColor = mGrid[index].mColor;
        }

        // shift right
        else if (distrib(gen) == 2 && mGrid[(index + 1)].mIsShowing == 0)
        {
            mGrid[index].mIsShowing = 0;
            mGrid[index + 1].mIsShowing = 1;
            mGrid[index + 1].mType = mGrid[index].mType;
            mGrid[index + 1].mColor = mGrid[index].mColor;
        }
    }
}

void Particle::MoveBubbles(int index, std::vector<Particle> &mGrid, int columns)
{

    gasCounter = index;
    if (gasCounter == columns)
    {
        gasCounter = 0;
    }

    if (mGrid[index].mIsShowing == 0)
    {
        mGrid[index].mIsShowing = 0;
        mGrid[columns - gasCounter].mIsShowing = 1;
        mGrid[columns - gasCounter].mType = mGrid[index].mType;
        mGrid[columns - gasCounter].mColor = mGrid[index].mColor;
    }
    // else
    // {
    //     if (distrib(gen) == 1 && mGrid[(index - columns) - 1].mIsShowing == 0)
    //     {
    //         mGrid[index].mIsShowing = 0;
    //         mGrid[index - columns - 1].mIsShowing = 1;
    //         mGrid[index - columns - 1].mType = mGrid[index].mType;
    //         mGrid[index - columns - 1].mColor = mGrid[index].mColor;
    //     }

    //     // shift down right
    //     else if (distrib(gen) == 2 && mGrid[(index - columns) + 1].mIsShowing == 0)
    //     {
    //         mGrid[index].mIsShowing = 0;
    //         mGrid[index - columns + 1].mIsShowing = 1;
    //         mGrid[index - columns + 1].mType = mGrid[index].mType;
    //         mGrid[index - columns + 1].mColor = mGrid[index].mColor;
    //     }

    //     // Shiftleft
    //     else if (distrib(gen) == 1 && mGrid[(index - 1)].mIsShowing == 0)
    //     {
    //         mGrid[index].mIsShowing = 0;
    //         mGrid[index - 1].mIsShowing = 1;
    //         mGrid[index - 1].mType = mGrid[index].mType;
    //         mGrid[index - 1].mColor = mGrid[index].mColor;
    //     }

    //     // shift right
    //     else if (distrib(gen) == 2 && mGrid[(index + 1)].mIsShowing == 0)
    //     {
    //         mGrid[index].mIsShowing = 0;
    //         mGrid[index + 1].mIsShowing = 1;
    //         mGrid[index + 1].mType = mGrid[index].mType;
    //         mGrid[index + 1].mColor = mGrid[index].mColor;
    //     }
    // }
    gasCounter++;
}