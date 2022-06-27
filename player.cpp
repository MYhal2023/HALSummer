//=============================================================================
//
// バトル中のプレイヤー処理 [player.cpp]
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
#include "enemy.h"
#include "base.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VALUE_MOVE			(2.0f)							// 移動量
#define	VALUE_AT_MOVE		(4.0f)							// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// 回転量

#define PLAYER_SHADOW_SIZE	(1.0f)							// 影の大きさ
#define PLAYER_OFFSET_Y		(5.0f)							// プレイヤーの足元をあわせる
#define PLAYER_OFFSET_Z		(-300.0f)							// プレイヤーの足元をあわせる
#define PLAYER_LIFE			(100)								// プレイヤーのライフ

#define PLAYER_PARTS_MAX	(1)								// プレイヤーのパーツの数
#define PLAYER_AT_FLAME		(30.0f)							// プレイヤーの攻撃フレーム
#define PLAYER_INVINC_FLAME	(120.0f)						// プレイヤー無敵フレーム

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static PLAYER		g_Player[MAX_PLAYER];						// プレイヤー
static PlayerParts	g_Parts[MAX_PLAYER * 2];						// プレイヤー
static BOOL			g_Load = FALSE;
static int			playerNum;
static int			partsNum;
// 階層アニメーションデータ
static INTERPOLATION_DATA neutro_Attack[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 10, },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	   XMFLOAT3(2.0f, 2.0f, 2.0f), 50 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(0.8f, 1.0f, 1.0f), 30 },
};



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		//LoadModel(MODEL_PLAYER, &g_Player[i].model);
		//// モデルのディフューズを保存しておく。色変え対応の為。
		//GetModelDiffuse(&g_Player[i].model, &g_Player[i].diffuse[0]);

		g_Player[i].pos = { 200.0f, PLAYER_OFFSET_Y, 100.0f };
		g_Player[i].rot = { 0.0f, -XM_PI * 0.5f, 0.0f };
		g_Player[i].scl = { 0.8f, 1.0f, 1.0f };

		g_Player[i].size = PLAYER_SIZE;	// 当たり判定の大きさ
		g_Player[i].use = FALSE;
		g_Player[i].life = PLAYER_LIFE;
		g_Player[i].lifeMax = g_Player[i].life;
		g_Player[i].power = 5;
		g_Player[i].diffend = 3;
		g_Player[i].attack = FALSE;
		g_Player[i].attackUse = FALSE;
		g_Player[i].blockMax = 2;
		g_Player[i].blockNum = 0;

		// 階層アニメーション用の初期化処理
		g_Player[i].parent = NULL;			// 本体（親）なのでNULLを入れる
	}
	g_Load = TRUE;
	playerNum = 0;
	return S_OK;

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	if (g_Load == FALSE) return;

	for (int i = 0; i < MAX_PLAYER; i++)
	{
		// モデルの解放処理
		if (g_Player[i].load)
		{
			UnloadModel(&g_Player[i].model);
			g_Player[i].load = FALSE;
		}
	}
	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	BlockEnemy();	//ブロック情報を更新

	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (g_Player[i].use != TRUE)continue;

		g_Player[i].StateCheck(i);
		CheckEnemyTarget(i);
		switch (g_Player[i].state)
		{
		case Standby:
			break;

		case Deffend:
			PlayerInterPoration(i);
			break;
		}

	}
#ifdef _DEBUG
	PrintDebugProc("プレイヤー体力:%d\n", g_Player[0].life);
	PrintDebugProc("プレイヤー体力:%d\n", g_Player[1].life);
	PrintDebugProc("プレイヤー状態:%d\n", g_Player[1].state);
	PrintDebugProc("プレイヤーターゲット:%d\n", g_Player[1].target);
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (g_Player[i].use != TRUE)continue;
		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Player[i].scl.x, g_Player[i].scl.y, g_Player[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Player[i].rot.x, g_Player[i].rot.y + XM_PI, g_Player[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Player[i].pos.x, g_Player[i].pos.y, g_Player[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Player[i].mtxWorld, mtxWorld);

		// モデル描画
		DrawModel(&g_Player[i].model);

		if (g_Player[i].partsNum == 0)continue;

		// パーツの階層アニメーション
		for (int k = g_Player[i].startNum; k < g_Player[i].startNum + g_Player[i].partsNum; k++)
		{
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_Parts[k].scl.x, g_Parts[k].scl.y, g_Parts[k].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[k].rot.x, g_Parts[k].rot.y, g_Parts[k].rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_Parts[k].pos.x, g_Parts[k].pos.y, g_Parts[k].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			if (g_Parts[k].parent != NULL)	// 子供だったら親と結合する
			{
				mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[k].parent->mtxWorld));
				// ↑
				// g_Player[i].mtxWorldを指している
			}

			XMStoreFloat4x4(&g_Parts[k].mtxWorld, mtxWorld);

			// 使われているなら処理する。ここまで処理している理由は他のパーツがこのパーツを参照している可能性があるから。
			if (g_Parts[k].load == FALSE) continue;

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			// モデル描画
			DrawModel(&g_Parts[k].model);
		}
	}

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}

//プレイヤーの線形補間。攻撃モーション中にここに来たらダメージ処理も行う
void PlayerInterPoration(int i)
{
	if (g_Player[i].tbl_adr == NULL)return;	// 線形補間を実行する？

	//
	// 線形補間の処理
	// 移動処理
	int		index = (int)g_Player[i].move_time;
	float	time = g_Player[i].move_time - index;
	int		size = g_Player[i].tbl_size;

	float dt = 1.0f / g_Player[i].tbl_adr[index].frame;	// 1フレームで進める時間
	g_Player[i].move_time += dt;							// アニメーションの合計時間に足す

	if (index > (size - 2))	// ゴールをオーバーしていたら、データを最初に戻して攻撃を終了
	{
		g_Player[i].move_time = 0.0f;
		index = 0;
		g_Player[i].atCount = 0;
		g_Player[i].state = Standby;
		g_Player[i].attackUse = FALSE;
	}

	// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
	XMVECTOR p1 = XMLoadFloat3(&g_Player[i].tbl_adr[index + 1].pos);	// 次の場所
	XMVECTOR p0 = XMLoadFloat3(&g_Player[i].tbl_adr[index + 0].pos);	// 現在の場所
	XMVECTOR vec = p1 - p0;
	XMStoreFloat3(&g_Player[i].pos, XMLoadFloat3(&g_Player[i].pos) + p0 + vec * time);

	// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
	XMVECTOR r1 = XMLoadFloat3(&g_Player[i].tbl_adr[index + 1].rot);	// 次の角度
	XMVECTOR r0 = XMLoadFloat3(&g_Player[i].tbl_adr[index + 0].rot);	// 現在の角度
	XMVECTOR rot = r1 - r0;
	XMStoreFloat3(&g_Player[i].rot, XMLoadFloat3(&g_Player[i].rot) + r0 + rot * time);

	// scaleを求める S = StartX + (EndX - StartX) * 今の時間
	XMVECTOR s1 = XMLoadFloat3(&g_Player[i].tbl_adr[index + 1].scl);	// 次のScale
	XMVECTOR s0 = XMLoadFloat3(&g_Player[i].tbl_adr[index + 0].scl);	// 現在のScale
	XMVECTOR scl = s1 - s0;
	XMStoreFloat3(&g_Player[i].scl, s0 + scl * time);

	//ここから攻撃処理
	if (g_Player[i].target == 99 || g_Player[i].attackUse == TRUE)return;	//セットしていない、セットする必要がない攻撃があるかも？
	g_Player[i].atFrameCount++;
	ENEMY *enemy = GetEnemy();
	//攻撃フレームに達したら、ダメージ計算関数を元にターゲットにダメージ
	if (g_Player[i].atFrameCount >= g_Player[i].atFrame) {
		enemy[g_Player[i].target].life -= DamageFunc(g_Player[i].power, enemy[g_Player[i].target].diffend);
		g_Player[i].atFrameCount = 0;
		g_Player[i].attackUse = TRUE;
	}
}

void BlockEnemy(void)
{
	ENEMY *enemy = GetEnemy();
	for(int k = 0; k < MAX_ENEMY; k++)
		enemy[k].blocked = FALSE;
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (g_Player[i].use != TRUE)continue;
		g_Player[i].blockNum = 0;	//ブロック数を0にリセット
		for (int k = 0; k < MAX_ENEMY; k++)
		{
			//もうブロック出来ない、ブロック不可能か使われてない、そもそも近接していないならスキップ
			//ブロックするための前提条件は上へ、ブロックするための条件は下へ記載
			if (g_Player[i].blockNum >= g_Player[i].blockMax ||
				enemy[k].type != Proximity ||
				enemy[k].use != TRUE)continue;
			if (!CollisionBC(g_Player[i].pos, enemy[k].pos, 10.0f, 10.0f))continue;

			//ここでエネミーを被ブロック状態へ変更する。攻撃先も自分へ
			g_Player[i].blockNum++;
			enemy[k].blocked = TRUE;
			enemy[k].target = &g_Player[i];
		}
	}
}

//敵キャラの攻撃ターゲット決定。一番近い敵を見つけて攻撃する
void CheckEnemyTarget(int i)
{
	//攻撃中ではないならここでターゲット決定はしない
	if (g_Player[i].state != Deffend) {
		g_Player[i].target = 99;
		return;
	}
	ENEMY *enemy = GetEnemy();
	Base *base = GetBase();
	float cmp = 0.0f;;
	for (int k = 0; k < g_Player[i].count; k++)
	{
		if (g_Player[i].targetable[k] == 99)continue;
		for (int j = 0; j < base->baseNum; j++)
		{
			XMVECTOR v1 = XMLoadFloat3(&base->pos[j]) - XMLoadFloat3(&enemy[g_Player[i].targetable[k]].pos);
			XMFLOAT3 countData;
			XMStoreFloat3(&countData, v1);
			float dist = fabsf(countData.x) + fabsf(countData.y) + fabsf(countData.z);
			if (dist > cmp)
			{
				cmp = dist;
				g_Player[i].target = g_Player[i].targetable[k];	//エネミーの配列番号で識別。ポインターで渡したいけど、お互いの構造体にポインターメンバ変数を入れると怒られる…
			}
		}
	}
}

void SetPlayer(XMFLOAT3 pos)
{
	LoadModel(MODEL_PLAYER, &g_Player[playerNum].model);
	// モデルのディフューズを保存しておく。色変え対応の為。
	GetModelDiffuse(&g_Player[playerNum].model, &g_Player[playerNum].diffuse[0]);

	g_Player[playerNum].load = TRUE;

	g_Player[playerNum].pos = pos;
	g_Player[playerNum].rot = { 0.0f, XM_PI * -0.5f, 0.0f };
	g_Player[playerNum].scl = { 0.8f, 1.0f, 1.0f };

	g_Player[playerNum].size = PLAYER_SIZE;	// 当たり判定の大きさ
	g_Player[playerNum].use = TRUE;
	g_Player[playerNum].life = PLAYER_LIFE;
	g_Player[playerNum].lifeMax = g_Player[playerNum].life;
	g_Player[playerNum].power = 5;
	g_Player[playerNum].diffend = 3;
	g_Player[playerNum].attack = FALSE;
	g_Player[playerNum].attackUse = FALSE;
	g_Player[playerNum].blockMax = 2;
	g_Player[playerNum].blockNum = 0;
	g_Player[playerNum].target = 99;
	for (int i = 0; i < MAX_TARGET; i++)
		g_Player[playerNum].targetable[i] = 99;
	g_Player[playerNum].count = 0;
	g_Player[playerNum].startNum = partsNum;
	g_Player[playerNum].partsNum = 0;

	// 階層アニメーション用の初期化処理
	g_Player[playerNum].parent = NULL;			// 本体（親）なのでNULLを入れる
	playerNum++;

	//パーツの初期化処理
	for (int k = 0; k < g_Player[playerNum].partsNum; k++)
	{
		LoadModel(MODEL_GRAPE_PARTS, &g_Parts[partsNum].model);
		// モデルのディフューズを保存しておく。色変え対応の為。
		GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);
		g_Parts[partsNum].load = TRUE;

		g_Parts[partsNum].pos = { 20.0f, 10.0f, 0.0f };		// ポリゴンの位置
		g_Parts[partsNum].rot = { 0.0f, 0.0f, 0.0f };		// ポリゴンの向き(回転)
		g_Parts[partsNum].scl = { 1.0f, 1.0f, 1.0f };		// ポリゴンの大きさ(スケール)

		// 階層アニメーション用のメンバー変数
		g_Parts[partsNum].tbl_adr = NULL;	// アニメデータのテーブル先頭アドレス
		g_Parts[partsNum].tbl_size = 0;	// 登録したテーブルのレコード総数
		g_Parts[partsNum].move_time = 0;	// 実行時間
		g_Parts[partsNum].parent = &g_Player[playerNum];	// 自分が親ならNULL、自分が子供なら親のenemyアドレス
		partsNum++;
	}

}

void SetNeutrophils(XMFLOAT3 pos)
{
	LoadModel(MODEL_NEUTROPHILS, &g_Player[playerNum].model);
	// モデルのディフューズを保存しておく。色変え対応の為。
	GetModelDiffuse(&g_Player[playerNum].model, &g_Player[playerNum].diffuse[0]);

	g_Player[playerNum].load = TRUE;

	g_Player[playerNum].pos = pos;
	g_Player[playerNum].rot = { 0.0f, XM_PI * 0.5f, 0.0f };
	g_Player[playerNum].scl = { 0.8f, 1.0f, 1.0f };

	g_Player[playerNum].size = PLAYER_SIZE;	// 当たり判定の大きさ
	g_Player[playerNum].use = TRUE;
	g_Player[playerNum].life = PLAYER_LIFE;
	g_Player[playerNum].lifeMax = g_Player[playerNum].life;
	g_Player[playerNum].power = 5;
	g_Player[playerNum].diffend = 3;
	g_Player[playerNum].attack = FALSE;
	g_Player[playerNum].attackUse = FALSE;
	g_Player[playerNum].blockMax = 2;
	g_Player[playerNum].blockNum = 0;
	g_Player[playerNum].target = 99;
	for (int i = 0; i < MAX_TARGET; i++)
		g_Player[playerNum].targetable[i] = 99;
	g_Player[playerNum].count = 0;
	g_Player[playerNum].atCount = 0;
	g_Player[playerNum].atFrameCount = 0;
	g_Player[playerNum].atFrame = 20;
	g_Player[playerNum].startNum = partsNum;
	g_Player[playerNum].partsNum = 0;
	g_Player[playerNum].tbl_adr = neutro_Attack;	// アニメデータのテーブル先頭アドレス
	g_Player[playerNum].tbl_size = sizeof(neutro_Attack) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
	g_Player[playerNum].move_time = 0.0f;	// 実行時間

	// 階層アニメーション用の初期化処理
	g_Player[playerNum].parent = NULL;			// 本体（親）なのでNULLを入れる
	playerNum++;

	//パーツの初期化処理
	for (int k = 0; k < g_Player[playerNum].partsNum; k++)
	{
		LoadModel(MODEL_GRAPE_PARTS, &g_Parts[partsNum].model);
		// モデルのディフューズを保存しておく。色変え対応の為。
		GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);
		g_Parts[partsNum].load = TRUE;

		g_Parts[partsNum].pos = { 20.0f, 10.0f, 0.0f };		// ポリゴンの位置
		g_Parts[partsNum].rot = { 0.0f, 0.0f, 0.0f };		// ポリゴンの向き(回転)
		g_Parts[partsNum].scl = { 1.0f, 1.0f, 1.0f };		// ポリゴンの大きさ(スケール)

		// 階層アニメーション用のメンバー変数
		g_Parts[partsNum].tbl_adr = NULL;	// アニメデータのテーブル先頭アドレス
		g_Parts[partsNum].tbl_size = 0;	// 登録したテーブルのレコード総数
		g_Parts[partsNum].move_time = 0;	// 実行時間
		g_Parts[partsNum].parent = &g_Player[playerNum];	// 自分が親ならNULL、自分が子供なら親のenemyアドレス
		partsNum++;
	}

}

int GetPlayerNum(void)
{
	return playerNum;
}
//=============================================================================
// プレイヤー情報を取得
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player[0];
}

void PLAYER::StateCheck(int i)
{
	g_Player[i].state = Standby;	//とりあえず待機状態にセット
	ENEMY *enemy = GetEnemy();
	g_Player[i].count = 0;
	for (int k = 0; k < MAX_TARGET; k++)
	{
		g_Player[i].targetable[k] = 99;
	}
	for (int k = 0; k < GetEnemyNum(); k++)
	{
		//プレイヤーの攻撃範囲に1体でも敵がいるならば攻撃準備に入る
		if (CollisionBC(g_Player[i].pos, enemy[k].pos, g_Player[i].size, 10.0f))
		{
			g_Player[i].state = Deffend;
			g_Player[i].targetable[g_Player[i].count] = k;
			g_Player[i].count++;
		}
	}
}
