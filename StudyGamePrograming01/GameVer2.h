#pragma once
#include <cstdio>
#include <cstdlib>
#include <iostream>//デバックとかで
#include "SDL.h"
#include "Pong.h"
using namespace std;


class Game
{
public:
	Game();
	virtual ~Game();
	bool Initialize();			//ゲームを初期化する
	void RunLoop();				//ゲームオーバーまでゲームループを実行する
	void Shutdown();			//ゲームをシャットダウンする

protected:
	const int WIN_W = 1024;		// ゲーム画面横幅
	const int WIN_H = 768;		// ゲーム画面縦幅

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