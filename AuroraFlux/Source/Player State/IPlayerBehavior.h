/***********************************************
* Filename:  		IEntity.h
* Date:      		12/08/2012
* Mod. Date: 		11/08/2012
* Mod. Initials:	AG
* Author:    		Andrew Goossens
* Purpose:   		This class will be the abstract base class all player behaviors will inherit from.
************************************************/
#ifndef IPLAYERBEHAVIOR_H
#define IPLAYERBEHAVIOR_H

#include "../Definitions.h"

class IPlayerBehavior
{
public:
	/*****************************************************************
	* ~CPlayerBehavior():Default Destructor
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	virtual ~IPlayerBehavior() = 0 {};

	/*****************************************************************
	* Update():		Will pass down the update calls to its children.
	* Ins:			    fElapedTime    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		12/08/2012
	* Mod. Initials:	AG
	*****************************************************************/
	virtual void Update(float _fElapsedTime) = 0 {};

	/*****************************************************************
	* Render():		Will pass down the draw calls to its children.
	* Ins:			    fElapedTime    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		12/08/2012
	* Mod. Initials:	AG
	*****************************************************************/
	virtual void Render() = 0 {};

	/*****************************************************************
	* GetMaxShield():	Will pass down the max shield to its children
	* Ins:			    fElapedTime    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		12/08/2012
	* Mod. Initials:	AG
	*****************************************************************/
	virtual int GetMaxShield() = 0 {};

	/*****************************************************************
	* GetMaxHealth():	Will pass down the max health to its children
	* Ins:			    fElapedTime    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		12/08/2012
	* Mod. Initials:	AG
	*****************************************************************/
	virtual int GetMaxHealth() = 0 {};
};

#endif