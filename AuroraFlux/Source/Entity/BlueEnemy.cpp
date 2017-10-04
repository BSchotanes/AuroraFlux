/***********************************************
* Filename:  		BlueEnemy.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will hold all the Blue enemy entities unique data.
************************************************/
#include "../StdAfx.h"
#include "../AI/ThinkingMovement.h"
#include "../AI/MovementBehavior.h"
#include "BlueEnemy.h"
#include "../AI/BlueShooting.h"
#include "../Entity/Shield.h"
#include "../AI/AIHelper.h"
#include "../Collision/Physics.h"
/*****************************************************************
* CBlueEnemy():	Default Constructor will set its type and all of his internal data.
* Ins:			    None    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CBlueEnemy::CBlueEnemy(void) : m_fMissileTimer(0), m_bCanFire(false), m_bLockingOn(false)
{

	m_nObjectType	    = eBLUE_ENEMY;
	m_nHealth	    = s_AIHelper->GetHealth();
	m_nShields	    = s_AIHelper->GetblueShield();
	m_nDamage	    = s_AIHelper->GetmissileDamage();
	m_nVelocityModifier = s_AIHelper->GetblueVelocityModifier();
	m_nTurnRate	    = s_AIHelper->GetblueTurnRate();
	m_pTarget	    = nullptr;
	m_bActive	    = true;
	m_bIsStunned = (false);
	m_cFlags |= eAVOIDANCE;

	SetIsHit(false);

	SetBV(CCollOBJ::Create(eSPH, D3DXVECTOR3(0,0,0), 20.0));
	SetShieldBV(CCollOBJ::Create(eSPH, D3DXVECTOR3(0,0,0), 20.0));
	switchShootState(new CBlueShooting(this));

	m_d3dVelocity = D3DXVECTOR3(0,0,0);

	m_pEnemyShield = new CShield(this);

	EffectInformation::CreateEffect(eBLUE_ENEMY_DAMAGED, &m_esDamage, this, true, false);

	SetEntityMass(BLUE_ENEMY_MASS);

}
/*****************************************************************
* CBlueEnemy():	Overload Constructor will set its type and all of his
*					internal data as well as set its target to the passed in one.
* Ins:			    _target    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CBlueEnemy::CBlueEnemy(CEntity* _pTarget, CAIHelper * _AIHelper,CWaypoint* _pCurrWaypoint, CObjectManager* _pObjManager) : m_fMissileTimer(6), m_bCanFire(false), m_bLockingOn(false)
{
	s_AIHelper = _AIHelper;
	m_nObjectType	    = eBLUE_ENEMY;
	m_nHealth	    = s_AIHelper->GetHealth();
	m_nShields	    = s_AIHelper->GetblueShield();
	m_nDamage	    = s_AIHelper->GetmissileDamage();
	m_nVelocityModifier = s_AIHelper->GetblueVelocityModifier();
	m_nTurnRate	    = s_AIHelper->GetblueTurnRate();
	m_pTarget	    = _pTarget;
	m_bActive	    = true;
	m_bIsStunned = (false);
	m_cFlags |= eAVOIDANCE;
			    
	m_pPlayer	    = _pTarget;
	m_pWaypoint = _pCurrWaypoint;
	SetIsHit(false);

	SetBV(CCollOBJ::Create(eSPH, D3DXVECTOR3(0,0,0), 30.0));
//	SetShieldBV(CCollOBJ::Create(eSPH, D3DXVECTOR3(0,0,0), 30.0));
	switchShootState(new CBlueShooting(this));
	m_pEnemyShield = new CShield(this);
	m_pEnemyShield->Scale(4.5,4.5,4.5);
	m_pEnemyShield->SetActive(true);

	EffectInformation::CreateEffect(eBLUE_ENEMY_DAMAGED, &m_esDamage, this, true, false);

	m_d3dVelocity = D3DXVECTOR3(0,0,0);

	SetEntityMass(BLUE_ENEMY_MASS);
	m_pObjectManager = _pObjManager;
}

/*Destructor*/
CBlueEnemy::~CBlueEnemy(void)
{
	
}

void CBlueEnemy::UpdateHelperAI(float _elapsedTime)
{
	this->m_nVelocityModifier = s_AIHelper->GetblueVelocityModifier();
	this->m_nTurnRate = s_AIHelper->GetblueTurnRate();


}
/*****************************************************************
* Update():		Will update its respective shooting and movement behaviors.
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CBlueEnemy::Update(float _fElapedTime)
{

	CEnemy::Update(_fElapedTime);

	

	//TODO: REMOVE THIS WHEN AI IS FINISHED

	m_fMissileTimer -= _fElapedTime;
	if ( m_fMissileTimer <= 0 )
	{
		int nRandFireTimer = rand() % 5 + (int)s_AIHelper->Getblue_enemy_missile_fire_timer();
		m_fMissileTimer = (float)nRandFireTimer;
		m_bCanFire = true;

	}

	if(m_bShieldVisible)
	{
		if(m_nShields <= 0)
		{
			m_bShieldVisible = false;
			SetMesh(m_nInvisibleShieldMesh);
		}
	}
	else
	{
		if(m_nShields > 0)
		{
			m_bShieldVisible = true;
			SetMesh(m_nVisibleShieldMesh);
		}
	}


	if(m_pTarget && D3DXVec3Length( &(GetPosition() - m_pTarget->GetPosition() )) >= BLUE_CHASE_RANGE)
	{
		SetVelocityModifier( 30 );
		m_bCanFire = false;
	}
	else
	{
		SetVelocityModifier( 0 );

	}


	if (m_pCurrMoveBehavior)
	{
		m_pCurrMoveBehavior->Update(_fElapedTime);
	}
	if (m_pShootBehavior)
	{
		m_pShootBehavior->Update(_fElapedTime);
	}


// 	if(abs(D3DXVec3Dot(&m_d3dVelocity, &m_d3dVelocity)) > EPSILON * 4)
// 	{
// 		D3DXMATRIX d3dTransMat;
// 
// 		D3DXVECTOR3 d3dLocalZ; D3DXVec3Normalize(&d3dLocalZ, &CPhysics::GetMat4Vec(eZAXIS, GetMatrix()));
// 		D3DXVECTOR3 _d3dLocalY; D3DXVec3Normalize(&_d3dLocalY, &CPhysics::GetMat4Vec(eYAXIS, GetMatrix()));
// 		D3DXVECTOR3 d3dLocalX; D3DXVec3Normalize(&d3dLocalX, &CPhysics::GetMat4Vec(eXAXIS, GetMatrix()));
// 
// 
// 		float fTmpLen = abs(D3DXVec3Length(&m_d3dVelocity));
// 
// 		if(m_d3dVelocity.x > EPSILON*2)
// 		{
// 			m_d3dVelocity.x = max(0, m_d3dVelocity.x - fTmpLen * _fElapedTime);
// 		}
// 		else if(m_d3dVelocity.x < -EPSILON*2)
// 		{
// 			m_d3dVelocity.x = min(0, m_d3dVelocity.x + fTmpLen * _fElapedTime);
// 		}
// 
// 		if(m_d3dVelocity.y > EPSILON*2)
// 		{
// 			m_d3dVelocity.y = max(0, m_d3dVelocity.y - fTmpLen * _fElapedTime);
// 		}
// 		else if(m_d3dVelocity.y < -EPSILON*2)
// 		{
// 			m_d3dVelocity.y = min(0, m_d3dVelocity.y + fTmpLen * _fElapedTime);
// 		}
// 
// 		if(m_d3dVelocity.z > EPSILON*2)
// 		{
// 			m_d3dVelocity.z = max(0, m_d3dVelocity.z - fTmpLen * _fElapedTime);
// 		}
// 		else if(m_d3dVelocity.z < -EPSILON*2)
// 		{
// 			m_d3dVelocity.z = min(0, m_d3dVelocity.z + fTmpLen * _fElapedTime);
// 		}
// 
// 		D3DXMatrixTranslation(&d3dTransMat, m_d3dVelocity.x * _fElapedTime, m_d3dVelocity.y * _fElapedTime, m_d3dVelocity.z * _fElapedTime);
// 
// 		SetMatrix(&(*GetMatrix() * d3dTransMat));
// 
// 	}

	float currHealthPercentage = (float)m_nHealth / (float)s_AIHelper->GetHealth();


	if(currHealthPercentage < .95f )
	{
		m_esDamage.SetEmitterRate(currHealthPercentage);
		m_esDamage.SetPlay(true);

// 		if(currHealthPercentage < .50f)
// 		{
// 			m_esDamage.SetEmitterRate(.3f);
// 
// 			if(currHealthPercentage < .25f)
// 			{
// 				m_esDamage.SetEmitterRate(.1f);
// 			}
// 
// 		}
	}

}

/*****************************************************************
* Render():		Draw effects or whatnot.
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CBlueEnemy::Render() 
{

}

void CBlueEnemy::CreateArrivalEffect()
{
	EffectInformation::CreateEffect(eRED_ENEMY_ARRIVAL, &m_esArrival, this, false, true);
}