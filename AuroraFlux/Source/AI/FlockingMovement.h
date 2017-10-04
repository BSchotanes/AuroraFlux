/***********************************************
* Filename:  		FlockingMovement.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class is for the red enemy normally. Will keep enemies flocked together.
************************************************/
#ifndef CFLOCKINGMOVEMENT_H
#define	CFLOCKINGMOVEMENT_H

#include "MovementBehavior.h"
class CEntity;
class  CFlockingMovement  : public CMovementBehavior
{
protected:

	/*D3DXVECTOR3 AverageForward;
	D3DXVECTOR3 AveragePosition;
	float AlignmentStrength;
	float FlockRadius;
	float CohesionStrength;*/

public:
	/*****************************************************************
	* CFlockingMovement():  Overload Constructor will set its type. Does mostly nothing. 
	* Ins:			    _pEnemy    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CFlockingMovement(CEnemy* _pEnenmy );
	/*Destructor*/
	~CFlockingMovement(void);
	/*****************************************************************
	* Update():		Average positions of flockers.
	* Ins:			    fElapedTime    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Update(float _fElapedTime);

	/*
	void CalculateAverages();
	D3DXVECTOR3 CalculateAlignmentAcceleration(CEntity * _enemy);
	D3DXVECTOR3 CalculateCohesionAcceleration(CEntity * _enemy);
	D3DXVECTOR3 CalculateSeparationAcceleration(CEntity * _enemy);
	*/


};

#endif
