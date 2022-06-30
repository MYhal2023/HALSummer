//=============================================================================
//
// ï“ê¨èàóù [team.h]
// Author : 
//
//=============================================================================
#pragma once
#include "main.h"
#include "player.h"
#define MAX_PLAYER_SET (10)

class Team : public PLAYER, public PlayerParts
{
private:
	int member[MAX_PLAYER_SET];
public:
	void InitTeam(void);
	void SetMember(void);
	static PLAYER* GetTeam(void);
	static PlayerParts* GetTeamParts(void);
};