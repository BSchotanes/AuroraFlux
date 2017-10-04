/***********************************************
* Filename:  		FlankingMovement.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		Enemies will flank their target.
************************************************/
#ifndef ROLLINGMOVEMENT_H
#define	ROLLINGMOVEMENT_H

#include "MovementBehavior.h"

class  CRollingMovement  : public CMovementBehavior
{
public:
	float m_fTimeToRoll;

	// all the points to wander to
	vector<D3DXVECTOR3>		m_d3dWanderingPoints;

	//the current point to fly towards
	D3DXVECTOR3				m_d3dCurrentWaypoint;

	//index of which wandering point
	unsigned int			m_uiWaypointIndex;
	/**************************
	
	***************************************
	* CFlankingMovement():  Overload Constructor will set its type. Does mostly nothing. 
	* Ins:			    _pEnemy    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CRollingMovement(CEnemy* _pEnenmy );
	/*Destructor*/
	~CRollingMovement(void);
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