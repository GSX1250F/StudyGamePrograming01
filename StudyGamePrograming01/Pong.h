#pragma once
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <math.h>
#include "SDL.h"
#include "Ball.h"
#include "Paddle.h"
#include "Court.h"
using namespace std;


class Ball;			//相互参照
class Paddle;		//相互参照

class Pong
{
public:
//ゲームに使用する定数群
	int scene = 1;				// ゲームシーン
	int PADDLES_NUM;		// パドルの数
	int BALLS_NUM;			// ボールの数
	// なんやろ・・・
	// float PREDICTION_WAITTIME 0.1f;
	// int WARD_W = 50;
	// int WARD_WG = 5;
	// int WARD_HG = 20;

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

