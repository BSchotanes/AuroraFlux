/***********************************************
* Filename:  		RedShooting.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This is the class that will handle how the red enemy shoots.
************************************************/
#ifndef REDSHOOTING_H
#define	REDSHOOTING_H

#include "ShootingBehavior.h"

class CRedShooting : public CShootingBehavior
{
	float s_fCounter;
public:
	/*****************************************************************
	* CRedShooting():  Overload Constructor will set its type. Does mostly nothing. 
	* Ins:			    _pEnemy    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CRedShooting(CEnemy* _pEnemy );
	/*Destructor*/
	~CRedShooting(void);
	/*****************************************************************
	* Update():		Will fire lasers whenever the enemy it is 
	*					attached to is facing the target.
	* Ins:			    fElapedTime    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Update(float _fElapedTime);
};

#endif