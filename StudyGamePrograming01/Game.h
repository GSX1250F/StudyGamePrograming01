#pragma once
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "SDL.h"

struct Vector2
{
	float x;
	float y;
};

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();
private:
	void ProcessInput();	// 1.���͂��󂯕t����
	void UpdateGame();		// 2.�Q�[���������E�X�V����
	void GenerateOutput();	// 3.��ʂ≹�Ȃǂ��o�͂���

	SDL_Window* mWindow;	// SDL�ł�����E�B���h�E
	SDL_Renderer* mRenderer;	// 2D�`��p�����_��
	Uint32 mTicksCount;		// �Q�[���J�n������̌o�ߎ���
	bool mIsRunning;		// �Q�[�����s�����ۂ�
	
	// Game specific
	int mPaddleDir;		// �p�h���̓�������B+���������A-��������B
	Vector2 mPaddlePos;		// �p�h���̈ʒu�i2�����x�N�g���`���j
	Vector2 mBallPos;		// �{�[���̈ʒu�i2�����x�N�g���`���j
	Vector2 mBallVel;		// �{�[���̑��x�i2�����x�N�g���`���j
	const int thickness = 15;	// �ǁE�{�[���E�p�h���̌���
	const float paddleH = 150.0f;	// �p�h���̍���
	const int mWindowW = 1024;	//�E�B���h�E�̉���
	const int mWindowH = 768;	//�E�B���h�E�̏c��
	SDL_Texture* paddleImage;	// �p�h���̃e�N�X�`��
	SDL_Texture* gameOverText;		// �Q�[���I�[�o�[�̃e�L�X�g�\���p

};