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
#include "../AI/DeathMovement.h"
#include "WanderingMovement.h"
#include "..\Entity\Player.h"
#include "..\Entity\YellowEnemy.h"
#include "..\Entity\RedEnemy.h"
#include "..\Entity\BlueEnemy.h"
#include "../Collision/Physics.h"
#include "..\Entity/Waypoint.h"
#include "../AI/forcechasemovement.h"
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
CForceChaseMovement::CForceChaseMovement(CEnemy* _pEnemy) : CMovementBehavior(_pEnemy)
{
	m_fChaseLifeTime = 6.0f;
}
/*Destructor*/
CForceChaseMovement::~CForceChaseMovement(void)
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
void CForceChaseMovement::Update(float _fElapedTime)
{

	if(m_pEnemy->GetHealth() <= 0)
	{
		m_pEnemy->PushBehavior(new CDeathMovement(m_pEnemy));
		return;
	}

	int nType = m_pEnemy->GetType();
	CEntity * pTarget = m_pEnemy->GetTarget();

	
		CPlayer * pPlayer = (CPlayer*)m_pEnemy->GetPlayer();
		
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
		//everything else chase
		if (nType != eMISSILE && m_pEnemy->GetTarget() )
		{
			//////////////////////////////////////////////////////////////////////////
			CPhysics::TurnTo(m_pEnemy, pPlayer, _fElapedTime, m_pEnemy->GetTurnRate() * 1.5f);
			//////////////////////////////////////////////////////////////////////////

			//distance from enemy to its target
			float fSquaredDistance = D3DXVec3Length( &(m_pEnemy->GetPosition() - m_pEnemy->GetTarget()->GetPosition() ) );

			//all other flee
// 			if (fSquaredDistance <( ((CRedEnemy*)m_pEnemy)->GetHelper()->Getforce_red_to_flee_distance() / 6)  )
// 			{	
// 				m_pEnemy->PushBehavior(new CFleeMovement( m_pEnemy ) );
// 				return;
// 			}
			//blue enemy flee
			// 			else if (nType == eBLUE_ENEMY && nType != eYELLOW_ENEMY && fSquaredDistance <  ((CBlueEnemy*)m_pEnemy)->GetHelper()->Getblue_to_flee_distance() )
			// 			{	
			// 				m_pEnemy->PushBehavior(new CFleeMovement( m_pEnemy ) );
			// 			}

		}

		m_fChaseLifeTime -= _fElapedTime;
		if (m_fChaseLifeTime <= 0)
		{
			m_fChaseLifeTime = 0.0f;
			m_pEnemy->SetShouldChasePlayer(false);
			m_pEnemy->PopBehavior();
			return;
		}
	
}