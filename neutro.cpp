//=============================================================================
//
// �D�������� [neutro.cpp]
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