/***********************************************
* Filename:  		InactivePlayer.cpp
* Date:      		2/1/2013
* Mod. Date: 		2/1/2013
* Mod. Initials:	AL
* Author:    		Anton M. Lopez
* Purpose:   		This class will hold all data neccissary to make the player inactive during cinematics.
************************************************/
#include "../Definitions.h"
#include "InactivePlayer.h"


CInactivePlayer::~CInactivePlayer()
{
}
CInactivePlayer::CInactivePlayer()
{
	m_pPlayer = nullptr;
}
CInactivePlayer::CInactivePlayer(CPlayer* _player)
{
	m_pPlayer = _player;
}
void CInactivePlayer::Update(float _fElapsedTime)
{
	m_pPlayer->SetInactive(true);
}
void CInactivePlayer::Render()
{
}