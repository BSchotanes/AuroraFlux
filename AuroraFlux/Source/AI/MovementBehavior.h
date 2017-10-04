/***********************************************
* Filename:  		MovementBehavior.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This is the class that will help control each of
*					the movement behaviors.This will choose which
*					respective movement behavior for the entity to choose
************************************************/
#ifndef CMOVEMENTBEHAVIOR_H
#define	CMOVEMENTBEHAVIOR_H

#include "Behaviors.h"

class CMovementBehavior : public CBehavior
{

protected:
	//on if a partner is hit and the current owner is forced to chase the player
	bool m_bForceChase;

public:
	/*****************************************************************
	* CMovementBehavior():     Overload Constructor will set its type. Does mostly nothing. 
	* Ins:			    _pEnemy    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CMovementBehavior(CEnemy* _pEnenmy);
	/*Destructor*/
	~CMovementBehavior(void);
	/*****************************************************************
	* Update():		Will pass down the updates to its children. 
	* Ins:			    fElapedTime    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Update(float _fElapedTime);

	/*****************************************************************
	* SetForceChasebool():		Will set the chase bool to true or false that is passed in. 
	* Ins:			    _chase    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void SetForceChasebool(bool _bChase){m_bForceChase = _bChase;}
	/*****************************************************************
	* GetForceChase():		Will return the chase bool. 
	* Ins:			    fElapedTime    		      
	* Outs:		    None
	* Returns:		    bool
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	bool GetForceChase() {return m_bForceChase;}
};

#endif