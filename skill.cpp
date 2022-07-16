//=============================================================================
//
//  �X�L������[skill.cpp]
// Author : 
//�D�����݂̂ɍs�����ꏈ�����L��
//=============================================================================
#include "player.h"
#include "cost.h"
#define SKILL_TIME (600)

//�D�����̃X�L������
//�R�X�g�����H
void NeutroSkill(PLAYER *player)
{
	IncreaseCost(6);
	player->skillUse = FALSE;
	player->state = Standby;
}

//�}�N���t�@�[�W�̃X�L������
//�U�����H
void MacroSkill(int i)
{
	PLAYER *player = GetPlayer();
	//�U���͈͓��ɓG������ꍇ
	if (player[i].count > 0)
	{
		CheckEnemyTarget(i);
	}
	else
	{
		PlayerStandLiner(i);
	}

	//�X�L�����Ԃ𒴉߂����Ȃ�X�L����؂�
	player[i].skillTime++;
	if (player[i].skillTime >= player[i].skillTimeMax)
	{
		player[i].skillUse = FALSE;
		player[i].state = Standby;
	}
}