
#include "GameVer2.h"

int main(int argc, char** argv)
{
	Game game;		//ゲームをクラス化
	
	bool success = game.Initialize();			//ゲームを初期化
	if (success)
	{
		game.RunLoop();			//ゲームオーバーまでゲームをループ
	}
	
	game.Shutdown();		//ゲームを終了
	
	return 0;
}
