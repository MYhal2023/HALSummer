//=============================================================================
//
// �Q�[����ʏ��� [game.cpp]
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
// �}�N����`
//*****************************************************************************



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int	g_ViewPortType_Game = TYPE_FULL_SCREEN;

static BOOL	g_bPause = TRUE;	// �|�[�YON/OFF
static BOOL g_Slow = FALSE;
static int s_mode = FALSE;
static int	g_PlayMode = MAIN_GAME;
static int mode = 1;
//�����Ŏg�p����}�b�v�`�b�v�i���o�[��ݒ�B�����z�񂾂��A�ő吔�ő�����B
//�ԈႦ�ď㉺�t�ɂȂ���������c���E�͋t����Ȃ���
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
// ����������
//=============================================================================
HRESULT InitGame(void)
{
	g_ViewPortType_Game = TYPE_FULL_SCREEN;
	InitSystem();
	switch (GetDay())
	{
	case 0:	//0�̓f�o�b�O�p�Ŏc���Ă����H
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
	// ���C�g��L����	// �e�̏���������
	InitShadow();

	InitPlayer();

	InitEnemy();

	// �t�B�[���h�̏�����
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
// �I������
//=============================================================================
void UninitGame(void)
{
	// �e�̏I������
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
// �X�V����
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

	//�����Ɣ{���̐؂�ւ�
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
		else return;	//�X���[���Ȃ�2���1�񂾂�����
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
		// �e�̍X�V����
		UpdateShadow();

		UpdateFog();

		UpdateSound();

		UpdateUI();
	}
}

//=============================================================================
// �`�揈��(�J�����ڐ�)
//=============================================================================
void DrawGame0(void)
{
	//�V�F�[�_�[�Ǘ�
	//�|�X�g�G�t�F�N�g��������ꍇ�͂�������
	int ans = MODE_PLANE;
	SwapShader(ans);

	DrawMeshField();

	DrawBG();	//�w�i���ɕ`��

	DrawMapChip();

	DrawPlayer();

	DrawEnemy();

	DrawPlayerLife();

	DrawEnemyLife();

	// 2D���W�ŕ���`�悷�鏈��
	// Z��r�Ȃ�
	SetDepthEnable(FALSE);

	// ���C�e�B���O�𖳌�
	SetLightEnable(FALSE);

	DrawUI();

	DrawOver();

	//// �X�R�A�̕`�揈��
	//DrawScore();


	//�V�F�[�_�[�Ǘ�
	//�V�F�[�_�[�����ɖ߂��B�|�X�g�G�t�F�N�g�͂����܂�
	ans = MODE_PLANE;
	SwapShader(ans);

	// ���C�e�B���O��L����
	SetLightEnable(TRUE);

	// Z��r����
	SetDepthEnable(TRUE);
}


//=============================================================================
// �`�揈��(���C�g�ڐ��B�����������e�̃I�u�W�F�N�g�������w��)
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

	// �v���C���[���_
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

		// �G�l�~�[���_
		break;

	case TYPE_UP_HALF_SCREEN:
	case TYPE_DOWN_HALF_SCREEN:
		SetViewPort(TYPE_UP_HALF_SCREEN);
		DrawGame0();

		break;

	case TYPE_LIGHT_SCREEN:	//���C�g�ڐ��̕`������邽�߂̃r���[�|�[�g�^�C�v
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

//���l�̃N�����v�B����1:�N�����v���������l�A����2:�ŏ��l�A����3:�ő�l
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

//float�^�̐��l�̔�r������Bflag��FALSE�Ȃ珬���������ATRUE�Ȃ�傫������Ԃ�
float FloatCompare(BOOL flag, float a, float b)
{
	float ans = 0.0f;//�ꉞ0.0f���Z�b�g���Ă���
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

//�_���[�W�v�Z�֐��B�����̂��ꂼ��ɁA�U���҂̍U���͂Ɩh�䑤�̖h��͂������Ă���
int DamageFunc(int attacker, int diffender)
{
	//�ЂƂ܂��P���Ȍv�Z���ōς܂�
	int ans = attacker - diffender;
	//�K��1�ȏ�Ń_���[�W��Ԃ��悤�ɂ���
	if (ans <= 0)
		ans = 1;
	return ans;
}

int GetSpeedMode(void)
{
	return mode;
}