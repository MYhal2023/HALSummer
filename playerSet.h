//=============================================================================
//
// �v���C���[�Z�b�g���� [playerSet.h]
// Author : 
//
//=============================================================================
#pragma once
#include "main.h"
#include "model.h"
#include "input.h"
#include "fieldchip.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
enum PLAYER_CHAR
{
	ID_MainPlayer,
	ID_Neutro,
};
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void UninitPlayerSet(void);
void UpdatePlayerSet(void);
void DrawPlayerSet(void);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//�v���C���[�̃X�e�[�^�X��ۑ��������
class PlayerStatus
{
public:
	DX11_MODEL			model;		// ���f�����
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// ���f���̐F
	XMFLOAT3			scl;	//�Z�b�g�����̍��W
	float				size;		// �U���͈͂̑傫��
	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
	INTERPOLATION_DATA	*tbl_adrA;	// �U���A�j���f�[�^�̃e�[�u���擪�A�h���X
	INTERPOLATION_DATA	*tbl_adrM;	// �ҋ@�A�j���f�[�^�̃e�[�u���擪�A�h���X

	int					tbl_sizeA;	// �o�^�����e�[�u���̃��R�[�h����
	int					tbl_sizeM;	// �o�^�����e�[�u���̃��R�[�h����
	float				move_time;	// ���s����
	int					partsNum;
	int					startNum;	// �p�[�c�̓Y�����J�n�ԍ��B�����ۑ����Ă����Γ���̃p�[�c����������������
	int					atFrame;		//���ۂɃ_���[�W������t���[���^�C�~���O
	int				life;			//�v���C���[HP
	int				lifeMax;		//�v���C���[�ő�HP
	int				power;
	int				diffend;
	int				blockNum;		//�u���b�N���Ă��鐔
	int				blockMax;		//�u���b�N�\��
	int				charID;			//���̃f�[�^�ɓn���Ƃ��̃L�������ʗp�ϐ�
	int				charType;		//�ߐڃL�������������L�������BCHIP_TYPE�Ɠ�������������
	BOOL			use;			//�����������̘g���g���Ă���̂�
};

//�v���C���[�ɕt������p�[�c����ۑ��������
class PlayerPartsStatus
{
public:
	DX11_MODEL			model;		// ���f�����
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// ���f���̐F
	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
	INTERPOLATION_DATA	*tbl_adrA;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
	INTERPOLATION_DATA	*tbl_adrM;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
	int					tbl_sizeA;	// �o�^�����e�[�u���̃��R�[�h����
	int					tbl_sizeM;	// �o�^�����e�[�u���̃��R�[�h����
	float				move_time;	// ���s����
};

class PlayerSet
{
public:
	XMFLOAT3 setRot;	//�Z�b�g�����̍��W
	BOOL use[MAX_PLAYER_SET];			//���̕Ґ��ԍ��͗L�����ۂ�
	int setPlayer;	//�Z�b�g����v���C���[�ԍ��e�[�u��(�Y�����H)
	int setCharID;	//�Z�b�g���悤�Ƃ��Ă�L���������Ȃ̂��H
	PlayerStatus *setPlayerNum;
	PlayerPartsStatus *setPlayerParts;
	XMFLOAT3 setPos;	//�Z�b�g�����̍��W
	BOOL setMode;		//�Z�b�g���[�h�Ɉڍs�������ۂ�
};
HRESULT InitPlayerSet(void);
void PlayerSetMap(void);
void SetModeChange(int i);
void SetPosition(void);
void SetPlayerInfo(PlayerStatus* member, PlayerPartsStatus* memberParts);
BOOL CheckPlayerAble(PlayerStatus *member);
PlayerSet *GetSetPos(void);