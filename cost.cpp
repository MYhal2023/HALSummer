//=============================================================================
//
// コスト処理 [cost.cpp]
// Author : 
//
//=============================================================================
#include "cost.h"
static Cost g_Cost;

void InitCost(void)
{
	g_Cost.cost = 0;
	g_Cost.time = 0;
}

void IncreaseCost(int increase)
{
	g_Cost.cost += increase;
}

int GetCost()
{
	return g_Cost.cost;
}

void DecreaseCost(int decrease)
{
	g_Cost.cost -= decrease;
}

//時間経過でコストが増えるようにする
void IncTimeCost(void)
{
	const int MaxTime = 180;
	g_Cost.time++;
	if (g_Cost.time >= MaxTime)
	{
		g_Cost.time = 0;
		IncreaseCost(1);
	}
}

Cost *GetCostNum(void)
{
	return &g_Cost;
}