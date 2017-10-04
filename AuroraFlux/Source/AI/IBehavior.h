/***********************************************
* Filename:  		IBehavior.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This is the abstract base class that the behaviors 
*					will inherit off of.
************************************************/
#ifndef IBEHAVIOR_H
#define	IBEHAVIOR_H

class IBehavior
{
public:
	/*****************************************************************
	* ~IBehavior():    Destructor
	* Ins:			    None    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	virtual ~IBehavior(void)   = 0 {};
	/*****************************************************************
	* ~Update():		Will update all of its childres behaviors
	* Ins:			    fElapedTime    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	virtual void Update(float _fElapedTime) = 0;
	/*****************************************************************
	* ~GetType():      Returns the type of behavior
	* Ins:			    None    		      
	* Outs:		    None
	* Returns:		    int
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	virtual int GetType()  = 0;
};

#endif