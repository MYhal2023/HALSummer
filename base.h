//=============================================================================
//
// 拠点ライフ処理 [base.cpp]
// Author : 
//
//=============================================================================
#pragma once
#include "debugproc.h"
#include "player.h"
#include "enemy.h"
#define MAX_BASE (2)

struct Base
{
	int life;	//拠点ライフ
	int baseNum;
	XMFLOAT3 pos[MAX_BASE];	//拠点がある座標。複数あるかもしれないから、ここは可変長配列
};

HRESULT InitBase(int life, XMFLOAT3 pos[], int baseNum);
int GetBaseLife(void);
Base *GetBase(void);