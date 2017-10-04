/***********************************************
* Filename:  		BlueShooting.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This is the class that will handle how the blue enemy shoots.
************************************************/
#ifndef BLUESHOOTING_H
#define	BLUESHOOTING_H

#include "ShootingBehavior.h"

class CBlueShooting : public CShootingBehavior
{
	float laserTimer;

public:
	/*****************************************************************
	* CBlueShooting():  Overload Constructor will set its type. Does mostly nothing. 
	* Ins:			    _pEnemy    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CBlueShooting(CEnemy* _pEnemy );
	/*Destructor*/
	~CBlueShooting(void);
	/*****************************************************************
	* Update():		Will randomly fire a missile whenever the enemy it is 
	*					attached to is ready to fire a missile.
	* Ins:			    fElapedTime    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Update(float _fElapedTime);
};

#endif