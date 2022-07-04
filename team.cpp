//=============================================================================
//
// �Ґ����� [team.cpp]
// Author : 
//
//=============================================================================
#include "team.h"
#include "player.h"
#include "interporationdata.h"
#include <string>
static PlayerStatus g_Team[MAX_PLAYER_SET];
static PlayerPartsStatus g_Parts [ MAX_PLAYER_SET * 2];
static char name[2][64];
static BOOL g_Load = FALSE;

void InitTeam(void)
{
	strcpy(name[MainPlayer], MODEL_PLAYER);
	strcpy(name[Neutrophils], MODEL_NEUTROPHILS);

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
	g_Team[i].size = PLAYER_SIZE;	// �����蔻��̑傫��
	g_Team[i].life = 100;
	g_Team[i].lifeMax = g_Team[i].life;
	g_Team[i].power = 5;
	g_Team[i].diffend = 3;
	g_Team[i].blockMax = 2;
	g_Team[i].blockNum = 0;
	g_Team[i].startNum = GetPlayerPartsNum();
	g_Team[i].partsNum = 0;
	g_Team[i].atFrame = 20;
	//�A�j���[�V�����f�[�^�̃Z�b�g
	INTERPOLATION_DATA* tbl = GetData(0);	//�����g���������Y�����Ō��߂�
	int *size = GetDataSize();
	g_Team[i].tbl_adrA = &tbl[0];	//�擪�A�h���X�̎w��Ȃ��߁A�Y������i
	g_Team[i].tbl_sizeA = size[0];	//�f�[�^�T�C�Y
	g_Team[i].tbl_adrM = NULL;	//�擪�A�h���X�̎w��Ȃ��߁A�Y������i
	g_Team[i].tbl_sizeM = 0;	//�f�[�^�T�C�Y
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
}

PlayerStatus* GetTeam(void)
{
	return &g_Team[0];
}

PlayerPartsStatus* GetTeamParts(void)
{
	return &g_Parts[0];
}

