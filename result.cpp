//=============================================================================
//
// ���U���g���� [result.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "sprite.h"
#include "renderer.h"
#include "input.h"
#include "game.h"
#include "result.h"
#include "gameover.h"
#include "debugproc.h"
#include "fade.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX			(2)				// �e�N�X�`���̐�


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// ���_���
static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����
static char* g_TextureName[] = {
	"data/TEXTURE/win_texture.png",
	"data/TEXTURE/lose_texture.png",
};
static Result g_Result;
static BOOL g_Load = FALSE;

HRESULT InitResult(void)
{
	ID3D11Device *pDevice = GetDevice();

	//�e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	g_Result.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	g_Result.pos = { SCREEN_CENTER_X, SCREEN_CENTER_Y };
	g_Result.size = { SCREEN_WIDTH, SCREEN_HEIGHT };
	g_Result.textNo = 0;
	g_Result.type = GetOverType();
	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitResult(void)
{
	if (g_Load == FALSE) return;

	// ���_�o�b�t�@�̉��
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	// �e�N�X�`���̉��
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResult(void)
{
	switch (g_Result.type)
	{
	case OVER_WIN:
		g_Result.textNo = 0;
		WinResult();
		break;
	case OVER_LOSE:
		g_Result.textNo = 1;
		LoseResult();
		break;
	}

//#ifdef _DEBUG
//	PrintDebugProc("")
//#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResult(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// ���U���g�̔w�i��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Result.textNo]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}


}

void WinResult(void)
{
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		SetFade(FADE_OUT, MODE_RESERVE);	//���󃋁[�v����悤��
	}
}

void LoseResult(void)
{
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		SetFade(FADE_OUT, MODE_RESERVE);	//���󃋁[�v����悤��
	}
}