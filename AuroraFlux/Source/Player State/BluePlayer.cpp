/***********************************************
* Filename:  		BluePlayer.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will hold all the blue player entities data.
************************************************/
#include "../StdAfx.h"
//#include "../IEntity.h"
#include "BluePlayer.h"
#include "../Input/Input.h"

/*****************************************************************
* CBluePlayer():		Default Constructor will set up his type and timers
* Ins:			    None    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CBluePlayer::CBluePlayer(void) : m_fMissileTimer(0), m_bCanFire(0),m_nMaxHealth(100),m_nMaxShield(200)
{
}

/*****************************************************************
* CBluePlayer():	Overload Constructor will set its type and all of his
*					internal data as well as set its target to the passed in one.
* Ins:			    _missileTimer, _target  		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CBluePlayer::CBluePlayer(float _fMissileTimer, int _nTarget) : m_fMissileTimer(_fMissileTimer), m_bCanFire(0),m_nMaxHealth(100),m_nMaxShield(200)
{
}

CBluePlayer::CBluePlayer(CPlayer* _pPlayer) : m_bCanFire(0),m_nMaxHealth(100),m_nMaxShield(200)
{
	m_pPlayer = _pPlayer;
}

/*****************************************************************
* ~CRedPlayer():	Default Destructor
* Ins:			    None    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CBluePlayer::~CBluePlayer(void)
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
void CBluePlayer::Update(float _fElapsedTime)
{
	// Set the players information
	CPlayerBehavior::Update(_fElapsedTime);
	m_pPlayer->SetType(eBLUE_PLAYER);
	m_pPlayer->SetDamage(PLAYER_MISSILE_DAMAGE);


	float fCurrVelMod = m_pPlayer->GetVelocityModifier();
	// Apply Velocity
	if(CInput::ButtonHeld(eUp))
	{
		if(fCurrVelMod < BLUE_STATE_MAX_VELOCITY )
		{
			m_pPlayer->SetVelocityModifier(min(BLUE_STATE_MAX_VELOCITY, fCurrVelMod + _fElapsedTime*BLUE_ACCELERATION));
		}
		else if(fCurrVelMod > BLUE_STATE_MAX_VELOCITY )
		{
			m_pPlayer->SetVelocityModifier(max(BLUE_STATE_MIN_VELOCITY, fCurrVelMod - _fElapsedTime*BLUE_ACCELERATION));
		}

		if(CInput::BufferedButtonPress(eUp))
		{
			WwiseNS::SoundEvent(ePlayerThrust, ePlay);
		}
	}
	else if( CInput::ButtonHeld(eDown))
	{
		if(fCurrVelMod > BLUE_STATE_MIN_VELOCITY )
		{
			m_pPlayer->SetVelocityModifier(max(BLUE_STATE_MIN_VELOCITY, fCurrVelMod - _fElapsedTime* BLUE_ACCELERATION));
		}
		if(fCurrVelMod < BLUE_STATE_MIN_VELOCITY )
		{
			m_pPlayer->SetVelocityModifier(min(BLUE_STATE_MIN_VELOCITY, fCurrVelMod + _fElapsedTime* BLUE_ACCELERATION));
		}

			WwiseNS::SoundEvent(ePlayerThrust, eStop);
	}
	else
	{
		if(fCurrVelMod < BLUE_STATE_MIN_VELOCITY)
		{
			m_pPlayer->SetVelocityModifier(fCurrVelMod + _fElapsedTime*10);
		}
		else if(fCurrVelMod > BLUE_STATE_MAX_VELOCITY )
		{
			m_pPlayer->SetVelocityModifier(fCurrVelMod - _fElapsedTime*10);
		}

		WwiseNS::SoundEvent(ePlayerThrust, eStop);
	}

	if ( false == m_bCanFire )
	{
		m_fMissileTimer -= _fElapsedTime;
	}
	if (m_fMissileTimer <= 0)
	{
		m_fMissileTimer = 5;
		m_bCanFire = true;
	}

	// Regenerate shields/health if less than max shield
	static float s_fHealthRegenTimer = 0.0f; 
	static float s_fShieldRegenTimer = 0.0f; 
	static bool s_bSoundPlay = false;

	if (s_fShieldRegenTimer >= 0.1f)
	{
		if(m_pPlayer->GetShields() < BLUE_STATE_SHIELD && m_pPlayer->GetShields() > 0)
		{
			m_pPlayer->SetShields(m_pPlayer->GetShields() + 1);

			if(m_pPlayer->GetShields() >= BLUE_STATE_SHIELD)
			{
				WwiseNS::SoundEvent(eSheildFull, ePlay);
			}

			if(!s_bSoundPlay)
			{
				s_bSoundPlay=true;
			}
		}
		else
		{
			if(s_bSoundPlay)
			{
				WwiseNS::SoundEvent(eSheildRecharge, ePlay);
				s_bSoundPlay=false;
			}
		}

		s_fShieldRegenTimer = 0.0f;
	}

	if(s_fHealthRegenTimer >= 0.3f)
	{
		if(m_pPlayer->GetHealth() < PLAYER_HEALTH)
		{
			m_pPlayer->SetHealth(m_pPlayer->GetHealth() + 1);
			if(m_pPlayer->GetHealth() >= PLAYER_HEALTH)
			{
				WwiseNS::SoundEvent(ePlayerHealthFull, ePlay);
			}
		}
#if _DEBUG
		if(CInput::ButtonHeld(eExtraButton))
		{
			cout << "Speed Mod:" << m_pPlayer->GetVelocityModifier() << endl;
		}
#endif
		s_fHealthRegenTimer = 0.0f;
	}

	s_fShieldRegenTimer += _fElapsedTime;
	s_fHealthRegenTimer += _fElapsedTime;

	// Only regenerate shields if certain amount of time has passed
	if(m_pPlayer->GetShields() <= 0)
	{
		static float s_fRegenWaitTimer = 0.0f;
		s_fRegenWaitTimer += _fElapsedTime;

		if (s_fRegenWaitTimer > 3)
		{
			m_pPlayer->SetShields(m_pPlayer->GetShields() + 1);
			s_fRegenWaitTimer = 0.0f;
		}
	}
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
* Render():		Draw effects or whatnot.
* Ins:			    None    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CBluePlayer::Render()
{
}

/*****************************************************************
* FireMissile():		Creates a missile with its target
* Ins:			    None    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
bool CBluePlayer::FireMissile(CEntity* _pTarget)
{
	if (false == m_bCanFire)
	{
		return false;
	}

	//Make a missile, set its position and set its target

	m_bCanFire = false;
	return true;
}