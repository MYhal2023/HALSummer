//=============================================================================
//
// コスト処理 [cost.h]
// Author : 
//
//=============================================================================
#pragma once
#define MAX_COST (99)
struct Cost
{
	int cost;
	int time;
	int costMaxTime;
};
void InitCost(void);		//イニシャライザ
void SetCost(int i);
void IncreaseCost(int increase);	//コストを増やす関数
int GetCost(void);					//現在のコストがいくつなのか
void DecreaseCost(int decrease);	//コストを減らす関数
void IncTimeCost(void);
Cost* GetCostNum(void);
