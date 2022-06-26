//=============================================================================
//
// �t�B�[���h���f������ [fieldchip.cpp]
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
#include "fieldchip.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static MAP_CHIP		g_MapChip[MAX_CHIP_NUM];						// �v���C���[
static MAP_CHIP		g_MapSet[MAX_CHIP_HEIGHT][MAX_CHIP_WIDTH + 1];
static BOOL			g_Load = FALSE;

//�����Ŏg�p����}�b�v�`�b�v�i���o�[��ݒ�
int g_DebugMap[MAX_CHIP_HEIGHT][MAX_CHIP_WIDTH + 1]
{
	{1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,0},
	{1,1,1,1,1,1,1,1},
};
//=============================================================================
// ����������
//=============================================================================
HRESULT InitMapChip(void)
{
	for (int i = 0; i < MAX_CHIP_NUM; i++)
	{
		switch (i)
		{
		case ROW:
			LoadModel(MODEL_FIELD001, &g_MapChip[i].model);
			break;
		case HIGH:
			LoadModel(MODEL_FIELD002, &g_MapChip[i].model);
			break;
		case MAX_VAL:
			break;
		}
		// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		GetModelDiffuse(&g_MapChip[i].model, &g_MapChip[i].diffuse[0]);

		g_MapChip[i].load = TRUE;

		g_MapChip[i].pos = { 0.0f, 0.0f, 0.0f };
		g_MapChip[i].rot = { 0.0f, 0.0f, 0.0f };
		g_MapChip[i].scl = { 1.0f, 1.0f, 1.0f };

		g_MapChip[i].size = CHIP_SIZE;	// �����蔻��̑傫��
	}

	for (int i = 0; i < MAX_CHIP_HEIGHT; i++)
	{
		for (int k = 0; k < MAX_CHIP_WIDTH; k++)
		{
			g_MapSet[i][k].pos = { 0.0f + CHIP_SIZE * k, -1.0f,  0.0f + CHIP_SIZE * i };
			g_MapSet[i][k].rot = { 0.0f, 0.0f, 0.0f };
			g_MapSet[i][k].scl = { 1.7f, 1.0f, 1.7f };
		}
	}
	g_Load = TRUE;
	return S_OK;

}

//=============================================================================
// �I������
//=============================================================================
void UninitMapChip(void)
{
	if (g_Load == FALSE) return;

	for (int i = 0; i < MAX_CHIP_NUM; i++)
	{
		// ���f���̉������
		if (g_MapChip[i].load)
		{
			UnloadModel(&g_MapChip[i].model);
			g_MapChip[i].load = FALSE;
		}
	}
	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMapChip(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMapChip(void)
{
	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	for (int i = 0; i < MAX_CHIP_HEIGHT; i++)
	{
		for (int k = 0; k < MAX_CHIP_WIDTH; k++)
		{
			int m = g_DebugMap[i][k];
			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_MapSet[i][k].scl.x, g_MapSet[i][k].scl.y, g_MapSet[i][k].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(g_MapSet[i][k].rot.x, g_MapSet[i][k].rot.y + XM_PI, g_MapSet[i][k].rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_MapSet[i][k].pos.x, g_MapSet[i][k].pos.y, g_MapSet[i][k].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_MapSet[i][k].mtxWorld, mtxWorld);

			// ���f���`��
			//�����������O�ɗp�ӂ����}�b�v�`�b�v�i���o�[���Q��
			DrawModel(&g_MapChip[m].model);
		}
	}


	//// �p�[�c�̊K�w�A�j���[�V����
	//for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	//{
	//	// ���[���h�}�g���b�N�X�̏�����
	//	mtxWorld = XMMatrixIdentity();

	//	// �X�P�[���𔽉f
	//	mtxScl = XMMatrixScaling(g_Parts[i].scl.x, g_Parts[i].scl.y, g_Parts[i].scl.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	//	// ��]�𔽉f
	//	mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i].rot.x, g_Parts[i].rot.y, g_Parts[i].rot.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	//	// �ړ��𔽉f
	//	mtxTranslate = XMMatrixTranslation(g_Parts[i].pos.x, g_Parts[i].pos.y, g_Parts[i].pos.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	//	if (g_Parts[i].parent != NULL)	// �q����������e�ƌ�������
	//	{
	//		mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i].parent->mtxWorld));
	//		// ��
	//		// g_MapChip[i].mtxWorld���w���Ă���
	//	}

	//	XMStoreFloat4x4(&g_Parts[i].mtxWorld, mtxWorld);

	//	// �g���Ă���Ȃ珈������B�����܂ŏ������Ă��闝�R�͑��̃p�[�c�����̃p�[�c���Q�Ƃ��Ă���\�������邩��B
	//	if (g_Parts[i].use == FALSE) continue;

	//	// ���[���h�}�g���b�N�X�̐ݒ�
	//	SetWorldMatrix(&mtxWorld);


	//	// ���f���`��
	//	DrawModel(&g_Parts[i].model);

	//}


	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// �v���C���[�����擾
//=============================================================================
MAP_CHIP *GetMapChip(void)
{
	return &g_MapChip[0];
}