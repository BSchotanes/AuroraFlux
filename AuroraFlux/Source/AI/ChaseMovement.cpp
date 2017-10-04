/***********************************************
* Filename:  		ChaseMovement.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will be for when an enemy needs to chase its target.				
************************************************/
#include "../StdAfx.h"
#include "../Definitions.h"
#include "../Entity/Enemy.h"
#include "ChaseMovement.h"
#include "FleeMovement.h"
#include "LockedOnMovement.h"
#include "../AI/ReboundMovement.h"
#include "WanderingMovement.h"
#include "..\Entity\Player.h"
#include "..\Entity\YellowEnemy.h"
#include "..\Entity\RedEnemy.h"
#include "..\Entity\BlueEnemy.h"
#include "../Collision/Physics.h"
#include "..\Entity/Waypoint.h"
#include "../AI/DeathMovement.h"
#include "../AI/ReturnMovement.h"
#include "../Object Manager/ObjectManager.h"
#include "../Entity/Missile.h"
/*****************************************************************
* CChaseMovement():  Overload Constructor will set its type. Does mostly nothing. 
* Ins:			    _pEnemy    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CChaseMovement::CChaseMovement(CEnemy* _pEnemy) : CMovementBehavior(_pEnemy)
{

}
/*Destructor*/
CChaseMovement::~CChaseMovement(void)
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
void CChaseMovement::Update(float _fElapedTime)
{
	
	if(m_pEnemy->GetHealth() <= 0)
	{
		m_pEnemy->PushBehavior(new CDeathMovement(m_pEnemy));
		return;
	}
// 	if (m_pEnemy->GetIsStunned())
// 	{
// 		m_pEnemy->PushBehavior(new CLockedOnMovement(m_pEnemy));
// 		return;
// 	}
	int nType = m_pEnemy->GetType();
	CEntity * pTarget = m_pEnemy->GetTarget();
// 	if( nType == eBLUE_ENEMY && D3DXVec3Length( &(m_pEnemy->GetPosition() - m_pEnemy->GetWaypoint()->GetPosition() )) >= RETURN_RANGE)
// 	{
// 		m_pEnemy->PushBehavior(new CReturnMovement(m_pEnemy));
// 		return;
// 	}
	// if this enemy takes damage and needs to evade
	//if (m_pEnemy->GetShouldRun() && nType != eYELLOW_ENEMY )
	//{
	//	m_pEnemy->PushBehavior(new CWanderingMovement(m_pEnemy));
	//	return;
	//}
	//	For yellow enemy to chase
// 	if ( ((CYellowEnemy*)m_pEnemy)->GetFound() && nType == eYELLOW_ENEMY )
// 	{
// 		///// Foward Movement
// 		D3DXMATRIX d3dMat;
// 		D3DXMatrixIdentity( &d3dMat );
// 		d3dMat._43 = _fElapedTime * m_pEnemy->GetVelocityModifier();
// 		m_pEnemy->SetMatrix( &( d3dMat * ( *m_pEnemy->GetMatrix() ) ) );
// 		//////////////////////////////////////////////////////////////////////////
// 		CPhysics::TurnTo(m_pEnemy, pTarget, _fElapedTime, m_pEnemy->GetTurnRate());
// 		//////////////////////////////////////////////////////////////////////////
// 
// 		CYellowEnemy * pYellow = (CYellowEnemy*)m_pEnemy;
// 		CPlayer* pTempPlayer = (CPlayer*)pYellow->GetPlayer();
// 
// 		if (D3DXVec3Length( &(pYellow->GetPosition() - pTempPlayer->GetPosition() ) ) < YELLOW_STEALTH_CHASE_RADIUS / 2 )
// 		{
// 
// 			if(pTempPlayer->GetHasDodged())
// 			{
// 				pYellow->SetIsStunned(true, 1.0f);
// 				pYellow->PushBehavior(new CLockedOnMovement(m_pEnemy));
// 				return;
// 			}
// 		}
// 	}
// 	else 
	{
		CPlayer * pPlayer = (CPlayer*)m_pEnemy->GetPlayer();
		///////// Wandering check
		if (pTarget && nType != eYELLOW_ENEMY && nType != eMISSILE && !pPlayer->GetIsVisable() )
		{
			m_pEnemy->SetTarget(nullptr);
			m_pEnemy->PushBehavior(new CWanderingMovement( m_pEnemy ));
			return;
		}
		D3DXVECTOR3 d3dVelocity = D3DXVECTOR3(0,0,((CEnemy*)m_pEnemy)->GetVelocityModifier());
		///// ////////////////////Foward Movement
		if (nType != eMISSILE)
		{

			D3DXMATRIX d3dMat;
			D3DXMatrixIdentity( &d3dMat );
			d3dMat._41 = d3dVelocity.x * _fElapedTime; 
			d3dMat._42 = d3dVelocity.y * _fElapedTime; 
			d3dMat._43 = d3dVelocity.z * _fElapedTime; 
			m_pEnemy->SetMatrix( &( d3dMat * ( *m_pEnemy->GetMatrix() ) ) );
		//////////////////////////
		}
		if (nType == eLASER)
		{
			float fSquaredDistance = D3DXVec3Length( &(m_pEnemy->GetPosition() - m_pEnemy->GetTarget()->GetPosition() ) );

			if (pTarget->GetType() == eBLUE_ENEMY && ((CEnemy*)pTarget)->GetShields() <= 0  && fSquaredDistance <= 170 )
			{
				m_pEnemy->PushBehavior(new CReboundMovement( m_pEnemy ));
				return;
			}
			else if (pTarget->GetType() == eHanger && ((CEnemy*)pTarget)->GetShields() <= 0  && fSquaredDistance <= 170 )
			{
				m_pEnemy->PushBehavior(new CReboundMovement( m_pEnemy ));
				return;
			}
		}
		//Missile chase
		if (nType == eMISSILE)
		{
			if (((CMissile*)m_pEnemy)->GetIsThrusted())
			{
				d3dVelocity = D3DXVECTOR3(0,0,((CEnemy*)m_pEnemy)->GetVelocityModifier());
				D3DXMATRIX d3dMat;
				D3DXMatrixIdentity( &d3dMat );
				d3dMat._41 = d3dVelocity.x * _fElapedTime; 
				d3dMat._42 = d3dVelocity.y * _fElapedTime; 
				d3dMat._43 = d3dVelocity.z * _fElapedTime; 
				m_pEnemy->SetMatrix( &( d3dMat * ( *m_pEnemy->GetMatrix() ) ) );
				//////////////////////////////////////////////////////////////////////////
				CPhysics::TurnTo(m_pEnemy, pTarget, _fElapedTime, m_pEnemy->GetTurnRate());
				//////////////////////////////////////////////////////////////////////////
				if ( pTarget && pTarget->GetActive())
				{
				
					float fSquaredDistance = D3DXVec3Length( &(m_pEnemy->GetPosition() - m_pEnemy->GetTarget()->GetPosition() ) );

					if (pTarget->GetType() == eBLUE_ENEMY && ((CEnemy*)pTarget)->GetShields() > 0  && fSquaredDistance <= 70 )
					{
						m_pEnemy->PushBehavior(new CReboundMovement( m_pEnemy ));
						return;
					}
					else if (pTarget->GetType() == eHanger && ((CEnemy*)pTarget)->GetShields() > 0  && fSquaredDistance <= 90 )
					{
						m_pEnemy->PushBehavior(new CReboundMovement( m_pEnemy ));
						return;
					}
					else if (pTarget->GetType() == eGENERATOR && fSquaredDistance <= 70 )
					{
						m_pEnemy->PushBehavior(new CReboundMovement( m_pEnemy ));
						return;
					}
				}

			}
			else
			{
				d3dVelocity = D3DXVECTOR3(0,-35,((CEnemy*)m_pEnemy)->GetVelocityModifier());
				D3DXMATRIX d3dMat;
				D3DXMatrixIdentity( &d3dMat );
				d3dMat._41 = d3dVelocity.x * _fElapedTime; 
				d3dMat._42 = d3dVelocity.y * _fElapedTime; 
				d3dMat._43 = d3dVelocity.z * _fElapedTime; 
				m_pEnemy->SetMatrix( &( d3dMat * ( *m_pEnemy->GetMatrix() ) ) );
			}

		}
		//everything else chase
		if (nType != eMISSILE && m_pEnemy->GetTarget() && pPlayer && pPlayer->GetIsVisable() )
		{
			//////////////////////////////////////////////////////////////////////////
			CPhysics::TurnTo(m_pEnemy, pPlayer, _fElapedTime, m_pEnemy->GetTurnRate());
			//////////////////////////////////////////////////////////////////////////

			//distance from enemy to its target
			float fSquaredDistance = D3DXVec3Length( &(m_pEnemy->GetPosition() - m_pEnemy->GetTarget()->GetPosition() ) );

			//all other flee
			if (nType != eBLUE_ENEMY && nType != eYELLOW_ENEMY && fSquaredDistance < ((CRedEnemy*)m_pEnemy)->GetHelper()->Getforce_red_to_flee_distance()  )
			{	
				m_pEnemy->PushBehavior(new CFleeMovement( m_pEnemy ) );
			}
			//blue enemy flee
// 			else if (nType == eBLUE_ENEMY && nType != eYELLOW_ENEMY && fSquaredDistance <  ((CBlueEnemy*)m_pEnemy)->GetHelper()->Getblue_to_flee_distance() )
// 			{	
// 				m_pEnemy->PushBehavior(new CFleeMovement( m_pEnemy ) );
// 			}

		}
	}
}