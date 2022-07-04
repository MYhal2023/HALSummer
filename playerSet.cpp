//=============================================================================
//
// �v���C���[�Z�b�g���� [playerSet.cpp]
// Author : 
//
//=============================================================================
#include "playerSet.h"
#include "debugproc.h"
#include "team.h"
#include "player.h"
#include "game.h"

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
HRESULT InitPlayerSet(void)
{
	g_PlayerSet.setPos = XMFLOAT3(0.0f, 15.0f, 0.0f);
	g_PlayerSet.setMode = FALSE;
	PlayerStatus *member = GetTeam();
	//�Ґ���񂩂�A�Z�b�g�\�Ȕz��ӏ���ǂݍ���
	for (int i = 0; i < MAX_PLAYER_SET; i++)
	{
		g_PlayerSet.use[i] = FALSE;
		if (member[i].use == FALSE)continue;	//�Ґ��g���g�p
		g_PlayerSet.use[i] = TRUE;	//�Z�b�g�\�ł��邱�Ƃ�\���Ă���
		g_PlayerSet.setCharID = member[i].charID;
	}
	g_Load = TRUE;
	return S_OK;
}

void UninitPlayerSet(void)
{
	if (!g_Load)return;


}

void UpdatePlayerSet(void)
{
	PlayerSetMap();
	SetPosition();
#ifdef _DEBUG
	PrintDebugProc("�J�[�\�����W:X:%f Z:%f", g_PlayerSet.setPos.x, g_PlayerSet.setPos.z);
	PrintDebugProc("�Z�b�g���[�h:%d", g_PlayerSet.setMode);
#endif
}

void DrawPlayerSet(void)
{

}

//�����ꂽ�e���L�[����Ƀv���C���[���Z�b�g���鏀��������
void PlayerSetMap(void)
{
	if (GetKeyboardTrigger(DIK_0))
	{
		SetModeChange(0);
	}
	else if (GetKeyboardTrigger(DIK_1))
	{
		SetModeChange(1);
	}
	else if (GetKeyboardTrigger(DIK_2))
	{
		SetModeChange(2);

	}
	else if (GetKeyboardTrigger(DIK_3))
	{
		SetModeChange(3);

	}
	else if (GetKeyboardTrigger(DIK_4))
	{
		SetModeChange(4);

	}
	else if (GetKeyboardTrigger(DIK_5))
	{
		SetModeChange(5);

	}
	else if (GetKeyboardTrigger(DIK_6))
	{
		SetModeChange(6);

	}
	else if (GetKeyboardTrigger(DIK_7))
	{
		SetModeChange(7);

	}
	else if (GetKeyboardTrigger(DIK_8))
	{
		SetModeChange(8);

	}
	else if (GetKeyboardTrigger(DIK_9))
	{
		SetModeChange(9);
	}
}

void SetModeChange(int i)
{
	if (g_PlayerSet.use[i] == FALSE)return;	//�Ґ��ɓo�^����Ă��Ȃ����ߕԂ�

	//�Z�b�g���[�h�ɏ��߂Ĉڍs
	if (g_PlayerSet.setMode == FALSE)
	{
		g_PlayerSet.setMode = TRUE;
		g_PlayerSet.setPlayer = i;
		SetSlowMode(TRUE);
	}
	//�Z�b�g���[�h�Ɉڍs�ς݂����A�Z�b�g����Ґ��ԍ������ύX���ꂽ
	else if (g_PlayerSet.setMode == TRUE && g_PlayerSet.setPlayer != i)
	{
		g_PlayerSet.setPlayer = i;
	}
	//�Z�b�g���[�h�Ɉڍs�ς݂œ��L�������I�����ꂽ���߁A�Z�b�g���[�h����������
	else if(g_PlayerSet.setMode == TRUE && g_PlayerSet.setPlayer == i)
	{
		g_PlayerSet.setMode = FALSE;
		g_PlayerSet.setPlayer = 99;
		SetSlowMode(FALSE);
	}
}

//�v���C���[���Z�b�g������W���\���L�[�ŕύX�ł���
void SetPosition(void)
{
	//�Z�b�g���[�h�Ɉڍs���ĂȂ���΍��W�ύX�ł��Ȃ�
	if (!g_PlayerSet.setMode)return;

	//�}�b�v���E�̒��̂݃J�[�\���ړ���
	if (GetKeyboardTrigger(DIK_LEFT))
	{
		if(g_PlayerSet.setPos.x > 0.0f)
		g_PlayerSet.setPos.x -= CHIP_SIZE;
	}
	else if (GetKeyboardTrigger(DIK_RIGHT))
	{
		if (g_PlayerSet.setPos.x < GetMapWidth())
		g_PlayerSet.setPos.x += CHIP_SIZE;
	}
	else if (GetKeyboardTrigger(DIK_UP))
	{
		if (g_PlayerSet.setPos.z < GetMapHeight())
		g_PlayerSet.setPos.z += CHIP_SIZE;
	}
	else if (GetKeyboardTrigger(DIK_DOWN))
	{
		if (g_PlayerSet.setPos.z > 0.0f)
		g_PlayerSet.setPos.z -= CHIP_SIZE;
	}

	if (GetKeyboardTrigger(DIK_RETURN))
	{
		//�v���C���[���w�肳�ꂽ�}�b�v�ɃZ�b�g
		PLAYER *player = GetPlayer();
		PlayerStatus *member = GetTeam();
		PlayerPartsStatus *parts = GetTeamParts();
		int x = (int)(g_PlayerSet.setPos.x / CHIP_SIZE);
		int z = (int)(g_PlayerSet.setPos.z / CHIP_SIZE);
		//�ݒu�\�}�X�ŁA�܂��g���ĂȂ��}�X�Ȃ�ݒu
		if (CheckPlayerAble(&member[g_PlayerSet.setPlayer]) && !GetMapChipUse(z,x))
		{
			SetPlayerInfo(&member[g_PlayerSet.setPlayer], &parts[g_PlayerSet.setPlayer]);
			g_PlayerSet.use[g_PlayerSet.setPlayer] = FALSE;	//�Z�b�g�s��Ԃɂ���
			g_PlayerSet.setMode = FALSE;					//�Z�b�g���[�h����
			SetMapChipUse(TRUE, z, x);
			SetSlowMode(FALSE);
		}
	}
}

//�Ґ���񂩂甲���o�����v���C���[����퓬�p�v���C���[�z��ɃZ�b�g���ď�����
void SetPlayerInfo(PlayerStatus *member, PlayerPartsStatus* memberParts)
{
	PLAYER *player = GetPlayer();
	PlayerParts *parts = GetPlayerParts();
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (player[i].use != FALSE)continue;	//���g�p�v���C���[�z��܂ŃX�L�b�v

		player[i].model = member->model;
		GetModelDiffuse(&player[i].model, &player[i].diffuse[0]);
		player[i].load = TRUE;
		player[i].use = TRUE;
		player[i].pos = g_PlayerSet.setPos;
		player[i].rot = g_PlayerSet.setRot;
		player[i].scl = member->scl;
		player[i].atCount = 0;
		player[i].atFrameCount = 0;
		player[i].atFrame = member->atFrame;
		player[i].size = member->size;	// �����蔻��̑傫��
		player[i].state = Standby;
		player[i].life = member->life;
		player[i].lifeMax = member->lifeMax;
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
		player[i].tbl_adrA = member->tbl_adrA;
		player[i].tbl_sizeA = member->tbl_sizeA;
		player[i].move_time = 0.0f;
		SetPlayerNum(1);

		//�p�[�c�̏���������
		//�g�p����z��ꏊ�́Amember->startNum��partsNum�ɂ���Ď��O�Ɍ��߂��Ă���
		for (int k = player[i].startNum; k < player[i].partsNum + player[i].startNum; k++)
		{
			parts[k].model = memberParts[k].model;
			GetModelDiffuse(&memberParts[k].model, &memberParts[k].diffuse[0]);
			parts[k].load = TRUE;

			parts[k].pos = { 0.0f, 0.0f, 0.0f };		// �|���S���̈ʒu
			parts[k].rot = { 0.0f, 0.0f, 0.0f };		// �|���S���̌���(��])
			parts[k].scl = { 1.0f, 1.0f, 1.0f };		// �|���S���̑傫��(�X�P�[��)

			// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
			parts[k].tbl_adrA = memberParts[k].tbl_adrA;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
			parts[k].tbl_sizeA = sizeof(memberParts[k].tbl_adrA) / sizeof(INTERPOLATION_DATA);;	// �o�^�����e�[�u���̃��R�[�h����
			parts[k].tbl_adrM = memberParts[k].tbl_adrM;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
			parts[k].tbl_sizeM = sizeof(memberParts[k].tbl_adrM) / sizeof(INTERPOLATION_DATA);;	// �o�^�����e�[�u���̃��R�[�h����
			parts[k].move_time = 0;			// ���s����
			parts[k].parent = &player[i];	// �������e�Ȃ�NULL�A�������q���Ȃ�e�̃A�h���X
			SetPlayerPartsNum(1);
		}
		return;	//�Z�b�g���������̂ŏ����I��
	}
}

//�z�u���悤�Ƃ��Ă���}�X�͐ݒu�\���ۂ�
BOOL CheckPlayerAble(PlayerStatus *member)
{
	BOOL ans = TRUE;
	int x = (int)(g_PlayerSet.setPos.x / CHIP_SIZE);
	int z = (int)(g_PlayerSet.setPos.z / CHIP_SIZE);
	if (member->charType != GetMapChiptype(z, x))
		ans = FALSE;

	return ans;
}
PlayerSet *GetSetPos(void)
{
	return &g_PlayerSet;
}