//=============================================================================
//
// プレイヤーセット処理 [playerSet.h]
// Author : 
//
//=============================================================================
#pragma once
#include "main.h"
#include "model.h"
#include "input.h"
#include "fieldchip.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
enum PLAYER_CHAR
{
	ID_MainPlayer,
	ID_Neutro,
};
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void UninitPlayerSet(void);
void UpdatePlayerSet(void);
void DrawPlayerSet(void);


//*****************************************************************************
// グローバル変数
//*****************************************************************************

//プレイヤーのステータスを保存するもの
class PlayerStatus
{
public:
	DX11_MODEL			model;		// モデル情報
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// モデルの色
	XMFLOAT3			scl;	//セットする先の座標
	float				size;		// 攻撃範囲の大きさ
	// 階層アニメーション用のメンバー変数
	INTERPOLATION_DATA	*tbl_adrA;	// 攻撃アニメデータのテーブル先頭アドレス
	INTERPOLATION_DATA	*tbl_adrM;	// 待機アニメデータのテーブル先頭アドレス

	int					tbl_sizeA;	// 登録したテーブルのレコード総数
	int					tbl_sizeM;	// 登録したテーブルのレコード総数
	float				move_time;	// 実行時間
	int					partsNum;
	int					startNum;	// パーツの添え字開始番号。これを保存しておけば特定のパーツだけ処理させられる
	int					atFrame;		//実際にダメージを入れるフレームタイミング
	int				life;			//プレイヤーHP
	int				lifeMax;		//プレイヤー最大HP
	int				power;
	int				diffend;
	int				blockNum;		//ブロックしている数
	int				blockMax;		//ブロック可能数
	int				charID;			//他のデータに渡すときのキャラ判別用変数
	int				charType;		//近接キャラか遠距離キャラか。CHIP_TYPEと同じ数字を入れる
	BOOL			use;			//そもそもこの枠を使っているのか
};

//プレイヤーに付随するパーツ情報を保存するもの
class PlayerPartsStatus
{
public:
	DX11_MODEL			model;		// モデル情報
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// モデルの色
	// 階層アニメーション用のメンバー変数
	INTERPOLATION_DATA	*tbl_adrA;	// アニメデータのテーブル先頭アドレス
	INTERPOLATION_DATA	*tbl_adrM;	// アニメデータのテーブル先頭アドレス
	int					tbl_sizeA;	// 登録したテーブルのレコード総数
	int					tbl_sizeM;	// 登録したテーブルのレコード総数
	float				move_time;	// 実行時間
};

class PlayerSet
{
public:
	XMFLOAT3 setRot;	//セットする先の座標
	BOOL use[MAX_PLAYER_SET];			//この編成番号は有効か否か
	int setPlayer;	//セットするプレイヤー番号テーブル(添え字？)
	int setCharID;	//セットしようとしてるキャラが何なのか？
	PlayerStatus *setPlayerNum;
	PlayerPartsStatus *setPlayerParts;
	XMFLOAT3 setPos;	//セットする先の座標
	BOOL setMode;		//セットモードに移行したか否か
};
HRESULT InitPlayerSet(void);
void PlayerSetMap(void);
void SetModeChange(int i);
void SetPosition(void);
void SetPlayerInfo(PlayerStatus* member, PlayerPartsStatus* memberParts);
BOOL CheckPlayerAble(PlayerStatus *member);
PlayerSet *GetSetPos(void);