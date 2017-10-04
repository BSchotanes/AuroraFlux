/***********************************************
* Filename:  		Missile.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will hold all the missile entities data.
************************************************/
#include "../StdAfx.h"
#include "Missile.h"
#include "Player.h"
#include "RedEnemy.h"
#include <math.h>
#include "../AI/AIHelper.h"
#include "../AI/FleeMovement.h"
#include "../AI/RollingMovement.h"
#include "../../Source/Collision/Physics.h"
#include "../Entity/Boss.h"


/*****************************************************************
* CMissile():		Default Constructor will set up his data members
* Ins:			    None    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CMissile::CMissile(void) : m_pOwner(nullptr), m_fLifetime(0), m_bDoOnce(true)
{
	m_bShouldMakeExplosionSound = true;
	SetBV(CCollOBJ::Create(eCAP, D3DXVECTOR3(0,0,-1.5), D3DXVECTOR3(0,0,1.5), 3.0));

	SetTarget(nullptr);
	SetActive(true);

	m_nObjectType       = eMISSILE;

	m_Effect.SetOwner(this);
	m_esAsteroidDust.SetOwner(this);

	SetVelocity(D3DXVECTOR3(0,0,0));

	m_pSpaceDibris = NULL;
}
/*****************************************************************
* CMissile():		overload Constructor will take in what the missile will chase as well as where it came from.
* Ins:			    _target, _owner  		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CMissile::CMissile(CEntity* _pTarget, CEntity* _pOwner ,CAIHelper *_AIHelper) : m_pOwner(_pOwner), m_fLifetime(7.0f), m_bDoOnce(true), s_AIHelper(_AIHelper)
{
	m_bShouldMakeExplosionSound = true;
	SetBV(CCollOBJ::Create(eCAP, D3DXVECTOR3(0,0,-1.5), D3DXVECTOR3(0,0,1.5), 3.0));

	m_nObjectType	= eMISSILE;
	m_nTurnRate	    = _AIHelper->GetmissileTurnRate();
	m_nVelocityModifier = _AIHelper->GetmissileVelocityModifier();
	m_pTarget	    = _pTarget;
	SetActive(true);
	m_bIsStunned = false;
	SetHealth(100);
	m_Effect.SetOwner(this);
	m_esAsteroidDust.SetOwner(this);
	m_nTurn = rand()%2;
	SetVelocity(D3DXVECTOR3(0,0,0));
	EffectInformation::CreateEffect(eRED_ENEMY_THRUSTER, &m_ESMissileThruster, this, true, false);
	m_pSpaceDibris = NULL;
	isThrusted = false;
}
/*Destructor*/
CMissile::~CMissile(void)
{
}

void CMissile::SetOwner(CEntity* _pOwner, CEnvironmentObject* _pSpaceDibris)
{ 
	m_pOwner = _pOwner; 
	if( m_pOwner && !m_pSpaceDibris )
	{
		if( m_pOwner->GetType() == eBOSS )
		{				
			m_pSpaceDibris = _pSpaceDibris;
			if( m_pSpaceDibris )
			{
				m_pSpaceDibris->SetIsVisable(true);
				m_pSpaceDibris->SetRotation(D3DXVECTOR3((float)(rand()%2), (float)(rand()%2), (float)(rand()%2)), (float)(rand()%6-3)*0.01f);

				m_pSpaceDibris->Scale(rand()%20*0.1f+3.0f, rand()%20*0.1f+3.0f, rand()%20*0.1f+3.0f);

				m_pSpaceDibris->SetIsScenery(true);
			}
		}
	}
}

/*****************************************************************
* Update():		Will update its respective data, shields, and health.
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CMissile::Update(float _fElapedTime)
{
	CEnemy::Update(_fElapedTime);

	CPlayer* m_pPlayer = ((CPlayer*)m_pTarget);

	m_fLifetime -= _fElapedTime;

	if(GetTarget())
	{
		if(!GetTarget()->GetActive())
		{
			m_pTarget = 0;
			return;
		}
		int nTargetType = m_pTarget->GetType();

		if( nTargetType == eRED_PLAYER || nTargetType == eBLUE_PLAYER)
		{
			m_fMissileDistance = D3DXVec3Length( &(GetPosition() - m_pPlayer->GetPosition() ) );

			if(m_fMissileDistance > 120)
			{
				static float m_fLockOnTime = 0.0f;

				/*
				fBeepTime is the time between each beep when the player is locked on. It is calculated by taking the max beep time (0.2f)
				and subtracting the min beep time (0.1f) to get a value between 0 (newMin) and 0.2f (newMax). From there, I use cross multiplication 
				(newMax * Missile distance from player) / (Max distance from player before sound proc [100]) to get the value between 0 and 0.2f, then add 0.1f to 
				it to make it a number between the initial min (0.1f) and max (0.2f) beep time.
				*/
				float fBeepTime = ((0.1f * m_fMissileDistance) / 75) + 0.1f;

				if(m_fLockOnTime > fBeepTime)
				{
					WwiseNS::SoundEvent(eLockOnWarning, ePlay);

					m_fLockOnTime = 0.0f;
				}

				m_fLockOnTime += _fElapedTime;
			}
			else if(m_fMissileDistance <= 75)
			{
				static float m_fTime = 0.0f;
				if(m_fTime > 0.01f)
				{ 
					WwiseNS::SoundEvent(eLockOnWarning, ePlay);
					m_fTime = 0.0f;
				}

				m_fTime += _fElapedTime;
			}

			m_cFlags |= eLOCKED_ON;

			char cTmpPlayerFlags = ((CPlayer*)m_pTarget)->GetHasDodged();

			if(cTmpPlayerFlags)
			{		
				if(m_fMissileDistance < 75)
				{
					m_nTurnRate = 0.0f;
				}
			}
		}

	}

	/*if(GetTarget() == nullptr)
	{
	static float s_fMissileLifeTimer = 0.0f;

	if(s_fMissileLifeTimer > 1.0f)
	{
	m_nHealth = 0;
	s_fMissileLifeTimer = 0.0f;
	}

	s_fMissileLifeTimer += _fElapedTime;
	}*/

	if(m_pOwner)
	{
		if(((CEnemy*)m_pOwner)->GetHealth() <= 0)
		{
			m_fLifetime = 0.0f;
			m_ESMissileThruster.SetPlay(false);
			WwiseNS::SoundEvent(eMissileTravel, eStop, false, this);
		}
	}

	if (m_fLifetime <= 0.0f)
	{
		m_nHealth = 0;
		m_ESMissileThruster.SetPlay(false);

		if(m_bShouldMakeExplosionSound == true)
		{
			WwiseNS::SoundEvent(eMissileTravel, eStop, false, this);
		}
		//this->CreateEffect(eASTEROID_COLLISION_MISSILE);
		if( m_pOwner && m_pSpaceDibris )
		{
			if( m_pOwner->GetType() == eBOSS )
			{
				CBoss* boss = ((CBoss*)(m_pOwner));
				float z = m_pSpaceDibris->GetPosition().z;
				if( boss->GetIsVisable() && (z > 700000.0f) )
				{		
					m_pSpaceDibris->SetOwner(this);
					m_pSpaceDibris->SetShouldUpdate(true);
					m_pSpaceDibris->SetShouldRun(true);
					m_pSpaceDibris->SetActive(true);
					m_pSpaceDibris->SetPosition(GetPosition());
					m_pSpaceDibris->SetIsVisable(true);
				}
			}
		}
	}

	if(m_bDoOnce == true)
	{
		if (m_fLifetime <= 5.45f)
		{
			m_nTurn = rand()%2;
			if(m_nTurn < 1)
				m_nTurn = -1;
			SetVelocityModifier(GetVelocityModifier() * 3.7f);
			m_ESMissileThruster.SetPlay(true);
			isThrusted = true;
			m_bDoOnce = false;
		}
	}

	if(GetTarget() && m_fLifetime > 0.3f)
	{
		float _fMissileDistance = D3DXVec3Length( &(GetPosition() - m_pTarget->GetPosition() ) );

		if (_fMissileDistance > 240)
		{
			m_nTurnRate = s_AIHelper->GetmissileLowerTurnRate();
		}
		else if (_fMissileDistance < 60)
		{
			m_nTurnRate = s_AIHelper->GetmissileTurnRate() * 4;
		}
		else if(_fMissileDistance <= ((CSphere*)GetTarget()->GetBV())->GetRadius()*2)
		{
			//SetTarget(0);
			// 			this->SetTurnRate(0);
			// 			m_fLifetime = 0.3f;
		}
		else
		{
			m_nTurnRate =  s_AIHelper->GetmissileTurnRate();
		}
	}

	if (m_pCurrMoveBehavior)
	{
		m_pCurrMoveBehavior->Update(_fElapedTime);
	}
	if (m_pShootBehavior)
	{
		m_pShootBehavior->Update(_fElapedTime);
	}

	if (m_pTarget && m_pTarget->GetType() == eRED_ENEMY )
	{
		float fMissileDistance = D3DXVec3Length( &(GetPosition() - m_pTarget->GetPosition() ) );
		if (fMissileDistance <= 55.0f)
		{
			((CRedEnemy*)m_pTarget)->SetShouldDodge(true);
			((CRedEnemy*)m_pTarget)->SetCanRoll(false);
			((CRedEnemy*)m_pTarget)->SetRollTimer(0.0f);
			m_pTarget = nullptr;
			isScenery = true;
		}
	}
	else if (m_pTarget != nullptr && (m_pTarget->GetType() == eRED_PLAYER || m_pTarget->GetType() == eBLUE_PLAYER) )
	{
		if (((CPlayer*)m_pTarget)->GetHealth() <= 0)
		{
			m_pTarget = nullptr;
		}
	}
	
// 	if (m_pTarget && m_pTarget->GetType() == eRED_ENEMY)
// 	{
// 
// 		float fMissileDistance = D3DXVec3Length( &(GetPosition() - m_pTarget->GetPosition() ) );
// 		if (fMissileDistance <= 40.0f)
// 		{
// 			((CRedEnemy*)m_pTarget)->SetShouldDodge(true);
// 			((CRedEnemy*)m_pTarget)->SetCanRoll(false);
// 			((CRedEnemy*)m_pTarget)->SetRollTimer(0.0f);
// 			m_pTarget = nullptr;
// 			isScenery = true;
// 
// 		}
// 
// 	}


	//   	if(abs(D3DXVec3Dot(&m_d3dVelocity, &m_d3dVelocity)) > EPSILON * 4)
	//   	{
	//   		D3DXMATRIX d3dTransMat;
	//   
	//   		D3DXVECTOR3 d3dLocalZ; D3DXVec3Normalize(&d3dLocalZ, &CPhysics::GetMat4Vec(eZAXIS, GetMatrix()));
	//   		D3DXVECTOR3 _d3dLocalY; D3DXVec3Normalize(&_d3dLocalY, &CPhysics::GetMat4Vec(eYAXIS, GetMatrix()));
	//   		D3DXVECTOR3 d3dLocalX; D3DXVec3Normalize(&d3dLocalX, &CPhysics::GetMat4Vec(eXAXIS, GetMatrix()));
	//   
	//   
	//   		float fTmpLen = abs(D3DXVec3Length(&m_d3dVelocity));
	//   
	//   		if(m_d3dVelocity.x > EPSILON*2)
	//   		{
	//   			m_d3dVelocity.x = max(0, m_d3dVelocity.x - fTmpLen * _fElapedTime);
	//   		}
	//   		else if(m_d3dVelocity.x < -EPSILON*2)
	//   		{
	//   			m_d3dVelocity.x = min(0, m_d3dVelocity.x + fTmpLen * _fElapedTime);
	//   		}
	//   
	//   		if(m_d3dVelocity.y > EPSILON*2)
	//   		{
	//   			m_d3dVelocity.y = max(0, m_d3dVelocity.y - fTmpLen * _fElapedTime);
	//   		}
	//   		else if(m_d3dVelocity.y < -EPSILON*2)
	//   		{
	//   			m_d3dVelocity.y = min(0, m_d3dVelocity.y + fTmpLen * _fElapedTime);
	//   		}
	//   
	//   		if(m_d3dVelocity.z > EPSILON*2)
	//   		{
	//   			m_d3dVelocity.z = max(0, m_d3dVelocity.z - fTmpLen * _fElapedTime);
	//   		}
	//   		else if(m_d3dVelocity.z < -EPSILON*2)
	//   		{
	//   			m_d3dVelocity.z = min(0, m_d3dVelocity.z + fTmpLen * _fElapedTime);
	//   		}
	//   
	//   		D3DXMatrixTranslation(&d3dTransMat, m_d3dVelocity.x * _fElapedTime, m_d3dVelocity.y * _fElapedTime, m_d3dVelocity.z * _fElapedTime);
	//   
	//   		//	Rotate(_fElapedTime * rotationamount * D3DXVec3Dot(&m_d3dVelocity, &D3DXVECTOR3(0.3f,0.3f,0.3f)),rotationDirectionX,rotationDirectionY,rotationDirectionZ);
	//   
	//   		SetMatrix(&( d3dTransMat * *GetMatrix()));
	//   
	//   	}
}
/*****************************************************************
* Render():		Draw stuff
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CMissile::Render() 
{

}

void CMissile::CreateEffect(Effects eType)
{
	EffectInformation::CreateEffect(eType, &m_Effect, this, false, true);
}

void CMissile::AsteroidDust()
{
	EffectInformation::CreateEffect(eASTEROID_COLLISION_LASER, &m_esAsteroidDust, this, false, true);
}

void CMissile::DisableEffects()
{
	m_ESMissileThruster.SetPlay(false);

}

void CMissile::MissileFlash()
{
	EffectInformation::CreateEffect(eBLUE_ENEMY_FIRE_MISSILE, &m_Effect2, this, false, true);
}

void CMissile::SetLifetime(float _fLifetime) 
{ 
	m_fLifetime = _fLifetime; 
	if( m_fLifetime < 0.1)
	{
		m_bShouldMakeExplosionSound = false;
	} 
}