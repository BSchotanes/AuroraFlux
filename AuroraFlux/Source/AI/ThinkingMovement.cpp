/***********************************************
* Filename:  		ThinkingMovement.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This is the class that will handle how the rest of the behaviors 
*					are going to be passed out.
************************************************/
#include "../StdAfx.h"
#include "../Entity/Enemy.h"
#include "../Entity/RedEnemy.h"
#include "ThinkingMovement.h"
#include "PathingMovement.h"
#include "FleeMovement.h"
#include "FlockingMovement.h"
#include "ChaseMovement.h"
#include "WanderingMovement.h"
#include "FlankingMovement.h"
#include "FodderMovement.h"
#include "InnactiveMovement.h"
#include "AmbientFlockingMovement.h"
#include "../Entity/EnvironmentObjects.h"
#include "ReboundMovement.h"
#include "../Entity/Lasers.h"
//ThinkingState
//push chase till player dead
//if hp low, push flee
//if missile timer, push lockon
//if timer off, pop lockon
//ih shiwlds back up, pop flee
//
//if(blue_enemy)
//{
//if((blueEnemy*)m_enemy->getCanFire())
//{
//	//inside movement
//	push(lock-on);
//	//inside shooting
//	make and fire a missile the toggle bool 
//
//off
//}
//}

/*****************************************************************
* CThinkingMovement():  Overload Constructor will set its type. Does mostly nothing. 
* Ins:			    _pEnemy    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CThinkingMovement::CThinkingMovement(CEnemy* _pEnenmy) : CMovementBehavior(_pEnenmy)
{

}
/*Destructor*/
CThinkingMovement::~CThinkingMovement(void)
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
void CThinkingMovement::Update(float _fElapedTime)
{
	switch(m_pEnemy->GetType() )
	{
	case eRED_ENEMY:
		{
			m_pEnemy->PushBehavior( new CFlockingMovement(m_pEnemy));
		}
		break;

	case eMISSILE:
	case eBLUE_ENEMY:
		{
			m_pEnemy->PushBehavior( new CChaseMovement(m_pEnemy));
		}
		break;

	case eYELLOW_ENEMY:
		{
			m_pEnemy->PushBehavior( new CFodderMovement(m_pEnemy));
		}
		break;

	case eLASER:
		{
			if (((CLaser*)m_pEnemy)->GetShouldBounce())
			{
				m_pEnemy->PushBehavior( new CReboundMovement(m_pEnemy));
			}
		}
		break; 
	case eGENERATOR:
	case eTURRET_LASER:
	case eTURRET_MISSILE:
	case eHanger:
	case eTURRET_SMALL:
	case eASTEROID: 
	case eSPACE_DEBRIS: 
	case eSPACE_DEBRIS2: 
	case eSPACE_DEBRIS3: 
		{
			//m_pEnemy->PushBehavior( new CFlankingMovement(m_pEnemy));
			m_pEnemy->PushBehavior(new CInnactiveMovement(m_pEnemy));
		}
		break;
	case eLARGE_ASTEROID:
		{
			if (m_pEnemy->GetIsScenery() && ((CEnvironmentObject*)m_pEnemy)->GetIsOrbit())
			{
				//m_pEnemy->PushBehavior(new CPathingMovement(m_pEnemy));
			}
		}
	break;
	case eAMBIENT_ENEMY:
		{
			//m_pEnemy->PushBehavior(new CAmbientFlockingMovement((CAmbientFighter*)m_pEnemy));
		}
		break;
	};

}