
#include "GameVer2.h"

int main(int argc, char** argv)
{
	Game game;		//�Q�[�����N���X��
	
	bool success = game.Initialize();			//�Q�[����������
	if (success)
	{
		game.RunLoop();			//�Q�[���I�[�o�[�܂ŃQ�[�������[�v
	}
	
	game.Shutdown();		//�Q�[�����I��
	
	return 0;
}
