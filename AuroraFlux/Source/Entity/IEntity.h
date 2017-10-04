/***********************************************
 * Filename:  		IEntity.h
 * Date:      		11/27/2012
 * Mod. Date: 		11/29/2012
 * Mod. Initials:	AR
 * Author:    		Andrew A Rosser
 * Purpose:   		This class will be the abstract base class all entities will inherit from.
 ************************************************/
#ifndef	IENTITY_H
#define IENTITY_H

#include "../Definitions.h"
//Base class for all objects
class IEntity
{
public:
/*Destructor*/
	virtual ~IEntity(void)   = 0 {};
/*****************************************************************
 * Update():		Will pass down the update calls to its children.
 * Ins:			    fElapedTime    		      
 * Outs:		    None
 * Returns:		    void
 * Mod. Date:		11/29/2012
 * Mod. Initials:	AR
 *****************************************************************/
	virtual void Update(float fElapedTime) = 0;
/*****************************************************************
 * Render():		Will pass down the draw calls to its children.
 * Ins:			    fElapedTime    		      
 * Outs:		    None
 * Returns:		    void
 * Mod. Date:		11/29/2012
 * Mod. Initials:	AR
 *****************************************************************/
	virtual void Render()  = 0;

	/*Accessors*/
	virtual int GetType()  = 0;
	virtual D3DXMATRIX* GetMatrix() = 0;
};

#endif