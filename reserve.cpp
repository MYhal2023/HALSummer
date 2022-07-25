//=============================================================================
//
// Reserve処理 [ui.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "sprite.h"
#include "renderer.h"
#include "game.h"
#include "playerSet.h"
#include "reserve.h"
#include "base.h"
#include "cost.h"
#include "team.h"
#include "ui.h"
#include "text_texture.h"
#include "fade.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX			(8)				// テクスチャの数
#define CHAR_TEXTURE_MAX	(3)				// キャラテクスチャの数
#define NUMBER_SIZE			(30.0f)			// x方向のサイズ
#define COST_NUMBER_SIZE	(45.0f)			// x方向のサイズ
#define BUTTON_SIZE			(106.0f)		// ボタンの縦幅サイズ。多分これくらい
#define BUTTON_MAX			(3)				// ユーサーが選択できるボタン数
#define ROW_NUM				(5)				// 一列に並べるユニット数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// 頂点情報
static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
static ID3D11ShaderResourceView		*g_CharTexture[CHAR_TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char* g_TextureName[TEXTURE_MAX] = {
	"data/TEXTURE/var.png",
	"data/TEXTURE/title_bg.png",
	"data/TEXTURE/number.png",
	"data/TEXTURE/costbox.png",
	"data/TEXTURE/t_confirm.png",
	"data/TEXTURE/t_day.png",
	"data/TEXTURE/t_powerup.png",
	"data/TEXTURE/t_start.png",

};
static char* g_CharTextureName[] = {
	"data/TEXTURE/neutro.png",
	"data/TEXTURE/neutro.png",
	"data/TEXTURE/macro.png",
};

static Reserve g_Reserve;
static TEXT_TEXTURE g_text[TEXTURE_MAX];
static Button g_Button[BUTTON_MAX];
static int cursol;
static BOOL g_Load = FALSE;
static BOOL restart = FALSE;

HRESULT InitReserve(void)
{
	//起動時、一度だけ初期化
	if (!restart)
	{
		g_Reserve.day = 1;
		g_Reserve.energy = 0;
		g_Reserve.oxygen = 0;
		g_Reserve.mode = 99;
		g_Reserve.selectPw = 0;
		restart = TRUE;
	}

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

	for (int i = 0; i < BUTTON_MAX; i++)
	{
		g_Button[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };
		g_Button[i].pos = { 0.0f, 0.0f };
		g_Button[i].size = { 50.0f, 50.0f };
		g_Button[i].use = TRUE;
	}


	float py = BUTTON_SIZE * 0.5f + 16.0f;;
	
	g_Button[UnitPowerUp].pos.y = py + BUTTON_SIZE + 40.0f;
	g_Button[UnitPowerUp].pos.x = 8.0f + BUTTON_SIZE * 1.5f;
	
	g_Button[UnitConfirm].pos.y = g_Button[UnitPowerUp].pos.y + BUTTON_SIZE + 16.0f;
	g_Button[UnitConfirm].pos.x = 8.0f + BUTTON_SIZE * 1.5f;
	
	g_Button[GameStart].pos.y = SCREEN_HEIGHT - 40.0f - BUTTON_SIZE * 0.5f;
	g_Button[GameStart].pos.x = SCREEN_WIDTH - 16.0f - BUTTON_SIZE * 1.5f;

	cursol = 0;
	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitReserve(void)
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
void UpdateReserve(void)
{
	switch (g_Reserve.mode)
	{
	case UnitPowerUp:
		UnitPowerUpMode();
		break;
	case UnitConfirm:
		break;
	case 99:
		NormalRserveMode();
		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawReserve(void)
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
	
	NormalRserveModeDraw();	//全体共通部分を描画(共通部はここに入れる。レイヤーに気を付ける事)

	const XMFLOAT4 color = { 0.0f, 0.0f, 0.0f, 0.75f };
	switch (g_Reserve.mode)
	{
	case UnitPowerUp:
		DrawButton(color, SCREEN_WIDTH * 0.55f, SCREEN_HEIGHT * 0.475f, SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT* 0.70f);
		DrawReserveChar();
		break;
	case UnitConfirm:
		DrawButton(color, SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.25f, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT* 0.5f);
		DrawReserveChar();
		break;
	case 99:
		break;
	}

	SetDepthEnable(TRUE);

	// ライティングを無効
	SetLightEnable(TRUE);

}

void DrawButton(XMFLOAT4 color, float px, float py, float sx, float sy)
{
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[Button_bg]);

	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, px, py, sx, sy, 0.0f, 0.0f, 1.0f, 1.0f,
		color);

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

}
//引数:表示したい数字、表示座標(x,y)、表示サイズ(x方向,y方向)
void DrawNumberRe(int numb, float px, float py, float sx, float sy, XMFLOAT4 color)
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
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[re_number]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, px, py, sx, sy, tx, 0.0f, 0.1f, 1.0f,
			color);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
		numb /= 10;
	}
}

void DrawTextReserve(int k, float px, float py, float sx, float sy, XMFLOAT4 color)	//引数に描画したいやつを入れる。
{
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[k]);


	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, px, py, sx, sy, 0.0f, 0.0f, 1.0f, 1.0f,color);

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

}

void NormalRserveMode(void)
{
	if (cursol < GameStart && GetKeyboardTrigger(DIK_DOWN))
		cursol++;
	if (cursol > 0 && GetKeyboardTrigger(DIK_UP))
		cursol--;

	//選択されているボタンを強調表示に
	for (int i = 0; i < BUTTON_MAX; i++)
	{
		if (i == cursol)
			g_Button[i].color = { 1.5f, 1.5f, 1.5f, 1.0f };
		else
			g_Button[i].color = { 0.5f, 0.5f, 0.5f, 1.0f };
	}

	//決定ボタンを押したらそれに応じて画面遷移
	if (GetKeyboardTrigger(DIK_RETURN)) {
		switch (cursol)
		{
		case UnitPowerUp:
			PowerUpButton();
			break;
		case UnitConfirm:
			ConfirmButton();
			break;
		case GameStart:
			GameStartButton();
			break;
		}
	}
}

void NormalRserveModeDraw(void)
{
	//背景描画
// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[Reserve_bg]);

	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X, SCREEN_CENTER_Y, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f,
		XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);


	//日数描画
	float px = 16.0f + BUTTON_SIZE * 0.25f;
	float py = BUTTON_SIZE * 0.5f + 16.0f;
	DrawNumberRe(g_Reserve.day, px, py, BUTTON_SIZE * 0.5f, BUTTON_SIZE, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	DrawTextReserve(TEXT_DAY, px + BUTTON_SIZE * 1.5f, py, BUTTON_SIZE * 2.0f, BUTTON_SIZE, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));


	////ボタン描画(上に設置してあるやつから順に描画)
	DrawButton(g_Button[UnitPowerUp].color, g_Button[UnitPowerUp].pos.x, g_Button[UnitPowerUp].pos.y, BUTTON_SIZE * 2.5f, BUTTON_SIZE);
	DrawTextReserve(TEXT_POWERUP, g_Button[UnitPowerUp].pos.x, g_Button[UnitPowerUp].pos.y, BUTTON_SIZE * 2.0f, BUTTON_SIZE,
		XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });

	DrawButton(g_Button[UnitConfirm].color, g_Button[UnitConfirm].pos.x, g_Button[UnitConfirm].pos.y, BUTTON_SIZE * 2.5f, BUTTON_SIZE);
	DrawTextReserve(TEXT_CONFIRM, g_Button[UnitConfirm].pos.x, g_Button[UnitConfirm].pos.y, BUTTON_SIZE * 2.0f, BUTTON_SIZE,
		XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });

	DrawButton(g_Button[GameStart].color, g_Button[GameStart].pos.x, g_Button[GameStart].pos.y, BUTTON_SIZE * 2.5f, BUTTON_SIZE);
	DrawTextReserve(TEXT_START, g_Button[GameStart].pos.x, g_Button[GameStart].pos.y, BUTTON_SIZE * 2.0f, BUTTON_SIZE,
		XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });
}

void UnitPowerUpMode(void)
{
	if (GetKeyboardTrigger(DIK_LEFT) && g_Reserve.selectPw > 0)
	{
		g_Reserve.selectPw--;
	}
	else if (GetKeyboardTrigger(DIK_RIGHT) && g_Reserve.selectPw < GetMemberNum() - 1)
	{
		g_Reserve.selectPw++;
	}

	if (GetKeyboardTrigger(DIK_UP) && g_Reserve.selectPw >4)
	{
		g_Reserve.selectPw -= ROW_NUM;
	}
	else if (GetKeyboardTrigger(DIK_DOWN) && g_Reserve.selectPw < GetMemberNum() - 1)
	{
		if (g_Reserve.selectPw + ROW_NUM > GetMemberNum() - 1)
			g_Reserve.selectPw = GetMemberNum() - 1;
		else
			g_Reserve.selectPw += ROW_NUM;
	}
	if (GetKeyboardTrigger(DIK_RETURN))
	{

	}

	if (GetKeyboardTrigger(DIK_C))
	{
		g_Reserve.mode = 99;
	}
}

void PowerUpButton(void)
{
	g_Reserve.mode = UnitPowerUp;
}

void ConfirmButton(void)
{
	g_Reserve.mode = UnitConfirm;
}

void GameStartButton(void)
{
	SetFade(FADE_OUT, MODE_GAME);
}

int GetDay(void)
{
	return g_Reserve.day;
}

//=============================================================================
// キャラボックス描画処理
//=============================================================================
void DrawReserveChar(void)
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
		XMFLOAT2 pos = { (SCREEN_WIDTH * 0.25f) + (k %ROW_NUM) * boxsize + buffx, (SCREEN_HEIGHT *0.2f) + boxsize * (0.5f + (float)(k/ ROW_NUM)) + buffy};
		//キャラIDを抽出してキャラクターを最初に描画
		int id = member[i].charID;
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_CharTexture[id]);
		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, pos.x, pos.y, boxsize, boxsize, 0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);


		//キャラの上にボックスを描画
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[re_box]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, pos.x, pos.y, boxsize, boxsize, 0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

		//カーソルが合っているならそれっぽいボックスを重ねて描画
		if (k == g_Reserve.selectPw) {
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[Button_bg]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, pos.x, pos.y, boxsize, boxsize, 0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
		k++;

	}
}
