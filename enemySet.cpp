
#include "enemy.h"
#include "enemyLinerData.h"


//���F�u�h�E���ۗp�̃Z�b�g�֐�
//����1:���t���[���̎��ɏo���̂��A�̗́A�U���́A�h���
void SetGrape(float time, int life, int power, int deffend)
{
	ENEMY *g_Enemy = GetEnemy();
	EnemyParts *g_Parts = GetEnemyParts();
	LoadModel(MODEL_GRAPE, &g_Enemy[GetEnemyNum()].model);
	// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
	GetModelDiffuse(&g_Enemy[GetEnemyNum()].model, &g_Enemy[GetEnemyNum()].diffuse[0]);
	g_Enemy[GetEnemyNum()].load = TRUE;

	g_Enemy[GetEnemyNum()].pos = { moveTbl[0].start.x, 0.0f, moveTbl[0].start.z };
	g_Enemy[GetEnemyNum()].rot = { 0.0f, 0.0f, 0.0f };
	g_Enemy[GetEnemyNum()].scl = { 0.8f, 1.0f, 1.0f };

	g_Enemy[GetEnemyNum()].size = ENEMY_SIZE;	// �����蔻��̑傫��
	g_Enemy[GetEnemyNum()].life = life;
	g_Enemy[GetEnemyNum()].lifeMax = g_Enemy[GetEnemyNum()].life;
	g_Enemy[GetEnemyNum()].spd = 1.0f;
	g_Enemy[GetEnemyNum()].use = FALSE;
	g_Enemy[GetEnemyNum()].attack = FALSE;
	g_Enemy[GetEnemyNum()].attackUse = FALSE;
	g_Enemy[GetEnemyNum()].atInterval = 180;
	g_Enemy[GetEnemyNum()].atFrame = 20;
	g_Enemy[GetEnemyNum()].atFrameCount = 0;
	g_Enemy[GetEnemyNum()].power = power;
	g_Enemy[GetEnemyNum()].diffend = deffend;
	g_Enemy[GetEnemyNum()].target = NULL;
	g_Enemy[GetEnemyNum()].atCount = g_Enemy[GetEnemyNum()].atInterval;	//�ŏ��̓m�[�^�C���ōU�����[�V������
	g_Enemy[GetEnemyNum()].type = Proximity;;
	g_Enemy[GetEnemyNum()].start = time;
	g_Enemy[GetEnemyNum()].moveData = moveTbl;
	g_Enemy[GetEnemyNum()].moveCount = 0.0f;
	g_Enemy[GetEnemyNum()].moveTblSize = sizeof(moveTbl) / sizeof(MOVERINE);
	g_Enemy[GetEnemyNum()].nowTbl = 0;
	g_Enemy[GetEnemyNum()].tbl_adrA = grape_Attack;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
	g_Enemy[GetEnemyNum()].tbl_adrM = grape_Move;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
	g_Enemy[GetEnemyNum()].tbl_sizeA = sizeof(grape_Attack) / sizeof(INTERPOLATION_DATA);	// �o�^�����e�[�u���̃��R�[�h����
	g_Enemy[GetEnemyNum()].tbl_sizeM = sizeof(grape_Move) / sizeof(INTERPOLATION_DATA);	// �o�^�����e�[�u���̃��R�[�h����
	g_Enemy[GetEnemyNum()].move_time = 0.0f;	// ���s����
	g_Enemy[GetEnemyNum()].partsNum = 3;
	g_Enemy[GetEnemyNum()].startNum = GetEnemyPartsNum();
	// �K�w�A�j���[�V�����p�̏���������
	g_Enemy[GetEnemyNum()].parent = NULL;			// �{�́i�e�j�Ȃ̂�NULL������
	{
		LoadModel(MODEL_GRAPE_PARTS001, &g_Parts[GetEnemyPartsNum()].model);
		// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		GetModelDiffuse(&g_Parts[GetEnemyPartsNum()].model, &g_Parts[GetEnemyPartsNum()].diffuse[0]);
		g_Parts[GetEnemyPartsNum()].load = TRUE;

		g_Parts[GetEnemyPartsNum()].pos = { 0.0f, 0.0f, 0.0f };		// �|���S���̈ʒu
		g_Parts[GetEnemyPartsNum()].rot = { 0.0f, 0.0f, 0.0f };		// �|���S���̌���(��])
		g_Parts[GetEnemyPartsNum()].scl = { 1.0f, 1.0f, 1.0f };		// �|���S���̑傫��(�X�P�[��)

		// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
		g_Parts[GetEnemyPartsNum()].tbl_adrA = grape_Parts001Attack;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
		g_Parts[GetEnemyPartsNum()].tbl_adrM = grape_Parts001Move;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
		g_Parts[GetEnemyPartsNum()].tbl_sizeA = sizeof(grape_Parts001Attack) / sizeof(INTERPOLATION_DATA);	// �o�^�����e�[�u���̃��R�[�h����
		g_Parts[GetEnemyPartsNum()].tbl_sizeM = sizeof(grape_Parts001Move) / sizeof(INTERPOLATION_DATA);	// �o�^�����e�[�u���̃��R�[�h����
		g_Parts[GetEnemyPartsNum()].move_time = 0;	// ���s����
		g_Parts[GetEnemyPartsNum()].parent = &g_Enemy[GetEnemyNum()];	// �������e�Ȃ�NULL�A�������q���Ȃ�e��enemy�A�h���X
		SetEnemyPartsNum();
	}

	{
		LoadModel(MODEL_GRAPE_PARTS002, &g_Parts[GetEnemyPartsNum()].model);
		// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		GetModelDiffuse(&g_Parts[GetEnemyPartsNum()].model, &g_Parts[GetEnemyPartsNum()].diffuse[0]);
		g_Parts[GetEnemyPartsNum()].load = TRUE;

		g_Parts[GetEnemyPartsNum()].pos = { 0.0f, 0.0f, 0.0f };		// �|���S���̈ʒu
		g_Parts[GetEnemyPartsNum()].rot = { 0.0f, 0.0f, 0.0f };		// �|���S���̌���(��])
		g_Parts[GetEnemyPartsNum()].scl = { 1.0f, 1.0f, 1.0f };		// �|���S���̑傫��(�X�P�[��)
		// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
		g_Parts[GetEnemyPartsNum()].tbl_adrA = grape_Parts002Attack;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
		g_Parts[GetEnemyPartsNum()].tbl_adrM = grape_Parts002Move;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
		g_Parts[GetEnemyPartsNum()].tbl_sizeA = sizeof(grape_Parts002Attack) / sizeof(INTERPOLATION_DATA);	// �o�^�����e�[�u���̃��R�[�h����
		g_Parts[GetEnemyPartsNum()].tbl_sizeM = sizeof(grape_Parts002Move) / sizeof(INTERPOLATION_DATA);	// �o�^�����e�[�u���̃��R�[�h����
		g_Parts[GetEnemyPartsNum()].move_time = 0;	// ���s����
		g_Parts[GetEnemyPartsNum()].parent = &g_Enemy[GetEnemyNum()];	// �������e�Ȃ�NULL�A�������q���Ȃ�e��enemy�A�h���X
		SetEnemyPartsNum();
	}
	{
		LoadModel(MODEL_GRAPE_PARTS003, &g_Parts[GetEnemyPartsNum()].model);
		// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		GetModelDiffuse(&g_Parts[GetEnemyPartsNum()].model, &g_Parts[GetEnemyPartsNum()].diffuse[0]);
		g_Parts[GetEnemyPartsNum()].load = TRUE;

		g_Parts[GetEnemyPartsNum()].pos = { 0.0f, 0.0f, 0.0f };		// �|���S���̈ʒu
		g_Parts[GetEnemyPartsNum()].rot = { 0.0f, 0.0f, 0.0f };		// �|���S���̌���(��])
		g_Parts[GetEnemyPartsNum()].scl = { 1.0f, 1.0f, 1.0f };		// �|���S���̑傫��(�X�P�[��)

			// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
		g_Parts[GetEnemyPartsNum()].tbl_adrA = grape_Parts003Attack;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
		g_Parts[GetEnemyPartsNum()].tbl_adrM = grape_Parts003Move;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
		g_Parts[GetEnemyPartsNum()].tbl_sizeA = sizeof(grape_Parts003Attack) / sizeof(INTERPOLATION_DATA);	// �o�^�����e�[�u���̃��R�[�h����
		g_Parts[GetEnemyPartsNum()].tbl_sizeM = sizeof(grape_Parts003Move) / sizeof(INTERPOLATION_DATA);	// �o�^�����e�[�u���̃��R�[�h����
		g_Parts[GetEnemyPartsNum()].move_time = 0;	// ���s����
		g_Parts[GetEnemyPartsNum()].parent = &g_Enemy[GetEnemyNum()];	// �������e�Ȃ�NULL�A�������q���Ȃ�e��enemy�A�h���X
		SetEnemyPartsNum();
	}
	SetEnemyNum();
}
