//=============================================================================
//
// �R�X�g���� [cost.cpp]
// Author : 
//
//=============================================================================
#include "cost.h"
#include "debugproc.h"
static Cost g_Cost;

//�R�X�g�Ɋւ���ϐ�������
void InitCost(void)
{
	g_Cost.cost = 0;
	g_Cost.time = 0;
}

//�R�X�g����C�ɑ��₵��������
void IncreaseCost(int increase)
{
	g_Cost.cost += increase;
}

int GetCost()
{
	return g_Cost.cost;
}

//�R�X�g�����炵�������ɁB��Ƀ��j�b�g��u�������Ɏg������
void DecreaseCost(int decrease)
{
	g_Cost.cost -= decrease;
}

//���Ԍo�߂ŃR�X�g��������悤�ɂ���
void IncTimeCost(void)
{
	const int MaxTime = 180;
	g_Cost.time++;
	if (g_Cost.time >= MaxTime)
	{
		g_Cost.time = 0;
		IncreaseCost(1);
	}
#ifdef _DEBUG
	PrintDebugProc("�R�X�g:%d\n", g_Cost.cost);

#endif
}

Cost *GetCostNum(void)
{
	return &g_Cost;
}