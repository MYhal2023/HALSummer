//=============================================================================
//
// Ug [result.h]
// Author : 
//
//=============================================================================
#pragma once
#define MAX_REWARD	(5)
struct Result
{
	XMFLOAT4 color;
	XMFLOAT2 pos;
	XMFLOAT2 size;
	int textNo;
	int type;

};

struct Reward
{
	int num;				//½íÞÌñVª éÌ©
	int ID[MAX_REWARD];		//ñVÌfÞID
	int value[MAX_REWARD];	//ñVÌfÞÊ
};
enum ResultType
{
	Lose,
	Win
};

enum ResultTexture
{
	win,
	lose,
};

HRESULT InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
void WinResult(void);
void LoseResult(void);
void IncreaseReward(Reward *reward);
Reward *GetReward(void);