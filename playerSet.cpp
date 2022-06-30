//=============================================================================
//
// プレイヤーセット処理 [playerSet.cpp]
// Author : 
//
//=============================================================================
#include "playerSet.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************



//*****************************************************************************
// グローバル変数
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

//押されたテンキーを基にプレイヤーをセットする準備をする
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
	if (member[i].use != TRUE)return; //セット番号が空白。ここで返す
	//編成情報から、セットするプレイヤーの先頭アドレスを抜き出す
	g_PlayerSet.setMode = TRUE;
	g_PlayerSet.setPlayerParts = &parts[member[i].startNum];
	g_PlayerSet.setPlayerNum = &member[i];
}

//プレイヤーをセットする座標を十字キーで変更できる
void PlayerSet::SetPosition(void)
{
	//セットモードに移行してなければ座標変更できない
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

//抜き出したプレイヤー情報を出す
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
		player[i].size = member->size;	// 当たり判定の大きさ
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
		player[i].parent = NULL;			// 本体（親）なのでNULLを入れる
		SetPlayerNum(1);

		//パーツの初期化処理
		for (int k = GetPlayerPartsNum(); k < player[i].partsNum + GetPlayerPartsNum(); k++)
		{
			player[i].model = memberParts[k].model;
			GetModelDiffuse(&memberParts[k].model, &memberParts[k].diffuse[0]);
			parts[k].load = TRUE;

			parts[k].pos = { 0.0f, 0.0f, 0.0f };		// ポリゴンの位置
			parts[k].rot = { 0.0f, 0.0f, 0.0f };		// ポリゴンの向き(回転)
			parts[k].scl = { 1.0f, 1.0f, 1.0f };		// ポリゴンの大きさ(スケール)

			// 階層アニメーション用のメンバー変数
			parts[k].tbl_adr = memberParts[k].tbl_adr;	// アニメデータのテーブル先頭アドレス
			parts[k].tbl_size = 0;	// 登録したテーブルのレコード総数
			parts[k].move_time = 0;	// 実行時間
			parts[k].parent = &player[i];	// 自分が親ならNULL、自分が子供なら親のアドレス
			SetPlayerPartsNum(1);
		}
		return;
	}
}
