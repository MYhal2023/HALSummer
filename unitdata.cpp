//=============================================================================
//
// ƒ†ƒjƒbƒg‹¤’Êˆ— [unitdata.cpp]
// Author : 
//
//=============================================================================

#include "unitdata.h"
static UnitFade g_UnitFade[MAX_BATTLE_MODEL];
//“Y‚¦š‚ğˆø”‚É‚Á‚Ä‚­‚é
void FadeEnemy(DX11_MODEL *character, int type)
{
	if (type == 0)
	{
		for (int j = 0; j < character->SubsetNum; j++)
			SetModelDiffuse(character, j, XMFLOAT4(1.0f, 0.0f, 1.0f, 0.0f));
	}
}
