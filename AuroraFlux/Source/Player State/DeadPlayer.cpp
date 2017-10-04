/***********************************************
* Filename:  		RedPlayer.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will hold all the red player entities data.
************************************************/
#include "../Definitions.h"
#include "../Sound/Wwise.h"
#include "DeadPlayer.h"
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
CDeadPlayer::CDeadPlayer(void) : m_bCanRoll(false), m_fRollTimer(0),m_nMaxHealth(100),m_nMaxShield(RED_STATE_SHIELD), m_bSoundPlay(false), m_pPlayer(nullptr)
{
	m_bDoWhenStart = true;
}

CDeadPlayer::CDeadPlayer(CPlayer* _pPlayer) : m_bCanRoll(false), m_fRollTimer(0),m_nMaxHealth(100),m_nMaxShield(RED_STATE_SHIELD), m_fDeathTime(5.0f), m_pPlayer(nullptr)
{
	m_pPlayer = _pPlayer;
	m_bDoWhenStart = true;
 	m_DeathInitialExplosion.SetOwner(_pPlayer);
	m_DeathImplosion.SetOwner(_pPlayer);
}

/*Destructor*/
CDeadPlayer::~CDeadPlayer(void)
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
void CDeadPlayer::Update(float _fElapsedTime)
{
	// Set the players information
	m_pPlayer->SetShields(0);
	m_pPlayer->SetVelocityModifier(50.0f);
	m_pPlayer->StopAllEffects();
	
	m_fDeathTime -= _fElapsedTime;

	D3DXMATRIX d3dManipMat;
	D3DXMatrixIdentity(&d3dManipMat);

	if(m_bDoWhenStart == true)
	{
		m_pPlayer->SetType(eDEAD_PLAYER);
		WwiseNS::SoundEvent(ePlayerDeath, ePlay);
		m_bDoWhenStart = false;
	}

	if(m_fDeathTime < 3.25f && m_fDeathTime > 0 && m_fDeathTime + _fElapsedTime > 3.25f)
	{
		EffectInformation::CreateEffect(eBLUE_ENEMY_DEATH, &m_DeathInitialExplosion, m_pPlayer, false, true);
	}

	if(m_fDeathTime < 2.25f && m_fDeathTime > 0)
	{
		if(m_fDeathTime + _fElapsedTime > 2.25)
		{
			EffectInformation::CreateEffect(ePLAYER_DEATH, &m_DeathImplosion, m_pPlayer, false, true);
		}
		if(m_fDeathTime < 2.1 && m_fDeathTime + _fElapsedTime > 2.1)
		{
			m_pPlayer->SetIsVisable(false);
		}
	}
	else if(m_fDeathTime > 0)
	{
 		D3DXMatrixRotationAxis(&d3dManipMat, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), m_fDeathTime * 10);
 		m_pPlayer->SetMatrix(&(d3dManipMat * *m_pPlayer->GetMatrix()));
	} 
	else
	{
		m_pPlayer->SetActive(false);
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
* DoRoll():		If a missile is incoming, dodge it if the timer is ready.
* Ins:			    None    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CDeadPlayer::Render()
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
void CDeadPlayer::DoRoll()
{
	if (m_bCanRoll == false)
	{
		return;
	}
}

void CDeadPlayer::CreateEffect()
{
	EffectInformation::CreateEffect(eGENERIC_EXPLOSION, &m_DeathInitialExplosion, m_pPlayer, false, true);
}

void CDeadPlayer::SetPlayer( CPlayer* _pPlayer )
{
	 m_pPlayer = _pPlayer; 
	 m_DeathInitialExplosion.SetOwner(_pPlayer);
	 m_DeathImplosion.SetOwner(_pPlayer);
}