//=============================================================================
//
// フィールドモデル処理 [fieldchip.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "model.h"
#include "game.h"
#include "player.h"
#include "shadow.h"
#include "light.h"
#include "meshfield.h"
#include "collision.h"
#include "time.h"
#include "fieldchip.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static MAP_CHIP		g_MapChip[MAX_CHIP_NUM];						// プレイヤー
static MAP_CHIP		g_MapSet[MAX_CHIP_HEIGHT][MAX_CHIP_WIDTH + 1];
static BOOL			g_Load = FALSE;

//ここで使用するマップチップナンバーを設定
int g_DebugMap[MAX_CHIP_HEIGHT][MAX_CHIP_WIDTH + 1]
{
	{1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,0},
	{1,1,1,1,1,1,1,1},
};
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMapChip(void)
{
	for (int i = 0; i < MAX_CHIP_NUM; i++)
	{
		switch (i)
		{
		case ROW:
			LoadModel(MODEL_FIELD001, &g_MapChip[i].model);
			break;
		case HIGH:
			LoadModel(MODEL_FIELD002, &g_MapChip[i].model);
			break;
		case MAX_VAL:
			break;
		}
		// モデルのディフューズを保存しておく。色変え対応の為。
		GetModelDiffuse(&g_MapChip[i].model, &g_MapChip[i].diffuse[0]);

		g_MapChip[i].load = TRUE;

		g_MapChip[i].pos = { 0.0f, 0.0f, 0.0f };
		g_MapChip[i].rot = { 0.0f, 0.0f, 0.0f };
		g_MapChip[i].scl = { 1.0f, 1.0f, 1.0f };

		g_MapChip[i].size = CHIP_SIZE;	// 当たり判定の大きさ
	}

	for (int i = 0; i < MAX_CHIP_HEIGHT; i++)
	{
		for (int k = 0; k < MAX_CHIP_WIDTH; k++)
		{
			g_MapSet[i][k].pos = { 0.0f + CHIP_SIZE * k, -1.0f,  0.0f + CHIP_SIZE * i };
			g_MapSet[i][k].rot = { 0.0f, 0.0f, 0.0f };
			g_MapSet[i][k].scl = { 1.7f, 1.0f, 1.7f };
		}
	}
	g_Load = TRUE;
	return S_OK;

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMapChip(void)
{
	if (g_Load == FALSE) return;

	for (int i = 0; i < MAX_CHIP_NUM; i++)
	{
		// モデルの解放処理
		if (g_MapChip[i].load)
		{
			UnloadModel(&g_MapChip[i].model);
			g_MapChip[i].load = FALSE;
		}
	}
	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMapChip(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMapChip(void)
{
	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	for (int i = 0; i < MAX_CHIP_HEIGHT; i++)
	{
		for (int k = 0; k < MAX_CHIP_WIDTH; k++)
		{
			int m = g_DebugMap[i][k];
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_MapSet[i][k].scl.x, g_MapSet[i][k].scl.y, g_MapSet[i][k].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_MapSet[i][k].rot.x, g_MapSet[i][k].rot.y + XM_PI, g_MapSet[i][k].rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_MapSet[i][k].pos.x, g_MapSet[i][k].pos.y, g_MapSet[i][k].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_MapSet[i][k].mtxWorld, mtxWorld);

			// モデル描画
			//ここだけ事前に用意したマップチップナンバーを参照
			DrawModel(&g_MapChip[m].model);
		}
	}


	//// パーツの階層アニメーション
	//for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	//{
	//	// ワールドマトリックスの初期化
	//	mtxWorld = XMMatrixIdentity();

	//	// スケールを反映
	//	mtxScl = XMMatrixScaling(g_Parts[i].scl.x, g_Parts[i].scl.y, g_Parts[i].scl.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	//	// 回転を反映
	//	mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i].rot.x, g_Parts[i].rot.y, g_Parts[i].rot.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	//	// 移動を反映
	//	mtxTranslate = XMMatrixTranslation(g_Parts[i].pos.x, g_Parts[i].pos.y, g_Parts[i].pos.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	//	if (g_Parts[i].parent != NULL)	// 子供だったら親と結合する
	//	{
	//		mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i].parent->mtxWorld));
	//		// ↑
	//		// g_MapChip[i].mtxWorldを指している
	//	}

	//	XMStoreFloat4x4(&g_Parts[i].mtxWorld, mtxWorld);

	//	// 使われているなら処理する。ここまで処理している理由は他のパーツがこのパーツを参照している可能性があるから。
	//	if (g_Parts[i].use == FALSE) continue;

	//	// ワールドマトリックスの設定
	//	SetWorldMatrix(&mtxWorld);


	//	// モデル描画
	//	DrawModel(&g_Parts[i].model);

	//}


	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// プレイヤー情報を取得
//=============================================================================
MAP_CHIP *GetMapChip(void)
{
	return &g_MapChip[0];
}