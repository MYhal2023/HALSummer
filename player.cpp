//=============================================================================
//
// バトル中のプレイヤー処理 [player.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "math.h"
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
#include "fieldchip.h"
#include "skill.h"
#include "sound.h"
#include "bullet.h"
#include "playerSet.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX			(1)								// テクスチャの数
#define VAR_WIDTH			(50.0f)
#define VAR_HEIGHT			(5.0f)
#define	VALUE_MOVE			(2.0f)							// 移動量
#define	VALUE_AT_MOVE		(4.0f)							// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// 回転量

#define PLAYER_SHADOW_SIZE	(1.0f)							// 影の大きさ
#define PLAYER_OFFSET_Y		(5.0f)							// プレイヤーの足元をあわせる
#define PLAYER_OFFSET_Z		(-300.0f)							// プレイヤーの足元をあわせる
#define PLAYER_LIFE			(100)								// プレイヤーのライフ

#define PLAYER_PARTS_MAX	(1)								// プレイヤーのパーツの数
#define PLAYER_AT_FLAME		(30.0f)							// プレイヤーの攻撃フレーム
#define PLAYER_SP_FLAME		(30.0f)							// プレイヤーのSPが増える間隔
#define PLAYER_INVINC_FLAME	(120.0f)						// プレイヤー無敵フレーム
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
static PLAYER_VAR	g_PlayerVar;
static PLAYER		g_Player[MAX_PLAYER];						// プレイヤー
static Playerliner  g_Playerline[MAX_PLAYER];					//プレイヤーの線形補間データ
static PlayerParts	g_Parts[MAX_PLAYER_PARTS];					// プレイヤー
static BOOL			g_Load = FALSE;
static int			playerNum = 0;
static int			partsNum = 0;
static int			atNum[MAX_PLAYER];
static char name[2][64];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	ID3D11Device *pDevice = GetDevice();
	MakeVertexPlayerVar();
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

	ZeroMemory(&g_PlayerVar.material, sizeof(g_PlayerVar.material));
	g_PlayerVar.material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_PlayerVar.pos = { 0.0f, 0.0f, 0.0f };
	g_PlayerVar.rot = { XM_PI * -0.5f, 0.0f, 0.0f };
	g_PlayerVar.scl = { 1.0f, 1.0f, 1.0f };
	g_PlayerVar.pardiff = 0.0f;
	g_PlayerVar.parpolar = 0.02f;
	g_PlayerVar.load = TRUE;

	strcpy(name[0], MODEL_PLAYER);
	strcpy(name[1], MODEL_NEUTROPHILS);

	for (int i = 0; i < MAX_PLAYER; i++)
	{
		g_Player[i].load = FALSE;
		g_Player[i].pos = { 0.0f, PLAYER_OFFSET_Y, 0.0f };
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
		g_Player[i].partsNum = 0;
		g_Player[i].startNum = 0;
		g_Player[i].keyNum = 99;
		// 階層アニメーション用の初期化処理
		g_Player[i].parent = NULL;			// 本体（親）なのでNULLを入れる
		g_Playerline[i].pos = { 0.0f, 0.0f, 0.0f };
		g_Playerline[i].rot = { 0.0f, 0.0f, 0.0f };
		g_Playerline[i].scl = { 0.0f, 0.0f, 0.0f };
	}
	
	for (int i = 0; i < MAX_PLAYER_PARTS; i++)
	{
		g_Parts[i].load = FALSE;
		g_Parts[i].use = FALSE;
	}
	
	g_Load = TRUE;
	playerNum = 0;
	partsNum = 0;
	for (int i = 0; i < MAX_PLAYER; i++)
		atNum[i] = 0;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	if (g_Load == FALSE) return;

	name[0][0] = '0';
	name[1][0] = '0';
	// 頂点バッファの解放
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}


	for (int i = 0; i < MAX_PLAYER; i++)
	{
		// モデルの解放処理
		if (g_Player[i].load)
		{
			UnloadModel(&g_Player[i].model);
			g_Player[i].load = FALSE;
		}
	}
	for (int i = 0; i < MAX_PLAYER_PARTS; i++)
	{
		// モデルの解放処理
		if (g_Player[i].load)
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
void UpdatePlayer(void)
{
	BlockEnemy();	//ブロック情報を更新

	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (g_Player[i].use != TRUE)continue;
		//全キャラ共通処理
		//体力が無くなったキャラの処理。消去する。そこのマップチップを開ける
		if (g_Player[i].life <= 0 && g_Player[i].use) {
			PlayerSet  *ps = GetSetPos();
			g_Player[i].use = FALSE;
			ps->use[g_Player[i].keyNum] = TRUE;
			g_Player[i].keyNum = 99;
			for (int k = g_Player[i].startNum; k < g_Player[i].startNum + g_Player[i].partsNum; k++)
			{
				g_Parts[k].use = FALSE;
			}
				int x = (int)(g_Player[i].pos.x / CHIP_SIZE);
			int z = (int)(g_Player[i].pos.z / CHIP_SIZE);
			SetMapChipUse(FALSE, z, x);
			continue;
		}
		IncreaseSP(i);
		//キャラ別処理
		switch (g_Player[i].skillID)	//攻撃方法等はスキルIDで判別。ここら辺はコードぐちゃぐちゃなので注意
		{
		case neutro_skill:
		case macro_skill:
		case killerT_skill:
		case NK_skill:
		case kouen_skill:
			AttackChar(i);
			break;
		case helperT_skill:
			HealChar(i);
			break;
		}

	}
#ifdef _DEBUG
	PrintDebugProc("プレイヤー:%d\n", g_Player[0].atFrameCount);

#endif
}

void AttackChar(int i)
{
	int oldState = g_Player[i].state;
	g_Player[i].StateCheck(i);
	//①ステート遷移
	if (oldState != g_Player[i].state)
	{
		g_Player[i].move_time = 0.0f;
		g_Player[i].atCount = 0;
		g_Player[i].attackUse = FALSE;
		atNum[i] = 0;
		g_Playerline[i].pos = { 0.0f, 0.0f, 0.0f };
		g_Playerline[i].rot = { 0.0f, 0.0f, 0.0f };
		g_Playerline[i].scl = { 0.0f, 0.0f, 0.0f };
		for (int k = g_Player[i].startNum; k < g_Player[i].startNum + g_Player[i].partsNum; k++)
			g_Parts[k].move_time = 0.0f;
	}
	CheckEnemyTarget(i);
	//スキル使用中ならステートを上書き
	int atState;
	if (g_Player[i].skillUse) {
		atState = g_Player[i].state;
		g_Player[i].state = Skill;
	}
	switch (g_Player[i].state)
	{
	case Standby:
		PlayerStandLiner(i);
		break;

	case Deffend:
		PlayerInterPoration(i);
		break;
	case Skill:
		PlayerSkill(i);
		break;
	}
	if (g_Player[i].skillUse) {
		g_Player[i].state = atState;
	}

}

void HealChar(int i)
{
	int oldState = g_Player[i].state;
	if (g_Player[i].state != Deffend) {
		g_Player[i].state = Standby;	//とりあえず待機状態にセット
		g_Player[i].count = 0;
		for (int k = 0; k < MAX_TARGET; k++)
		{
			g_Player[i].targetable[k] = 99;
		}

		for (int k = 0; k < MAX_PLAYER; k++)
		{
			if (g_Player[k].use != TRUE || g_Player[k].life >= g_Player[k].lifeMax)continue;
			//プレイヤーの攻撃範囲に1体でも敵がいるならば回復準備に入る。ターゲット情報も保存
			if (CollisionBC(g_Player[i].pos, g_Player[k].pos, g_Player[i].size * 1.0f, 1.0f))
			{
				g_Player[i].state = Deffend;
				g_Player[i].targetable[g_Player[i].count] = k;
				g_Player[i].count++;
			}
		}
	}
	//①ステート遷移
	if (oldState != g_Player[i].state)
	{
		g_Player[i].move_time = 0.0f;
		g_Player[i].atCount = 0;
		g_Playerline[i].pos = { 0.0f, 0.0f, 0.0f };
		g_Playerline[i].rot = { 0.0f, 0.0f, 0.0f };
		g_Playerline[i].scl = { 0.0f, 0.0f, 0.0f };
		for (int k = g_Player[i].startNum; k < g_Player[i].startNum + g_Player[i].partsNum; k++)
			g_Parts[k].move_time = 0.0f;
	}
	CheckHealTarget(i);
	//スキル使用中ならステートを上書き
	int atState;
	if (g_Player[i].skillUse) {
		atState = g_Player[i].state;
		g_Player[i].state = Skill;
	}
	switch (g_Player[i].state)
	{
	case Standby:
		PlayerStandLiner(i);
		break;

	case Deffend:
		PlayerInterPoration(i);
		break;
	case Skill:
		PlayerSkill(i);
		break;
	}
	if (g_Player[i].skillUse) {
		g_Player[i].state = atState;
	}
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
		mtxScl = XMMatrixScaling(g_Player[i].scl.x + g_Playerline[i].scl.x, g_Player[i].scl.y + g_Playerline[i].scl.y, g_Player[i].scl.z + g_Playerline[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Player[i].rot.x + g_Playerline[i].rot.x, g_Player[i].rot.y + XM_PI + g_Playerline[i].rot.y, g_Player[i].rot.z + g_Playerline[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Player[i].pos.x + g_Playerline[i].pos.x, g_Player[i].pos.y + g_Playerline[i].pos.y, g_Player[i].pos.z + g_Playerline[i].pos.z);
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

//待機ﾓｰｼｮﾝ処理
void PlayerStandLiner(int i)
{
	if (g_Player[i].tbl_adrM == NULL)return;	// 線形補間を実行する？

//
// 線形補間の処理
// 移動処理
	int		index = (int)g_Player[i].move_time;
	float	time = g_Player[i].move_time - index;
	int		size = g_Player[i].tbl_sizeM;

	float dt = 1.0f / g_Player[i].tbl_adrM[index].frame;	// 1フレームで進める時間
	g_Player[i].move_time += dt;							// アニメーションの合計時間に足す

	if (index > (size - 2))	// ゴールをオーバーしていたら、データを最初に戻して攻撃を終了
	{
		g_Player[i].move_time = 0.0f;
		index = 0;
		g_Playerline[i].pos = { 0.0f, 0.0f, 0.0f };
		g_Playerline[i].rot = { 0.0f, 0.0f, 0.0f };
		g_Playerline[i].scl = { 0.0f, 0.0f, 0.0f };
	}

	// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
	XMVECTOR p1 = XMLoadFloat3(&g_Player[i].tbl_adrM[index + 1].pos);	// 次の場所
	XMVECTOR p0 = XMLoadFloat3(&g_Player[i].tbl_adrM[index + 0].pos);	// 現在の場所
	XMVECTOR vec = p1 - p0;
	XMStoreFloat3(&g_Playerline[i].pos, p0 + vec * time);

	// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
	XMVECTOR r1 = XMLoadFloat3(&g_Player[i].tbl_adrM[index + 1].rot);	// 次の角度
	XMVECTOR r0 = XMLoadFloat3(&g_Player[i].tbl_adrM[index + 0].rot);	// 現在の角度
	XMVECTOR rot = r1 - r0;
	XMStoreFloat3(&g_Playerline[i].rot, r0 + rot * time);

	// scaleを求める S = StartX + (EndX - StartX) * 今の時間
	XMVECTOR s1 = XMLoadFloat3(&g_Player[i].tbl_adrM[index + 1].scl);	// 次のScale
	XMVECTOR s0 = XMLoadFloat3(&g_Player[i].tbl_adrM[index + 0].scl);	// 現在のScale
	XMVECTOR scl = s1 - s0;
	XMStoreFloat3(&g_Playerline[i].scl, s0 + scl * time);

	for (int k = g_Player[i].startNum; k < g_Player[i].startNum + g_Player[i].partsNum; k++)
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
		XMStoreFloat3(&g_Parts[k].pos, p0 + vec * time);

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

//プレイヤーの線形補間。攻撃モーション中にここに来たらダメージ処理も行う
void PlayerInterPoration(int i)
{
	if (g_Player[i].tbl_adrA == NULL)return;	// 線形補間を実行する？

	//
	// 線形補間の処理
	// 移動処理
	int		index = (int)g_Player[i].move_time;
	float	time = g_Player[i].move_time - index;
	int		size = g_Player[i].tbl_sizeA;

	float dt = 1.0f / g_Player[i].tbl_adrA[index].frame;	// 1フレームで進める時間
	g_Player[i].move_time += dt;							// アニメーションの合計時間に足す

	if (index > (size - 2))	// ゴールをオーバーしていたら、データを最初に戻して攻撃を終了
	{
		g_Player[i].move_time = 0.0f;
		index = 0;
		g_Player[i].atCount = 0;
		g_Player[i].state = Standby;
		g_Player[i].attackUse = FALSE;
		atNum[i] = 0;
		g_Playerline[i].pos = { 0.0f, 0.0f, 0.0f };
		g_Playerline[i].rot = { 0.0f, 0.0f, 0.0f };
		g_Playerline[i].scl = { 0.0f, 0.0f, 0.0f };
	}
	// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
	XMVECTOR p1 = XMLoadFloat3(&g_Player[i].tbl_adrA[index + 1].pos);	// 次の場所
	XMVECTOR p0 = XMLoadFloat3(&g_Player[i].tbl_adrA[index + 0].pos);	// 現在の場所
	XMVECTOR vec = p1 - p0;
	XMStoreFloat3(&g_Playerline[i].pos, p0 + vec * time);

	// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
	XMVECTOR r1 = XMLoadFloat3(&g_Player[i].tbl_adrA[index + 1].rot);	// 次の角度
	XMVECTOR r0 = XMLoadFloat3(&g_Player[i].tbl_adrA[index + 0].rot);	// 現在の角度
	XMVECTOR rot = r1 - r0;
	XMStoreFloat3(&g_Playerline[i].rot, r0 + rot * time);

	// scaleを求める S = StartX + (EndX - StartX) * 今の時間
	XMVECTOR s1 = XMLoadFloat3(&g_Player[i].tbl_adrA[index + 1].scl);	// 次のScale
	XMVECTOR s0 = XMLoadFloat3(&g_Player[i].tbl_adrA[index + 0].scl);	// 現在のScale
	XMVECTOR scl = s1 - s0;
	XMStoreFloat3(&g_Playerline[i].scl, s0 + scl * time);

	for (int k = g_Player[i].startNum; k < g_Player[i].startNum + g_Player[i].partsNum; k++)
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
		XMStoreFloat3(&g_Parts[k].pos, p0 + vec * time);

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

	//ここから攻撃処理もしくは回復処理
	if (g_Player[i].skillID == kouen_skill && g_Player[i].attackUse && 
		atNum[i] < 5) {
		g_Player[i].atFrameCount++;
		if (g_Player[i].atFrameCount >= g_Player[i].atFrame)
			g_Player[i].attackUse = FALSE;
	}
	if (g_Player[i].target == 99 || g_Player[i].attackUse == TRUE)return;	//セットしていない、セットする必要がない攻撃があるかも？
	g_Player[i].atFrameCount++;
	//ヘルパーTなら回復、そうでないなら攻撃
	if (g_Player[i].skillID != helperT_skill) {
		ENEMY *enemy = GetEnemy();
		//演出用のバレットセット
		if (g_Player[i].skillID == NK_skill &&
			g_Player[i].atFrameCount == g_Player[i].atFrame - 5) {
			ENEMY *enemy = GetEnemy();
			XMFLOAT3 setpos = { g_Player[i].pos.x, g_Player[i].pos.y + 15.0f, g_Player[i].pos.z};
			SetBullet(setpos, g_Player[i].rot, 1.0f, &enemy[g_Player[i].target]);
		}
		else if (g_Player[i].skillID == kouen_skill &&
			g_Player[i].atFrameCount >= g_Player[i].atFrame - 1 &&
			atNum[i] < 5) {
			ENEMY *enemy = GetEnemy();
			XMFLOAT3 setpos = { g_Player[i].pos.x + cosf(g_Player[i].rot.y) * 20.0f, g_Player[i].pos.y, g_Player[i].pos.z + sinf(g_Player[i].rot.y) *20.0f };
			XMFLOAT3 invpos = { g_Player[i].pos.x - cosf(g_Player[i].rot.y) * 20.0f, g_Player[i].pos.y, g_Player[i].pos.z - sinf(g_Player[i].rot.y) *20.0f };
			SetBullet(setpos, g_Player[i].rot, 0.5f, &enemy[g_Player[i].target]);
			SetBullet(invpos, g_Player[i].rot, 0.5f, &enemy[g_Player[i].target]);
		}

		//攻撃フレームに達したら、ダメージ計算関数を元にターゲットにダメージ
		if (g_Player[i].atFrameCount >= g_Player[i].atFrame) {
			enemy[g_Player[i].target].life -= DamageFunc(g_Player[i].power, enemy[g_Player[i].target].diffend);
			g_Player[i].atFrameCount = 0;
			g_Player[i].attackUse = TRUE;
			if (atNum[i] == 0)
				PlaySound(g_Player[i].attackSE);
			atNum[i]++;
		}

	}
	else {
		if (g_Player[i].atFrameCount >= g_Player[i].atFrame) {
			g_Player[g_Player[i].target].life += DamageFunc(g_Player[i].power, 0);
			//回復後は最大値に合わせる
			if (g_Player[g_Player[i].target].life > g_Player[g_Player[i].target].lifeMax)
				g_Player[g_Player[i].target].life = g_Player[g_Player[i].target].lifeMax;
			g_Player[i].atFrameCount = 0;
			g_Player[i].attackUse = TRUE;
			PlaySound(g_Player[i].attackSE);
		}
	}
}

//スキル処理(ここではスキルタイプの判別を行い、それに基づいた関数を呼び出す)
void PlayerSkill(int i)
{
	//ポリモーフィズム使えよ！って言いたいけどめんどくさい…
	//スキル関数先でスキル発動を切ること(skillUse,state)
	switch (g_Player[i].skillID)
	{
	case neutro_skill:
		NeutroSkill(&g_Player[i]);
		break;
	case macro_skill:
		MacroSkill(&g_Player[i], &g_Playerline[i], &g_Parts[0]);
		break;
	case helperT_skill:
		helperTSkill(&g_Player[i], &g_Playerline[i], &g_Parts[0]);
		break;
	case killerT_skill:
		KillerSkill(&g_Player[i], &g_Playerline[i], &g_Parts[0]);
		break;
	case NK_skill:
		NKSkill(&g_Player[i], &g_Playerline[i], &g_Parts[0]);
		break;
	case kouen_skill:
		KouenSkill(&g_Player[i], &g_Playerline[i], &g_Parts[0]);
		break;
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
				enemy[k].use != TRUE ||
				enemy[k].blocked == TRUE)continue;
			if (CollisionBC(g_Player[i].pos, enemy[k].pos, 30.0f, 1.0f)) {
				//ここでエネミーを被ブロック状態へ変更する。攻撃先も自分へ
				g_Player[i].blockNum++;
				enemy[k].blocked = TRUE;
				enemy[k].target = &g_Player[i];
			}

		}
	}
}

//遠距離キャラのターゲット決定関数？一応近接キャラもいける
//敵キャラの攻撃ターゲット決定。拠点から一番近い敵を見つけて攻撃する
void CheckEnemyTarget(int i)
{
	//攻撃中ではないならここでターゲット決定はしない
	if (g_Player[i].state == Standby) {
		g_Player[i].target = 99;
		return;
	}
	ENEMY *enemy = GetEnemy();
	Base *base = GetBase();
	float cmp = 0.0f;
	for (int k = 0; k < g_Player[i].count; k++)
	{
		if (g_Player[i].targetable[k] == 99)continue;
		for (int j = 0; j < base->baseNum; j++)
		{
			XMVECTOR v1 = XMLoadFloat3(&base->pos[j]) - XMLoadFloat3(&enemy[g_Player[i].targetable[k]].pos);
			XMFLOAT3 countData;
			XMStoreFloat3(&countData, v1);
			float dist = fabsf(countData.x) + fabsf(countData.y) + fabsf(countData.z);
			if (dist < cmp || cmp <= 0.0f)//初めてここを通る場合は絶対入れるために初期値を参照
			{
				cmp = dist;
				g_Player[i].target = g_Player[i].targetable[k];	//エネミーの配列番号で識別。ポインターで渡したいけど、お互いの構造体にポインターメンバ変数を入れると怒られる…
				XMVECTOR v2 = XMLoadFloat3(&enemy[g_Player[i].targetable[k]].pos) - XMLoadFloat3(&g_Player[i].pos);
				XMStoreFloat3(&countData, v2);
				float angle = atan2f(countData.x, countData.z);
				if (g_Player[i].rot.y == 0.0f)g_Player[i].rot.y = XM_PI * 2.0f;	//バグ修正の為、処理を付け足した
				for (int d = 0; d < g_Player[i].tbl_sizeA; d++)
				{
					float buffx = g_Player[i].tbl_adrA[d].pos.x;
					float buffz = g_Player[i].tbl_adrA[d].pos.z;
					g_Player[i].tbl_adrA[d].pos.x = buffx * cosf(angle - g_Player[i].rot.y) + buffz * sinf(angle - g_Player[i].rot.y);
					g_Player[i].tbl_adrA[d].pos.z = buffz * cosf(angle - g_Player[i].rot.y) + buffx * -sinf(angle - g_Player[i].rot.y);
				}
				g_Player[i].rot.y = angle;
			}
		}
	}
}
void CheckHealTarget(int i)
{
	//攻撃中ではないならここでターゲット決定はしない
	if (g_Player[i].state == Standby) {
		g_Player[i].target = 99;
		return;
	}
	int cmp = 0;
	for (int k = 0; k < g_Player[i].count; k++)
	{
		int num = g_Player[i].targetable[k];	//添え字を格納
		//ターゲットが無いか、ライフがマックスならスルー
		if (g_Player[i].targetable[k] == 99 ||
			g_Player[num].life == g_Player[num].lifeMax)continue;

		if (g_Player[num].life < cmp || cmp <= 0)	//最初に来たときはcmpが0
		{
			XMVECTOR v1 = XMLoadFloat3(&g_Player[num].pos) - XMLoadFloat3(&g_Player[i].pos);
			XMFLOAT3 countData;
			XMStoreFloat3(&countData, v1);
			float angle = atan2f(countData.x, countData.z);
			if (g_Player[i].rot.y == 0.0f)g_Player[i].rot.y = XM_PI * 2.0f;	//バグ修正の為、処理を付け足した
			for (int d = 0; d < g_Player[i].tbl_sizeA; d++)
			{
				float buffx = g_Player[i].tbl_adrA[d].pos.x;
				float buffz = g_Player[i].tbl_adrA[d].pos.z;
				g_Player[i].tbl_adrA[d].pos.x = buffx * cosf(angle - g_Player[i].rot.y) + buffz * sinf(angle - g_Player[i].rot.y);
				g_Player[i].tbl_adrA[d].pos.z = buffz * cosf(angle - g_Player[i].rot.y) + buffx * -sinf(angle - g_Player[i].rot.y);
			}
			g_Player[i].target = num;
			g_Player[i].rot.y = angle;
			cmp = g_Player[num].life;
		}
	}
}

//スキルポイントに関する処理
void IncreaseSP(int i)
{
	if (g_Player[i].skillUse == TRUE)return;

	//一定フレームごとにSPを加算していく
	if (g_Player[i].intervalSP < PLAYER_SP_FLAME)
	g_Player[i].intervalSP++;

	if (g_Player[i].intervalSP >= PLAYER_SP_FLAME &&
		g_Player[i].skillPoint < g_Player[i].skillPointMax) {
		g_Player[i].skillPoint += g_Player[i].increaseSP;
		g_Player[i].intervalSP = 0;
	}

	if (g_Player[i].skillPoint >= g_Player[i].skillPointMax)
		g_Player[i].skillAble = TRUE;
	else
		g_Player[i].skillAble = FALSE;
}

void SetPlayer(XMFLOAT3 pos)
{
	LoadModel(name[0], &g_Player[playerNum].model);
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
		//LoadModel(MODEL_GRAPE_PARTS, &g_Parts[partsNum].model);
		//// モデルのディフューズを保存しておく。色変え対応の為。
		//GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);
		g_Parts[partsNum].load = TRUE;

		g_Parts[partsNum].pos = { 20.0f, 10.0f, 0.0f };		// ポリゴンの位置
		g_Parts[partsNum].rot = { 0.0f, 0.0f, 0.0f };		// ポリゴンの向き(回転)
		g_Parts[partsNum].scl = { 1.0f, 1.0f, 1.0f };		// ポリゴンの大きさ(スケール)

		// 階層アニメーション用のメンバー変数
		g_Parts[partsNum].tbl_adrA = NULL;	// アニメデータのテーブル先頭アドレス
		g_Parts[partsNum].tbl_sizeA = 0;	// 登録したテーブルのレコード総数
		g_Parts[partsNum].tbl_adrM = NULL;	// アニメデータのテーブル先頭アドレス
		g_Parts[partsNum].tbl_sizeM = 0;	// 登録したテーブルのレコード総数
		g_Parts[partsNum].move_time = 0;	// 実行時間
		g_Parts[partsNum].parent = &g_Player[playerNum];	// 自分が親ならNULL、自分が子供なら親のenemyアドレス
		partsNum++;
	}

}

void SetNeutrophils(XMFLOAT3 pos)
{
	LoadModel(name[1], &g_Player[playerNum].model);
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
	g_Player[playerNum].move_time = 0.0f;	// 実行時間

	// 階層アニメーション用の初期化処理
	g_Player[playerNum].parent = NULL;			// 本体（親）なのでNULLを入れる
	playerNum++;

	//パーツの初期化処理
	for (int k = 0; k < g_Player[playerNum].partsNum; k++)
	{
		//LoadModel(MODEL_GRAPE_PARTS, &g_Parts[partsNum].model);
		//// モデルのディフューズを保存しておく。色変え対応の為。
		//GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);
		g_Parts[partsNum].load = TRUE;

		g_Parts[partsNum].pos = { 20.0f, 10.0f, 0.0f };		// ポリゴンの位置
		g_Parts[partsNum].rot = { 0.0f, 0.0f, 0.0f };		// ポリゴンの向き(回転)
		g_Parts[partsNum].scl = { 1.0f, 1.0f, 1.0f };		// ポリゴンの大きさ(スケール)

		// 階層アニメーション用のメンバー変数
		g_Parts[partsNum].tbl_adrA = NULL;	// アニメデータのテーブル先頭アドレス
		g_Parts[partsNum].tbl_sizeA = 0;	// 登録したテーブルのレコード総数
		g_Parts[partsNum].tbl_adrM = NULL;	// アニメデータのテーブル先頭アドレス
		g_Parts[partsNum].tbl_sizeM = 0;	// 登録したテーブルのレコード総数
		g_Parts[partsNum].move_time = 0;	// 実行時間
		g_Parts[partsNum].parent = &g_Player[playerNum];	// 自分が親ならNULL、自分が子供なら親のenemyアドレス
		partsNum++;
	}

}

int GetPlayerNum(void)
{
	return playerNum;
}

void SetPlayerNum(int s)
{
	playerNum += s;
}
int GetPlayerPartsNum(void)
{
	return partsNum;
}

void SetPlayerPartsNum(int s)
{
	partsNum += s;
}
//=============================================================================
// プレイヤー情報を取得
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player[0];
}

PlayerParts *GetPlayerParts(void)
{
	return &g_Parts[0];
}


//現在は最初に登場した敵から順にターゲットテーブルに入れる
void PLAYER::StateCheck(int i)
{
	if (g_Player[i].state == Deffend)return;
	g_Player[i].state = Standby;	//とりあえず待機状態にセット
	ENEMY *enemy = GetEnemy();
	g_Player[i].count = 0;
	for (int k = 0; k < MAX_TARGET; k++)
	{
		g_Player[i].targetable[k] = 99;
	}

	for (int k = 0; k < MAX_ENEMY; k++)
	{
		if (enemy[k].use != TRUE)continue;
		//プレイヤーの攻撃範囲に1体でも敵がいるならば攻撃準備に入る。ターゲット情報も保存
		if (CollisionBC(g_Player[i].pos, enemy[k].pos, g_Player[i].size, 1.0f))
		{
			g_Player[i].state = Deffend;
			g_Player[i].targetable[g_Player[i].count] = k;
			g_Player[i].count++;
		}
	}
}

//プレイヤーキャラの体力バーの表示処理とSP表示処理
void DrawPlayerLife(void)
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
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (g_Player[i].use != TRUE)continue;	//使われてないプレイヤーはスルー
		
		for (int k = 0; k < 2; k++)//最初に最大値体力を、次に現体力を表示
		{
			g_PlayerVar.pos = { g_Player[i].pos.x, g_Player[i].pos.y + 15.0f, g_Player[i].pos.z - 35.0f };
			if (k == 0)
			{
				g_PlayerVar.scl = { 1.0f, 1.0f, 1.0f };
				g_PlayerVar.material.Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.7f);
			}
			else
			{
				float par = (float)(g_Player[i].life) / (float)(g_Player[i].lifeMax);
				g_PlayerVar.scl = { par, 1.0f, 1.0f };
				g_PlayerVar.pos.x -= (VAR_WIDTH * (1.0f - par)) * 0.5f;
				g_PlayerVar.material.Diffuse = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
			}
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_PlayerVar.scl.x, g_PlayerVar.scl.y, g_PlayerVar.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_PlayerVar.rot.x, g_PlayerVar.rot.y, g_PlayerVar.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_PlayerVar.pos.x, g_PlayerVar.pos.y, g_PlayerVar.pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			// マテリアルの設定
			SetMaterial(g_PlayerVar.material);

			// ポリゴンの描画
			GetDeviceContext()->Draw(4, 0);
		}
	}

	DrawPlayerSP();
	// Z比較あり
	SetDepthEnable(TRUE);
	// ライティングを有効に
	SetLightEnable(TRUE);

}

void DrawPlayerSP(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;
	//hpバーを設置
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (g_Player[i].use != TRUE)continue;	//使われてないプレイヤーはスルー

		for (int k = 0; k < 2; k++)//最初に最大値体力を、次に現体力を表示
		{
			g_PlayerVar.pos = { g_Player[i].pos.x, g_Player[i].pos.y + 15.0f, g_Player[i].pos.z - 40.0f };
			if (k == 0)
			{
				g_PlayerVar.scl = { 1.0f, 1.0f, 1.0f };
				g_PlayerVar.material.Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.7f);
			}
			else
			{
				float par = (float)(g_Player[i].skillPoint) / (float)(g_Player[i].skillPointMax);
				g_PlayerVar.scl = { par, 1.0f, 1.0f };
				g_PlayerVar.pos.x -= (VAR_WIDTH * (1.0f - par)) * 0.5f;
				if (par < 1.0f)
					g_PlayerVar.material.Diffuse = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
				else
				{
					g_PlayerVar.material.Diffuse = XMFLOAT4(1.0f, 1.0f, g_PlayerVar.pardiff, 1.0f);
					g_PlayerVar.pardiff += g_PlayerVar.parpolar;
					if(g_PlayerVar.pardiff < 0.0f || g_PlayerVar.pardiff > 1.0f)
					g_PlayerVar.parpolar *= -1.0f;
				}
			}
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_PlayerVar.scl.x, g_PlayerVar.scl.y, g_PlayerVar.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_PlayerVar.rot.x, g_PlayerVar.rot.y, g_PlayerVar.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_PlayerVar.pos.x, g_PlayerVar.pos.y, g_PlayerVar.pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			// マテリアルの設定
			SetMaterial(g_PlayerVar.material);

			// ポリゴンの描画
			GetDeviceContext()->Draw(4, 0);
		}
	}

}

HRESULT MakeVertexPlayerVar(void)
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
