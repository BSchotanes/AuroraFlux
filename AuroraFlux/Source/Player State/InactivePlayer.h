/***********************************************
* Filename:  		InactivePlayer.h
* Date:      		2/1/2013
* Mod. Date: 		2/1/2013
* Mod. Initials:	AL
* Author:    		Anton M. Lopez
* Purpose:   		This class will hold all data neccissary to make the player inactive during cinematics.
************************************************/
#ifndef CINACTIVEPLAYER_H
#define CINACTIVEPLAYER_H

#include "PlayerBehavior.h"
#include "../Entity/Player.h"

class CInactivePlayer : public CPlayerBehavior 
{
private:
	CPlayer* m_pPlayer;
public:
	~CInactivePlayer();
	CInactivePlayer();
	CInactivePlayer(CPlayer* obj);
	void Update(float _fElapsedTime);
	void Render();
	int GetMaxShield()		{return RED_STATE_SHIELD;}
	int GetMaxHealth()		{return PLAYER_HEALTH;}
};

#endif