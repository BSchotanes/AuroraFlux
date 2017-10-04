/***********************************************
* Filename:  		YellowShooting.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This is the class that will handle how the yellow enemy shoots.
************************************************/
#include "../StdAfx.h"
#include "YellowShooting.h"
#include "ChaseMovement.h"
#include "LockedOnMovement.h"
#include "../Entity/Player.h"
#include "../Entity/YellowEnemy.h"
#include "..\Collision\Physics.h"

/*****************************************************************
* CYellowShooting():  Overload Constructor will set its type. Does mostly nothing. 
* Ins:			    _pEnemy    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CYellowShooting::CYellowShooting(CEnemy* _pEnemy) : CShootingBehavior(_pEnemy)
{
	  doOnce = true;
	  laserTimer = 0.0f;
}
/*Destructor*/
CYellowShooting::~CYellowShooting(void)
{

}

/*****************************************************************
* Update():		Will blow up for massive damage if the player is in range.
*					Will not shoot otherwise.
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/

void CYellowShooting::Update(float _fElapedTime)
{

	if(m_pEnemy->GetHealth() <= 0)
	{
		return;
	}
	if (m_pEnemy->GetJustSpawned())
	{
		return;
	}
	CYellowEnemy * pYellow = (CYellowEnemy*)m_pEnemy;
	CPlayer* pTempPlayer = (CPlayer*)pYellow->GetPlayer();
// 	bool bChaseThem = false;
// 	static bool stunned = false;
// 	static float draintimer = 0.0f;
// 	if (pTempPlayer == nullptr)
// 	{
// 		return;
// 	}
// 	if ( pYellow->GetIsStunned()  )
// 	{
// 		return;
// 	}


	CEntity* pTarget = m_pEnemy->GetTarget();
	CYellowEnemy* pBlueEnemy = (CYellowEnemy*)m_pEnemy;

	if (pTarget && ((CPlayer*)pTarget)->GetIsVisable() )
	{
	
		laserTimer+= _fElapedTime;
		
			//can i shoot?
			D3DXVECTOR3 d3dEnemyPosition = m_pEnemy->GetPosition();
			d3dEnemyPosition.z += m_pEnemy->GetVelocityModifier(); 
			D3DXVECTOR3 d3dVectorBetween =	pTarget->GetPosition() - d3dEnemyPosition;//m_pEnemy->GetPosition();
			D3DXVec3Normalize(  &d3dVectorBetween, &d3dVectorBetween );
			D3DXVECTOR3 d3dZAxis =  CPhysics::GetMat4Vec(eZAXIS,m_pEnemy->GetMatrix());
			D3DXVec3Normalize(  &d3dZAxis, &d3dZAxis );

			if( D3DXVec3Dot( &d3dVectorBetween, &d3dZAxis)  > .95f )  
			{

				if (laserTimer >=( ((rand() % 35) + 10) / 40.0f ))
				{
					m_pEnemy->SetFlags(m_pEnemy->GetFlags() | eFIRE_LASER);
					laserTimer = 0.0f;
				}
			}	
	}	
// 	//chase if they are stealthed
// 	if (!pYellow->GetFound() && !pTempPlayer->GetIsVisable()  && D3DXVec3Length( &(pYellow->GetPosition() - pTempPlayer->GetPosition() ) ) < YELLOW_STEALTH_CHASE_RADIUS  )
// 	{
// 		bChaseThem = true;
// 	}
// 	//chase if they are NOT stealthed
// 	else if (!pYellow->GetFound() && pTempPlayer->GetIsVisable()  && D3DXVec3Length( &(pYellow->GetPosition() - pTempPlayer->GetPosition() ) ) < YELLOW_NOT_STEALTH_CHASE_RADIUS  )
// 	{
// 		bChaseThem = true;
// 	}
// 
// 	if(bChaseThem == true)
// 	{
// 		if (doOnce)
// 		{
// 			m_pEnemy->PushBehavior(new CChaseMovement(m_pEnemy));
// 			doOnce = false;
// 		}
// 		pYellow->SetFound(true);
// 		pYellow->SetVelocityModifier(pTempPlayer->GetVelocityModifier() + 10);
// 		pYellow->SetTurnRate(YELLOW_CHASE_TURN_RATE);
// 
// 
// 	}
// 	if (pYellow->GetFound() )
// 	{
// 
// 			if (D3DXVec3Length( &(pYellow->GetPosition() - pTempPlayer->GetPosition() ) ) < YELLOW_STEALTH_CHASE_RADIUS / 2)
// 			{
// 				
// 				pTempPlayer->SetVelocityModifier(PLAYER_SLOWED_VELOCITY);
// 
// 				draintimer += _fElapedTime;
// 				if(draintimer >= 0.1f)
// 				{
// 					if( pTempPlayer->GetShields() >= 0)
// 					{
// 						pTempPlayer->SetShields( pTempPlayer->GetShields() - 1);
// 					}
// 					else
// 					{
// 						pTempPlayer->SetHealth(max(pTempPlayer->GetHealth() - 30, 0));
// 						pYellow->SetActive(false);
// 					}
// 
// 					draintimer = 0.0f;
// 				}
// 				pYellow->SetVelocityModifier(pTempPlayer->GetVelocityModifier());
// 
// 
// 			}
// 			else
// 			{
// 				pYellow->SetVelocityModifier(pTempPlayer->GetVelocityModifier() + 5);
// 			}
// 		
// 	}


}