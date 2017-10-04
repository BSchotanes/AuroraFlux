/***********************************************
* Filename:  		PathingMovement.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will follow waypoint targets supplied by the yellow enemy.
************************************************/
#include "../StdAfx.h"
#include "../Entity/Enemy.h"
#include "PathingMovement.h"
#include "../Collision/Physics.h"
#include "LockedOnMovement.h"
#include "../Entity/EnvironmentObjects.h"
/*****************************************************************
* CPathingMovement():  Overload Constructor will set its type. Does mostly nothing. 
* Ins:			    _pEnemy    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CPathingMovement::CPathingMovement(CEnemy* _pEnenmy) : CMovementBehavior(_pEnenmy)
{
	D3DXVECTOR3 ownerPos = ((CEnvironmentObject*)m_pEnemy )->GetOwner()->GetPosition();

	D3DXVECTOR3 pos2 = D3DXVECTOR3(ownerPos.x - 7000, ownerPos.y, ownerPos.z);
	m_vWaypoints.push_back(D3DXVECTOR3(pos2));
	pos2 = D3DXVECTOR3(ownerPos.x - 7500, ownerPos.y, ownerPos.z);
	m_vWaypoints.push_back(D3DXVECTOR3(pos2));
	waypointIndex = 0;
}
/*Destructor*/
CPathingMovement::~CPathingMovement(void)
{

}
/*****************************************************************
* Update():		Will hand out the behaviors based on what type of enemy and situation they are in.
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CPathingMovement::Update(float _fElapedTime)
{


		if (D3DXVec3Length(&( m_pEnemy->GetPosition() - m_vWaypoints[waypointIndex])) <= YELLOW_WAYPOINT_RANGE)
		{
			waypointIndex++;
			if (waypointIndex >= m_vWaypoints.size() )
			{
				waypointIndex = 0;
			}
		
		}

		D3DXMATRIX d3dMat;
		D3DXMatrixIdentity( &d3dMat );

		d3dMat._43 = _fElapedTime * m_pEnemy->GetVelocityModifier();

		GetEnemy()->SetMatrix( &( d3dMat * ( *m_pEnemy->GetMatrix() ) ) );

		CPhysics::TurnTo( m_pEnemy, &m_vWaypoints[waypointIndex], _fElapedTime, m_pEnemy->GetTurnRate() );
	
}