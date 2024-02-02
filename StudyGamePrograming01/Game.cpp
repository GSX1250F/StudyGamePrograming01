// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
	:mWindow(nullptr)
	, mRenderer(nullptr)
	, mTicksCount(0)
	, mIsRunning(true)
	, mPaddleDir(0)
{	

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

	//
	mPaddlePos.x = 10.0f;
	mPaddlePos.y = 768.0f / 2.0f;
	mBallPos.x = 1024.0f / 2.0f;
	mBallPos.y = 768.0f / 2.0f;
	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;
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

	// Update paddle direction based on W/S keys
	mPaddleDir = 0;
	if (state[SDL_SCANCODE_W])
	{
		mPaddleDir -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		mPaddleDir += 1;
	}
}

void Game::UpdateGame()
{
	


	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// Delta time is the difference in ticks from last frame
	// (converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();

	// Update paddle position based on direction
	if (mPaddleDir != 0)
	{
		mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen!
		if (mPaddlePos.y < (paddleH / 2.0f + thickness))
		{
			mPaddlePos.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddlePos.y > (768.0f - paddleH / 2.0f - thickness))
		{
			mPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	// Update ball position based on ball velocity
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	// Bounce if needed
	// Did we intersect with the paddle?
	float diff = mPaddlePos.y - mBallPos.y;
	// Take absolute value of difference
	diff = (diff > 0.0f) ? diff : -diff;
	if (
		// Our y-difference is small enough
		diff <= paddleH / 2.0f &&
		// We are in the correct x-position
		mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
		// The ball is moving to the left
		mBallVel.x < 0.0f)
	{
		mBallVel.x *= -1.0f;
	}
	// Did the ball go off the screen? (if so, end game)
	else if (mBallPos.x <= 0.0f)
	{
		mIsRunning = false;
	}
	// Did the ball collide with the right wall?
	else if (mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f)
	{
		mBallVel.x *= -1.0f;
	}

	// Did the ball collide with the top wall?
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
	{
		mBallVel.y *= -1;
	}
	// Did the ball collide with the bottom wall?
	else if (mBallPos.y >= (768 - thickness) &&
		mBallVel.y > 0.0f)
	{
		mBallVel.y *= -1;
	}
}

void Game::GenerateOutput()
{
	// Set draw color to blue
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		0,		// G 
		255,	// B
		255		// A
	);

	// Clear back buffer
	SDL_RenderClear(mRenderer);

	// Draw walls
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	// Draw top wall
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		1024,		// Width
		thickness	// Height
	};
	SDL_RenderFillRect(mRenderer, &wall);

	// Draw bottom wall
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);

	// Draw right wall
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 1024;
	SDL_RenderFillRect(mRenderer, &wall);

	// Draw paddle
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle);

	// Draw ball
	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);	// SDL_Renderer��j��
	SDL_DestroyWindow(mWindow);		// SDL_Window��j��
	SDL_Quit();						// SDL�I��
}
