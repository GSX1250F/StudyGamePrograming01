#include "Game2player.h"
// 課題1.1　２人のプレイヤーバージョン

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
{
	mWindow = nullptr;
	mRenderer = nullptr;
	mTicksCount = 0;
	mIsRunning = true;
}

bool Game::Initialize()
{
	// Initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);	//ビデオサブシステムの初期化。返り値は整数。返り値が0でないときは初期化失敗
	// SDL_INIT_AUDIO	:	オーディオデバイスの管理、再生、録音
	// SDL_INIT_HAPTIC	:	フォースフィードバック（振動など）のサブシステム
	// SDL_INIT_GAMECONTROLLER	:	コントローラ入力デバイスのサブシステム

	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Create an SDL Window
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)", // Window title
		100,	// Top left x-coordinate of window
		100,	// Top left y-coordinate of window
		1024,	// Width of window
		768,	// Height of window
		0		// Flags (0 for no flags set)
	);
	// SDL_WINDOW_FULLSCREEN : フルスクリーンモード使用
	// SDL_WINDOW_FULSCREEN_DESKTOP : 現在のデスクトップの解像度でフルスクリーンモードを使用
	// SDL_WINDOW_OPENGL : OpenGLグラフィックスライブラリを使用
	// SDL_WINDOW_RESIZABLE : ユーザーがウィンドウの大きさを変えられる。

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	//// Create SDL renderer
	// ラスターグラフィックス：ディスプレイはピクセルの2次元格子で構成されている。
	// カラーバッファという2次元配列に画面全体の色情報が置かれる。
	// ※フレームバッファはカラーバッファ・デプスバッファ・ステンシルバッファなどの組合せの総称。

	// ダブルバッファ技術
	// カラーバッファを２つ用意する。ゲームは１つのバッファに書き込み、ディスプレイはもう１つのバッファを表示する。
	// ディスプレイが表示を終えると、２つのバッファは交換される。
	// ディスプレイが表示を終えて、バッファが交換されるサイクルを　vsync（垂直同期）　という。
	// 2Dグラフィックス描画用の関数群を　SDL_Renderer　で利用する。
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for
		-1,		 // Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		//レンダラーの初期化に失敗したとき
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	//ボールの位置・速さ・方向を初期化
	Ball ball1_init = { {1024.0f / 2.0f , 768.0f / 3.0f } , { -200.0f , 235.0f} };	//ボール1の初期情報
	Ball ball2_init = { {1024.0f / 2.0f , 768.0f * 2.0f / 3.0f } , { 200.0f , 235.0f} };	//ボール2の初期情報
	//mBalls配列にボール1,2のVector2配列をpush backで追加
	mBalls.push_back(ball1_init);
	mBalls.push_back(ball2_init);
	
	//パドルの位置・方向を初期化
	mPaddlesPos.push_back({ 10.0f , 768.0f / 2.0f });	//パドル1の初期位置
	mPaddlesPos.push_back({ 1000.0f , 768.0f / 2.0f });	//パドル2の初期位置
	mPaddlesDir.push_back(0);	//パドル1の初期方向
	mPaddlesDir.push_back(0);	//パドル2の初期方向
	
	
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
			// If we get an SDL_QUIT event, end loop
		case SDL_QUIT:		// ユーザーがウィンドウを閉じようとした入力のイベント
			mIsRunning = false;		//ゲームを終了するフラグ
			break;
		}
	}

	// Get state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// If escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE])		//ESCキーが押されたとき
	{
		mIsRunning = false;		//ゲームを終了するフラグ
	}

	// パドル1の方向
	mPaddlesDir[0] = 0;
	if (state[SDL_SCANCODE_Q])		//　Qキーで上方向
	{
		mPaddlesDir[0] -= 1;
	}
	if (state[SDL_SCANCODE_A])		//　Aキーで下方向
	{
		mPaddlesDir[0] += 1;
	}

	// パドル2の方向
	mPaddlesDir[1] = 0;
	if (state[SDL_SCANCODE_UP])		//　↑キーで上方向
	{
		mPaddlesDir[1] -= 1;
	}
	if (state[SDL_SCANCODE_DOWN])		//　↓キーで上方向
	{
		mPaddlesDir[1] += 1;
	}
}

void Game::UpdateGame()
{
	//ゲーム時間の更新、制限など	
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// Delta time is the difference in ticks from last frame
	// (converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// Update tick counts (for next frame)
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

	// Update paddle2 position based on direction
	if (mPaddle2Dir != 0)
	{
		mPaddle2Pos.y += mPaddle2Dir * 300.0f * deltaTime;	//パドルのY座標を、300ピクセル/秒だけ増減
		// Make sure paddle doesn't move off screen!
		if (mPaddle2Pos.y < (paddleH / 2.0f + thickness))
		{
			mPaddle2Pos.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddle2Pos.y > (768.0f - paddleH / 2.0f - thickness))
		{
			mPaddle2Pos.y = 768.0f - paddleH / 2.0f - thickness;
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
	if (mBallPos.x <= 0.0f || mBallPos.x >= 1024.0f)
	{
		mIsRunning = false;
	}
	//
	// Did the ball collide with the right wall?
	//else if (mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f)
	//{
	//	mBallVel.x *= -1.0f;
	//}

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

	// 2プレイヤーでの玉の跳ね返り
	// Did we intersect with the paddle2?
	float diff2 = mPaddle2Pos.y - mBallPos.y;
	// Take absolute value of difference
	diff2 = (diff2 > 0.0f) ? diff2 : -diff2;
	if (
		// Our y-difference is small enough
		diff2 <= paddleH / 2.0f &&
		// We are in the correct x-position
		mBallPos.x >= ( 1024.0f - 25.0f ) && mBallPos.x <= ( 1024.0f - 20.0f ) &&
		// The ball is moving to the right
		mBallVel.x > 0.0f)
	{
		mBallVel.x *= -1.0f;
	}

	
}

void Game::GenerateOutput()
{
	//背景の色を設定
	SDL_SetRenderDrawColor(
		mRenderer,
		150,		// R
		150,		// G 
		150,		// B
		255			// A
	);
	// 背景を単色でクリア
	SDL_RenderClear(mRenderer);

	// 壁の描画
	// 壁の色を設定
	SDL_SetRenderDrawColor(mRenderer, 220, 220, 0, 255);
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
	// 右側の壁を描画　※2人プレイヤーでは不要
	//wall.x = 1024 - thickness;
	//wall.y = 0;
	//wall.w = thickness;
	//wall.h = 1024;
	//SDL_RenderFillRect(mRenderer, &wall);


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

	// パドル2を描画
	SDL_Rect paddle2{
		// static_cast演算子は、floatからintに変換する
		static_cast<int>(mPaddle2Pos.x),
		static_cast<int>(mPaddle2Pos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	//パドルの色を設定
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		200,		// G 
		0,		// B
		255		// A
	);
	SDL_RenderFillRect(mRenderer, &paddle2);

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
