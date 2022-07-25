//=============================================================================
//
// Reserve���� [ui.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "sprite.h"
#include "renderer.h"
#include "game.h"
#include "playerSet.h"
#include "reserve.h"
#include "base.h"
#include "cost.h"
#include "team.h"
#include "ui.h"
#include "text_texture.h"
#include "fade.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX			(8)				// �e�N�X�`���̐�
#define CHAR_TEXTURE_MAX	(3)				// �L�����e�N�X�`���̐�
#define NUMBER_SIZE			(30.0f)			// x�����̃T�C�Y
#define COST_NUMBER_SIZE	(45.0f)			// x�����̃T�C�Y
#define BUTTON_SIZE			(106.0f)		// �{�^���̏c���T�C�Y�B�������ꂭ�炢
#define BUTTON_MAX			(3)				// ���[�T�[���I���ł���{�^����
#define ROW_NUM				(5)				// ���ɕ��ׂ郆�j�b�g��

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// ���_���
static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����
static ID3D11ShaderResourceView		*g_CharTexture[CHAR_TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char* g_TextureName[TEXTURE_MAX] = {
	"data/TEXTURE/var.png",
	"data/TEXTURE/title_bg.png",
	"data/TEXTURE/number.png",
	"data/TEXTURE/costbox.png",
	"data/TEXTURE/t_confirm.png",
	"data/TEXTURE/t_day.png",
	"data/TEXTURE/t_powerup.png",
	"data/TEXTURE/t_start.png",

};
static char* g_CharTextureName[] = {
	"data/TEXTURE/neutro.png",
	"data/TEXTURE/neutro.png",
	"data/TEXTURE/macro.png",
};

static Reserve g_Reserve;
static TEXT_TEXTURE g_text[TEXTURE_MAX];
static Button g_Button[BUTTON_MAX];
static int cursol;
static BOOL g_Load = FALSE;
static BOOL restart = FALSE;

HRESULT InitReserve(void)
{
	//�N�����A��x����������
	if (!restart)
	{
		g_Reserve.day = 1;
		g_Reserve.energy = 0;
		g_Reserve.oxygen = 0;
		g_Reserve.mode = 99;
		g_Reserve.selectPw = 0;
		restart = TRUE;
	}

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

	for (int i = 0; i < BUTTON_MAX; i++)
	{
		g_Button[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };
		g_Button[i].pos = { 0.0f, 0.0f };
		g_Button[i].size = { 50.0f, 50.0f };
		g_Button[i].use = TRUE;
	}


	float py = BUTTON_SIZE * 0.5f + 16.0f;;
	
	g_Button[UnitPowerUp].pos.y = py + BUTTON_SIZE + 40.0f;
	g_Button[UnitPowerUp].pos.x = 8.0f + BUTTON_SIZE * 1.5f;
	
	g_Button[UnitConfirm].pos.y = g_Button[UnitPowerUp].pos.y + BUTTON_SIZE + 16.0f;
	g_Button[UnitConfirm].pos.x = 8.0f + BUTTON_SIZE * 1.5f;
	
	g_Button[GameStart].pos.y = SCREEN_HEIGHT - 40.0f - BUTTON_SIZE * 0.5f;
	g_Button[GameStart].pos.x = SCREEN_WIDTH - 16.0f - BUTTON_SIZE * 1.5f;

	cursol = 0;
	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitReserve(void)
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
void UpdateReserve(void)
{
	switch (g_Reserve.mode)
	{
	case UnitPowerUp:
		UnitPowerUpMode();
		break;
	case UnitConfirm:
		break;
	case 99:
		NormalRserveMode();
		break;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawReserve(void)
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
	
	NormalRserveModeDraw();	//�S�̋��ʕ�����`��(���ʕ��͂����ɓ����B���C���[�ɋC��t���鎖)

	const XMFLOAT4 color = { 0.0f, 0.0f, 0.0f, 0.75f };
	switch (g_Reserve.mode)
	{
	case UnitPowerUp:
		DrawButton(color, SCREEN_WIDTH * 0.55f, SCREEN_HEIGHT * 0.475f, SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT* 0.70f);
		DrawReserveChar();
		break;
	case UnitConfirm:
		DrawButton(color, SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.25f, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT* 0.5f);
		DrawReserveChar();
		break;
	case 99:
		break;
	}

	SetDepthEnable(TRUE);

	// ���C�e�B���O�𖳌�
	SetLightEnable(TRUE);

}

void DrawButton(XMFLOAT4 color, float px, float py, float sx, float sy)
{
	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[Button_bg]);

	// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	SetSpriteColor(g_VertexBuffer, px, py, sx, sy, 0.0f, 0.0f, 1.0f, 1.0f,
		color);

	// �|���S���`��
	GetDeviceContext()->Draw(4, 0);

}
//����:�\�������������A�\�����W(x,y)�A�\���T�C�Y(x����,y����)
void DrawNumberRe(int numb, float px, float py, float sx, float sy, XMFLOAT4 color)
{
	int digit = 0;
	int clock = numb;
	if (clock != 0)
	{
		while (clock != 0)
		{
			clock /= 10;
			digit++;
		}
	}
	else
		digit = 1;

	for (int i = 0; i < digit; i++)
	{
		px = px - sx * i;
		float x = (float)(numb % 10);		//����\�����鐔��
		float tx = x * 0.1f;			// �e�N�X�`���̍���X���W

		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[re_number]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, px, py, sx, sy, tx, 0.0f, 0.1f, 1.0f,
			color);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
		numb /= 10;
	}
}

void DrawTextReserve(int k, float px, float py, float sx, float sy, XMFLOAT4 color)	//�����ɕ`�悵�����������B
{
	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[k]);


	// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	SetSpriteColor(g_VertexBuffer, px, py, sx, sy, 0.0f, 0.0f, 1.0f, 1.0f,color);

	// �|���S���`��
	GetDeviceContext()->Draw(4, 0);

}

void NormalRserveMode(void)
{
	if (cursol < GameStart && GetKeyboardTrigger(DIK_DOWN))
		cursol++;
	if (cursol > 0 && GetKeyboardTrigger(DIK_UP))
		cursol--;

	//�I������Ă���{�^���������\����
	for (int i = 0; i < BUTTON_MAX; i++)
	{
		if (i == cursol)
			g_Button[i].color = { 1.5f, 1.5f, 1.5f, 1.0f };
		else
			g_Button[i].color = { 0.5f, 0.5f, 0.5f, 1.0f };
	}

	//����{�^�����������炻��ɉ����ĉ�ʑJ��
	if (GetKeyboardTrigger(DIK_RETURN)) {
		switch (cursol)
		{
		case UnitPowerUp:
			PowerUpButton();
			break;
		case UnitConfirm:
			ConfirmButton();
			break;
		case GameStart:
			GameStartButton();
			break;
		}
	}
}

void NormalRserveModeDraw(void)
{
	//�w�i�`��
// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[Reserve_bg]);

	// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X, SCREEN_CENTER_Y, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f,
		XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });

	// �|���S���`��
	GetDeviceContext()->Draw(4, 0);


	//�����`��
	float px = 16.0f + BUTTON_SIZE * 0.25f;
	float py = BUTTON_SIZE * 0.5f + 16.0f;
	DrawNumberRe(g_Reserve.day, px, py, BUTTON_SIZE * 0.5f, BUTTON_SIZE, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	DrawTextReserve(TEXT_DAY, px + BUTTON_SIZE * 1.5f, py, BUTTON_SIZE * 2.0f, BUTTON_SIZE, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));


	////�{�^���`��(��ɐݒu���Ă������珇�ɕ`��)
	DrawButton(g_Button[UnitPowerUp].color, g_Button[UnitPowerUp].pos.x, g_Button[UnitPowerUp].pos.y, BUTTON_SIZE * 2.5f, BUTTON_SIZE);
	DrawTextReserve(TEXT_POWERUP, g_Button[UnitPowerUp].pos.x, g_Button[UnitPowerUp].pos.y, BUTTON_SIZE * 2.0f, BUTTON_SIZE,
		XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });

	DrawButton(g_Button[UnitConfirm].color, g_Button[UnitConfirm].pos.x, g_Button[UnitConfirm].pos.y, BUTTON_SIZE * 2.5f, BUTTON_SIZE);
	DrawTextReserve(TEXT_CONFIRM, g_Button[UnitConfirm].pos.x, g_Button[UnitConfirm].pos.y, BUTTON_SIZE * 2.0f, BUTTON_SIZE,
		XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });

	DrawButton(g_Button[GameStart].color, g_Button[GameStart].pos.x, g_Button[GameStart].pos.y, BUTTON_SIZE * 2.5f, BUTTON_SIZE);
	DrawTextReserve(TEXT_START, g_Button[GameStart].pos.x, g_Button[GameStart].pos.y, BUTTON_SIZE * 2.0f, BUTTON_SIZE,
		XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });
}

void UnitPowerUpMode(void)
{
	if (GetKeyboardTrigger(DIK_LEFT) && g_Reserve.selectPw > 0)
	{
		g_Reserve.selectPw--;
	}
	else if (GetKeyboardTrigger(DIK_RIGHT) && g_Reserve.selectPw < GetMemberNum() - 1)
	{
		g_Reserve.selectPw++;
	}

	if (GetKeyboardTrigger(DIK_UP) && g_Reserve.selectPw >4)
	{
		g_Reserve.selectPw -= ROW_NUM;
	}
	else if (GetKeyboardTrigger(DIK_DOWN) && g_Reserve.selectPw < GetMemberNum() - 1)
	{
		if (g_Reserve.selectPw + ROW_NUM > GetMemberNum() - 1)
			g_Reserve.selectPw = GetMemberNum() - 1;
		else
			g_Reserve.selectPw += ROW_NUM;
	}
	if (GetKeyboardTrigger(DIK_RETURN))
	{

	}

	if (GetKeyboardTrigger(DIK_C))
	{
		g_Reserve.mode = 99;
	}
}

void PowerUpButton(void)
{
	g_Reserve.mode = UnitPowerUp;
}

void ConfirmButton(void)
{
	g_Reserve.mode = UnitConfirm;
}

void GameStartButton(void)
{
	SetFade(FADE_OUT, MODE_GAME);
}

int GetDay(void)
{
	return g_Reserve.day;
}

//=============================================================================
// �L�����{�b�N�X�`�揈��
//=============================================================================
void DrawReserveChar(void)
{
	const float boxsize = 180.0f;	//�{�b�N�X�T�C�Y��`
	const float boxbuff = 24.0f;
	float buffx = 0.0f;	//�{�b�N�X�Ԃ̋����������J����
	float buffy = 0.0f;	//�{�b�N�X�Ԃ̋����������J����
	const int rowNum = 5;
	int k = 0;
	for (int i = 0; i < MAX_PLAYER_SET; i++)
	{
		PlayerStatus *member = GetTeam();

		if (member[i].use != TRUE)continue;	//���g�p�Ґ��g�̓X���[

		if (k % rowNum != 0)buffx = boxbuff * (k % rowNum);
		else buffx = 0.0f;

		if (k >= rowNum)buffy = boxbuff;
		//�E���珇�ԂɁA�Ґ��̍Ō������`��
		XMFLOAT2 pos = { (SCREEN_WIDTH * 0.25f) + (k %ROW_NUM) * boxsize + buffx, (SCREEN_HEIGHT *0.2f) + boxsize * (0.5f + (float)(k/ ROW_NUM)) + buffy};
		//�L����ID�𒊏o���ăL�����N�^�[���ŏ��ɕ`��
		int id = member[i].charID;
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_CharTexture[id]);
		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, pos.x, pos.y, boxsize, boxsize, 0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);


		//�L�����̏�Ƀ{�b�N�X��`��
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[re_box]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, pos.x, pos.y, boxsize, boxsize, 0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);

		//�J�[�\���������Ă���Ȃ炻����ۂ��{�b�N�X���d�˂ĕ`��
		if (k == g_Reserve.selectPw) {
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[Button_bg]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer, pos.x, pos.y, boxsize, boxsize, 0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
		k++;

	}
}
