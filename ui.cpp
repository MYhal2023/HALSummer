//=============================================================================
//
// UI���� [ui.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "sprite.h"
#include "renderer.h"
#include "game.h"
#include "playerSet.h"
#include "ui.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX			(1)				// �e�N�X�`���̐�
#define CHAR_TEXTURE_MAX	(2)		// �L�����e�N�X�`���̐�

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// ���_���
static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����
static ID3D11ShaderResourceView		*g_CharTexture[CHAR_TEXTURE_MAX] = { NULL };	// �e�N�X�`�����
static char* g_TextureName[] = {
	"data/TEXTURE/box.png",
};
static char* g_CharTextureName[] = {
	"data/TEXTURE/neutro.png",
	"data/TEXTURE/neutro.png",
};
static UI g_UI[TEXTURE_MAX];
static BOOL g_Load = FALSE;

HRESULT InitUI(void)
{
	ID3D11Device *pDevice = GetDevice();

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
	// �e�N�X�`������
	for (int i = 0; i < CHAR_TEXTURE_MAX; i++)
	{
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_CharTextureName[i],
			NULL,
			NULL,
			&g_CharTexture[i],
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

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_UI[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };
		g_UI[i].pos = { 0.0f, 0.0f };
		g_UI[i].size = { 50.0f, 50.0f };
		g_UI[i].use = TRUE;
	}
	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitUI(void)
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
	// �e�N�X�`���̉��
	for (int i = 0; i < CHAR_TEXTURE_MAX; i++)
	{
		if (g_CharTexture[i])
		{
			g_CharTexture[i]->Release();
			g_CharTexture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateUI(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawUI(void)
{
	SetDepthEnable(FALSE);

	// ���C�e�B���O�𖳌�
	SetLightEnable(FALSE);
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

	DrawCharBox();

	SetDepthEnable(TRUE);

	// ���C�e�B���O�𖳌�
	SetLightEnable(TRUE);

}

//=============================================================================
// �L�����{�b�N�X�`�揈��
//=============================================================================
void DrawCharBox(void)
{
	g_UI[charBox].size = { 160.0f, 160.0f };
	int k = 1;
	for (int i = MAX_PLAYER_SET - 1; i >= 0; i--)
	{
		PlayerSet *ps = GetSetPos();

		if (ps->use[i] != TRUE)continue;	//���g�p�Ґ��g�̓X���[

		//�E���珇�ԂɁA�Ґ��̍Ō������`��
		g_UI[charBox].pos = { SCREEN_WIDTH - k * g_UI[charBox].size.x, SCREEN_HEIGHT - g_UI[charBox].size.y * 0.5f };
		k++;
		//�L����ID�𒊏o���ăL�����N�^�[���ŏ��ɕ`��
		int id = ps->setCharID;
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_CharTexture[id]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, g_UI[charBox].pos.x, g_UI[charBox].pos.y, g_UI[charBox].size.x, g_UI[charBox].size.y, 0.0f, 0.0f, 1.0f, 1.0f,
			g_UI[charBox].color);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);


		//�L�����̏�Ƀ{�b�N�X��`��
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[charBox]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, g_UI[charBox].pos.x, g_UI[charBox].pos.y, g_UI[charBox].size.x, g_UI[charBox].size.y, 0.0f, 0.0f, 1.0f, 1.0f,
			g_UI[charBox].color);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}
}

