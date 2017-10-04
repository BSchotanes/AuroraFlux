/***********************************************
 * Filename:  		YellowPlayer.h
 * Date:      		11/27/2012
 * Mod. Date: 		11/29/2012
 * Mod. Initials:	AR
 * Author:    		Andrew A Rosser
 * Purpose:   		This class will hold all the yellow player entities data.
 ************************************************/
#include "../StdAfx.h"
#include "../Entity/IEntity.h"
#include "YellowPlayer.h"
#include "../Input/Input.h"

/*****************************************************************
 * CYellowPlayer():		Default Constructor will set up his type and timers
 * Ins:			    None    		      
 * Outs:		    None
 * Returns:		    None
 * Mod. Date:		11/29/2012
 * Mod. Initials:	AR
 *****************************************************************/
CYellowPlayer::CYellowPlayer(void) : m_fStealthTimer(0), m_bTimerDone(false),m_nMaxHealth(100),m_nMaxShield(YELLOW_STATE_SHIELD)
{
	
}

CYellowPlayer::CYellowPlayer(CPlayer* _pPlayer): m_nMaxHealth(100),m_nMaxShield(RED_STATE_SHIELD)
{
	m_pPlayer = _pPlayer;
}

/*****************************************************************
* ~CYellowPlayer():	Default Destructor will set up his type and timers
* Ins:			    None    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CYellowPlayer::~CYellowPlayer(void)
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
void CYellowPlayer::Update(float _fElapsedTime)
{
	// Set the players information
	CPlayerBehavior::Update(_fElapsedTime);
	m_fStealthTimer -= _fElapsedTime;
	m_pPlayer->SetType(eYELLOW_PLAYER);
	m_pPlayer->SetDamage(0);
	m_pPlayer->SetIsVisable(false);
	
	float fCurrVelMod = m_pPlayer->GetVelocityModifier();
	// Velocity
	if(CInput::ButtonHeld(eUp))
	{
		if(fCurrVelMod < YELLOW_STATE_MAX_VELOCITY )
		{
			m_pPlayer->SetVelocityModifier(fCurrVelMod + _fElapsedTime*3);
		}
		else if(fCurrVelMod > YELLOW_STATE_MAX_VELOCITY )
		{
			m_pPlayer->SetVelocityModifier(fCurrVelMod - _fElapsedTime*3);
		}

		if(CInput::BufferedButtonPress(eUp))
		{
			WwiseNS::SoundEvent(ePlayerThrust, ePlay);
		}
	}
	else if(CInput::ButtonHeld(eDown))
	{
		if(fCurrVelMod > YELLOW_STATE_MIN_VELOCITY )
		{
			m_pPlayer->SetVelocityModifier(fCurrVelMod - _fElapsedTime*3);
		}
		else if(fCurrVelMod < YELLOW_STATE_MIN_VELOCITY )
		{
			m_pPlayer->SetVelocityModifier(max(fCurrVelMod + _fElapsedTime*3, 0));
		}

		WwiseNS::SoundEvent(ePlayerThrust, eStop);
	}
	else
	{
		if(fCurrVelMod < (YELLOW_STATE_MAX_VELOCITY + YELLOW_STATE_MIN_VELOCITY)*0.5f)
		{
			m_pPlayer->SetVelocityModifier(fCurrVelMod + _fElapsedTime*3);
		}
		else if(fCurrVelMod > (YELLOW_STATE_MAX_VELOCITY + YELLOW_STATE_MIN_VELOCITY)*0.5f)
		{
			m_pPlayer->SetVelocityModifier(fCurrVelMod - _fElapsedTime*3);
		}

		WwiseNS::SoundEvent(ePlayerThrust, eStop);
	}

	// Deplete shield over time
	static float s_fShieldDepleteTimer = 1;
	s_fShieldDepleteTimer += _fElapsedTime;


	if ((int)s_fShieldDepleteTimer % 2 == 0 && s_fShieldDepleteTimer != 0)
	{
		
		if (m_fStealthTimer <= 0)
		{
			m_bTimerDone = true;
		}

	/*	if(m_pPlayer->GetShields() > 100)
		{
			m_pPlayer->SetShields(m_pPlayer->GetShields() - 25);
		}
		else if(m_pPlayer->GetShields() > 10)
		{
			m_pPlayer->SetShields(m_pPlayer->GetShields() - 10);
		}
		else*/ if(m_pPlayer->GetShields() > 0)
		{
			m_pPlayer->SetShields(m_pPlayer->GetShields() - 1);
		}

		s_fShieldDepleteTimer = 1;
	}

}
/*****************************************************************
 * Render():		Draw effects or whatnot.
 * Ins:			    None    		      
 * Outs:		    None
 * Returns:		    void
 * Mod. Date:		11/29/2012
 * Mod. Initials:	AR
 *****************************************************************/
void CYellowPlayer::Render()
{

}