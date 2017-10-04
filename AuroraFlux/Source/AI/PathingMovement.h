/***********************************************
* Filename:  		PathingMovement.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will follow waypoint targets supplied by the yellow enemy.
************************************************/
#ifndef CPATHINGMOVEMENT_H
#define CPATHINGMOVEMENT_H

#include "MovementBehavior.h"

class  CPathingMovement  : public CMovementBehavior
{	
	vector<D3DXVECTOR3> m_vWaypoints;
	unsigned int waypointIndex;

public:
	/*****************************************************************
	* CPathingMovement():  Overload Constructor will set its type. Does mostly nothing. 
	* Ins:			    _pEnemy    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CPathingMovement(CEnemy* _pEnenmy );
	/*Destructor*/
	~CPathingMovement(void);
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