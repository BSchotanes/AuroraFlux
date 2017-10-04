/***********************************************
* Filename:  		LockedOnMovement.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class is for the blue enemy.i think this might get cut... 
*					we can just check is any missiles target is the player and blink based on distance
************************************************/
#ifndef CLOCKEDONMOVEMENT_H
#define	CLOCKEDONMOVEMENT_H

#include "MovementBehavior.h"

class  CLockedOnMovement  : public CMovementBehavior
{

	int m_nDirection;
	int m_nRotationXDirection;
	int m_nRotationYDirection;
	int m_nRotationZDirection;

public:
	/*****************************************************************
	* CThinkingMovement():  Overload Constructor will set its type. Does mostly nothing. 
	* Ins:			    _pEnemy    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CLockedOnMovement(CEnemy* _pEnenmy );
	/*Destructor*/
	~CLockedOnMovement(void);
	/*****************************************************************
	* Update():		Will hand out the behaviors based on what type of enemy and situation they are in.
	* Ins:			    fElapedTime    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Update(float _fElapedTime);
};

#endif