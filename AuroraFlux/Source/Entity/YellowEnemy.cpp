/***********************************************
* Filename:  		YellowEnemy.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This is what will contain the 
*					yellow enemys personal data.				
************************************************/

#include "../StdAfx.h"
#include "../AI/YellowShooting.h"
#include "../AI/MovementBehavior.h"
#include "YellowEnemy.h"
#include "../Entity/Shield.h"
#include "../Collision/Physics.h"
/*****************************************************************
* CYellowEnemy():  Default Constructor. Will set its type and behaviors.
* Ins:			    None    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CYellowEnemy::CYellowEnemy(void) : m_fSightRadius(50.0f), m_unWaypointIndex(0), m_bFoundPlayer(0), m_pThePlayer(0)
{
	needAnArrow = false;
	m_vpWaypoints.clear();
	m_nObjectType = eYELLOW_ENEMY;
	/*m_nHealth = ENEMY_HEALTH;
	m_nShields = YELLOW_ENEMY_SHIELD;
	m_nDamage = YELLOW_ENEMY_DAMAGE;
	m_nVelocityModifier = YELLOW_VELOCITY_MODIFIER;
	m_nTurnRate = YELLOW_TURN_RATE;*/
	m_pTarget = nullptr;
	SetBV(CCollOBJ::Create(eCAP, D3DXVECTOR3(0,0,0), D3DXVECTOR3(0,-1,0), 3.0));
	m_bActive = true;
	SetIsHit(false);
	m_bIsStunned = (false);
	switchShootState(new CYellowShooting(this));
	m_pEnemyShield = new CShield(this);

	m_d3dVelocity = D3DXVECTOR3(0,0,0);

}
/*****************************************************************
* CYellowEnemy():  Overload Constructor. Will set its type, target, and behaviors. 
*					Sets the target parameter to its target member.
* Ins:			    _target    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CYellowEnemy::CYellowEnemy(CEntity* _pTarget,CAIHelper *_AIHelper ,CWaypoint * _pWaypoint, CObjectManager* _pObjectManager) : m_unWaypointIndex(0), m_bFoundPlayer(0)
{
	s_AIHelper = _AIHelper;
	m_nObjectType = eYELLOW_ENEMY;
	m_nHealth			= _AIHelper->GetyellowShield();
	m_nShields			= 0;
	m_nDamage			= _AIHelper->GetyellowDamage();
	m_nVelocityModifier	= _AIHelper->GetyellowVelocityModifier();
	m_nTurnRate			= _AIHelper->GetyellowTurnRate();
	m_bActive = true;
	m_pTarget			= _pTarget;
	m_pWaypoint = _pWaypoint;
	SetIsHit(false);
	m_bIsStunned = (false);
	m_cFlags |= eAVOIDANCE;
	m_pThePlayer = _pTarget;
	needAnArrow = false;
	m_d3dVelocity = D3DXVECTOR3(0,0,0);

	SetBV(CCollOBJ::Create(eSPH, D3DXVECTOR3(0,0,0), 5.0));
	SetIsHit(false);
	m_bIsStunned = (false);

	switchShootState(new CYellowShooting(this));
	//m_pEnemyShield = new CShield(this);

	EffectInformation::CreateEffect(eRED_ENEMY_THRUSTER, &m_esEngineTrail, this, true, false);
	EffectInformation::CreateEffect(eRED_ENEMY_DAMAGED, &m_esDamage, this, true, false);
	m_pObjectManager = _pObjectManager;

}
/*****************************************************************
* ~CYellowEnemy(): Destructor, probably doing nothing
* Ins:			    None    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CYellowEnemy::~CYellowEnemy(void)
{
// 	for (unsigned int waypts = 0; waypts < m_vpWaypoints.size(); waypts++)
// 	{ 
// 		if (m_vpWaypoints[waypts])	
// 		{
// 			delete m_vpWaypoints[waypts];
// 		}
// 
// 		m_vpWaypoints[waypts] = 0;
// 	}

	if(m_pEnemyShield != nullptr)
	{
		delete m_pEnemyShield;
		m_pEnemyShield = nullptr;
	}
}
/*****************************************************************
* Update():		Will update the enemys internal timers and update the behaviors 
*					based on the elapsed time.
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CYellowEnemy::Update(float _fElapedTime)
{
	CEnemy::Update(_fElapedTime);

	//if hes too close to his target, switch target to next in the list



// 	if (!m_bFoundPlayer && D3DXVec3Length(&( GetPosition() - m_pTarget->GetPosition())) <= YELLOW_WAYPOINT_RANGE)
// 	{
// 		m_unWaypointIndex++;
// 		if (m_unWaypointIndex >= m_vpWaypoints.size() )
// 		{
// 			m_unWaypointIndex = 0;
// 		}
// 
// 		SetTarget( m_vpWaypoints[m_unWaypointIndex] );
// 	}
// 	if(m_bFoundPlayer)
// 	{
// 		SetTarget( m_pThePlayer );
// 	}

	if (m_pCurrMoveBehavior)
	{
		m_pCurrMoveBehavior->Update(_fElapedTime);
	}
	if (m_pShootBehavior)
	{
		m_pShootBehavior->Update(_fElapedTime);
	}

	if(abs(D3DXVec3Dot(&m_d3dVelocity, &m_d3dVelocity)) > EPSILON * 4)
	{
		D3DXMATRIX d3dTransMat;

		D3DXVECTOR3 d3dLocalZ; D3DXVec3Normalize(&d3dLocalZ, &CPhysics::GetMat4Vec(eZAXIS, GetMatrix()));
		D3DXVECTOR3 _d3dLocalY; D3DXVec3Normalize(&_d3dLocalY, &CPhysics::GetMat4Vec(eYAXIS, GetMatrix()));
		D3DXVECTOR3 d3dLocalX; D3DXVec3Normalize(&d3dLocalX, &CPhysics::GetMat4Vec(eXAXIS, GetMatrix()));


		float fTmpLen = abs(D3DXVec3Length(&m_d3dVelocity))*0.6f;

		if(m_d3dVelocity.x > EPSILON*2)
		{
			m_d3dVelocity.x = max(0, m_d3dVelocity.x - fTmpLen * _fElapedTime);
		}
		else if(m_d3dVelocity.x < -EPSILON*2)
		{
			m_d3dVelocity.x = min(0, m_d3dVelocity.x + fTmpLen * _fElapedTime);
		}

		if(m_d3dVelocity.y > EPSILON*2)
		{
			m_d3dVelocity.y = max(0, m_d3dVelocity.y - fTmpLen * _fElapedTime);
		}
		else if(m_d3dVelocity.y < -EPSILON*2)
		{
			m_d3dVelocity.y = min(0, m_d3dVelocity.y + fTmpLen * _fElapedTime);
		}

		if(m_d3dVelocity.z > EPSILON*2)
		{
			m_d3dVelocity.z = max(0, m_d3dVelocity.z - fTmpLen * _fElapedTime);
		}
		else if(m_d3dVelocity.z < -EPSILON*2)
		{
			m_d3dVelocity.z = min(0, m_d3dVelocity.z + fTmpLen * _fElapedTime);
		}

		D3DXMatrixTranslation(&d3dTransMat, m_d3dVelocity.x * _fElapedTime, m_d3dVelocity.y * _fElapedTime, m_d3dVelocity.z * _fElapedTime);

		SetMatrix(&(*GetMatrix() * d3dTransMat));

	}


	if(!m_esEngineTrail.GetPlay())
	{
		m_esEngineTrail.ResetPosition();
		m_esEngineTrail.SetPlay(true);
	}
	float currHealthPercentage = (float)m_nHealth / (float)s_AIHelper->GetyellowShield();


	if(currHealthPercentage < .90f )
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
* Render():		Draws the boss effects.				
* Ins:			    None    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CYellowEnemy::Render() 
{

}

void CYellowEnemy::AddWaypoint(CEntity* _pWpt)
{
	m_vpWaypoints.push_back(_pWpt);
}


void CYellowEnemy::CreateArrivalEffect()
{
	EffectInformation::CreateEffect(eRED_ENEMY_ARRIVAL, &m_esArrival, this, false, true);
}