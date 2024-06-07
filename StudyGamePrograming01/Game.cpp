#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;
const int mWindowW = 1024;
const int mWindowH = 768;

Game::Game()
	:mWindow(nullptr)
	, mRenderer(nullptr)
	, mTicksCount(0)
	, mIsRunning(true)
	
{	

}

bool Game::Initialize()
{
	//SDL��������
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);	//�r�f�I�T�u�V�X�e���̏������B�Ԃ�l�͐����B�Ԃ�l��0�łȂ��Ƃ��͏��������s
	
	if (sdlResult != 0)
	{
		SDL_Log("SDL�̏������Ɏ��s���܂���: %s", SDL_GetError());
		return false;
	}

	// SDL Window���쐬
	mWindow = SDL_CreateWindow(
		"Game Programming in C++", // Window�^�C�g��
		100,	// window�̍����X���W
		100,	// window�̍����Y���W
		mWindowW,	// window�̉���
		mWindowH,	// window�̏c��
		0		// �t���O (0 �͉����Z�b�g���Ȃ�)
	);
	
	if (!mWindow)
	{
		SDL_Log("window�쐬�Ɏ��s���܂���: %s", SDL_GetError());
		return false;
	}

	// SDL�����_���[���쐬
	mRenderer = SDL_CreateRenderer(
		mWindow, // �����_���[�̑Ώ�
		-1,		 // �ʏ�� -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		SDL_Log("�����_���[�̍쐬�Ɏ��s���܂���: %s", SDL_GetError());
		return false;
	}

	//�p�h���A�{�[���̈ʒu�E�����E������������
	mPaddlePos.x = thickness;
	mPaddlePos.y = mWindowH / 2.0f;
	mBallPos.x = mWindowW / 2.0f;
	mBallPos.y = mWindowH / 2.0f;
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
			case SDL_QUIT:		// ���[�U�[���E�B���h�E����悤�Ƃ������͂̃C�x���g
				mIsRunning = false;		//�Q�[�����I������t���O
				break;
		}
	}

	// �L�[�{�[�h�̏�Ԃ��擾
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])		//ESC�L�[�������ꂽ�Ƃ�
	{
		mIsRunning = false;		//�Q�[�����I������t���O
	}

	// �����L�[�Ńp�h���̕���������
	mPaddleDir = 0;		//������������
	if (state[SDL_SCANCODE_UP])
	{
		mPaddleDir -= 1;		//�������y���W�����炷����
	}
	if (state[SDL_SCANCODE_DOWN])
	{
		mPaddleDir += 1;		//��������y���W�𑝂₷����
	}
}

void Game::UpdateGame()
{
	//�Q�[�����Ԃ̍X�V�A�����Ȃ�	
	//16ms�o�߂܂ł͑҂i�t���[�������j�B��60fps
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// Delta time���v�Z�B�P�ʂ͕b�ɂ���
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// �X�V���x�����Ă��Œ��fps���m��
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;		//50ms (20fps)
	}

	// ���̃t���[���̂���tick counts���X�V
	mTicksCount = SDL_GetTicks();

		// �p�h���ʒu�̍X�V
	// Update paddle position based on direction
	if (mPaddleDir != 0)
	{
		mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;	//�p�h����Y���W���A300�s�N�Z��/�b��������
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

	// �{�[���ʒu�̍X�V
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
	//�w�i�̐F��ݒ�
	SDL_SetRenderDrawColor(
		mRenderer,
		50,		// R
		50,		// G 
		50,		// B
		255		// A
	);
	// �w�i��P�F�ŃN���A
	SDL_RenderClear(mRenderer);

	// �ǂ̕`��
	// �ǂ̐F��ݒ�
	SDL_SetRenderDrawColor(mRenderer, 200, 200, 200, 255);
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
	wall.y = 768 - thickness;		
	SDL_RenderFillRect(mRenderer, &wall);	//�쐬���������`��`��i�h��Ԃ��j	
	// �E���̕ǂ�`��
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 1024;
	SDL_RenderFillRect(mRenderer, &wall);


	// �p�h����`��
	SDL_Rect paddle{
		// static_cast���Z�q�́Afloat����int�ɕϊ�����
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	//�p�h���̐F��ݒ�
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		0,		// G 
		200,		// B
		255		// A
	);
	SDL_RenderFillRect(mRenderer, &paddle);

	// �{�[���̕`��
	SDL_Rect ball{
		// static_cast���Z�q�́Afloat����int�ɕϊ�����
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
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

	// �`��o�b�t�@�̌���
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);	// SDL_Renderer��j��
	SDL_DestroyWindow(mWindow);		// SDL_Window��j��
	SDL_Quit();						// SDL�I��
}