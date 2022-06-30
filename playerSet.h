//=============================================================================
//
// プレイヤーセット処理 [playerSet.h]
// Author : 
//
//=============================================================================
#pragma once
#include "player.h"
#include "main.h"
#include "model.h"
#include "input.h"
#include "fieldchip.h"
#include "team.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************



//*****************************************************************************
// グローバル変数
//*****************************************************************************

class PlayerSet
{
private:
	XMFLOAT3 setPos;	//セットする先の座標
	BOOL setMode;		//セットモードに移行したか否か
	int setPlayerTable[MAX_PLAYER_SET];	//セットするプレイヤー番号テーブル(添え字？)
	PLAYER *setPlayerNum;	
	PlayerParts *setPlayerParts;
public:
	HRESULT InitPlayerSet(void);
	void PlayerSetMap(void);
	void SetModeChange(int i);
	void SetPosition(void);
	void SetPlayerInfo(PLAYER* member, PlayerParts* parts);
};