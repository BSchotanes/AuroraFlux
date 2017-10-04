/***********************************************
* Filename:  		FleeMovement.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will be for when an enemy needs to run away from its target.				
************************************************/
#include "../StdAfx.h"
#include "../Entity/Enemy.h"
#include "../Entity/RedEnemy.h"
#include "FleeMovement.h"
#include "../Collision/Physics.h"
#include "LockedOnMovement.h"

#include "WanderingMovement.h"
#include "..\Entity\Player.h"

#include "..\Entity\RedEnemy.h"
#include "..\Entity\YellowEnemy.h"
#include "..\Entity/Waypoint.h"
#include "..\Entity\BlueEnemy.h"
#include "../AI/DeathMovement.h"
#include "../AI/ReturnMovement.h"
#include "../Object Manager/ObjectManager.h"
#include "ObstacleAvoidanceMovement.h"
#include "../AI/RollingMovement.h"
#include "../AI/ForceChaseMovement.h"

/*****************************************************************
* CFleeMovement():  Overload Constructor will set its type. Does mostly nothing. 
* Ins:			    _pEnemy    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CFleeMovement::CFleeMovement(CEnemy* _pEnenmy) : CMovementBehavior(_pEnenmy)
{

}
/*Destructor*/
CFleeMovement::~CFleeMovement(void)
{

}
/*****************************************************************
* Update():		Will run away from its target.
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CFleeMovement::Update(float _fElapedTime)
{

	if(m_pEnemy->GetHealth() <= 0)
	{
		m_pEnemy->PushBehavior(new CDeathMovement(m_pEnemy));
		return;
	}
	D3DXVECTOR3 d3dVelocity = D3DXVECTOR3(0.0f,0.0f,((CEnemy*)m_pEnemy)->GetVelocityModifier());

	//Forward Movement
	//////////////////////////////////////////////////////////////////////////
	D3DXMATRIX d3dMat;
	D3DXMatrixIdentity( &d3dMat );
	d3dMat._41 = d3dVelocity.x * _fElapedTime; 
	d3dMat._42 = d3dVelocity.y * _fElapedTime; 
	d3dMat._43 = d3dVelocity.z * _fElapedTime; 
	m_pEnemy->SetMatrix( &( d3dMat * ( *m_pEnemy->GetMatrix() ) ) );
	//////////////////////////////////////////////////////////////////////////	
	//////////////////////////////////////////////////////////////////////////
	if ( m_pEnemy->GetTarget())
	{
		CPhysics::TurnTo(m_pEnemy, m_pEnemy->GetTarget(),_fElapedTime, -1* m_pEnemy->GetTurnRate());
	}
	//////////////////////////////////////////////////////////////////////////
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
	if (m_pEnemy->GetIsStunned())
	{
		m_pEnemy->PushBehavior(new CLockedOnMovement(m_pEnemy));
		return;
	}
	CMovementBehavior::Update(_fElapedTime);
	int nType = m_pEnemy->GetType();

	if( nType!= eMISSILE && D3DXVec3Length( &(m_pEnemy->GetPosition() - m_pEnemy->GetWaypoint()->GetPosition() )) >= RETURN_RANGE)
	{
		m_pEnemy->PushBehavior(new CReturnMovement(m_pEnemy));
		return;
	}
	CEntity * pTarget = m_pEnemy->GetTarget();
	float fSquaredDistance = 0.0f;

	if( pTarget )
	{
		fSquaredDistance = D3DXVec3Length( &(m_pEnemy->GetPosition() - pTarget->GetPosition() ) );
	}
	//early out if this enemys partner was hit and is being forced to chase
	if ( nType == eRED_ENEMY && m_bForceChase && fSquaredDistance > ((CRedEnemy*)m_pEnemy)->GetHelper()->Getforce_red_to_chase_distance())
	{
		m_pEnemy->PopBehavior();
		return;
	}
	//if the target is not visible go to wandering
	if (pTarget && nType != eYELLOW_ENEMY && nType != eMISSILE && !((CPlayer*)m_pEnemy->GetPlayer())->GetIsVisable() )
	{
		m_pEnemy->PushBehavior(new CWanderingMovement( m_pEnemy ));
		return;
	}
	if (m_pEnemy->GetShouldRun() && !m_bForceChase)
	{
		m_pEnemy->PushBehavior(new CWanderingMovement(m_pEnemy));
		return;
	}
	//if your the one hit; evade
	// 	if ( m_pEnemy->GetShouldRun() )
	// 	{
	// 		m_pEnemy->PushBehavior(new CWanderingMovement(m_pEnemy));
	// 		return;
	// 	}
	// influence from partners
// 	if ( nType == eRED_ENEMY )
// 	{
// 		vector<CEntity*> vPartners = ((CRedEnemy*)m_pEnemy)->GetPartners();
// 		for (unsigned int partnerInfluence = 0; partnerInfluence <vPartners.size(); partnerInfluence++)
// 		{
// 			if ( vPartners[partnerInfluence]->GetActive() )
// 			{
// 				D3DXVECTOR3 d3dEnemyPosition = m_pEnemy->GetPosition();
// 				D3DXVECTOR3 d3dPartnerPosition = vPartners[partnerInfluence]->GetPosition();
// 				float fPartnerDistance = D3DXVec3Length( &(m_pEnemy->GetPosition() - vPartners[partnerInfluence]->GetPosition()) );
// 
// 				if(  fPartnerDistance < 15 )
// 				{
// 					if ( d3dEnemyPosition.x  > d3dPartnerPosition.x )
// 					{
// 						d3dVelocity.x += 20*_fElapedTime;
// 					}
// 					else
// 					{
// 						d3dVelocity.x -= 20*_fElapedTime;
// 					}
// 					if ( d3dEnemyPosition.y > d3dPartnerPosition.y)
// 					{
// 						d3dVelocity.y += 20*_fElapedTime;
// 					}
// 					else
// 					{
// 						d3dVelocity.y -= 20*_fElapedTime;
// 					}
// 
// 				}
// 
// 				if( fPartnerDistance > 20 )
// 				{
// 
// 					if ( d3dEnemyPosition.x  > d3dPartnerPosition.x )
// 					{
// 						d3dVelocity.x -= 20*_fElapedTime;
// 					}
// 					else
// 					{
// 						d3dVelocity.x += 20*_fElapedTime;
// 					}
// 					if ( d3dEnemyPosition.y > d3dPartnerPosition.y)
// 					{
// 						d3dVelocity.y -= 20*_fElapedTime;
// 					}
// 					else
// 					{
// 						d3dVelocity.y += 20*_fElapedTime;
// 					}
// 
// 				}
// 			}
// 		}
// 	}

	if( pTarget )
	{
		
		/////////////////////////////// exit: when to stop fleeing
		if (nType == eRED_ENEMY)
		{
			if ( fSquaredDistance > ((CRedEnemy*)m_pEnemy)->GetHelper()->Getred_to_chase_distance() )
			{
				m_pEnemy->PopBehavior();
				return;
			}
		}
		else if(nType == eBLUE_ENEMY)
		{
			if ( fSquaredDistance > ((CBlueEnemy*)m_pEnemy)->GetHelper()->Getblue_to_chase_distance() )
			{
				m_pEnemy->PopBehavior();
				return;

			}
		}
		else
		{
			if ( fSquaredDistance > ((CYellowEnemy*)m_pEnemy)->GetHelper()->Getred_to_chase_distance() - 90 )
			{
				m_pEnemy->PopBehavior();
				return;

			}
		}
		//////////////////////////////
	}
}
