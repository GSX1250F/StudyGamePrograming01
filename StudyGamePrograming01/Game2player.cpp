#include "Game2player.h"
#include <iostream>
#include <random>
#include <math.h>

using namespace std;
// �ۑ�1.1�@�Q�l�̃v���C���[�o�[�W����
// �ۑ�1.2�@�Q�l���Q�{�[���̃o�[�W����
// �ۑ�1.3�@�v���C���[����I�𐧂ɂ���
// �ۑ�1.4�@�{�[������I�𐧂ɂ���
// �ۑ�1.5�@�{�[���̏��������������_���ɂ���B
// �ۑ�1.6�@�{�[�����p�h���Ńo�E���h���閈�ɁA�X�s�[�h�������������Ȃ�B�i��Փx�������A�b�v�j
// �ۑ�1.7�@�{�[��2�ȏ���\


Game::Game()
{
	mWindow = nullptr;
	mRenderer = nullptr;
	mTicksCount = 0;
	mIsRunning = true;
	scene = 0;	//0�̓^�C�g�����
	mPaddlesNum = 0;
	mBallsNum = 0;
	thickness = 15.0f;
	paddleH = 200.0f;
	mBallsAcc = 1.5f;

	while (mPaddlesNum == 0)
	{
		cout << "Number of players ? (1 or 2) : ";
		cin >> mPaddlesNum;
		static_cast<int>(mPaddlesNum);
		if (mPaddlesNum != 1 && mPaddlesNum != 2) 
		{
			cout << "incorrect number." << endl;
			mPaddlesNum = 0;
		}
	}
	while (mBallsNum <= 0)
	{
		cout << "Number of Balls ? (int) : ";
		cin >> mBallsNum;
		static_cast<int>(mBallsNum);
		if (mBallsNum <= 0)
		{
			cout << "incorrect number." << endl;
			mBallsNum = 0;
		}
	}
}
	

bool Game::Initialize()
{
	// Initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);	//�r�f�I�T�u�V�X�e���̏������B�Ԃ�l�͐����B�Ԃ�l��0�łȂ��Ƃ��͏��������s
	// SDL_INIT_AUDIO	:	�I�[�f�B�I�f�o�C�X�̊Ǘ��A�Đ��A�^��
	// SDL_INIT_HAPTIC	:	�t�H�[�X�t�B�[�h�o�b�N�i�U���Ȃǁj�̃T�u�V�X�e��
	// SDL_INIT_GAMECONTROLLER	:	�R���g���[�����̓f�o�C�X�̃T�u�V�X�e��

	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Create an SDL Window
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)", // Window title
		100,	// Top left x-coordinate of window
		100,	// Top left y-coordinate of window
		1024,	// Width of window
		768,	// Height of window
		0		// Flags (0 for no flags set)
	);
	// SDL_WINDOW_FULLSCREEN : �t���X�N���[�����[�h�g�p
	// SDL_WINDOW_FULSCREEN_DESKTOP : ���݂̃f�X�N�g�b�v�̉𑜓x�Ńt���X�N���[�����[�h���g�p
	// SDL_WINDOW_OPENGL : OpenGL�O���t�B�b�N�X���C�u�������g�p
	// SDL_WINDOW_RESIZABLE : ���[�U�[���E�B���h�E�̑傫����ς�����B

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	//// Create SDL renderer
	// ���X�^�[�O���t�B�b�N�X�F�f�B�X�v���C�̓s�N�Z����2�����i�q�ō\������Ă���B
	// �J���[�o�b�t�@�Ƃ���2�����z��ɉ�ʑS�̂̐F��񂪒u�����B
	// ���t���[���o�b�t�@�̓J���[�o�b�t�@�E�f�v�X�o�b�t�@�E�X�e���V���o�b�t�@�Ȃǂ̑g�����̑��́B

	// �_�u���o�b�t�@�Z�p
	// �J���[�o�b�t�@���Q�p�ӂ���B�Q�[���͂P�̃o�b�t�@�ɏ������݁A�f�B�X�v���C�͂����P�̃o�b�t�@��\������B
	// �f�B�X�v���C���\�����I����ƁA�Q�̃o�b�t�@�͌��������B
	// �f�B�X�v���C���\�����I���āA�o�b�t�@�����������T�C�N�����@vsync�i���������j�@�Ƃ����B
	// 2D�O���t�B�b�N�X�`��p�̊֐��Q���@SDL_Renderer�@�ŗ��p����B
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for
		-1,		 // Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		//�����_���[�̏������Ɏ��s�����Ƃ�
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	//�{�[���̈ʒu�E�����E������������
	double mBallsAngle = 0;		//�{�[���ˏo�p�x
	int mBallsDirx = -1;	//�{�[���ˏo��������x
	int mBallsDiry = -1;	//�{�[���ˏo��������y
	random_device rnd;		// �񌈒�I�ȗ���������𐶐�
	mt19937 mt(rnd());		//  �����Z���k�E�c�C�X�^��32�r�b�g�ŁA�����͏����V�[�h�l
	uniform_int_distribution<> rand(20, 61);	// �͈͎w���l�����@�ˏo�p�x
	uniform_int_distribution<> randx(0, 2);	// �͈͎w���l����
	uniform_int_distribution<> randy(0, 2);	// �͈͎w���l����
	for (int i = 0; i < mBallsNum; i++) {
		if (randx(mt) == 0) { mBallsDirx = -1; }
		else { mBallsDirx = 1; }
		if (randy(mt) == 0) { mBallsDiry = -1; }
		else { mBallsDiry = 1; }
		mBallsAngle = rand(mt) / 180.0f * M_PI;

		Ball ball_init = { {1024.0f / 2.0f , 768.0f * (i+1) / (mBallsNum+1) } , { static_cast<float>(mBallsDirx * 300.0f * cos(mBallsAngle)) , mBallsDiry * static_cast<float>(mBallsDiry * 300.0f * sin(mBallsAngle))} };	//�{�[���̏������
		//mBalls�z��Ƀ{�[����Vector2�z���push back�Œǉ�
		mBalls.push_back(ball_init);
	}
	//�p�h���̈ʒu�E������������
	mPaddlesPos.push_back({ 10.0f+thickness/2.0f , 768.0f / 2.0f });	//�p�h��0�̏����ʒu
	mPaddlesPos.push_back({ 1024.0f - 10.0f -thickness/2.0f , 768.0f / 2.0f });	//�p�h��1�̏����ʒu
	mPaddlesDir.push_back(0);	//�p�h��0�̏�������
	mPaddlesDir.push_back(0);	//�p�h��1�̏�������
	
	
	return true;	//������������true��Ԃ��B
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{	// SDL�̃C�x���g������̃L���[�ŊǗ�����B
	// �C�x���g�L���[�ł́A1�t���[���ɕ����̃C�x���g�������Ă���\��������̂ŁA
	// �L���[�ɂ��邷�ׂẴC�x���g�����[�v��������K�v������B
	SDL_Event event;
	while (SDL_PollEvent(&event))		// SDL�L���[�ɃC�x���g�������true
	{
		switch (event.type)
		{
			// If we get an SDL_QUIT event, end loop
		case SDL_QUIT:		// ���[�U�[���E�B���h�E����悤�Ƃ������͂̃C�x���g
			mIsRunning = false;		//�Q�[�����I������t���O
			break;
		}
	}

	// Get state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// If escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE])		//ESC�L�[�������ꂽ�Ƃ�
	{
		mIsRunning = false;		//�Q�[�����I������t���O
	}

	// �p�h��0�̕���
	mPaddlesDir[0] = 0;
	if (state[SDL_SCANCODE_Q])		//�@Q�L�[�ŏ����
	{
		mPaddlesDir[0] -= 1;
	}
	if (state[SDL_SCANCODE_A])		//�@A�L�[�ŉ�����
	{
		mPaddlesDir[0] += 1;
	}

	// �p�h��1�̕���
	mPaddlesDir[1] = 0;
	if (state[SDL_SCANCODE_UP])		//�@���L�[�ŏ����
	{
		mPaddlesDir[1] -= 1;
	}
	if (state[SDL_SCANCODE_DOWN])		//�@���L�[�ŏ����
	{
		mPaddlesDir[1] += 1;
	}
}

void Game::UpdateGame()
{
	//�Q�[�����Ԃ̍X�V�A�����Ȃ�	
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// Delta time is the difference in ticks from last frame
	// (converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// Clamp maximum delta time value
	if (deltaTime > 0.033f)
	{
		deltaTime = 0.033f;
	}

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();




	// �p�h���ʒu�̍X�V
	// Update paddle position based on direction
	for (int i = 0; i < mPaddlesNum; i++) 
	{
		if (mPaddlesDir[i] != 0)
		{
			mPaddlesPos[i].y += mPaddlesDir[i] * 300.0f * deltaTime;	//�p�h����Y���W���A300�s�N�Z��/�b��������
			// Make sure paddle doesn't move off screen!
			if (mPaddlesPos[i].y < (paddleH / 2.0f + thickness))
			{
				mPaddlesPos[i].y = paddleH / 2.0f + thickness;
			}
			else if (mPaddlesPos[i].y >(768.0f - paddleH / 2.0f - thickness))
			{
				mPaddlesPos[i].y = 768.0f - paddleH / 2.0f - thickness;
			}
		}

	}

	// �{�[���ʒu�̍X�V
	// Update ball position based on ball velocity
	for (int i = 0; i < mBallsNum; i++) 
	{
		mBalls[i].pos.x += mBalls[i].vel.x * deltaTime;
		mBalls[i].pos.y += mBalls[i].vel.y * deltaTime;
		
		// Did the ball go off the screen? (if so, end game)
		if (mBalls[i].pos.x <= 0.0f || mBalls[i].pos.x >= 1024.0f)
		{
			mIsRunning = false;
		}


		// �{�[���@�ǂł̒��˕Ԃ�@
		// Did the ball collide with the top or bottom wall?
		if ( ( (mBalls[i].pos.y <= thickness*1.5f) && (mBalls[i].vel.y < 0.0f) )
			|| ((mBalls[i].pos.y >= (768.0f - thickness*1.5f)) && (mBalls[i].vel.y > 0.0f)))
		{
			mBalls[i].vel.y *= -1.0f;
		}
		// Did the ball collide with the right wall ? (1player)
		if (mPaddlesNum == 1) 
		{
			if ( (mBalls[i].pos.x >= 1024.0f - thickness*1.5f) &&
				 (mBalls[i].vel.x > 0.0f))
			{
				mBalls[i].vel.x *= -1.0f;
				

			}
		}


		//�@�{�[���@�p�h���ł̒��˕Ԃ�
		//�@��x�V���v���ɂ���
		float diff[2] = { 0.0f,0.0f };
		diff[0] = mPaddlesPos[0].y - mBalls[i].pos.y;	//�p�h��0�ƃ{�[����y��������
		diff[1] = mPaddlesPos[1].y - mBalls[i].pos.y;	//�p�h��1�ƃ{�[����y��������
		// ��Βl���Ƃ�
		if (diff[0] < 0.0f) { diff[0] *= -1.0f; }
		if (diff[1] < 0.0f) { diff[1] *= -1.0f; }

		if (
			// �{�[���ƃp�h����y�����������\���ɏ�����
			diff[0] <= paddleH / 2.0f &&
			// �{�[����������x�ʒu�ɂ���
			mBalls[i].pos.x <= mPaddlesPos[0].x + thickness * 1.5f && mBalls[i].pos.x >= mPaddlesPos[0].x &&
			// �{�[�����������ɓ����Ă����
			mBalls[i].vel.x < 0.0f
			)
		{
			mBalls[i].vel.x *= -1.0f * mBallsAcc;
			
		}
		if (
			// �{�[���ƃp�h����y�����������\���ɏ�����
			diff[1] <= paddleH / 2.0f &&
			// �{�[����������x�ʒu�ɂ���
			(mBalls[i].pos.x <= mPaddlesPos[1].x ) && (mBalls[i].pos.x >= mPaddlesPos[1].x - thickness * 1.5f) &&
			// �{�[�����E�����ɓ����Ă����
			mBalls[i].vel.x > 0.0f
			)
		{
			mBalls[i].vel.x *= -1.0f * mBallsAcc;
		}
		if (mBalls[i].vel.x < -1000.0f) { mBalls[i].vel.x = -1000.0f; }
		else if (mBalls[i].vel.x > 1000.0f) { mBalls[i].vel.x = 1000.0f; }
	}
	

	
}

void Game::GenerateOutput()
{
	//�w�i�̐F��ݒ�
	SDL_SetRenderDrawColor(
		mRenderer,
		150,		// R
		150,		// G 
		150,		// B
		255			// A
	);
	// �w�i��P�F�ŃN���A
	SDL_RenderClear(mRenderer);

	// �ǂ̕`��
	// �ǂ̐F��ݒ�
	SDL_SetRenderDrawColor(mRenderer, 220, 220, 0, 255);
	// �㑤�̕ǂ�`��
	// SDL_Rect : �����`���쐬
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		1024,		// Width
		thickness	// Height
	};
	SDL_RenderFillRect(mRenderer, &wall);	//�쐬���������`��`��i�h��Ԃ��j
	// �����̕ǂ�`��
	wall.y = static_cast<int>(768-thickness);
	SDL_RenderFillRect(mRenderer, &wall);	//�쐬���������`��`��i�h��Ԃ��j	
	// �E���̕ǂ�`��@��2�l�v���C���[�ł͕s�v
	if (mPaddlesNum < 2) {
		wall.x = 1024 - static_cast<int>(thickness);
		wall.y = 0;
		wall.w = static_cast<int>(thickness);
		wall.h = 1024;
		SDL_RenderFillRect(mRenderer, &wall);
	}
	

	// �p�h����`��
	for (int j = 0; j < mPaddlesNum; j++)
	{
		SDL_Rect paddle{
			// static_cast���Z�q�́Afloat����int�ɕϊ�����
			static_cast<int>(mPaddlesPos[j].x - thickness/2.0f),
			static_cast<int>(mPaddlesPos[j].y - paddleH / 2.0f),
			thickness,
			static_cast<int>(paddleH)
		};
		//�p�h���̐F��ݒ�
		SDL_SetRenderDrawColor(
			mRenderer,
			0,		// R
			200*j,		// G 
			200*(1-j),	// B
			255		// A
		);
		SDL_RenderFillRect(mRenderer, &paddle);

	}

	// �{�[���̕`��
	for (int i = 0; i < mBallsNum; i++)
	{
		SDL_Rect ball{
			// static_cast���Z�q�́Afloat����int�ɕϊ�����
			static_cast<int>(mBalls[i].pos.x - thickness / 2.0f),
			static_cast<int>(mBalls[i].pos.y - thickness / 2.0f),
			thickness,
			thickness
		};
		//�{�[���̐F��ݒ�
		SDL_SetRenderDrawColor(
			mRenderer,
			200,		// R
			0,		// G 
			0,		// B
			255		// A
		);
		SDL_RenderFillRect(mRenderer, &ball);
	}
	
	// �`��o�b�t�@�̌���
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);	// SDL_Renderer��j��
	SDL_DestroyWindow(mWindow);		// SDL_Window��j��
	SDL_Quit();						// SDL�I��
}
