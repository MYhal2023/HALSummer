//=============================================================================
//
// ゲームオーバー処理 [gameovera.cpp]
// Author : 
//
//=============================================================================
#include "gameover.h"
#include "game.h"
#include "sprite.h"
#include "renderer.h"
#include "input.h"
#include "base.h"
#include "fade.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX			(1)				// テクスチャの数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// 頂点情報
static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
static char* g_TextureName[] = {
	"data/TEXTURE/fade_red.png",
};

static Over g_Over[TEXTURE_MAX];
static BOOL g_Load = FALSE;
HRESULT InitOver(void)
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
		g_Over[i].color = { 1.0f, 1.0f, 1.0f, 0.0f };
		g_Over[i].pos = { SCREEN_CENTER_X, SCREEN_CENTER_Y };
		g_Over[i].size = { SCREEN_WIDTH, SCREEN_HEIGHT };
		g_Over[i].use = TRUE;
	}
	g_Load = TRUE;
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitOver(void)
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

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateOver(void)
{
	Base *base = GetBase();
	if (!CheckGameover())return;
	//ここからはゲームオーバーとなった時に入る
	if (g_Over[FadeRed].color.w < 0.5f)
		g_Over[FadeRed].color.w += 0.025f;
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		SetFade(FADE_OUT, MODE_GAME);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawOver(void)
{
	if (!CheckGameover())return;

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

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Over[i].use != TRUE)continue;
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[i]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, g_Over[i].pos.x, g_Over[i].pos.y, g_Over[i].size.x, g_Over[i].size.y, 0.0f, 0.0f, 1.0f, 1.0f,
			g_Over[i].color);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
	SetDepthEnable(TRUE);

	// ライティングを無効
	SetLightEnable(TRUE);

}