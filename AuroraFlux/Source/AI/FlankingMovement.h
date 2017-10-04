/***********************************************
* Filename:  		FlankingMovement.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		Enemies will flank their target.
************************************************/
#ifndef CFLANKINGMOVEMENT_H
#define	CFLANKINGMOVEMENT_H

#include "MovementBehavior.h"

class  CFlankingMovement  : public CMovementBehavior
{
public:

	/*****************************************************************
	* CFlankingMovement():  Overload Constructor will set its type. Does mostly nothing. 
	* Ins:			    _pEnemy    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CFlankingMovement(CEnemy* _pEnenmy );
	/*Destructor*/
	~CFlankingMovement(void);
	/*****************************************************************
	* Update():		Flanking
	* Ins:			    fElapedTime    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Update(float _fElapedTime);
};

#endif