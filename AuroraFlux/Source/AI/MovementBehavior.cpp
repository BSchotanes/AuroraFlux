/***********************************************
* Filename:  		MovementBehavior.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This is the class that will help control each of
*					the movement behaviors.This will choose which
*					respective movement behavior for the entity to choose
************************************************/
#include "../StdAfx.h"
#include "../Entity/Enemy.h"
#include "../Entity/RedEnemy.h"
#include "../Entity/BlueEnemy.h"
#include "MovementBehavior.h"
#include "BlueShooting.h"
#include "RedShooting.h"
#include "YellowShooting.h"
/*****************************************************************
* CMovementBehavior():     Overload Constructor will set its type. Does mostly nothing. 
* Ins:			    _pEnemy    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CMovementBehavior::CMovementBehavior(CEnemy* _pEnenmy) : CBehavior(_pEnenmy), m_bForceChase(false)
{
}
/*****************************************************************
* Update():		Will pass down the updates to its children. 
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/

CMovementBehavior::~CMovementBehavior(void)
{

}

void CMovementBehavior::Update(float _fElapedTime)
{
	//CBlueMovement* bluMov = new CBlueMovement(GetEnemy());
	//GetEnemy().SetMovementBehavior( bluMov );
	static float s_fChaseTimer = 0.0f;
	if (m_bForceChase)
	{
		s_fChaseTimer += _fElapedTime;
		CAIHelper *tmpHelp;
		switch (m_pEnemy->GetType())
		{
		case eRED_ENEMY:
			tmpHelp = ((CRedEnemy*)m_pEnemy)->GetHelper();
			break;
		case eBLUE_ENEMY:
			tmpHelp = ((CBlueEnemy*)m_pEnemy)->GetHelper();
			break;		
		}

		if (s_fChaseTimer >=  tmpHelp->Getforce_chase_timer() )
		{
			s_fChaseTimer = 0.0f;
			m_bForceChase = false;
		}

	}

	char cFlagHolder = m_pEnemy->GetFlags();
	cFlagHolder |= eAVOIDANCE ;
	m_pEnemy->SetFlags(  cFlagHolder );


}