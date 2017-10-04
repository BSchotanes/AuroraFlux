/***********************************************
* Filename:  		FodderMovement.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		Enemies will attack their target.
************************************************/
#ifndef CFODDERMOVEMENT_H
#define	CFODDERMOVEMENT_H

#include "MovementBehavior.h"

class  CFodderMovement  : public CMovementBehavior
{
	float m_fWanderTiemr;
public:

	/*****************************************************************
	* CFlankingMovement():  Overload Constructor will set its type. Does mostly nothing. 
	* Ins:			    _pEnemy    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CFodderMovement(CEnemy* _pEnenmy );
	/*Destructor*/
	~CFodderMovement(void);
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