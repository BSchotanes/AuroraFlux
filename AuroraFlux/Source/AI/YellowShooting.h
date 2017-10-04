/***********************************************
* Filename:  		YellowShooting.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This is the class that will handle how the yellow enemy shoots.
************************************************/
#ifndef YELLOWSHOOTING_H
#define YELLOWSHOOTING_H

#include "ShootingBehavior.h"

class CYellowShooting : public CShootingBehavior
{

protected:
	bool doOnce;
	float laserTimer;
public:
	/*****************************************************************
	* CYellowShooting():  Overload Constructor will set its type. Does mostly nothing. 
	* Ins:			    _pEnemy    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CYellowShooting(CEnemy* _pEnemy );
	/*Destructor*/
	~CYellowShooting(void);
	/*****************************************************************
	* Update():		Will blow up for massive damage if the player is in range.
	*					Will not shoot otherwise.
	* Ins:			    fElapedTime    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Update(float _fElapedTime);
	bool GetDoOnce(){return doOnce;}
	void SetDoOnce(bool _t){doOnce = _t;}
};

#endif