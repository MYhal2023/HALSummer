//=============================================================================
//
// モデル処理 [fieldchip.h]
// Author : 
//
//=============================================================================
#pragma once
#include "model.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_FIELD001		"data/MODEL/shortrange2.obj"			// 読み込むモデル名
#define	MODEL_FIELD002		"data/MODEL/longrange2.obj"			// 読み込むモデル名
#define MAX_CHIP_NUM		(2)					// フィールドチップ種類
#define MAX_CHIP_WIDTH		(8)					// 横のセット数
#define MAX_CHIP_HEIGHT		(4)					// 縦のセット数

#define	CHIP_SIZE		(30.0f)					// チップサイズの大きさ

enum CHIP_VAL
{
	ROW,
	HIGH,
	MAX_VAL
};
//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct MAP_CHIP
{
	XMFLOAT3			pos;		// ポリゴンの位置
	XMFLOAT3			rot;		// ポリゴンの向き(回転)
	XMFLOAT3			scl;		// ポリゴンの大きさ(スケール)

	XMFLOAT4X4			mtxWorld;	// ワールドマトリックス

	BOOL				load;
	DX11_MODEL			model;		// モデル情報
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// モデルの色

	float				size;		// 当たり判定の大きさ
	BOOL				use;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMapChip(void);
void UninitMapChip(void);
void UpdateMapChip(void);
void DrawMapChip(void);

MAP_CHIP *GetMapChip(void);