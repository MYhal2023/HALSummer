//=============================================================================
//
// 弾発射処理 [bullet.h]
// Author : 
//
//=============================================================================
#pragma once
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_BULLET		(128)	// 弾最大数

#define	BULLET_WH		(5.0f)	// 当たり判定の大きさ

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	XMFLOAT4X4	mtxWorld;		// ワールドマトリックス
	XMFLOAT3	pos;			// 位置
	XMFLOAT3	rot;			// 角度
	XMFLOAT3	scl;			// スケール
	MATERIAL	material;		// マテリアル
	float		spd;			// 移動量
	float		fWidth;			// 幅
	float		fHeight;		// 高さ
	int			shadowIdx;		// 影ID
	int			life;
	BOOL		use;			// 使用しているかどうか
	ENEMY		*enemy;			//攻撃対象

} BULLET;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

int SetBullet(XMFLOAT3 pos, XMFLOAT3 rot, float scl, ENEMY *enemy);

BULLET *GetBullet(void);

