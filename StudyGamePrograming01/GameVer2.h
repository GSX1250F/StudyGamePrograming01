#pragma once
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <math.h>
#include <iostream>//�f�o�b�N�Ƃ���
#include "SDL.h"
#include "Ball.h"
#include "Paddle.h"
#include "Pong.h"

#define WIN_W 1024
#define WIN_H 768
#define WALL_W 12
#define PADDLE_W 12
#define PADDLE_H 60
#define PADDLE_SPEED 150.0f
#define L_PADDLE_POS 24.0f
#define R_PADDLE_POS WIN_W - 24.0f

#define BALL_W 12
#define BALL_SPEED 300.0f
#define BALL_INIT_X WIN_W
#define BALL_INIT_Y WIN_H/2
#define BALL_INIT_VX cos(2.0/3.0*M_PI)
#define BALL_INIT_VY sin(2.0/3.0*M_PI)

//#define PREDICTION_WAITTIME 0.1f
//#define WARD_W 50
//#define WARD_WG 5
//#define WARD_HG 20

class Game
{
public:
	Game();
	bool Initialize();			//�Q�[��������������
	void RunLoop();				//�Q�[���I�[�o�[�܂ŃQ�[�����[�v�����s����
	void Shutdown();			//�Q�[�����V���b�g�_�E������

private:
	//�Q�[�����[�v�̂��߂̃w���p�[�֐��Q
	void ProcessInput();		//���͂��󂯕t����
	void UpdateGame();			//�Q�[������������
	void GenerateOutput();		//�X�V����B

	SDL_Window* mWindow;		//SDL�����E�B���h�E
	SDL_Renderer* mRenderer;	//2D�`��̂��߂̃����_�����O
	Uint32 mTicksCount;			//�Q�[������
	bool mIsRunning;			//�Q�[���̑��s���x������

	Pong* pong;					//�Q�[�����e���N���X��

};