// 2プレイヤー＆2ボール
// 位置と速度の2つのVector2を含むBall構造体を作る。 Ball{ Vector2 位置 , Vector2 速度 }
// 複数のボールのために、std::vector<Ball>型のメンバー変数配列をGameに追加する。
// つまりボールのvector配列は　{ {位置.x , 位置.y } , { 速度.x , 速度.y} } となる。
// Game::Initializeのコードを変更して、複数のボールの位置と速度を初期化する。
// Game::UpdateGameで、ボール更新のコードを変更する。
// mBallVelとmBallPosを使う代わりに、std::vectorをループして、すべてのボールを更新する。
// パドルについて
// std::vector<int>型でパドルの方向の配列を作る。 { パドル0の方向 , パドル1の方向 }
// std::vector<Vector2>型でパドルの位置の配列を作る。{ { パドル0.x , パドル0.y } , { パドル1.x , パドル1.y }  }

//プレイヤー数を選択制にする場合、タイトル画面とプレイ画面を構築。
// scene変数を用意し、0のときはタイトル、1のときはプレイ画面とする。


#pragma once
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "SDL.h"

// Vector2 struct just stores x/y coordinates
// (for now)
// structはデフォルトのアクセシビリティが public
// classはデフォルトのアクセシビリティが private
struct Vector2
{
public: //←structだからあっても無くても動作は同じ。
	float x;
	float y;
};

struct Ball		// Ball構造体。Vector2型の位置と速度をもつ。
{
	Vector2 pos;
	Vector2 vel;
};

// Game class
class Game
{
public:
	// コンストラクタ
	// Initialize the game
	Game();
	// Runs the game loop until the game is over
	bool Initialize();
	void RunLoop();
	// Shutdown the game
	void Shutdown();
private:
	// Helper functions for the game loop
	void ProcessInput();	// 1.入力を受け付ける
	void UpdateGame();		// 2.ゲームを処理する
	void GenerateOutput();	// 3.更新

	// Window created by SDL
	SDL_Window* mWindow;
	// Renderer for 2D drawing
	SDL_Renderer* mRenderer;
	// Number of ticks since start of game
	Uint32 mTicksCount;
	// Game should continue to run
	bool mIsRunning;

	// Pong specific
	std::vector<int> mPaddlesDir;	//パドルの方向配列。パドル0とパドル1の方向をもつ
	std::vector<Vector2> mPaddlesPos;	//パドルの位置のVector2型配列。パドル0のx,y とパドル1のx,y
	std::vector<Ball> mBalls;		//ボール型の配列。ボールの位置x,yと速度x,y
	int mBallsNum;	//ボールの数
	int mPaddlesNum;	//パドルの数
	float paddleH;	//パドルの長さ
	float thickness;	//壁・玉・パドルの厚み
	float mBallsAcc;	//ボールの加速度

	// タイトル画面かプレイ画面かの変数
	int scene;

	//メモ　ボールをBall構造体にするなら、パドルもPaddle構造体にした方がいい？
	// struct Paddle { int dir , Vector2　pos } として、 std::vector<Paddle> mPaddles　とする。
	// mPaddleNum を 1か2で選択にして、Paddle構造体を その数だけ作ればいいのでは。
	// 
};
