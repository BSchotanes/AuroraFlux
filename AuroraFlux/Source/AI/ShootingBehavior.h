/***********************************************
* Filename:  		ShootingBehavior.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This is the class that will help control each of
*					the shooting behaviors.
************************************************/
#ifndef CSHOOTINGBEHAIOR_H
#define CSHOOTINGBEHAIOR_H

#include "Behaviors.h"

class CBlueShooting;
class CRedShooting;
class CYellowShooting;

class CShootingBehavior : public CBehavior
{

public:
	/*****************************************************************
	* CShootingBehavior():     Overload Constructor will set its type. Does mostly nothing. 
	* Ins:			    _pEnemy    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CShootingBehavior(CEnemy* _pEnemy );
	/*Destructor*/
	~CShootingBehavior(void);
	/*****************************************************************
	* Update():		Will pass down the updates to its children. 
	* Ins:			    fElapedTime    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Update(float _fElapedTime);

};

#endif