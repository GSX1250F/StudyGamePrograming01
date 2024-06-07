#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>



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
	mPaddlePos.x = thickness * 2.0f;
	mPaddlePos.y = mWindowH / 2.0f;
	mBallPos.x = mWindowW / 2.0f;
	mBallPos.y = mWindowH / 2.0f;
	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;

	// �p�h���̃X�v���C�g�p�摜��ǂݍ���
	paddleImage = nullptr;
	std::string filename = "Assets/paddle.png";
	SDL_Surface* surf = IMG_Load(filename.c_str());
	if (!surf)
	{
		SDL_Log("�e�N�X�`���t�@�C���̓ǂݍ��݂Ɏ��s���܂��� %s", filename.c_str());
		return false;
	}
	paddleImage = SDL_CreateTextureFromSurface(mRenderer, surf);
	SDL_FreeSurface(surf);
	if (!paddleImage)
	{
		SDL_Log("�T�[�t�F�C�X����e�N�X�`���̍쐬�Ɏ��s���܂��� %s", filename.c_str());
		return false;
	}

	// �Q�[���I�[�o�[��ʂ�p��
	if (TTF_Init()<0) 
	{
		SDL_Log("TTF�̏������Ɏ��s���܂��� %s", filename.c_str());
		return false;
	}
	TTF_Font* font = TTF_OpenFont("ipag-mona.ttf", 24); /* IPA���i�[�t�H���g���g�p */
	auto string_color = SDL_Color();
	string_color.r = 0;
	string_color.g = 0;
	string_color.b = 255;
	string_color.a = 255;
	auto test = TTF_RenderUTF8_Blended(font, u8"anti aliased string", string_color);
	gameOver = SDL_CreateTextureFromSurface(mRenderer, surf);

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
		else if (mPaddlePos.y > (mWindowH - paddleH / 2.0f - thickness))
		{
			mPaddlePos.y = mWindowH - paddleH / 2.0f - thickness;
		}
	}

	// �{�[���ʒu�̍X�V
	// �ʒu += ����*deltaTime
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	// �p�h���Ń{�[���𔽎ˁB
	float diff = mPaddlePos.y - mBallPos.y;
	diff = (diff > 0.0f) ? diff : -diff;		//��Βl�ɂ���
	if (
		
		diff <= paddleH / 2.0f &&		// y���W�̍����\���ɏ�����
		mBallPos.x <= mPaddlePos.x + thickness && mBallPos.x >= mPaddlePos.x + thickness / 2.0f &&		// �{�[����x���W���p�h���͈͓̔��ɂ���
		mBallVel.x < 0.0f)		// �{�[�����������ɓ����Ă���
	{
		mBallVel.x *= -1.1f;		// �{�[���X�s�[�h�A�b�v
	}
	// �{�[�������[�ɂ����Ă��܂�����Q�[���I�[�o�[�B
	else if (mBallPos.x <= 0.0f)
	{
		mIsRunning = false;
	}
	// �{�[�����E�ǂɓ��������甽��
	else if (mBallPos.x >= (mWindowW - thickness) && mBallVel.x > 0.0f)
	{
		mBallVel.x *= -1.0f;
	}

	// �{�[������ǂɓ��������甽��
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
	{
		mBallVel.y *= -1;
	}
	// �{�[�������ǂɓ��������甽��
	else if (mBallPos.y >= (mWindowH - thickness) &&
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
		mWindowW,	// Width
		thickness	// Height
	};
	SDL_RenderFillRect(mRenderer, &wall);	//�쐬���������`��`��i�h��Ԃ��j
	// �����̕ǂ�`��
	wall.y = mWindowH - thickness;		
	SDL_RenderFillRect(mRenderer, &wall);	//�쐬���������`��`��i�h��Ԃ��j	
	// �E���̕ǂ�`��
	wall.x = mWindowW - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = mWindowW;
	SDL_RenderFillRect(mRenderer, &wall);

	// �p�h����`��
	/*
	SDL_Rect paddle{
		// static_cast���Z�q�́Afloat����int�ɕϊ�����
		static_cast<int>(mPaddlePos.x - thickness / 2.0f),
		static_cast<int>(mPaddlePos.y - paddleH / 2.0f),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_SetRenderDrawColor(
		mRenderer,
		255,		// R
		255,		// G 
		255,		// B
		255			// A
	);
	SDL_RenderFillRect(mRenderer, &paddle);
	*/
	if (paddleImage)
	{
		SDL_Rect r;
		r.w = static_cast<int>(thickness);
		r.h = static_cast<int>(paddleH);
		r.x = static_cast<int>(mPaddlePos.x - thickness / 2.0f);
		r.y = static_cast<int>(mPaddlePos.y - paddleH / 2.0f);

		SDL_RenderCopyEx(mRenderer, paddleImage, nullptr, &r, 0, nullptr, SDL_FLIP_NONE);
	}


	// �{�[���̕`��
	SDL_Rect ball{
		// static_cast���Z�q�́Afloat����int�ɕϊ�����
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_SetRenderDrawColor(
		mRenderer,
		255,		// R
		255,		// G 
		255,		// B
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