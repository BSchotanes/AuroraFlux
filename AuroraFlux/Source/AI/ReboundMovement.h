/***********************************************
* Filename:  		FlankingMovement.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		Enemies will flank their target.
************************************************/
#ifndef CREBOUNDMOVEMENT_H
#define	CREBOUNDMOVEMENT_H

#include "MovementBehavior.h"

class  CReboundMovement  : public CMovementBehavior
{
public:
	float m_fKillTimer;
	D3DXVECTOR3 upWaypoint;
	/*****************************************************************
	* CFlankingMovement():  Overload Constructor will set its type. Does mostly nothing. 
	* Ins:			    _pEnemy    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CReboundMovement(CEnemy* _pEnenmy );
	/*Destructor*/
	~CReboundMovement(void);
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