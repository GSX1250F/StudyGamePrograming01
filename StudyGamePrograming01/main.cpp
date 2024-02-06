// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game2player.h"

int main(int argc, char** argv)
{
	// フレームの中身
	// 1.　入力があれば処理する
	// 2.　ゲームワールドを更新する。
	// 3.　出力する者を生成する。
	
	// 1.について：入力＝デバイスからの入力、インターネットからのデータなど、ゲームで扱う情報すべて
	// 2.について：ゲームに影響するしないに関わらず、オブジェクトをすべてたどり、必要に応じて更新する。
	// 3.について：出力＝グラフィック、音楽、振動、インターネットへのデータ
	// パックマンの場合
	// 1. ジョイスティックからの入力受付
	// 2. 各キャラの位置・状態を処理・計算・判定
	// 3. 画像出力、音楽出力

	//ゲームの初期化
	Game game;		

	//ゲームオーバーまでゲームをループ
	bool success = game.Initialize();	
	if (success)
	{
		game.RunLoop();
	}
	
	//ゲームを終了
	game.Shutdown();
	
	
	return 0;
}
