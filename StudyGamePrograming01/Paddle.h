#pragma once
#include "SDL.h"
#include "Ball.h"
class Ball;

class Paddle
{
private:
	float pos_x, pos_y;
	float dir_y;
public:
	Paddle(float x, float y);
	float getPosX();
	float getPosY();
	void moveUp();
	void moveDown();
	void moveStop();
	void render(SDL_Renderer*);
	void update(float);
};