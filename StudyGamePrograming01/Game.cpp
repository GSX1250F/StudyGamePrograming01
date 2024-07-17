#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <SDL_ttf.h>
#include "Random.hpp"
#include <math.h>

Game::Game()
	:mWindow(nullptr)
	, mIsRunning(true)
	, mRenderer(nullptr)
	, mTicksCount(0)
{}

bool Game::Initialize()
{
	//SDL��������
	//�r�f�I�T�u�V�X�e���̏������B
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
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
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!mRenderer)
	{
		SDL_Log("�����_���[�̍쐬�Ɏ��s���܂���: %s", SDL_GetError());
		return false;
	}

	// �p�h���̃X�v���C�g�p�摜��ǂݍ���
	paddleImage = nullptr;		//�e�N�X�`���p�|�C���^�ϐ���������
	std::string filename = "Assets/paddle.png";		//�t�@�C�����i�p�X�܂ށj
	SDL_Surface* surf = IMG_Load(filename.c_str());		//�T�[�t�F�C�X�ɉ摜��ǂݍ���
	if (!surf)
	{
		SDL_Log("�e�N�X�`���t�@�C���̓ǂݍ��݂Ɏ��s���܂��� %s", filename.c_str());
		return false;
	}
	paddleImage = SDL_CreateTextureFromSurface(mRenderer, surf);	//�T�[�t�F�C�X����e�N�X�`�����쐬
	if (!paddleImage)
	{
		SDL_Log("�T�[�t�F�C�X����e�N�X�`���̍쐬�Ɏ��s���܂��� %s", filename.c_str());
		return false;
	}
	SDL_FreeSurface(surf);	//�T�[�t�F�C�X���N���A����B

	// �e�L�X�g�\����p��
	if (TTF_Init() < 0)
	{
		SDL_Log("TTF�̏������Ɏ��s���܂��� %s", filename.c_str());
		return false;
	}
	TTF_Font* font = TTF_OpenFont("Assets/PixelMplus10-Regular.ttf", mFontSize);	// �t�H���g�t�@�C���̓ǂݍ��݂ƃT�C�Y�ݒ�
	// �|�[�Y�̃e�L�X�g
	surf = TTF_RenderUTF8_Blended(font, "Press S to Pause", { 255,255,255,255 });
	mText.push_back(SDL_CreateTextureFromSurface(mRenderer, surf));		//mText[0]���|�[�Y�̃e�L�X�g
	int iw, ih;
	SDL_QueryTexture(mText[0], NULL, NULL, &iw, &ih);	//������`�ʂ���Texture�̃T�C�Y���擾����
	mTextSize.push_back(Vector2{ iw * 1.0f, ih * 1.0f });		//mTextSize[0]���|�[�Y�̃e�L�X�g�T�C�Y
	mTextPos.push_back(Vector2{ mWindowW / 2.0f - iw / 2.0f, mWindowH / 2.0f - ih / 2.0f });		//mTextOffset[0]���|�[�Y�̃e�L�X�g�\���ʒu
	SDL_FreeSurface(surf);
	// GameOver�̃e�L�X�g
	surf = TTF_RenderUTF8_Blended(font, "Game Over", { 255,255,255,255 });
	mText.push_back(SDL_CreateTextureFromSurface(mRenderer, surf));
	SDL_QueryTexture(mText[1], NULL, NULL, &iw, &ih);
	mTextSize.push_back(Vector2{ iw * 1.0f, ih * 1.0f });
	mTextPos.push_back(Vector2{ mWindowW / 2.0f - iw / 2.0f, mWindowH / 2.0f - ih });
	SDL_FreeSurface(surf);
	// ���X�^�[�g�̃e�L�X�g
	surf = TTF_RenderUTF8_Blended(font, "Press R to restart", { 255,255,255,255 });
	mText.push_back(SDL_CreateTextureFromSurface(mRenderer, surf));
	SDL_QueryTexture(mText[2], NULL, NULL, &iw, &ih);
	mTextSize.push_back(Vector2{ iw * 1.0f, ih * 1.0f });
	mTextPos.push_back(Vector2{ mWindowW / 2.0f - iw / 2.0f, mWindowH / 2.0f - ih + mFontSize });
	SDL_FreeSurface(surf);

	ResetGame();

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
{
	SDL_Event event;
	while (SDL_PollEvent(&event))		// SDL�L���[�ɃC�x���g�������true
	{
		if (event.type == SDL_QUIT)
		{
			// ���[�U�[���E�B���h�E����悤�Ƃ������͂̃C�x���g
			mIsRunning = false;		//�Q�[�����I������t���O
		}
		if (event.type == SDL_KEYDOWN)	// �L�[�������ꂽ����s����
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				mIsRunning = false;		//�Q�[�����I������t���O
			}
			if (event.key.keysym.sym == SDLK_UP)
			{
				mPaddleDir -= 1;
				if (mPaddleDir < -1) { mPaddleDir = -1; }
			}
			if (event.key.keysym.sym == SDLK_DOWN)
			{
				mPaddleDir += 1;
				if (mPaddleDir > 1) { mPaddleDir = 1; }
			}
			if (event.key.keysym.sym == SDLK_s && !event.key.repeat)
			{
				if (scene == 0 && pause == false)
				{
					scene = 1;
				}
				if (scene == 1 && pause == true)
				{
					scene = 0;
					pause = false;
				}
			}
			if (event.key.keysym.sym == SDLK_r)
			{
				if (scene == 2) { ResetGame(); }
			}
		}
		else    // �������삵�Ă��Ȃ��Ƃ�
		{
			mPaddleDir = 0;
		}
		if (event.type == SDL_KEYUP)	// �L�[�������ꂽ����s����
		{
			if (event.key.keysym.sym == SDLK_s)
			{
				if (scene == 1)
				{
					pause = true;
				}
			}
		}
	}
}

void Game::UpdateGame()
{
	//�Q�[�����Ԃ̍X�V�A�����Ȃ�	
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));	//16ms�o�߂܂ł͑҂i�t���[�������j�B��60fps
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;		// deltaTime���v�Z�B�P�ʂ͕b�ɂ���
	if (deltaTime > 0.05f) { deltaTime = 0.05f; }		// �X�V���x�����Ă��Œ��fps���m�ہB50ms (20fps)
	mTicksCount = SDL_GetTicks();		// ���̃t���[���̂���tick counts���X�V

	if (scene == 0)
	{
		// �p�h���ʒu�̍X�V
		mPaddlePos.y += mPaddleDir * mPaddleSpeed * deltaTime;
		if (mPaddlePos.y < (paddleH / 2.0f + thickness))
		{
			mPaddlePos.y = paddleH / 2.0f + thickness;
		}
		if (mPaddlePos.y > (mWindowH - paddleH / 2.0f - thickness))
		{
			mPaddlePos.y = mWindowH - paddleH / 2.0f - thickness;
		}

		// �X�V��̃{�[���ʒu���v�Z
		Vector2 mBallPosPost;
		mBallPosPost.x = mBallPos.x + mBallVel.x * deltaTime;
		mBallPosPost.y = mBallPos.y + mBallVel.y * deltaTime;
		// �{�[�����ǂɓ��������璵�˕Ԃ�
		if (mBallPosPost.x + thickness * 0.5f >= (mWindowW - thickness) && mBallVel.x > 0.0f)
		{
			mBallVel.x *= -1.0f;
			mBallPosPost.x = mWindowW - thickness - thickness * 0.5f;
		}
		if (mBallPosPost.y - thickness * 0.5f <= thickness && mBallVel.y < 0.0f)
		{
			mBallVel.y *= -1.0f;
			mBallPosPost.y = thickness + thickness * 0.5f;
		}
		if (mBallPosPost.y + thickness * 0.5f >= mWindowH - thickness && mBallVel.y > 0.0f)
		{
			mBallVel.y *= -1.0f;
			mBallPosPost.y = mWindowH - thickness - thickness * 0.5f;
		}
		// �p�h���Ń{�[�������˕Ԃ锻��
		// �X�V��̃{�[�����[���p�h���E�[��菬�����A�{�[�����������ł���Ƃ��ɁA
		// �{�[���X�V�O�ƍX�V��̒��������߁A�p�h����x���W�ł�y���W���p�h���͈͓��ł��邩
		if (mBallPosPost.x < mPaddlePos.x + thickness && mBallVel.x < 0.0f)
		{
			float intersection_y = (mBallPos.y - mBallPosPost.y) / (mBallPos.x - mBallPosPost.x) * (mPaddlePos.x - mBallPosPost.x) + mBallPosPost.y;
			if (intersection_y >= mPaddlePos.y - paddleH * 0.5f && intersection_y <= mPaddlePos.y + paddleH * 0.5f)
			{
				mBallVel.x *= -1.1f;	// �������{�[���X�s�[�hup
				if (mBallVel.x < -2500.0f) { mBallVel.x = -2500.0f; }
				if (mBallVel.x > 2500.0f) { mBallVel.x = 2500.0f; }
				mBallPosPost.x = mPaddlePos.x + thickness;
			}
		}
		// �{�[���ʒu���X�V
		mBallPos.x = mBallPosPost.x;
		mBallPos.y = mBallPosPost.y;
		
		// �{�[�������[�ɂ����Ă��܂�����Q�[���I�[�o�[�B
		if (mBallPos.x <= 0.0f)
		{
			scene = 2;
		}

	}

}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 50, 50, 50, 255);		//�w�i�̐F���D�F�ɐݒ�
	SDL_RenderClear(mRenderer);		// �w�i��P�F�ŃN���A

	// �ǂ̕`��
	SDL_SetRenderDrawColor(mRenderer, 200, 200, 200, 255);		// �ǂ̐F��ݒ�
	SDL_Rect wall{ 0,0,mWindowW,thickness };		// �㑤�̕�
	SDL_RenderFillRect(mRenderer, &wall);		//�쐬���������`��`��i�h��Ԃ��j
	wall = { 0,mWindowH - thickness,mWindowW,thickness };		// �����̕ǂ�`��
	SDL_RenderFillRect(mRenderer, &wall);
	wall = { mWindowW - thickness,0,thickness,mWindowH };		// �E���̕�
	SDL_RenderFillRect(mRenderer, &wall);

	if (scene == 0 || scene == 1)		// �Q�[�����s���܂��̓Q�[���|�[�Y��
	{
		// �p�h����`��
		/*
		SDL_Rect paddle{
			static_cast<int>(mPaddlePos.x - thickness / 2.0f),
			static_cast<int>(mPaddlePos.y - paddleH / 2.0f),
			thickness,
			static_cast<int>(paddleH)
		};
		SDL_SetRenderDrawColor(mRenderer,255,255,255,255);
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
			static_cast<int>(mBallPos.x - thickness / 2),
			static_cast<int>(mBallPos.y - thickness / 2),
			thickness,
			thickness
		};
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
		SDL_RenderFillRect(mRenderer, &ball);

		// �e�L�X�g�\��
		SDL_Rect txtRect = { 0,0,static_cast<int>(mTextSize[0].x),static_cast<int>(mTextSize[0].y) };
		SDL_Rect pasteRect = { static_cast<int>(mTextPos[0].x),static_cast<int>(mTextPos[0].y),static_cast<int>(mTextSize[0].x),static_cast<int>(mTextSize[0].y) };
		SDL_RenderCopy(mRenderer, mText[0], &txtRect, &pasteRect);
	}
	else
	{
		// �Q�[���I�[�o�[��
		for (int i = 1; i <= 2; i++)
		{
			// �e�L�X�g�\��
			SDL_Rect txtRect = { 0,0,static_cast<int>(mTextSize[i].x),static_cast<int>(mTextSize[i].y) };
			SDL_Rect pasteRect = { static_cast<int>(mTextPos[i].x),static_cast<int>(mTextPos[i].y),static_cast<int>(mTextSize[i].x),static_cast<int>(mTextSize[i].y) };
			SDL_RenderCopy(mRenderer, mText[i], &txtRect, &pasteRect);
		}
	}

	SDL_RenderPresent(mRenderer);		// �`��o�b�t�@�̌���
}

void Game::ResetGame()
{
	// �p�h���ƃ{�[���ʒu�E�����E���������Z�b�g
	mPaddlePos = Vector2{ thickness * 2.0f,mWindowH / 2.0f };
	mPaddleDir = 0;
	mPaddleSpeed = 200.0f;
	mBallPos = Vector2{ mWindowW / 2.0f,mWindowH / 2.0f };
	//mBallVel = Vector2{ -200.0f,235.0f };
	int angle = 1.0f * rnd(15, 75);	// 15�`75�͈̗̔͂������擾
	int pmx = 2 * rnd(0, 1) - 1;	// -1 , 1 �̂ǂ��炩�𗐐��Ŏ擾
	int pmy = 2 * rnd(0, 1) - 1;	// -1 , 1 �̂ǂ��炩�𗐐��Ŏ擾
	float velx = 1.0f * pmx * mWindowH * 0.4f * cos(angle / 180.0f * M_PI);
	float vely = 1.0f * pmy * mWindowH * 0.4f * sin(angle / 180.0f * M_PI);
	mBallVel = Vector2{velx , vely};

	scene = 0;
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);	// SDL_Renderer��j��
	SDL_DestroyWindow(mWindow);		// SDL_Window��j��
	SDL_Quit();						// SDL�I��
}