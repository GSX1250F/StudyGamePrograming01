#pragma once
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <math.h>
#include "SDL.h"
#include "Ball.h"
#include "Paddle.h"
#include "Court.h"
using namespace std;


class Ball;			//���ݎQ��
class Paddle;		//���ݎQ��

class Pong
{
public:
//�Q�[���Ɏg�p����萔�Q
	int scene = 1;				// �Q�[���V�[��
	int PADDLES_NUM;		// �p�h���̐�
	int BALLS_NUM;			// �{�[���̐�
	// �Ȃ���E�E�E
	// float PREDICTION_WAITTIME 0.1f;
	// int WARD_W = 50;
	// int WARD_WG = 5;
	// int WARD_HG = 20;

	Pong();
	~Pong();
	void render(SDL_Renderer*);			// �����_���[
	void init();						// ������
	void update(float);;					// ���� (������ deltatime)
	void input(const Uint8*);			// ���͎�t (������ const Uint8* state = SDL_GetKeyboardState(NULL) �̓��e
	
	//�R�[�g
	Court* court;
	//�o�h��
	Paddle* L_Paddle;
	Paddle* R_Paddle;
	//�{�[��
	vector<Ball> mBalls;


};

