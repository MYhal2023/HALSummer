//=============================================================================
//
// ���j�b�g���ʏ��� [unitdata.cpp]
// Author : 
//
//=============================================================================

#include "unitdata.h"
static UnitFade g_UnitFade[MAX_BATTLE_MODEL];
//�Y�����������Ɏ����Ă���
void FadeEnemy(DX11_MODEL *character, int type)
{
	if (type == 0)
	{
		for (int j = 0; j < character->SubsetNum; j++)
			SetModelDiffuse(character, j, XMFLOAT4(1.0f, 0.0f, 1.0f, 0.0f));
	}
}
