//=============================================================================
//
// �o�g�����̃v���C���[���� [player.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "model.h"
#include "game.h"
#include "player.h"
#include "shadow.h"
#include "light.h"
#include "meshfield.h"
#include "collision.h"
#include "time.h"
#include "enemy.h"
#include "base.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VALUE_MOVE			(2.0f)							// �ړ���
#define	VALUE_AT_MOVE		(4.0f)							// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// ��]��

#define PLAYER_SHADOW_SIZE	(1.0f)							// �e�̑傫��
#define PLAYER_OFFSET_Y		(5.0f)							// �v���C���[�̑��������킹��
#define PLAYER_OFFSET_Z		(-300.0f)							// �v���C���[�̑��������킹��
#define PLAYER_LIFE			(100)								// �v���C���[�̃��C�t

#define PLAYER_PARTS_MAX	(1)								// �v���C���[�̃p�[�c�̐�
#define PLAYER_AT_FLAME		(30.0f)							// �v���C���[�̍U���t���[��
#define PLAYER_INVINC_FLAME	(120.0f)						// �v���C���[���G�t���[��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static PLAYER		g_Player[MAX_PLAYER];						// �v���C���[
static PlayerParts	g_Parts[MAX_PLAYER * 2];						// �v���C���[
static BOOL			g_Load = FALSE;
static int			playerNum;
static int			partsNum;
// �K�w�A�j���[�V�����f�[�^
static INTERPOLATION_DATA neutro_Attack[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 10, },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	   XMFLOAT3(2.0f, 2.0f, 2.0f), 50 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(0.8f, 1.0f, 1.0f), 30 },
};



//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		//LoadModel(MODEL_PLAYER, &g_Player[i].model);
		//// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		//GetModelDiffuse(&g_Player[i].model, &g_Player[i].diffuse[0]);

		g_Player[i].pos = { 200.0f, PLAYER_OFFSET_Y, 100.0f };
		g_Player[i].rot = { 0.0f, -XM_PI * 0.5f, 0.0f };
		g_Player[i].scl = { 0.8f, 1.0f, 1.0f };

		g_Player[i].size = PLAYER_SIZE;	// �����蔻��̑傫��
		g_Player[i].use = FALSE;
		g_Player[i].life = PLAYER_LIFE;
		g_Player[i].lifeMax = g_Player[i].life;
		g_Player[i].power = 5;
		g_Player[i].diffend = 3;
		g_Player[i].attack = FALSE;
		g_Player[i].attackUse = FALSE;
		g_Player[i].blockMax = 2;
		g_Player[i].blockNum = 0;

		// �K�w�A�j���[�V�����p�̏���������
		g_Player[i].parent = NULL;			// �{�́i�e�j�Ȃ̂�NULL������
	}
	g_Load = TRUE;
	playerNum = 0;
	return S_OK;

}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	if (g_Load == FALSE) return;

	for (int i = 0; i < MAX_PLAYER; i++)
	{
		// ���f���̉������
		if (g_Player[i].load)
		{
			UnloadModel(&g_Player[i].model);
			g_Player[i].load = FALSE;
		}
	}
	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	BlockEnemy();	//�u���b�N�����X�V

	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (g_Player[i].use != TRUE)continue;

		g_Player[i].StateCheck(i);
		CheckEnemyTarget(i);
		switch (g_Player[i].state)
		{
		case Standby:
			break;

		case Deffend:
			PlayerInterPoration(i);
			break;
		}

	}
#ifdef _DEBUG
	PrintDebugProc("�v���C���[�̗�:%d\n", g_Player[0].life);
	PrintDebugProc("�v���C���[�̗�:%d\n", g_Player[1].life);
	PrintDebugProc("�v���C���[���:%d\n", g_Player[1].state);
	PrintDebugProc("�v���C���[�^�[�Q�b�g:%d\n", g_Player[1].target);
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (g_Player[i].use != TRUE)continue;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Player[i].scl.x, g_Player[i].scl.y, g_Player[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Player[i].rot.x, g_Player[i].rot.y + XM_PI, g_Player[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Player[i].pos.x, g_Player[i].pos.y, g_Player[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Player[i].mtxWorld, mtxWorld);

		// ���f���`��
		DrawModel(&g_Player[i].model);

		if (g_Player[i].partsNum == 0)continue;

		// �p�[�c�̊K�w�A�j���[�V����
		for (int k = g_Player[i].startNum; k < g_Player[i].startNum + g_Player[i].partsNum; k++)
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
				// g_Player[i].mtxWorld���w���Ă���
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

//�v���C���[�̐��`��ԁB�U�����[�V�������ɂ����ɗ�����_���[�W�������s��
void PlayerInterPoration(int i)
{
	if (g_Player[i].tbl_adr == NULL)return;	// ���`��Ԃ����s����H

	//
	// ���`��Ԃ̏���
	// �ړ�����
	int		index = (int)g_Player[i].move_time;
	float	time = g_Player[i].move_time - index;
	int		size = g_Player[i].tbl_size;

	float dt = 1.0f / g_Player[i].tbl_adr[index].frame;	// 1�t���[���Ői�߂鎞��
	g_Player[i].move_time += dt;							// �A�j���[�V�����̍��v���Ԃɑ���

	if (index > (size - 2))	// �S�[�����I�[�o�[���Ă�����A�f�[�^���ŏ��ɖ߂��čU�����I��
	{
		g_Player[i].move_time = 0.0f;
		index = 0;
		g_Player[i].atCount = 0;
		g_Player[i].state = Standby;
		g_Player[i].attackUse = FALSE;
	}

	// ���W�����߂�	X = StartX + (EndX - StartX) * ���̎���
	XMVECTOR p1 = XMLoadFloat3(&g_Player[i].tbl_adr[index + 1].pos);	// ���̏ꏊ
	XMVECTOR p0 = XMLoadFloat3(&g_Player[i].tbl_adr[index + 0].pos);	// ���݂̏ꏊ
	XMVECTOR vec = p1 - p0;
	XMStoreFloat3(&g_Player[i].pos, XMLoadFloat3(&g_Player[i].pos) + p0 + vec * time);

	// ��]�����߂�	R = StartX + (EndX - StartX) * ���̎���
	XMVECTOR r1 = XMLoadFloat3(&g_Player[i].tbl_adr[index + 1].rot);	// ���̊p�x
	XMVECTOR r0 = XMLoadFloat3(&g_Player[i].tbl_adr[index + 0].rot);	// ���݂̊p�x
	XMVECTOR rot = r1 - r0;
	XMStoreFloat3(&g_Player[i].rot, XMLoadFloat3(&g_Player[i].rot) + r0 + rot * time);

	// scale�����߂� S = StartX + (EndX - StartX) * ���̎���
	XMVECTOR s1 = XMLoadFloat3(&g_Player[i].tbl_adr[index + 1].scl);	// ����Scale
	XMVECTOR s0 = XMLoadFloat3(&g_Player[i].tbl_adr[index + 0].scl);	// ���݂�Scale
	XMVECTOR scl = s1 - s0;
	XMStoreFloat3(&g_Player[i].scl, s0 + scl * time);

	//��������U������
	if (g_Player[i].target == 99 || g_Player[i].attackUse == TRUE)return;	//�Z�b�g���Ă��Ȃ��A�Z�b�g����K�v���Ȃ��U�������邩���H
	g_Player[i].atFrameCount++;
	ENEMY *enemy = GetEnemy();
	//�U���t���[���ɒB������A�_���[�W�v�Z�֐������Ƀ^�[�Q�b�g�Ƀ_���[�W
	if (g_Player[i].atFrameCount >= g_Player[i].atFrame) {
		enemy[g_Player[i].target].life -= DamageFunc(g_Player[i].power, enemy[g_Player[i].target].diffend);
		g_Player[i].atFrameCount = 0;
		g_Player[i].attackUse = TRUE;
	}
}

void BlockEnemy(void)
{
	ENEMY *enemy = GetEnemy();
	for(int k = 0; k < MAX_ENEMY; k++)
		enemy[k].blocked = FALSE;
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (g_Player[i].use != TRUE)continue;
		g_Player[i].blockNum = 0;	//�u���b�N����0�Ƀ��Z�b�g
		for (int k = 0; k < MAX_ENEMY; k++)
		{
			//�����u���b�N�o���Ȃ��A�u���b�N�s�\���g���ĂȂ��A���������ߐڂ��Ă��Ȃ��Ȃ�X�L�b�v
			//�u���b�N���邽�߂̑O������͏�ցA�u���b�N���邽�߂̏����͉��֋L��
			if (g_Player[i].blockNum >= g_Player[i].blockMax ||
				enemy[k].type != Proximity ||
				enemy[k].use != TRUE)continue;
			if (!CollisionBC(g_Player[i].pos, enemy[k].pos, 10.0f, 10.0f))continue;

			//�����ŃG�l�~�[���u���b�N��Ԃ֕ύX����B�U�����������
			g_Player[i].blockNum++;
			enemy[k].blocked = TRUE;
			enemy[k].target = &g_Player[i];
		}
	}
}

//�G�L�����̍U���^�[�Q�b�g����B��ԋ߂��G�������čU������
void CheckEnemyTarget(int i)
{
	//�U�����ł͂Ȃ��Ȃ炱���Ń^�[�Q�b�g����͂��Ȃ�
	if (g_Player[i].state != Deffend) {
		g_Player[i].target = 99;
		return;
	}
	ENEMY *enemy = GetEnemy();
	Base *base = GetBase();
	float cmp = 0.0f;;
	for (int k = 0; k < g_Player[i].count; k++)
	{
		if (g_Player[i].targetable[k] == 99)continue;
		for (int j = 0; j < base->baseNum; j++)
		{
			XMVECTOR v1 = XMLoadFloat3(&base->pos[j]) - XMLoadFloat3(&enemy[g_Player[i].targetable[k]].pos);
			XMFLOAT3 countData;
			XMStoreFloat3(&countData, v1);
			float dist = fabsf(countData.x) + fabsf(countData.y) + fabsf(countData.z);
			if (dist > cmp)
			{
				cmp = dist;
				g_Player[i].target = g_Player[i].targetable[k];	//�G�l�~�[�̔z��ԍ��Ŏ��ʁB�|�C���^�[�œn���������ǁA���݂��̍\���̂Ƀ|�C���^�[�����o�ϐ�������Ɠ{����c
			}
		}
	}
}

void SetPlayer(XMFLOAT3 pos)
{
	LoadModel(MODEL_PLAYER, &g_Player[playerNum].model);
	// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
	GetModelDiffuse(&g_Player[playerNum].model, &g_Player[playerNum].diffuse[0]);

	g_Player[playerNum].load = TRUE;

	g_Player[playerNum].pos = pos;
	g_Player[playerNum].rot = { 0.0f, XM_PI * -0.5f, 0.0f };
	g_Player[playerNum].scl = { 0.8f, 1.0f, 1.0f };

	g_Player[playerNum].size = PLAYER_SIZE;	// �����蔻��̑傫��
	g_Player[playerNum].use = TRUE;
	g_Player[playerNum].life = PLAYER_LIFE;
	g_Player[playerNum].lifeMax = g_Player[playerNum].life;
	g_Player[playerNum].power = 5;
	g_Player[playerNum].diffend = 3;
	g_Player[playerNum].attack = FALSE;
	g_Player[playerNum].attackUse = FALSE;
	g_Player[playerNum].blockMax = 2;
	g_Player[playerNum].blockNum = 0;
	g_Player[playerNum].target = 99;
	for (int i = 0; i < MAX_TARGET; i++)
		g_Player[playerNum].targetable[i] = 99;
	g_Player[playerNum].count = 0;
	g_Player[playerNum].startNum = partsNum;
	g_Player[playerNum].partsNum = 0;

	// �K�w�A�j���[�V�����p�̏���������
	g_Player[playerNum].parent = NULL;			// �{�́i�e�j�Ȃ̂�NULL������
	playerNum++;

	//�p�[�c�̏���������
	for (int k = 0; k < g_Player[playerNum].partsNum; k++)
	{
		LoadModel(MODEL_GRAPE_PARTS, &g_Parts[partsNum].model);
		// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);
		g_Parts[partsNum].load = TRUE;

		g_Parts[partsNum].pos = { 20.0f, 10.0f, 0.0f };		// �|���S���̈ʒu
		g_Parts[partsNum].rot = { 0.0f, 0.0f, 0.0f };		// �|���S���̌���(��])
		g_Parts[partsNum].scl = { 1.0f, 1.0f, 1.0f };		// �|���S���̑傫��(�X�P�[��)

		// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
		g_Parts[partsNum].tbl_adr = NULL;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
		g_Parts[partsNum].tbl_size = 0;	// �o�^�����e�[�u���̃��R�[�h����
		g_Parts[partsNum].move_time = 0;	// ���s����
		g_Parts[partsNum].parent = &g_Player[playerNum];	// �������e�Ȃ�NULL�A�������q���Ȃ�e��enemy�A�h���X
		partsNum++;
	}

}

void SetNeutrophils(XMFLOAT3 pos)
{
	LoadModel(MODEL_NEUTROPHILS, &g_Player[playerNum].model);
	// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
	GetModelDiffuse(&g_Player[playerNum].model, &g_Player[playerNum].diffuse[0]);

	g_Player[playerNum].load = TRUE;

	g_Player[playerNum].pos = pos;
	g_Player[playerNum].rot = { 0.0f, XM_PI * 0.5f, 0.0f };
	g_Player[playerNum].scl = { 0.8f, 1.0f, 1.0f };

	g_Player[playerNum].size = PLAYER_SIZE;	// �����蔻��̑傫��
	g_Player[playerNum].use = TRUE;
	g_Player[playerNum].life = PLAYER_LIFE;
	g_Player[playerNum].lifeMax = g_Player[playerNum].life;
	g_Player[playerNum].power = 5;
	g_Player[playerNum].diffend = 3;
	g_Player[playerNum].attack = FALSE;
	g_Player[playerNum].attackUse = FALSE;
	g_Player[playerNum].blockMax = 2;
	g_Player[playerNum].blockNum = 0;
	g_Player[playerNum].target = 99;
	for (int i = 0; i < MAX_TARGET; i++)
		g_Player[playerNum].targetable[i] = 99;
	g_Player[playerNum].count = 0;
	g_Player[playerNum].atCount = 0;
	g_Player[playerNum].atFrameCount = 0;
	g_Player[playerNum].atFrame = 20;
	g_Player[playerNum].startNum = partsNum;
	g_Player[playerNum].partsNum = 0;
	g_Player[playerNum].tbl_adr = neutro_Attack;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
	g_Player[playerNum].tbl_size = sizeof(neutro_Attack) / sizeof(INTERPOLATION_DATA);	// �o�^�����e�[�u���̃��R�[�h����
	g_Player[playerNum].move_time = 0.0f;	// ���s����

	// �K�w�A�j���[�V�����p�̏���������
	g_Player[playerNum].parent = NULL;			// �{�́i�e�j�Ȃ̂�NULL������
	playerNum++;

	//�p�[�c�̏���������
	for (int k = 0; k < g_Player[playerNum].partsNum; k++)
	{
		LoadModel(MODEL_GRAPE_PARTS, &g_Parts[partsNum].model);
		// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);
		g_Parts[partsNum].load = TRUE;

		g_Parts[partsNum].pos = { 20.0f, 10.0f, 0.0f };		// �|���S���̈ʒu
		g_Parts[partsNum].rot = { 0.0f, 0.0f, 0.0f };		// �|���S���̌���(��])
		g_Parts[partsNum].scl = { 1.0f, 1.0f, 1.0f };		// �|���S���̑傫��(�X�P�[��)

		// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
		g_Parts[partsNum].tbl_adr = NULL;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
		g_Parts[partsNum].tbl_size = 0;	// �o�^�����e�[�u���̃��R�[�h����
		g_Parts[partsNum].move_time = 0;	// ���s����
		g_Parts[partsNum].parent = &g_Player[playerNum];	// �������e�Ȃ�NULL�A�������q���Ȃ�e��enemy�A�h���X
		partsNum++;
	}

}

int GetPlayerNum(void)
{
	return playerNum;
}
//=============================================================================
// �v���C���[�����擾
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player[0];
}

void PLAYER::StateCheck(int i)
{
	g_Player[i].state = Standby;	//�Ƃ肠�����ҋ@��ԂɃZ�b�g
	ENEMY *enemy = GetEnemy();
	g_Player[i].count = 0;
	for (int k = 0; k < MAX_TARGET; k++)
	{
		g_Player[i].targetable[k] = 99;
	}
	for (int k = 0; k < GetEnemyNum(); k++)
	{
		//�v���C���[�̍U���͈͂�1�̂ł��G������Ȃ�΍U�������ɓ���
		if (CollisionBC(g_Player[i].pos, enemy[k].pos, g_Player[i].size, 10.0f))
		{
			g_Player[i].state = Deffend;
			g_Player[i].targetable[g_Player[i].count] = k;
			g_Player[i].count++;
		}
	}
}
