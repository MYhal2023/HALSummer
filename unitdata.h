#pragma once
#include "main.h"
#include "model.h"
class Unit
{
public:
	XMFLOAT3			pos;		// �|���S���̈ʒu
	XMFLOAT3			rot;		// �|���S���̌���(��])
	XMFLOAT3			scl;		// �|���S���̑傫��(�X�P�[��)

	XMFLOAT4X4			mtxWorld;	// ���[���h�}�g���b�N�X

	BOOL				load;
	DX11_MODEL			model;		// ���f�����
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// ���f���̐F

	float				dir;		// ����
	float				size;		// �����蔻��̑傫��
	BOOL				use;

	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
	INTERPOLATION_DATA	*tbl_adr;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X

	int					tbl_size;	// �o�^�����e�[�u���̃��R�[�h����
	float				move_time;	// ���s����
	int					partsNum;
	int					startNum;	// �p�[�c�̓Y�����J�n�ԍ��B�����ۑ����Ă����Γ���̃p�[�c����������������
	int					atCount;
	int					atFrameCount;	//�t���[���^�C�~���O�̃J�E���g�ϐ�
	int					atFrame;		//���ۂɃ_���[�W������t���[���^�C�~���O

	//�U��
	BOOL			attack;			//�U�������ۂ�
	BOOL			attackUse;		//���̍U�����肪�g�p���ꂽ��

	int				state;
	int				life;			//�v���C���[HP
	int				lifeMax;		//�v���C���[�ő�HP
	int				power;
	int				diffend;
};