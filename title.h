//=============================================================================
//
// É^ÉCÉgÉãèàóù [title.h]
// Author : 
//
//=============================================================================
#pragma once
struct Title
{
	XMFLOAT4 color;
	XMFLOAT2 pos;
	XMFLOAT2 size;
	int textNo;

};


enum TitleTexture
{
	title_bg,
	title_allies,
	title_enemy,
	newgame,
	loadgame,
	title_logo,
};

HRESULT InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);
void FadeTexture(int type, Title *title);
void MoveTexture(float speed, Title *title);