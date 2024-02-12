#include "GameVer2.h"
// ゲーム内容、ゲームで動かすもの（ボール、パドル、壁）はすべてクラス化する。
// ゲームを動かす全体だけを実装

Game::Game()
{
	mWindow = nullptr;
	mRenderer = nullptr;
	mTicksCount = 0;
	mIsRunning = true;
	pong = new Pong();			//ゲーム内容をクラス化
}

bool Game::Initialize()			//ゲーム初期設定。ウィンドウ作成、レンダラー初期化
{
	// SDL初期化
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0) {
		SDL_Log("SDLを初期化できません : %s", SDL_GetError());
		return false;
	}

	// SDLウィンドウを作成
	mWindow = SDL_CreateWindow
	(
		"Pong in C++", // ウィンドウのタイトル
		100, // ウィンドウ左上隅のx座標
		100, // ウィンドウ左上隅のy座標
		WIN_W, // ウィンドウの幅(width)		Game.hで定義
		WIN_H, // ウィンドウの高さ(height)		Game.hで定義
		0 // フラグ(設定しない時は0)
	);

	if (!mWindow)
	{
		SDL_Log("ウィンドウの作成に失敗しました : %s", SDL_GetError());
		return false;
	}

	// SDLレンダラーを作成
	mRenderer = SDL_CreateRenderer
	(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		//レンダラーの初期化に失敗したとき
		SDL_Log("レンダラーの初期化に失敗しました　: %s", SDL_GetError());
		return false;
	}

	return true;	//初期化完了でtrueを返す。
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

void Game::ProcessInput()	//入力受付
{	
	SDL_Event event;
	// キューにイベントがあれば繰り返す
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:		// ユーザーがウィンドウを閉じようとした入力のイベント
			mIsRunning = false;
			break;			//ゲームを終了するフラグ
		default:
			break;
		}
	}
	const Uint8* state = SDL_GetKeyboardState(NULL);	//キーボード入力

	if (state[SDL_SCANCODE_ESCAPE])
	{
		//ESCでゲーム終了
		mIsRunning = false;
	}
	pong->input(state);			//ゲーム詳細をクラス化して、キーボード入力を引数で渡す。
}

void Game::UpdateGame()			//更新
{
	//前のフレームから16ms(約60fps)が経過するまで待つ
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// deltatimeは前のフレームとの時刻の差を秒に変換した値
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	//deltatimeを最大値で制限する
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;		//50ms(20fps)
	}
	//パドル用
	mTicksCount = SDL_GetTicks();
	pong->update(deltaTime,mIsRunning);		//ゲーム詳細をクラス化して、ゲーム経過時間を引数で渡し、ゲーム内容を処理する。
}

void Game::GenerateOutput()			//画面を更新
{
	// 背景
	// 描画色を黒色に設定
	SDL_SetRenderDrawColor
	(
		mRenderer,
		0, 0, 0, 0	 // (R,G,B,A)
	);
	// バックバッファを黒色で塗りつぶす
	SDL_RenderClear(mRenderer);

	// 描画色を白色に設定
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);


	pong->render(mRenderer);		//ゲーム詳細をクラス化して、レンダラー情報を引数で渡し、描画内容を処理する。


	// バックバッファとフロントバッファを入れ替え
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown() {
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
	delete pong;	//メモリ解放
}
