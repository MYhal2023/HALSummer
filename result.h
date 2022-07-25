//=============================================================================
//
// ÉäÉUÉãÉgèàóù [result.h]
// Author : 
//
//=============================================================================
#pragma once
struct Result
{
	XMFLOAT4 color;
	XMFLOAT2 pos;
	XMFLOAT2 size;
	int textNo;
	int type;

};

enum ResultType
{
	Lose,
	Win
};

enum ResultTexture
{
	win,
	lose,
};

HRESULT InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
void WinResult(void);
void LoseResult(void);