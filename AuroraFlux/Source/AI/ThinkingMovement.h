/***********************************************
 * Filename:  		ThinkingMovement.h
 * Date:      		11/27/2012
 * Mod. Date: 		11/29/2012
 * Mod. Initials:	AR
 * Author:    		Andrew A Rosser
 * Purpose:   		This is the class that will handle how the rest of the behaviors 
 *					are going to be passed out.
 ************************************************/
#ifndef CTHINKINGMOVEMENT_H
#define CTHINKINGMOVEMENT_H

#include "MovementBehavior.h"

class  CThinkingMovement  : public CMovementBehavior
{
public:
/*****************************************************************
 * CThinkingMovement():  Overload Constructor will set its type. Does mostly nothing. 
 * Ins:			    _pEnemy    		      
 * Outs:		    None
 * Returns:		    None
 * Mod. Date:		11/29/2012
 * Mod. Initials:	AR
 *****************************************************************/
	CThinkingMovement( CEnemy* _pEnenmy );
/*Destructor*/
	~CThinkingMovement( void );
/*****************************************************************
 * Update():		Will hand out the behaviors based on what type of enemy and situation they are in.
 * Ins:			    fElapedTime    		      
 * Outs:		    None
 * Returns:		    void
 * Mod. Date:		11/29/2012
 * Mod. Initials:	AR
 *****************************************************************/
	void Update( float _fElapedTime );
};

#endif CTHINKINKMOVEMENT_H