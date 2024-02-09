#pragma once
#include "Pong.h"

class Paddle : public Pong
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