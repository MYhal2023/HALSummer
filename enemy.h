//=============================================================================
//
// モデル処理 [enemy.h]
// Author : 
//
//=============================================================================
#pragma once
#include "model.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_ENEMY		"data/MODEL/slime.obj"			// 読み込むモデル名
#define MAX_ENEMY		(1)					// プレイヤーの数

#define	ENEMY_SIZE		(5.0f)				// 当たり判定の大きさ


//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct ENEMY
{
	XMFLOAT3			pos;		// ポリゴンの位置
	XMFLOAT3			rot;		// ポリゴンの向き(回転)
	XMFLOAT3			scl;		// ポリゴンの大きさ(スケール)

	XMFLOAT4X4			mtxWorld;	// ワールドマトリックス

	BOOL				load;
	DX11_MODEL			model;		// モデル情報
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// モデルの色

	float				dir;		// 向き
	float				size;		// 当たり判定の大きさ
	BOOL				use;

	// 階層アニメーション用のメンバー変数
	INTERPOLATION_DATA	*tbl_adr;	// アニメデータのテーブル先頭アドレス
	int					tbl_size;	// 登録したテーブルのレコード総数
	float				move_time;	// 実行時間

	// 親は、NULL、子供は親のアドレスを入れる
	ENEMY				*parent;	// 自分が親ならNULL、自分が子供なら親のenemyアドレス

	//攻撃
	BOOL			attack;			//攻撃中か否か
	BOOL			attackUse;		//この攻撃判定が使用されたか

	int				life;			//プレイヤーHP
	int				lifeMax;		//プレイヤー最大HP
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

ENEMY *GetEnemy(void);