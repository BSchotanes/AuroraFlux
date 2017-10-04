/***********************************************
* Filename:  		ReturnMovement.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		Enemies will flank their target.
************************************************/
#include "../StdAfx.h"

#include "../Entity/Enemy.h"
#include "ReturnMovement.h"
#include "FleeMovement.h"

#include "WanderingMovement.h"
#include "DeathMovement.h"
#include "..\Entity\Player.h"
#include "..\Entity\YellowEnemy.h"
#include "..\Entity\RedEnemy.h"
#include "..\Entity/Waypoint.h"
#include "../Collision/Physics.h"
#include "../Object Manager/ObjectManager.h"
#include "ObstacleAvoidanceMovement.h"
#include "../AI/RollingMovement.h"
#include "ForceChaseMovement.h"

/************************************ *****************************
* CFlankingMovement():  Overload Constructor will set its type. Does mostly nothing. 
* Ins:			    _pEnemy    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CReturnMovement::CReturnMovement(CEnemy* _pEnenmy) : CMovementBehavior(_pEnenmy)
{

}
/*Destructor*/
CReturnMovement::~CReturnMovement(void)
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
void CReturnMovement::Update(float _fElapedTime)
{


	if(m_pEnemy->GetHealth() <= 0)
	{
		m_pEnemy->PushBehavior(new CDeathMovement(m_pEnemy));
		return;
	}
	if (m_pEnemy->GetShouldChasePlayer())
	{
		((CRedEnemy*)m_pEnemy)->PushBehavior(new CForceChaseMovement(m_pEnemy));
		return;
	}
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
//	if (m_pEnemy->GetIsStunned())
	//{
		//m_pEnemy->PushBehavior(new CLockedOnMovement(m_pEnemy));
//		return;
//	}
	CMovementBehavior::Update(_fElapedTime);

	CEntity * pTarget = m_pEnemy->GetTarget();

	D3DXMATRIX d3dMat;
	D3DXMatrixIdentity( &d3dMat );
	d3dMat._43 = _fElapedTime * m_pEnemy->GetVelocityModifier(); 
	m_pEnemy->SetMatrix( &( d3dMat * ( *m_pEnemy->GetMatrix() ) ) );

	float fSquaredDistance = D3DXVec3Length( &(m_pEnemy->GetPosition() - m_pEnemy->GetWaypoint()->GetPosition() ) );
	//all other flee
	if (fSquaredDistance < 1800  )
	{
		m_pEnemy->PopBehavior();
		return;
	}
	//Missile chase
	CPhysics::TurnTo(m_pEnemy, m_pEnemy->GetWaypoint(), _fElapedTime, m_pEnemy->GetTurnRate());

	if (((CEnemy*)m_pEnemy)->GetType() == eRED_ENEMY && ((CRedEnemy*)m_pEnemy)->GetShouldDodge())
	{

		((CRedEnemy*)m_pEnemy)->SetShouldDodge(false);
		((CRedEnemy*)m_pEnemy)->PushBehavior(new CRollingMovement(m_pEnemy));
		return;

	}

}