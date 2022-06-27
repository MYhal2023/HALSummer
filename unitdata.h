#pragma once
#include "main.h"
#include "model.h"
class Unit
{
public:
	XMFLOAT3			pos;		// ポリゴンの位置
	XMFLOAT3			rot;		// ポリゴンの向き(回転)
	XMFLOAT3			scl;		// ポリゴンの大きさ(スケール)

	XMFLOAT4X4			mtxWorld;	// ワールドマトリックス

	BOOL				load;
	DX11_MODEL			model;		// モデル情報
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// モデルの色

	float				dir;		// 向き
	float				size;		// 攻撃範囲の大きさ
	BOOL				use;

	// 階層アニメーション用のメンバー変数
	INTERPOLATION_DATA	*tbl_adr;	// アニメデータのテーブル先頭アドレス

	int					tbl_size;	// 登録したテーブルのレコード総数
	float				move_time;	// 実行時間
	int					partsNum;
	int					startNum;	// パーツの添え字開始番号。これを保存しておけば特定のパーツだけ処理させられる
	int					atCount;
	int					atFrameCount;	//フレームタイミングのカウント変数
	int					atFrame;		//実際にダメージを入れるフレームタイミング

	//攻撃
	BOOL			attack;			//攻撃中か否か
	BOOL			attackUse;		//この攻撃判定が使用されたか

	int				state;
	int				life;			//プレイヤーHP
	int				lifeMax;		//プレイヤー最大HP
	int				power;
	int				diffend;
};