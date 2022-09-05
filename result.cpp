//=============================================================================
//
// リザルト処理 [result.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "sprite.h"
#include "renderer.h"
#include "input.h"
#include "game.h"
#include "result.h"
#include "gameover.h"
#include "debugproc.h"
#include "fade.h"
#include "reserve.h"
#include "playerSet.h"
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX			(8)				// テクスチャの数
#define CH_TEXTURE_MAX			(3)				// テクスチャの数
#define ROW_NUM				(5)				// 一列に並べるユニット数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// 頂点情報
static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
static ID3D11ShaderResourceView		*g_CharTexture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
static char* g_TextureName[TEXTURE_MAX] = {
	"data/TEXTURE/win_texture.png",
	"data/TEXTURE/lose_texture.png",
	"data/TEXTURE/number.png",
	"data/TEXTURE/var.png",
	"data/TEXTURE/icon_energy.png",
	"data/TEXTURE/icon_oxygen.png",
	"data/TEXTURE/icon_iron.png",
	"data/TEXTURE/Result.png",

};
static char* g_CharTextureName[CH_TEXTURE_MAX] = {
	"data/TEXTURE/neutro.png",
	"data/TEXTURE/neutro.png",
	"data/TEXTURE/macro.png",
};

static Result g_Result;
static Reward g_Reward;
static BOOL g_Load = FALSE;
static BOOL once = FALSE;
static BOOL bgm = FALSE;
static float alpha[10];
HRESULT InitResult(void)
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
	for (int i = 0; i < CH_TEXTURE_MAX; i++)
	{
		g_CharTexture[i] = NULL;
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

	g_Result.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	g_Result.pos = { SCREEN_CENTER_X, SCREEN_CENTER_Y };
	g_Result.size = { SCREEN_WIDTH, SCREEN_HEIGHT };
	g_Result.textNo = 0;
	g_Result.type = GetOverType();
	g_Result.enemyNum = 0;
	g_Result.beatNum = 0;
	for (int i = 0; i < MAX_PLAYER_NUM; i++) {
		g_Result.charId[i] = 0;
		g_Result.damage[i] = 0;
	}
	for (int i = 0; i < 10; i++) { alpha[i] = 0.0f; }
	g_Load = TRUE;
	once = FALSE;
	bgm = FALSE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
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
	for (int i = 0; i < CH_TEXTURE_MAX; i++)
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
void UpdateResult(void)
{
	switch (g_Result.type)
	{
	case OVER_WIN:
		g_Result.textNo = 0;
		WinResult();
		break;
	case OVER_LOSE:
		g_Result.textNo = 1;
		LoseResult();
		break;
	}

//#ifdef _DEBUG
//	PrintDebugProc("")
//#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
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

	// リザルトの背景を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Result.textNo]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

	//// 報告書を描画
	//{
	//	// テクスチャ設定
	//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[Report]);
	//	const float x = 1754 * 0.4f;
	//	const float y = 2480 * 0.4f;
	//	// １枚のポリゴンの頂点とテクスチャ座標を設定
	//	SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X, SCREEN_CENTER_Y, x, y, 0.0f, 0.0f, 1.0f, 1.0f,
	//		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	//	// ポリゴン描画
	//	GetDeviceContext()->Draw(4, 0);
	//}


}

void WinResult(void)
{
	if (!bgm) { 
		PlaySound(SOUND_LABEL_BGM_Win);
		bgm = TRUE;
	}
	if (GetKeyboardTrigger(DIK_RETURN) && !once)
	{
		Reserve* rs = GetReserve();
		rs->day++;
		IncreaseReward(&g_Reward);
		once = TRUE;
		SetFade(FADE_OUT, MODE_RESERVE, BlackFade);	//現状ループするように
	}
}

void LoseResult(void)
{
	if (!bgm) {
		PlaySound(SOUND_LABEL_BGM_Lose);
		bgm = TRUE;
	}
	if (GetKeyboardTrigger(DIK_RETURN) && !once)
	{
		once = TRUE;
		SetFade(FADE_OUT, MODE_RESERVE, BlackFade);	//現状ループするように
	}
}

void InitReward(void)
{
	g_Reward.num = 0;
	for (int i = 0; i < MAX_REWARD; i++) {
		g_Reward.ID[i] = 99;
		g_Reward.value[i] = 0;
	}
}

void DrawReward(XMFLOAT2 pos, float size)
{
	for (int i = 0; i < g_Reward.num; i++) {
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Reward.ID[i] + result_energy]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, pos.x, pos.y, size, size, 0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

		DrawResultNumber(g_Reward.value[i], pos.x, pos.y + size, size * 0.25f, size * 0.5f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	}
}
//一種類ずつ設定。最大設定数は5
void SetReward(int id, int value)
{
	g_Reward.ID[g_Reward.num] = id;
	g_Reward.value[g_Reward.num] = value;
	g_Reward.num++;
}

void IncreaseReward(Reward *reward)
{
	Reserve *reserve = GetReserve();
	for (int i = 0; i < reward->num; i++) {
		switch (reward->ID[i])
		{
		case energy:
			reserve->energy += reward->value[i];
			break;
		case oxygen:
			reserve->oxygen += reward->value[i];
			break;
		case iron:
			reserve->iron += reward->value[i];
			break;
		}
	}
}

Reward *GetReward(void) { return &g_Reward; };

//こっからUI描画に関するやつ
void DrawResultButton(XMFLOAT4 color, float px, float py, float sx, float sy)
{
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[result_var]);

	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, px, py, sx, sy, 0.0f, 0.0f, 1.0f, 1.0f,
		color);

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

}
//引数:表示したい数字、表示座標(x,y)、表示サイズ(x方向,y方向)
void DrawResultNumber(int numb, float px, float py, float sx, float sy, XMFLOAT4 color)
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

	float psx = px + sx * digit;
	for (int i = 0; i < digit; i++)
	{
		psx -= sx;
		float x = (float)(numb % 10);		//今回表示する数字
		float tx = x * 0.1f;			// テクスチャの左上X座標

		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[result_numb]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, psx, py, sx, sy, tx, 0.0f, 0.1f, 1.0f,
			color);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
		numb /= 10;
	}
}

//=============================================================================
// キャラボックス描画処理
//=============================================================================
void DrawResultChar(void)
{
	const float boxsize = 180.0f;	//ボックスサイズ定義
	const float boxbuff = 24.0f;
	float buffx = 0.0f;	//ボックス間の距離を少し開ける
	float buffy = 0.0f;	//ボックス間の距離を少し開ける
	const int rowNum = 5;
	int k = 0;
	for (int i = 0; i < MAX_PLAYER_SET; i++)
	{
		PlayerStatus *member = GetTeam();

		if (member[i].use != TRUE)continue;	//未使用編成枠はスルー

		if (k % rowNum != 0)buffx = boxbuff * (k % rowNum);
		else buffx = 0.0f;

		if (k >= rowNum)buffy = boxbuff;

		//右から順番に、編成の最後尾から描画
		XMFLOAT2 pos = { (SCREEN_WIDTH * 0.25f) + (k %ROW_NUM) * boxsize + buffx, (SCREEN_HEIGHT *0.2f) + boxsize * (0.5f + (float)(k / ROW_NUM)) + buffy };
		//キャラの下にボックスを描画
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[result_var]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, pos.x, pos.y, boxsize, boxsize, 0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

		//キャラIDを抽出してキャラクターを最初に描画
		int id = member[i].charID;
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_CharTexture[id]);
		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, pos.x, pos.y, boxsize, boxsize, 0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

		k++;

	}
}
