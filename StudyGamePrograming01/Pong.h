#pragma once
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <math.h>
#include "SDL.h"
#include "Ball.h"
#include "Paddle.h"
#include "Court.h"
#include "GameVer2.h"

using namespace std;


class Ball;			//�q�N���X�Ƃ��ă{�[�����쐬
class Paddle;		//�q�N���X�Ƃ��ăp�h�����쐬
class Court;		//�q�N���X�Ƃ��ăR�[�g���쐬
class Game;

class Pong : public Game
{
protected:
	//Game�N���X >�p��> Pong >�p��> �e�����v���O����
//�Q�[���Ɏg�p����萔�Q
	int scene = 1;				// �Q�[���V�[��
	const int WALL_W = 12;		// �ǂ̌���
	const int PADDLE_W = 12;	// �p�h���̌���
	const int PADDLE_H = 150;		// �p�h���̒���
	const float PADDLE_SPEED = 300.0f;	// �p�h���̃X�s�[�h
	const float L_PADDLE_POS = 20.0f;		// ���p�h�������ʒu
	const float R_PADDLE_POS = WIN_W - 20.0f;		// �E�p�h�������ʒu
	const int BALL_W = 12;		// �{�[������
	const float BALL_SPEED = 300.0f;		// �{�[���̃X�s�[�h
	const float BALL_INIT_X = WIN_W / 2;		// �{�[���̏����ʒux
	const float BALL_INIT_Y = WIN_H / 2;		// �{�[���̏����ʒuy
	float BALL_INIT_VX;		// �{�[���̏����X�s�[�hx
	float BALL_INIT_VY;		// �{�[���̏����X�s�[�hy
	int PADDLES_NUM;		// �p�h���̐�
	int BALLS_NUM;			// �{�[���̐�
	// �Ȃ���E�E�E
	// float PREDICTION_WAITTIME 0.1f;
	// int WARD_W = 50;
	// int WARD_WG = 5;
	// int WARD_HG = 20;


public:
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

