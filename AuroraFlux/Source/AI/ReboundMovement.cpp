/***********************************************
* Filename:  		FlankingMovement.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		Enemies will flank their target.
************************************************/
#include "../StdAfx.h"
#include "ReboundMovement.h"

#include "../Entity/Enemy.h"
#include "ChaseMovement.h"
#include "FleeMovement.h"

#include "WanderingMovement.h"
#include "DeathMovement.h"
#include "..\Entity\Player.h"
#include "..\Entity/Missile.h"
#include "..\Entity\YellowEnemy.h"
#include "../Collision/Physics.h"
/************************************ *****************************
* CFlankingMovement():  Overload Constructor will set its type. Does mostly nothing. 
* Ins:			    _pEnemy    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CReboundMovement::CReboundMovement(CEnemy* _pEnenmy) : CMovementBehavior(_pEnenmy)
{
	// make waypoints
	D3DXVECTOR3 enemyPos =  m_pEnemy->GetPosition();
	int randomNum = ((rand() % 150) - 75);
	if (randomNum <= 10 && randomNum >= -10 )
	{
		randomNum = 40;
	}
	upWaypoint = D3DXVECTOR3(enemyPos.x + ((rand() % 30) - 30), enemyPos.y + randomNum , enemyPos.z + ((rand() % 30) - 30));
	//make timer
	m_fKillTimer = 0.0f;
	m_pEnemy->SetVelocityModifier( 100 );

}
/*Destructor*/
CReboundMovement::~CReboundMovement(void)
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
void CReboundMovement::Update(float _fElapedTime)
{
	m_fKillTimer += _fElapedTime;
	if(m_pEnemy->GetHealth() <= 0)
	{
		m_pEnemy->PushBehavior(new CDeathMovement(m_pEnemy));
		return;
	}
	
	D3DXVECTOR3 d3dVelocity = D3DXVECTOR3(0,0,(m_pEnemy)->GetVelocityModifier());
	if (m_pEnemy->GetType() == eLASER)
	{
		d3dVelocity = D3DXVECTOR3(0,0,(m_pEnemy)->GetVelocityModifier() * .8f);
	}
	
	D3DXMATRIX d3dMat;
	D3DXMatrixIdentity( &d3dMat );
	d3dMat._41 = d3dVelocity.x * _fElapedTime; 
	d3dMat._42 = d3dVelocity.y * _fElapedTime; 
	d3dMat._43 = d3dVelocity.z * _fElapedTime; 

	m_pEnemy->SetMatrix( &( d3dMat * ( *m_pEnemy->GetMatrix() ) ) );

	if (m_pEnemy->GetType() == eMISSILE)
	{
		CPhysics::TurnTo(m_pEnemy, &upWaypoint, _fElapedTime, 0.08f);

	}
	else
	{
		CPhysics::TurnTo(m_pEnemy, &upWaypoint, _fElapedTime, 0.68f);

	}

	if ( m_fKillTimer >= .855f )
	{
		if (m_pEnemy->GetType() == eMISSILE)
		{
			((CMissile*)m_pEnemy)->CreateEffect(eMISSILE_HULL);
		}
		m_fKillTimer = 0.0f;
		m_pEnemy->SetHealth(0);
		return;
	}

}