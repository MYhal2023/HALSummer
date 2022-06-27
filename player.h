//=============================================================================
//
// ���f������ [player.h]
// Author : 
//
//=============================================================================
#pragma once
#ifndef PLAYER_H_
#define PLAYER_H_
#include "unitdata.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_PLAYER		"data/MODEL/dendritic_cells.obj"			// �ǂݍ��ރ��f����
#define	MODEL_NEUTROPHILS	"data/MODEL/neutrophils.obj"			// �ǂݍ��ރ��f����
#define MAX_PLAYER		(2)					// �v���C���[�̐�
#define MAX_TARGET		(10)					// �v���C���[�̐�

#define	PLAYER_SIZE		(10.0f)				// �����蔻��̑傫��

enum PLAYER_STATE
{
	Standby,	//�ҋ@��
	Deffend,	//�}����
};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
class PLAYER:public Unit
{
public:
	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
	PLAYER				*parent;	// �������e�Ȃ�NULL�A�������q���Ȃ�e��enemy�A�h���X

	int				blockNum;		//�u���b�N���Ă��鐔
	int				blockMax;		//�u���b�N�\��
	int				target;			//�^�[�Q�b�g������99��
	int				targetable[MAX_TARGET];	//�^�[�Q�b�g�\�ȓG�̔z��Y������ۑ����Ă������́B���̃��X�g�̓�����Atarget�ϐ��Ɉ����n�����
	int				count;					//���݃^�[�Q�b�g�ɂ��Ă��鐔
	void StateCheck(int i);			//�z��̓Y�����������Ɏ����Ă���
};

struct PlayerParts
{
	XMFLOAT3			pos;		// �|���S���̈ʒu
	XMFLOAT3			rot;		// �|���S���̌���(��])
	XMFLOAT3			scl;		// �|���S���̑傫��(�X�P�[��)

	XMFLOAT4X4			mtxWorld;	// ���[���h�}�g���b�N�X

	BOOL				load;
	DX11_MODEL			model;		// ���f�����
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// ���f���̐F
	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
	INTERPOLATION_DATA	*tbl_adr;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
	int					tbl_size;	// �o�^�����e�[�u���̃��R�[�h����
	float				move_time;	// ���s����
	PLAYER				*parent;	// �������e�Ȃ�NULL�A�������q���Ȃ�e��enemy�A�h���X
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

void BlockEnemy(void);
PLAYER *GetPlayer(void);
void PlayerInterPoration(int i);
void CheckEnemyTarget(int i);
void SetPlayer(XMFLOAT3 pos);
void SetNeutrophils(XMFLOAT3 pos);
int GetPlayerNum(void);

#endif