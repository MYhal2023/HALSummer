//=============================================================================
//
// プレイヤーセット処理 [playerSet.cpp]
// Author : 
//
//=============================================================================
#include "playerSet.h"
#include "debugproc.h"
#include "team.h"
#include "player.h"
#include "game.h"

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
	g_PlayerSet.setMode = FALSE;
	PlayerStatus *member = GetTeam();
	//編成情報から、セット可能な配列箇所を読み込む
	for (int i = 0; i < MAX_PLAYER_SET; i++)
	{
		g_PlayerSet.use[i] = FALSE;
		if (member[i].use == FALSE)continue;	//編成枠未使用
		g_PlayerSet.use[i] = TRUE;	//セット可能であることを表している
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
	PrintDebugProc("カーソル座標:X:%f Z:%f", g_PlayerSet.setPos.x, g_PlayerSet.setPos.z);
	PrintDebugProc("セットモード:%d", g_PlayerSet.setMode);
#endif
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
	if (g_PlayerSet.use[i] == FALSE)return;	//編成に登録されていないため返す

	//セットモードに初めて移行
	if (g_PlayerSet.setMode == FALSE)
	{
		g_PlayerSet.setMode = TRUE;
		g_PlayerSet.setPlayer = i;
		SetSlowMode(TRUE);
	}
	//セットモードに移行済みだが、セットする編成番号だけ変更された
	else if (g_PlayerSet.setMode == TRUE && g_PlayerSet.setPlayer != i)
	{
		g_PlayerSet.setPlayer = i;
	}
	//セットモードに移行済みで同キャラが選択されたため、セットモードを解除する
	else if(g_PlayerSet.setMode == TRUE && g_PlayerSet.setPlayer == i)
	{
		g_PlayerSet.setMode = FALSE;
		g_PlayerSet.setPlayer = 99;
		SetSlowMode(FALSE);
	}
}

//プレイヤーをセットする座標を十字キーで変更できる
void SetPosition(void)
{
	//セットモードに移行してなければ座標変更できない
	if (!g_PlayerSet.setMode)return;

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
			SetPlayerInfo(&member[g_PlayerSet.setPlayer], &parts[g_PlayerSet.setPlayer]);
			g_PlayerSet.use[g_PlayerSet.setPlayer] = FALSE;	//セット不可状態にする
			g_PlayerSet.setMode = FALSE;					//セットモード解除
			SetMapChipUse(TRUE, z, x);
			SetSlowMode(FALSE);
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
		player[i].size = member->size;	// 当たり判定の大きさ
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
		player[i].parent = NULL;			// 本体（親）なのでNULLを入れる
		player[i].tbl_adrA = member->tbl_adrA;
		player[i].tbl_sizeA = member->tbl_sizeA;
		player[i].move_time = 0.0f;
		SetPlayerNum(1);

		//パーツの初期化処理
		//使用する配列場所は、member->startNumとpartsNumによって事前に決められている
		for (int k = player[i].startNum; k < player[i].partsNum + player[i].startNum; k++)
		{
			parts[k].model = memberParts[k].model;
			GetModelDiffuse(&memberParts[k].model, &memberParts[k].diffuse[0]);
			parts[k].load = TRUE;

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
			SetPlayerPartsNum(1);
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
PlayerSet *GetSetPos(void)
{
	return &g_PlayerSet;
}