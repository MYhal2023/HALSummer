//=============================================================================
//
// �v���C���[�Z�b�g(�ݒu�A�X�L���g�p)���� [playerSet.cpp]
// Author : 
//
//=============================================================================
#include "playerSet.h"
#include "debugproc.h"
#include "team.h"
#include "player.h"
#include "game.h"
#include "cost.h"

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
	g_PlayerSet.setRot = XMFLOAT3(0.0f, XM_PI * 0.5f, 0.0f);
	g_PlayerSet.setMode = FALSE;
	g_PlayerSet.setCheckMode = FALSE;
	g_PlayerSet.setPlayer = 99;
	PlayerStatus *member = GetTeam();
	//�Ґ���񂩂�A�Z�b�g�\�Ȕz��ӏ���ǂݍ���
	for (int i = 0; i < MAX_PLAYER_SET; i++)
	{
		g_PlayerSet.use[i] = FALSE;
		member[i].setAble = FALSE;
		if (member[i].use == FALSE)continue;	//�Ґ��g���g�p
		g_PlayerSet.use[i] = TRUE;	//�Z�b�g�\�ł��邱�Ƃ�\���Ă���
		g_PlayerSet.setCharID[i] = member[i].charID;
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
	SetAbleChar();
	PlayerSetMap();
	SetPosition();
	CheckSetChar();

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
	if (!g_PlayerSet.setMode)
		CharaStateCheck(i);	//�Ґ��ɓo�^����Ă��Ȃ����߃L�����ڍ׊֐���
	if (g_PlayerSet.setCheckMode)
	{
		return;
	}
	PlayerStatus *member = GetTeam();
	if (!g_PlayerSet.use[i] || !member[i].setAble)return;
	//�Z�b�g���[�h�ɏ��߂Ĉڍs
	if (g_PlayerSet.setMode == FALSE)
	{
		g_PlayerSet.setMode = TRUE;
		g_PlayerSet.setCheckMode = FALSE;
		g_PlayerSet.setPlayer = i;
		SetSlowMode(TRUE);
	}
	//�Z�b�g���[�h�Ɉڍs�ς݂����A�Z�b�g����Ґ��ԍ������ύX���ꂽ
	else if (g_PlayerSet.setMode == TRUE && g_PlayerSet.setPlayer != i)
	{
		g_PlayerSet.setPlayer = i;
		g_PlayerSet.setCheckMode = FALSE;
	}
	//�Z�b�g���[�h�Ɉڍs�ς݂œ��L�������I�����ꂽ���߁A�Z�b�g���[�h����������
	else if(g_PlayerSet.setMode == TRUE && g_PlayerSet.setPlayer == i)
	{
		g_PlayerSet.setMode = FALSE;
		g_PlayerSet.setCheckMode = FALSE;
		g_PlayerSet.setPlayer = 99;
		SetSlowMode(FALSE);
	}
}

//�v���C���[���Z�b�g������W���\���L�[�ŕύX�ł���
void SetPosition(void)
{
	//�Z�b�g���[�h�Ɉڍs���ĂȂ���΍��W�ύX�ł��Ȃ�
	if (!g_PlayerSet.setMode || g_PlayerSet.setCheckMode)return;

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
			switch (GetMapChiptype(z, x))
			{
			case LowPlaces:
				g_PlayerSet.setPos.y = 20.0f;
				break;
			case HighPlaces:
				g_PlayerSet.setPos.y =32.0f;
				break;
			}
			SetPlayerInfo(&member[g_PlayerSet.setPlayer], &parts[member[g_PlayerSet.setPlayer].startNum]);
			g_PlayerSet.use[g_PlayerSet.setPlayer] = FALSE;	//�Z�b�g�s��Ԃɂ���
			g_PlayerSet.setMode = FALSE;					//�Z�b�g���[�h����
			SetMapChipUse(TRUE, z, x);
			SetSlowMode(FALSE);
			DecreaseCost(member[g_PlayerSet.setPlayer].cost[member[g_PlayerSet.setPlayer].level - 1]);
			g_PlayerSet.setPlayer = 99;
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
		player[i].size = member->size;	// �U���T�m�̓����蔻��̑傫��
		player[i].state = Standby;
		player[i].level = member->level;
		player[i].lifeMax = member->lifeMax[member->level - 1];
		player[i].life = player[i].lifeMax;
		player[i].power = member->power[member->level - 1];
		player[i].diffend = member->diffend[member->level - 1];
		player[i].attack = FALSE;
		player[i].attackUse = FALSE;
		player[i].blockMax = member->blockMax;
		player[i].blockNum = 0;
		player[i].target = 99;
		player[i].skillAble = FALSE;
		player[i].skillID = member->skillID;
		player[i].skillPoint = 0;
		player[i].skillPointMax = member->spMax[member->level - 1];
		player[i].increaseSP = 1;
		player[i].skillUse = FALSE;
		player[i].intervalSP = 0;
		player[i].cost = member->cost[member->level - 1];
		for (int k = 0; k < MAX_TARGET; k++)
			player[i].targetable[k] = 99;
		player[i].count = 0;
		player[i].startNum = member->startNum;
		player[i].partsNum = member->partsNum;
		player[i].parent = NULL;			// �{�́i�e�j�Ȃ̂�NULL������
		player[i].tbl_adrA = member->tbl_adrA;
		player[i].tbl_sizeA = member->tbl_sizeA;
		player[i].tbl_adrM = member->tbl_adrM;
		player[i].tbl_sizeM = member->tbl_sizeM;
		player[i].move_time = 0.0f;
		player[i].keyNum = g_PlayerSet.setPlayer;	//�X�L�������̃V���g�J�ׂ̈ɕۑ�
		SetPlayerNum(1);

		if (player[i].partsNum == 0)return;	//�p�[�c���Ȃ��Ȃ炱��ȍ~�̏����͕s�v
		//�p�[�c�̏���������
		for (int k = member->startNum; k < member->startNum + member->partsNum; k++)
		{
			if (parts[k].load != FALSE)continue;	//���g�p�z��܂ŃX�L�b�v

			parts[k].model = memberParts->model;
			GetModelDiffuse(&memberParts->model, &memberParts->diffuse[0]);
			parts[k].load = TRUE;
			parts[k].use = TRUE;

			parts[k].pos = { 0.0f, 0.0f, 0.0f };		// �|���S���̈ʒu
			parts[k].rot = { 0.0f, 0.0f, 0.0f };		// �|���S���̌���(��])
			parts[k].scl = { 1.0f, 1.0f, 1.0f };		// �|���S���̑傫��(�X�P�[��)

			// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
			parts[k].tbl_adrA = memberParts->tbl_adrA;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
			parts[k].tbl_sizeA = memberParts->tbl_sizeA;	// �o�^�����e�[�u���̃��R�[�h����
			parts[k].tbl_adrM = memberParts->tbl_adrM;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
			parts[k].tbl_sizeM = memberParts->tbl_sizeM;	// �o�^�����e�[�u���̃��R�[�h����
			parts[k].move_time = 0.0f;			// ���s����
			parts[k].parent = &player[i];	// �������e�Ȃ�NULL�A�������q���Ȃ�e�̃A�h���X
			memberParts++;
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
//�z�u�ς݂̃L�����̐����L�[���Ăщ����ꂽ�炱�ꂪ�Ă΂��
void CharaStateCheck(int num)
{
	PLAYER *player = GetPlayer();
	for (int k = 0; k < MAX_PLAYER; k++)
	{
		if (player[k].keyNum == num && g_PlayerSet.setPlayer != k)
		{
			g_PlayerSet.setCheckMode = TRUE;	//�L�����ڍ׃��[�h��
			g_PlayerSet.setPlayer = k;
			return;
		}
		else if (player[k].keyNum == num && g_PlayerSet.setPlayer == k)
		{
			g_PlayerSet.setCheckMode = FALSE;
			g_PlayerSet.setPlayer = 99;
			return;
		}
	}
}

//�S�L�����̐ݒu�ۂ���ɐݒ�
void SetAbleChar(void)
{
	PlayerStatus *member = GetTeam();
	Cost *cost = GetCostNum();
	for (int i = 0; i < MAX_PLAYER_SET; i++)
	{
		if (member[i].use == FALSE)continue;
		if (member[i].cost[member[i].level] <= cost->cost)
			member[i].setAble = TRUE;
		else
			member[i].setAble = FALSE;
	}
}

//�X�L�������ƃ��j�b�g�����Ɋւ��鏈��
void CheckSetChar(void)
{
	PLAYER *player = GetPlayer();
	PlayerParts *parts = GetPlayerParts();
	if (g_PlayerSet.setCheckMode &&
		player[g_PlayerSet.setPlayer].skillAble &&
		GetKeyboardTrigger(DIK_RETURN))
	{
		player[g_PlayerSet.setPlayer].skillPoint = 0;
		player[g_PlayerSet.setPlayer].skillUse = TRUE;
		g_PlayerSet.setCheckMode = FALSE;
		g_PlayerSet.setPlayer = 99;
	}
	else if (g_PlayerSet.setCheckMode &&
		player[g_PlayerSet.setPlayer].skillAble &&
		GetKeyboardTrigger(DIK_C))
	{
		player[g_PlayerSet.setPlayer].use = FALSE;
		for (int k = player[g_PlayerSet.setPlayer].startNum; k < player[g_PlayerSet.setPlayer].startNum + player[g_PlayerSet.setPlayer].partsNum; k++)
		{
			parts[k].use = FALSE;
		}
		int x = (int)(player[g_PlayerSet.setPlayer].pos.x / CHIP_SIZE);
		int z = (int)(player[g_PlayerSet.setPlayer].pos.z / CHIP_SIZE);
		SetMapChipUse(FALSE, z, x);
		IncreaseCost(player[g_PlayerSet.setPlayer].cost / 2);
	}
}

PlayerSet *GetSetPos(void)
{
	return &g_PlayerSet;
}