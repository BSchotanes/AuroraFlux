/***********************************************
* Filename:  		Behavior.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This is the base class that the Movement and
*					shooting behaviors will inherit off of.
************************************************/
#include "../StdAfx.h"
#include "Behaviors.h"


/*****************************************************************
* CBehavior():     Overload Constructor will set its type. The enemy passed in 
*					will be the enemy this behavior is attached to.
* Ins:			    _pEnemy    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CBehavior::CBehavior(CEnemy* _pEnenmy) : m_pEnemy(_pEnenmy)
{
	/////// CHANGED
}
// Destructor
CBehavior::~CBehavior(void)
{
	if (m_pEnemy != nullptr)
	{
		//delete m_pEnemy;
		m_pEnemy = nullptr;
	}
}
/*****************************************************************
* Update():		Will pass down the updates to its children. 
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CBehavior::Update(float _fElapedTime)
{


}