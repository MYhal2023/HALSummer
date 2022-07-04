//=============================================================================
//
// バトル中の線形補完データ(敵味方含む) [interporationdata.h]
// Author : 
//
//=============================================================================
#pragma once
#include "main.h"
#define IP_DATA_NUM (3)
#define IE_DATA_NUM (3)
// 階層アニメーションデータ
static INTERPOLATION_DATA neutro_Attack[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 10, },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	   XMFLOAT3(2.0f, 2.0f, 2.0f), 50 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(0.8f, 1.0f, 1.0f), 30 },
};
static INTERPOLATION_DATA neutro_Attack2[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 10, },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	   XMFLOAT3(3.0f, 3.0f, 3.0f), 50 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(0.8f, 1.0f, 1.0f), 30 },
};
static INTERPOLATION_DATA neutro_Attack3[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 10, },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	   XMFLOAT3(2.0f, 2.0f, 2.0f), 50 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(0.8f, 1.0f, 1.0f), 30 },
};
enum EnemyLinerData
{
	neuterMainAttack,
	neuterParts001Attack,
	neuterParts002Attack,
};
enum PlayerLinerData
{
	neuterMainAttack,
	neuterParts001Attack,
	neuterParts002Attack,
};

static INTERPOLATION_DATA* playerData[IP_DATA_NUM] = { neutro_Attack ,neutro_Attack2 ,neutro_Attack3 };
static int pDatasize[IP_DATA_NUM] = { 3,3,3};

static INTERPOLATION_DATA* enemyData[IE_DATA_NUM] = { neutro_Attack ,neutro_Attack2 ,neutro_Attack3 };
static int eDatasize[IE_DATA_NUM] = { 3,3,3 };

INTERPOLATION_DATA *GetData(int i)
{
	return playerData[i];
}

int *GetDataSize(void)
{
	return &pDatasize[0];
}

INTERPOLATION_DATA *GetEnemyData(int i)
{
	return enemyData[i];
}

int *GetEnemyDataSize(void)
{
	return &eDatasize[0];
}