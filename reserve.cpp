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
#include "ui.h"
#include "text_texture.h"
#include "fade.h"
#include "debugproc.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX			(13)				// テクスチャの数
#define CH_TEXTURE_MAX		(7)				// キャラテクスチャの数
#define IC_TEXTURE_MAX		(9)				// アイコンテクスチャの数
#define CHAR_TEXTURE_MAX	(7)				// キャラテクスチャの数
#define SKILL_TEXTURE_MAX	(7)				// キャラスキルテクスチャの数
#define NUMBER_SIZE			(30.0f)			// x方向のサイズ
#define COST_NUMBER_SIZE	(45.0f)			// x方向のサイズ
#define BUTTON_SIZE			(106.0f)		// ボタンの縦幅サイズ。多分これくらい
#define BUTTON_MAX			(4)				// ユーサーが選択できるボタン数
#define ROW_NUM				(5)				// 一列に並べるユニット数
#define HELP_TEX_NUM		(1)				// 一列に並べるユニット数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// 頂点情報
static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
static ID3D11ShaderResourceView		*g_CharTexture[CHAR_TEXTURE_MAX] = { NULL };	// テクスチャ情報
static ID3D11ShaderResourceView		*g_IconTexture[IC_TEXTURE_MAX] = { NULL };	// テクスチャ情報
static ID3D11ShaderResourceView		*g_SkillTexture[SKILL_TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char* g_TextureName[TEXTURE_MAX] = {
	"data/TEXTURE/var.png",
	"data/TEXTURE/button.png",
	"data/TEXTURE/title_bg.png",
	"data/TEXTURE/number.png",
	"data/TEXTURE/costbox.png",
	"data/TEXTURE/arrow_right.png",
	"data/TEXTURE/t_confirm.png",
	"data/TEXTURE/t_day.png",
	"data/TEXTURE/t_powerup.png",
	"data/TEXTURE/t_start.png",
	"data/TEXTURE/t_levelup.png",
	"data/TEXTURE/t_cancel.png",
	"data/TEXTURE/var.png",
};
static char* g_CharTextureName[CH_TEXTURE_MAX] = {
	"data/TEXTURE/neutro.png",
	"data/TEXTURE/neutro.png",
	"data/TEXTURE/macro.png",
	"data/TEXTURE/thelper.png",
	"data/TEXTURE/tkiller.png",
	"data/TEXTURE/NK.png",
	"data/TEXTURE/kouen.png",

};
static char* g_IconTextureName[IC_TEXTURE_MAX] = {
	"data/TEXTURE/icon_cost.png",
	"data/TEXTURE/icon_life.png",
	"data/TEXTURE/icon_attack.png",
	"data/TEXTURE/icon_diffend.png",
	"data/TEXTURE/icon_block.png",
	"data/TEXTURE/icon_sp.png",
	"data/TEXTURE/icon_energy.png",
	"data/TEXTURE/icon_oxygen.png",
	"data/TEXTURE/icon_iron.png",
};
static char* g_SkillTextureName[SKILL_TEXTURE_MAX] = {
	"data/TEXTURE/var.png",
	"data/TEXTURE/var.png",
	"data/TEXTURE/var.png",
	"data/TEXTURE/var.png",
	"data/TEXTURE/var.png",
	"data/TEXTURE/var.png",
	"data/TEXTURE/var.png",

};

static Reserve g_Reserve;
static TEXT_TEXTURE g_text[TEXTURE_MAX];
static Button g_Button[BUTTON_MAX];
static Button g_PwButton[2];	//キャンセルとOKボタン
static int cursol;
static int cursolPw;	//パワーアップ画面に使われるカーソル
static int HelpTexNum = 0;	//ヘルプの画像枚数
static float cursolAlpha;	//カーソル透明度
static float alphaSpeed;	//カーソル透明度加算量
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
		g_Reserve.iron = 0;
		g_Reserve.mode = 99;
		g_Reserve.selectPw = 0;
		g_Reserve.pwMode = FALSE;
		restart = TRUE;
#ifdef _DEBUG
		g_Reserve.energy = 9999;
		g_Reserve.oxygen = 999;
		g_Reserve.iron = 9;
#endif
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
	for (int i = 0; i < IC_TEXTURE_MAX; i++)
	{
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_IconTextureName[i],
			NULL,
			NULL,
			&g_IconTexture[i],
			NULL);
	}
	for (int i = 0; i < SKILL_TEXTURE_MAX; i++)
	{
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_SkillTextureName[i],
			NULL,
			NULL,
			&g_SkillTexture[i],
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

	g_Button[ReserveHelp].pos.y = g_Button[UnitConfirm].pos.y + BUTTON_SIZE + 16.0f;
	g_Button[ReserveHelp].pos.x = 8.0f + BUTTON_SIZE * 1.5f;

	g_Button[GameStart].pos.y = SCREEN_HEIGHT - 40.0f - BUTTON_SIZE * 0.5f;
	g_Button[GameStart].pos.x = SCREEN_WIDTH - 16.0f - BUTTON_SIZE * 1.5f;

	g_PwButton[CanselButton].pos.y = SCREEN_HEIGHT * 0.6f;
	g_PwButton[CanselButton].pos.x = SCREEN_WIDTH * 0.8f;
	g_PwButton[CanselButton].color = { 0.2f, 0.2f, 0.2f, 1.0f };

	g_PwButton[LevelupButton].pos.y = SCREEN_HEIGHT * 0.75f;
	g_PwButton[LevelupButton].pos.x = SCREEN_WIDTH * 0.8f;
	g_PwButton[LevelupButton].color = { 0.0f, 0.5f, 1.0f, 1.0f };
	cursol = 0;
	cursolPw = 0;
	cursolAlpha = 0.5f;
	alphaSpeed = 0.02f;
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
	// テクスチャの解放
	for (int i = 0; i < CH_TEXTURE_MAX; i++)
	{
		if (g_CharTexture[i])
		{
			g_CharTexture[i]->Release();
			g_CharTexture[i] = NULL;
		}
	}
	// テクスチャの解放
	for (int i = 0; i < IC_TEXTURE_MAX; i++)
	{
		if (g_IconTexture[i])
		{
			g_IconTexture[i]->Release();
			g_IconTexture[i] = NULL;
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
		UnitConfirmMode();
		break;
	case ReserveHelp:
		ReserveHelpMode();
		break;
	case 99:
		NormalRserveMode();
		break;
	}
}

//=============================================================================
// 描画処理 ここに全部持ってくるので頂点バッファ設定等は個々でいらない
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
	const XMFLOAT2 pos = { SCREEN_WIDTH * 0.55f, SCREEN_HEIGHT * 0.475f };
	XMFLOAT2 pos2 = { SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 0.25f };
	PlayerStatus *member = GetTeam();
	//必要素材関連の変数設定
	const int nowLevel = member[g_Reserve.selectPw].level;
	const float size = 100.0f;
	const float numsize = 60.0f;
	XMFLOAT4 matColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	int number = 0;
	switch (g_Reserve.mode)
	{
	case UnitPowerUp:
		DrawButton(color, SCREEN_WIDTH * 0.55f, SCREEN_HEIGHT * 0.475f, SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT* 0.70f);
		DrawReserveChar();
		if (g_Reserve.pwMode && member[g_Reserve.selectPw].level < MAX_LEVEL) {//強化モードになったら、画面の上に色々描画する
			DrawCharStatus(pos, g_Reserve.selectPw);	//ステータス描画
			for (int k = 0; k < MAX_MATERIAL; k++) {
				if (member[g_Reserve.selectPw].material[k].no == 99 || 
					member[g_Reserve.selectPw].material[k].value[nowLevel - 1] == 0)continue;	//設定されてないならスルー
				pos2.x += (size * 1.5f) * number;	//座標更新して横続きに描画
				DrawNeedMaterial(pos2, size, member[g_Reserve.selectPw].material[k].no);//先に画像描画してから必要素材数を描画
				DrawNumberRe(member[g_Reserve.selectPw].material[k].value[nowLevel - 1], pos2.x + size * 0.25f, pos2.y + size*0.75f, numsize * 0.5f, numsize, matColor);
				number++;
			}
			//ボタン描画
			DrawButton(g_PwButton[CanselButton].color, g_PwButton[CanselButton].pos.x, g_PwButton[CanselButton].pos.y, BUTTON_SIZE * 2.5f, BUTTON_SIZE);
			DrawTextReserve(TEXT_CANCEL, g_PwButton[CanselButton].pos.x, g_PwButton[CanselButton].pos.y, BUTTON_SIZE * 2.0f, BUTTON_SIZE,
				XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });
			//ボタン描画
			DrawButton(g_PwButton[LevelupButton].color, g_PwButton[LevelupButton].pos.x, g_PwButton[LevelupButton].pos.y, BUTTON_SIZE * 2.5f, BUTTON_SIZE);
			DrawTextReserve(TEXT_LUP, g_PwButton[LevelupButton].pos.x, g_PwButton[LevelupButton].pos.y, BUTTON_SIZE * 2.0f, BUTTON_SIZE,
				XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });
			//カーソルに対応して上から透明なボックスを描画
			if ((cursolAlpha > 0.8f && alphaSpeed > 0.0f) || (cursolAlpha < 0.4f && alphaSpeed < 0.0f))alphaSpeed *= -1;
			cursolAlpha += alphaSpeed;
			switch (cursolPw) {
			case 0:
				DrawButton(XMFLOAT4{1.0f, 1.0f, 1.0f, cursolAlpha }, g_PwButton[CanselButton].pos.x, g_PwButton[CanselButton].pos.y, BUTTON_SIZE * 2.5f, BUTTON_SIZE);
				break;
			case 1:
				DrawButton(XMFLOAT4{ 1.0f, 1.0f, 1.0f, cursolAlpha }, g_PwButton[LevelupButton].pos.x, g_PwButton[LevelupButton].pos.y, BUTTON_SIZE * 2.5f, BUTTON_SIZE);
				break;
			}
		}
		else if (g_Reserve.pwMode && member[g_Reserve.selectPw].level >= MAX_LEVEL)
		{
			DrawMaxLevelChar(pos, g_Reserve.selectPw);
			//ボタン描画
			DrawButton(g_PwButton[CanselButton].color, g_PwButton[CanselButton].pos.x, g_PwButton[CanselButton].pos.y, BUTTON_SIZE * 2.5f, BUTTON_SIZE);
			DrawTextReserve(TEXT_CANCEL, g_PwButton[CanselButton].pos.x, g_PwButton[CanselButton].pos.y, BUTTON_SIZE * 2.0f, BUTTON_SIZE,
				XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });
			//カーソルに対応して上から透明なボックスを描画
			if ((cursolAlpha > 0.8f && alphaSpeed > 0.0f) || (cursolAlpha < 0.4f && alphaSpeed < 0.0f))alphaSpeed *= -1;
			cursolAlpha += alphaSpeed;
			switch (cursolPw) {
			case 0:
				DrawButton(XMFLOAT4{ 1.0f, 1.0f, 1.0f, cursolAlpha }, g_PwButton[CanselButton].pos.x, g_PwButton[CanselButton].pos.y, BUTTON_SIZE * 2.5f, BUTTON_SIZE);
				break;
			case 1:
				DrawButton(XMFLOAT4{ 1.0f, 1.0f, 1.0f, cursolAlpha }, g_PwButton[LevelupButton].pos.x, g_PwButton[LevelupButton].pos.y, BUTTON_SIZE * 2.5f, BUTTON_SIZE);
				break;
			}
		}
		break;
	case UnitConfirm:
		DrawButton(color, SCREEN_WIDTH * 0.55f, SCREEN_HEIGHT * 0.475f, SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT* 0.70f);
		break;
	case ReserveHelp:
		DrawReserveHelp();
		break;
	case 99:
		break;
	}

	SetDepthEnable(TRUE);

	// ライティングを無効
	SetLightEnable(TRUE);

}

Reserve *GetReserve(void) { return &g_Reserve; };

void DrawButton(XMFLOAT4 color, float px, float py, float sx, float sy)
{
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[Button_tx]);

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

	float psx = px + sx * digit;
	for (int i = 0; i < digit; i++)
	{
		psx -= sx;
		float x = (float)(numb % 10);		//今回表示する数字
		float tx = x * 0.1f;			// テクスチャの左上X座標

		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[re_number]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, psx, py, sx, sy, tx, 0.0f, 0.1f, 1.0f,
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
	if (cursol < GameStart && GetKeyboardTrigger(DIK_DOWN)) {
		cursol++;
		PlaySound(SOUND_LABEL_SE_Select);
	}
	if (cursol > 0 && GetKeyboardTrigger(DIK_UP)) {
		cursol--;
		PlaySound(SOUND_LABEL_SE_Select);
	}
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
		PlaySound(SOUND_LABEL_SE_Decision);
		switch (cursol)
		{
		case UnitPowerUp:
			PowerUpButton();
			break;
		case UnitConfirm:
			ConfirmButton();
			break;
		case ReserveHelp:
			ReserveHelpButton();
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

	XMFLOAT2 size = { SCREEN_WIDTH, 130.0f };
	//保有素材量の描画
	DrawButton(XMFLOAT4{ 0.0f, 0.0f, 0.0f, 0.7f }, SCREEN_CENTER_X, size.y * 0.5f, size.x, size.y);
	XMFLOAT2 pos = { SCREEN_WIDTH * 0.45f, size.y * 0.5f };
	//エネルギー量
	DrawNeedMaterial(pos, size.y * 0.8f, energy);
	pos.x += size.y * 0.7f;
	DrawNumberRe(g_Reserve.energy, pos.x, pos.y, size.y * 0.25f, size.y * 0.5f, XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });
	//酸素量
	pos.x += size.y * 1.8f;
	DrawNeedMaterial(pos, size.y * 0.8f, oxygen);
	pos.x += size.y * 0.7f;
	DrawNumberRe(g_Reserve.oxygen, pos.x, pos.y, size.y * 0.25f, size.y * 0.5f, XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });
	//鉄分
	pos.x += size.y * 1.8f;
	DrawNeedMaterial(pos, size.y * 0.8f, iron);
	pos.x += size.y * 0.7f;
	DrawNumberRe(g_Reserve.iron, pos.x, pos.y, size.y * 0.25f, size.y * 0.5f, XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });

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
	DrawButton(g_Button[ReserveHelp].color, g_Button[ReserveHelp].pos.x, g_Button[ReserveHelp].pos.y, BUTTON_SIZE * 2.5f, BUTTON_SIZE);
	DrawTextReserve(TEXT_CONFIRM, g_Button[ReserveHelp].pos.x, g_Button[ReserveHelp].pos.y, BUTTON_SIZE * 2.0f, BUTTON_SIZE,
		XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });

	DrawButton(g_Button[GameStart].color, g_Button[GameStart].pos.x, g_Button[GameStart].pos.y, BUTTON_SIZE * 2.5f, BUTTON_SIZE);
	DrawTextReserve(TEXT_START, g_Button[GameStart].pos.x, g_Button[GameStart].pos.y, BUTTON_SIZE * 2.0f, BUTTON_SIZE,
		XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });

}

void UnitPowerUpMode(void)
{
	PlayerStatus *member = GetTeam();
	if (member[g_Reserve.selectPw].level >= MAX_LEVEL && g_Reserve.pwMode)
	{
		cursolPw = 0;
		if (GetKeyboardTrigger(DIK_C) || GetKeyboardTrigger(DIK_RETURN))
		{
			g_Reserve.pwMode = FALSE;
			cursolPw = 0;
			PlaySound(SOUND_LABEL_SE_Cancel);
		}
	}
	else {
		//まず強化モードの操作を優先して処理
		if (GetKeyboardTrigger(DIK_UP) && g_Reserve.pwMode && cursolPw > 0) {
			cursolPw--;
			PlaySound(SOUND_LABEL_SE_Select);
		}
		else if (GetKeyboardTrigger(DIK_DOWN) && g_Reserve.pwMode && cursolPw <= 0) {
			cursolPw++;
			PlaySound(SOUND_LABEL_SE_Select);
		}
		if ((GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_Z)) && !g_Reserve.pwMode)
		{
			g_Reserve.pwMode = TRUE;
			PlaySound(SOUND_LABEL_SE_Decision);
		}
		else if (GetKeyboardTrigger(DIK_C) && !g_Reserve.pwMode)
		{
			g_Reserve.mode = 99;
			PlaySound(SOUND_LABEL_SE_Cancel);
		}
		else if (GetKeyboardTrigger(DIK_C))
		{
			g_Reserve.pwMode = FALSE;
			cursolPw = 0;
			PlaySound(SOUND_LABEL_SE_Cancel);
		}
		else if ((GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_Z)) && g_Reserve.pwMode)
		{
			switch (cursolPw) {
			case 0:
				g_Reserve.pwMode = FALSE;
				PlaySound(SOUND_LABEL_SE_Cancel);
				break;
			case 1:
				PlayerStatus *member = GetTeam();
				if (CheckPowerUpMaterial(&member[g_Reserve.selectPw]))	//セレクトしているキャラはレベルアップ可能か？
				{
					ReduceMaterial(&member[g_Reserve.selectPw]);	//素材量を減らす
					member[g_Reserve.selectPw].level++;	//レベルアップ！
					PlaySound(SOUND_LABEL_SE_PowerUp);
				}
				break;
			}
		}

		if (g_Reserve.pwMode)return;//強化中なら他操作を受け付けない

		if (GetKeyboardTrigger(DIK_LEFT) && g_Reserve.selectPw > 0)
		{
			g_Reserve.selectPw--;
			PlaySound(SOUND_LABEL_SE_Select);
		}
		else if (GetKeyboardTrigger(DIK_RIGHT) && g_Reserve.selectPw < GetMemberNum() - 1)
		{
			g_Reserve.selectPw++;
			PlaySound(SOUND_LABEL_SE_Select);
		}

		else if (GetKeyboardTrigger(DIK_UP) && g_Reserve.selectPw > 4)
		{
			g_Reserve.selectPw -= ROW_NUM;
			PlaySound(SOUND_LABEL_SE_Select);
		}
		else if (GetKeyboardTrigger(DIK_DOWN) && g_Reserve.selectPw < GetMemberNum() - 1)
		{
			if (g_Reserve.selectPw + ROW_NUM > GetMemberNum() - 1)
				g_Reserve.selectPw = GetMemberNum() - 1;
			else
				g_Reserve.selectPw += ROW_NUM;
			PlaySound(SOUND_LABEL_SE_Select);
		}
	}
}

void UnitConfirmMode(void)
{
	if (GetKeyboardTrigger(DIK_LEFT) && g_Reserve.selectPw > 0)
	{
		g_Reserve.selectPw--;
		PlaySound(SOUND_LABEL_SE_Select);
	}
	else if (GetKeyboardTrigger(DIK_RIGHT) && g_Reserve.selectPw < GetMemberNum() - 1)
	{
		g_Reserve.selectPw++;
		PlaySound(SOUND_LABEL_SE_Select);
	}

	if (GetKeyboardTrigger(DIK_UP) && g_Reserve.selectPw > 4)
	{
		g_Reserve.selectPw -= ROW_NUM;
		PlaySound(SOUND_LABEL_SE_Select);
	}
	else if (GetKeyboardTrigger(DIK_DOWN) && g_Reserve.selectPw < GetMemberNum() - 1)
	{
		if (g_Reserve.selectPw + ROW_NUM > GetMemberNum() - 1)
			g_Reserve.selectPw = GetMemberNum() - 1;
		else
			g_Reserve.selectPw += ROW_NUM;
		PlaySound(SOUND_LABEL_SE_Select);
	}
	if (GetKeyboardTrigger(DIK_RETURN))
	{

	}

	if (GetKeyboardTrigger(DIK_C))
	{
		g_Reserve.mode = 99;
		PlaySound(SOUND_LABEL_SE_Cancel);
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

void ReserveHelpButton(void)
{
	g_Reserve.mode = ReserveHelp;
}

void GameStartButton(void)
{
	SetFade(FADE_OUT, MODE_GAME, battleFade);
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

//引数：表示の中心位置とキャラステータスポインター
void DrawCharStatus(XMFLOAT2 pos,int k)
{
	PlayerStatus *member = GetTeam();
	//下地の枠を描画
	const float sizeX = SCREEN_WIDTH * 0.75f;
	const float sizeY = SCREEN_HEIGHT * 0.70f;
	XMFLOAT4 color = { 0.4f, 0.4f, 1.0f, 1.0f };
	DrawButton(color, pos.x, pos.y, sizeX, sizeY);

	//キャラ画像描画
	const float boxsize = 180.0f;	//ボックスサイズ定義
	const int id = member[k].charID;
	float posX = pos.x - sizeX * 0.36f;
	float posY = pos.y - sizeY * 0.375f;
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_CharTexture[id]);
	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, posX, posY, boxsize, boxsize, 0.0f, 0.0f, 1.0f, 1.0f,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);
	posX -= boxsize * 0.35f;
	XMFLOAT2 set = { posX, posY + boxsize * 0.75f };
	DrawCharAllStatus(set, k);

	const float arrow = 160.0f * 0.25f;
	for (int i = 0; i < 6; i++) {
		for (int k = 0; k < 3; k++) {
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[arRight]);
			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, posX + boxsize * 1.25f + k * arrow * 0.75f, set.y + i * arrow + i * 52.0f, arrow, arrow, 0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}
	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);
	posX += sizeX * 0.30f;
	set = { posX , posY + boxsize * 0.75f };
	member[k].level++;
	DrawCharAllStatus(set, k);
	member[k].level--;
}

void DrawCharAllStatus(XMFLOAT2 pos, int k)
{
	PlayerStatus *member = GetTeam();
	//コスト描画
	float posX = pos.x;
	float posY = pos.y;
	const float iconSize = 75.0f;	//ボックスサイズ定義
	const float iconBuff = 16.0f;
	float buffX = iconSize + iconBuff;
	int level = member[k].level - 1;
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_IconTexture[icCost]);
	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, posX, posY, iconSize, iconSize, 0.0f, 0.0f, 1.0f, 1.0f,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	float isx = iconSize - 8.0f;
	posX = pos.x + isx;
	posY = pos.y + buffX * 0.0f;
	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);
	DrawNumberRe(member[k].cost[level], posX, posY, iconSize * 0.5f, iconSize, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));


	//ライフ描画
	posX = pos.x;
	posY = pos.y + buffX * 1.0f;
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_IconTexture[icLife]);
	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, posX, posY, iconSize, iconSize, 0.0f, 0.0f, 1.0f, 1.0f,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	posX = pos.x + isx;
	posY = pos.y + buffX * 1.0f;
	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);
	DrawNumberRe(member[k].lifeMax[level], posX, posY, iconSize * 0.5f, iconSize, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));


	//攻撃力描画
	posX = pos.x;
	posY = pos.y + buffX * 2.0f;
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_IconTexture[icAttack]);
	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, posX, posY, iconSize, iconSize, 0.0f, 0.0f, 1.0f, 1.0f,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	posX = pos.x + isx;
	posY = pos.y + buffX * 2.0f;
	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);
	DrawNumberRe(member[k].power[level], posX, posY, iconSize * 0.5f, iconSize, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));


	//防御力描画
	posX = pos.x;
	posY = pos.y + buffX * 3.0f;
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_IconTexture[icDiffend]);
	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, posX, posY, iconSize, iconSize, 0.0f, 0.0f, 1.0f, 1.0f,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	posX = pos.x + isx;
	posY = pos.y + buffX * 3.0f;
	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);
	DrawNumberRe(member[k].diffend[level], posX, posY, iconSize * 0.5f, iconSize, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));


	//ブロック数描画
	posX = pos.x;
	posY = pos.y + buffX * 4.0f;
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_IconTexture[icBlock]);
	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, posX, posY, iconSize, iconSize, 0.0f, 0.0f, 1.0f, 1.0f,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	posX = pos.x + isx;
	posY = pos.y + buffX * 4.0f;
	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);
	DrawNumberRe(member[k].blockMax, posX, posY, iconSize * 0.5f, iconSize, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	//SP描画
	posX = pos.x;
	posY = pos.y + buffX * 5.0f;
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_IconTexture[icSp]);
	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, posX, posY, iconSize, iconSize, 0.0f, 0.0f, 1.0f, 1.0f,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	posX = pos.x + isx;
	posY = pos.y + buffX * 5.0f;
	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);
	DrawNumberRe(member[k].spMax[level], posX, posY, iconSize * 0.5f, iconSize, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

}
//needって書いてあるけど、普通に使いまわし可能
//引数に座標と描画する素材IDを持ってくる
void DrawNeedMaterial(XMFLOAT2 pos, float size, int no)
{
	XMFLOAT2 texSize = { size, size };
	//とりあえず下地描画
	XMFLOAT4 color = { 0.0f, 0.0f, 0.0f, 1.0f };
	DrawButton(color, pos.x, pos.y, texSize.x, texSize.y);

	//テクスチャ画像の設定
	int texNo = 0;
	switch (no)
	{
	case energy:
		texNo = icEnergy;
		break;
	case oxygen:
		texNo = icOxygen;
		break;
	case iron:
		texNo = icIron;
		break;
	}
	//アイコン描画
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_IconTexture[texNo]);
	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, pos.x, pos.y, texSize.x * 0.9f, texSize.y * 0.9f, 0.0f, 0.0f, 1.0f, 1.0f,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

}
//引数には強化対象のアドレスをもってくる
BOOL CheckPowerUpMaterial(PlayerStatus *member)
{
	int level = member->level;
	for (int i = 0; i < MAX_MATERIAL; i++) {
		//必要量が所持量を上回っていたらFALEを返す
		switch(member->material[i].no){
		case energy:
			if (member->material[i].value[level - 1] > g_Reserve.energy)return FALSE;
			break;
		case oxygen:
			if (member->material[i].value[level - 1] > g_Reserve.oxygen)return FALSE;
			break;
		case iron:
			if (member->material[i].value[level - 1] > g_Reserve.iron)return FALSE;
			break;
		}
	}

	return TRUE;
}

void ReduceMaterial(PlayerStatus *member)
{
	int level = member->level;
	for (int i = 0; i < MAX_MATERIAL; i++) {
		//必要量が所持量を上回っていたらFALEを返す
		switch (member->material[i].no) {
		case energy:
			g_Reserve.energy -= member->material[i].value[level - 1];
			break;
		case oxygen:
			g_Reserve.oxygen -= member->material[i].value[level - 1];
			break;
		case iron:
			g_Reserve.iron -= member->material[i].value[level - 1];
			break;
		}
	}
}

void ReserveHelpMode(void)
{
	if (GetKeyboardTrigger(DIK_RIGHT)) {
		HelpTexNum++;
		PlaySound(SOUND_LABEL_SE_Select);
	}
	else if (GetKeyboardTrigger(DIK_LEFT) && HelpTexNum > 0) {
		HelpTexNum--;
		PlaySound(SOUND_LABEL_SE_Select);
	}

	//ヘルプ画像をオーバーしていたらリセット
	if (HelpTexNum >= HELP_TEX_NUM || GetKeyboardTrigger(DIK_C))
	{
		HelpTexNum = 0;
		g_Reserve.mode = 99;
		PlaySound(SOUND_LABEL_SE_Cancel);
	}
}

void DrawReserveHelp(void)
{
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[Help_ReserveTex + HelpTexNum]);
	const XMFLOAT2 pos = { SCREEN_CENTER_X, SCREEN_CENTER_Y };
	const XMFLOAT2 texSize = { 1600.0f, 900.0f };
	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, pos.x, pos.y, texSize.x, texSize.y, 0.0f, 0.0f, 1.0f, 1.0f,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);
}

void DrawMaxLevelChar(XMFLOAT2 pos, int k)
{
	PlayerStatus *member = GetTeam();
	//下地の枠を描画
	const float sizeX = SCREEN_WIDTH * 0.75f;
	const float sizeY = SCREEN_HEIGHT * 0.70f;
	XMFLOAT4 color = { 0.4f, 0.4f, 1.0f, 1.0f };
	DrawButton(color, pos.x, pos.y, sizeX, sizeY);

	//キャラ画像描画
	const float boxsize = 180.0f;	//ボックスサイズ定義
	const int id = member[k].charID;
	float posX = pos.x - sizeX * 0.36f;
	float posY = pos.y - sizeY * 0.375f;
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_CharTexture[id]);
	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, posX, posY, boxsize, boxsize, 0.0f, 0.0f, 1.0f, 1.0f,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	// ポリゴン描画
	pos.y = 180.0f*0.75f + pos.y - SCREEN_HEIGHT * 0.70f * 0.375f;
	pos.x = pos.x - SCREEN_WIDTH * 0.75f * 0.36f - 180.0f * 0.35f;
	GetDeviceContext()->Draw(4, 0);
	DrawCharAllStatus(pos, k);
}

void DrawCharSkill(XMFLOAT2 pos, int k)
{

}