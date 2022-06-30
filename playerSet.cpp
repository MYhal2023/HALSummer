//=============================================================================
//
// �v���C���[�Z�b�g���� [playerSet.cpp]
// Author : 
//
//=============================================================================
#include "playerSet.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static PlayerSet g_PlayerSet;
static BOOL g_Load = FALSE;
HRESULT PlayerSet::InitPlayerSet(void)
{
	g_PlayerSet.setPos = XMFLOAT3(0.0f, 15.0f, 0.0f);
	g_Load = TRUE;
	return S_OK;
}

void UninitPlayerSet(void)
{
	if (!g_Load)return;
}

void UpdatePlayerSet(void)
{
	g_PlayerSet.PlayerSetMap();
	g_PlayerSet.SetPosition();
}

void DrawPlayerSet(void)
{

}

//�����ꂽ�e���L�[����Ƀv���C���[���Z�b�g���鏀��������
void PlayerSet::PlayerSetMap(void)
{
	if (GetKeyboardTrigger(DIK_0))
	{
		g_PlayerSet.SetModeChange(0);
	}
	if (GetKeyboardTrigger(DIK_1))
	{
		g_PlayerSet.SetModeChange(1);
	}
	else if (GetKeyboardTrigger(DIK_2))
	{
		g_PlayerSet.SetModeChange(2);

	}
	else if (GetKeyboardTrigger(DIK_3))
	{
		g_PlayerSet.SetModeChange(3);

	}
	else if (GetKeyboardTrigger(DIK_4))
	{
		g_PlayerSet.SetModeChange(4);

	}
	else if (GetKeyboardTrigger(DIK_5))
	{
		g_PlayerSet.SetModeChange(5);

	}
	else if (GetKeyboardTrigger(DIK_6))
	{
		g_PlayerSet.SetModeChange(6);

	}
	else if (GetKeyboardTrigger(DIK_7))
	{
		g_PlayerSet.SetModeChange(7);

	}
	else if (GetKeyboardTrigger(DIK_8))
	{
		g_PlayerSet.SetModeChange(8);

	}
	else if (GetKeyboardTrigger(DIK_9))
	{
		g_PlayerSet.SetModeChange(9);

	}
}

void PlayerSet::SetModeChange(int i)
{
	PLAYER *member = Team::GetTeam();
	PlayerParts *parts = Team::GetTeamParts();
	if (member[i].use != TRUE)return; //�Z�b�g�ԍ����󔒁B�����ŕԂ�
	//�Ґ���񂩂�A�Z�b�g����v���C���[�̐擪�A�h���X�𔲂��o��
	g_PlayerSet.setMode = TRUE;
	g_PlayerSet.setPlayerParts = &parts[member[i].startNum];
	g_PlayerSet.setPlayerNum = &member[i];
}

//�v���C���[���Z�b�g������W���\���L�[�ŕύX�ł���
void PlayerSet::SetPosition(void)
{
	//�Z�b�g���[�h�Ɉڍs���ĂȂ���΍��W�ύX�ł��Ȃ�
	if (!g_PlayerSet.setMode)return;

	if (GetKeyboardTrigger(DIK_LEFT))
	{
		g_PlayerSet.setPos.x -= CHIP_SIZE;
	}
	else if (GetKeyboardTrigger(DIK_RIGHT))
	{
		g_PlayerSet.setPos.x += CHIP_SIZE;
	}
	else if (GetKeyboardTrigger(DIK_UP))
	{
		g_PlayerSet.setPos.z += CHIP_SIZE;
	}
	else if (GetKeyboardTrigger(DIK_DOWN))
	{
		g_PlayerSet.setPos.z -= CHIP_SIZE;
	}

	if (GetKeyboardTrigger(DIK_RETURN))
	{
		g_PlayerSet.setPlayerNum->pos = g_PlayerSet.setPos;
		g_PlayerSet.SetPlayerInfo(g_PlayerSet.setPlayerNum, g_PlayerSet.setPlayerParts);
	}
}

//�����o�����v���C���[�����o��
void PlayerSet::SetPlayerInfo(PLAYER *member, PlayerParts* memberParts)
{
	PLAYER *player = GetPlayer();
	PlayerParts *parts = GetPlayerParts();
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (player[i].use != FALSE)continue;

		player[i].model = member->model;
		GetModelDiffuse(&player[i].model, &player[i].diffuse[0]);
		player[i].use = TRUE;
		player[i].pos = member->pos;
		player[i].rot = member->rot;
		player[i].scl = member->scl;
		player[i].atCount = member->atCount;
		player[i].size = member->size;	// �����蔻��̑傫��
		player[i].life = member->life;
		player[i].lifeMax = player[i].life;
		player[i].power = member->power;
		player[i].diffend = member->diffend;
		player[i].attack = FALSE;
		player[i].attackUse = FALSE;
		player[i].blockMax = member->blockMax;
		player[i].blockNum = 0;
		player[i].target = 99;
		for (int k = 0; k < MAX_TARGET; k++)
			player[i].targetable[k] = 99;
		player[i].count = 0;
		player[i].startNum = GetPlayerPartsNum();;
		player[i].partsNum = member->partsNum;
		player[i].parent = NULL;			// �{�́i�e�j�Ȃ̂�NULL������
		SetPlayerNum(1);

		//�p�[�c�̏���������
		for (int k = GetPlayerPartsNum(); k < player[i].partsNum + GetPlayerPartsNum(); k++)
		{
			player[i].model = memberParts[k].model;
			GetModelDiffuse(&memberParts[k].model, &memberParts[k].diffuse[0]);
			parts[k].load = TRUE;

			parts[k].pos = { 0.0f, 0.0f, 0.0f };		// �|���S���̈ʒu
			parts[k].rot = { 0.0f, 0.0f, 0.0f };		// �|���S���̌���(��])
			parts[k].scl = { 1.0f, 1.0f, 1.0f };		// �|���S���̑傫��(�X�P�[��)

			// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
			parts[k].tbl_adr = memberParts[k].tbl_adr;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
			parts[k].tbl_size = 0;	// �o�^�����e�[�u���̃��R�[�h����
			parts[k].move_time = 0;	// ���s����
			parts[k].parent = &player[i];	// �������e�Ȃ�NULL�A�������q���Ȃ�e�̃A�h���X
			SetPlayerPartsNum(1);
		}
		return;
	}
}
