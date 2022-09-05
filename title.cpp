//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "sprite.h"
#include "renderer.h"
#include "input.h"
#include "game.h"
#include "title.h"
#include "gameover.h"
#include "debugproc.h"
#include "fade.h"
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX			(6)				// テクスチャの数
#define TEXTURE_SPEED		(0.1f)		// テクスチャの移動速度
#define TITLE_BUTTON_NUM	(2)			//
#define COLOR_SPEED			(0.01f)		// テクスチャの変色速度


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// 頂点情報
static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
static char* g_TextureName[] = {
	"data/TEXTURE/title_bg.png",
	"data/TEXTURE/title_allies.png",
	"data/TEXTURE/title_enemy.png",
	"data/TEXTURE/newgame.png",
	"data/TEXTURE/loadgame.png",
	"data/TEXTURE/title_logo.png",
};
static Title g_Title[TEXTURE_MAX];
static BOOL g_Load = FALSE;
static float speed[TEXTURE_MAX];
static int cursol;
HRESULT InitTitle(void)
{
	ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
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
		g_Title[i].textNo = i;
		switch (i)
		{
		case title_bg:
			g_Title[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			g_Title[i].pos = { SCREEN_CENTER_X, SCREEN_CENTER_Y };
			g_Title[i].size = { SCREEN_WIDTH, SCREEN_HEIGHT };
			break;
		case title_allies:
			g_Title[i].color = { 1.0f, 1.0f, 1.0f, 0.0f };
			g_Title[i].pos = { SCREEN_CENTER_X, SCREEN_CENTER_Y };
			g_Title[i].size = { SCREEN_WIDTH, SCREEN_HEIGHT *1.1f};
			break;
		case title_enemy:
			g_Title[i].color = { 1.0f, 1.0f, 1.0f, 0.0f };
			g_Title[i].pos = { SCREEN_CENTER_X, SCREEN_CENTER_Y };
			g_Title[i].size = { SCREEN_WIDTH, SCREEN_HEIGHT };
			break;
		case newgame:
			g_Title[i].color = { 1.0f, 1.0f, 1.0f, 0.8f };
			g_Title[i].pos = { SCREEN_CENTER_X, SCREEN_CENTER_Y *1.2f };
			g_Title[i].size = { 390, 90 };
			break;
		case loadgame:
			g_Title[i].color = { 1.0f, 1.0f, 1.0f, 0.8f };
			g_Title[i].pos = { SCREEN_CENTER_X, SCREEN_CENTER_Y *1.2f };
			g_Title[i].size = { 390, 90 };
			break;
		case title_logo:
			g_Title[i].color = { 1.0f, 1.0f, 1.0f, 0.0f };
			g_Title[i].pos = { SCREEN_CENTER_X, SCREEN_HEIGHT * 0.18f };
			g_Title[i].size = { 400*1.75f, 200*1.75f };
			break;
		}
	}
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		switch (i)
		{
		case title_enemy:
			speed[i] = TEXTURE_SPEED;
			break;
		case title_allies:
			speed[i] = TEXTURE_SPEED * 4.0f;
			break;
		case newgame:
			speed[i] = COLOR_SPEED;
			break;
		case loadgame:
			speed[i] = COLOR_SPEED;
			break;
		case title_logo:
			speed[i] = 0.1f;
			break;
		}
	}
	cursol = 0;
		g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
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
void UpdateTitle(void)
{
	//一つ一つをフェードインで登場させる
	if (g_Title[title_allies].color.w < 1.0f) {
		if (g_Title[title_bg].color.w < 1.0f)
			FadeTexture(0, &g_Title[title_bg]);

		if (g_Title[title_bg].color.w > 0.5f && g_Title[title_enemy].color.w < 1.0f)
		{
			FadeTexture(0, &g_Title[title_enemy]);
		}

		if (g_Title[title_enemy].color.w > 0.5f && g_Title[title_allies].color.w < 1.0f)
		{
			FadeTexture(0, &g_Title[title_allies]);
		}

		if (g_Title[title_logo].color.w < 1.0f) {
			FadeTexture(0, &g_Title[title_logo]);
		}
	}
	//フェードインしたら各自で動かさせる
	if (g_Title[title_enemy].color.w > 0.5f) {
		if (g_Title[title_enemy].pos.y > SCREEN_CENTER_Y * 1.025f || g_Title[title_enemy].pos.y < SCREEN_CENTER_Y * 0.975f)
			speed[title_enemy] *= -1;
		MoveTexture(speed[title_enemy], &g_Title[title_enemy]);
	}

	if (g_Title[title_allies].color.w > 0.5f) {

		if (g_Title[title_allies].pos.y > SCREEN_CENTER_Y * 1.025f || g_Title[title_allies].pos.y < SCREEN_CENTER_Y * 1.0f)
			speed[title_allies] *= -1;
		MoveTexture(speed[title_allies], &g_Title[title_allies]);
	}

	//カーソルの変色
	int num = newgame + cursol;
	if (g_Title[num].color.w > 1.2f || g_Title[num].color.w < 0.7f)
		speed[num] *= -1;
	g_Title[num].color.w += speed[num];
	g_Title[num].color.x += speed[num];
	g_Title[num].color.y += speed[num];
	g_Title[num].color.z += speed[num];

	//カーソル変更
	if (GetKeyboardTrigger(DIK_LEFT) && cursol > 0)
	{
		cursol--;
		PlaySound(SOUND_LABEL_SE_Select);
	}
	else if (GetKeyboardTrigger(DIK_RIGHT) && cursol < TITLE_BUTTON_NUM - 1)
	{
		cursol++;
		PlaySound(SOUND_LABEL_SE_Select);
	}
	if (GetKeyboardTrigger(DIK_Z) || GetKeyboardTrigger(DIK_RETURN))
	{
		switch (cursol) {
		case 0://はじめから
			PlaySound(SOUND_LABEL_SE_Decision);
			SetFade(FADE_OUT, MODE_RESERVE, BlackFade);
			break;
		case 1://つづきから
			PlaySound(SOUND_LABEL_SE_Decision);
			SetFade(FADE_OUT, MODE_RESERVE, BlackFade);
			break;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
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

	// 背景を描画
		// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Title[title_bg].textNo]);

	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);


	// エネミー群を描画
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Title[title_enemy].textNo]);

	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, g_Title[title_enemy].pos.x, g_Title[title_enemy].pos.y, g_Title[title_enemy].size.x, g_Title[title_enemy].size.y, 0.0f, 0.0f, 1.0f, 1.0f,
		g_Title[title_enemy].color);

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);


	// プレイヤー群を描画
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Title[title_allies].textNo]);

	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, g_Title[title_allies].pos.x, g_Title[title_allies].pos.y, g_Title[title_allies].size.x, g_Title[title_allies].size.y, 0.0f, 0.0f, 1.0f, 1.0f,
		g_Title[title_allies].color);

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

	// タイトルロゴを描画
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Title[title_logo].textNo]);

	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, g_Title[title_logo].pos.x, g_Title[title_logo].pos.y, g_Title[title_logo].size.x, g_Title[title_logo].size.y, 0.0f, 0.0f, 1.0f, 1.0f,
		g_Title[title_logo].color);

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

	// カーソル描画
	// テクスチャ設定
	int num = newgame + cursol;
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Title[num].textNo]);

	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, g_Title[num].pos.x, g_Title[num].pos.y, g_Title[num].size.x, g_Title[num].size.y, 0.0f, 0.0f, 1.0f, 1.0f,
		g_Title[num].color);

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

	if (cursol > 0) {
		int num = newgame + cursol - 1;
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Title[num].textNo]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, g_Title[num].pos.x - 390.0f, g_Title[num].pos.y + 45.0f, g_Title[num].size.x * 0.5f, g_Title[num].size.y * 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
			g_Title[num].color);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
	if (cursol < TITLE_BUTTON_NUM - 1) {
		int num = newgame + cursol + 1;
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Title[num].textNo]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, g_Title[num].pos.x + 390.0f, g_Title[num].pos.y + 45.0f, g_Title[num].size.x * 0.5f, g_Title[num].size.y * 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
			g_Title[num].color);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
}
//0はフェードイン、1はフェードアウト
void FadeTexture(int type, Title *title)
{
	switch (type) {
	case 0: 
		title->color.w += 0.01f;
			break;
	case 1:
		title->color.w -= 0.01f;
		break;
	}
}

//移動速度、座標最大値、座標最小値、ポインター
void MoveTexture(float speed, Title *title)
{
	title->pos.y += speed;
}