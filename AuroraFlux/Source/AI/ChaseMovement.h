/***********************************************
* Filename:  		ChaseMovement.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will be for when an enemy needs to chase its target.				
************************************************/
#ifndef CCHASEMOVEMENT_H
#define	CCHASEMOVEMENT_H

#include "MovementBehavior.h"

class  CChaseMovement  : public CMovementBehavior
{
public:
	/*****************************************************************
	* CChaseMovement():  Overload Constructor will set its type. Does mostly nothing. 
	* Ins:			    _pEnemy    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CChaseMovement(CEnemy* _pEnemy );
	/*Destructor*/
	~CChaseMovement(void);
	/*****************************************************************
	* Update():		Will turn to its target.
	* Ins:			    fElapedTime    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Update(float _fElapedTime);
};

#endif