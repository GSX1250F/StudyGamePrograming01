#include "Pong.h"
#include "GameVer2.h"

Pong::Pong()	//コンストラクタ 初期設定
{
	PADDLES_NUM = 1;		//とりあえず1player
	BALLS_NUM = 1;		//とりあえず1個
	for (int i = 0; i < BALLS_NUM; i++)
	{
		Ball ball0_info = Ball();
		mBalls.push_back(ball0_info);
	}
	
	L_Paddle = new Paddle(L_PADDLE_POS, WIN_H / 2.0f);
	R_Paddle = new Paddle(R_PADDLE_POS, WIN_H / 2.0f);
	court = new Court();
}

Pong::~Pong()	//デストラクタ　作った構造体を消す
{
	delete court;
	delete L_Paddle;
	delete R_Paddle;
}

void Pong::render(SDL_Renderer* renderer)
{
	//描画処理
	L_Paddle->render(renderer);
	R_Paddle->render(renderer);
	for (auto it = mBalls.begin(); it != mBalls.end(); it++)
	{
		it->render(renderer);
	}
	court->render(renderer,PADDLES_NUM);
}

void Pong::init()
{
	//起動直後の状態に戻す
	for (int i = 0; i < mBalls.size() - 1; i++)
		mBalls.pop_back();
	mBalls[0].init();
}

void Pong::update(float deltaTime)
{
	if (scene == 1)
	{
		//データ更新（全てのシーン）
		L_Paddle->update(deltaTime);
		R_Paddle->update(deltaTime);
		for (auto it = mBalls.begin(); it != mBalls.end(); it++)
		{
			it->update(L_Paddle, R_Paddle, this, deltaTime, it,scene);
		}

	}
	return;
}

void Pong::input(const Uint8* state)		//入力情報に基づいて処理
{
	//左パドル操作
	if (state[SDL_SCANCODE_Q]) { L_Paddle->moveUp(); }
	else if (state[SDL_SCANCODE_A]) { L_Paddle->moveDown(); }
	else { L_Paddle->moveStop(); }
	//右パドル操作
	if (state[SDL_SCANCODE_UP]) { R_Paddle->moveUp(); }
	else if (state[SDL_SCANCODE_DOWN]) { R_Paddle->moveDown(); }
	else { R_Paddle->moveStop(); }
	//リトライ操作
	if (state[SDL_SCANCODE_R]) { init(); }
	//スタート操作（シーン0）
	if (scene == 0)
	{
//		if (state[SDL_SCANCODE_1])
//		{
//			scene = 1;
//		}
//		if (state[SDL_SCANCODE_2])
//		{
//			if (scene == 0)scene = 1;
//		}
		return;
	}
}