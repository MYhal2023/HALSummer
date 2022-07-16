//=============================================================================
//
//  スキル処理[skill.cpp]
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

//マクロファージのスキル処理
//攻撃寄り？
void MacroSkill(int i)
{
	PLAYER *player = GetPlayer();
	//攻撃範囲内に敵がいる場合
	if (player[i].count > 0)
	{
		CheckEnemyTarget(i);
	}
	else
	{
		PlayerStandLiner(i);
	}

	//スキル時間を超過したならスキルを切る
	player[i].skillTime++;
	if (player[i].skillTime >= player[i].skillTimeMax)
	{
		player[i].skillUse = FALSE;
		player[i].state = Standby;
	}
}