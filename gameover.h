//=============================================================================
//
// �Q�[���I�[�o�[���� [gameover.h]
// Author : 
//
//=============================================================================
#pragma once
#include "main.h"

class Over
{
public:
	XMFLOAT4 color;	//�����n���F���
	XMFLOAT2 pos;	//�\�����W
	XMFLOAT2 size;	//�\�����W
	BOOL use;		//���̉摜��\�����邩
};

enum OVER_TEXT
{
	FadeRed,
};
HRESULT InitOver(void);
void UninitOver(void);
void UpdateOver(void);
void DrawOver(void);