#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>



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
	mPaddlePos.x = thickness * 2.0f;
	mPaddlePos.y = mWindowH / 2.0f;
	mBallPos.x = mWindowW / 2.0f;
	mBallPos.y = mWindowH / 2.0f;
	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;

	// パドルのスプライト用画像を読み込み
	paddleImage = nullptr;
	std::string filename = "Assets/paddle.png";
	SDL_Surface* surf = IMG_Load(filename.c_str());
	if (!surf)
	{
		SDL_Log("テクスチャファイルの読み込みに失敗しました %s", filename.c_str());
		return false;
	}
	paddleImage = SDL_CreateTextureFromSurface(mRenderer, surf);
	if (!paddleImage)
	{
		SDL_Log("サーフェイスからテクスチャの作成に失敗しました %s", filename.c_str());
		return false;
	}
	SDL_FreeSurface(surf);

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
	mTextSize.push_back(Vector2{iw*1.0f, ih*1.0f});		//mTextSize[0]がポーズのテキストサイズ
	mTextPos.push_back(Vector2{ mWindowW / 2.0f - iw / 2.0f, mWindowH / 2.0f - ih / 2.0f });		//mTextOffset[0]がポーズのテキスト表示位置
	SDL_FreeSurface(surf);
	// GameOverのテキスト
	surf = TTF_RenderUTF8_Blended(font, "Game Over", {255,255,255,255});
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
	mTextPos.push_back(Vector2{ mWindowW / 2.0f - iw / 2.0f, mWindowH / 2.0f - ih + mFontSize});
	SDL_FreeSurface(surf);

	scene = 0;

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
			case SDL_KEYDOWN:	// キーが押されたとき
				//mPaddleDir = 0;		//パドル方向を初期化
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					mIsRunning = false;		//ゲームを終了するフラグ
					break;
				case SDLK_UP:
					if (event.key.repeat)	// 押したままのとき
					{
						mPaddleDir = -1;		//上方向はy座標を減らす方向
					}
					break;
				case SDLK_DOWN:
					if (event.key.repeat)	// 押したままのとき
					{
						mPaddleDir = 1;		//上方向はy座標を減らす方向
					}
					break;
				case SDLK_s:
					if (!event.key.repeat &&		// 押してから離されたとき
						scene == 0)
					{
						scene = 1;
					}
					break;
				case SDLK_r:
					if (scene == 2)
					{
						scene = 0;
					}
					break;
				}
		}
	}

	// キーボードの状態を取得
	/*
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
	if (state[SDL_SCANCODE_S])
	{
		switch (scene)
		{
			case 0:
				scene = 1;
				break;
			case 1:
				scene = 0;
				break;
		}
	}
	*/

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

	if (scene == 0) 
	{
		// パドル位置の更新
		if (mPaddleDir != 0)
		{
			mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;	//パドルのY座標を、300ピクセル/秒だけ増減
			if (mPaddlePos.y < (paddleH / 2.0f + thickness))
			{
				mPaddlePos.y = paddleH / 2.0f + thickness;
			}
			else if (mPaddlePos.y > (mWindowH - paddleH / 2.0f - thickness))
			{
				mPaddlePos.y = mWindowH - paddleH / 2.0f - thickness;
			}
		}

		// ボール位置の更新
		// 位置 += 速さ*deltaTime
		mBallPos.x += mBallVel.x * deltaTime;
		mBallPos.y += mBallVel.y * deltaTime;

		// パドルでボールを反射。
		float diff = mPaddlePos.y - mBallPos.y;
		diff = (diff > 0.0f) ? diff : -diff;		//絶対値にする
		if (

			diff <= paddleH / 2.0f &&		// y座標の差が十分に小さく
			mBallPos.x <= mPaddlePos.x + thickness && mBallPos.x >= mPaddlePos.x + thickness / 2.0f &&		// ボールのx座標がパドルの範囲内にあり
			mBallVel.x < 0.0f)		// ボールが左向きに動いている
		{
			mBallVel.x *= -1.1f;		// ボールスピードアップ
		}
		// ボールが左端にいってしまったらゲームオーバー。
		else if (mBallPos.x <= 0.0f)
		{
			scene = 2;
		}
		// ボールが右壁に当たったら反射
		else if (mBallPos.x >= (mWindowW - thickness) && mBallVel.x > 0.0f)
		{
			mBallVel.x *= -1.0f;
		}

		// ボールが上壁に当たったら反射
		if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
		{
			mBallVel.y *= -1;
		}
		// ボールが下壁に当たったら反射
		else if (mBallPos.y >= (mWindowH - thickness) &&
			mBallVel.y > 0.0f)
		{
			mBallVel.y *= -1;
		}
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
		mWindowW,	// Width
		thickness	// Height
	};
	SDL_RenderFillRect(mRenderer, &wall);	//作成した長方形を描画（塗りつぶし）
	// 下側の壁を描画
	wall.y = mWindowH - thickness;		
	SDL_RenderFillRect(mRenderer, &wall);	//作成した長方形を描画（塗りつぶし）	
	// 右側の壁を描画
	wall.x = mWindowW - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = mWindowW;
	SDL_RenderFillRect(mRenderer, &wall);

	// パドルを描画
	/*
	SDL_Rect paddle{
		// static_cast演算子は、floatからintに変換する
		static_cast<int>(mPaddlePos.x - thickness / 2.0f),
		static_cast<int>(mPaddlePos.y - paddleH / 2.0f),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_SetRenderDrawColor(
		mRenderer,
		255,		// R
		255,		// G 
		255,		// B
		255			// A
	);
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
		// static_cast演算子は、floatからintに変換する
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_SetRenderDrawColor(
		mRenderer,
		255,		// R
		255,		// G 
		255,		// B
		255		// A
	);
	SDL_RenderFillRect(mRenderer, &ball);

	// テキスト表示
	for (int i = 0; i < mText.size(); i++) {
		SDL_Rect txtRect = { 0,0,static_cast<int>(mTextSize[i].x),static_cast<int>(mTextSize[i].y)};
		SDL_Rect pasteRect = { static_cast<int>(mTextPos[i].x),static_cast<int>(mTextPos[i].y),static_cast<int>(mTextSize[i].x),static_cast<int>(mTextSize[i].y) };
		//Textureを描写する      
		//描写元の描写する部分,描写先の描写する部分)      
		//サイズが違うと勝手にTextureを伸展してくれる      
		SDL_RenderCopy(mRenderer, mText[i], &txtRect, &pasteRect);
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