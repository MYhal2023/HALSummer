//=============================================================================
//
// �o�g�����̃G�l�~�[���� [enemy.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "model.h"
#include "game.h"
#include "enemy.h"
#include "shadow.h"
#include "light.h"
#include "meshfield.h"
#include "collision.h"
#include "time.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VALUE_MOVE			(2.0f)							// �ړ���
#define	VALUE_AT_MOVE		(4.0f)							// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// ��]��

#define ENEMY_SHADOW_SIZE	(1.0f)							// �e�̑傫��
#define ENEMY_OFFSET_Y		(0.0f)							// �v���C���[�̑��������킹��
#define ENEMY_OFFSET_Z		(-300.0f)							// �v���C���[�̑��������킹��
#define ENEMY_LIFE			(100)								// �v���C���[�̃��C�t

#define ENEMY_PARTS_MAX	(1)								// �v���C���[�̃p�[�c�̐�
#define ENEMY_AT_FLAME		(30.0f)							// �v���C���[�̍U���t���[��
#define ENEMY_INVINC_FLAME	(120.0f)						// �v���C���[���G�t���[��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ENEMY		g_Enemy[MAX_ENEMY];						// �G�l�~�[
static EnemyParts	g_Parts[MAX_ENEMY * 3];					// �G�l�~�[�̃p�[�c�B�]�T�������ăG�l�~�[�~2�{�̐��p��

static BOOL			g_Load = FALSE;
static int			atCount;
static int			enemyNum = 0;		//���̂̃G�l�~�[�����邩
static int			partsNum = 0;		//���v�ł����̃p�[�c���g���̂�
static MOVERINE		moveTbl[] = { XMFLOAT3(300.0f, 0.0f, 150.0f), XMFLOAT3(300.0f, 0.0f, 100.0f), XMFLOAT3(0.0f, 0.0f, 100.0f) };
// �K�w�A�j���[�V�����f�[�^
static INTERPOLATION_DATA grape_Attack[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 10, },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.5f, 0.0f),	   XMFLOAT3(2.0f, 2.0f, 2.0f), 50 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};




//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//LoadModel(MODEL_GRAPE, &g_Enemy[i].model);
		//// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		//GetModelDiffuse(&g_Enemy[i].model, &g_Enemy[i].diffuse[0]);

		g_Enemy[i].load = FALSE;

		g_Enemy[i].pos = { -100.0f, ENEMY_OFFSET_Y, 0.0f };
		g_Enemy[i].rot = { 0.0f, 0.0f, 0.0f };
		g_Enemy[i].scl = { 0.8f, 1.0f, 1.0f };

		g_Enemy[i].size = ENEMY_SIZE;	// �����蔻��̑傫��
		g_Enemy[i].life = ENEMY_LIFE;
		g_Enemy[i].lifeMax = g_Enemy[i].life;
		g_Enemy[i].use = FALSE;
		g_Enemy[i].attack = FALSE;
		g_Enemy[i].attackUse = FALSE;
		g_Enemy[i].start = 0;
		g_Enemy[i].tbl_adr = NULL;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
		g_Enemy[i].tbl_size = 0;	// �o�^�����e�[�u���̃��R�[�h����
		g_Enemy[i].move_time = 0;	// ���s����

		// �K�w�A�j���[�V�����p�̏���������
		g_Enemy[i].parent = NULL;			// �{�́i�e�j�Ȃ̂�NULL������
	}
	for (int i = 0; i < MAX_ENEMY * 2; i++)
	{
		g_Parts[i].pos = { 0.0f, 0.0f, 0.0f };		// �|���S���̈ʒu
		g_Parts[i].rot = { 0.0f, 0.0f, 0.0f };		// �|���S���̌���(��])
		g_Parts[i].scl = {1.0f, 1.0f, 1.0f};		// �|���S���̑傫��(�X�P�[��)

		g_Parts[i].load = FALSE;
		// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
		g_Parts[i].tbl_adr = NULL;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
		g_Parts[i].tbl_size = 0;	// �o�^�����e�[�u���̃��R�[�h����
		g_Parts[i].move_time = 0;	// ���s����
		g_Parts[i].parent = NULL;	// �������e�Ȃ�NULL�A�������q���Ȃ�e��enemy�A�h���X
	}

	enemyNum = 0;
	partsNum = 0;
	g_Load = TRUE;
	atCount = 0;
	return S_OK;

}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	if (g_Load == FALSE) return;

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		// ���f���̉������
		if (g_Enemy[i].load)
		{
			UnloadModel(&g_Enemy[i].model);
			g_Enemy[i].load = FALSE;
		}
	}
	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	for (int i = 0; i < enemyNum; i++)
	{
		//���[���x�[�X�œG���U�����A�X�e�[�g�x�[�X�őJ�ڂ��s���B���`��Ԃňړ�

		SetEnemyTime(i);	//�G�l�~�[�̏o���`�F�b�N

		if (g_Enemy[i].use != TRUE)continue;	//����ł邩�o�����ĂȂ��z�̓X���[

		//�G�l�~�[�̃X�e�[�g����
		g_Enemy[i].state = StateCheck(i);
		CheckTarget(i);
		switch (g_Enemy[i].state)
		{
		case Move:
			EnemyMove(i);
			break;

		case Attack:
			EnemyInterPoration(i);
			break;
		}

	}
#ifdef _DEBUG
	PrintDebugProc("�G�l�~�[�̗�:%d\n", g_Enemy[0].life);
	PrintDebugProc("�G�l�~�[�̗�:%d\n", g_Enemy[1].life);
	PrintDebugProc("�G�l�~�[�̗�:%d\n", g_Enemy[2].life);
	PrintDebugProc("�G�l�~�[�̗�:%d\n", g_Enemy[3].life);
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	for (int i = 0; i < enemyNum; i++)
	{
		if (g_Enemy[i].use != TRUE)continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Enemy[i].pos.x, g_Enemy[i].pos.y + 20.0f, g_Enemy[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Enemy[i].mtxWorld, mtxWorld);

		// ���f���`��
		DrawModel(&g_Enemy[i].model);

		if (g_Enemy[i].partsNum == 0)continue;

		// �p�[�c�̊K�w�A�j���[�V����
		for (int k = g_Enemy[i].startNum; k < g_Enemy[i].startNum + g_Enemy[i].partsNum; k++)
		{
			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_Parts[k].scl.x, g_Parts[k].scl.y, g_Parts[k].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[k].rot.x, g_Parts[k].rot.y, g_Parts[k].rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_Parts[k].pos.x, g_Parts[k].pos.y, g_Parts[k].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			if (g_Parts[k].parent != NULL)	// �q����������e�ƌ�������
			{
				mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[k].parent->mtxWorld));
				// ��
				// g_Enemy[k].mtxWorld���w���Ă���
			}

			XMStoreFloat4x4(&g_Parts[k].mtxWorld, mtxWorld);

			// �g���Ă���Ȃ珈������B�����܂ŏ������Ă��闝�R�͑��̃p�[�c�����̃p�[�c���Q�Ƃ��Ă���\�������邩��B
			if (g_Parts[k].load == FALSE) continue;

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			// ���f���`��
			DrawModel(&g_Parts[k].model);

		}

	}


	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// �v���C���[�����擾
//=============================================================================
ENEMY *GetEnemy(void)
{
	return &g_Enemy[0];
}

//����1�F�z��̎g�p���J�n����Y�����A����2�F���t���[���̎��ɏo���̂�
void SetGrape(float time)
{
	LoadModel(MODEL_GRAPE, &g_Enemy[enemyNum].model);
	// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
	GetModelDiffuse(&g_Enemy[enemyNum].model, &g_Enemy[enemyNum].diffuse[0]);

	g_Enemy[enemyNum].load = TRUE;

	g_Enemy[enemyNum].pos = { moveTbl[0].start.x, ENEMY_OFFSET_Y, moveTbl[0].start.z };
	g_Enemy[enemyNum].rot = { 0.0f, 0.0f, 0.0f };
	g_Enemy[enemyNum].scl = { 0.8f, 1.0f, 1.0f };

	g_Enemy[enemyNum].size = ENEMY_SIZE;	// �����蔻��̑傫��
	g_Enemy[enemyNum].life = ENEMY_LIFE;
	g_Enemy[enemyNum].lifeMax = g_Enemy[enemyNum].life;
	g_Enemy[enemyNum].spd = 1.0f;
	g_Enemy[enemyNum].use = FALSE;
	g_Enemy[enemyNum].attack = FALSE;
	g_Enemy[enemyNum].attackUse = FALSE;
	g_Enemy[enemyNum].atInterval = 180;
	g_Enemy[enemyNum].atFrame = 20;
	g_Enemy[enemyNum].atFrameCount = 0;
	g_Enemy[enemyNum].power = 5;
	g_Enemy[enemyNum].target = NULL;
	g_Enemy[enemyNum].atCount = g_Enemy[enemyNum].atInterval;	//�ŏ��̓m�[�^�C���ōU�����[�V������
	g_Enemy[enemyNum].type = Proximity;;
	g_Enemy[enemyNum].start = time;
	g_Enemy[enemyNum].moveData = moveTbl;
	g_Enemy[enemyNum].moveCount = 0.0f;
	g_Enemy[enemyNum].moveTblSize = sizeof(moveTbl) / sizeof(MOVERINE);
	g_Enemy[enemyNum].nowTbl = 0;
	g_Enemy[enemyNum].tbl_adr = grape_Attack;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
	g_Enemy[enemyNum].tbl_size = sizeof(grape_Attack) / sizeof(INTERPOLATION_DATA);	// �o�^�����e�[�u���̃��R�[�h����
	g_Enemy[enemyNum].move_time = 0.0f;	// ���s����
	g_Enemy[enemyNum].partsNum = 3;
	g_Enemy[enemyNum].startNum = partsNum;
	// �K�w�A�j���[�V�����p�̏���������
	g_Enemy[enemyNum].parent = NULL;			// �{�́i�e�j�Ȃ̂�NULL������

	LoadModel(MODEL_GRAPE_PARTS001, &g_Parts[partsNum].model);
	// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
	GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);
	g_Parts[partsNum].load = TRUE;

	g_Parts[partsNum].pos = { 0.0f, 0.0f, 0.0f };		// �|���S���̈ʒu
	g_Parts[partsNum].rot = { 0.0f, 0.0f, 0.0f };		// �|���S���̌���(��])
	g_Parts[partsNum].scl = { 1.0f, 1.0f, 1.0f };		// �|���S���̑傫��(�X�P�[��)

	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
	g_Parts[partsNum].tbl_adr = NULL;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
	g_Parts[partsNum].tbl_size = 0;	// �o�^�����e�[�u���̃��R�[�h����
	g_Parts[partsNum].move_time = 0;	// ���s����
	g_Parts[partsNum].parent = &g_Enemy[enemyNum];	// �������e�Ȃ�NULL�A�������q���Ȃ�e��enemy�A�h���X
	partsNum++;

	LoadModel(MODEL_GRAPE_PARTS002, &g_Parts[partsNum].model);
	// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
	GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);
	g_Parts[partsNum].load = TRUE;

	g_Parts[partsNum].pos = { 0.0f, 0.0f, 0.0f };		// �|���S���̈ʒu
	g_Parts[partsNum].rot = { 0.0f, 0.0f, 0.0f };		// �|���S���̌���(��])
	g_Parts[partsNum].scl = { 1.0f, 1.0f, 1.0f };		// �|���S���̑傫��(�X�P�[��)
	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
	g_Parts[partsNum].tbl_adr = NULL;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
	g_Parts[partsNum].tbl_size = 0;	// �o�^�����e�[�u���̃��R�[�h����
	g_Parts[partsNum].move_time = 0;	// ���s����
	g_Parts[partsNum].parent = &g_Enemy[enemyNum];	// �������e�Ȃ�NULL�A�������q���Ȃ�e��enemy�A�h���X
	partsNum++;

	LoadModel(MODEL_GRAPE_PARTS003, &g_Parts[partsNum].model);
	// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
	GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);
	g_Parts[partsNum].load = TRUE;

	g_Parts[partsNum].pos = { 0.0f, 0.0f, 0.0f };		// �|���S���̈ʒu
	g_Parts[partsNum].rot = { 0.0f, 0.0f, 0.0f };		// �|���S���̌���(��])
	g_Parts[partsNum].scl = { 1.0f, 1.0f, 1.0f };		// �|���S���̑傫��(�X�P�[��)
	partsNum++;

		// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
	g_Parts[partsNum-1].tbl_adr = NULL;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
	g_Parts[partsNum-1].tbl_size = 0;	// �o�^�����e�[�u���̃��R�[�h����
	g_Parts[partsNum-1].move_time = 0;	// ���s����
	g_Parts[partsNum-1].parent = &g_Enemy[enemyNum];	// �������e�Ȃ�NULL�A�������q���Ȃ�e��enemy�A�h���X
	partsNum++;

	enemyNum++;
}

//�Y�����������Ɏ����Ă���
void SetEnemyTime(int i)
{
	for (int k = 0; k < enemyNum; k++)
	{
		if (g_Enemy[i].start <= 0.0f)continue;	//���ɏo���ς݂̃G�l�~�[�͂����Œe��

		g_Enemy[i].start -= 1.0f;
		if (g_Enemy[i].start <= 0.0f)//�G�l�~�[���o��������
			g_Enemy[i].use = TRUE;
	}
}

//��ɃG�l�~�[�̍U�����̐��`��ԁB�_���[�W�����������ōs��
void EnemyInterPoration(int i)
{
	if (g_Enemy[i].tbl_adr == NULL)return;	// ���`��Ԃ����s����H

	//
	// ���`��Ԃ̏���
	// �ړ�����
	int		index = (int)g_Enemy[i].move_time;
	float	time = g_Enemy[i].move_time - index;
	int		size = g_Enemy[i].tbl_size;

	float dt = 1.0f / g_Enemy[i].tbl_adr[index].frame;	// 1�t���[���Ői�߂鎞��
	g_Enemy[i].move_time += dt;							// �A�j���[�V�����̍��v���Ԃɑ���

	if (index > (size - 2))	// �S�[�����I�[�o�[���Ă�����A�f�[�^���ŏ��ɖ߂��čU�����I��
	{
		g_Enemy[i].move_time = 0.0f;
		index = 0;
		g_Enemy[i].atCount = 0;
		g_Enemy[i].state = Move;
		g_Enemy[i].attackUse = FALSE;
	}

	// ���W�����߂�	X = StartX + (EndX - StartX) * ���̎���
	XMVECTOR p1 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 1].pos);	// ���̏ꏊ
	XMVECTOR p0 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 0].pos);	// ���݂̏ꏊ
	XMVECTOR vec = p1 - p0;
	XMStoreFloat3(&g_Enemy[i].pos, XMLoadFloat3(&g_Enemy[i].pos) + p0 + vec * time);

	// ��]�����߂�	R = StartX + (EndX - StartX) * ���̎���
	XMVECTOR r1 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 1].rot);	// ���̊p�x
	XMVECTOR r0 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 0].rot);	// ���݂̊p�x
	XMVECTOR rot = r1 - r0;
	XMStoreFloat3(&g_Enemy[i].rot, r0 + rot * time);

	// scale�����߂� S = StartX + (EndX - StartX) * ���̎���
	XMVECTOR s1 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 1].scl);	// ����Scale
	XMVECTOR s0 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 0].scl);	// ���݂�Scale
	XMVECTOR scl = s1 - s0;
	XMStoreFloat3(&g_Enemy[i].scl, s0 + scl * time);

	for (int k = g_Enemy[i].startNum; k < g_Enemy[i].startNum + g_Enemy[i].partsNum; k++)
	{
		if (g_Parts[k].tbl_adr == NULL)continue;;	// ���`��Ԃ����s����H

		int		index = (int)g_Parts[k].move_time;
		float	time = g_Parts[k].move_time - index;
		int		size = g_Parts[k].tbl_size;

		float dt = 1.0f / g_Parts[k].tbl_adr[index].frame;	// 1�t���[���Ői�߂鎞��
		g_Parts[k].move_time += dt;							// �A�j���[�V�����̍��v���Ԃɑ���

		if (index > (size - 2))	// �S�[�����I�[�o�[���Ă�����A�f�[�^���ŏ��ɖ߂��čU�����I��
		{
			g_Parts[k].move_time = 0.0f;
			index = 0;
		}

		// ���W�����߂�	X = StartX + (EndX - StartX) * ���̎���
		XMVECTOR p1 = XMLoadFloat3(&g_Parts[k].tbl_adr[index + 1].pos);	// ���̏ꏊ
		XMVECTOR p0 = XMLoadFloat3(&g_Parts[k].tbl_adr[index + 0].pos);	// ���݂̏ꏊ
		XMVECTOR vec = p1 - p0;
		XMStoreFloat3(&g_Parts[k].pos, XMLoadFloat3(&g_Parts[k].pos) + p0 + vec * time);

		// ��]�����߂�	R = StartX + (EndX - StartX) * ���̎���
		XMVECTOR r1 = XMLoadFloat3(&g_Parts[k].tbl_adr[index + 1].rot);	// ���̊p�x
		XMVECTOR r0 = XMLoadFloat3(&g_Parts[k].tbl_adr[index + 0].rot);	// ���݂̊p�x
		XMVECTOR rot = r1 - r0;
		XMStoreFloat3(&g_Parts[k].rot, r0 + rot * time);

		// scale�����߂� S = StartX + (EndX - StartX) * ���̎���
		XMVECTOR s1 = XMLoadFloat3(&g_Parts[k].tbl_adr[index + 1].scl);	// ����Scale
		XMVECTOR s0 = XMLoadFloat3(&g_Parts[k].tbl_adr[index + 0].scl);	// ���݂�Scale
		XMVECTOR scl = s1 - s0;
		XMStoreFloat3(&g_Parts[k].scl, s0 + scl * time);
	}


	//��������U������
	if (g_Enemy[i].target == NULL || g_Enemy[i].attackUse == TRUE)return;	//�Z�b�g���Ă��Ȃ��A�Z�b�g����K�v���Ȃ��U�������邩���H
	g_Enemy[i].atFrameCount++;
	//�U���t���[���ɒB������A�_���[�W�v�Z�֐������Ƀ^�[�Q�b�g�Ƀ_���[�W
	if (g_Enemy[i].atFrameCount >= g_Enemy[i].atFrame) {
		g_Enemy[i].target->life -= DamageFunc(g_Enemy[i].power, g_Enemy[i].target->diffend);
		g_Enemy[i].atFrameCount = 0;
		g_Enemy[i].attackUse = TRUE;
	}
}

void EnemyMove(int i)
{
	//�ߐڃL�����ł��u���b�N���Ȃ�ړ����Ȃ�
	if (g_Enemy[i].type == Proximity && g_Enemy[i].blocked)
		return;

	int tbl = g_Enemy[i].nowTbl;
	//�G�l�~�[�̈ړ����x�����ɍ��W�X�V
	XMVECTOR v1 = XMLoadFloat3(&moveTbl[tbl + 1].start) - XMLoadFloat3(&moveTbl[tbl].start);
	XMVECTOR nor = XMVector3Normalize(v1);
	XMStoreFloat3(&g_Enemy[i].pos, XMLoadFloat3(&g_Enemy[i].pos) + nor * g_Enemy[i].spd);

	//�f�[�^�̈ړ��ʂƌ��݂̈ړ��ʂ�float�l�ɕϊ����Ĕ�r
	XMFLOAT3 countData;
	XMStoreFloat3(&countData, v1);
	XMFLOAT3 countMove;
	XMStoreFloat3(&countMove, nor * g_Enemy[i].spd);
	float moveMax = fabsf(countData.x) + fabsf(countData.y) + fabsf(countData.z);
	g_Enemy[i].moveCount += fabsf(countMove.x) + fabsf(countMove.y) + fabsf(countMove.z);
	//�ړ��ʂ𒴂��Ă����玟�̃f�[�^�e�[�u����
	if (g_Enemy[i].moveCount >= moveMax)
	{
		g_Enemy[i].nowTbl++;
		g_Enemy[i].moveCount = 0.0f;
	}

	if (g_Enemy[i].nowTbl >= g_Enemy[i].moveTblSize - 1)
		g_Enemy[i].use = FALSE;
}


//����A���̃X�e�[�g�J�ڕ��@�͉������L�����B�ߋ����L�����͉~�߂����Ꮼ��������
int StateCheck(int i)
{
	//�U�����Ȃ�X�e�[�g�͂����ł͕ύX���Ȃ�
	if (g_Enemy[i].state == Attack)return Attack;

	int ans = Move;			//�f�t�H���g�͈ړ�
	PLAYER *player = GetPlayer();
	if (!CollisionBC(g_Enemy[i].pos, player->pos, 100.0f, 1.0f))return ans;

	g_Enemy[i].atCount++;
	if (g_Enemy[i].atCount >= g_Enemy[i].atInterval)
	{
		ans = Attack;
		g_Enemy[i].atCount = 0;
	}
	return ans;
}

//�G�L�����̍U���^�[�Q�b�g����B��ԋ߂��G�������čU������
void CheckTarget(int i)
{
	//�U�����ł͂Ȃ��A���̓u���b�N����Ă���Ȃ炱���Ń^�[�Q�b�g����͂��Ȃ�
	if (g_Enemy[i].state != Attack || g_Enemy[i].blocked != FALSE)return;

	PLAYER *player = GetPlayer();
	float cmp = 0.0f;;
	for (int k = 0; k < MAX_PLAYER; k++)
	{
		XMVECTOR v1 = XMLoadFloat3(&g_Enemy[i].pos) - XMLoadFloat3(&player[k].pos);
		XMFLOAT3 countData;
		XMStoreFloat3(&countData, v1);
		float dist = fabsf(countData.x) + fabsf(countData.y) + fabsf(countData.z);
		if (dist > cmp)
		{
			cmp = dist;
			g_Enemy[i].target = &player[k];
		}
	}
}

int GetEnemyNum(void)
{
	return enemyNum;
}