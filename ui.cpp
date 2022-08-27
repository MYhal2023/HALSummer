//=============================================================================
//
// UI処理 [ui.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "sprite.h"
#include "renderer.h"
#include "game.h"
#include "playerSet.h"
#include "ui.h"
#include "base.h"
#include "cost.h"
#include "team.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX			(9)				// テクスチャの数
#define CHAR_TEXTURE_MAX	(7)				// キャラテクスチャの数
#define NUMBER_SIZE			(30.0f)			// x方向のサイズ
#define COST_NUMBER_SIZE	(45.0f)			// x方向のサイズ

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// 頂点情報
static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
static ID3D11ShaderResourceView		*g_CharTexture[CHAR_TEXTURE_MAX] = { NULL };	// テクスチャ情報
static char* g_TextureName[] = {
	"data/TEXTURE/box.png",
	"data/TEXTURE/var.png",
	"data/TEXTURE/number.png",
	"data/TEXTURE/HP_UI.png",
	"data/TEXTURE/ENEMY_UI.png",
	"data/TEXTURE/button_normal.png",
	"data/TEXTURE/button_double.png",
	"data/TEXTURE/button_stop.png",
	"data/TEXTURE/costbox.png",
};
static char* g_CharTextureName[] = {
	"data/TEXTURE/neutro.png",
	"data/TEXTURE/neutro.png",
	"data/TEXTURE/macro.png",
	"data/TEXTURE/thelper.png",
	"data/TEXTURE/tkiller.png",
	"data/TEXTURE/var.png",
	"data/TEXTURE/var.png",
};
static UI g_UI[TEXTURE_MAX];
static BOOL g_Load = FALSE;

HRESULT InitUI(void)
{
	ID3D11Device *pDevice = GetDevice();

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
	// テクスチャ生成
	for (int i = 0; i < CHAR_TEXTURE_MAX; i++)
	{
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_CharTextureName[i],
			NULL,
			NULL,
			&g_CharTexture[i],
			NULL);
	}
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_UI[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };
		g_UI[i].pos = { 0.0f, 0.0f };
		g_UI[i].size = { 50.0f, 50.0f };
		g_UI[i].use = TRUE;
	}

	g_UI[var_bg].pos = { SCREEN_CENTER_X, 50.0f };
	g_UI[var_bg].size = { 500.0f, 80.0f };
	g_UI[var_bg].color = { 0.0f, 0.0f, 0.0f, 0.6f };
	g_UI[baseLife].pos = { g_UI[var_bg].pos.x + 80.0f, 50.0f };
	g_UI[baseLife].size = { 642.0f * 0.065f, 990.0f*0.065f };
	g_UI[enemyNum].pos = { g_UI[var_bg].pos.x - 190.0f, 50.0f };
	g_UI[enemyNum].size = { 65.0f, 65.0f };
	const float mnp = 0.15f;
	g_UI[button_n].pos = { 1600.0f, 60.0f };
	g_UI[button_n].size = { 1000.0f * mnp, 600.0f * mnp };
	g_UI[button_d].pos = { 1600.0f, 60.0f };
	g_UI[button_d].size = { 1000.0f * mnp, 600.0f * mnp };
	g_UI[button_s].pos = { 1800.0f, 60.0f };
	g_UI[button_s].size = { 1000.0f * mnp, 600.0f * mnp };
	g_UI[costbox].size = { 160.0f, 160.0f };
	g_UI[costbox].pos = { SCREEN_WIDTH - g_UI[costbox].size.x*0.5f , SCREEN_HEIGHT - g_UI[costbox].size.y * 0.5f };

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitUI(void)
{
	if (g_Load == FALSE) return;

	// 頂点バッファの解放
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	// テクスチャの解放
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}
	// テクスチャの解放
	for (int i = 0; i < CHAR_TEXTURE_MAX; i++)
	{
		if (g_CharTexture[i])
		{
			g_CharTexture[i]->Release();
			g_CharTexture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateUI(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawUI(void)
{
	SetDepthEnable(FALSE);

	// ライティングを無効
	SetLightEnable(FALSE);
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	DrawCharBox();
	DrawUIbg();
	DrawLife();
	DrawEnemyNum();
	DrawButtonNorD();
	DrawButtonStop();
	DrawCost();
	SetDepthEnable(TRUE);

	// ライティングを無効
	SetLightEnable(TRUE);

}

//=============================================================================
// キャラボックス描画処理
//=============================================================================
void DrawCharBox(void)
{
	g_UI[charBox].size = { 160.0f, 160.0f };
	int k = 1;
	for (int i = MAX_PLAYER_SET - 1; i >= 0; i--)
	{
		PlayerSet *ps = GetSetPos();
		PlayerStatus *member = GetTeam();

		if (ps->use[i] != TRUE)continue;	//未使用編成枠はスルー

		//if (member[i].setAble)
		//	g_UI[charBox].color = { 1.0f, 1.0f, 1.0f, 1.0f };
		//else
		//	g_UI[charBox].color = { 0.3f, 0.3f, 0.3f, 1.0f };
		//右から順番に、編成の最後尾から描画
		g_UI[charBox].pos = { (SCREEN_WIDTH * 0.8f) - k * g_UI[charBox].size.x, SCREEN_HEIGHT - g_UI[charBox].size.y * 0.5f };
		k++;
		//キャラIDを抽出してキャラクターを最初に描画
		int id = ps->setCharID[i];
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_CharTexture[id]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, g_UI[charBox].pos.x, g_UI[charBox].pos.y, g_UI[charBox].size.x, g_UI[charBox].size.y, 0.0f, 0.0f, 1.0f, 1.0f,
			g_UI[charBox].color);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);


		//キャラの上にボックスを描画
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[charBox]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, g_UI[charBox].pos.x, g_UI[charBox].pos.y, g_UI[charBox].size.x, g_UI[charBox].size.y, 0.0f, 0.0f, 1.0f, 1.0f,
			g_UI[charBox].color);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
}

void DrawUIbg(void)
{
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[var_bg]);

	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, g_UI[var_bg].pos.x, g_UI[var_bg].pos.y, g_UI[var_bg].size.x, g_UI[var_bg].size.y, 0.0f, 0.0f, 1.0f, 1.0f,
		g_UI[var_bg].color);

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

}

//引数:表示したい数字、表示座標(x,y)、表示サイズ(x方向,y方向)
void DrawNumber(int numb, float px, float py, float sx, float sy, XMFLOAT4 color)
{
	int digit = 0;
	int clock = numb;
	if (clock != 0)
	{
		while (clock != 0)
		{
			clock /= 10;
			digit++;
		}
	}
	else
		digit = 1;

	for (int i = 0; i < digit; i++)
	{
		px = px - sx * i;
		float x = (float)(numb % 10);		//今回表示する数字
		float tx = x * 0.1f;			// テクスチャの左上X座標

		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[number]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, px, py, sx, sy, tx, 0.0f, 0.1f, 1.0f,
			color);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
		numb /= 10;
	}
}


void DrawLife(void)
{
	DrawNumber(GetBaseLife(), g_UI[baseLife].pos.x + g_UI[baseLife].size.x * 1.1f, g_UI[baseLife].pos.y, NUMBER_SIZE, NUMBER_SIZE * 1.5f, XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[baseLife]);

	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, g_UI[baseLife].pos.x, g_UI[baseLife].pos.y, g_UI[baseLife].size.x, g_UI[baseLife].size.y, 0.0f, 0.0f, 1.0f, 1.0f,
		g_UI[baseLife].color);

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

}

void DrawEnemyNum(void)
{
	int life = GetEnemyNum() - GetBanishEnemy();
	DrawNumber(life, g_UI[enemyNum].pos.x + g_UI[enemyNum].size.x * 2.0f, g_UI[enemyNum].pos.y, NUMBER_SIZE, NUMBER_SIZE * 1.5f, XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[enemyNum]);

	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, g_UI[enemyNum].pos.x, g_UI[enemyNum].pos.y, g_UI[enemyNum].size.x, g_UI[enemyNum].size.y, 0.0f, 0.0f, 1.0f, 1.0f,
		g_UI[enemyNum].color);

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

}

void DrawButtonNorD(void)
{
	if (GetSpeedMode() == 1)
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[button_n]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, g_UI[button_n].pos.x, g_UI[button_n].pos.y, g_UI[button_n].size.x, g_UI[button_n].size.y, 0.0f, 0.0f, 1.0f, 1.0f,
			g_UI[button_n].color);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
	else if (GetSpeedMode() == 2)
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[button_d]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, g_UI[button_d].pos.x, g_UI[button_d].pos.y, g_UI[button_d].size.x, g_UI[button_d].size.y, 0.0f, 0.0f, 1.0f, 1.0f,
			g_UI[button_d].color);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
}

void DrawButtonStop(void)
{
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[button_s]);

	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, g_UI[button_s].pos.x, g_UI[button_s].pos.y, g_UI[button_s].size.x, g_UI[button_s].size.y, 0.0f, 0.0f, 1.0f, 1.0f,
		g_UI[button_s].color);

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);
}

void DrawCost(void)
{
	Cost *cost = GetCostNum();
	float per = (float)(cost->time) / (float)(cost->costMaxTime);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[costbox]);

	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, g_UI[costbox].pos.x, g_UI[costbox].pos.y, g_UI[costbox].size.x, g_UI[costbox].size.y, 0.0f, 0.0f, 1.0f, 1.0f,
		g_UI[costbox].color);

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[var_bg]);

	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, g_UI[costbox].pos.x, g_UI[costbox].pos.y - (g_UI[costbox].size.y * per) * 0.5f + g_UI[costbox].size.y * 0.5f, g_UI[costbox].size.x, g_UI[costbox].size.y * per, 0.0f, 0.0f, 1.0f, 1.0f,
		XMFLOAT4(0.0f, 1.0f, 0.0f, 0.7f));

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

	DrawNumber(GetCost(), g_UI[costbox].pos.x + COST_NUMBER_SIZE * 0.5f, g_UI[costbox].pos.y, COST_NUMBER_SIZE, COST_NUMBER_SIZE * 1.25f,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
}