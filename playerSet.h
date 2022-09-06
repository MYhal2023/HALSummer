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
#define MAX_LEVEL (5)
#define MAX_MATERIAL (3)
enum PLAYER_CHAR
{
	ID_MainPlayer,
	ID_Neutro,
	ID_Macro,
	ID_HelperT,
	ID_KillerT,
	ID_NK,
	ID_Kouen,
};

//素材ID
enum Material
{
	energy,
	oxygen,
	iron,
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
//プレイヤーステータス構造体の中に入れる
class NeedMaterial
{
public:
	int no;	//素材ID
	int value[MAX_LEVEL - 1]; //必要量
};

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
	NeedMaterial		*material;	//強化に必要な素材数
	int					tbl_sizeA;	// 登録したテーブルのレコード総数
	int					tbl_sizeM;	// 登録したテーブルのレコード総数
	float				move_time;	// 実行時間
	int					partsNum;
	int					startNum;	// パーツの添え字開始番号。これを保存しておけば特定のパーツだけ処理させられる
	int					atFrame;		//実際にダメージを入れるフレームタイミング
	int				level;			//レベル
	int				life;			//プレイヤーHP
	int				lifeMax[MAX_LEVEL];		//プレイヤー最大HP
	int				power[MAX_LEVEL];
	int				diffend[MAX_LEVEL];
	int				spMax[MAX_LEVEL];
	int				cost[MAX_LEVEL];
	int				blockNum;		//ブロックしている数
	int				blockMax;		//ブロック可能数
	int				charID;			//他のデータに渡すときのキャラ判別用変数
	int				skillID;
	int				charType;		//近接キャラか遠距離キャラか。CHIP_TYPEと同じ数字を入れる
	int				attackSE;		//近接キャラか遠距離キャラか。CHIP_TYPEと同じ数字を入れる
	int				skillTimeMax;
	BOOL			setAble;
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
	BOOL setAble[MAX_PLAYER_SET];
	int cost[MAX_PLAYER_SET];			//キャラコスト
	int setPlayer;	//セットするプレイヤー番号テーブル(添え字？)
	int setCharID[MAX_PLAYER_SET];	//セットしようとしてるキャラが何なのか？
	PlayerStatus *setPlayerNum;
	PlayerPartsStatus *setPlayerParts;
	XMFLOAT3 setPos;	//セットする先の座標
	BOOL setMode;		//セットモードに移行したか否か
	BOOL setCheckMode;
};

HRESULT InitPlayerSet(void);
void PlayerSetMap(void);
void SetModeChange(int i);
void SetPosition(void);
void SetPlayerInfo(PlayerStatus* member, PlayerPartsStatus* memberParts);
BOOL CheckPlayerAble(PlayerStatus *member);
void CharaStateCheck(int num);
void SetAbleChar(void);
void CheckSetChar(void);
PlayerSet *GetSetPos(void);
void DrawBattleSetChar(void);