/***********************************************
* Filename:  		FlankingMovement.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		Enemies will flank their target.
************************************************/
#ifndef CDEATHMOVEMENT_H
#define	CDEATHMOVEMENT_H

#include "MovementBehavior.h"
#include "../Particle System/EffectSystem.h"

class  CDeathMovement  : public CMovementBehavior
{
	CEffectSystem m_DeathAnimation;

public:

	/*****************************************************************
	* CFlankingMovement():  Overload Constructor will set its type. Does mostly nothing. 
	* Ins:			    _pEnemy    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CDeathMovement(CEnemy* _pEnemy );
	/*Destructor*/
	~CDeathMovement(void);
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