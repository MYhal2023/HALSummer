//=============================================================================
//
// �Ґ����� [team.cpp]
// Author : 
//
//=============================================================================
#include "team.h"
#include <string>
static PLAYER g_Team[MAX_PLAYER_SET];
static PlayerParts g_Parts [ MAX_PLAYER_SET * 3];
static char name[2][64];

void Team::InitTeam(void)
{
	for (int i = 0; i < MAX_PLAYER_SET; i++)
		member[i] = 99;
}

//�f�o�b�O�p��
void Team::SetMember(void)
{
	strcpy(name[0], MODEL_PLAYER);
	//LoadModel(name[0], &g_Team[0].model);
	//// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
	//GetModelDiffuse(&g_Team[0].model, &g_Team[0].diffuse[0]);

	g_Team[0].load = TRUE;

	g_Team[0].pos = {0.0f, 0.0f, 0.0f};
	g_Team[0].rot = { 0.0f, 0.0f, 0.0f };
	g_Team[0].scl = { 0.8f, 1.0f, 1.0f };

	g_Team[0].size = PLAYER_SIZE;	// �����蔻��̑傫��
	g_Team[0].use = TRUE;
	g_Team[0].life = 100;
	g_Team[0].lifeMax = g_Team[0].life;
	g_Team[0].power = 5;
	g_Team[0].diffend = 3;
	g_Team[0].attack = FALSE;
	g_Team[0].attackUse = FALSE;
	g_Team[0].blockMax = 2;
	g_Team[0].blockNum = 0;
	g_Team[0].target = 99;
	for (int i = 0; i < MAX_TARGET; i++)
		g_Team[0].targetable[i] = 99;
	g_Team[0].count = 0;
	g_Team[0].startNum = partsNum;
	g_Team[0].partsNum = 0;

	// �K�w�A�j���[�V�����p�̏���������
	g_Team[0].parent = NULL;			// �{�́i�e�j�Ȃ̂�NULL������

	//�p�[�c�̏���������
	for (int k = 0; k < g_Team[0].partsNum; k++)
	{
		if (g_Parts[k].diffuse != FALSE)continue;
		//LoadModel(MODEL_GRAPE_PARTS, &g_Parts[partsNum].model);
		//// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		//GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);
		g_Parts[k].load = TRUE;

		g_Parts[k].pos = { 20.0f, 10.0f, 0.0f };		// �|���S���̈ʒu
		g_Parts[k].rot = { 0.0f, 0.0f, 0.0f };		// �|���S���̌���(��])
		g_Parts[k].scl = { 1.0f, 1.0f, 1.0f };		// �|���S���̑傫��(�X�P�[��)

		// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
		g_Parts[k].tbl_adr = NULL;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
		g_Parts[k].tbl_size = 0;	// �o�^�����e�[�u���̃��R�[�h����
		g_Parts[k].move_time = 0;	// ���s����
		g_Parts[k].parent = &g_Team[0];	// �������e�Ȃ�NULL�A�������q���Ȃ�e��enemy�A�h���X
	}

}

PLAYER* Team::GetTeam(void)
{
	return &g_Team[0];
}

PlayerParts* Team::GetTeamParts(void)
{
	return &g_Parts[0];
}