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
	Game();			// �R���X�g���N�^
	bool Initialize();		// ������
	void RunLoop();			// ���[�v���s
	void Shutdown();		// �V���b�g�_�E��
private:
	void ProcessInput();	// ���͎�t
	void UpdateGame();		// �Q�[���̏����ƍX�V
	void GenerateOutput();	// ��ʂ≹�Ȃǂ̏o��

	SDL_Window* mWindow;	// SDL�ł�����E�B���h�E
	SDL_Renderer* mRenderer;	// 2D�`��p�����_��
	Uint32 mTicksCount;		// �Q�[���J�n������̌o�ߎ���
	bool mIsRunning;		// �Q�[�����s�����ۂ�
	const int mWindowW = 1024;	//�E�B���h�E�̉���
	const int mWindowH = 768;	//�E�B���h�E�̏c��


	// Game specific
	void ResetGame();		// �Q�[���̊e�p�����[�^�����Z�b�g
	int mPaddleDir;		// �p�h���̓�������B+���������A-��������B
	float mPaddleSpeed;		// �p�h���̓��쑬�x
	Vector2 mPaddlePos;		// �p�h���̈ʒu�i2�����x�N�g���`���j
	Vector2 mBallPos;		// �{�[���̈ʒu�i2�����x�N�g���`���j
	Vector2 mBallVel;		// �{�[���̑��x�i2�����x�N�g���`���j
	const int thickness = 15;	// �ǁE�{�[���E�p�h���̌���
	const float paddleH = 150.0f;	// �p�h���̍���
	SDL_Texture* paddleImage;	// �p�h���̃e�N�X�`��
	int scene = 0;	//0:�Q�[���� , 1:�|�[�Y�� , 2:�Q�[���I�[�o�[
	bool pause = false;	//true:�|�[�Y��
	std::vector<SDL_Texture*> mText;		// �e�L�X�g�\���p
	std::vector<Vector2> mTextPos;		// �e�L�X�g�\���ʒu
	std::vector<Vector2> mTextSize;		// �e�L�X�g�T�C�Y
	const int mFontSize = 100;	//�e�L�X�g�̃t�H���g�T�C�Y
};