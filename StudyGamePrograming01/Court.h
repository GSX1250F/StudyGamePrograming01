#pragma once
#include "SDL.h"
#include "Pong.h"


class Court : public Pong
{
private:
	SDL_Rect upperWall;
	SDL_Rect lowerWall;
	SDL_Rect rightWall;
	SDL_Rect net[14];
public:
	Court();
	void render(SDL_Renderer*);
};

