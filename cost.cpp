//=============================================================================
//
// コスト処理 [cost.cpp]
// Author : 
//
//=============================================================================
#include "cost.h"
#include "debugproc.h"
static Cost g_Cost;

//コストに関する変数初期化
void InitCost(void)
{
	g_Cost.cost = 10;
	g_Cost.time = 0;
	g_Cost.costMaxTime = 120;

}

void SetCost(int i)
{
	g_Cost.cost = i;
#ifdef _DEBUG
	g_Cost.cost = 90;
#endif
}
//コストを一気に増やしたい時に
void IncreaseCost(int increase)
{
	g_Cost.cost += increase;
}

int GetCost(void)
{
	return g_Cost.cost;
}

//コストを減らしたい時に。主にユニットを置いた時に使うかも
void DecreaseCost(int decrease)
{
	g_Cost.cost -= decrease;
}

//時間経過でコストが増えるようにする
void IncTimeCost(void)
{
	g_Cost.time++;
	if (g_Cost.time >= g_Cost.costMaxTime)
	{
		g_Cost.time = 0;
		IncreaseCost(1);
	}
}

Cost *GetCostNum(void)
{
	return &g_Cost;
}