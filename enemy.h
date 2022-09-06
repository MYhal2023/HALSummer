//=============================================================================
//
// モデル処理 [enemy.h]
// Author : 
//
//=============================================================================
#pragma once
#ifndef ENEMY_H_
#define ENEMY_H_
#include "unitdata.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_GRAPE		"data/MODEL/grape0.obj"			// 読み込むモデル名
#define	MODEL_GRAPE_PARTS001		"data/MODEL/grape_parts001.obj"			// 読み込むモデル名
#define	MODEL_GRAPE_PARTS002		"data/MODEL/grape_parts002.obj"			// 読み込むモデル名
#define	MODEL_GRAPE_PARTS003		"data/MODEL/grape_parts003.obj"			// 読み込むモデル名
#define	MODEL_STREPT		"data/MODEL/streptococcus_pyogenes.obj"			// 読み込むモデル名
#define	MODEL_STREPT_HEAD		"data/MODEL/streptococcus_pyogenes_head.obj"			// 読み込むモデル名
#define	MODEL_STREPT_NECK		"data/MODEL/streptococcus_pyogenes_neck.obj"			// 読み込むモデル名
#define	MODEL_STREPT_TAIL		"data/MODEL/streptococcus_pyogenes_tail.obj"			// 読み込むモデル名
#define	MODEL_RYOKU				"data/MODEL/ryokunoukin.obj"			// 読み込むモデル名
#define	MODEL_RYOKU_LEFT		"data/MODEL/ryokunoukin_left.obj"			// 読み込むモデル名
#define	MODEL_RYOKU_RIGHT		"data/MODEL/ryokunoukin_right.obj"			// 読み込むモデル名
#define	MODEL_INFLUE		"data/MODEL/influenza.obj"			// 読み込むモデル名

#define MAX_ENEMY		(50)					// エネミーの数

#define	ENEMY_SIZE		(5.0f)				// 当たり判定の大きさ

enum ENEMY_STATE
{
	Move,
	Attack,
};

enum ENEMY_TYPE
{
	Proximity,	//近接攻撃キャラ
	Remote,		//遠距離攻撃キャラ
	Abnormal,	//特殊なキャラ。ユニークボスとかに使う
};

enum ENEMY_ID
{
	Grape,
	Strept,
	Ryoku,
	Infule,
};
//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct MOVERINE
{
	XMFLOAT3 start;
};

struct ENEMY:public Unit
{
public:
	float				move_time;	// 実行時間
	int					moveTblSize;
	int					nowTbl;
	// 親は、NULL、子供は親のアドレスを入れる
	ENEMY				*parent;	// 自分が親ならNULL、自分が子供なら親のenemyアドレス
	MOVERINE			*moveData;
	PLAYER				*target;
	float				start;		//エネミーを出現させるスタート時間。これがゼロ以下になった時、線形補間を開始する
	float				spd;		//エネミーの移動速度
	float				moveCount;	//移動した量
	int					atFrame;		//実際にダメージを入れるフレームタイミング
	int					atFrameCount;		//フレームタイミングのカウント変数
	BOOL				blocked;		//ブロックされているか否か
	int					type;
	int					atInterval;
	int					charID;
};
//プレイヤーの線形補間データを保存
class Enemyliner
{
public:
	XMFLOAT3			pos;		// ポリゴンの位置
	XMFLOAT3			rot;		// ポリゴンの向き(回転)
	XMFLOAT3			scl;		// ポリゴンの大きさ(スケール)
};

struct EnemyParts
{
	XMFLOAT3			pos;		// ポリゴンの位置
	XMFLOAT3			rot;		// ポリゴンの向き(回転)
	XMFLOAT3			scl;		// ポリゴンの大きさ(スケール)

	XMFLOAT4X4			mtxWorld;	// ワールドマトリックス

	BOOL				load;
	DX11_MODEL			model;		// モデル情報
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// モデルの色
	// 階層アニメーション用のメンバー変数
	INTERPOLATION_DATA	*tbl_adrA;	// 攻撃アニメデータのテーブル先頭アドレス
	INTERPOLATION_DATA	*tbl_adrM;	// 移動アニメデータのテーブル先頭アドレス
	int					tbl_sizeA;	// 登録したテーブルのレコード総数
	int					tbl_sizeM;	// 登録したテーブルのレコード総数
	float				move_time;	// 実行時間
	ENEMY				*parent;	// 自分が親ならNULL、自分が子供なら親のenemyアドレス
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

ENEMY *GetEnemy(void);
EnemyParts *GetEnemyParts(void);
Unit *GetEnemyModel(void);
Unit *GetPartsModel(void);
//void SetGrape(float time);
int StateCheck(int i);
void SetEnemyTime(int i);
void EnemyMoveLiner(int i);
void EnemyInterPoration(int i);
void EnemyMove(int i);
void CheckTarget(int i);
void BaseDamage(int i);
int GetEnemyNum(void);
int GetBanishEnemy(void);
int GetEnemyPartsNum(void);
void SetEnemyNum(void);
void SetEnemyPartsNum(void);
void DrawEnemyLife(void);
HRESULT MakeVertexEnemyVar(void);
#endif