//=============================================================================
//
// ���f������ [enemy.h]
// Author : 
//
//=============================================================================
#pragma once
#ifndef ENEMY_H_
#define ENEMY_H_
#include "unitdata.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_GRAPE		"data/MODEL/grape0.obj"			// �ǂݍ��ރ��f����
#define	MODEL_GRAPE_PARTS001		"data/MODEL/grape_parts001.obj"			// �ǂݍ��ރ��f����
#define	MODEL_GRAPE_PARTS002		"data/MODEL/grape_parts002.obj"			// �ǂݍ��ރ��f����
#define	MODEL_GRAPE_PARTS003		"data/MODEL/grape_parts003.obj"			// �ǂݍ��ރ��f����
#define MAX_ENEMY		(5)					// �G�l�~�[�̐�

#define	ENEMY_SIZE		(5.0f)				// �����蔻��̑傫��

enum ENEMY_STATE
{
	Move,
	Attack,
};

enum ENEMY_TYPE
{
	Proximity,	//�ߐڍU���L����
	Remote,		//�������U���L����
	Abnormal,	//����ȃL�����B���j�[�N�{�X�Ƃ��Ɏg��
};
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct MOVERINE
{
	XMFLOAT3 start;
};

struct ENEMY:public Unit
{
public:
	float				move_time;	// ���s����
	int					moveTblSize;
	int					nowTbl;
	// �e�́ANULL�A�q���͐e�̃A�h���X������
	ENEMY				*parent;	// �������e�Ȃ�NULL�A�������q���Ȃ�e��enemy�A�h���X
	MOVERINE			*moveData;
	PLAYER				*target;
	float				start;		//�G�l�~�[���o��������X�^�[�g���ԁB���ꂪ�[���ȉ��ɂȂ������A���`��Ԃ��J�n����
	float				spd;		//�G�l�~�[�̈ړ����x
	float				moveCount;	//�ړ�������
	int					atFrame;		//���ۂɃ_���[�W������t���[���^�C�~���O
	int					atFrameCount;		//�t���[���^�C�~���O�̃J�E���g�ϐ�
	BOOL				blocked;		//�u���b�N����Ă��邩�ۂ�
	int					type;
	int					atInterval;
};

struct EnemyParts
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
	ENEMY				*parent;	// �������e�Ȃ�NULL�A�������q���Ȃ�e��enemy�A�h���X
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

ENEMY *GetEnemy(void);
void SetGrape(float time);
int StateCheck(int i);
void SetEnemyTime(int i);
void EnemyInterPoration(int i);
void EnemyMove(int i);
void CheckTarget(int i);
int GetEnemyNum(void);

#endif