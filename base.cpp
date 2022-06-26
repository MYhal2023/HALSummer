//=============================================================================
//
// 拠点ライフ処理 [base.cpp]
// Author : 
//
//=============================================================================
#include "base.h"
#include "collision.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static Base g_Base;

//拠点初期化。引数は順に拠点体力、拠点の座標、拠点の数。第2引数と第三引数の数は"絶対に"一緒にする事
HRESULT InitBase(int life, XMFLOAT3 pos[], int baseNum)
{
	g_Base.life = life;
	g_Base.baseNum = baseNum;
	for (int i = 0; i < baseNum; i++)
		g_Base.pos[i] = pos[i];

	return S_OK;
}

//拠点がダメージを受ける条件
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
	PrintDebugProc("拠点体力:%d\n", g_Base.life);
#endif

}

int GetBaseLife(void)
{
	return g_Base.life;
}