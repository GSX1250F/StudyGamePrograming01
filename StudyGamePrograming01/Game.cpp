#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <SDL_ttf.h>
#include "Random.hpp"
#include <math.h>

Game::Game()
	:mWindow(nullptr)
	, mIsRunning(true)
	, mRenderer(nullptr)
	, mTicksCount(0)
{}

bool Game::Initialize()
{
	//SDLを初期化
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);	//ビデオサブシステムの初期化。
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
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!mRenderer)
	{
		SDL_Log("レンダラーの作成に失敗しました: %s", SDL_GetError());
		return false;
	}

	// パドルのスプライト用画像を読み込み
	paddleImage = nullptr;		//テクスチャ用ポインタ変数を初期化
	std::string filename = "Assets/paddle.png";		//ファイル名（パス含む）
	SDL_Surface* surf = IMG_Load(filename.c_str());		//サーフェイスに画像を読み込む
	if (!surf)
	{
		SDL_Log("テクスチャファイルの読み込みに失敗しました %s", filename.c_str());
		return false;
	}
	paddleImage = SDL_CreateTextureFromSurface(mRenderer, surf);	//サーフェイスからテクスチャを作成
	if (!paddleImage)
	{
		SDL_Log("サーフェイスからテクスチャの作成に失敗しました %s", filename.c_str());
		return false;
	}
	SDL_FreeSurface(surf);	//サーフェイスをクリアする。

	// テキスト表示を用意
	if (TTF_Init() < 0)
	{
		SDL_Log("TTFの初期化に失敗しました %s", filename.c_str());
		return false;
	}
	TTF_Font* font = TTF_OpenFont("Assets/PixelMplus10-Regular.ttf", mFontSize);	// フォントファイルの読み込みとサイズ設定
	// ポーズのテキスト
	surf = TTF_RenderUTF8_Blended(font, "Press S to Pause", { 255,255,255,255 });
	mText.push_back(SDL_CreateTextureFromSurface(mRenderer, surf));		//mText[0]がポーズのテキスト
	int iw, ih;
	SDL_QueryTexture(mText[0], NULL, NULL, &iw, &ih);	//文字を描写したTextureのサイズを取得する
	mTextSize.push_back(Vector2{ iw * 1.0f, ih * 1.0f });		//mTextSize[0]がポーズのテキストサイズ
	mTextPos.push_back(Vector2{ mWindowW / 2.0f - iw / 2.0f, mWindowH / 2.0f - ih / 2.0f });		//mTextOffset[0]がポーズのテキスト表示位置
	SDL_FreeSurface(surf);
	// GameOverのテキスト
	surf = TTF_RenderUTF8_Blended(font, "Game Over", { 255,255,255,255 });
	mText.push_back(SDL_CreateTextureFromSurface(mRenderer, surf));
	SDL_QueryTexture(mText[1], NULL, NULL, &iw, &ih);
	mTextSize.push_back(Vector2{ iw * 1.0f, ih * 1.0f });
	mTextPos.push_back(Vector2{ mWindowW / 2.0f - iw / 2.0f, mWindowH / 2.0f - ih });
	SDL_FreeSurface(surf);
	// リスタートのテキスト
	surf = TTF_RenderUTF8_Blended(font, "Press R to restart", { 255,255,255,255 });
	mText.push_back(SDL_CreateTextureFromSurface(mRenderer, surf));
	SDL_QueryTexture(mText[2], NULL, NULL, &iw, &ih);
	mTextSize.push_back(Vector2{ iw * 1.0f, ih * 1.0f });
	mTextPos.push_back(Vector2{ mWindowW / 2.0f - iw / 2.0f, mWindowH / 2.0f - ih + mFontSize });
	SDL_FreeSurface(surf);

	ResetGame();

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
{
	SDL_Event event;
	while (SDL_PollEvent(&event))		// SDLキューにイベントがあればtrue
	{
		if (event.type == SDL_QUIT)
		{
			// ユーザーがウィンドウを閉じようとした入力のイベント
			mIsRunning = false;		//ゲームを終了するフラグ
		}
		if (event.type == SDL_KEYDOWN)	// キーが押されたら実行する
		{
			if (event.key.repeat)
			{
				switch (event.key.keysym.sym)	// キーリピート中
				{
				}
			}
			else
			{
				switch (event.key.keysym.sym)	// キーリピートされていないとき
				{
				case SDLK_ESCAPE:
					mIsRunning = false;		//ゲームを終了するフラグ
					break;
				case SDLK_UP:
					mPaddleDir = -1;
					break;
				case SDLK_DOWN:
					mPaddleDir = 1;
					break;
				case SDLK_s:
					if (scene == 0 && pause == false)
					{
						scene = 1;
					}
					if (scene == 1 && pause == true)
					{
						scene = 0;
						pause = false;
					}
					break;
				case SDLK_r:
					if (scene == 2) { ResetGame(); }
					break;
				}
			}

		}
		if (event.type == SDL_KEYUP)	// キーが離されたら実行する
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_UP:
				mPaddleDir = 0;
				break;
			case SDLK_DOWN:
				mPaddleDir = 0;
				break;
			case SDLK_s:
				if (scene == 1)
				{
					pause = true;
				}
				break;
			}
		}
	}
}

void Game::UpdateGame()
{
	//ゲーム時間の更新、制限など	
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));	//16ms経過までは待つ（フレーム制限）。約60fps
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;		// deltaTimeを計算。単位は秒にする
	if (deltaTime > 0.05f) { deltaTime = 0.05f; }		// 更新が遅すぎても最低のfpsを確保。50ms (20fps)
	mTicksCount = SDL_GetTicks();		// 次のフレームのためtick countsを更新

	if (scene == 0)
	{
		// パドル位置の更新
		if (mPaddleDir != 0)
		{
			mPaddlePos.y += mPaddleDir * mPaddleSpeed * deltaTime;
		}
		if (mPaddlePos.y < (paddleH / 2.0f + thickness))
		{
			mPaddlePos.y = paddleH / 2.0f + thickness;
		}
		if (mPaddlePos.y > (mWindowH - paddleH / 2.0f - thickness))
		{
			mPaddlePos.y = mWindowH - paddleH / 2.0f - thickness;
		}

		// ボール位置の更新
		mBallPos.x += mBallVel.x * deltaTime;
		mBallPos.y += mBallVel.y * deltaTime;
		// ボールが壁に当たったら跳ね返る
		if (mBallPos.x >= (mWindowW - thickness) && mBallVel.x > 0.0f)
		{
			mBallVel.x *= -1.0f;
		}
		if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
		{
			mBallVel.y *= -1.0f;
		}
		if (mBallPos.y >= (mWindowH - thickness) && mBallVel.y > 0.0f)
		{
			mBallVel.y *= -1.0f;
		}
		// パドルでボールが跳ね返る
		float diff = mPaddlePos.y - mBallPos.y;
		diff = (diff > 0.0f) ? diff : -diff;		//絶対値にする
		if (diff <= paddleH / 2.0f &&		// y座標の差が十分に小さく
			mBallPos.x <= mPaddlePos.x + thickness && mBallPos.x >= mPaddlePos.x + thickness / 2.0f &&		// ボールのx座標がパドルの範囲内にあり
			mBallVel.x < 0.0f)		// ボールが左向きに動いている
		{
			mBallVel.x *= -1.1f;	// 横方向ボールスピードup
		}
		// ボールが左端にいってしまったらゲームオーバー。
		if (mBallPos.x <= 0.0f)
		{
			scene = 2;
		}
	}

}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 50, 50, 50, 255);		//背景の色を灰色に設定
	SDL_RenderClear(mRenderer);		// 背景を単色でクリア

	// 壁の描画
	SDL_SetRenderDrawColor(mRenderer, 200, 200, 200, 255);		// 壁の色を設定
	SDL_Rect wall{ 0,0,mWindowW,thickness };		// 上側の壁
	SDL_RenderFillRect(mRenderer, &wall);		//作成した長方形を描画（塗りつぶし）
	wall = { 0,mWindowH - thickness,mWindowW,thickness };		// 下側の壁を描画
	SDL_RenderFillRect(mRenderer, &wall);
	wall = { mWindowW - thickness,0,thickness,mWindowW };		// 右側の壁
	SDL_RenderFillRect(mRenderer, &wall);

	if (scene == 0 || scene == 1)		// ゲーム実行中またはゲームポーズ中
	{
		// パドルを描画
		/*SDL_Rect paddle{
			static_cast<int>(mPaddlePos.x - thickness / 2.0f),
			static_cast<int>(mPaddlePos.y - paddleH / 2.0f),
			thickness,
			static_cast<int>(paddleH)
		};
		SDL_SetRenderDrawColor(mRenderer,255,255,255,255);
		SDL_RenderFillRect(mRenderer, &paddle);
		*/
		if (paddleImage)
		{
			SDL_Rect r;
			r.w = static_cast<int>(thickness);
			r.h = static_cast<int>(paddleH);
			r.x = static_cast<int>(mPaddlePos.x - thickness / 2.0f);
			r.y = static_cast<int>(mPaddlePos.y - paddleH / 2.0f);
			SDL_RenderCopyEx(mRenderer, paddleImage, nullptr, &r, 0, nullptr, SDL_FLIP_NONE);
		}

		// ボールの描画
		SDL_Rect ball{
			static_cast<int>(mBallPos.x - thickness / 2),
			static_cast<int>(mBallPos.y - thickness / 2),
			thickness,
			thickness
		};
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
		SDL_RenderFillRect(mRenderer, &ball);

		// テキスト表示
		SDL_Rect txtRect = { 0,0,static_cast<int>(mTextSize[0].x),static_cast<int>(mTextSize[0].y) };
		SDL_Rect pasteRect = { static_cast<int>(mTextPos[0].x),static_cast<int>(mTextPos[0].y),static_cast<int>(mTextSize[0].x),static_cast<int>(mTextSize[0].y) };
		SDL_RenderCopy(mRenderer, mText[0], &txtRect, &pasteRect);
	}
	else
	{
		// ゲームオーバー中
		for (int i = 1; i <= 2; i++)
		{
			// テキスト表示
			SDL_Rect txtRect = { 0,0,static_cast<int>(mTextSize[i].x),static_cast<int>(mTextSize[i].y) };
			SDL_Rect pasteRect = { static_cast<int>(mTextPos[i].x),static_cast<int>(mTextPos[i].y),static_cast<int>(mTextSize[i].x),static_cast<int>(mTextSize[i].y) };
			SDL_RenderCopy(mRenderer, mText[i], &txtRect, &pasteRect);
		}
	}

	SDL_RenderPresent(mRenderer);		// 描画バッファの交換
}

void Game::ResetGame()
{
	// パドルとボール位置・速さ・方向をリセット
	mPaddlePos = Vector2{ thickness * 2.0f,mWindowH / 2.0f };
	mPaddleDir = 0;
	mPaddleSpeed = 200.0f;
	mBallPos = Vector2{ mWindowW / 2.0f,mWindowH / 2.0f };
	//mBallVel = Vector2{ -200.0f,235.0f };
	int angle = 1.0f * rnd(15, 75);	// 15～75の範囲の乱数を取得
	int pmx = 2 * rnd(0, 1) - 1;	// -1 , 1 のどちらかを乱数で取得
	int pmy = 2 * rnd(0, 1) - 1;	// -1 , 1 のどちらかを乱数で取得
	float velx = 1.0f * pmx * mWindowH * 0.4f * cos(angle / 180.0f * M_PI);
	float vely = 1.0f * pmy * mWindowH * 0.4f * sin(angle / 180.0f * M_PI);
	mBallVel = Vector2{velx , vely};

	scene = 0;
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);	// SDL_Rendererを破棄
	SDL_DestroyWindow(mWindow);		// SDL_Windowを破棄
	SDL_Quit();						// SDL終了
}