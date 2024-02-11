#include "Paddle.h"
#include "GameVer2.h"

Paddle::Paddle(float x, float y) 
{
	pos_x = x;
	pos_y = y;
	dir_y = 0;
}

float Paddle::getPosX()
{
	return pos_x;
}

float Paddle::getPosY()
{
	return pos_y;
}


void Paddle::moveUp()
{
	dir_y = -1;
}

void Paddle::moveDown()
{
	dir_y = 1;
}

void Paddle::moveStop()
{
	dir_y = 0;
}

void Paddle::render(SDL_Renderer* renderer)
{
	// パドル作成
	SDL_Rect paddle0{
		static_cast<int>(this->pos_x - PADDLE_W / 2.0f),
		static_cast<int>(this->pos_y - PADDLE_H / 2.0f),
		PADDLE_W,
		static_cast<int>(PADDLE_H)
	};
	SDL_RenderFillRect(renderer, &paddle0);
}

void Paddle::update(float deltaTime)
{
	if (dir_y != 0)
	{
		pos_y += dir_y * PADDLE_SPEED * deltaTime;
		// パドルが画面から出ないようにする
		if (pos_y < (PADDLE_H / 2.0f + WALL_W))
		{
			pos_y = PADDLE_H / 2.0f + WALL_W;
		}
		else if (pos_y > (WIN_H - PADDLE_H / 2.0f - WALL_W))
		{
			pos_y = WIN_H - PADDLE_H / 2.0f - WALL_W;
		}
	}
}
