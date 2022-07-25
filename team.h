//=============================================================================
//
// ï“ê¨èàóù [team.h]
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
void SetNeutro(int i);
void SetMacrophages(int i);
int GetMemberNum(void);