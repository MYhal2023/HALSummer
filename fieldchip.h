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
#define	MODEL_PLAYER		"data/MODEL/slime.obj"			// �ǂݍ��ރ��f����
#define MAX_CHIP_NUM		(1)					// �t�B�[���h�`�b�v���
#define MAX_CHIP_WIDTH		(8)					// �t�B�[���h�`�b�v���
#define MAX_CHIP_HEIGHT		(4)					// �t�B�[���h�`�b�v���

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
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer(void);