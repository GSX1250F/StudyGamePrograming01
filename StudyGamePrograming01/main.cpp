
#include "GameVer2.h"

int main(int argc, char** argv)
{
	// �t���[���̒��g
	// 1.�@���͂�����Ώ�������
	// 2.�@�Q�[�����[���h���X�V����B
	// 3.�@�o�͂���҂𐶐�����B
	
	// 1.�ɂ��āF���́��f�o�C�X����̓��́A�C���^�[�l�b�g����̃f�[�^�ȂǁA�Q�[���ň�����񂷂ׂ�
	// 2.�ɂ��āF�Q�[���ɉe�����邵�Ȃ��Ɋւ�炸�A�I�u�W�F�N�g�����ׂĂ��ǂ�A�K�v�ɉ����čX�V����B
	// 3.�ɂ��āF�o�́��O���t�B�b�N�A���y�A�U���A�C���^�[�l�b�g�ւ̃f�[�^
	// �p�b�N�}���̏ꍇ
	// 1. �W���C�X�e�B�b�N����̓��͎�t
	// 2. �e�L�����̈ʒu�E��Ԃ������E�v�Z�E����
	// 3. �摜�o�́A���y�o��

	//�Q�[���̏�����
	Game game;		
	
	//�Q�[���I�[�o�[�܂ŃQ�[�������[�v
	bool success = game.Initialize();	
	if (success)
	{
		game.RunLoop();
	}
	
	//�Q�[�����I��
	game.Shutdown();
	
	
	return 0;
}
