/***********************************************
* Filename:  		FlankingMovement.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		Enemies will flank their target.
************************************************/
#include "../StdAfx.h"
#include "FodderMovement.h"

#include "../Entity/Enemy.h"
#include "ChaseMovement.h"
#include "FleeMovement.h"

#include "WanderingMovement.h"
#include "DeathMovement.h"
#include "..\Entity\Player.h"
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
CFodderMovement::CFodderMovement(CEnemy* _pEnenmy) : CMovementBehavior(_pEnenmy)
{
	m_fWanderTiemr = 0.0f;
}
/*Destructor*/
CFodderMovement::~CFodderMovement(void)
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
void CFodderMovement::Update(float _fElapedTime)
{
	if(m_pEnemy->GetHealth() <= 0)
	{
		m_pEnemy->PushBehavior(new CDeathMovement(m_pEnemy));
		return;
	}
	D3DXMATRIX d3dMat;
	D3DXMatrixIdentity( &d3dMat );
	d3dMat._43 = _fElapedTime * m_pEnemy->GetVelocityModifier(); 
	m_pEnemy->SetMatrix( &( d3dMat * ( *m_pEnemy->GetMatrix() ) ) );
	//////////////////////////////////////////////////////////////////////////
	CPhysics::TurnTo(m_pEnemy, &m_pEnemy->GetTarget()->GetPosition(), _fElapedTime, m_pEnemy->GetTurnRate());
	//////////////////////////////////////////////////////////////////////////


	m_fWanderTiemr += _fElapedTime;
	
	if ((m_pEnemy->GetShouldRun() && !m_bForceChase) || m_fWanderTiemr >= 5.0f)
	{
		m_fWanderTiemr = 0.0f;
		m_pEnemy->PushBehavior(new CWanderingMovement(m_pEnemy));
		return;
	}

	float fSquaredDistance = D3DXVec3Length( &(m_pEnemy->GetPosition() - m_pEnemy->GetTarget()->GetPosition() ) );

	//all other flee
	if (fSquaredDistance < 200 )
	{	
		m_pEnemy->PushBehavior(new CFleeMovement( m_pEnemy ) );
		return;
	}

}