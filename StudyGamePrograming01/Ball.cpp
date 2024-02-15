#include "Ball.h"
#include "GameVer2.h"
#include <random>

Ball::Ball()
{
	init();
}

void Ball::init()
{
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_real_distribution<float> distr(1, 11);
	float iniv = distr(eng);
	pos_x = BALL_INIT_X;
	pos_y = BALL_INIT_Y;
	vel_x = (float)cos(iniv / 12.0 * M_PI);
	vel_y = (float)sin(iniv / 12.0 * M_PI);
}

void Ball::update(Paddle* leftPaddle, Paddle* rightPaddle, Pong* pong, float deltaTime, std::vector<Ball>::iterator it,bool& mIsRunning,int PADDLES_NUM)
{
	float x1 = pos_x;
	float y1 = pos_y;
	//�Փ˂��Ȃ��ꍇ��deltaTime��̈ʒu
	float x2 = this->pos_x + this->vel_x * deltaTime * BALL_SPEED;
	float y2 = this->pos_y + this->vel_y * deltaTime * BALL_SPEED;

	// ��ǂł̒��˕Ԃ�
	if (y2 <= WALL_W && this->vel_y < 0.0f)
	{
		y2 = WALL_W - (y2 - WALL_W);
		this->vel_y *= -1;
	}
	// ���ǂł̒��˕Ԃ�
	else if (y2 >= (WIN_H - WALL_W) &&
		this->vel_y > 0.0f)
	{
		y2 = WIN_H - WALL_W - (y2 - (WIN_H - WALL_W));
		this->vel_y *= -1;
	}

	// �E�ǂł̒��˕Ԃ�
	else if (PADDLES_NUM == 1 &&
		x2 >= (WIN_W - WALL_W) &&
		this->vel_x > 0.0f)
	{
		x2 = WIN_W - WALL_W - (x2 - (WIN_W - WALL_W));
		this->vel_x *= -1;
	}


	//�p�h���ł̒��˕Ԃ�
	Paddle* paddle = (this->vel_x < 0) ? leftPaddle : rightPaddle;
	InterceptPoint point{ 0,0,0 };
	//�ڐG����
	if (this->vel_x > 0)point = intercept(x1, y1, x2, y2, paddle->getPosX() - PADDLE_W / 2.0f, paddle->getPosY() + PADDLE_H / 2.0f, paddle->getPosX() - PADDLE_W / 2.0f, paddle->getPosY() - PADDLE_H / 2.0f, 1);
	else if (this->vel_x < 0)point = intercept(x1, y1, x2, y2, paddle->getPosX() + PADDLE_W / 2.0f, paddle->getPosY() + PADDLE_H / 2.0f, paddle->getPosX() + PADDLE_W / 2.0f, paddle->getPosY() - PADDLE_H / 2.0f, 2);
	if (point.side == 0) {
		if (this->vel_y > 0)point = intercept(x1, y1, x2, y2, paddle->getPosX() - PADDLE_W / 2.0f, paddle->getPosY() - PADDLE_H / 2.0f, paddle->getPosX() + PADDLE_W / 2.0f, paddle->getPosY() - PADDLE_H / 2.0f, 3);
		else if (this->vel_y < 0)point = intercept(x1, y1, x2, y2, paddle->getPosX() - PADDLE_W / 2.0f, paddle->getPosY() + PADDLE_H / 2.0f, paddle->getPosX() + PADDLE_W / 2.0f, paddle->getPosY() + PADDLE_H / 2.0f, 4);
	}
	//�ڐG�ʒu�ɂ���Ē��˕Ԃ�
	if (point.side != 0) {
		switch (point.side) {
		case 1:
		case 2:
			this->pos_x = point.x;
			this->vel_x *= -1.0f;
			this->vel_x *= 1.1f;
			break;
		case 3:
		case 4:
			this->pos_y = point.y;
			this->vel_y *= -1.0f;
			break;
		}
	}
	else {
		//���˂Ȃ��ꍇ
		this->pos_x = x2;
		this->pos_y = y2;
	}
	//��ʊO�ɂ���ꍇ
	if (this->pos_x <= 0.0f)
	{
		mIsRunning = false;
	}
	else if (this->pos_x >= WIN_W)
	{
		mIsRunning = false;
	}

}

InterceptPoint Ball::intercept(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int side)
{
	//�x�N�g���̊O�ς��g�p���������蔻��
	float denom = ((y4 - y3) * (x2 - x1)) - ((x4 - x3) * (y2 - y1));
	if (denom != 0) {
		float ua = (((x4 - x3) * (y1 - y3)) - ((y4 - y3) * (x1 - x3))) / denom;
		if ((ua >= 0) && (ua <= 1)) {
			float ub = (((x2 - x1) * (y1 - y3)) - ((y2 - y1) * (x1 - x3))) / denom;
			if ((ub >= 0) && (ub <= 1)) {
				float x = x1 + (ua * (x2 - x1));
				float y = y1 + (ua * (y2 - y1));
				return { x,y,side };
			}
		}
	}
	return { 0,0,0 };
}



void Ball::render(SDL_Renderer* renderer)
{
	//�{�[����`��
	SDL_Rect ball0{
		static_cast<int>(this->pos_x - BALL_W / 2),
		static_cast<int>(this->pos_y - BALL_W / 2),
		BALL_W,
		BALL_W
	};
	SDL_RenderFillRect(renderer, &ball0);
}