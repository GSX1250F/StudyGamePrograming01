#include "Game2player.h"
#include <iostream>
#include <random>
#include <math.h>

using namespace std;
// 課題1.1　２人のプレイヤーバージョン
// 課題1.2　２人＆２ボールのバージョン
// 課題1.3　プレイヤー数を選択制にする
// 課題1.4　ボール数を選択制にする
// 課題1.5　ボールの初期方向をランダムにする。
// 課題1.6　ボールがパドルでバウンドする毎に、スピードが少しずつ速くなる。（難易度少しずつアップ）
// 課題1.7　ボール2つ以上も可能


Game::Game()
{
	mWindow = nullptr;
	mRenderer = nullptr;
	mTicksCount = 0;
	mIsRunning = true;
	scene = 0;	//0はタイトル画面
	mPaddlesNum = 0;
	mBallsNum = 0;
	thickness = 15.0f;
	paddleH = 200.0f;
	mBallsAcc = 1.5f;

	while (mPaddlesNum == 0)
	{
		cout << "Number of players ? (1 or 2) : ";
		cin >> mPaddlesNum;
		static_cast<int>(mPaddlesNum);
		if (mPaddlesNum != 1 && mPaddlesNum != 2) 
		{
			cout << "incorrect number." << endl;
			mPaddlesNum = 0;
		}
	}
	while (mBallsNum <= 0)
	{
		cout << "Number of Balls ? (int) : ";
		cin >> mBallsNum;
		static_cast<int>(mBallsNum);
		if (mBallsNum <= 0)
		{
			cout << "incorrect number." << endl;
			mBallsNum = 0;
		}
	}
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
	double mBallsAngle = 0;		//ボール射出角度
	int mBallsDirx = -1;	//ボール射出方向正負x
	int mBallsDiry = -1;	//ボール射出方向正負y
	random_device rnd;		// 非決定的な乱数生成器を生成
	mt19937 mt(rnd());		//  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
	uniform_int_distribution<> rand(20, 61);	// 範囲指定一様乱数　射出角度
	uniform_int_distribution<> randx(0, 2);	// 範囲指定一様乱数
	uniform_int_distribution<> randy(0, 2);	// 範囲指定一様乱数
	for (int i = 0; i < mBallsNum; i++) {
		if (randx(mt) == 0) { mBallsDirx = -1; }
		else { mBallsDirx = 1; }
		if (randy(mt) == 0) { mBallsDiry = -1; }
		else { mBallsDiry = 1; }
		mBallsAngle = rand(mt) / 180.0f * M_PI;

		Ball ball_init = { {1024.0f / 2.0f , 768.0f * (i+1) / (mBallsNum+1) } , { static_cast<float>(mBallsDirx * 300.0f * cos(mBallsAngle)) , mBallsDiry * static_cast<float>(mBallsDiry * 300.0f * sin(mBallsAngle))} };	//ボールの初期情報
		//mBalls配列にボールのVector2配列をpush backで追加
		mBalls.push_back(ball_init);
	}
	//パドルの位置・方向を初期化
	mPaddlesPos.push_back({ 10.0f+thickness/2.0f , 768.0f / 2.0f });	//パドル0の初期位置
	mPaddlesPos.push_back({ 1024.0f - 10.0f -thickness/2.0f , 768.0f / 2.0f });	//パドル1の初期位置
	mPaddlesDir.push_back(0);	//パドル0の初期方向
	mPaddlesDir.push_back(0);	//パドル1の初期方向
	
	
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

	// パドル0の方向
	mPaddlesDir[0] = 0;
	if (state[SDL_SCANCODE_Q])		//　Qキーで上方向
	{
		mPaddlesDir[0] -= 1;
	}
	if (state[SDL_SCANCODE_A])		//　Aキーで下方向
	{
		mPaddlesDir[0] += 1;
	}

	// パドル1の方向
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
	if (deltaTime > 0.033f)
	{
		deltaTime = 0.033f;
	}

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();




	// パドル位置の更新
	// Update paddle position based on direction
	for (int i = 0; i < mPaddlesNum; i++) 
	{
		if (mPaddlesDir[i] != 0)
		{
			mPaddlesPos[i].y += mPaddlesDir[i] * 300.0f * deltaTime;	//パドルのY座標を、300ピクセル/秒だけ増減
			// Make sure paddle doesn't move off screen!
			if (mPaddlesPos[i].y < (paddleH / 2.0f + thickness))
			{
				mPaddlesPos[i].y = paddleH / 2.0f + thickness;
			}
			else if (mPaddlesPos[i].y >(768.0f - paddleH / 2.0f - thickness))
			{
				mPaddlesPos[i].y = 768.0f - paddleH / 2.0f - thickness;
			}
		}

	}

	// ボール位置の更新
	// Update ball position based on ball velocity
	for (int i = 0; i < mBallsNum; i++) 
	{
		mBalls[i].pos.x += mBalls[i].vel.x * deltaTime;
		mBalls[i].pos.y += mBalls[i].vel.y * deltaTime;
		
		// Did the ball go off the screen? (if so, end game)
		if (mBalls[i].pos.x <= 0.0f || mBalls[i].pos.x >= 1024.0f)
		{
			mIsRunning = false;
		}


		// ボール　壁での跳ね返り　
		// Did the ball collide with the top or bottom wall?
		if ( ( (mBalls[i].pos.y <= thickness*1.5f) && (mBalls[i].vel.y < 0.0f) )
			|| ((mBalls[i].pos.y >= (768.0f - thickness*1.5f)) && (mBalls[i].vel.y > 0.0f)))
		{
			mBalls[i].vel.y *= -1.0f;
		}
		// Did the ball collide with the right wall ? (1player)
		if (mPaddlesNum == 1) 
		{
			if ( (mBalls[i].pos.x >= 1024.0f - thickness*1.5f) &&
				 (mBalls[i].vel.x > 0.0f))
			{
				mBalls[i].vel.x *= -1.0f;
				

			}
		}


		//　ボール　パドルでの跳ね返り
		//　一度シンプルにする
		float diff[2] = { 0.0f,0.0f };
		diff[0] = mPaddlesPos[0].y - mBalls[i].pos.y;	//パドル0とボールのy方向距離
		diff[1] = mPaddlesPos[1].y - mBalls[i].pos.y;	//パドル1とボールのy方向距離
		// 絶対値をとる
		if (diff[0] < 0.0f) { diff[0] *= -1.0f; }
		if (diff[1] < 0.0f) { diff[1] *= -1.0f; }

		if (
			// ボールとパドルのy方向距離が十分に小さく
			diff[0] <= paddleH / 2.0f &&
			// ボールが正しいx位置にあり
			mBalls[i].pos.x <= mPaddlesPos[0].x + thickness * 1.5f && mBalls[i].pos.x >= mPaddlesPos[0].x &&
			// ボールが左向きに動いていれば
			mBalls[i].vel.x < 0.0f
			)
		{
			mBalls[i].vel.x *= -1.0f * mBallsAcc;
			
		}
		if (
			// ボールとパドルのy方向距離が十分に小さく
			diff[1] <= paddleH / 2.0f &&
			// ボールが正しいx位置にあり
			(mBalls[i].pos.x <= mPaddlesPos[1].x ) && (mBalls[i].pos.x >= mPaddlesPos[1].x - thickness * 1.5f) &&
			// ボールが右向きに動いていれば
			mBalls[i].vel.x > 0.0f
			)
		{
			mBalls[i].vel.x *= -1.0f * mBallsAcc;
		}
		if (mBalls[i].vel.x < -1000.0f) { mBalls[i].vel.x = -1000.0f; }
		else if (mBalls[i].vel.x > 1000.0f) { mBalls[i].vel.x = 1000.0f; }
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
	wall.y = static_cast<int>(768-thickness);
	SDL_RenderFillRect(mRenderer, &wall);	//作成した長方形を描画（塗りつぶし）	
	// 右側の壁を描画　※2人プレイヤーでは不要
	if (mPaddlesNum < 2) {
		wall.x = 1024 - static_cast<int>(thickness);
		wall.y = 0;
		wall.w = static_cast<int>(thickness);
		wall.h = 1024;
		SDL_RenderFillRect(mRenderer, &wall);
	}
	

	// パドルを描画
	for (int j = 0; j < mPaddlesNum; j++)
	{
		SDL_Rect paddle{
			// static_cast演算子は、floatからintに変換する
			static_cast<int>(mPaddlesPos[j].x - thickness/2.0f),
			static_cast<int>(mPaddlesPos[j].y - paddleH / 2.0f),
			thickness,
			static_cast<int>(paddleH)
		};
		//パドルの色を設定
		SDL_SetRenderDrawColor(
			mRenderer,
			0,		// R
			200*j,		// G 
			200*(1-j),	// B
			255		// A
		);
		SDL_RenderFillRect(mRenderer, &paddle);

	}

	// ボールの描画
	for (int i = 0; i < mBallsNum; i++)
	{
		SDL_Rect ball{
			// static_cast演算子は、floatからintに変換する
			static_cast<int>(mBalls[i].pos.x - thickness / 2.0f),
			static_cast<int>(mBalls[i].pos.y - thickness / 2.0f),
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
	}
	
	// 描画バッファの交換
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);	// SDL_Rendererを破棄
	SDL_DestroyWindow(mWindow);		// SDL_Windowを破棄
	SDL_Quit();						// SDL終了
}
