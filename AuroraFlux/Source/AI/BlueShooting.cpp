/***********************************************
* Filename:  		BlueShooting.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This is the class that will handle how the blue enemy shoots.
************************************************/
#include "../StdAfx.h"
#include "BlueShooting.h"
#include "..\Entity\BlueEnemy.h"
#include "..\Entity\Missile.h"
#include "..\Entity\Player.h"
#include "../Collision/Physics.h"
/*****************************************************************
* CBlueShooting():  Overload Constructor will set its type. Does mostly nothing. 
* Ins:			    _pEnemy    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CBlueShooting::CBlueShooting(CEnemy* _pEnemy) : CShootingBehavior(_pEnemy)
{
	laserTimer = 0.0f;
}
/*Destructor*/
CBlueShooting::~CBlueShooting(void)
{

}
/*****************************************************************
* Update():		Will randomly fire a missile whenever the enemy it is 
*					attached to is ready to fire a missile.
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CBlueShooting::Update(float _fElapedTime)
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
	CBlueEnemy* pBlueEnemy = (CBlueEnemy*)m_pEnemy;

	if (pTarget && ((CPlayer*)pTarget)->GetIsVisable() )
	{
		laserTimer+= _fElapedTime;
		
			//can i shoot?
			D3DXVECTOR3 d3dVectorBetween =	pTarget->GetPosition() - m_pEnemy->GetPosition();
			D3DXVec3Normalize(  &d3dVectorBetween, &d3dVectorBetween );
			D3DXVECTOR3 d3dZAxis =  CPhysics::GetMat4Vec(eZAXIS,m_pEnemy->GetMatrix()); 
			D3DXVec3Normalize(  &d3dZAxis, &d3dZAxis );



			if (laserTimer >= BLUE_ENEMY_LASER_RATE)
			{
				if( D3DXVec3Dot( &d3dVectorBetween, &d3dZAxis)  > .985f )  
				{
					m_pEnemy->SetFlags(m_pEnemy->GetFlags() | eFIRE_LASER);
					laserTimer = 0.0f;
				}
			}

			if ( pBlueEnemy->GetCanFire() )
			{

				if( D3DXVec3Dot( &d3dVectorBetween, &d3dZAxis)  > .95f )  
				{
					//tell object manager to make a missile
					m_pEnemy->SetFlags(m_pEnemy->GetFlags() | eFIRE_MISSILE);
					pBlueEnemy->SetCanFire( false );
				}	

			}

	}	
}