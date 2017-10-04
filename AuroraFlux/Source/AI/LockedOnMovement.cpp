/***********************************************
* Filename:  		LockedOnMovement.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class is for the blue enemy.i think this might get cut... 
*					we can just check is any missiles target is the player and blink based on distance
************************************************/
#include "../StdAfx.h"
#include "LockedOnMovement.h"
#include "../Entity/YellowEnemy.h"

/*****************************************************************
* CThinkingMovement():  Overload Constructor will set its type. Does mostly nothing. 
* Ins:			    _pEnemy    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CLockedOnMovement::CLockedOnMovement(CEnemy* _pEnenmy) : CMovementBehavior(_pEnenmy)
{
	m_nDirection = rand() % 3;
	m_nRotationXDirection = rand() % 2;
	m_nRotationYDirection = rand() % 2;
	m_nRotationZDirection = rand() % 2;

	if (m_nRotationXDirection == 0 && m_nRotationYDirection == 0 && m_nRotationYDirection == 0 )
	{
		m_nRotationXDirection = 1;
		m_nRotationZDirection = 1;
	}

}
/*Destructor*/
CLockedOnMovement::~CLockedOnMovement(void)
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

void CLockedOnMovement::Update(float _fElapedTime)
{
	static float _time = 0.0f;

	if (m_pEnemy->GetIsStunned())
	{

		if(m_nDirection == 0)
		{
			D3DXMATRIX d3dMat;
			D3DXMatrixIdentity( &d3dMat );
			d3dMat._43 = _fElapedTime * m_pEnemy->GetVelocityModifier() / 5;
			m_pEnemy->SetMatrix( &( d3dMat * ( *m_pEnemy->GetMatrix() ) ) );
			 
		}
		else if(m_nDirection == 1)
		{
			D3DXMATRIX d3dMat;
			D3DXMatrixIdentity( &d3dMat );
			d3dMat._42 = _fElapedTime * m_pEnemy->GetVelocityModifier() / 5;
			m_pEnemy->SetMatrix( &( d3dMat * ( *m_pEnemy->GetMatrix() ) ) );
		}
		else
		{
			D3DXMATRIX d3dMat;
			D3DXMatrixIdentity( &d3dMat );
			d3dMat._41 = _fElapedTime * m_pEnemy->GetVelocityModifier() / 5;
			m_pEnemy->SetMatrix( &( d3dMat * ( *m_pEnemy->GetMatrix() ) ) );
      	}

		m_pEnemy->Rotate(.6f*_fElapedTime, m_nRotationXDirection,m_nRotationYDirection,m_nRotationZDirection);

		_time += _fElapedTime;
		if (_time >= 10.0f * m_pEnemy->GetIsStunnedPower())
		{
			_time = 0.0f;
			m_pEnemy->SetIsStunned(false, 0.0f);
			m_pEnemy->PopBehavior(); 
			return;

		}

	}
	
}