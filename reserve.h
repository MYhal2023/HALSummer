//=============================================================================
//
// 育成画面処理 [reserve.h]
// Author : 
//
//=============================================================================
#pragma once
class Reserve
{
public:
	int	day;	//何日目か
	int	energy;	//所持エネルギー量
	int oxygen; //所持酸素量
	int mode;	//ボタンを押した時の画面変化に使う
	int selectPw; //強化画面で何を選択しているか
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
	GameStart,
};

enum Reserve_TEXTURE
{
	Button_bg,
	Reserve_bg,
	re_number,
	re_box,
	TEXT_CONFIRM,
	TEXT_DAY,
	TEXT_POWERUP,
	TEXT_START,
};

HRESULT InitReserve(void);
void UninitReserve(void);
void UpdateReserve(void);
void DrawReserve(void);
void DrawButton(XMFLOAT4 color, float px, float py, float sx, float sy);
void DrawNumberRe(int numb, float px, float py, float sx, float sy, XMFLOAT4 color);
void DrawTextReserve(int k, float px, float py, float sx, float sy, XMFLOAT4 color);
void NormalRserveMode(void);
void NormalRserveModeDraw(void);
void UnitPowerUpMode(void);
void PowerUpButton(void);
void ConfirmButton(void); 
void GameStartButton(void);
int GetDay(void);
void DrawReserveChar(void);