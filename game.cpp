//=============================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "camera.h"
#include "light.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "game.h"
#include "collision.h"
#include "debugproc.h"
#include "file.h"
#include "shadow.h"
#include "meshfield.h"
#include "player.h"
#include "cost.h"
#include "enemy.h"
#include "fieldchip.h"
#include "base.h"
#include "playerSet.h"
#include "team.h"
#include "gameover.h"
#include "ui.h"
#include "enemySet.h"
#include "unitdata.h"
#include "text_texture.h"
#include "reserve.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************



//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int	g_ViewPortType_Game = TYPE_FULL_SCREEN;

static BOOL	g_bPause = TRUE;	// ポーズON/OFF
static BOOL g_Slow = FALSE;
static int s_mode = FALSE;
static int	g_PlayMode = MAIN_GAME;
static int mode = 1;
//ここで使用するマップチップナンバーを設定。横幅配列だけ、最大数で揃える。
//間違えて上下逆になっちゃった…左右は逆じゃないよ
int g_DebugMap[][MAX_CHIP_WIDTH + 1]
{
	{1,1,1,1,1,1,1,1},
	{2,2,0,0,0,0,2,2},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,0},
	{3,3,3,3,1,1,1,1},
};

int g_DebugMapObject[][MAX_CHIP_WIDTH + 1]
{
	{9,9,9,9,9,9,9,9},
	{9,9,9,9,9,9,9,1},
	{9,9,9,9,9,9,9,1},
	{9,9,9,9,9,9,9,1},
	{9,9,9,9,9,9,9,9},
	{0,1,9,9,9,9,9,1},
	{9,9,9,9,9,9,0,0},
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGame(void)
{
	g_ViewPortType_Game = TYPE_FULL_SCREEN;
	InitSystem();
	switch (GetDay())
	{
	case 0:	//0はデバッグ用で残しておく？
		InitSystem();
		break;
	case 1:
		InitSystem();
		break;
	case 2:
		break;
	}

	return S_OK;
}

void InitSystem(void)
{
	// ライトを有効化	// 影の初期化処理
	InitShadow();

	InitPlayer();

	InitEnemy();

	// フィールドの初期化
	InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), FIELD_X, FIELD_Z, BLOCK_SIZE, BLOCK_SIZE, WATER);

	InitCost();

	InitOver();


	InitCharFade();

	InitTexttex();

	InitPlayerSet();

	InitUI();
	InitMapChip(g_DebugMap, g_DebugMapObject, 7, MAX_CHIP_WIDTH);
	SetGrape(300.0f, 40, 5, 1);
	SetGrape(1200.0f, 40, 5, 1);
	SetStrept(8000.0f,20,5,1);
	XMFLOAT3 pos[1];
	pos[0] = { 25.0f, 0.0f, 100.0f };
	InitBase(3, &pos[0], 1);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{
	// 影の終了処理
	UninitShadow();

	UninitPlayer();

	UninitOver();
	//UninitTeam();

	UninitEnemy();

	UninitMapChip();

	UninitUI();

	UninitTexttex();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_P))
	{
		if (g_bPause == TRUE)
			g_bPause = FALSE;
		else
			g_bPause = TRUE;
	}

	//等速と倍速の切り替え
	if (GetKeyboardTrigger(DIK_O))
	{
		mode == 1 ? mode++ : mode--;
	}

	FADE fade = GetFade();
	if (fade != FADE_NONE)return;


	if(g_bPause == FALSE)
		return;

	UpdateOver();

	UpdateTexttex();
	if (CheckGameover())return;

	UpdatePlayerSet();

	if (g_Slow == TRUE)
	{
		mode = 1;
		s_mode++;
		if (s_mode > 2)
		{
			s_mode = 0;
		}
		else return;	//スロー中なら2回に1回だけ処理
	}

	for (int i = 0; i < mode; i++)
	{
		UpdateCamera();

		UpdateMapChip();

		UpdatePlayer();

		UpdateEnemy();

		UpdateLight();

		IncTimeCost();

		UpdateCharFade();
		// 影の更新処理
		UpdateShadow();

		UpdateFog();

		UpdateSound();

		UpdateUI();
	}
}

//=============================================================================
// 描画処理(カメラ目線)
//=============================================================================
void DrawGame0(void)
{
	//シェーダー管理
	//ポストエフェクトをかける場合はここから
	int ans = MODE_PLANE;
	SwapShader(ans);

	DrawMeshField();

	DrawBG();	//背景を先に描画

	DrawMapChip();

	DrawPlayer();

	DrawEnemy();

	DrawPlayerLife();

	DrawEnemyLife();

	// 2D座標で物を描画する処理
	// Z比較なし
	SetDepthEnable(FALSE);

	// ライティングを無効
	SetLightEnable(FALSE);

	DrawUI();

	DrawOver();

	//// スコアの描画処理
	//DrawScore();


	//シェーダー管理
	//シェーダーを元に戻す。ポストエフェクトはここまで
	ans = MODE_PLANE;
	SwapShader(ans);

	// ライティングを有効に
	SetLightEnable(TRUE);

	// Z比較あり
	SetDepthEnable(TRUE);
}


//=============================================================================
// 描画処理(ライト目線。生成したい影のオブジェクトだけを指定)
//=============================================================================
void DrawGame1(void)
{
	DrawMeshField();

	DrawMapChip();

	DrawPlayer();

	DrawEnemy();
}


void DrawGame(void)
{
	XMFLOAT3 pos;

	// プレイヤー視点
	//pos = GetPlayer()->pos;
	CAMERA *cam = GetCamera();
	PlayerSet *ps = GetSetPos();
	PLAYER *player = GetPlayer();
	float center_w = CHIP_SIZE * (MAX_CHIP_WIDTH - 1) * 0.5f;
	float center_h = CHIP_SIZE * MAX_CHIP_HEIGHT * 0.5f;
	XMFLOAT3 x = player[ps->setPlayer].pos;

	pos = { center_w, 0.0f, center_h };

	if (!ps->setCheckMode)
		SetCameraAT(pos);
	else
		SetCharaCamera(x);

	SetCamera();
	switch(g_ViewPortType_Game)
	{
	case TYPE_FULL_SCREEN:
			DrawGame0();
			break;
	case TYPE_LEFT_HALF_SCREEN:
	case TYPE_RIGHT_HALF_SCREEN:
		SetViewPort(TYPE_LEFT_HALF_SCREEN);
		DrawGame0();

		// エネミー視点
		break;

	case TYPE_UP_HALF_SCREEN:
	case TYPE_DOWN_HALF_SCREEN:
		SetViewPort(TYPE_UP_HALF_SCREEN);
		DrawGame0();

		break;

	case TYPE_LIGHT_SCREEN:	//ライト目線の描画をするためのビューポートタイプ
		SetViewPort(TYPE_FULL_SCREEN);
		DrawGame1();
		break;

	}

}


void SetPlayMode(int playMode)
{
	g_PlayMode = playMode;
}

int GetPlayMode(void)
{
	return g_PlayMode;
}

void SetSlowMode(BOOL flag)
{
	g_Slow = flag;
}
//void SetPlayStage(int stageNum)
//{
//	g_PlayStage = stageNum;
//}

//int GetPlayStage(void)
//{
//	return g_PlayStage;
//}

void SetViewPortType(int viewport)
{
	g_ViewPortType_Game = viewport;
}

int GetViewPortTypeGame(void)
{
	return g_ViewPortType_Game;
}

//数値のクランプ。引数1:クランプしたい数値、引数2:最小値、引数3:最大値
float FloatClamp(float x, float a, float b)
{
	if (x < a)
	{
		return a;
	}
	else if (x > b)
	{
		return b;
	}
	else
	{
		return x;
	}
}

//float型の数値の比較をする。flagがFALSEなら小さい方を、TRUEなら大きい方を返す
float FloatCompare(BOOL flag, float a, float b)
{
	float ans = 0.0f;//一応0.0fをセットしておく
	switch (flag)
	{
	case FALSE:
		if (a > b)return b;
		else	  return a;
		break;
	case TRUE:
		if (a > b)return a;
		else	  return b;
		break;
	}

	return ans;
}


BOOL CheckGameover(void)
{
	BOOL ans = FALSE;
	int life = GetEnemyNum() - GetBanishEnemy();
	if (GetBaseLife() <= 0) { 
		ans = TRUE;
		SetOverType(OVER_LOSE);
	}
	else if (life <= 0) {
		ans = TRUE;
		SetOverType(OVER_WIN);
	}
	return ans;
}

//ダメージ計算関数。引数のそれぞれに、攻撃者の攻撃力と防御側の防御力を持ってくる
int DamageFunc(int attacker, int diffender)
{
	//ひとまず単純な計算式で済ます
	int ans = attacker - diffender;
	//必ず1以上でダメージを返すようにする
	if (ans <= 0)
		ans = 1;
	return ans;
}

int GetSpeedMode(void)
{
	return mode;
}