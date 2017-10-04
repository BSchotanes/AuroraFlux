/***********************************************
* Filename:  		PlayerEnterWarp.h
* Date:      		1/19/13
* Mod. Date: 		1/19/13
* Mod. Initials:	AQ
* Author:    		Alexander Garcia de Quevedo
* Purpose:   		This class will serve as a state for the 
*					player to fly off into the distance.
************************************************/
#include "../Definitions.h"
#include "PlayerEnterWarp.h"
#include "../Input/Input.h"
#include "../Particle System/EffectInformation.h"

/*****************************************************************
* CRedPlayer():		Default Constructor will set up his type and timers
* Ins:			    None    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CPlayerEnterWarp::CPlayerEnterWarp(void) : m_bCanRoll(false), m_fRollTimer(0),m_nMaxHealth(100),m_nMaxShield(RED_STATE_SHIELD), m_bSoundPlay(false), m_pPlayer(nullptr)
{
}

CPlayerEnterWarp::CPlayerEnterWarp(CPlayer* _pPlayer) : m_bCanRoll(false), m_fRollTimer(0),m_nMaxHealth(100),m_nMaxShield(RED_STATE_SHIELD), m_pPlayer(_pPlayer)
{
}

/*****************************************************************
* ~CBluePlayer():	Default Destructor
* Ins:			    None    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CPlayerEnterWarp::~CPlayerEnterWarp(void)
{
}

/*****************************************************************
* Update():		Will update its respective data, shields, and health.
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CPlayerEnterWarp::Update(float _fElapsedTime)
{
	m_pPlayer->DisableInput(false);
}
/*****************************************************************
* DoRoll():		If a missile is incoming, dodge it if the timer is ready.
* Ins:			    None    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CPlayerEnterWarp::Render()
{
}

/*****************************************************************
* DoRoll():		If a missile is incoming, dodge it if the timer is ready.
* Ins:			    None    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CPlayerEnterWarp::DoRoll()
{
	if (m_bCanRoll == false)
	{
		return;
	}
}