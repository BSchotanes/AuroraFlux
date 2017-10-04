/***********************************************
* Filename:  		FleeMovement.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will be for when an enemy needs to run away from its target.				
************************************************/
#ifndef CFLEEMOVEMENT_H
#define	CFLEEMOVEMENT_H

#include "MovementBehavior.h"

class  CFleeMovement  : public CMovementBehavior
{
public:
	/*****************************************************************
	* CFleeMovement():  Overload Constructor will set its type. Does mostly nothing. 
	* Ins:			    _pEnemy    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CFleeMovement(CEnemy* _pEnenmy );
	/*Destructor*/
	~CFleeMovement(void);
	/*****************************************************************
	* Update():		Will run away from its target.
	* Ins:			    fElapedTime    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Update(float _fElapedTime);
};

#endif