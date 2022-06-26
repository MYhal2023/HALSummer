//=============================================================================
//
// モデル処理 [player.h]
// Author : 
//
//=============================================================================
#pragma once
#ifndef PLAYER_H_
#define PLAYER_H_
#include "unitdata.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_PLAYER		"data/MODEL/dendritic_cells.obj"			// 読み込むモデル名
#define	MODEL_NEUTROPHILS	"data/MODEL/neutrophils.obj"			// 読み込むモデル名
#define MAX_PLAYER		(2)					// プレイヤーの数

#define	PLAYER_SIZE		(5.0f)				// 当たり判定の大きさ

enum PLAYER_STATE
{
	Standby,	//待機中
	Diffend,	//迎撃中
};

//*****************************************************************************
// 構造体定義
//*****************************************************************************
class PLAYER:public Unit
{
public:
	// 階層アニメーション用のメンバー変数
	PLAYER				*parent;	// 自分が親ならNULL、自分が子供なら親のenemyアドレス

	int				blockNum;		//ブロックしている数
	int				blockMax;		//ブロック可能数
	int				target;			//ターゲット無しは99で
	void StateCheck(void);
};

struct PlayerParts
{
	XMFLOAT3			pos;		// ポリゴンの位置
	XMFLOAT3			rot;		// ポリゴンの向き(回転)
	XMFLOAT3			scl;		// ポリゴンの大きさ(スケール)

	XMFLOAT4X4			mtxWorld;	// ワールドマトリックス

	BOOL				load;
	DX11_MODEL			model;		// モデル情報
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// モデルの色
	// 階層アニメーション用のメンバー変数
	INTERPOLATION_DATA	*tbl_adr;	// アニメデータのテーブル先頭アドレス
	int					tbl_size;	// 登録したテーブルのレコード総数
	float				move_time;	// 実行時間
	PLAYER				*parent;	// 自分が親ならNULL、自分が子供なら親のenemyアドレス
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

void BlockEnemy(void);
PLAYER *GetPlayer(void);
void PlayerInterPoration(int i);
void CheckEnemyTarget(int i);
void SetPlayer(XMFLOAT3 pos);
void SetNeutrophils(XMFLOAT3 pos);
int GetPlayerNum(void);

#endif