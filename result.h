//=============================================================================
//
// リザルト処理 [result.h]
// Author : 
//
//=============================================================================
#pragma once
#define MAX_REWARD	(5)
#define MAX_PLAYER_NUM	(10)
struct Result
{
	XMFLOAT4 color;
	XMFLOAT2 pos;
	XMFLOAT2 size;
	int textNo;
	int type;
	int enemyNum;	//敵総数
	int beatNum;	//撃退数
	int charId[MAX_PLAYER_NUM];		//キャラID
	int damage[MAX_PLAYER_NUM];		//ダメージを与えた数
};

struct Reward
{
	int num;				//何種類の報酬があるのか
	int ID[MAX_REWARD];		//報酬の素材ID
	int value[MAX_REWARD];	//報酬の素材量
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
	result_numb,
	result_var,
	result_energy,
	result_oxy,
	result_iron,
	Report,
};

HRESULT InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
void WinResult(void);
void LoseResult(void);
void InitReward(void);
void DrawReward(XMFLOAT2 pos, float size);
void SetReward(int id, int value);
void IncreaseReward(Reward *reward);
Reward *GetReward(void);
void DrawResultButton(XMFLOAT4 color, float px, float py, float sx, float sy);
void DrawResultNumber(int numb, float px, float py, float sx, float sy, XMFLOAT4 color);
void DrawResultChar(void);