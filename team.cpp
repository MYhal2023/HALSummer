//=============================================================================
//
// 編成処理 [team.cpp]
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

//デバッグ用で
void Team::SetMember(void)
{
	strcpy(name[0], MODEL_PLAYER);
	//LoadModel(name[0], &g_Team[0].model);
	//// モデルのディフューズを保存しておく。色変え対応の為。
	//GetModelDiffuse(&g_Team[0].model, &g_Team[0].diffuse[0]);

	g_Team[0].load = TRUE;

	g_Team[0].pos = {0.0f, 0.0f, 0.0f};
	g_Team[0].rot = { 0.0f, 0.0f, 0.0f };
	g_Team[0].scl = { 0.8f, 1.0f, 1.0f };

	g_Team[0].size = PLAYER_SIZE;	// 当たり判定の大きさ
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

	// 階層アニメーション用の初期化処理
	g_Team[0].parent = NULL;			// 本体（親）なのでNULLを入れる

	//パーツの初期化処理
	for (int k = 0; k < g_Team[0].partsNum; k++)
	{
		if (g_Parts[k].diffuse != FALSE)continue;
		//LoadModel(MODEL_GRAPE_PARTS, &g_Parts[partsNum].model);
		//// モデルのディフューズを保存しておく。色変え対応の為。
		//GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);
		g_Parts[k].load = TRUE;

		g_Parts[k].pos = { 20.0f, 10.0f, 0.0f };		// ポリゴンの位置
		g_Parts[k].rot = { 0.0f, 0.0f, 0.0f };		// ポリゴンの向き(回転)
		g_Parts[k].scl = { 1.0f, 1.0f, 1.0f };		// ポリゴンの大きさ(スケール)

		// 階層アニメーション用のメンバー変数
		g_Parts[k].tbl_adr = NULL;	// アニメデータのテーブル先頭アドレス
		g_Parts[k].tbl_size = 0;	// 登録したテーブルのレコード総数
		g_Parts[k].move_time = 0;	// 実行時間
		g_Parts[k].parent = &g_Team[0];	// 自分が親ならNULL、自分が子供なら親のenemyアドレス
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