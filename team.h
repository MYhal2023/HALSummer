//=============================================================================
//
// �Ґ����� [team.h]
// Author : 
//
//=============================================================================
#pragma once
#include "main.h"
#include "playerSet.h"

class Team
{
public:
};

void UninitTeam(void);
void InitTeam(void);
void SetMember(int i);
PlayerStatus* GetTeam(void);
PlayerPartsStatus* GetTeamParts(void);