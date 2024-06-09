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
	Game();			// コンストラクタ
	bool Initialize();		// 初期化
	void RunLoop();			// ループ実行
	void Shutdown();		// シャットダウン
private:
	void ProcessInput();	// 入力受付
	void UpdateGame();		// ゲームの処理と更新
	void GenerateOutput();	// 画面や音などの出力

	SDL_Window* mWindow;	// SDLでつくられるウィンドウ
	SDL_Renderer* mRenderer;	// 2D描画用レンダラ
	Uint32 mTicksCount;		// ゲーム開始時からの経過時間
	bool mIsRunning;		// ゲーム実行中か否か
	const int mWindowW = 1024;	//ウィンドウの横幅
	const int mWindowH = 768;	//ウィンドウの縦幅


	// Game specific
	void ResetGame();		// ゲームの各パラメータをリセット
	int mPaddleDir;		// パドルの動作方向。+が下方向、-が上方向。
	float mPaddleSpeed;		// パドルの動作速度
	Vector2 mPaddlePos;		// パドルの位置（2次元ベクトル形式）
	Vector2 mBallPos;		// ボールの位置（2次元ベクトル形式）
	Vector2 mBallVel;		// ボールの速度（2次元ベクトル形式）
	const int thickness = 15;	// 壁・ボール・パドルの厚み
	const float paddleH = 150.0f;	// パドルの高さ
	SDL_Texture* paddleImage;	// パドルのテクスチャ
	int scene = 0;	//0:ゲーム中 , 1:ポーズ中 , 2:ゲームオーバー
	bool pause = false;	//true:ポーズ中
	std::vector<SDL_Texture*> mText;		// テキスト表示用
	std::vector<Vector2> mTextPos;		// テキスト表示位置
	std::vector<Vector2> mTextSize;		// テキストサイズ
	const int mFontSize = 100;	//テキストのフォントサイズ
};