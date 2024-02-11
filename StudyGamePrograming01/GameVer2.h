#pragma once
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <math.h>
#include <iostream>//デバックとかで
#include "SDL.h"
#include "Ball.h"
#include "Paddle.h"
#include "Pong.h"

#define WIN_W 1024
#define WIN_H 768
#define WALL_W 12
#define PADDLE_W 12
#define PADDLE_H 60
#define PADDLE_SPEED 150.0f
#define L_PADDLE_POS 24.0f
#define R_PADDLE_POS WIN_W - 24.0f

#define BALL_W 12
#define BALL_SPEED 300.0f
#define BALL_INIT_X WIN_W
#define BALL_INIT_Y WIN_H/2
#define BALL_INIT_VX cos(2.0/3.0*M_PI)
#define BALL_INIT_VY sin(2.0/3.0*M_PI)

//#define PREDICTION_WAITTIME 0.1f
//#define WARD_W 50
//#define WARD_WG 5
//#define WARD_HG 20

class Game
{
public:
	Game();
	bool Initialize();			//ゲームを初期化する
	void RunLoop();				//ゲームオーバーまでゲームループを実行する
	void Shutdown();			//ゲームをシャットダウンする

private:
	//ゲームループのためのヘルパー関数群
	void ProcessInput();		//入力を受け付ける
	void UpdateGame();			//ゲームを処理する
	void GenerateOutput();		//更新する。

	SDL_Window* mWindow;		//SDLが作るウィンドウ
	SDL_Renderer* mRenderer;	//2D描画のためのレンダリング
	Uint32 mTicksCount;			//ゲーム時間
	bool mIsRunning;			//ゲームの続行を支持する

	Pong* pong;					//ゲーム内容をクラス化

};