//=============================================================================
//
// 文字テクスチャ等の表示処理 [text_texture.h]
// Author : 
//
//=============================================================================
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************
enum TEXT_TEXTURE_NUM
{
	TEXT_GAMEOVER,
	TEXT_WIN_RESULT,
};

struct TEXT_TEXTURE
{
	XMFLOAT3	pos;						// ポリゴンの位置
	XMFLOAT4	color;						//色
	int			TexNo;						//テクスチャ番号
	int			time;						//表示時間のカウント用変数
	int			timeMax;					//表示時間
	float		w, h, tw, th, tx, ty;		// 幅と高さ、テクスチャ座標
	int			type;
	BOOL		use;
};
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTexttex(void);
void UninitTexttex(void);
void UpdateTexttex(void);
void DrawTexttex(void);
void DrawTexttChose(int k);
void SetText(int i, int type, XMFLOAT3 pos);
void SetTextpos(int i, XMFLOAT3 pos);