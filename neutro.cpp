//=============================================================================
//
// 好中球処理 [neutro.cpp]
// Author : 
//好中球のみに行う特殊処理を記載
//=============================================================================
#include "player.h"
#include "cost.h"
#define SKILL_TIME (600)

//好中球のスキル処理
//コスト増加？
void NeutroSkill(PLAYER *player)
{
	IncreaseCost(6);
	player->skillUse = FALSE;
	player->state = Standby;
}