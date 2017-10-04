/***********************************************
* Filename:  		RedPlayer.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will hold all the red player entities data.
************************************************/
#include "../StdAfx.h"
#include "RedPlayer.h"
#include "../Input/Input.h"

/*****************************************************************
* CRedPlayer():		Default Constructor will set up his type and timers
* Ins:			    None    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CRedPlayer::CRedPlayer(void) : m_bCanRoll(false), m_fRollTimer(0),m_nMaxHealth(100),m_nMaxShield(RED_STATE_SHIELD), m_bSoundPlay(false)
{
}

CRedPlayer::CRedPlayer(CPlayer* _pPlayer) : m_bCanRoll(false), m_fRollTimer(0),m_nMaxHealth(100),m_nMaxShield(RED_STATE_SHIELD)
{
	m_pPlayer = _pPlayer;
}

/*****************************************************************
* ~CRedPlayer():	Default Destructor will set up his type and timers
* Ins:			    None    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CRedPlayer::~CRedPlayer(void)
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
void CRedPlayer::Update(float _fElapsedTime)
{
	// Set the players information
	CPlayerBehavior::Update(_fElapsedTime);
	m_pPlayer->SetType(eRED_PLAYER);
	m_pPlayer->SetDamage(PLAYER_LASER_DAMAGE);

	float fCurrVelMod = m_pPlayer->GetVelocityModifier();

	// Apply Velocity
	if(m_pPlayer->GetCamera()->GetWaitForGame() == false)
	{
		if(CInput::ButtonHeld(eUp))
		{
			if(fCurrVelMod < RED_STATE_MAX_VELOCITY )
			{
				m_pPlayer->SetVelocityModifier(min(RED_STATE_MAX_VELOCITY, fCurrVelMod + _fElapsedTime*RED_ACCELERATION));
			}
			else if(fCurrVelMod > RED_STATE_MAX_VELOCITY )
			{
				m_pPlayer->SetVelocityModifier(max(RED_STATE_MAX_VELOCITY, fCurrVelMod - _fElapsedTime*RED_ACCELERATION));
			}

			if(CInput::BufferedButtonPress(eUp))
			{
				WwiseNS::SoundEvent(ePlayerThrust, ePlay);
			}
		}
		else if(CInput::ButtonHeld(eDown))
		{
			if(fCurrVelMod > RED_STATE_MIN_VELOCITY )
			{
				m_pPlayer->SetVelocityModifier(max(RED_STATE_MIN_VELOCITY, fCurrVelMod - _fElapsedTime*(RED_ACCELERATION / 2)));
			}
			if(fCurrVelMod < RED_STATE_MIN_VELOCITY )
			{
				m_pPlayer->SetVelocityModifier(min(RED_STATE_MIN_VELOCITY, fCurrVelMod + _fElapsedTime*(RED_ACCELERATION / 2)));
			}

			WwiseNS::SoundEvent(ePlayerThrust, eStop);
		}
		else
		{
			if(fCurrVelMod < RED_STATE_MIN_VELOCITY)
			{
				m_pPlayer->SetVelocityModifier(fCurrVelMod + _fElapsedTime*10);
			}
			else if(fCurrVelMod > RED_STATE_MAX_VELOCITY )
			{
				m_pPlayer->SetVelocityModifier(fCurrVelMod - _fElapsedTime*10);
			}

			WwiseNS::SoundEvent(ePlayerThrust, eStop);
		}
	}

	if (m_bCanRoll == false)
	{
		m_fRollTimer -= _fElapsedTime;
	}

	if ( m_fRollTimer <= 0 )
	{
		m_fRollTimer = 10.0f;
		m_bCanRoll = true;
	}

	// Deplete shield if over 100
 	static float s_fDepleteShield = 0.0f;
 	s_fDepleteShield += _fElapsedTime;

	if(s_fDepleteShield > 0.1f)
	{
		if(m_pPlayer->GetShields() > 100)
		{
			m_pPlayer->SetShields(m_pPlayer->GetShields() - 1);
		}

 		s_fDepleteShield = 0.0f;
	}

	// Wait some time before regenerating shield
	//if(m_pPlayer->GetShields() <= 0)
	//{
	//	static float s_fRegenTimer = 1;
	//	s_fRegenTimer += _fElapsedTime;
	//
	//	if ((int)s_fRegenTimer % 4 == 0 && s_fRegenTimer != 0)
	//	{
	//		m_pPlayer->SetShields(m_pPlayer->GetShields() + 2);
	//
	//		/*if(!soundPlay)
	//		{
	//		WwiseNS::SoundEvent(eSheildRecharge, ePlay);
	//		soundPlay=true;
	//		}*/
	//
	//		s_fRegenTimer = 1;
	//	}
	//}
	/*else
	{
	if(soundPlay)
	{
	WwiseNS::SoundEvent(eSheildRecharge, eStop);
	soundPlay=false;
	}
	}*/


}
/*****************************************************************
* DoRoll():		If a missile is incoming, dodge it if the timer is ready.
* Ins:			    None    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CRedPlayer::Render()
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
void CRedPlayer::DoRoll()
{
	if (m_bCanRoll == false)
	{
		return;
	}
}