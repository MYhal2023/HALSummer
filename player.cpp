//=============================================================================
//
// �o�g�����̃v���C���[���� [player.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "math.h"
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
#include "fieldchip.h"
#include "skill.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX			(1)								// �e�N�X�`���̐�
#define VAR_WIDTH			(50.0f)
#define VAR_HEIGHT			(5.0f)
#define	VALUE_MOVE			(2.0f)							// �ړ���
#define	VALUE_AT_MOVE		(4.0f)							// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// ��]��

#define PLAYER_SHADOW_SIZE	(1.0f)							// �e�̑傫��
#define PLAYER_OFFSET_Y		(5.0f)							// �v���C���[�̑��������킹��
#define PLAYER_OFFSET_Z		(-300.0f)							// �v���C���[�̑��������킹��
#define PLAYER_LIFE			(100)								// �v���C���[�̃��C�t

#define PLAYER_PARTS_MAX	(1)								// �v���C���[�̃p�[�c�̐�
#define PLAYER_AT_FLAME		(30.0f)							// �v���C���[�̍U���t���[��
#define PLAYER_SP_FLAME		(30.0f)							// �v���C���[��SP��������Ԋu
#define PLAYER_INVINC_FLAME	(120.0f)						// �v���C���[���G�t���[��
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// ���_���
static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����
static char* g_TextureName[] = {
	"data/TEXTURE/var.png",
};
static PLAYER_VAR	g_PlayerVar;
static PLAYER		g_Player[MAX_PLAYER];						// �v���C���[
static Playerliner  g_Playerline[MAX_PLAYER];					//�v���C���[�̐��`��ԃf�[�^
static PlayerParts	g_Parts[MAX_PLAYER_PARTS];					// �v���C���[
static BOOL			g_Load = FALSE;
static int			playerNum;
static int			partsNum;
// �K�w�A�j���[�V�����f�[�^
static char name[2][64];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	ID3D11Device *pDevice = GetDevice();
	MakeVertexPlayerVar();
	// �e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}

	ZeroMemory(&g_PlayerVar.material, sizeof(g_PlayerVar.material));
	g_PlayerVar.material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_PlayerVar.pos = { 0.0f, 0.0f, 0.0f };
	g_PlayerVar.rot = { XM_PI * -0.5f, 0.0f, 0.0f };
	g_PlayerVar.scl = { 1.0f, 1.0f, 1.0f };
	g_PlayerVar.pardiff = 0.0f;
	g_PlayerVar.parpolar = 0.02f;
	g_PlayerVar.load = TRUE;

	strcpy(name[0], MODEL_PLAYER);
	strcpy(name[1], MODEL_NEUTROPHILS);

	for (int i = 0; i < MAX_PLAYER; i++)
	{
		//LoadModel(MODEL_PLAYER, &g_Player[i].model);
		//// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		//GetModelDiffuse(&g_Player[i].model, &g_Player[i].diffuse[0]);
		g_Player[i].load = FALSE;
		g_Player[i].pos = { 0.0f, PLAYER_OFFSET_Y, 0.0f };
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
		g_Player[i].partsNum = 0;
		g_Player[i].startNum = 0;
		g_Player[i].keyNum = 99;
		// �K�w�A�j���[�V�����p�̏���������
		g_Player[i].parent = NULL;			// �{�́i�e�j�Ȃ̂�NULL������
		g_Playerline[i].pos = { 0.0f, 0.0f, 0.0f };
		g_Playerline[i].rot = { 0.0f, 0.0f, 0.0f };
		g_Playerline[i].scl = { 0.0f, 0.0f, 0.0f };
	}
	
	for (int i = 0; i < MAX_PLAYER_PARTS; i++)
	{
		g_Parts[i].load = FALSE;
	}
	
	g_Load = TRUE;
	playerNum = 0;
	partsNum = 0;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	if (g_Load == FALSE) return;

	name[0][0] = '0';
	name[1][0] = '0';
	// ���_�o�b�t�@�̉��
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}


	for (int i = 0; i < MAX_PLAYER; i++)
	{
		// ���f���̉������
		if (g_Player[i].load)
		{
			UnloadModel(&g_Player[i].model);
			g_Player[i].load = FALSE;
		}
	}
	for (int i = 0; i < MAX_PLAYER_PARTS; i++)
	{
		// ���f���̉������
		if (g_Player[i].load)
		{
			UnloadModel(&g_Parts[i].model);
			g_Parts[i].load = FALSE;
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

		//�̗͂������Ȃ����L�����̏����B��������B�����̃}�b�v�`�b�v���J����
		if (g_Player[i].life <= 0 && g_Player[i].use) {
			g_Player[i].use = FALSE;
			for (int k = g_Player[i].startNum; k < g_Player[i].startNum + g_Player[i].partsNum; k++)
			{
				g_Parts[k].use = FALSE;
			}
				int x = (int)(g_Player[i].pos.x / CHIP_SIZE);
			int z = (int)(g_Player[i].pos.z / CHIP_SIZE);
			SetMapChipUse(FALSE, z, x);
			continue;
		}
		IncreaseSP(i);
		int oldState = g_Player[i].state;
		g_Player[i].StateCheck(i);
		if (oldState != g_Player[i].state)
		{
			g_Player[i].move_time = 0.0f;
			g_Player[i].atCount = 0;
			g_Playerline[i].pos = { 0.0f, 0.0f, 0.0f };
			g_Playerline[i].rot = { 0.0f, 0.0f, 0.0f };
			g_Playerline[i].scl = { 0.0f, 0.0f, 0.0f };
			for (int k = g_Player[i].startNum; k < g_Player[i].startNum + g_Player[i].partsNum; k++)
				g_Parts[k].move_time = 0.0f;
		}
		CheckEnemyTarget(i);
		switch (g_Player[i].state)
		{
		case Standby:
			PlayerStandLiner(i);
			break;

		case Deffend:
			PlayerInterPoration(i);
			break;
		case Skill:
			PlayerSkill(i);
			break;
		}
			
	}
#ifdef _DEBUG
	PrintDebugProc("�v���C���[���WZ:%f\n", g_Player[0].pos.z);
	PrintDebugProc("�v���C���[���WX:%f\n", g_Player[1].pos.x);
	PrintDebugProc("�v���C���[���WZ:%f\n", g_Player[1].pos.z);
	PrintDebugProc("�v���C��use:%d\n", g_Player[0].use);
	PrintDebugProc("�v���C���[�̗�:%d\n", g_Player[0].life);
	PrintDebugProc("�v���C���[�̗�:%d\n", g_Player[1].life);
	PrintDebugProc("�v���C���[���:%d\n", g_Player[1].state);
	PrintDebugProc("�v���C���[�^�[�Q�b�g:%d\n", g_Player[1].target);
	PrintDebugProc("�v���C���[SP:%d\n", g_Player[0].skillPoint);

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
		mtxScl = XMMatrixScaling(g_Player[i].scl.x + g_Playerline[i].scl.x, g_Player[i].scl.y + g_Playerline[i].scl.y, g_Player[i].scl.z + g_Playerline[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Player[i].rot.x + g_Playerline[i].rot.x, g_Player[i].rot.y + XM_PI + g_Playerline[i].rot.y, g_Player[i].rot.z + g_Playerline[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Player[i].pos.x + g_Playerline[i].pos.x, g_Player[i].pos.y + g_Playerline[i].pos.y, g_Player[i].pos.z + g_Playerline[i].pos.z);
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

//�ҋ@Ӱ��ݏ���
void PlayerStandLiner(int i)
{
	if (g_Player[i].tbl_adrM == NULL)return;	// ���`��Ԃ����s����H

//
// ���`��Ԃ̏���
// �ړ�����
	int		index = (int)g_Player[i].move_time;
	float	time = g_Player[i].move_time - index;
	int		size = g_Player[i].tbl_sizeM;

	float dt = 1.0f / g_Player[i].tbl_adrM[index].frame;	// 1�t���[���Ői�߂鎞��
	g_Player[i].move_time += dt;							// �A�j���[�V�����̍��v���Ԃɑ���

	if (index > (size - 2))	// �S�[�����I�[�o�[���Ă�����A�f�[�^���ŏ��ɖ߂��čU�����I��
	{
		g_Player[i].move_time = 0.0f;
		index = 0;
		g_Playerline[i].pos = { 0.0f, 0.0f, 0.0f };
		g_Playerline[i].rot = { 0.0f, 0.0f, 0.0f };
		g_Playerline[i].scl = { 0.0f, 0.0f, 0.0f };
	}

	// ���W�����߂�	X = StartX + (EndX - StartX) * ���̎���
	XMVECTOR p1 = XMLoadFloat3(&g_Player[i].tbl_adrM[index + 1].pos);	// ���̏ꏊ
	XMVECTOR p0 = XMLoadFloat3(&g_Player[i].tbl_adrM[index + 0].pos);	// ���݂̏ꏊ
	XMVECTOR vec = p1 - p0;
	XMStoreFloat3(&g_Playerline[i].pos, p0 + vec * time);

	// ��]�����߂�	R = StartX + (EndX - StartX) * ���̎���
	XMVECTOR r1 = XMLoadFloat3(&g_Player[i].tbl_adrM[index + 1].rot);	// ���̊p�x
	XMVECTOR r0 = XMLoadFloat3(&g_Player[i].tbl_adrM[index + 0].rot);	// ���݂̊p�x
	XMVECTOR rot = r1 - r0;
	XMStoreFloat3(&g_Playerline[i].rot, r0 + rot * time);

	// scale�����߂� S = StartX + (EndX - StartX) * ���̎���
	XMVECTOR s1 = XMLoadFloat3(&g_Player[i].tbl_adrM[index + 1].scl);	// ����Scale
	XMVECTOR s0 = XMLoadFloat3(&g_Player[i].tbl_adrM[index + 0].scl);	// ���݂�Scale
	XMVECTOR scl = s1 - s0;
	XMStoreFloat3(&g_Playerline[i].scl, s0 + scl * time);

	for (int k = g_Player[i].startNum; k < g_Player[i].startNum + g_Player[i].partsNum; k++)
	{
		if (g_Parts[k].tbl_adrM == NULL)continue;;	// ���`��Ԃ����s����H

		int		index = (int)g_Parts[k].move_time;
		float	time = g_Parts[k].move_time - index;
		int		size = g_Parts[k].tbl_sizeM;

		float dt = 1.0f / g_Parts[k].tbl_adrM[index].frame;	// 1�t���[���Ői�߂鎞��
		g_Parts[k].move_time += dt;							// �A�j���[�V�����̍��v���Ԃɑ���

		if (index > (size - 2))	// �S�[�����I�[�o�[���Ă�����A�f�[�^���ŏ��ɖ߂��čU�����I��
		{
			g_Parts[k].move_time = 0.0f;
			index = 0;
		}

		// ���W�����߂�	X = StartX + (EndX - StartX) * ���̎���
		XMVECTOR p1 = XMLoadFloat3(&g_Parts[k].tbl_adrM[index + 1].pos);	// ���̏ꏊ
		XMVECTOR p0 = XMLoadFloat3(&g_Parts[k].tbl_adrM[index + 0].pos);	// ���݂̏ꏊ
		XMVECTOR vec = p1 - p0;
		XMStoreFloat3(&g_Parts[k].pos, p0 + vec * time);

		// ��]�����߂�	R = StartX + (EndX - StartX) * ���̎���
		XMVECTOR r1 = XMLoadFloat3(&g_Parts[k].tbl_adrM[index + 1].rot);	// ���̊p�x
		XMVECTOR r0 = XMLoadFloat3(&g_Parts[k].tbl_adrM[index + 0].rot);	// ���݂̊p�x
		XMVECTOR rot = r1 - r0;
		XMStoreFloat3(&g_Parts[k].rot, r0 + rot * time);

		// scale�����߂� S = StartX + (EndX - StartX) * ���̎���
		XMVECTOR s1 = XMLoadFloat3(&g_Parts[k].tbl_adrM[index + 1].scl);	// ����Scale
		XMVECTOR s0 = XMLoadFloat3(&g_Parts[k].tbl_adrM[index + 0].scl);	// ���݂�Scale
		XMVECTOR scl = s1 - s0;
		XMStoreFloat3(&g_Parts[k].scl, s0 + scl * time);
	}
}

//�v���C���[�̐��`��ԁB�U�����[�V�������ɂ����ɗ�����_���[�W�������s��
void PlayerInterPoration(int i)
{
	if (g_Player[i].tbl_adrA == NULL)return;	// ���`��Ԃ����s����H

	//
	// ���`��Ԃ̏���
	// �ړ�����
	int		index = (int)g_Player[i].move_time;
	float	time = g_Player[i].move_time - index;
	int		size = g_Player[i].tbl_sizeA;

	float dt = 1.0f / g_Player[i].tbl_adrA[index].frame;	// 1�t���[���Ői�߂鎞��
	g_Player[i].move_time += dt;							// �A�j���[�V�����̍��v���Ԃɑ���

	if (index > (size - 2))	// �S�[�����I�[�o�[���Ă�����A�f�[�^���ŏ��ɖ߂��čU�����I��
	{
		g_Player[i].move_time = 0.0f;
		index = 0;
		g_Player[i].atCount = 0;
		g_Player[i].state = Standby;
		g_Player[i].attackUse = FALSE;
		g_Playerline[i].pos = { 0.0f, 0.0f, 0.0f };
		g_Playerline[i].rot = { 0.0f, 0.0f, 0.0f };
		g_Playerline[i].scl = { 0.0f, 0.0f, 0.0f };
	}
	// ���W�����߂�	X = StartX + (EndX - StartX) * ���̎���
	XMVECTOR p1 = XMLoadFloat3(&g_Player[i].tbl_adrA[index + 1].pos);	// ���̏ꏊ
	XMVECTOR p0 = XMLoadFloat3(&g_Player[i].tbl_adrA[index + 0].pos);	// ���݂̏ꏊ
	XMVECTOR vec = p1 - p0;
	XMStoreFloat3(&g_Playerline[i].pos, p0 + vec * time);

	// ��]�����߂�	R = StartX + (EndX - StartX) * ���̎���
	XMVECTOR r1 = XMLoadFloat3(&g_Player[i].tbl_adrA[index + 1].rot);	// ���̊p�x
	XMVECTOR r0 = XMLoadFloat3(&g_Player[i].tbl_adrA[index + 0].rot);	// ���݂̊p�x
	XMVECTOR rot = r1 - r0;
	XMStoreFloat3(&g_Playerline[i].rot, r0 + rot * time);

	// scale�����߂� S = StartX + (EndX - StartX) * ���̎���
	XMVECTOR s1 = XMLoadFloat3(&g_Player[i].tbl_adrA[index + 1].scl);	// ����Scale
	XMVECTOR s0 = XMLoadFloat3(&g_Player[i].tbl_adrA[index + 0].scl);	// ���݂�Scale
	XMVECTOR scl = s1 - s0;
	XMStoreFloat3(&g_Playerline[i].scl, s0 + scl * time);

	for (int k = g_Player[i].startNum; k < g_Player[i].startNum + g_Player[i].partsNum; k++)
	{
		if (g_Parts[k].tbl_adrA == NULL)continue;;	// ���`��Ԃ����s����H

		int		index = (int)g_Parts[k].move_time;
		float	time = g_Parts[k].move_time - index;
		int		size = g_Parts[k].tbl_sizeA;

		float dt = 1.0f / g_Parts[k].tbl_adrA[index].frame;	// 1�t���[���Ői�߂鎞��
		g_Parts[k].move_time += dt;							// �A�j���[�V�����̍��v���Ԃɑ���

		if (index > (size - 2))	// �S�[�����I�[�o�[���Ă�����A�f�[�^���ŏ��ɖ߂��čU�����I��
		{
			g_Parts[k].move_time = 0.0f;
			index = 0;
		}

		// ���W�����߂�	X = StartX + (EndX - StartX) * ���̎���
		XMVECTOR p1 = XMLoadFloat3(&g_Parts[k].tbl_adrA[index + 1].pos);	// ���̏ꏊ
		XMVECTOR p0 = XMLoadFloat3(&g_Parts[k].tbl_adrA[index + 0].pos);	// ���݂̏ꏊ
		XMVECTOR vec = p1 - p0;
		XMStoreFloat3(&g_Parts[k].pos, p0 + vec * time);

		// ��]�����߂�	R = StartX + (EndX - StartX) * ���̎���
		XMVECTOR r1 = XMLoadFloat3(&g_Parts[k].tbl_adrA[index + 1].rot);	// ���̊p�x
		XMVECTOR r0 = XMLoadFloat3(&g_Parts[k].tbl_adrA[index + 0].rot);	// ���݂̊p�x
		XMVECTOR rot = r1 - r0;
		XMStoreFloat3(&g_Parts[k].rot, r0 + rot * time);

		// scale�����߂� S = StartX + (EndX - StartX) * ���̎���
		XMVECTOR s1 = XMLoadFloat3(&g_Parts[k].tbl_adrA[index + 1].scl);	// ����Scale
		XMVECTOR s0 = XMLoadFloat3(&g_Parts[k].tbl_adrA[index + 0].scl);	// ���݂�Scale
		XMVECTOR scl = s1 - s0;
		XMStoreFloat3(&g_Parts[k].scl, s0 + scl * time);
	}

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

//�X�L������(�����ł̓X�L���^�C�v�̔��ʂ��s���A����Ɋ�Â����֐����Ăяo��)
void PlayerSkill(int i)
{
	//�|�����[�t�B�Y���g����I���Č����������ǂ߂�ǂ������c
	//�X�L���֐���ŃX�L��������؂邱��(skillUse,state)
	switch (g_Player[i].skillID)
	{
	case neutro_skill:
		NeutroSkill(&g_Player[i]);
		break;
	case macro_skill:
		break;
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
			if (CollisionBC(g_Player[i].pos, enemy[k].pos, 20.0f, 1.0f)) {
				//�����ŃG�l�~�[���u���b�N��Ԃ֕ύX����B�U�����������
				g_Player[i].blockNum++;
				enemy[k].blocked = TRUE;
				enemy[k].target = &g_Player[i];
			}

		}
	}
}

//�������L�����̃^�[�Q�b�g����֐��H�ꉞ�ߐڃL������������
//�G�L�����̍U���^�[�Q�b�g����B���_�����ԋ߂��G�������čU������
void CheckEnemyTarget(int i)
{
	//�U�����ł͂Ȃ��Ȃ炱���Ń^�[�Q�b�g����͂��Ȃ�
	if (g_Player[i].state == Standby) {
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
				XMVECTOR v2 = XMLoadFloat3(&enemy[g_Player[i].targetable[k]].pos) - XMLoadFloat3(&g_Player[i].pos);
				XMStoreFloat3(&countData, v2);
				float angle = atan2f(countData.x, countData.z);
				for (int d = 0; d < g_Player[i].tbl_sizeA; d++)
				{
					float buffx = g_Player[i].tbl_adrA[d].pos.x;
					float buffz = g_Player[i].tbl_adrA[d].pos.z;
					g_Player[i].tbl_adrA[d].pos.x = buffx * cosf(angle - g_Player[i].rot.y) + buffz * sinf(angle - g_Player[i].rot.y);
					g_Player[i].tbl_adrA[d].pos.z = buffz * cosf(angle - g_Player[i].rot.y) + buffx * -sinf(angle - g_Player[i].rot.y);
				}
				g_Player[i].rot.y = angle;
			}
		}
	}
}

//�X�L���|�C���g�Ɋւ��鏈��
void IncreaseSP(int i)
{
	//���t���[�����Ƃ�SP�����Z���Ă���
	if (g_Player[i].intervalSP < PLAYER_SP_FLAME)
	g_Player[i].intervalSP++;

	if (g_Player[i].intervalSP >= PLAYER_SP_FLAME &&
		g_Player[i].skillPoint < g_Player[i].skillPointMax) {
		g_Player[i].skillPoint += g_Player[i].increaseSP;
		g_Player[i].intervalSP = 0;
	}

	if (g_Player[i].skillPoint >= g_Player[i].skillPointMax)
		g_Player[i].skillAble = TRUE;
	else
		g_Player[i].skillAble = FALSE;
}

void SetPlayer(XMFLOAT3 pos)
{
	LoadModel(name[0], &g_Player[playerNum].model);
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
		//LoadModel(MODEL_GRAPE_PARTS, &g_Parts[partsNum].model);
		//// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		//GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);
		g_Parts[partsNum].load = TRUE;

		g_Parts[partsNum].pos = { 20.0f, 10.0f, 0.0f };		// �|���S���̈ʒu
		g_Parts[partsNum].rot = { 0.0f, 0.0f, 0.0f };		// �|���S���̌���(��])
		g_Parts[partsNum].scl = { 1.0f, 1.0f, 1.0f };		// �|���S���̑傫��(�X�P�[��)

		// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
		g_Parts[partsNum].tbl_adrA = NULL;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
		g_Parts[partsNum].tbl_sizeA = 0;	// �o�^�����e�[�u���̃��R�[�h����
		g_Parts[partsNum].tbl_adrM = NULL;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
		g_Parts[partsNum].tbl_sizeM = 0;	// �o�^�����e�[�u���̃��R�[�h����
		g_Parts[partsNum].move_time = 0;	// ���s����
		g_Parts[partsNum].parent = &g_Player[playerNum];	// �������e�Ȃ�NULL�A�������q���Ȃ�e��enemy�A�h���X
		partsNum++;
	}

}

void SetNeutrophils(XMFLOAT3 pos)
{
	LoadModel(name[1], &g_Player[playerNum].model);
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
	g_Player[playerNum].move_time = 0.0f;	// ���s����

	// �K�w�A�j���[�V�����p�̏���������
	g_Player[playerNum].parent = NULL;			// �{�́i�e�j�Ȃ̂�NULL������
	playerNum++;

	//�p�[�c�̏���������
	for (int k = 0; k < g_Player[playerNum].partsNum; k++)
	{
		//LoadModel(MODEL_GRAPE_PARTS, &g_Parts[partsNum].model);
		//// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		//GetModelDiffuse(&g_Parts[partsNum].model, &g_Parts[partsNum].diffuse[0]);
		g_Parts[partsNum].load = TRUE;

		g_Parts[partsNum].pos = { 20.0f, 10.0f, 0.0f };		// �|���S���̈ʒu
		g_Parts[partsNum].rot = { 0.0f, 0.0f, 0.0f };		// �|���S���̌���(��])
		g_Parts[partsNum].scl = { 1.0f, 1.0f, 1.0f };		// �|���S���̑傫��(�X�P�[��)

		// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
		g_Parts[partsNum].tbl_adrA = NULL;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
		g_Parts[partsNum].tbl_sizeA = 0;	// �o�^�����e�[�u���̃��R�[�h����
		g_Parts[partsNum].tbl_adrM = NULL;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
		g_Parts[partsNum].tbl_sizeM = 0;	// �o�^�����e�[�u���̃��R�[�h����
		g_Parts[partsNum].move_time = 0;	// ���s����
		g_Parts[partsNum].parent = &g_Player[playerNum];	// �������e�Ȃ�NULL�A�������q���Ȃ�e��enemy�A�h���X
		partsNum++;
	}

}

int GetPlayerNum(void)
{
	return playerNum;
}

void SetPlayerNum(int s)
{
	playerNum += s;
}
int GetPlayerPartsNum(void)
{
	return partsNum;
}

void SetPlayerPartsNum(int s)
{
	partsNum += s;
}
//=============================================================================
// �v���C���[�����擾
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player[0];
}

PlayerParts *GetPlayerParts(void)
{
	return &g_Parts[0];
}


//���݂͍ŏ��ɓo�ꂵ���G���珇�Ƀ^�[�Q�b�g�e�[�u���ɓ����
void PLAYER::StateCheck(int i)
{
	if (g_Player[i].state == Deffend)return;
	g_Player[i].state = Standby;	//�Ƃ肠�����ҋ@��ԂɃZ�b�g
	ENEMY *enemy = GetEnemy();
	g_Player[i].count = 0;
	for (int k = 0; k < MAX_TARGET; k++)
	{
		g_Player[i].targetable[k] = 99;
	}
	for (int k = 0; k < MAX_ENEMY; k++)
	{
		if (enemy[k].use != TRUE)continue;
		//�v���C���[�̍U���͈͂�1�̂ł��G������Ȃ�΍U�������ɓ���B�^�[�Q�b�g�����ۑ�
		if (CollisionBC(g_Player[i].pos, enemy[k].pos, g_Player[i].size, 10.0f))
		{
			g_Player[i].state = Deffend;
			g_Player[i].targetable[g_Player[i].count] = k;
			g_Player[i].count++;
		}
	}
	//�X�L���g�p���Ȃ�X�e�[�g���㏑��(�^�[�Q�b�g���͎�肽������)
	if(g_Player[i].skillUse)
		g_Player[i].state = Skill;
}

//�v���C���[�L�����̗̑̓o�[�̕\��������SP�\������
void DrawPlayerLife(void)
{
	// Z��r�Ȃ�
	SetDepthEnable(FALSE);
	// ���C�e�B���O�𖳌�
	SetLightEnable(FALSE);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;
	//hp�o�[��ݒu
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (g_Player[i].use != TRUE)continue;	//�g���ĂȂ��v���C���[�̓X���[
		
		for (int k = 0; k < 2; k++)//�ŏ��ɍő�l�̗͂��A���Ɍ��̗͂�\��
		{
			g_PlayerVar.pos = { g_Player[i].pos.x, g_Player[i].pos.y + 15.0f, g_Player[i].pos.z - 35.0f };
			if (k == 0)
			{
				g_PlayerVar.scl = { 1.0f, 1.0f, 1.0f };
				g_PlayerVar.material.Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.7f);
			}
			else
			{
				float par = (float)(g_Player[i].life) / (float)(g_Player[i].lifeMax);
				g_PlayerVar.scl = { par, 1.0f, 1.0f };
				g_PlayerVar.pos.x -= (VAR_WIDTH * (1.0f - par)) * 0.5f;
				g_PlayerVar.material.Diffuse = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
			}
			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_PlayerVar.scl.x, g_PlayerVar.scl.y, g_PlayerVar.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(g_PlayerVar.rot.x, g_PlayerVar.rot.y, g_PlayerVar.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_PlayerVar.pos.x, g_PlayerVar.pos.y, g_PlayerVar.pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			// �}�e���A���̐ݒ�
			SetMaterial(g_PlayerVar.material);

			// �|���S���̕`��
			GetDeviceContext()->Draw(4, 0);
		}
	}

	DrawPlayerSP();
	// Z��r����
	SetDepthEnable(TRUE);
	// ���C�e�B���O��L����
	SetLightEnable(TRUE);

}

void DrawPlayerSP(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;
	//hp�o�[��ݒu
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (g_Player[i].use != TRUE)continue;	//�g���ĂȂ��v���C���[�̓X���[

		for (int k = 0; k < 2; k++)//�ŏ��ɍő�l�̗͂��A���Ɍ��̗͂�\��
		{
			g_PlayerVar.pos = { g_Player[i].pos.x, g_Player[i].pos.y + 15.0f, g_Player[i].pos.z - 40.0f };
			if (k == 0)
			{
				g_PlayerVar.scl = { 1.0f, 1.0f, 1.0f };
				g_PlayerVar.material.Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.7f);
			}
			else
			{
				float par = (float)(g_Player[i].skillPoint) / (float)(g_Player[i].skillPointMax);
				g_PlayerVar.scl = { par, 1.0f, 1.0f };
				g_PlayerVar.pos.x -= (VAR_WIDTH * (1.0f - par)) * 0.5f;
				if (par < 1.0f)
					g_PlayerVar.material.Diffuse = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
				else
				{
					g_PlayerVar.material.Diffuse = XMFLOAT4(1.0f, 1.0f, g_PlayerVar.pardiff, 1.0f);
					g_PlayerVar.pardiff += g_PlayerVar.parpolar;
					if(g_PlayerVar.pardiff < 0.0f || g_PlayerVar.pardiff > 1.0f)
					g_PlayerVar.parpolar *= -1.0f;
				}
			}
			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_PlayerVar.scl.x, g_PlayerVar.scl.y, g_PlayerVar.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(g_PlayerVar.rot.x, g_PlayerVar.rot.y, g_PlayerVar.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_PlayerVar.pos.x, g_PlayerVar.pos.y, g_PlayerVar.pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			// �}�e���A���̐ݒ�
			SetMaterial(g_PlayerVar.material);

			// �|���S���̕`��
			GetDeviceContext()->Draw(4, 0);
		}
	}

}

HRESULT MakeVertexPlayerVar(void)
{
	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	{//���_�o�b�t�@�̒��g�𖄂߂�
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		// ���_���W�̐ݒ�
		vertex[0].Position = XMFLOAT3(-VAR_WIDTH / 2, 0.0f, VAR_HEIGHT / 2);
		vertex[1].Position = XMFLOAT3(VAR_WIDTH / 2, 0.0f, VAR_HEIGHT / 2);
		vertex[2].Position = XMFLOAT3(-VAR_WIDTH / 2, 0.0f, -VAR_HEIGHT / 2);
		vertex[3].Position = XMFLOAT3(VAR_WIDTH / 2, 0.0f, -VAR_HEIGHT / 2);

		// �@���̐ݒ�
		vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

		// �g�U���̐ݒ�
		vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
		vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
		vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
		vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

		GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}

	return S_OK;
}
