//=============================================================================
//
// ���f������ [fieldchip.h]
// Author : 
//
//=============================================================================
#pragma once
#include "model.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_FIELD001		"data/MODEL/shortrange2.obj"			// �ǂݍ��ރ��f����
#define	MODEL_FIELD002		"data/MODEL/longrange2.obj"			// �ǂݍ��ރ��f����
#define MAX_CHIP_NUM		(2)					// �t�B�[���h�`�b�v���
#define MAX_CHIP_WIDTH		(8)					// ���̃Z�b�g��
#define MAX_CHIP_HEIGHT		(4)					// �c�̃Z�b�g��

#define	CHIP_SIZE		(30.0f)					// �`�b�v�T�C�Y�̑傫��

enum CHIP_VAL
{
	ROW,
	HIGH,
	MAX_VAL
};
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct MAP_CHIP
{
	XMFLOAT3			pos;		// �|���S���̈ʒu
	XMFLOAT3			rot;		// �|���S���̌���(��])
	XMFLOAT3			scl;		// �|���S���̑傫��(�X�P�[��)

	XMFLOAT4X4			mtxWorld;	// ���[���h�}�g���b�N�X

	BOOL				load;
	DX11_MODEL			model;		// ���f�����
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// ���f���̐F

	float				size;		// �����蔻��̑傫��
	BOOL				use;
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMapChip(void);
void UninitMapChip(void);
void UpdateMapChip(void);
void DrawMapChip(void);

MAP_CHIP *GetMapChip(void);