/***********************************************
* Filename:  		WanderingMovement.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will be for when an enemy needs to chase its target.				
************************************************/
#include "../StdAfx.h"
#include "../Entity/Enemy.h"
#include "../Entity/RedEnemy.h"
#include "../Entity/BlueEnemy.h"
#include "../Entity/YellowEnemy.h"
#include "WanderingMovement.h"
#include "FleeMovement.h"
#include "..\Entity/Waypoint.h"
#include "..\Entity\Player.h"
#include "LockedOnMovement.h"
#include "../AI/DeathMovement.h"
#include "../Collision/Physics.h"
#include "../AI/ReturnMovement.h"
#include "../Object Manager/ObjectManager.h"
#include "ObstacleAvoidanceMovement.h"
#include "../AI/RollingMovement.h"
#include "../AI/ForceChaseMovement.h"

/*****************************************************************
* CWanderingMovement():  Overload Constructor will set its type. Does mostly nothing. 
* Ins:			    _pEnemy    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CWanderingMovement::CWanderingMovement(CEnemy* _pEnenmy) : CMovementBehavior(_pEnenmy), m_uiWaypointIndex(0), m_hitTimer(0.0f)
{

	if (m_pEnemy->GetShouldRun() == true)
	{
		if (m_pEnemy->GetType() == eRED_ENEMY)
		{
			m_hitTimer = ((CRedEnemy*)m_pEnemy)->GetHelper()->Getevade_timer();
			m_pEnemy->SetVelocityModifier( ((CRedEnemy*)m_pEnemy)->GetHelper()->GetredVelocityModifier() );
		}
		else if (m_pEnemy->GetType() == eBLUE_ENEMY)
		{
			m_hitTimer = ((CBlueEnemy*)m_pEnemy)->GetHelper()->Getblue_evade_timer();
			m_pEnemy->SetVelocityModifier( ((CBlueEnemy*)m_pEnemy)->GetHelper()->GetblueEvadeVelocityModifier() );
			m_pEnemy->SetTurnRate( ((CBlueEnemy*)m_pEnemy)->GetHelper()->GetblueEvadeTurnRate() );
		}
		else
		{
			m_hitTimer = ((CYellowEnemy*)m_pEnemy)->GetHelper()->Getforce_chase_timer();
			m_pEnemy->SetVelocityModifier( ((CYellowEnemy*)m_pEnemy)->GetHelper()->GetyellowChaseVelocityModifier() );

		}
	}
	//create points
	for(unsigned int waypointsToPath = 1; waypointsToPath < 4; waypointsToPath++)
	{
		D3DXVECTOR3 d3dNewPoint = D3DXVECTOR3(((((rand() % 450) - 225 ))) + m_pEnemy->GetPosition().x ,(((((rand() % 450) - 225 )  ))) + m_pEnemy->GetPosition().y,(rand() % 50 ) - 25 + m_pEnemy->GetPosition().z);
		if (d3dNewPoint.x == 0)
		{
			d3dNewPoint.x = 100;
		}
		if (d3dNewPoint.y == 0)
		{
			d3dNewPoint.y = 100;
		}
		m_d3dWanderingPoints.push_back(d3dNewPoint);
	}

	//set target to first point
	m_d3dCurrentWaypoint = m_d3dWanderingPoints[0];
	
}
/*Destructor*/
CWanderingMovement::~CWanderingMovement(void)
{

}
/*****************************************************************
* Update():		Will turn to its target.
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CWanderingMovement::Update(float _fElapedTime)
{
	if(m_pEnemy->GetHealth() <= 0)
	{
		m_pEnemy->PushBehavior(new CDeathMovement(m_pEnemy));
		return;
	}
	if (((CEnemy*)m_pEnemy)->GetType() == eRED_ENEMY && ((CRedEnemy*)m_pEnemy)->GetShouldDodge())
	{

		((CRedEnemy*)m_pEnemy)->SetShouldDodge(false);
		((CRedEnemy*)m_pEnemy)->PushBehavior(new CRollingMovement(m_pEnemy));
		return;

	}
	if (m_pEnemy->GetShouldChasePlayer())
	{
		((CRedEnemy*)m_pEnemy)->PushBehavior(new CForceChaseMovement(m_pEnemy));
		return;
	}
// 	if (m_pEnemy->GetIsStunned())
// 	{
// 		m_pEnemy->PushBehavior(new CLockedOnMovement(m_pEnemy));
// 		return;
// 	}
	CMovementBehavior::Update(_fElapedTime);
	///////////////////////////////////////////////////////////////////////////////////////
// 	if(m_pEnemy->GetObjectManager())
// 	{
// 		D3DXVECTOR3 _d3dVelocity = D3DXVECTOR3(0,0,0);
// 		std::vector<int> avoidFlags;
// 		avoidFlags.push_back(eRED_ENEMY); avoidFlags.push_back(eBLUE_ENEMY); avoidFlags.push_back(eYELLOW_ENEMY); avoidFlags.push_back(eRED_PLAYER); avoidFlags.push_back(eBLUE_PLAYER);
// 		avoidFlags.push_back(eASTEROID); avoidFlags.push_back(eLARGE_ASTEROID);
// 		_d3dVelocity += m_pEnemy->GetObjectManager()->AdjustToAvoidCollision(m_pEnemy, avoidFlags);
// 		if(D3DXVec3LengthSq(&_d3dVelocity) > 0.7)
// 		{
// 			//m_pEnemy->PushBehavior(new CObstacleAvoidanceMovement(m_pEnemy));
// 		}
// 	}
	D3DXVECTOR3 d3dVelocity = D3DXVECTOR3(0.0f,0.0f, 0.0f);
		int type = m_pEnemy->GetType();
	if (type == eRED_ENEMY)
	{
		d3dVelocity = D3DXVECTOR3(0.0f,0.0f,((CRedEnemy*)m_pEnemy)->GetHelper()->GetredVelocityModifier());
	}
	else if(type == eBLUE_ENEMY)
	{
		d3dVelocity = D3DXVECTOR3(0.0f,0.0f,((CBlueEnemy*)m_pEnemy)->GetHelper()->GetblueVelocityModifier());
	}
	else if(type == eYELLOW_ENEMY)
	{
		d3dVelocity = D3DXVECTOR3(0.0f,0.0f,((CYellowEnemy*)m_pEnemy)->GetHelper()->GetyellowVelocityModifier());
	}
	///////////////////////////////////////////////////////////////////////////////////////
	//Forward Movement
	D3DXMATRIX d3dMat;
	D3DXMatrixIdentity( &d3dMat );
	d3dMat._41 = d3dVelocity.x * _fElapedTime; 
	d3dMat._42 = d3dVelocity.y * _fElapedTime; 
	d3dMat._43 = d3dVelocity.z * _fElapedTime; 
	m_pEnemy->SetMatrix( &( d3dMat * ( *m_pEnemy->GetMatrix() ) ) );

	//turn to new wayPoints
	//////////////////////////////////////////////////////////////////////////
	CPhysics::TurnTo(m_pEnemy, &m_d3dCurrentWaypoint, _fElapedTime, m_pEnemy->GetTurnRate()); 
	//////////////////////////////////////////////////////////////////////////

// 	if(type != eYELLOW_ENEMY)
// 	{
// 
// 	
// 	//for obstacle avoidance
// 	if(m_pEnemy->GetObjectManager())
// 	{
// 		std::vector<int> avoidFlags;
// 		avoidFlags.push_back(eRED_ENEMY); avoidFlags.push_back(eBLUE_ENEMY); avoidFlags.push_back(eYELLOW_ENEMY); avoidFlags.push_back(eRED_PLAYER); avoidFlags.push_back(eBLUE_PLAYER);
// 		avoidFlags.push_back(eASTEROID); avoidFlags.push_back(eLARGE_ASTEROID);
// 		d3dVelocity += m_pEnemy->GetObjectManager()->AdjustToAvoidCollision(m_pEnemy, avoidFlags);
// 	}
// 
// 	}
	
	//early out if we're being forced to chase the target
	// 	if (m_bForceChase)
	// 	{
	// 		m_pEnemy->PopBehavior();
	// 		return;
	// 	}
	m_hitTimer-=_fElapedTime;
	CEntity* pTarget = m_pEnemy->GetTarget();

	if (m_pEnemy && m_pEnemy->GetActive())
	{
		//fix target if its missing
		CPlayer * pPlayer = ((CPlayer*)m_pEnemy->GetPlayer());

		if (!pTarget && pPlayer->GetIsVisable()  && m_hitTimer <= 0.0f)
		{
			m_pEnemy->SetTarget(pPlayer);
		}
		int nType = m_pEnemy->GetType();
		// if i see my target and im not evading go back to previous behavior
		if (pTarget && ((CPlayer*)pTarget)->GetIsVisable() && m_hitTimer <= 0.0f )
		{
			m_pEnemy->SetShouldRun(false);
			if ( nType == eRED_ENEMY)
			{
				m_pEnemy->SetVelocityModifier( ((CRedEnemy*)m_pEnemy)->GetHelper()->GetredVelocityModifier() );
				m_pEnemy->SetTurnRate( ((CRedEnemy*)m_pEnemy)->GetHelper()->GetredTurnRate() );
			}
			else if( nType == eBLUE_ENEMY)
			{
				m_pEnemy->SetVelocityModifier( ((CBlueEnemy*)m_pEnemy)->GetHelper()->GetblueVelocityModifier() );
				m_pEnemy->SetTurnRate( ((CBlueEnemy*)m_pEnemy)->GetHelper()->GetblueTurnRate() );
			}
			else
			{
				m_pEnemy->SetVelocityModifier( ((CYellowEnemy*)m_pEnemy)->GetHelper()->GetyellowVelocityModifier() );
				m_pEnemy->SetTurnRate( ((CYellowEnemy*)m_pEnemy)->GetHelper()->GetyellowTurnRate() );

			}
			m_pEnemy->PopBehavior();
			return;
		}

		//find the correcet waypoint
		if (  D3DXVec3Length( &(m_pEnemy->GetPosition() - m_d3dCurrentWaypoint ) ) < WANDERING_WAYPOINT_RANGE )
		{
			m_uiWaypointIndex++;
			if (m_uiWaypointIndex >= m_d3dWanderingPoints.size())
			{
				m_uiWaypointIndex = 0;
			}
			m_d3dCurrentWaypoint = m_d3dWanderingPoints[m_uiWaypointIndex];
		}

		if(  D3DXVec3Length( &(m_pEnemy->GetPosition() - m_pEnemy->GetWaypoint()->GetPosition() )) >= RETURN_RANGE)
		{
			m_pEnemy->PushBehavior(new CReturnMovement(m_pEnemy));
			return;
		}
	}

}