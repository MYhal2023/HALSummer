//=============================================================================
//
// バトル中のエネミー処理 [enemy.cpp]
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
#define TEXTURE_MAX			(1)								// テクスチャの数
#define VAR_WIDTH			(50.0f)
#define VAR_HEIGHT			(5.0f)
#define	VALUE_MOVE			(2.0f)							// 移動量
#define	VALUE_AT_MOVE		(4.0f)							// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// 回転量

#define ENEMY_SHADOW_SIZE	(1.0f)							// 影の大きさ
#define ENEMY_OFFSET_Y		(0.0f)							// プレイヤーの足元をあわせる
#define ENEMY_OFFSET_Z		(-300.0f)							// プレイヤーの足元をあわせる
#define ENEMY_LIFE			(100)								// プレイヤーのライフ

#define ENEMY_PARTS_MAX	(1)								// プレイヤーのパーツの数
#define ENEMY_AT_FLAME		(30.0f)							// プレイヤーの攻撃フレーム
#define ENEMY_INVINC_FLAME	(120.0f)						// プレイヤー無敵フレーム
#define MAX_ENEMY_PARTS (MAX_ENEMY * 3)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// 頂点情報
static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
static char* g_TextureName[] = {
	"data/TEXTURE/var.png",
};
static PLAYER_VAR	g_EnemyVar;
static ENEMY		g_Enemy[MAX_ENEMY];						// エネミー
static Enemyliner   g_Enemyline[MAX_PLAYER];
static EnemyParts	g_Parts[MAX_ENEMY_PARTS];					// エネミーのパーツ。余裕をもってエネミー×2倍の数用意

static BOOL			g_Load = FALSE;
static int			atCount;
static int			enemyNum = 0;		//何体のエネミーがいるか
static int			partsNum = 0;		//合計でいくつのパーツを使うのか
static MOVERINE		moveTbl[] = { XMFLOAT3(300.0f, 0.0f, 150.0f), XMFLOAT3(300.0f, 0.0f, 100.0f), XMFLOAT3(0.0f, 0.0f, 100.0f) };
// 階層アニメーションデータ
static INTERPOLATION_DATA grape_Attack[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(0.0f, 0.0f, 0.0f), 10, },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.5f, 0.0f),	   XMFLOAT3(1.0f, 1.0f, 1.0f), 50 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(0.0f, 0.0f, 0.0f), 60 },

};
// 階層アニメーションデータ
static INTERPOLATION_DATA grape_Move[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(0.0f, 0.0f, 0.0f), 30, },
	{ XMFLOAT3(0.0f, 20.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	   XMFLOAT3(0.0f, 0.0f, 0.0f), 30 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(0.0f, 0.0f, 0.0f), 60 },

};




//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	ID3D11Device *pDevice = GetDevice();
	MakeVertexEnemyVar();
	// テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}

	ZeroMemory(&g_EnemyVar.material, sizeof(g_EnemyVar.material));
	g_EnemyVar.material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_EnemyVar.pos = { 0.0f, 0.0f, 0.0f };
	g_EnemyVar.rot = { XM_PI * -0.5f, 0.0f, 0.0f };
	g_EnemyVar.scl = { 1.0f, 1.0f, 1.0f };
	g_EnemyVar.load = TRUE;

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//LoadModel(MODEL_GRAPE, &g_Enemy[i].model);
		//// モデルのディフューズを保存しておく。色変え対応の為。
		//GetModelDiffuse(&g_Enemy[i].model, &g_Enemy[i].diffuse[0]);

		g_Enemy[i].load = FALSE;

		g_Enemy[i].pos = { -100.0f, ENEMY_OFFSET_Y, 0.0f };
		g_Enemy[i].rot = { 0.0f, 0.0f, 0.0f };
		g_Enemy[i].scl = { 0.8f, 1.0f, 1.0f };

		g_Enemy[i].size = ENEMY_SIZE;	// 当たり判定の大きさ
		g_Enemy[i].state = Move;
		g_Enemy[i].life = ENEMY_LIFE;
		g_Enemy[i].lifeMax = g_Enemy[i].life;
		g_Enemy[i].use = FALSE;
		g_Enemy[i].attack = FALSE;
		g_Enemy[i].attackUse = FALSE;
		g_Enemy[i].start = 0;
		g_Enemy[i].tbl_adrA = NULL;	// アニメデータのテーブル先頭アドレス
		g_Enemy[i].tbl_adrM = NULL;	// アニメデータのテーブル先頭アドレス
		g_Enemy[i].tbl_sizeA = 0;	// 登録したテーブルのレコード総数
		g_Enemy[i].tbl_sizeM = 0;	// 登録したテーブルのレコード総数
		g_Enemy[i].move_time = 0;	// 実行時間
		g_Enemy[i].partsNum = 0;
		g_Enemy[i].startNum = 0;
		// 階層アニメーション用の初期化処理
		g_Enemy[i].parent = NULL;			// 本体（親）なのでNULLを入れる
	}
	for (int i = 0; i < MAX_ENEMY_PARTS; i++)
	{
		g_Parts[i].pos = { 0.0f, 0.0f, 0.0f };		// ポリゴンの位置
		g_Parts[i].rot = { 0.0f, 0.0f, 0.0f };		// ポリゴンの向き(回転)
		g_Parts[i].scl = {1.0f, 1.0f, 1.0f};		// ポリゴンの大きさ(スケール)

		g_Parts[i].load = FALSE;
		// 階層アニメーション用のメンバー変数
		g_Parts[i].tbl_adrA = NULL;	// アニメデータのテーブル先頭アドレス
		g_Parts[i].tbl_adrM = NULL;	// アニメデータのテーブル先頭アドレス
		g_Parts[i].tbl_sizeA = 0;	// 登録したテーブルのレコード総数
		g_Parts[i].tbl_sizeM = 0;	// 登録したテーブルのレコード総数
		g_Parts[i].move_time = 0;	// 実行時間
		g_Parts[i].parent = NULL;	// 自分が親ならNULL、自分が子供なら親のenemyアドレス
	}

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_Enemyline[i].pos = { 0.0f, 0.0f, 0.0f };
		g_Enemyline[i].rot = { 0.0f, 0.0f, 0.0f };
		g_Enemyline[i].scl = { 0.0f, 0.0f, 0.0f };
	}
	enemyNum = 0;
	partsNum = 0;
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
	// 頂点バッファの解放
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		// モデルの解放処理
		if (g_Enemy[i].load)
		{
			UnloadModel(&g_Enemy[i].model);
			g_Enemy[i].load = FALSE;
		}
	}
	for (int i = 0; i < MAX_ENEMY_PARTS; i++)
	{
		// モデルの解放処理
		if (g_Parts[i].load)
		{
			UnloadModel(&g_Parts[i].model);
			g_Parts[i].load = FALSE;
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

		SetEnemyTime(i);	//エネミーの出現チェック

		if (g_Enemy[i].life <= 0 && g_Enemy[i].use) {
			g_Enemy[i].use = FALSE;
			continue;
		}

		if (g_Enemy[i].use != TRUE)continue;	//死んでるか出現してない奴はスルー

		int oldState = g_Enemy[i].state;
		//エネミーのステート処理
		g_Enemy[i].state = StateCheck(i);
		if (oldState != g_Enemy[i].state)
		{
			g_Enemy[i].move_time = 0;	// 実行時間
			g_Enemy[i].atCount = 0;
			g_Enemy[i].attackUse = FALSE;
			g_Enemyline[i].pos = { 0.0f, 0.0f, 0.0f };
			g_Enemyline[i].rot = { 0.0f, 0.0f, 0.0f };
			g_Enemyline[i].scl = { 0.0f, 0.0f, 0.0f };
			for (int k = g_Enemy[i].startNum; k < g_Enemy[i].startNum + g_Enemy[i].partsNum; k++)
				g_Parts[k].move_time = 0.0f;
		}

		CheckTarget(i);
		switch (g_Enemy[i].state)
		{
		case Move:
			EnemyMove(i);
			EnemyMoveLiner(i);
			break;

		case Attack:
			EnemyInterPoration(i);
			break;
		}

	}
#ifdef _DEBUG
	PrintDebugProc("エネミー体力:%d\n", g_Enemy[0].life);
	PrintDebugProc("エネミー体力:%d\n", g_Enemy[1].life);
	PrintDebugProc("エネミー体力:%d\n", g_Enemy[2].life);
	PrintDebugProc("エネミー体力:%d\n", g_Enemy[3].life);
	PrintDebugProc("エネミー体力:%d\n", g_Enemy[0].blocked);
	PrintDebugProc("エネミー体力:%d\n", g_Enemy[1].blocked);
	PrintDebugProc("エネミー体力:%d\n", g_Enemy[2].blocked);
	PrintDebugProc("エネミー体力:%d\n", g_Enemy[3].blocked);
#endif

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
		if (g_Enemy[i].use != TRUE)continue;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Enemy[i].scl.x + g_Enemyline[i].scl.x, g_Enemy[i].scl.y + g_Enemyline[i].scl.y, g_Enemy[i].scl.z + g_Enemyline[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x + g_Enemyline[i].rot.x, g_Enemy[i].rot.y + XM_PI + g_Enemyline[i].rot.y, g_Enemy[i].rot.z + g_Enemyline[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Enemy[i].pos.x + g_Enemyline[i].pos.x, g_Enemy[i].pos.y + 20.0f + g_Enemyline[i].pos.y, g_Enemy[i].pos.z + g_Enemyline[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Enemy[i].mtxWorld, mtxWorld);

		// モデル描画
		DrawModel(&g_Enemy[i].model);

		if (g_Enemy[i].partsNum == 0)continue;

		// パーツの階層アニメーション
		for (int k = g_Enemy[i].startNum; k < g_Enemy[i].startNum + g_Enemy[i].partsNum; k++)
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
				// g_Enemy[k].mtxWorldを指している
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


//=============================================================================
// プレイヤー情報を取得
//=============================================================================
ENEMY *GetEnemy(void)
{
	return &g_Enemy[0];
}

//引数1：配列の使用を開始する添え字、引数2：何フレームの時に出すのか
void SetGrape(float time)
{
	LoadModel(MODEL_GRAPE, &g_Enemy[enemyNum].model);
	// モデルのディフューズを保存しておく。色変え対応の為。
	GetModelDiffuse(&g_Enemy[enemyNum].model, &g_Enemy[enemyNum].diffuse[0]);

	g_Enemy[enemyNum].load = TRUE;

	g_Enemy[enemyNum].pos = { moveTbl[0].start.x, ENEMY_OFFSET_Y, moveTbl[0].start.z };
	g_Enemy[enemyNum].rot = { 0.0f, 0.0f, 0.0f };
	g_Enemy[enemyNum].scl = { 0.8f, 1.0f, 1.0f };

	g_Enemy[enemyNum].size = ENEMY_SIZE;	// 当たり判定の大きさ
	g_Enemy[enemyNum].life = ENEMY_LIFE;
	g_Enemy[enemyNum].lifeMax = g_Enemy[enemyNum].life;
	g_Enemy[enemyNum].spd = 1.0f;
	g_Enemy[enemyNum].use = FALSE;
	g_Enemy[enemyNum].attack = FALSE;
	g_Enemy[enemyNum].attackUse = FALSE;
	g_Enemy[enemyNum].atInterval = 180;
	g_Enemy[enemyNum].atFrame = 20;
	g_Enemy[enemyNum].atFrameCount = 0;
	g_Enemy[enemyNum].power = 10;
	g_Enemy[enemyNum].target = NULL;
	g_Enemy[enemyNum].atCount = g_Enemy[enemyNum].atInterval;	//最初はノータイムで攻撃モーションへ
	g_Enemy[enemyNum].type = Proximity;;
	g_Enemy[enemyNum].start = time;
	g_Enemy[enemyNum].moveData = moveTbl;
	g_Enemy[enemyNum].moveCount = 0.0f;
	g_Enemy[enemyNum].moveTblSize = sizeof(moveTbl) / sizeof(MOVERINE);
	g_Enemy[enemyNum].nowTbl = 0;
	g_Enemy[enemyNum].tbl_adrA = grape_Attack;	// アニメデータのテーブル先頭アドレス
	g_Enemy[enemyNum].tbl_adrM = grape_Move;	// アニメデータのテーブル先頭アドレス
	g_Enemy[enemyNum].tbl_sizeA = sizeof(grape_Attack) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
	g_Enemy[enemyNum].tbl_sizeM = sizeof(grape_Move) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
	g_Enemy[enemyNum].move_time = 0.0f;	// 実行時間
	g_Enemy[enemyNum].partsNum = 3;
	g_Enemy[enemyNum].startNum = partsNum;
	// 階層アニメーション用の初期化処理
	g_Enemy[enemyNum].parent = NULL;			// 本体（親）なのでNULLを入れる

	LoadModel(MODEL_GRAPE_PARTS001, &g_Parts[partsNum].model);
	// モデルのディフューズを保存しておく。色変え対応の為。
	GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);
	g_Parts[partsNum].load = TRUE;

	g_Parts[partsNum].pos = { 0.0f, 0.0f, 0.0f };		// ポリゴンの位置
	g_Parts[partsNum].rot = { 0.0f, 0.0f, 0.0f };		// ポリゴンの向き(回転)
	g_Parts[partsNum].scl = { 1.0f, 1.0f, 1.0f };		// ポリゴンの大きさ(スケール)

	// 階層アニメーション用のメンバー変数
	g_Parts[partsNum].tbl_adrA = NULL;	// アニメデータのテーブル先頭アドレス
	g_Parts[partsNum].tbl_adrM = NULL;	// アニメデータのテーブル先頭アドレス
	g_Parts[partsNum].tbl_sizeA = 0;	// 登録したテーブルのレコード総数
	g_Parts[partsNum].tbl_sizeM = 0;	// 登録したテーブルのレコード総数
	g_Parts[partsNum].move_time = 0;	// 実行時間
	g_Parts[partsNum].parent = &g_Enemy[enemyNum];	// 自分が親ならNULL、自分が子供なら親のenemyアドレス
	partsNum++;


	LoadModel(MODEL_GRAPE_PARTS002, &g_Parts[partsNum].model);
	// モデルのディフューズを保存しておく。色変え対応の為。
	GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);
	g_Parts[partsNum].load = TRUE;

	g_Parts[partsNum].pos = { 0.0f, 0.0f, 0.0f };		// ポリゴンの位置
	g_Parts[partsNum].rot = { 0.0f, 0.0f, 0.0f };		// ポリゴンの向き(回転)
	g_Parts[partsNum].scl = { 1.0f, 1.0f, 1.0f };		// ポリゴンの大きさ(スケール)
	// 階層アニメーション用のメンバー変数
	g_Parts[partsNum].tbl_adrA = NULL;	// アニメデータのテーブル先頭アドレス
	g_Parts[partsNum].tbl_adrM = NULL;	// アニメデータのテーブル先頭アドレス
	g_Parts[partsNum].tbl_sizeA = 0;	// 登録したテーブルのレコード総数
	g_Parts[partsNum].tbl_sizeM = 0;	// 登録したテーブルのレコード総数
	g_Parts[partsNum].move_time = 0;	// 実行時間
	g_Parts[partsNum].parent = &g_Enemy[enemyNum];	// 自分が親ならNULL、自分が子供なら親のenemyアドレス
	partsNum++;


	LoadModel(MODEL_GRAPE_PARTS003, &g_Parts[partsNum].model);
	// モデルのディフューズを保存しておく。色変え対応の為。
	GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);
	g_Parts[partsNum].load = TRUE;

	g_Parts[partsNum].pos = { 0.0f, 0.0f, 0.0f };		// ポリゴンの位置
	g_Parts[partsNum].rot = { 0.0f, 0.0f, 0.0f };		// ポリゴンの向き(回転)
	g_Parts[partsNum].scl = { 1.0f, 1.0f, 1.0f };		// ポリゴンの大きさ(スケール)

		// 階層アニメーション用のメンバー変数
	g_Parts[partsNum].tbl_adrA = NULL;	// アニメデータのテーブル先頭アドレス
	g_Parts[partsNum].tbl_adrM = NULL;	// アニメデータのテーブル先頭アドレス
	g_Parts[partsNum].tbl_sizeA = 0;	// 登録したテーブルのレコード総数
	g_Parts[partsNum].tbl_sizeM = 0;	// 登録したテーブルのレコード総数
	g_Parts[partsNum].move_time = 0;	// 実行時間
	g_Parts[partsNum].parent = &g_Enemy[enemyNum];	// 自分が親ならNULL、自分が子供なら親のenemyアドレス
	partsNum++;

	enemyNum++;
}

//添え字を引数に持ってくる
void SetEnemyTime(int i)
{
	for (int k = 0; k < enemyNum; k++)
	{
		if (g_Enemy[i].start <= 0.0f)continue;	//既に出現済みのエネミーはここで弾く

		g_Enemy[i].start -= 1.0f;
		if (g_Enemy[i].start <= 0.0f)//エネミーを出現させる
			g_Enemy[i].use = TRUE;
	}
}

void EnemyMoveLiner(int i)
{
	if (g_Enemy[i].tbl_adrA == NULL)return;	// 線形補間を実行する？

//
// 線形補間の処理
// 移動処理
	int		index = (int)g_Enemy[i].move_time;
	float	time = g_Enemy[i].move_time - index;
	int		size = g_Enemy[i].tbl_sizeM;

	float dt = 1.0f / g_Enemy[i].tbl_adrM[index].frame;	// 1フレームで進める時間
	g_Enemy[i].move_time += dt;							// アニメーションの合計時間に足す

	if (index > (size - 2))	// ゴールをオーバーしていたら、データを最初に戻して攻撃を終了
	{
		g_Enemy[i].move_time = 0.0f;
		index = 0;
		g_Enemyline[i].pos = { 0.0f, 0.0f, 0.0f };
		g_Enemyline[i].rot = { 0.0f, 0.0f, 0.0f };
		g_Enemyline[i].scl = { 0.0f, 0.0f, 0.0f };
	}

	// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
	XMVECTOR p1 = XMLoadFloat3(&g_Enemy[i].tbl_adrM[index + 1].pos);	// 次の場所
	XMVECTOR p0 = XMLoadFloat3(&g_Enemy[i].tbl_adrM[index + 0].pos);	// 現在の場所
	XMVECTOR vec = p1 - p0;
	XMStoreFloat3(&g_Enemyline[i].pos, p0 + vec * time);

	// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
	XMVECTOR r1 = XMLoadFloat3(&g_Enemy[i].tbl_adrM[index + 1].rot);	// 次の角度
	XMVECTOR r0 = XMLoadFloat3(&g_Enemy[i].tbl_adrM[index + 0].rot);	// 現在の角度
	XMVECTOR rot = r1 - r0;
	XMStoreFloat3(&g_Enemyline[i].rot, r0 + rot * time);

	// scaleを求める S = StartX + (EndX - StartX) * 今の時間
	XMVECTOR s1 = XMLoadFloat3(&g_Enemy[i].tbl_adrM[index + 1].scl);	// 次のScale
	XMVECTOR s0 = XMLoadFloat3(&g_Enemy[i].tbl_adrM[index + 0].scl);	// 現在のScale
	XMVECTOR scl = s1 - s0;
	XMStoreFloat3(&g_Enemyline[i].scl, s0 + scl * time);

	for (int k = g_Enemy[i].startNum; k < g_Enemy[i].startNum + g_Enemy[i].partsNum; k++)
	{
		if (g_Parts[k].tbl_adrM == NULL)continue;;	// 線形補間を実行する？

		int		index = (int)g_Parts[k].move_time;
		float	time = g_Parts[k].move_time - index;
		int		size = g_Parts[k].tbl_sizeM;

		float dt = 1.0f / g_Parts[k].tbl_adrM[index].frame;	// 1フレームで進める時間
		g_Parts[k].move_time += dt;							// アニメーションの合計時間に足す

		if (index > (size - 2))	// ゴールをオーバーしていたら、データを最初に戻して攻撃を終了
		{
			g_Parts[k].move_time = 0.0f;
			index = 0;
		}

		// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
		XMVECTOR p1 = XMLoadFloat3(&g_Parts[k].tbl_adrM[index + 1].pos);	// 次の場所
		XMVECTOR p0 = XMLoadFloat3(&g_Parts[k].tbl_adrM[index + 0].pos);	// 現在の場所
		XMVECTOR vec = p1 - p0;
		XMStoreFloat3(&g_Parts[k].pos, XMLoadFloat3(&g_Parts[k].pos) + p0 + vec * time);

		// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
		XMVECTOR r1 = XMLoadFloat3(&g_Parts[k].tbl_adrM[index + 1].rot);	// 次の角度
		XMVECTOR r0 = XMLoadFloat3(&g_Parts[k].tbl_adrM[index + 0].rot);	// 現在の角度
		XMVECTOR rot = r1 - r0;
		XMStoreFloat3(&g_Parts[k].rot, r0 + rot * time);

		// scaleを求める S = StartX + (EndX - StartX) * 今の時間
		XMVECTOR s1 = XMLoadFloat3(&g_Parts[k].tbl_adrM[index + 1].scl);	// 次のScale
		XMVECTOR s0 = XMLoadFloat3(&g_Parts[k].tbl_adrM[index + 0].scl);	// 現在のScale
		XMVECTOR scl = s1 - s0;
		XMStoreFloat3(&g_Parts[k].scl, s0 + scl * time);
	}
}

//主にエネミーの攻撃時の線形補間。ダメージ処理もここで行う
void EnemyInterPoration(int i)
{
	if (g_Enemy[i].tbl_adrA == NULL)return;	// 線形補間を実行する？

	//
	// 線形補間の処理
	// 移動処理
	int		index = (int)g_Enemy[i].move_time;
	float	time = g_Enemy[i].move_time - index;
	int		size = g_Enemy[i].tbl_sizeA;

	float dt = 1.0f / g_Enemy[i].tbl_adrA[index].frame;	// 1フレームで進める時間
	g_Enemy[i].move_time += dt;							// アニメーションの合計時間に足す

	if (index > (size - 2))	// ゴールをオーバーしていたら、データを最初に戻して攻撃を終了
	{
		g_Enemy[i].move_time = 0.0f;
		index = 0;
		g_Enemy[i].atCount = 0;
		g_Enemy[i].state = Move;
		g_Enemy[i].attackUse = FALSE;
		g_Enemyline[i].pos = { 0.0f, 0.0f, 0.0f };
		g_Enemyline[i].rot = { 0.0f, 0.0f, 0.0f };
		g_Enemyline[i].scl = { 0.0f, 0.0f, 0.0f };
	}

	// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
	XMVECTOR p1 = XMLoadFloat3(&g_Enemy[i].tbl_adrA[index + 1].pos);	// 次の場所
	XMVECTOR p0 = XMLoadFloat3(&g_Enemy[i].tbl_adrA[index + 0].pos);	// 現在の場所
	XMVECTOR vec = p1 - p0;
	XMStoreFloat3(&g_Enemyline[i].pos, p0 + vec * time);

	// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
	XMVECTOR r1 = XMLoadFloat3(&g_Enemy[i].tbl_adrA[index + 1].rot);	// 次の角度
	XMVECTOR r0 = XMLoadFloat3(&g_Enemy[i].tbl_adrA[index + 0].rot);	// 現在の角度
	XMVECTOR rot = r1 - r0;
	XMStoreFloat3(&g_Enemyline[i].rot, r0 + rot * time);

	// scaleを求める S = StartX + (EndX - StartX) * 今の時間
	XMVECTOR s1 = XMLoadFloat3(&g_Enemy[i].tbl_adrA[index + 1].scl);	// 次のScale
	XMVECTOR s0 = XMLoadFloat3(&g_Enemy[i].tbl_adrA[index + 0].scl);	// 現在のScale
	XMVECTOR scl = s1 - s0;
	XMStoreFloat3(&g_Enemyline[i].scl, s0 + scl * time);

	for (int k = g_Enemy[i].startNum; k < g_Enemy[i].startNum + g_Enemy[i].partsNum; k++)
	{
		if (g_Parts[k].tbl_adrA == NULL)continue;;	// 線形補間を実行する？

		int		index = (int)g_Parts[k].move_time;
		float	time = g_Parts[k].move_time - index;
		int		size = g_Parts[k].tbl_sizeA;

		float dt = 1.0f / g_Parts[k].tbl_adrA[index].frame;	// 1フレームで進める時間
		g_Parts[k].move_time += dt;							// アニメーションの合計時間に足す

		if (index > (size - 2))	// ゴールをオーバーしていたら、データを最初に戻して攻撃を終了
		{
			g_Parts[k].move_time = 0.0f;
			index = 0;
		}

		// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
		XMVECTOR p1 = XMLoadFloat3(&g_Parts[k].tbl_adrA[index + 1].pos);	// 次の場所
		XMVECTOR p0 = XMLoadFloat3(&g_Parts[k].tbl_adrA[index + 0].pos);	// 現在の場所
		XMVECTOR vec = p1 - p0;
		XMStoreFloat3(&g_Parts[k].pos, XMLoadFloat3(&g_Parts[k].pos) + p0 + vec * time);

		// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
		XMVECTOR r1 = XMLoadFloat3(&g_Parts[k].tbl_adrA[index + 1].rot);	// 次の角度
		XMVECTOR r0 = XMLoadFloat3(&g_Parts[k].tbl_adrA[index + 0].rot);	// 現在の角度
		XMVECTOR rot = r1 - r0;
		XMStoreFloat3(&g_Parts[k].rot, r0 + rot * time);

		// scaleを求める S = StartX + (EndX - StartX) * 今の時間
		XMVECTOR s1 = XMLoadFloat3(&g_Parts[k].tbl_adrA[index + 1].scl);	// 次のScale
		XMVECTOR s0 = XMLoadFloat3(&g_Parts[k].tbl_adrA[index + 0].scl);	// 現在のScale
		XMVECTOR scl = s1 - s0;
		XMStoreFloat3(&g_Parts[k].scl, s0 + scl * time);
	}


	//ここから攻撃処理
	if (g_Enemy[i].target == NULL || g_Enemy[i].attackUse == TRUE)return;	//セットしていない、セットする必要がない攻撃があるかも？
	g_Enemy[i].atFrameCount++;
	//攻撃フレームに達したら、ダメージ計算関数を元にターゲットにダメージ
	if (g_Enemy[i].atFrameCount >= g_Enemy[i].atFrame) {
		g_Enemy[i].target->life -= DamageFunc(g_Enemy[i].power, g_Enemy[i].target->diffend);
		g_Enemy[i].atFrameCount = 0;
		g_Enemy[i].attackUse = TRUE;
	}
}

void EnemyMove(int i)
{
	//近接キャラでかつブロック中なら移動しない
	if (g_Enemy[i].type == Proximity && g_Enemy[i].blocked)
		return;

	int tbl = g_Enemy[i].nowTbl;
	//エネミーの移動速度を元に座標更新
	XMVECTOR v1 = XMLoadFloat3(&moveTbl[tbl + 1].start) - XMLoadFloat3(&moveTbl[tbl].start);
	XMVECTOR nor = XMVector3Normalize(v1);
	XMStoreFloat3(&g_Enemy[i].pos, XMLoadFloat3(&g_Enemy[i].pos) + nor * g_Enemy[i].spd);

	//データの移動量と現在の移動量をfloat値に変換して比較
	XMFLOAT3 countData;
	XMStoreFloat3(&countData, v1);
	XMFLOAT3 countMove;
	XMStoreFloat3(&countMove, nor * g_Enemy[i].spd);
	float moveMax = fabsf(countData.x) + fabsf(countData.y) + fabsf(countData.z);
	g_Enemy[i].moveCount += fabsf(countMove.x) + fabsf(countMove.y) + fabsf(countMove.z);
	//移動量を超えていたら次のデータテーブルへ
	if (g_Enemy[i].moveCount >= moveMax)
	{
		g_Enemy[i].nowTbl++;
		g_Enemy[i].moveCount = 0.0f;
	}

	if (g_Enemy[i].nowTbl >= g_Enemy[i].moveTblSize - 1)
		g_Enemy[i].use = FALSE;
}


//現状、このステート遷移方法は遠距離キャラ。近距離キャラは円めっちゃ小さい感じ
int StateCheck(int i)
{
	//攻撃中ならステートはここでは変更しない
	if (g_Enemy[i].state == Attack)return Attack;

	int ans = Move;			//デフォルトは移動
	BOOL check = FALSE;
	PLAYER *player = GetPlayer();
	for (int k = 0; k < MAX_PLAYER; k++)
	{
		//1人でも生存しているユニットを見つけたら攻撃モードへ
		if (player[k].use != TRUE)continue;
		if (CollisionBC(g_Enemy[i].pos, player[k].pos, 100.0f, 1.0f)) { 
			check = TRUE;
			break;
		}
	}
	if (check == FALSE)return ans;

	g_Enemy[i].atCount++;
	if (g_Enemy[i].atCount >= g_Enemy[i].atInterval)
	{
		ans = Attack;
		g_Enemy[i].atCount = 0;
	}
	return ans;
}

//敵キャラの攻撃ターゲット決定。一番近い敵を見つけて攻撃する
void CheckTarget(int i)
{
	//攻撃中ではない、又はブロックされているならここでターゲット決定はしない
	if (g_Enemy[i].state != Attack || g_Enemy[i].blocked != FALSE)return;

	PLAYER *player = GetPlayer();
	float cmp = 0.0f;;
	for (int k = 0; k < MAX_PLAYER; k++)
	{
		if (player[k].use != TRUE)continue;
		XMVECTOR v1 = XMLoadFloat3(&g_Enemy[i].pos) - XMLoadFloat3(&player[k].pos);
		XMFLOAT3 countData;
		XMStoreFloat3(&countData, v1);
		float dist = fabsf(countData.x) + fabsf(countData.y) + fabsf(countData.z);
		if (dist > cmp)
		{
			cmp = dist;
			g_Enemy[i].target = &player[k];
		}
	}
}

int GetEnemyNum(void)
{
	return enemyNum;
}

//プレイヤーキャラの体力バーの表示処理
void DrawEnemyLife(void)
{
	// Z比較なし
	SetDepthEnable(FALSE);
	// ライティングを無効
	SetLightEnable(FALSE);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;
	//hpバーを設置
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use != TRUE)continue;	//使われてないプレイヤーはスルー

		for (int k = 0; k < 2; k++)//最初に最大値体力を、次に現体力を表示
		{
			g_EnemyVar.pos = { g_Enemy[i].pos.x, g_Enemy[i].pos.y + 15.0f, g_Enemy[i].pos.z - 15.0f };
			if (k == 0)
			{
				g_EnemyVar.scl = { 1.0f, 1.0f, 1.0f };
				g_EnemyVar.material.Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.7f);
			}
			else
			{
				float par = (float)(g_Enemy[i].life) / (float)(g_Enemy[i].lifeMax);
				g_EnemyVar.scl = { par, 1.0f, 1.0f };
				g_EnemyVar.pos.x -= (VAR_WIDTH * (1.0f - par)) * 0.5f;
				g_EnemyVar.material.Diffuse = XMFLOAT4(0.8f, 0.0f, 0.0f, 1.0f);
			}
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_EnemyVar.scl.x, g_EnemyVar.scl.y, g_EnemyVar.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_EnemyVar.rot.x, g_EnemyVar.rot.y, g_EnemyVar.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_EnemyVar.pos.x, g_EnemyVar.pos.y, g_EnemyVar.pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			// マテリアルの設定
			SetMaterial(g_EnemyVar.material);

			// ポリゴンの描画
			GetDeviceContext()->Draw(4, 0);
		}
	}
	// Z比較あり
	SetDepthEnable(TRUE);
	// ライティングを有効に
	SetLightEnable(TRUE);

}

HRESULT MakeVertexEnemyVar(void)
{
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	{//頂点バッファの中身を埋める
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		// 頂点座標の設定
		vertex[0].Position = XMFLOAT3(-VAR_WIDTH / 2, 0.0f, VAR_HEIGHT / 2);
		vertex[1].Position = XMFLOAT3(VAR_WIDTH / 2, 0.0f, VAR_HEIGHT / 2);
		vertex[2].Position = XMFLOAT3(-VAR_WIDTH / 2, 0.0f, -VAR_HEIGHT / 2);
		vertex[3].Position = XMFLOAT3(VAR_WIDTH / 2, 0.0f, -VAR_HEIGHT / 2);

		// 法線の設定
		vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

		// 拡散光の設定
		vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
		vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
		vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
		vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

		GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}

	return S_OK;
}
