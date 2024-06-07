#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;
const int mWindowW = 1024;
const int mWindowH = 768;

Game::Game()
	:mWindow(nullptr)
	, mRenderer(nullptr)
	, mTicksCount(0)
	, mIsRunning(true)
	
{	

}

bool Game::Initialize()
{
	//SDLを初期化
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);	//ビデオサブシステムの初期化。返り値は整数。返り値が0でないときは初期化失敗
	
	if (sdlResult != 0)
	{
		SDL_Log("SDLの初期化に失敗しました: %s", SDL_GetError());
		return false;
	}

	// SDL Windowを作成
	mWindow = SDL_CreateWindow(
		"Game Programming in C++", // Windowタイトル
		100,	// windowの左上のX座標
		100,	// windowの左上のY座標
		mWindowW,	// windowの横幅
		mWindowH,	// windowの縦幅
		0		// フラグ (0 は何もセットしない)
	);
	
	if (!mWindow)
	{
		SDL_Log("window作成に失敗しました: %s", SDL_GetError());
		return false;
	}

	// SDLレンダラーを作成
	mRenderer = SDL_CreateRenderer(
		mWindow, // レンダラーの対象
		-1,		 // 通常は -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		SDL_Log("レンダラーの作成に失敗しました: %s", SDL_GetError());
		return false;
	}

	//パドル、ボールの位置・速さ・方向を初期化
	mPaddlePos.x = thickness;
	mPaddlePos.y = mWindowH / 2.0f;
	mBallPos.x = mWindowW / 2.0f;
	mBallPos.y = mWindowH / 2.0f;
	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;
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

void Game::ProcessInput()
{	// SDLのイベントを内部のキューで管理する。
	// イベントキューでは、1フレームに複数のイベントが入っている可能性があるので、
	// キューにあるすべてのイベントをループ処理する必要がある。
	SDL_Event event;
	while (SDL_PollEvent(&event))		// SDLキューにイベントがあればtrue
	{
		switch (event.type)
		{
			case SDL_QUIT:		// ユーザーがウィンドウを閉じようとした入力のイベント
				mIsRunning = false;		//ゲームを終了するフラグ
				break;
		}
	}

	// キーボードの状態を取得
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])		//ESCキーが押されたとき
	{
		mIsRunning = false;		//ゲームを終了するフラグ
	}

	// ↑↓キーでパドルの方向を決定
	mPaddleDir = 0;		//方向を初期化
	if (state[SDL_SCANCODE_UP])
	{
		mPaddleDir -= 1;		//上方向はy座標を減らす方向
	}
	if (state[SDL_SCANCODE_DOWN])
	{
		mPaddleDir += 1;		//下方向はy座標を増やす方向
	}
}

void Game::UpdateGame()
{
	//ゲーム時間の更新、制限など	
	//16ms経過までは待つ（フレーム制限）。約60fps
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// Delta timeを計算。単位は秒にする
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// 更新が遅すぎても最低のfpsを確保
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;		//50ms (20fps)
	}

	// 次のフレームのためtick countsを更新
	mTicksCount = SDL_GetTicks();

		// パドル位置の更新
	// Update paddle position based on direction
	if (mPaddleDir != 0)
	{
		mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;	//パドルのY座標を、300ピクセル/秒だけ増減
		// Make sure paddle doesn't move off screen!
		if (mPaddlePos.y < (paddleH / 2.0f + thickness))
		{
			mPaddlePos.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddlePos.y > (768.0f - paddleH / 2.0f - thickness))
		{
			mPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	// ボール位置の更新
	// Update ball position based on ball velocity
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	// Bounce if needed
	// Did we intersect with the paddle?
	float diff = mPaddlePos.y - mBallPos.y;
	// Take absolute value of difference
	diff = (diff > 0.0f) ? diff : -diff;
	if (
		// Our y-difference is small enough
		diff <= paddleH / 2.0f &&
		// We are in the correct x-position
		mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
		// The ball is moving to the left
		mBallVel.x < 0.0f)
	{
		mBallVel.x *= -1.0f;
	}
	// Did the ball go off the screen? (if so, end game)
	else if (mBallPos.x <= 0.0f)
	{
		mIsRunning = false;
	}
	// Did the ball collide with the right wall?
	else if (mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f)
	{
		mBallVel.x *= -1.0f;
	}

	// Did the ball collide with the top wall?
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
	{
		mBallVel.y *= -1;
	}
	// Did the ball collide with the bottom wall?
	else if (mBallPos.y >= (768 - thickness) &&
		mBallVel.y > 0.0f)
	{
		mBallVel.y *= -1;
	}
}

void Game::GenerateOutput()
{
	//背景の色を設定
	SDL_SetRenderDrawColor(
		mRenderer,
		50,		// R
		50,		// G 
		50,		// B
		255		// A
	);
	// 背景を単色でクリア
	SDL_RenderClear(mRenderer);

	// 壁の描画
	// 壁の色を設定
	SDL_SetRenderDrawColor(mRenderer, 200, 200, 200, 255);
	// 上側の壁を描画
	// SDL_Rect : 長方形を作成
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		1024,		// Width
		thickness	// Height
	};
	SDL_RenderFillRect(mRenderer, &wall);	//作成した長方形を描画（塗りつぶし）
	// 下側の壁を描画
	wall.y = 768 - thickness;		
	SDL_RenderFillRect(mRenderer, &wall);	//作成した長方形を描画（塗りつぶし）	
	// 右側の壁を描画
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 1024;
	SDL_RenderFillRect(mRenderer, &wall);


	// パドルを描画
	SDL_Rect paddle{
		// static_cast演算子は、floatからintに変換する
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	//パドルの色を設定
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		0,		// G 
		200,		// B
		255		// A
	);
	SDL_RenderFillRect(mRenderer, &paddle);

	// ボールの描画
	SDL_Rect ball{
		// static_cast演算子は、floatからintに変換する
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};
	//ボールの色を設定
	SDL_SetRenderDrawColor(
		mRenderer,
		200,		// R
		0,		// G 
		0,		// B
		255		// A
	);
	SDL_RenderFillRect(mRenderer, &ball);

	// 描画バッファの交換
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);	// SDL_Rendererを破棄
	SDL_DestroyWindow(mWindow);		// SDL_Windowを破棄
	SDL_Quit();						// SDL終了
}