//=============================================================================
//
// バトル中のプレイヤー処理 [enemy.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "model.h"
#include "game.h"
#include "enemy.h"
#include "shadow.h"
#include "light.h"
#include "meshfield.h"
#include "collision.h"
#include "time.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VALUE_MOVE			(2.0f)							// 移動量
#define	VALUE_AT_MOVE		(4.0f)							// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// 回転量

#define ENEMY_SHADOW_SIZE	(1.0f)							// 影の大きさ
#define ENEMY_OFFSET_Y		(0.0f)							// プレイヤーの足元をあわせる
#define ENEMY_OFFSET_Z		(-300.0f)							// プレイヤーの足元をあわせる
#define ENEMY_LIFE			(4)								// プレイヤーのライフ

#define ENEMY_PARTS_MAX	(1)								// プレイヤーのパーツの数
#define ENEMY_AT_FLAME		(30.0f)							// プレイヤーの攻撃フレーム
#define ENEMY_INVINC_FLAME	(120.0f)						// プレイヤー無敵フレーム

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ENEMY		g_Enemy[MAX_ENEMY];						// プレイヤー

static BOOL			g_Load = FALSE;
static int			atCount;
static int			enemyNum;					//何体のエネミーがいるか

// プレイヤーの階層アニメーションデータ
static INTERPOLATION_DATA move_tbl_right[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(20.0f, 15.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(20.0f, 15.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		 XMFLOAT3(1.0f, 1.0f, 1.0f), 180 },
	{ XMFLOAT3(20.0f, 15.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA move_tbl_left[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-20.0f, 10.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),         XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-20.0f, 10.0f, 0.0f), XMFLOAT3(XM_PI / 2, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 180 },
	{ XMFLOAT3(-20.0f, 10.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),         XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		LoadModel(MODEL_ENEMY, &g_Enemy[i].model);
		// モデルのディフューズを保存しておく。色変え対応の為。
		GetModelDiffuse(&g_Enemy[i].model, &g_Enemy[i].diffuse[0]);

		g_Enemy[i].load = TRUE;

		g_Enemy[i].pos = { 0.0f, ENEMY_OFFSET_Y, 0.0f };
		g_Enemy[i].rot = { 0.0f, 0.0f, 0.0f };
		g_Enemy[i].scl = { 0.8f, 1.0f, 1.0f };

		g_Enemy[i].size = ENEMY_SIZE;	// 当たり判定の大きさ
		g_Enemy[i].life = ENEMY_LIFE;
		g_Enemy[i].lifeMax = g_Enemy[i].life;
		g_Enemy[i].use = TRUE;
		g_Enemy[i].attack = FALSE;
		g_Enemy[i].attackUse = FALSE;

		// 階層アニメーション用の初期化処理
		g_Enemy[i].parent = NULL;			// 本体（親）なのでNULLを入れる
	}
	enemyNum = 3;
	g_Load = TRUE;
	atCount = 0;
	return S_OK;

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	if (g_Load == FALSE) return;

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		// モデルの解放処理
		if (g_Enemy[i].load)
		{
			UnloadModel(&g_Enemy[i].model);
			g_Enemy[i].load = FALSE;
		}
	}
	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	for (int i = 0; i < enemyNum; i++)
	{
		//ルールベースで敵を攻撃し、ステートベースで遷移を行う。線形補間で移動
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	for (int i = 0; i < enemyNum; i++)
	{

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Enemy[i].mtxWorld, mtxWorld);

		// モデル描画
		DrawModel(&g_Enemy[i].model);
	}


	//// パーツの階層アニメーション
	//for (int i = 0; i < ENEMY_PARTS_MAX; i++)
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
	//		// g_Enemy[i].mtxWorldを指している
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
ENEMY *GetEnemy(void)
{
	return &g_Enemy[0];
}