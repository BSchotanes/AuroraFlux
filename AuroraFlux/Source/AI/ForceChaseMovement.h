/***********************************************
* Filename:  		FlankingMovement.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		Enemies will flank their target.
************************************************/
#ifndef CFORCECHASEMOVEMENT_H
#define	CFORCECHASEMOVEMENT_H

#include "MovementBehavior.h"
#include "../Particle System/EffectSystem.h"

class  CForceChaseMovement  : public CMovementBehavior
{
	CEffectSystem m_DeathAnimation;
	float m_fChaseLifeTime;
public:

	/*****************************************************************
	* CFlankingMovement():  Overload Constructor will set its type. Does mostly nothing. 
	* Ins:			    _pEnemy    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CForceChaseMovement(CEnemy* _pEnemy );
	/*Destructor*/
	~CForceChaseMovement(void);
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