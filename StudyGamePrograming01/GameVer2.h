#pragma once
#include <cstdio>
#include <cstdlib>
#include <iostream>//�f�o�b�N�Ƃ���
#include "SDL.h"
#include "Pong.h"
using namespace std;


class Game
{
public:
	Game();
	virtual ~Game();
	bool Initialize();			//�Q�[��������������
	void RunLoop();				//�Q�[���I�[�o�[�܂ŃQ�[�����[�v�����s����
	void Shutdown();			//�Q�[�����V���b�g�_�E������

protected:
	const int WIN_W = 1024;		// �Q�[����ʉ���
	const int WIN_H = 768;		// �Q�[����ʏc��

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