//=============================================================================
//
// �v���C���[�Z�b�g���� [playerSet.h]
// Author : 
//
//=============================================================================
#pragma once
#include "player.h"
#include "main.h"
#include "model.h"
#include "input.h"
#include "fieldchip.h"
#include "team.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

class PlayerSet
{
private:
	XMFLOAT3 setPos;	//�Z�b�g�����̍��W
	BOOL setMode;		//�Z�b�g���[�h�Ɉڍs�������ۂ�
	int setPlayerTable[MAX_PLAYER_SET];	//�Z�b�g����v���C���[�ԍ��e�[�u��(�Y�����H)
	PLAYER *setPlayerNum;	
	PlayerParts *setPlayerParts;
public:
	HRESULT InitPlayerSet(void);
	void PlayerSetMap(void);
	void SetModeChange(int i);
	void SetPosition(void);
	void SetPlayerInfo(PLAYER* member, PlayerParts* parts);
};