#pragma once
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "SDL.h"

struct Vector2
{
	float x;
	float y;
};

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();
private:
	void ProcessInput();	// 1.入力を受け付ける
	void UpdateGame();		// 2.ゲームを処理・更新する
	void GenerateOutput();	// 3.画面や音などを出力する

	SDL_Window* mWindow;	// SDLでつくられるウィンドウ
	SDL_Renderer* mRenderer;	// 2D描画用レンダラ
	Uint32 mTicksCount;		// ゲーム開始時からの経過時間
	bool mIsRunning;		// ゲーム実行中か否か
	
	// Game specific
	int mPaddleDir;		// パドルの動作方向。+が下方向、-が上方向。
	Vector2 mPaddlePos;		// パドルの位置（2次元ベクトル形式）
	Vector2 mBallPos;		// ボールの位置（2次元ベクトル形式）
	Vector2 mBallVel;		// ボールの速度（2次元ベクトル形式）
	const int thickness = 15;	// 壁・ボール・パドルの厚み
	const float paddleH = 150.0f;	// パドルの高さ
	const int mWindowW = 1024;	//ウィンドウの横幅
	const int mWindowH = 768;	//ウィンドウの縦幅
	SDL_Texture* paddleImage;	// パドルのテクスチャ
	SDL_Texture* gameOverText;		// ゲームオーバーのテキスト表示用

};