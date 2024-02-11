#include "GameVer2.h"
// �Q�[�����e�A�Q�[���œ��������́i�{�[���A�p�h���A�ǁj�͂��ׂăN���X������B
// �Q�[���𓮂����S�̂���������

Game::Game()
{
	mWindow = nullptr;
	mRenderer = nullptr;
	mTicksCount = 0;
	mIsRunning = true;
	pong = new Pong();			//�Q�[�����e���N���X��
}

bool Game::Initialize()			//�Q�[�������ݒ�B�E�B���h�E�쐬�A�����_���[������
{
	// SDL������
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0) {
		SDL_Log("SDL���������ł��܂��� : %s", SDL_GetError());
		return false;
	}

	// SDL�E�B���h�E���쐬
	mWindow = SDL_CreateWindow
	(
		"Pong in C++", // �E�B���h�E�̃^�C�g��
		100, // �E�B���h�E�������x���W
		100, // �E�B���h�E�������y���W
		WIN_W, // �E�B���h�E�̕�(width)		Game.h�Œ�`
		WIN_H, // �E�B���h�E�̍���(height)		Game.h�Œ�`
		0 // �t���O(�ݒ肵�Ȃ�����0)
	);

	if (!mWindow)
	{
		SDL_Log("�E�B���h�E�̍쐬�Ɏ��s���܂��� : %s", SDL_GetError());
		return false;
	}

	// SDL�����_���[���쐬
	mRenderer = SDL_CreateRenderer
	(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		//�����_���[�̏������Ɏ��s�����Ƃ�
		SDL_Log("�����_���[�̏������Ɏ��s���܂����@: %s", SDL_GetError());
		return false;
	}

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

void Game::ProcessInput()	//���͎�t
{	
	SDL_Event event;
	// �L���[�ɃC�x���g������ΌJ��Ԃ�
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:		// ���[�U�[���E�B���h�E����悤�Ƃ������͂̃C�x���g
			mIsRunning = false;
			break;			//�Q�[�����I������t���O
		default:
			break;
		}
	}
	const Uint8* state = SDL_GetKeyboardState(NULL);	//�L�[�{�[�h����

	if (state[SDL_SCANCODE_ESCAPE])
	{
		//ESC�ŃQ�[���I��
		mIsRunning = false;
	}
	pong->input(state);			//�Q�[���ڍׂ��N���X�����āA�L�[�{�[�h���͂������œn���B
}

void Game::UpdateGame()			//�X�V
{
	//�O�̃t���[������16ms(��60fps)���o�߂���܂ő҂�
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// deltatime�͑O�̃t���[���Ƃ̎����̍���b�ɕϊ������l
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	//deltatime���ő�l�Ő�������
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;		//50ms(20fps)
	}
	//�p�h���p
	mTicksCount = SDL_GetTicks();
	pong->update(deltaTime,mIsRunning);		//�Q�[���ڍׂ��N���X�����āA�Q�[���o�ߎ��Ԃ������œn���A�Q�[�����e����������B
}

void Game::GenerateOutput()			//��ʂ��X�V
{
	// �w�i
	// �`��F�����F�ɐݒ�
	SDL_SetRenderDrawColor
	(
		mRenderer,
		0, 0, 0, 0	 // (R,G,B,A)
	);
	// �o�b�N�o�b�t�@�����F�œh��Ԃ�
	SDL_RenderClear(mRenderer);

	// �`��F�𔒐F�ɐݒ�
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);


	pong->render(mRenderer);		//�Q�[���ڍׂ��N���X�����āA�����_���[���������œn���A�`����e����������B


	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�����ւ�
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown() {
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
	delete pong;	//���������
}
