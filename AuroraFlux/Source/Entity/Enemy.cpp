/***********************************************
* Filename:  		Enemy.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will hold all the enemy entities data.
************************************************/
#include "../StdAfx.h"
#include "../AI/YellowShooting.h"
#include "../AI/BlueShooting.h"
#include "../AI/RedShooting.h"
#include "../AI/PathingMovement.h"
#include "../AI/LockedOnMovement.h"
#include "../AI/FlockingMovement.h"
#include "../AI/FleeMovement.h"
#include "../AI/FlankingMovement.h"
#include "../AI/ChaseMovement.h"
#include "../AI/ThinkingMovement.h"
#include "Enemy.h"
#include "../Object Manager/ObjectManager.h"
#include "../Collision/Physics.h"
/*****************************************************************
* CEnemy():		Default Constructor will pass down to its respective chlid.
* Ins:			    None    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CEnemy::CEnemy(void)
	: m_nHealth(0),m_pPlayer(0) , m_nShields(0), m_nDamage(0), m_nVelocityModifier(0), m_pCurrMoveBehavior(nullptr), m_pShootBehavior(nullptr), m_fStunPower(0.0f), m_pWaypoint(0)
{
	m_bShouldChasePlayer = false;
	D3DXMATRIX d3dTransMat;
	D3DXMatrixTranslation(&d3dTransMat,1.0f,0.0f,20.0f);
	SetMatrix(&d3dTransMat);
	SetIsHit(false);
	m_pObjectManager = NULL;

	justSpawned = false;
	justSpawnedTimer = 0.0f;
}
/*Destructor*/
CEnemy::~CEnemy(void)
{
	for (unsigned int nBehaviorIndex = 0; nBehaviorIndex < m_vpMoveBehaviors.size(); nBehaviorIndex++)
	{
		if (m_vpMoveBehaviors[nBehaviorIndex])
		{
			delete	m_vpMoveBehaviors[nBehaviorIndex];
			m_vpMoveBehaviors[nBehaviorIndex] = nullptr;
		}
	}

	m_pCurrMoveBehavior = nullptr;

	if(m_pShootBehavior)
	{
		delete m_pShootBehavior;
		m_pShootBehavior = nullptr;
	}
}
/*****************************************************************
* Update():		Will update behaviors. if any behavior stack is empty this 
*					guy will add thinking to	the stack.
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CEnemy::Update( float _fElapsedTime ) 
{
	CEntity::Update(_fElapsedTime);
	// if there arent any behaviors give them thinking
	if (m_vpMoveBehaviors.size() == 0)
	{
		if(GetType() == eRED_ENEMY || GetType() == eYELLOW_ENEMY)
		{
			WwiseNS::SoundEvent(eEnemyThrust, ePlay, false, this);
		}
		PushBehavior(new CThinkingMovement(this));
	}

	if (justSpawned)
	{
		justSpawnedTimer+= _fElapsedTime;
		if (justSpawnedTimer >= 2.5f)
		{
			justSpawnedTimer = 0.0f;
			justSpawned = false;
		}
	}
	

}
/*****************************************************************
* Render():		draw effects. most likely will do nothing here.
* Ins:			    None    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CEnemy::Render() 
{

}
/*****************************************************************
* PushBehavior():	Will add and set up a new behavior for the bottom of the stack.
* Ins:			    _behavior    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CEnemy::PushBehavior( CMovementBehavior *_pBehavior )
{
	/* Ensure the invoker isn't a dumbass. */
	if(_pBehavior == nullptr)
	{
		return;
	}
	/* Store the new state as the current state. */
	m_pCurrMoveBehavior = _pBehavior;

	/* Add the state to the stack. */
	m_vpMoveBehaviors.push_back(_pBehavior);
}
/*****************************************************************
* PopBehavior():	Will remove and clean up the last behavior off of the stack.
* Ins:			    None    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CEnemy::PopBehavior(  ) 
{
	/* Ensure the invoker isn't a dumbass. */
	if(m_vpMoveBehaviors.empty())
	{
		return;
	}

	/* Get the back (top) of the stack. */
	CMovementBehavior *pBack = m_vpMoveBehaviors.back();

	/* Free the state. */
	delete pBack;

	/* Remove the state. */
	m_vpMoveBehaviors.pop_back();

	/* Store the new current state. */
	m_pCurrMoveBehavior = m_vpMoveBehaviors.empty() ? nullptr : m_vpMoveBehaviors.back();
}
/*****************************************************************
* switchState():	Will create a new state of the passed in type and switch to it.
* Ins:			    _behavior    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CEnemy::switchState( CMovementBehavior *_pBehavior )
{
	/* Determine if there is a current state. */
	if(m_pCurrMoveBehavior)
	{
		/* Delete the current state. */
		delete m_pCurrMoveBehavior;

		/* Remove the state. */
		m_vpMoveBehaviors.pop_back();
	}

	/* Store the newly entered state. */
	m_pCurrMoveBehavior = _pBehavior;

	/* Push the new state. */
	m_vpMoveBehaviors.push_back(m_pCurrMoveBehavior);
}
/*****************************************************************
* switchShootState():	Only used for the shooting behavior to switch and set up 
*						how the enemy will shoot.
* Ins:			    _behavior    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CEnemy::switchShootState( CShootingBehavior *_pBehavior )
{
	/* Determine if there is a current state. */
	if(m_pShootBehavior)
	{
		/* Delete the current state. */
		delete m_pShootBehavior;
	}

	/* Store the newly entered state. */
	m_pShootBehavior = _pBehavior;

}
