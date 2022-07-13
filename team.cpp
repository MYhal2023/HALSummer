//=============================================================================
//
// 編成処理 [team.cpp]
// Author : 
//
//=============================================================================
#include "team.h"
#include "player.h"
#include "playerLinerData.h"
#include <string>
static PlayerStatus g_Team[MAX_PLAYER_SET];
static PlayerPartsStatus g_Parts [ MAX_PLAYER_SET * 2];
static char name[2][64];
static BOOL g_Load = FALSE;

void InitTeam(void)
{
	strcpy(name[MainPlayer], MODEL_PLAYER);
	strcpy(name[Neutrophils], MODEL_NEUTROPHILS);

	for (int i = 0; i < MAX_PLAYER_SET; i++)
	{
		g_Team[i].use = FALSE;
	}
	g_Load = TRUE;
}

void UninitTeam(void)
{
	if (g_Load == FALSE) return;
	name[0][0] = '0';
	name[1][0] = '0';

	for (int i = 0; i < MAX_PLAYER_SET; i++)
	{
		// モデルの解放処理
		if (g_Team[i].use)
		{
			UnloadModel(&g_Team[i].model);
			g_Team[i].use = FALSE;
		}
	}
	g_Load = FALSE;

}

//メンバーのセット関数
//デバッグ用で
void SetMember(int i)
{
	LoadModel(MODEL_NEUTROPHILS, &g_Team[i].model);
	// モデルのディフューズを保存しておく。色変え対応の為。
	GetModelDiffuse(&g_Team[i].model, &g_Team[i].diffuse[0]);
	g_Team[i].use = TRUE;
	g_Team[i].charID = ID_Neutro;
	g_Team[i].charType = LowPlaces;
	g_Team[i].scl = { 1.0f, 1.0f, 1.0f };
	g_Team[i].size = 25.0f;	// 当たり判定の大きさ
	g_Team[i].life = 100;
	g_Team[i].lifeMax = g_Team[i].life;
	g_Team[i].power = 5;
	g_Team[i].diffend = 3;
	g_Team[i].blockMax = 2;
	g_Team[i].blockNum = 0;
	g_Team[i].startNum = GetPlayerPartsNum();
	g_Team[i].partsNum = 0;
	g_Team[i].atFrame = 50;
	//アニメーションデータのセット
	g_Team[i].tbl_adrA = neutro_Attack;	//先頭アドレスの指定なため、添え字はi
	g_Team[i].tbl_sizeA = sizeof(neutro_Attack)/sizeof(INTERPOLATION_DATA);	//データサイズ
	g_Team[i].tbl_adrM = neutro_Standby;	//先頭アドレスの指定なため、添え字はi
	g_Team[i].tbl_sizeM = sizeof(neutro_Standby) / sizeof(INTERPOLATION_DATA);;	//データサイズ
	//パーツ情報の初期化処理
	for (int k = g_Team[i].startNum; k < g_Team[i].partsNum; k++)
	{
		//LoadModel(MODEL_GRAPE_PARTS, &g_Parts[partsNum].model);
		//// モデルのディフューズを保存しておく。色変え対応の為。
		//GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);

		// 階層アニメーション用のメンバー変数
		g_Parts[k].tbl_adrA = NULL;	// アニメデータのテーブル先頭アドレス
		g_Parts[k].tbl_sizeA = 0;	// 登録したテーブルのレコード総数
		g_Parts[k].tbl_adrM = NULL;	// アニメデータのテーブル先頭アドレス
		g_Parts[k].tbl_sizeM = 0;	// 登録したテーブルのレコード総数
		g_Parts[k].move_time = 0;	// 実行時間
		SetPlayerNum(1);
	}
}

void SetChara(int charID)
{
	switch (charID)
	{

	}
}


PlayerStatus* GetTeam(void)
{
	return &g_Team[0];
}

PlayerPartsStatus* GetTeamParts(void)
{
	return &g_Parts[0];
}

//デバッグ用。好中球をチームメンバーとしてセット
void SetNeutro(int i)
{
	LoadModel(MODEL_NEUTROPHILS, &g_Team[i].model);
	// モデルのディフューズを保存しておく。色変え対応の為。
	GetModelDiffuse(&g_Team[i].model, &g_Team[i].diffuse[0]);
	g_Team[i].use = TRUE;
	g_Team[i].charID = ID_Neutro;
	g_Team[i].charType = LowPlaces;
	g_Team[i].scl = { 1.0f, 1.0f, 1.0f };
	g_Team[i].size = PLAYER_SIZE;	// 当たり判定の大きさ
	g_Team[i].life = 100;
	g_Team[i].lifeMax = g_Team[i].life;
	g_Team[i].power = 5;
	g_Team[i].diffend = 3;
	g_Team[i].blockMax = 2;
	g_Team[i].blockNum = 0;
	g_Team[i].startNum = GetPlayerPartsNum();
	g_Team[i].partsNum = 0;
	g_Team[i].atFrame = 45;
	//アニメーションデータのセット
	g_Team[i].tbl_adrA = neutro_Attack;	//先頭アドレスの指定なため、添え字はi
	g_Team[i].tbl_sizeA = sizeof(neutro_Attack) / sizeof(INTERPOLATION_DATA);	//データサイズ
	g_Team[i].tbl_adrM = neutro_Standby;	//先頭アドレスの指定なため、添え字はi
	g_Team[i].tbl_sizeM = sizeof(neutro_Standby) / sizeof(INTERPOLATION_DATA);;	//データサイズ
}

//デバッグ用。マクロファージをチームメンバーとしてセット
void SetMacrophages(int i)
{
	LoadModel(MODEL_MACRO, &g_Team[i].model);
	// モデルのディフューズを保存しておく。色変え対応の為。
	GetModelDiffuse(&g_Team[i].model, &g_Team[i].diffuse[0]);
	g_Team[i].use = TRUE;
	g_Team[i].charID = ID_Macro;
	g_Team[i].charType = LowPlaces;
	g_Team[i].scl = { 1.0f, 1.0f, 1.0f };
	g_Team[i].size = PLAYER_SIZE;	// 当たり判定の大きさ
	g_Team[i].life = 100;
	g_Team[i].lifeMax = g_Team[i].life;
	g_Team[i].power = 5;
	g_Team[i].diffend = 3;
	g_Team[i].blockMax = 2;
	g_Team[i].blockNum = 0;
	g_Team[i].startNum = GetPlayerPartsNum();
	g_Team[i].partsNum = 2;
	g_Team[i].atFrame = 20;
	//アニメーションデータのセット
	g_Team[i].tbl_adrA = macro_Attack;	//先頭アドレスの指定なため、添え字はi
	g_Team[i].tbl_sizeA = sizeof(macro_Attack) / sizeof(INTERPOLATION_DATA);	//データサイズ
	g_Team[i].tbl_adrM = macro_Standby;	//先頭アドレスの指定なため、添え字はi
	g_Team[i].tbl_sizeM = sizeof(macro_Standby) / sizeof(INTERPOLATION_DATA);	//データサイズ
	
																				//パーツ情報の初期化処理
	LoadModel(MODEL_MACRO_ARM, &g_Parts[GetPlayerPartsNum()].model);
	// モデルのディフューズを保存しておく。色変え対応の為。
	GetModelDiffuse(&g_Parts[GetPlayerPartsNum()].model, &g_Parts[GetPlayerPartsNum()].diffuse[0]);

	// 階層アニメーション用のメンバー変数
	g_Parts[GetPlayerPartsNum()].tbl_adrA = macro_ArmAt;	// アニメデータのテーブル先頭アドレス
	g_Parts[GetPlayerPartsNum()].tbl_sizeA = sizeof(macro_ArmAt) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
	g_Parts[GetPlayerPartsNum()].tbl_adrM = macro_ArmSb;	// アニメデータのテーブル先頭アドレス
	g_Parts[GetPlayerPartsNum()].tbl_sizeM = sizeof(macro_ArmSb) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
	g_Parts[GetPlayerPartsNum()].move_time = 0;	// 実行時間
	SetPlayerPartsNum(1);

	LoadModel(MODEL_MACRO_LEG, &g_Parts[GetPlayerPartsNum()].model);
	// モデルのディフューズを保存しておく。色変え対応の為。
	GetModelDiffuse(&g_Parts[GetPlayerPartsNum()].model, &g_Parts[GetPlayerPartsNum()].diffuse[0]);

	// 階層アニメーション用のメンバー変数
	g_Parts[GetPlayerPartsNum()].tbl_adrA = macro_LegAt;	// アニメデータのテーブル先頭アドレス
	g_Parts[GetPlayerPartsNum()].tbl_sizeA = sizeof(macro_LegAt) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
	g_Parts[GetPlayerPartsNum()].tbl_adrM = macro_LegSb;	// アニメデータのテーブル先頭アドレス
	g_Parts[GetPlayerPartsNum()].tbl_sizeM = sizeof(macro_LegSb) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
	g_Parts[GetPlayerPartsNum()].move_time = 0;	// 実行時間
	SetPlayerPartsNum(1);

}