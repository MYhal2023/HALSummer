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
};

HRESULT InitUI(void);
void UninitUI(void);
void UpdateUI(void);
void DrawUI(void);
void DrawCharBox(void);
void DrawUIbg(void);
void DrawNumber(int numb, float x, float y, float sx, float sy);
void DrawLife(void);
void DrawEnemyNum(void);
void DrawButtonNorD(void);
void DrawButtonStop(void);