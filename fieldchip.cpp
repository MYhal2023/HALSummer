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
#include "playerSet.h"
#include "sprite.h"
#include "base.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX			(1)				// テクスチャの数
#define BG_MAX				(1)				// 用意する壁の数
#define BG_WIDTH	(960 * 1.0f)
#define BG_HEIGHT	(540 * 0.75f)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// 頂点情報
static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
static char* g_TextureName[] = {
	"data/TEXTURE/title_bg2.png",
};
static MAP_CHIP		g_MapChip[MAX_CHIP_NUM];						// プレイヤー
static MAP_CHIP		g_MapChipObj[MAX_CHIP_NUM];						// プレイヤー
static MAP_CHIP		g_MapSet[MAX_CHIP_HEIGHT][MAX_CHIP_WIDTH + 1];
static MAP_CHIP		g_MapSetObj[MAX_CHIP_HEIGHT][MAX_CHIP_WIDTH + 1];
static FIELD_BG		g_bg[BG_MAX];
static BOOL			g_Load = FALSE;
static int m_width, m_height;
int g_BattleMap[MAX_CHIP_HEIGHT][MAX_CHIP_WIDTH + 1];
int g_BattleMapObj[MAX_CHIP_HEIGHT][MAX_CHIP_WIDTH + 1];
//=============================================================================
// 初期化処理(マップチップセットと背景の用意、マップセットまで)
//=============================================================================
HRESULT InitMapChip(int map[][MAX_CHIP_WIDTH + 1], int mapObj[][MAX_CHIP_WIDTH + 1], int height, int width)
{
	ID3D11Device *pDevice = GetDevice();
	MakeVertexBG();
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
	for (int i = 0; i < MAX_CHIP_HEIGHT; i++)
	{
		for (int k = 0; k < MAX_CHIP_WIDTH; k++)
		{
			g_BattleMap[i][k] = 99;	//全マップチップ場所をリセット(99は配置しない番号)
			g_BattleMapObj[i][k] = 9;	//全マップチップ場所をリセット(9は配置しない番号)
		}
	}

	m_width = width;
	m_height = height;
	//背景設定
	for (int i = 0; i < BG_MAX; i++)
	{
		ZeroMemory(&g_bg[i].material, sizeof(g_bg[i].material));
		g_bg[i].material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		g_bg[i].pos = { m_width * CHIP_SIZE * 0.5f - 25.0f, 0.0f, m_height * CHIP_SIZE -25.0f};
		g_bg[i].rot = { XM_PI * -0.5f, 0.0f, 0.0f };
		g_bg[i].scl = { 1.0f, 1.0f, 1.0f };
		g_bg[i].load = TRUE;
	}
	//マップチップモデル用意
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
		case ROW_BLOOD:
			LoadModel(MODEL_FIELD003, &g_MapChip[i].model);
			break;
		case HIGH_BLOOD:
			LoadModel(MODEL_FIELD004, &g_MapChip[i].model);
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
	//マップチップオブジェクトモデル用意
	for (int i = 0; i < MAX_OBJ_NUM; i++)
	{
		switch (i)
		{
		case Building:
			LoadModel(MODEL_OBJECT001, &g_MapChipObj[i].model);
			g_MapChipObj[i].scl = { 1.0f, 1.0f, 1.0f };
			break;
		case Container:
			LoadModel(MODEL_OBJECT002, &g_MapChipObj[i].model);
			g_MapChipObj[i].scl = { 0.5f, 0.5f, 0.5f };
			break;
		case MAX_VAL:
			break;
		}
		// モデルのディフューズを保存しておく。色変え対応の為。
		GetModelDiffuse(&g_MapChipObj[i].model, &g_MapChipObj[i].diffuse[0]);

		g_MapChipObj[i].load = TRUE;

		g_MapChipObj[i].pos = { 0.0f, 0.0f, 0.0f };
		g_MapChipObj[i].rot = { 0.0f, 0.0f, 0.0f };

		g_MapChipObj[i].size = CHIP_SIZE;	// 当たり判定の大きさ
	}
	//マップチップ情報のリセット
	for (int i = 0; i < MAX_CHIP_HEIGHT; i++)
	{
		for (int k = 0; k < MAX_CHIP_WIDTH; k++)
		{
			g_MapSet[i][k].pos = { 0.0f + CHIP_SIZE * k, 0.0f,  0.0f + CHIP_SIZE * i };
			g_MapSet[i][k].rot = { 0.0f, 0.0f, 0.0f };
			g_MapSet[i][k].scl = { 1.7f, 1.0f, 1.7f };
			g_MapSet[i][k].type = LowPlaces;
			g_MapSet[i][k].use = FALSE;
		}
	}
	//マップチップオブジェ情報のリセット
	for (int i = 0; i < MAX_CHIP_HEIGHT; i++)
	{
		for (int k = 0; k < MAX_CHIP_WIDTH; k++)
		{
			g_MapSetObj[i][k].pos = { 0.0f + CHIP_SIZE * k, 0.0f,  0.0f + CHIP_SIZE * i };
			g_MapSetObj[i][k].rot = { 0.0f, 0.0f, 0.0f };
			g_MapSetObj[i][k].scl = { 2.0f, 2.0f, 2.0f };
			g_MapSetObj[i][k].type = LowPlaces;
			g_MapSetObj[i][k].use = FALSE;
		}
	}

	SetBattleMap(map, height, width);	//ここでマップをセットしている
	SetBattleMapObj(mapObj, height, width);	//ここでマップをセットしている

	g_Load = TRUE;
	return S_OK;

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMapChip(void)
{
	if (g_Load == FALSE) return;
	// 頂点バッファの解放
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < MAX_CHIP_NUM; i++)
	{
		// モデルの解放処理
		if (g_MapChip[i].load)
		{
			UnloadModel(&g_MapChip[i].model);
			g_MapChip[i].load = FALSE;
		}
	}
	for (int i = 0; i < MAX_CHIP_NUM; i++)
	{
		// モデルの解放処理
		if (g_MapChipObj[i].load)
		{
			UnloadModel(&g_MapChipObj[i].model);
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
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;
	//フィールドチップ用シェーダーに切り替え
	//選択されているチップは色が変わる
	SwapShader(MODE_FIELD_CHIP);
	PlayerSet *ps = GetSetPos();
	Base *bs = GetBase();
	for (int i = 0; i < m_height; i++)
	{
		for (int k = 0; k < m_width; k++)
		{
			if (ps->setPos.x == g_MapSet[i][k].pos.x &&
				ps->setPos.z == g_MapSet[i][k].pos.z &&
				ps->setMode == TRUE)
				SetChip(1);
			else
				SetChip(0);

			for (int s = 0; s < bs->baseNum; s++)
			{
				if (bs->pos->x - 25.0f == g_MapSet[i][k].pos.x &&
					bs->pos->z == g_MapSet[i][k].pos.z) {
					SetChip(2);
					break;
				}
			}
			int m = g_BattleMap[i][k];
			if (m == 99)continue;	//マップチップ無し。描画不要
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

	SwapShader(MODE_PLANE);

	DrawMapObject();

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}

void DrawBG(void)
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
	//背景設置
	for (int i = 0; i < BG_MAX; i++)
	{
		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_bg[i].scl.x, g_bg[i].scl.y, g_bg[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_bg[i].rot.x, g_bg[i].rot.y, g_bg[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_bg[i].pos.x, g_bg[i].pos.y, g_bg[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		// マテリアルの設定
		SetMaterial(g_bg[i].material);

		// ポリゴンの描画
		GetDeviceContext()->Draw(4, 0);
	}
	// Z比較あり
	SetDepthEnable(TRUE);
	// ライティングを無効
	SetLightEnable(TRUE);

}

void DrawMapObject(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;
	for (int i = 0; i < m_height; i++)
	{
		for (int k = 0; k < m_width; k++)
		{
			int m = g_BattleMapObj[i][k];
			if (m == 9)continue;	//マップチップ無し。描画不要
			switch (m)
			{
			case Building:
				g_MapSetObj[i][k].scl = { 2.0f, 2.0f, 2.0f };
				break;
			case Container:
				g_MapSetObj[i][k].scl = { 1.2f, 0.8f, 0.8f };
				g_MapSetObj[i][k].rot = { 0.0f, XM_PI * 0.5f, 0.0f };
				break;
			}
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_MapSetObj[i][k].scl.x, g_MapSetObj[i][k].scl.y, g_MapSetObj[i][k].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_MapSetObj[i][k].rot.x, g_MapSetObj[i][k].rot.y + XM_PI, g_MapSetObj[i][k].rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_MapSetObj[i][k].pos.x, g_MapSetObj[i][k].pos.y, g_MapSetObj[i][k].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_MapSetObj[i][k].mtxWorld, mtxWorld);

			// モデル描画
			//ここだけ事前に用意したマップチップナンバーを参照
			DrawModel(&g_MapChipObj[m].model);
		}
	}

}

HRESULT MakeVertexBG(void)
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
		vertex[0].Position = XMFLOAT3(-BG_WIDTH / 2, 0.0f, BG_HEIGHT / 2);
		vertex[1].Position = XMFLOAT3(BG_WIDTH / 2, 0.0f, BG_HEIGHT / 2);
		vertex[2].Position = XMFLOAT3(-BG_WIDTH / 2, 0.0f, -BG_HEIGHT / 2);
		vertex[3].Position = XMFLOAT3(BG_WIDTH / 2, 0.0f, -BG_HEIGHT / 2);

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

//バトルマップセット。マップ配列と縦横の長さを引数に渡す。ついでにチップタイプもセット
void SetBattleMap(int map[][MAX_CHIP_WIDTH + 1], int height, int width)
{
	for (int i = 0; i < height; i++)
	{
		for (int k = 0; k < width; k++)
		{
			g_BattleMap[i][k] = map[i][k];
			switch (g_BattleMap[i][k])
			{
			case ROW:
			case ROW_BLOOD:
				g_MapSet[i][k].type = LowPlaces;
				break;
			case HIGH:
			case HIGH_BLOOD:
				g_MapSet[i][k].type = HighPlaces;
				break;
			}
		}
	}
}
//バトルマップセット。マップ配列と縦横の長さを引数に渡す。ついでにチップタイプもセット
void SetBattleMapObj(int map[][MAX_CHIP_WIDTH + 1], int height, int width)
{
	for (int i = 0; i < height; i++)
	{
		for (int k = 0; k < width; k++)
		{
			g_BattleMapObj[i][k] = map[i][k];

			switch (g_MapSet[i][k].type)
			{
			case LowPlaces:
				g_MapSetObj[i][k].pos.y = 8.0f;
				break;
			case HighPlaces:
				g_MapSetObj[i][k].pos.y = 20.0f;
				break;
			}
			if(map[i][k] != 9)
			g_MapSet[i][k].type = NoEntry;	//何かしらがあるなら進入禁止で上書き
		}
	}
}

//=============================================================================
// プレイヤー情報を取得
//=============================================================================
MAP_CHIP *GetMapChip(void)
{
	return &g_MapChip[0];
}
//あらかじめ、座標をチップサイズで除算した引数を渡す
//2がx座標、1がz座標
int GetMapChiptype(int i, int k)
{
	return g_MapSet[i][k].type;
}

void SetMapChipUse(BOOL flag, int i, int k)
{
	g_MapSet[i][k].use = flag;
}

BOOL GetMapChipUse(int i, int k)
{
	return g_MapSet[i][k].use;
}

float GetMapWidth(void)
{
	return m_width * CHIP_SIZE;
}
float GetMapHeight(void)
{
	return m_height * CHIP_SIZE;
}