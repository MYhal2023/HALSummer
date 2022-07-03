//=============================================================================
//
// ゲームオーバー処理 [gameover.h]
// Author : 
//
//=============================================================================
#pragma once
#include "main.h"

class Over
{
public:
	XMFLOAT4 color;	//引き渡す色情報
	XMFLOAT2 pos;	//表示座標
	XMFLOAT2 size;	//表示座標
	BOOL use;		//この画像を表示するか
};

enum OVER_TEXT
{
	FadeRed,
};
HRESULT InitOver(void);
void UninitOver(void);
void UpdateOver(void);
void DrawOver(void);