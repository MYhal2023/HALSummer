//=============================================================================
//
// プレイヤーセット(設置、スキル使用)処理 [playerSet.cpp]
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
HRESULT InitPlayerSet(void)
{
	g_PlayerSet.setPos = XMFLOAT3(0.0f, 15.0f, 0.0f);
	g_PlayerSet.setRot = XMFLOAT3(0.0f, XM_PI * 0.5f, 0.0f);
	g_PlayerSet.setMode = FALSE;
	g_PlayerSet.setCheckMode = FALSE;
	g_PlayerSet.setPlayer = 99;
	PlayerStatus *member = GetTeam();
	//編成情報から、セット可能な配列箇所を読み込む
	for (int i = 0; i < MAX_PLAYER_SET; i++)
	{
		g_PlayerSet.use[i] = FALSE;
		member[i].setAble = FALSE;
		if (member[i].use == FALSE)continue;	//編成枠未使用
		g_PlayerSet.use[i] = TRUE;	//セット可能であることを表している
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

//押されたテンキーを基にプレイヤーをセットする準備をする
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
		CharaStateCheck(i);	//編成に登録されていないためキャラ詳細関数へ
	if (g_PlayerSet.setCheckMode)
	{
		return;
	}
	PlayerStatus *member = GetTeam();
	if (!g_PlayerSet.use[i] || !member[i].setAble)return;
	//セットモードに初めて移行
	if (g_PlayerSet.setMode == FALSE)
	{
		g_PlayerSet.setMode = TRUE;
		g_PlayerSet.setCheckMode = FALSE;
		g_PlayerSet.setPlayer = i;
		SetSlowMode(TRUE);
	}
	//セットモードに移行済みだが、セットする編成番号だけ変更された
	else if (g_PlayerSet.setMode == TRUE && g_PlayerSet.setPlayer != i)
	{
		g_PlayerSet.setPlayer = i;
		g_PlayerSet.setCheckMode = FALSE;
	}
	//セットモードに移行済みで同キャラが選択されたため、セットモードを解除する
	else if(g_PlayerSet.setMode == TRUE && g_PlayerSet.setPlayer == i)
	{
		g_PlayerSet.setMode = FALSE;
		g_PlayerSet.setCheckMode = FALSE;
		g_PlayerSet.setPlayer = 99;
		SetSlowMode(FALSE);
	}
}

//プレイヤーをセットする座標を十字キーで変更できる
void SetPosition(void)
{
	//セットモードに移行してなければ座標変更できない
	if (!g_PlayerSet.setMode || g_PlayerSet.setCheckMode)return;

	//マップ限界の中のみカーソル移動可
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
		//プレイヤーを指定されたマップにセット
		PLAYER *player = GetPlayer();
		PlayerStatus *member = GetTeam();
		PlayerPartsStatus *parts = GetTeamParts();
		int x = (int)(g_PlayerSet.setPos.x / CHIP_SIZE);
		int z = (int)(g_PlayerSet.setPos.z / CHIP_SIZE);
		//設置可能マスで、まだ使われてないマスなら設置
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
			g_PlayerSet.use[g_PlayerSet.setPlayer] = FALSE;	//セット不可状態にする
			g_PlayerSet.setMode = FALSE;					//セットモード解除
			SetMapChipUse(TRUE, z, x);
			SetSlowMode(FALSE);
			DecreaseCost(member[g_PlayerSet.setPlayer].cost);
			g_PlayerSet.setPlayer = 99;
		}
	}
}

//編成情報から抜き出したプレイヤー情報を戦闘用プレイヤー配列にセットして初期化
void SetPlayerInfo(PlayerStatus *member, PlayerPartsStatus* memberParts)
{
	PLAYER *player = GetPlayer();
	PlayerParts *parts = GetPlayerParts();
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (player[i].use != FALSE)continue;	//未使用プレイヤー配列までスキップ

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
		player[i].size = member->size;	// 攻撃探知の当たり判定の大きさ
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
		player[i].skillAble = FALSE;
		player[i].skillID = member->skillID;
		player[i].skillPoint = 0;
		player[i].skillPointMax = 5;
		player[i].increaseSP = 1;
		player[i].skillUse = FALSE;
		player[i].intervalSP = 0;
		player[i].cost = member->cost;
		for (int k = 0; k < MAX_TARGET; k++)
			player[i].targetable[k] = 99;
		player[i].count = 0;
		player[i].startNum = member->startNum;
		player[i].partsNum = member->partsNum;
		player[i].parent = NULL;			// 本体（親）なのでNULLを入れる
		player[i].tbl_adrA = member->tbl_adrA;
		player[i].tbl_sizeA = member->tbl_sizeA;
		player[i].tbl_adrM = member->tbl_adrM;
		player[i].tbl_sizeM = member->tbl_sizeM;
		player[i].move_time = 0.0f;
		player[i].keyNum = g_PlayerSet.setPlayer;	//スキル発動のショトカの為に保存
		SetPlayerNum(1);

		if (player[i].partsNum == 0)return;	//パーツがないならこれ以降の処理は不要
		//パーツの初期化処理
		for (int k = member->startNum; k < member->startNum + member->partsNum; k++)
		{
			if (parts[k].load != FALSE)continue;	//未使用配列までスキップ

			parts[k].model = memberParts[k].model;
			GetModelDiffuse(&memberParts[k].model, &memberParts[k].diffuse[0]);
			parts[k].load = TRUE;
			parts[k].use = TRUE;

			parts[k].pos = { 0.0f, 0.0f, 0.0f };		// ポリゴンの位置
			parts[k].rot = { 0.0f, 0.0f, 0.0f };		// ポリゴンの向き(回転)
			parts[k].scl = { 1.0f, 1.0f, 1.0f };		// ポリゴンの大きさ(スケール)

			// 階層アニメーション用のメンバー変数
			parts[k].tbl_adrA = memberParts[k].tbl_adrA;	// アニメデータのテーブル先頭アドレス
			parts[k].tbl_sizeA = sizeof(memberParts[k].tbl_adrA) / sizeof(INTERPOLATION_DATA);;	// 登録したテーブルのレコード総数
			parts[k].tbl_adrM = memberParts[k].tbl_adrM;	// アニメデータのテーブル先頭アドレス
			parts[k].tbl_sizeM = sizeof(memberParts[k].tbl_adrM) / sizeof(INTERPOLATION_DATA);;	// 登録したテーブルのレコード総数
			parts[k].move_time = 0;			// 実行時間
			parts[k].parent = &player[i];	// 自分が親ならNULL、自分が子供なら親のアドレス
		}
		return;	//セットしきったので処理終了
	}
}

//配置しようとしているマスは設置可能か否か
BOOL CheckPlayerAble(PlayerStatus *member)
{
	BOOL ans = TRUE;
	int x = (int)(g_PlayerSet.setPos.x / CHIP_SIZE);
	int z = (int)(g_PlayerSet.setPos.z / CHIP_SIZE);
	if (member->charType != GetMapChiptype(z, x))
		ans = FALSE;

	return ans;
}
//配置済みのキャラの数字キーが再び押されたらこれが呼ばれる
void CharaStateCheck(int num)
{
	PLAYER *player = GetPlayer();
	for (int k = 0; k < MAX_PLAYER; k++)
	{
		if (player[k].keyNum == num && g_PlayerSet.setPlayer != k)
		{
			g_PlayerSet.setCheckMode = TRUE;	//キャラ詳細モードへ
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

//全キャラの設置可否を常に設定
void SetAbleChar(void)
{
	PlayerStatus *member = GetTeam();
	Cost *cost = GetCostNum();
	for (int i = 0; i < MAX_PLAYER_SET; i++)
	{
		if (member[i].use == FALSE)continue;
		if (member[i].cost <= cost->cost)
			member[i].setAble = TRUE;
		else
			member[i].setAble = FALSE;
	}
}

//スキル発動とユニット除去に関する処理
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