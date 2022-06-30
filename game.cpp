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
static int	g_PlayMode = MAIN_GAME;
static int mode = 1;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitGame(void)
{
	g_ViewPortType_Game = TYPE_FULL_SCREEN;
	InitSystem();
	switch (GetMode())
	{
	case MODE_TITLE:
		break;
	case MODE_GAME:
		InitSystem();
		break;
	case MODE_RESULT:
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

	InitMapChip();

	InitCost();

	SetGrape(5.0f);
	SetGrape(50.0f);
	SetGrape(90.0f);
	SetGrape(100.0f);
	SetGrape(110.0f);
	SetPlayer(XMFLOAT3(200.0f, 10.0f, 100.0f));
	SetNeutrophils(XMFLOAT3(100.0f, 10.0f, 100.0f));
	XMFLOAT3 pos[2];
	pos[0] = { 25.0f, 0.0f, 100.0f };
	pos[1] = { 25.0f, 0.0f, 300.0f };
	InitBase(3, &pos[0], 2);
}

//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
{
	// �e�̏I������
	UninitShadow();

	UninitPlayer();

	UninitEnemy();
	UninitMapChip();
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(void)
{
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_P))
	{
		g_bPause = g_bPause ? FALSE : TRUE;
	}

	if (GetKeyboardTrigger(DIK_O))
	{
		mode == 1 ? mode++ : mode--;
	}
#endif
	FADE fade = GetFade();
	if (fade != FADE_NONE)return;


	if(g_bPause == FALSE)
		return;

	for (int i = 0; i < mode; i++)
	{
		UpdateCamera();

		UpdateMapChip();

		UpdatePlayer();

		UpdateEnemy();

		UpdateLight();

		IncTimeCost();

		// �e�̍X�V����
		UpdateShadow();

		UpdateFog();

		UpdateSound();

		BaseDamage();
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

	DrawMapChip();

	DrawPlayer();

	DrawEnemy();

	// �e�̕`�揈��
	DrawShadow();

	// 2D�̕���`�悷�鏈��
	// Z��r�Ȃ�
	SetDepthEnable(FALSE);

	// ���C�e�B���O�𖳌�
	SetLightEnable(FALSE);


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
// �`�揈��(���C�g�ڐ��B�����������e�Ɋւ���I�u�W�F�N�g�������w��)
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
	float center_w = CHIP_SIZE * (MAX_CHIP_WIDTH - 1) * 0.5f;
	float center_h = CHIP_SIZE * MAX_CHIP_HEIGHT * 0.5f;
	pos = { center_w, 0.0f, center_h };
	//pos = GetEnemy()->pos;	//�f�o�b�O�p
	SetCameraAT(pos);
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
	return FALSE;
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