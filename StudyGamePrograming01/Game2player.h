// 2�v���C���[��2�{�[��
// �ʒu�Ƒ��x��2��Vector2���܂�Ball�\���̂����B Ball{ Vector2 �ʒu , Vector2 ���x }
// �����̃{�[���̂��߂ɁAstd::vector<Ball>�^�̃����o�[�ϐ��z���Game�ɒǉ�����B
// �܂�{�[����vector�z��́@{ {�ʒu.x , �ʒu.y } , { ���x.x , ���x.y} } �ƂȂ�B
// Game::Initialize�̃R�[�h��ύX���āA�����̃{�[���̈ʒu�Ƒ��x������������B
// Game::UpdateGame�ŁA�{�[���X�V�̃R�[�h��ύX����B
// mBallVel��mBallPos���g������ɁAstd::vector�����[�v���āA���ׂẴ{�[�����X�V����B
// �p�h���ɂ���
// std::vector<int>�^�Ńp�h���̕����̔z������B { �p�h��0�̕��� , �p�h��1�̕��� }
// std::vector<Vector2>�^�Ńp�h���̈ʒu�̔z������B{ { �p�h��0.x , �p�h��0.y } , { �p�h��1.x , �p�h��1.y }  }

//�v���C���[����I�𐧂ɂ���ꍇ�A�^�C�g����ʂƃv���C��ʂ��\�z�B
// scene�ϐ���p�ӂ��A0�̂Ƃ��̓^�C�g���A1�̂Ƃ��̓v���C��ʂƂ���B


#pragma once
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "SDL.h"

// Vector2 struct just stores x/y coordinates
// (for now)
// struct�̓f�t�H���g�̃A�N�Z�V�r���e�B�� public
// class�̓f�t�H���g�̃A�N�Z�V�r���e�B�� private
struct Vector2
{
public: //��struct�����炠���Ă������Ă�����͓����B
	float x;
	float y;
};

struct Ball		// Ball�\���́BVector2�^�̈ʒu�Ƒ��x�����B
{
	Vector2 pos;
	Vector2 vel;
};

// Game class
class Game
{
public:
	// �R���X�g���N�^
	// Initialize the game
	Game();
	// Runs the game loop until the game is over
	bool Initialize();
	void RunLoop();
	// Shutdown the game
	void Shutdown();
private:
	// Helper functions for the game loop
	void ProcessInput();	// 1.���͂��󂯕t����
	void UpdateGame();		// 2.�Q�[������������
	void GenerateOutput();	// 3.�X�V

	// Window created by SDL
	SDL_Window* mWindow;
	// Renderer for 2D drawing
	SDL_Renderer* mRenderer;
	// Number of ticks since start of game
	Uint32 mTicksCount;
	// Game should continue to run
	bool mIsRunning;

	// Pong specific
	std::vector<int> mPaddlesDir;	//�p�h���̕����z��B�p�h��0�ƃp�h��1�̕���������
	std::vector<Vector2> mPaddlesPos;	//�p�h���̈ʒu��Vector2�^�z��B�p�h��0��x,y �ƃp�h��1��x,y
	std::vector<Ball> mBalls;		//�{�[���^�̔z��B�{�[���̈ʒux,y�Ƒ��xx,y
	int mBallsNum;	//�{�[���̐�
	int mPaddlesNum;	//�p�h���̐�
	float paddleH;	//�p�h���̒���
	float thickness;	//�ǁE�ʁE�p�h���̌���
	float mBallsAcc;	//�{�[���̉����x

	// �^�C�g����ʂ��v���C��ʂ��̕ϐ�
	int scene;

	//�����@�{�[����Ball�\���̂ɂ���Ȃ�A�p�h����Paddle�\���̂ɂ������������H
	// struct Paddle { int dir , Vector2�@pos } �Ƃ��āA std::vector<Paddle> mPaddles�@�Ƃ���B
	// mPaddleNum �� 1��2�őI���ɂ��āAPaddle�\���̂� ���̐��������΂����̂ł́B
	// 
};
