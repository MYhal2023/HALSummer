//=============================================================================
//
// UIèàóù [ui.h]
// Author : 
//
//=============================================================================
#pragma once

class UI
{
public:
	XMFLOAT4 color;
	XMFLOAT2 pos;
	XMFLOAT2 size;
	BOOL use;
};

enum UI_TEXTURE
{
	charBox,
	var_bg,
	number,
	baseLife,
	enemyNum,
	button_n,
	button_d,
	button_s,
	costbox,
	button_help,
	icon_help,
	help_texture,
	button_bg,
	icon_1,
	icon_2,
	icon_3,
	icon_4,
	icon_5,
	icon_6,
};


HRESULT InitUI(void);
void UninitUI(void);
void UpdateUI(void);
void DrawUI(void);
void DrawCharBox(void);
void DrawUIbg(void);
void DrawNumber(int numb, float px, float py, float sx, float sy, XMFLOAT4 color);
void DrawLife(void);
void DrawEnemyNum(void);
void DrawButtonNorD(void);
void DrawButtonStop(void);
void DrawCost(void);
void DrawHelpButton(void);
void SetHelpButton(BOOL flag); 
BOOL GetHelpButton(void);
void DrawBattleCharStatus(void);
void DrawCharAllStatus(XMFLOAT2 pos);