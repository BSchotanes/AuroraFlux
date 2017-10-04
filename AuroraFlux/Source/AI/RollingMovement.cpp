/***********************************************
* Filename:  		FlankingMovement.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		Enemies will flank their target.
************************************************/
#include "../StdAfx.h"
#include "RollingMovement.h"

#include "../Entity/Enemy.h"
#include "ChaseMovement.h"
#include "FleeMovement.h"

#include "WanderingMovement.h"
#include "DeathMovement.h"
#include "..\Entity\Player.h"
#include "..\Entity\YellowEnemy.h"
#include "..\Entity\RedEnemy.h"
#include "../Collision/Physics.h"
/************************************ *****************************
* CFlankingMovement():  Overload Constructor will set its type. Does mostly nothing. 
* Ins:			    _pEnemy    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CRollingMovement::CRollingMovement(CEnemy* _pEnenmy) : CMovementBehavior(_pEnenmy)
{
	m_fTimeToRoll = 0.85f;
	if (_pEnenmy->GetType() == eRED_ENEMY )
	{
		((CRedEnemy*)_pEnenmy)->SetIsRolling(true);
	}


	//create points
	for(unsigned int waypointsToPath = 1; waypointsToPath < 4; waypointsToPath++)
	{
		D3DXVECTOR3 d3dNewPoint = D3DXVECTOR3(((((rand() % 1050) - 525 ))) + m_pEnemy->GetPosition().x ,(((((rand() % 1050) - 525 )  ))) + m_pEnemy->GetPosition().y,(rand() % 50 ) - 25 + m_pEnemy->GetPosition().z);
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
CRollingMovement::~CRollingMovement(void)
{

}
/*****************************************************************
* Update():		Flanking
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CRollingMovement::Update(float _fElapedTime)
{
	if(m_pEnemy->GetHealth() <= 0)
	{
		m_pEnemy->PushBehavior(new CDeathMovement(m_pEnemy));
		return;
	}
	static float rollTimer = 0.0f;
	rollTimer+=_fElapedTime;
	if (rollTimer <= .665)
	{
		m_pEnemy->Rotate(_fElapedTime * 12,0,0,1);
	}
	else
	{
		//turn to new wayPoints
		//////////////////////////////////////////////////////////////////////////
		CPhysics::TurnTo(m_pEnemy, &m_d3dCurrentWaypoint, _fElapedTime, m_pEnemy->GetTurnRate()); 
		//////////////////////////////////////////////////////////////////////////
	}
	m_fTimeToRoll-=_fElapedTime;
	if (m_fTimeToRoll <= 0.0f)
	{
		if (m_pEnemy->GetType() == eRED_ENEMY )
		{
			((CRedEnemy*)m_pEnemy)->SetIsRolling(false);
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



	D3DXVECTOR3 d3dVelocity = D3DXVECTOR3(0,0,((CEnemy*)m_pEnemy)->GetVelocityModifier());
	///// ////////////////////Foward Movement
	D3DXMATRIX d3dMat;
	D3DXMatrixIdentity( &d3dMat );
	d3dMat._41 = d3dVelocity.x * _fElapedTime; 
	d3dMat._42 = d3dVelocity.y * _fElapedTime; 
	d3dMat._43 = d3dVelocity.z * _fElapedTime; 
	m_pEnemy->SetMatrix( &( d3dMat * ( *m_pEnemy->GetMatrix() ) ) );
	//////////////////////////
	
	if (m_pEnemy->GetShouldChasePlayer())
	{
		((CRedEnemy*)m_pEnemy)->PopBehavior();
		
		return;
	}

}