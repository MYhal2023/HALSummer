//=============================================================================
//
// ���_���C�t���� [base.cpp]
// Author : 
//
//=============================================================================
#include "base.h"
#include "collision.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static Base g_Base;

//���_�������B�����͏��ɋ��_�̗́A���_�̍��W�A���_�̐��B��2�����Ƒ�O�����̐���"��΂�"�ꏏ�ɂ��鎖
HRESULT InitBase(int life, XMFLOAT3 pos[], int baseNum)
{
	g_Base.life = life;
	g_Base.baseNum = baseNum;
	for (int i = 0; i < baseNum; i++)
		g_Base.pos[i] = pos[i];

	return S_OK;
}

//���_���_���[�W���󂯂����
void BaseDamage(void)
{
	ENEMY *enemy = GetEnemy();
	for (int i = 0; i < g_Base.baseNum; i++)
	{
		for (int k = 0; k < GetEnemyNum(); k++)
		{
			if (enemy[k].use != TRUE)continue;
			if (!CollisionBC(enemy[k].pos, g_Base.pos[i], 10.0f, 10.0f))continue;

			g_Base.life--;
			enemy[k].use = FALSE;
		}
	}

#ifdef _DEBUG
	PrintDebugProc("���_�̗�:%d\n", g_Base.life);
#endif

}

int GetBaseLife(void)
{
	return g_Base.life;
}