/***********************************************
* Filename:  		RedShooting.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This is the class that will handle how the red enemy shoots.
************************************************/
#include "../StdAfx.h"
#include "RedShooting.h"
#include "..\Entity\RedEnemy.h"
#include "..\Collision\Physics.h"
#include "..\Entity\Player.h"

/*****************************************************************
* CRedShooting():  Overload Constructor will set its type. Does mostly nothing. 
* Ins:			    _pEnemy    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CRedShooting::CRedShooting(CEnemy* _pEnemy) : CShootingBehavior(_pEnemy)
{
	s_fCounter = 0.0f;
}
/*Destructor*/
CRedShooting::~CRedShooting(void)
{

}
/*****************************************************************
* Update():		Will fire lasers whenever the enemy it is 
*					attached to is facing the target.
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CRedShooting::Update(float _fElapedTime)
{
	if(m_pEnemy->GetHealth() <= 0)
	{
		return;
	}
	if (m_pEnemy->GetIsStunned())
	{
		return;
	}
	if (m_pEnemy->GetJustSpawned())
	{
		return;
	}
	CEntity* pTarget = m_pEnemy->GetTarget();

	if ( pTarget && ((CPlayer*)pTarget)->GetIsVisable() )
	{
		
		s_fCounter += _fElapedTime;
		bool bCanFire = false;

		// how frequently the red enemy will shoot
		if (s_fCounter >=  ((CRedEnemy*)m_pEnemy)->GetHelper()->Getred_enemy_fire_rate() )
		{
			bCanFire = true;
		}
		//can i shoot?
		D3DXVECTOR3 d3dEnemyPosition = m_pEnemy->GetPosition();
		d3dEnemyPosition.z += m_pEnemy->GetVelocityModifier();
		D3DXVECTOR3 d3dVectorBetween = pTarget->GetPosition() - d3dEnemyPosition;
		D3DXVec3Normalize(  &d3dVectorBetween, &d3dVectorBetween );

		D3DXVECTOR3 d3dZAxis =  CPhysics::GetMat4Vec(eZAXIS,m_pEnemy->GetMatrix());
		D3DXVec3Normalize(  &d3dZAxis, &d3dZAxis );

		if(D3DXVec3Dot( &d3dVectorBetween, &d3dZAxis)  > 0.8f && D3DXVec3Length( &(d3dVectorBetween) ) < 650 )  
		{
			m_pEnemy->SetTurnRate( .05f );

			//Fire lasers at target if hes in front
			if (bCanFire)
			{
				//tell object manager to make a laser
				m_pEnemy->SetFlags(m_pEnemy->GetFlags() | eFIRE_LASER);
				s_fCounter = 0.0f;
			}
			else
			{
				m_pEnemy->SetTurnRate( .0024f );
			}

		}
	}
}