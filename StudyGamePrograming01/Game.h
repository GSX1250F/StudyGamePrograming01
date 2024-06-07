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
	SDL_Texture* mTexture;	// テクスチャ

	// Game specific
	int mPaddleDir;		// パドルの動作方向。+が下方向、-が上方向。
	Vector2 mPaddlePos;		// パドルの位置（2次元ベクトル形式）
	Vector2 mBallPos;		// ボールの位置（2次元ベクトル形式）
	Vector2 mBallVel;		// ボールの速度（2次元ベクトル形式）
};