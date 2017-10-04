/***********************************************
* Filename:  		FlankingMovement.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		Enemies will flank their target.
************************************************/
#include "../StdAfx.h"
#include "FlankingMovement.h"

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
CFlankingMovement::CFlankingMovement(CEnemy* _pEnenmy) : CMovementBehavior(_pEnenmy)
{

}
/*Destructor*/
CFlankingMovement::~CFlankingMovement(void)
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
void CFlankingMovement::Update(float _fElapedTime)
{
	if(m_pEnemy->GetHealth() <= 0)
	{
		m_pEnemy->PushBehavior(new CDeathMovement(m_pEnemy));
		return;
	}
}