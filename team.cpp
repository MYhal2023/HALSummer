//=============================================================================
//
// 編成処理 [team.cpp]
// Author : 
//
//=============================================================================
#include "team.h"
#include "player.h"
#include "playerLinerData.h"
#include "sound.h"
#include <string>
static PlayerStatus g_Team[MAX_PLAYER_SET];
//素材ID、必要量×4(4回までレベルアップするから)の順番。
static NeedMaterial Neutrovalue[MAX_MATERIAL] = { {(energy), {200, 400, 500, 800}},
										{(iron),{0, 1, 0, 1} },
										{(99), {0, 0, 0, 0}}/*ダミ－データ */ };
static NeedMaterial Macrovalue[MAX_MATERIAL] = { {(energy), {200, 400, 500, 800}},
										{(oxygen),{5, 10, 10, 15} },
										{(iron), {0, 1, 0, 1}} };
static NeedMaterial HelperTvalue[MAX_MATERIAL] = { {(energy), {300, 450, 600, 750}},
										{(oxygen),{5, 10, 10, 15} },
										{(iron), {0, 1, 0, 1}} };
static NeedMaterial KillerTvalue[MAX_MATERIAL] = { {(energy), {300, 400, 500, 600}},
										{(oxygen),{10, 20, 20, 25} },
										{(iron), {0, 1, 0, 2}} };
static NeedMaterial NKvalue[MAX_MATERIAL] = { {(energy), {300, 400, 500, 600}},
										{(iron),{1, 2, 1, 2} },
										{(99), {0, 0, 0, 0}}/*ダミ－データ */ };
static NeedMaterial Kouenvalue[MAX_MATERIAL] = { {(energy), {300, 500, 700, 1000}},
										{(oxygen),{20, 30, 20, 30} },
										{(iron), {1, 1, 1, 2}} };

static PlayerPartsStatus g_Parts [ MAX_PLAYER_SET * 2];
static char name[2][64];
static int memberNum;
static int partsNum;
static BOOL g_Load = FALSE;

void InitTeam(void)
{
	strcpy(name[MainPlayer], MODEL_PLAYER);
	strcpy(name[Neutrophils], MODEL_NEUTROPHILS);
	memberNum = 0;
	partsNum = 0;
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
	memberNum++;
}

void SetChara(int charID)
{
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
	g_Team[i].size = 50.0f;	// 当たり判定の大きさ
	g_Team[i].level = 1;
	int Life[MAX_LEVEL] = { 40, 45, 60, 65, 80 };
	int Power[MAX_LEVEL] = { 6, 7, 10, 12, 15 };
	int Diffend[MAX_LEVEL] = { 3, 4, 5, 7, 10 };
	int spMax[MAX_LEVEL] = { 15, 15, 15, 15, 12 };
	int cost[MAX_LEVEL] = { 8, 8, 7, 7, 6 };
	for (int k = 0; k < MAX_LEVEL; k++){
		g_Team[i].lifeMax[k] = Life[k];
		g_Team[i].power[k] = Power[k];
		g_Team[i].diffend[k] = Diffend[k];
		g_Team[i].spMax[k] = spMax[k];
		g_Team[i].cost[k] = cost[k];
	}
	g_Team[i].life = g_Team[i].lifeMax[0];
	g_Team[i].blockMax = 2;
	g_Team[i].blockNum = 0;
	g_Team[i].startNum = 0;
	g_Team[i].partsNum = 0;
	g_Team[i].atFrame = 45;
	g_Team[i].skillID = neutro_skill;
	g_Team[i].setAble = FALSE;
	g_Team[i].attackSE = SOUND_LABEL_SE_Panch;
	//強化必要素材の保存
	g_Team[i].material = Neutrovalue;


	//アニメーションデータのセット
	g_Team[i].tbl_adrA = neutro_Attack;	//先頭アドレスの指定なため、添え字はi
	g_Team[i].tbl_sizeA = sizeof(neutro_Attack) / sizeof(INTERPOLATION_DATA);	//データサイズ
	g_Team[i].tbl_adrM = neutro_Standby;	//先頭アドレスの指定なため、添え字はi
	g_Team[i].tbl_sizeM = sizeof(neutro_Standby) / sizeof(INTERPOLATION_DATA);;	//データサイズ
	memberNum++;
}

//デバッグ用。マクロファージをチームメンバーとしてセット
void SetMacrophages(int i)
{
	LoadModel(MODEL_MACRO, &g_Team[i].model);
	// モデルのディフューズを保存しておく。色変え対応の為。
	GetModelDiffuse(&g_Team[i].model, &g_Team[i].diffuse[0]);
	g_Team[i].use = TRUE;
	g_Team[i].charID = ID_Macro;
	g_Team[i].skillID = macro_skill;
	g_Team[i].skillTimeMax = 600;	//10秒間
	g_Team[i].charType = LowPlaces;
	g_Team[i].scl = { 1.0f, 1.0f, 1.0f };
	g_Team[i].size = 35.0f;	// 当たり判定の大きさ
	g_Team[i].level = 1;
	int Life[MAX_LEVEL] = { 80, 95, 120, 130, 150 };
	int Power[MAX_LEVEL] = { 3, 4, 5, 6, 7 };
	int Diffend[MAX_LEVEL] = { 5, 7, 10, 12, 15 };
	int spMax[MAX_LEVEL] = { 20, 20, 18, 18, 18 };
	int cost[MAX_LEVEL] = { 15, 15, 15, 15, 13 };
	for (int k = 0; k < MAX_LEVEL; k++) {
		g_Team[i].lifeMax[k] = Life[k];
		g_Team[i].power[k] = Power[k];
		g_Team[i].diffend[k] = Diffend[k];
		g_Team[i].spMax[k] = spMax[k];
		g_Team[i].cost[k] = cost[k];
	}

	g_Team[i].life = g_Team[i].lifeMax[g_Team[i].level - 1];
	g_Team[i].blockMax = 3;
	g_Team[i].blockNum = 0;
	g_Team[i].startNum = partsNum;
	g_Team[i].partsNum = 2;
	g_Team[i].atFrame = 25;
	g_Team[i].setAble = FALSE;

	g_Team[i].material = Macrovalue;
	g_Team[i].attackSE = SOUND_LABEL_SE_Kick;

	//アニメーションデータのセット
	g_Team[i].tbl_adrA = macro_Attack;	//先頭アドレスの指定なため、添え字はi
	g_Team[i].tbl_sizeA = sizeof(macro_Attack) / sizeof(INTERPOLATION_DATA);	//データサイズ
	g_Team[i].tbl_adrM = macro_Standby;	//先頭アドレスの指定なため、添え字はi
	g_Team[i].tbl_sizeM = sizeof(macro_Standby) / sizeof(INTERPOLATION_DATA);	//データサイズ
	
																				//パーツ情報の初期化処理
	LoadModel(MODEL_MACRO_ARM, &g_Parts[partsNum].model);
	// モデルのディフューズを保存しておく。色変え対応の為。
	GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);

	// 階層アニメーション用のメンバー変数
	g_Parts[partsNum].tbl_adrA = macro_ArmAt;	// アニメデータのテーブル先頭アドレス
	g_Parts[partsNum].tbl_sizeA = sizeof(macro_ArmAt) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
	g_Parts[partsNum].tbl_adrM = macro_ArmSb;	// アニメデータのテーブル先頭アドレス
	g_Parts[partsNum].tbl_sizeM = sizeof(macro_ArmSb) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
	g_Parts[partsNum].move_time = 0;	// 実行時間
	partsNum++;


	LoadModel(MODEL_MACRO_LEG, &g_Parts[partsNum].model);
	// モデルのディフューズを保存しておく。色変え対応の為。
	GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);

	// 階層アニメーション用のメンバー変数
	g_Parts[partsNum].tbl_adrA = macro_LegAt;	// アニメデータのテーブル先頭アドレス
	g_Parts[partsNum].tbl_sizeA = sizeof(macro_LegAt) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
	g_Parts[partsNum].tbl_adrM = macro_LegSb;	// アニメデータのテーブル先頭アドレス
	g_Parts[partsNum].tbl_sizeM = sizeof(macro_LegSb) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
	g_Parts[partsNum].move_time = 0;	// 実行時間
	partsNum++;
	memberNum++;
}

//デバッグ用。
void SetHelpeerT(int i)
{
	LoadModel(MODEL_HELPT, &g_Team[i].model);
	// モデルのディフューズを保存しておく。色変え対応の為。
	GetModelDiffuse(&g_Team[i].model, &g_Team[i].diffuse[0]);
	g_Team[i].use = TRUE;
	g_Team[i].charID = ID_HelperT;
	g_Team[i].skillID = helperT_skill;
	g_Team[i].skillTimeMax = 600;	//15秒間
	g_Team[i].charType = HighPlaces;
	g_Team[i].scl = { 1.0f, 1.0f, 1.0f };
	g_Team[i].size = 150.0f;	// 当たり判定の大きさ
	g_Team[i].level = 1;
	int Life[MAX_LEVEL] = { 30, 35, 50, 60, 70 };
	int Power[MAX_LEVEL] = { 4, 6, 8, 10, 12 };
	int Diffend[MAX_LEVEL] = { 2, 3, 5, 6, 7 };
	int spMax[MAX_LEVEL] = { 30, 30, 28, 28, 25 };
	int cost[MAX_LEVEL] = { 18, 18, 18, 18, 15 };
	for (int k = 0; k < MAX_LEVEL; k++) {
		g_Team[i].lifeMax[k] = Life[k];
		g_Team[i].power[k] = Power[k];
		g_Team[i].diffend[k] = Diffend[k];
		g_Team[i].spMax[k] = spMax[k];
		g_Team[i].cost[k] = cost[k];
	}

	g_Team[i].life = g_Team[i].lifeMax[g_Team[i].level - 1];
	g_Team[i].blockMax = 0;
	g_Team[i].blockNum = 0;
	g_Team[i].startNum = partsNum;
	g_Team[i].partsNum = 1;
	g_Team[i].atFrame = 59;
	g_Team[i].setAble = FALSE;

	g_Team[i].material = HelperTvalue;
	g_Team[i].attackSE = SOUND_LABEL_SE_Hit;

	//アニメーションデータのセット
	g_Team[i].tbl_adrA = helperT_Attack;	//先頭アドレスの指定なため、添え字はi
	g_Team[i].tbl_sizeA = sizeof(helperT_Attack) / sizeof(INTERPOLATION_DATA);	//データサイズ
	g_Team[i].tbl_adrM = helperT_Standby;	//先頭アドレスの指定なため、添え字はi
	g_Team[i].tbl_sizeM = sizeof(helperT_Standby) / sizeof(INTERPOLATION_DATA);	//データサイズ

																				//パーツ情報の初期化処理
	LoadModel(MODEL_HELPTTOP, &g_Parts[partsNum].model);
	// モデルのディフューズを保存しておく。色変え対応の為。
	GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);

	// 階層アニメーション用のメンバー変数
	g_Parts[partsNum].tbl_adrA = helperT_TopAt;	// アニメデータのテーブル先頭アドレス
	g_Parts[partsNum].tbl_sizeA = sizeof(helperT_TopAt) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
	g_Parts[partsNum].tbl_adrM = helperT_TopSb;	// アニメデータのテーブル先頭アドレス
	g_Parts[partsNum].tbl_sizeM = sizeof(helperT_TopSb) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
	g_Parts[partsNum].move_time = 0;	// 実行時間
	partsNum++;

	memberNum++;
}

//デバッグ用。
void SetKillerT(int i)
{
	LoadModel(MODEL_KILLT, &g_Team[i].model);
	// モデルのディフューズを保存しておく。色変え対応の為。
	GetModelDiffuse(&g_Team[i].model, &g_Team[i].diffuse[0]);
	g_Team[i].use = TRUE;
	g_Team[i].charID = ID_KillerT;
	g_Team[i].skillID = killerT_skill;
	g_Team[i].skillTimeMax = 0;	
	g_Team[i].charType = LowPlaces;
	g_Team[i].scl = { 1.2f, 1.2f, 1.2f };
	g_Team[i].size = 50.0f;	// 当たり判定の大きさ
	g_Team[i].level = 1;
	int Life[MAX_LEVEL] = { 45, 50, 65, 80, 100 };
	int Power[MAX_LEVEL] = { 10, 12, 14, 16, 20 };
	int Diffend[MAX_LEVEL] = { 3, 3, 5, 5, 8 };
	int spMax[MAX_LEVEL] = { 10, 10, 8, 8, 6 };
	int cost[MAX_LEVEL] = { 12, 11, 10, 10, 10 };
	for (int k = 0; k < MAX_LEVEL; k++) {
		g_Team[i].lifeMax[k] = Life[k];
		g_Team[i].power[k] = Power[k];
		g_Team[i].diffend[k] = Diffend[k];
		g_Team[i].spMax[k] = spMax[k];
		g_Team[i].cost[k] = cost[k];
	}

	g_Team[i].life = g_Team[i].lifeMax[g_Team[i].level - 1];
	g_Team[i].blockMax = 1;
	g_Team[i].blockNum = 0;
	g_Team[i].startNum = partsNum;
	g_Team[i].partsNum = 1;
	g_Team[i].atFrame = 59;
	g_Team[i].setAble = FALSE;
	g_Team[i].attackSE = SOUND_LABEL_SE_Hit;
	g_Team[i].material = KillerTvalue;


	//アニメーションデータのセット
	g_Team[i].tbl_adrA = killerT_Attack;	//先頭アドレスの指定なため、添え字はi
	g_Team[i].tbl_sizeA = sizeof(killerT_Attack) / sizeof(INTERPOLATION_DATA);	//データサイズ
	g_Team[i].tbl_adrM = killerT_Standby;	//先頭アドレスの指定なため、添え字はi
	g_Team[i].tbl_sizeM = sizeof(killerT_Standby) / sizeof(INTERPOLATION_DATA);	//データサイズ

																				//パーツ情報の初期化処理
	LoadModel(MODEL_KILLAXE, &g_Parts[partsNum].model);
	// モデルのディフューズを保存しておく。色変え対応の為。
	GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);

	// 階層アニメーション用のメンバー変数
	g_Parts[partsNum].tbl_adrA = killerT_AxeAt;	// アニメデータのテーブル先頭アドレス
	g_Parts[partsNum].tbl_sizeA = sizeof(killerT_AxeAt) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
	g_Parts[partsNum].tbl_adrM = killerT_AxeSb;	// アニメデータのテーブル先頭アドレス
	g_Parts[partsNum].tbl_sizeM = sizeof(killerT_AxeSb) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
	g_Parts[partsNum].move_time = 0;	// 実行時間
	partsNum++;

	memberNum++;
}

//デバッグ用。
void SetNK(int i)
{
	LoadModel(MODEL_NK, &g_Team[i].model);
	// モデルのディフューズを保存しておく。色変え対応の為。
	GetModelDiffuse(&g_Team[i].model, &g_Team[i].diffuse[0]);
	g_Team[i].use = TRUE;
	g_Team[i].charID = ID_NK;
	g_Team[i].skillID = NK_skill;
	g_Team[i].skillTimeMax = 600;
	g_Team[i].charType = HighPlaces;
	g_Team[i].scl = { 1.0f, 1.0f, 1.0f };
	g_Team[i].size = 180.0f;	// 当たり判定の大きさ
	g_Team[i].level = 1;
	int Life[MAX_LEVEL] = { 35, 40, 50, 55, 70 };
	int Power[MAX_LEVEL] = { 6, 8, 10, 11, 12 };
	int Diffend[MAX_LEVEL] = { 2, 3, 4, 5, 8 };
	int spMax[MAX_LEVEL] = { 30, 29, 28, 27, 25 };
	int cost[MAX_LEVEL] = { 14, 14, 13, 13, 12 };
	for (int k = 0; k < MAX_LEVEL; k++) {
		g_Team[i].lifeMax[k] = Life[k];
		g_Team[i].power[k] = Power[k];
		g_Team[i].diffend[k] = Diffend[k];
		g_Team[i].spMax[k] = spMax[k];
		g_Team[i].cost[k] = cost[k];
	}

	g_Team[i].life = g_Team[i].lifeMax[g_Team[i].level - 1];
	g_Team[i].blockMax = 0;
	g_Team[i].blockNum = 0;
	g_Team[i].startNum = partsNum;
	g_Team[i].partsNum = 0;
	g_Team[i].atFrame = 79;
	g_Team[i].setAble = FALSE;

	g_Team[i].material = NKvalue;
	g_Team[i].attackSE = SOUND_LABEL_SE_Cannon;


	//アニメーションデータのセット
	g_Team[i].tbl_adrA = NK_Attack;	//先頭アドレスの指定なため、添え字はi
	g_Team[i].tbl_sizeA = sizeof(NK_Attack) / sizeof(INTERPOLATION_DATA);	//データサイズ
	g_Team[i].tbl_adrM = NK_Standby;	//先頭アドレスの指定なため、添え字はi
	g_Team[i].tbl_sizeM = sizeof(NK_Standby) / sizeof(INTERPOLATION_DATA);	//データサイズ

	memberNum++;
}
//デバッグ用。
void SetKouen(int i)
{
	LoadModel(MODEL_KOUEN, &g_Team[i].model);
	// モデルのディフューズを保存しておく。色変え対応の為。
	GetModelDiffuse(&g_Team[i].model, &g_Team[i].diffuse[0]);
	g_Team[i].use = TRUE;
	g_Team[i].charID = ID_Kouen;
	g_Team[i].skillID = kouen_skill;
	g_Team[i].skillTimeMax = 600;
	g_Team[i].charType = HighPlaces;
	g_Team[i].scl = { 1.0f, 1.0f, 1.0f };
	g_Team[i].size = 180.0f;	// 当たり判定の大きさ
	g_Team[i].level = 1;
	int Life[MAX_LEVEL] = { 40, 40, 50, 50, 60 };
	int Power[MAX_LEVEL] = { 3, 3, 5, 6, 8 };
	int Diffend[MAX_LEVEL] = { 3, 3, 4, 5, 8 };
	int spMax[MAX_LEVEL] = { 40, 38, 37, 36, 34 };
	int cost[MAX_LEVEL] = { 25, 24, 22, 21, 20 };
	for (int k = 0; k < MAX_LEVEL; k++) {
		g_Team[i].lifeMax[k] = Life[k];
		g_Team[i].power[k] = Power[k];
		g_Team[i].diffend[k] = Diffend[k];
		g_Team[i].spMax[k] = spMax[k];
		g_Team[i].cost[k] = cost[k];
	}

	g_Team[i].life = g_Team[i].lifeMax[g_Team[i].level - 1];
	g_Team[i].blockMax = 0;
	g_Team[i].blockNum = 0;
	g_Team[i].startNum = partsNum;
	g_Team[i].partsNum = 2;
	g_Team[i].atFrame = 5;
	g_Team[i].setAble = FALSE;

	g_Team[i].material = Kouenvalue;
	g_Team[i].attackSE = SOUND_LABEL_SE_MachineGun;


	//アニメーションデータのセット
	g_Team[i].tbl_adrA = Kouen_Attack;	//先頭アドレスの指定なため、添え字はi
	g_Team[i].tbl_sizeA = sizeof(Kouen_Attack) / sizeof(INTERPOLATION_DATA);	//データサイズ
	g_Team[i].tbl_adrM = Kouen_Standby;	//先頭アドレスの指定なため、添え字はi
	g_Team[i].tbl_sizeM = sizeof(Kouen_Standby) / sizeof(INTERPOLATION_DATA);	//データサイズ
	
	
	LoadModel(MODEL_KOUEN_ARM001, &g_Parts[partsNum].model);
	// モデルのディフューズを保存しておく。色変え対応の為。
	GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);

	// 階層アニメーション用のメンバー変数
	g_Parts[partsNum].tbl_adrA = Kouen_Arm_Attack;	// アニメデータのテーブル先頭アドレス
	g_Parts[partsNum].tbl_sizeA = sizeof(Kouen_Arm_Attack) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
	g_Parts[partsNum].tbl_adrM = Kouen_Arm_Standby;	// アニメデータのテーブル先頭アドレス
	g_Parts[partsNum].tbl_sizeM = sizeof(Kouen_Arm_Standby) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
	g_Parts[partsNum].move_time = 0;	// 実行時間
	partsNum++;


	LoadModel(MODEL_KOUEN_ARM002, &g_Parts[partsNum].model);
	// モデルのディフューズを保存しておく。色変え対応の為。
	GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);

	// 階層アニメーション用のメンバー変数
	g_Parts[partsNum].tbl_adrA = Kouen_Arm2_Attack;	// アニメデータのテーブル先頭アドレス
	g_Parts[partsNum].tbl_sizeA = sizeof(Kouen_Arm2_Attack) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
	g_Parts[partsNum].tbl_adrM = Kouen_Arm2_Standby;	// アニメデータのテーブル先頭アドレス
	g_Parts[partsNum].tbl_sizeM = sizeof(Kouen_Arm2_Standby) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
	g_Parts[partsNum].move_time = 0;	// 実行時間
	partsNum++;

	memberNum++;
}

int GetMemberNum(void)
{
	return memberNum;
}
