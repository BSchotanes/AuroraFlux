/***********************************************
* Filename:  		WanderingMovement.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will be for when an enemy needs to chase its target.				
************************************************/
#ifndef CWANDERINGMOVEMENT_H
#define	CWANDERINGMOVEMENT_H

#include "MovementBehavior.h"

class  CWanderingMovement  : public CMovementBehavior
{
	// all the points to wander to
	vector<D3DXVECTOR3>		m_d3dWanderingPoints;

	//the current point to fly towards
	D3DXVECTOR3				m_d3dCurrentWaypoint;

	//index of which wandering point
	unsigned int			m_uiWaypointIndex;

	//timer for when to stop forcing the owner from fleeing
	float					m_hitTimer;

public:
	/*****************************************************************
	* CWanderingMovement():  Overload Constructor will set its type. Does mostly nothing. 
	* Ins:			    _pEnemy    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CWanderingMovement(CEnemy* _pEnenmy );
	/*Destructor*/
	~CWanderingMovement(void);
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