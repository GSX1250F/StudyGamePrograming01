#pragma once
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <math.h>
#include "SDL.h"
#include "Ball.h"
#include "Paddle.h"
#include "Court.h"
#include "GameVer2.h"

using namespace std;


class Ball;			//子クラスとしてボールを作成
class Paddle;		//子クラスとしてパドルを作成
class Court;		//子クラスとしてコートを作成
class Game;

class Pong : public Game
{
protected:
	//Gameクラス >継承> Pong >継承> 各実装プログラム
//ゲームに使用する定数群
	int scene = 1;				// ゲームシーン
	const int WALL_W = 12;		// 壁の厚み
	const int PADDLE_W = 12;	// パドルの厚み
	const int PADDLE_H = 150;		// パドルの長さ
	const float PADDLE_SPEED = 300.0f;	// パドルのスピード
	const float L_PADDLE_POS = 20.0f;		// 左パドル初期位置
	const float R_PADDLE_POS = WIN_W - 20.0f;		// 右パドル初期位置
	const int BALL_W = 12;		// ボール厚み
	const float BALL_SPEED = 300.0f;		// ボールのスピード
	const float BALL_INIT_X = WIN_W / 2;		// ボールの初期位置x
	const float BALL_INIT_Y = WIN_H / 2;		// ボールの初期位置y
	float BALL_INIT_VX;		// ボールの初期スピードx
	float BALL_INIT_VY;		// ボールの初期スピードy
	int PADDLES_NUM;		// パドルの数
	int BALLS_NUM;			// ボールの数
	// なんやろ・・・
	// float PREDICTION_WAITTIME 0.1f;
	// int WARD_W = 50;
	// int WARD_WG = 5;
	// int WARD_HG = 20;


public:
	Pong();
	~Pong();
	void render(SDL_Renderer*);			// レンダラー
	void init();						// 初期化
	void update(float);;					// 処理 (引数は deltatime)
	void input(const Uint8*);			// 入力受付 (引数は const Uint8* state = SDL_GetKeyboardState(NULL) の内容
	
	//コート
	Court* court;
	//バドル
	Paddle* L_Paddle;
	Paddle* R_Paddle;
	//ボール
	vector<Ball> mBalls;


};

