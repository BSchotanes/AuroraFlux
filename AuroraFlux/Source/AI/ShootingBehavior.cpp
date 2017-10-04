/***********************************************
* Filename:  		ShootingBehavior.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This is the class that will help control each of
*					the shooting behaviors.
************************************************/
#include "../StdAfx.h"
#include "../Entity/Enemy.h"
#include "ShootingBehavior.h"
#include "BlueShooting.h"
#include "RedShooting.h"
#include "YellowShooting.h"
/*****************************************************************
* CShootingBehavior():     Overload Constructor will set its type. Does mostly nothing. 
* Ins:			    _pEnemy    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CShootingBehavior::CShootingBehavior(CEnemy* _pEnemy) : CBehavior(_pEnemy)
{

}
/*Destructor*/
CShootingBehavior::~CShootingBehavior(void)
{

}
/*****************************************************************
* Update():		Will pass down the updates to its children. 
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CShootingBehavior::Update(float _fElapedTime)
{

}