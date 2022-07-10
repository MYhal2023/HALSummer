//=============================================================================
//
// コスト処理 [cost.h]
// Author : 
//
//=============================================================================
#pragma once

struct Cost
{
	int cost;
	int time;
	int costMaxTime;
};
void InitCost(void);		//イニシャライザ
void IncreaseCost(int increase);	//コストを増やす関数
int GetCost();					//現在のコストがいくつなのか
void DecreaseCost(int decrease);	//コストを減らす関数
void IncTimeCost(void);
Cost* GetCostNum(void);
