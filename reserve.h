//=============================================================================
//
// 育成画面処理 [reserve.h]
// Author : 
//
//=============================================================================
#pragma once
#include "team.h"

class Reserve
{
public:
	int	day;	//何日目か
	int	energy;	//所持エネルギー量
	int oxygen; //所持酸素量
	int iron;	//所持鉄分
	int mode;	//ボタンを押した時の画面変化に使う
	int selectPw; //強化画面で何を選択しているか
	BOOL pwMode;
};

class Button
{
public:
	XMFLOAT4 color;
	XMFLOAT2 pos;
	XMFLOAT2 size;
	int texNo;
	BOOL use;
	BOOL cursol;	//選択されているか

};

enum ButtonType
{
	UnitPowerUp,
	UnitConfirm,
	ReserveHelp,
	GameStart,
};

enum PwButtonType
{
	CanselButton,
	LevelupButton
};

//ヘルプ画像は連続させる
enum Reserve_TEXTURE
{
	Button_bg,
	Button_tx,
	Reserve_bg,
	re_number,
	re_box,
	arRight,
	TEXT_CONFIRM,
	TEXT_DAY,
	TEXT_POWERUP,
	TEXT_START,
	TEXT_LUP,
	TEXT_CANCEL,
	Help_ReserveTex,
};
enum Icon_TEXTURE
{
	icCost,
	icLife,
	icAttack,
	icDiffend,
	icBlock,
	icSp,
	icEnergy,
	icOxygen,
	icIron,
};


HRESULT InitReserve(void);
void UninitReserve(void);
void UpdateReserve(void);
void DrawReserve(void);
Reserve *GetReserve(void);
void DrawButton(XMFLOAT4 color, float px, float py, float sx, float sy);
void DrawNumberRe(int numb, float px, float py, float sx, float sy, XMFLOAT4 color);
void DrawTextReserve(int k, float px, float py, float sx, float sy, XMFLOAT4 color);
void NormalRserveMode(void);
void NormalRserveModeDraw(void);
void UnitPowerUpMode(void);
void UnitConfirmMode(void);
void PowerUpButton(void);
void ConfirmButton(void); 
void ReserveHelpButton(void);
void GameStartButton(void);
int GetDay(void);
void DrawReserveChar(void);
void DrawCharStatus(XMFLOAT2 pos, int k);
void DrawCharAllStatus(XMFLOAT2 pos, int k);
void DrawNeedMaterial(XMFLOAT2 pos, float size, int no);
BOOL CheckPowerUpMaterial(PlayerStatus *member);
void ReduceMaterial(PlayerStatus *member);
void ReserveHelpMode(void);
void DrawReserveHelp(void);
void DrawMaxLevelChar(XMFLOAT2 pos, int k);