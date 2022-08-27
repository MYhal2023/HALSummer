//=============================================================================
//
// �Ґ����� [team.cpp]
// Author : 
//
//=============================================================================
#include "team.h"
#include "player.h"
#include "playerLinerData.h"
#include "sound.h"
#include <string>
static PlayerStatus g_Team[MAX_PLAYER_SET];
//�f��ID�A�K�v�ʁ~4(4��܂Ń��x���A�b�v���邩��)�̏��ԁB
static NeedMaterial Neutrovalue[MAX_MATERIAL] = { {(energy), {200, 400, 500, 800}},
										{(iron),{0, 1, 0, 1} },
										{(99), {0, 0, 0, 0}}/*�_�~�|�f�[�^ */ };

static PlayerPartsStatus g_Parts [ MAX_PLAYER_SET * 2];
static char name[2][64];
static int memberNum;
static int partsNum;
static BOOL g_Load = FALSE;

void InitTeam(void)
{
	strcpy(name[MainPlayer], MODEL_PLAYER);
	strcpy(name[Neutrophils], MODEL_NEUTROPHILS);
	memberNum = 0;
	partsNum = 0;
	for (int i = 0; i < MAX_PLAYER_SET; i++)
	{
		g_Team[i].use = FALSE;
	}
	g_Load = TRUE;
}

void UninitTeam(void)
{
	if (g_Load == FALSE) return;
	name[0][0] = '0';
	name[1][0] = '0';

	for (int i = 0; i < MAX_PLAYER_SET; i++)
	{
		// ���f���̉������
		if (g_Team[i].use)
		{
			UnloadModel(&g_Team[i].model);
			g_Team[i].use = FALSE;
		}
	}
	g_Load = FALSE;

}

//�����o�[�̃Z�b�g�֐�
//�f�o�b�O�p��
void SetMember(int i)
{
	LoadModel(MODEL_NEUTROPHILS, &g_Team[i].model);
	// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
	GetModelDiffuse(&g_Team[i].model, &g_Team[i].diffuse[0]);
	g_Team[i].use = TRUE;
	g_Team[i].charID = ID_Neutro;
	g_Team[i].charType = LowPlaces;
	g_Team[i].scl = { 1.0f, 1.0f, 1.0f };
	g_Team[i].size = 25.0f;	// �����蔻��̑傫��
	g_Team[i].life = 100;
	g_Team[i].blockMax = 2;
	g_Team[i].blockNum = 0;
	g_Team[i].startNum = GetPlayerPartsNum();
	g_Team[i].partsNum = 0;
	g_Team[i].atFrame = 50;
	//�A�j���[�V�����f�[�^�̃Z�b�g
	g_Team[i].tbl_adrA = neutro_Attack;	//�擪�A�h���X�̎w��Ȃ��߁A�Y������i
	g_Team[i].tbl_sizeA = sizeof(neutro_Attack)/sizeof(INTERPOLATION_DATA);	//�f�[�^�T�C�Y
	g_Team[i].tbl_adrM = neutro_Standby;	//�擪�A�h���X�̎w��Ȃ��߁A�Y������i
	g_Team[i].tbl_sizeM = sizeof(neutro_Standby) / sizeof(INTERPOLATION_DATA);;	//�f�[�^�T�C�Y
	//�p�[�c���̏���������
	for (int k = g_Team[i].startNum; k < g_Team[i].partsNum; k++)
	{
		//LoadModel(MODEL_GRAPE_PARTS, &g_Parts[partsNum].model);
		//// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		//GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);

		// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
		g_Parts[k].tbl_adrA = NULL;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
		g_Parts[k].tbl_sizeA = 0;	// �o�^�����e�[�u���̃��R�[�h����
		g_Parts[k].tbl_adrM = NULL;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
		g_Parts[k].tbl_sizeM = 0;	// �o�^�����e�[�u���̃��R�[�h����
		g_Parts[k].move_time = 0;	// ���s����
		SetPlayerNum(1);
	}
	memberNum++;
}

void SetChara(int charID)
{
}


PlayerStatus* GetTeam(void)
{
	return &g_Team[0];
}

PlayerPartsStatus* GetTeamParts(void)
{
	return &g_Parts[0];
}

//�f�o�b�O�p�B�D�������`�[�������o�[�Ƃ��ăZ�b�g
void SetNeutro(int i)
{
	LoadModel(MODEL_NEUTROPHILS, &g_Team[i].model);
	// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
	GetModelDiffuse(&g_Team[i].model, &g_Team[i].diffuse[0]);
	g_Team[i].use = TRUE;
	g_Team[i].charID = ID_Neutro;
	g_Team[i].charType = LowPlaces;
	g_Team[i].scl = { 1.0f, 1.0f, 1.0f };
	g_Team[i].size = PLAYER_SIZE;	// �����蔻��̑傫��
	g_Team[i].level = 1;
	int Life[MAX_LEVEL] = { 40, 45, 60, 65, 80 };
	int Power[MAX_LEVEL] = { 6, 7, 10, 12, 15 };
	int Diffend[MAX_LEVEL] = { 3, 4, 5, 7, 10 };
	int spMax[MAX_LEVEL] = { 10, 10, 8, 8, 8 };
	int cost[MAX_LEVEL] = { 8, 8, 7, 7, 6 };
	for (int k = 0; k < MAX_LEVEL; k++){
		g_Team[i].lifeMax[k] = Life[k];
		g_Team[i].power[k] = Power[k];
		g_Team[i].diffend[k] = Diffend[k];
		g_Team[i].spMax[k] = spMax[k];
		g_Team[i].cost[k] = cost[k];
	}
	g_Team[i].life = g_Team[i].lifeMax[0];
	g_Team[i].blockMax = 2;
	g_Team[i].blockNum = 0;
	g_Team[i].startNum = 0;
	g_Team[i].partsNum = 0;
	g_Team[i].atFrame = 45;
	g_Team[i].skillID = neutro_skill;
	g_Team[i].setAble = FALSE;
	g_Team[i].attackSE = SOUND_LABEL_SE_Panch;
	//�����K�v�f�ނ̕ۑ�
	g_Team[i].material = Neutrovalue;


	//�A�j���[�V�����f�[�^�̃Z�b�g
	g_Team[i].tbl_adrA = neutro_Attack;	//�擪�A�h���X�̎w��Ȃ��߁A�Y������i
	g_Team[i].tbl_sizeA = sizeof(neutro_Attack) / sizeof(INTERPOLATION_DATA);	//�f�[�^�T�C�Y
	g_Team[i].tbl_adrM = neutro_Standby;	//�擪�A�h���X�̎w��Ȃ��߁A�Y������i
	g_Team[i].tbl_sizeM = sizeof(neutro_Standby) / sizeof(INTERPOLATION_DATA);;	//�f�[�^�T�C�Y
	memberNum++;
}

//�f�o�b�O�p�B�}�N���t�@�[�W���`�[�������o�[�Ƃ��ăZ�b�g
void SetMacrophages(int i)
{
	LoadModel(MODEL_MACRO, &g_Team[i].model);
	// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
	GetModelDiffuse(&g_Team[i].model, &g_Team[i].diffuse[0]);
	g_Team[i].use = TRUE;
	g_Team[i].charID = ID_Macro;
	g_Team[i].charType = LowPlaces;
	g_Team[i].scl = { 1.0f, 1.0f, 1.0f };
	g_Team[i].size = PLAYER_SIZE;	// �����蔻��̑傫��
	g_Team[i].level = 1;
	int Life[MAX_LEVEL] = { 80, 95, 120, 130, 150 };
	int Power[MAX_LEVEL] = { 4, 4, 6, 7, 10 };
	int Diffend[MAX_LEVEL] = { 8, 10, 15, 17, 20 };
	int spMax[MAX_LEVEL] = { 20, 20, 18, 18, 18 };
	int cost[MAX_LEVEL] = { 15, 15, 15, 15, 13 };
	for (int k = 0; k < MAX_LEVEL; k++) {
		g_Team[i].lifeMax[k] = Life[k];
		g_Team[i].power[k] = Power[k];
		g_Team[i].diffend[k] = Diffend[k];
		g_Team[i].spMax[k] = spMax[k];
		g_Team[i].cost[k] = cost[k];
	}

	g_Team[i].life = g_Team[i].lifeMax[g_Team[i].level - 1];
	g_Team[i].blockMax = 2;
	g_Team[i].blockNum = 0;
	g_Team[i].startNum = partsNum;
	g_Team[i].partsNum = 2;
	g_Team[i].atFrame = 25;
	g_Team[i].setAble = FALSE;

	g_Team[i].material = Neutrovalue;
	g_Team[i].attackSE = SOUND_LABEL_SE_Kick;

	//�A�j���[�V�����f�[�^�̃Z�b�g
	g_Team[i].tbl_adrA = macro_Attack;	//�擪�A�h���X�̎w��Ȃ��߁A�Y������i
	g_Team[i].tbl_sizeA = sizeof(macro_Attack) / sizeof(INTERPOLATION_DATA);	//�f�[�^�T�C�Y
	g_Team[i].tbl_adrM = macro_Standby;	//�擪�A�h���X�̎w��Ȃ��߁A�Y������i
	g_Team[i].tbl_sizeM = sizeof(macro_Standby) / sizeof(INTERPOLATION_DATA);	//�f�[�^�T�C�Y
	
																				//�p�[�c���̏���������
	LoadModel(MODEL_MACRO_ARM, &g_Parts[partsNum].model);
	// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
	GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);

	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
	g_Parts[partsNum].tbl_adrA = macro_ArmAt;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
	g_Parts[partsNum].tbl_sizeA = sizeof(macro_ArmAt) / sizeof(INTERPOLATION_DATA);	// �o�^�����e�[�u���̃��R�[�h����
	g_Parts[partsNum].tbl_adrM = macro_ArmSb;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
	g_Parts[partsNum].tbl_sizeM = sizeof(macro_ArmSb) / sizeof(INTERPOLATION_DATA);	// �o�^�����e�[�u���̃��R�[�h����
	g_Parts[partsNum].move_time = 0;	// ���s����
	partsNum++;


	LoadModel(MODEL_MACRO_LEG, &g_Parts[partsNum].model);
	// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
	GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);

	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
	g_Parts[partsNum].tbl_adrA = macro_LegAt;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
	g_Parts[partsNum].tbl_sizeA = sizeof(macro_LegAt) / sizeof(INTERPOLATION_DATA);	// �o�^�����e�[�u���̃��R�[�h����
	g_Parts[partsNum].tbl_adrM = macro_LegSb;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
	g_Parts[partsNum].tbl_sizeM = sizeof(macro_LegSb) / sizeof(INTERPOLATION_DATA);	// �o�^�����e�[�u���̃��R�[�h����
	g_Parts[partsNum].move_time = 0;	// ���s����
	partsNum++;
	memberNum++;
}

//�f�o�b�O�p�B
void SetHelpeerT(int i)
{
	LoadModel(MODEL_HELPT, &g_Team[i].model);
	// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
	GetModelDiffuse(&g_Team[i].model, &g_Team[i].diffuse[0]);
	g_Team[i].use = TRUE;
	g_Team[i].charID = ID_HelperT;
	g_Team[i].charType = HighPlaces;
	g_Team[i].scl = { 1.0f, 1.0f, 1.0f };
	g_Team[i].size = HelperT_SIZE;	// �����蔻��̑傫��
	g_Team[i].level = 1;
	int Life[MAX_LEVEL] = { 80, 95, 120, 130, 150 };
	int Power[MAX_LEVEL] = { 4, 4, 6, 7, 10 };
	int Diffend[MAX_LEVEL] = { 8, 10, 15, 17, 20 };
	int spMax[MAX_LEVEL] = { 20, 20, 18, 18, 18 };
	int cost[MAX_LEVEL] = { 15, 15, 15, 15, 13 };
	for (int k = 0; k < MAX_LEVEL; k++) {
		g_Team[i].lifeMax[k] = Life[k];
		g_Team[i].power[k] = Power[k];
		g_Team[i].diffend[k] = Diffend[k];
		g_Team[i].spMax[k] = spMax[k];
		g_Team[i].cost[k] = cost[k];
	}

	g_Team[i].life = g_Team[i].lifeMax[g_Team[i].level - 1];
	g_Team[i].blockMax = 2;
	g_Team[i].blockNum = 0;
	g_Team[i].startNum = partsNum;
	g_Team[i].partsNum = 1;
	g_Team[i].atFrame = 20;
	g_Team[i].setAble = FALSE;

	g_Team[i].material = Neutrovalue;
	g_Team[i].attackSE = SOUND_LABEL_SE_Hit;

	//�A�j���[�V�����f�[�^�̃Z�b�g
	g_Team[i].tbl_adrA = helperT_Attack;	//�擪�A�h���X�̎w��Ȃ��߁A�Y������i
	g_Team[i].tbl_sizeA = sizeof(helperT_Attack) / sizeof(INTERPOLATION_DATA);	//�f�[�^�T�C�Y
	g_Team[i].tbl_adrM = helperT_Standby;	//�擪�A�h���X�̎w��Ȃ��߁A�Y������i
	g_Team[i].tbl_sizeM = sizeof(helperT_Standby) / sizeof(INTERPOLATION_DATA);	//�f�[�^�T�C�Y

																				//�p�[�c���̏���������
	LoadModel(MODEL_HELPTTOP, &g_Parts[partsNum].model);
	// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
	GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);

	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
	g_Parts[partsNum].tbl_adrA = helperT_TopAt;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
	g_Parts[partsNum].tbl_sizeA = sizeof(helperT_TopAt) / sizeof(INTERPOLATION_DATA);	// �o�^�����e�[�u���̃��R�[�h����
	g_Parts[partsNum].tbl_adrM = helperT_TopSb;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
	g_Parts[partsNum].tbl_sizeM = sizeof(helperT_TopSb) / sizeof(INTERPOLATION_DATA);	// �o�^�����e�[�u���̃��R�[�h����
	g_Parts[partsNum].move_time = 0;	// ���s����
	partsNum++;

	memberNum++;
}

//�f�o�b�O�p�B
void SetKillerT(int i)
{
	LoadModel(MODEL_KILLT, &g_Team[i].model);
	// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
	GetModelDiffuse(&g_Team[i].model, &g_Team[i].diffuse[0]);
	g_Team[i].use = TRUE;
	g_Team[i].charID = ID_KillerT;
	g_Team[i].charType = LowPlaces;
	g_Team[i].scl = { 1.2f, 1.2f, 1.2f };
	g_Team[i].size = PLAYER_SIZE;	// �����蔻��̑傫��
	g_Team[i].level = 1;
	int Life[MAX_LEVEL] = { 80, 95, 120, 130, 150 };
	int Power[MAX_LEVEL] = { 4, 4, 6, 7, 10 };
	int Diffend[MAX_LEVEL] = { 8, 10, 15, 17, 20 };
	int spMax[MAX_LEVEL] = { 20, 20, 18, 18, 18 };
	int cost[MAX_LEVEL] = { 15, 15, 15, 15, 13 };
	for (int k = 0; k < MAX_LEVEL; k++) {
		g_Team[i].lifeMax[k] = Life[k];
		g_Team[i].power[k] = Power[k];
		g_Team[i].diffend[k] = Diffend[k];
		g_Team[i].spMax[k] = spMax[k];
		g_Team[i].cost[k] = cost[k];
	}

	g_Team[i].life = g_Team[i].lifeMax[g_Team[i].level - 1];
	g_Team[i].blockMax = 2;
	g_Team[i].blockNum = 0;
	g_Team[i].startNum = partsNum;
	g_Team[i].partsNum = 1;
	g_Team[i].atFrame = 20;
	g_Team[i].setAble = FALSE;
	g_Team[i].attackSE = SOUND_LABEL_SE_Hit;
	g_Team[i].material = Neutrovalue;


	//�A�j���[�V�����f�[�^�̃Z�b�g
	g_Team[i].tbl_adrA = killerT_Attack;	//�擪�A�h���X�̎w��Ȃ��߁A�Y������i
	g_Team[i].tbl_sizeA = sizeof(killerT_Attack) / sizeof(INTERPOLATION_DATA);	//�f�[�^�T�C�Y
	g_Team[i].tbl_adrM = killerT_Standby;	//�擪�A�h���X�̎w��Ȃ��߁A�Y������i
	g_Team[i].tbl_sizeM = sizeof(killerT_Standby) / sizeof(INTERPOLATION_DATA);	//�f�[�^�T�C�Y

																				//�p�[�c���̏���������
	LoadModel(MODEL_KILLAXE, &g_Parts[partsNum].model);
	// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
	GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);

	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
	g_Parts[partsNum].tbl_adrA = killerT_AxeAt;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
	g_Parts[partsNum].tbl_sizeA = sizeof(killerT_AxeAt) / sizeof(INTERPOLATION_DATA);	// �o�^�����e�[�u���̃��R�[�h����
	g_Parts[partsNum].tbl_adrM = killerT_AxeSb;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
	g_Parts[partsNum].tbl_sizeM = sizeof(killerT_AxeSb) / sizeof(INTERPOLATION_DATA);	// �o�^�����e�[�u���̃��R�[�h����
	g_Parts[partsNum].move_time = 0;	// ���s����
	partsNum++;

	memberNum++;
}

//�f�o�b�O�p�B
void SetNK(int i)
{
	LoadModel(MODEL_NK, &g_Team[i].model);
	// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
	GetModelDiffuse(&g_Team[i].model, &g_Team[i].diffuse[0]);
	g_Team[i].use = TRUE;
	g_Team[i].charID = ID_NK;
	g_Team[i].charType = HighPlaces;
	g_Team[i].scl = { 1.0f, 1.0f, 1.0f };
	g_Team[i].size = NK_SIZE;	// �����蔻��̑傫��
	g_Team[i].level = 1;
	int Life[MAX_LEVEL] = { 80, 95, 120, 130, 150 };
	int Power[MAX_LEVEL] = { 4, 4, 6, 7, 10 };
	int Diffend[MAX_LEVEL] = { 8, 10, 15, 17, 20 };
	int spMax[MAX_LEVEL] = { 20, 20, 18, 18, 18 };
	int cost[MAX_LEVEL] = { 15, 15, 15, 15, 13 };
	for (int k = 0; k < MAX_LEVEL; k++) {
		g_Team[i].lifeMax[k] = Life[k];
		g_Team[i].power[k] = Power[k];
		g_Team[i].diffend[k] = Diffend[k];
		g_Team[i].spMax[k] = spMax[k];
		g_Team[i].cost[k] = cost[k];
	}

	g_Team[i].life = g_Team[i].lifeMax[g_Team[i].level - 1];
	g_Team[i].blockMax = 2;
	g_Team[i].blockNum = 0;
	g_Team[i].startNum = partsNum;
	g_Team[i].partsNum = 0;
	g_Team[i].atFrame = 55;
	g_Team[i].setAble = FALSE;

	g_Team[i].material = Neutrovalue;
	g_Team[i].attackSE = SOUND_LABEL_SE_Cannon;


	//�A�j���[�V�����f�[�^�̃Z�b�g
	g_Team[i].tbl_adrA = NK_Attack;	//�擪�A�h���X�̎w��Ȃ��߁A�Y������i
	g_Team[i].tbl_sizeA = sizeof(NK_Attack) / sizeof(INTERPOLATION_DATA);	//�f�[�^�T�C�Y
	g_Team[i].tbl_adrM = NK_Standby;	//�擪�A�h���X�̎w��Ȃ��߁A�Y������i
	g_Team[i].tbl_sizeM = sizeof(NK_Standby) / sizeof(INTERPOLATION_DATA);	//�f�[�^�T�C�Y

	memberNum++;
}
//�f�o�b�O�p�B
void SetKouen(int i)
{
	LoadModel(MODEL_KOUEN, &g_Team[i].model);
	// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
	GetModelDiffuse(&g_Team[i].model, &g_Team[i].diffuse[0]);
	g_Team[i].use = TRUE;
	g_Team[i].charID = ID_Kouen;
	g_Team[i].charType = HighPlaces;
	g_Team[i].scl = { 1.0f, 1.0f, 1.0f };
	g_Team[i].size = NK_SIZE;	// �����蔻��̑傫��
	g_Team[i].level = 1;
	int Life[MAX_LEVEL] = { 80, 95, 120, 130, 150 };
	int Power[MAX_LEVEL] = { 4, 4, 6, 7, 10 };
	int Diffend[MAX_LEVEL] = { 8, 10, 15, 17, 20 };
	int spMax[MAX_LEVEL] = { 20, 20, 18, 18, 18 };
	int cost[MAX_LEVEL] = { 15, 15, 15, 15, 13 };
	for (int k = 0; k < MAX_LEVEL; k++) {
		g_Team[i].lifeMax[k] = Life[k];
		g_Team[i].power[k] = Power[k];
		g_Team[i].diffend[k] = Diffend[k];
		g_Team[i].spMax[k] = spMax[k];
		g_Team[i].cost[k] = cost[k];
	}

	g_Team[i].life = g_Team[i].lifeMax[g_Team[i].level - 1];
	g_Team[i].blockMax = 0;
	g_Team[i].blockNum = 0;
	g_Team[i].startNum = partsNum;
	g_Team[i].partsNum = 2;
	g_Team[i].atFrame = 55;
	g_Team[i].setAble = FALSE;

	g_Team[i].material = Neutrovalue;
	g_Team[i].attackSE = SOUND_LABEL_SE_Cannon;


	//�A�j���[�V�����f�[�^�̃Z�b�g
	g_Team[i].tbl_adrA = Kouen_Attack;	//�擪�A�h���X�̎w��Ȃ��߁A�Y������i
	g_Team[i].tbl_sizeA = sizeof(Kouen_Attack) / sizeof(INTERPOLATION_DATA);	//�f�[�^�T�C�Y
	g_Team[i].tbl_adrM = Kouen_Standby;	//�擪�A�h���X�̎w��Ȃ��߁A�Y������i
	g_Team[i].tbl_sizeM = sizeof(Kouen_Standby) / sizeof(INTERPOLATION_DATA);	//�f�[�^�T�C�Y
	
	
	LoadModel(MODEL_KOUEN_ARM001, &g_Parts[partsNum].model);
	// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
	GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);

	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
	g_Parts[partsNum].tbl_adrA = Kouen_Arm_Attack;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
	g_Parts[partsNum].tbl_sizeA = sizeof(Kouen_Arm_Attack) / sizeof(INTERPOLATION_DATA);	// �o�^�����e�[�u���̃��R�[�h����
	g_Parts[partsNum].tbl_adrM = Kouen_Arm_Standby;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
	g_Parts[partsNum].tbl_sizeM = sizeof(Kouen_Arm_Standby) / sizeof(INTERPOLATION_DATA);	// �o�^�����e�[�u���̃��R�[�h����
	g_Parts[partsNum].move_time = 0;	// ���s����
	partsNum++;


	LoadModel(MODEL_KOUEN_ARM002, &g_Parts[partsNum].model);
	// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
	GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);

	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
	g_Parts[partsNum].tbl_adrA = Kouen_Arm2_Attack;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
	g_Parts[partsNum].tbl_sizeA = sizeof(Kouen_Arm2_Attack) / sizeof(INTERPOLATION_DATA);	// �o�^�����e�[�u���̃��R�[�h����
	g_Parts[partsNum].tbl_adrM = Kouen_Arm2_Standby;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
	g_Parts[partsNum].tbl_sizeM = sizeof(Kouen_Arm2_Standby) / sizeof(INTERPOLATION_DATA);	// �o�^�����e�[�u���̃��R�[�h����
	g_Parts[partsNum].move_time = 0;	// ���s����
	partsNum++;

	memberNum++;
}

int GetMemberNum(void)
{
	return memberNum;
}
