/***********************************************
* Filename:  		Behavior.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This is the base class that the Movement and
*					shooting behaviors will inherit off of.
************************************************/
#ifndef	CBEHAVIOR_H
#define CBEHAVIOR_H

/////// CHANGED

#include "IBehavior.h"
#include "../Particle System/EffectInformation.h"
class CEnemy;

class CBehavior : public IBehavior
{
protected:
	// What type of behavior this is
	int m_nBehaviorType;

	// The enemy this behavior will belong to
	CEnemy* m_pEnemy;

public:

	/*****************************************************************
	* CBehavior():     Overload Constructor will set its type. The enemy passed in 
	*					will be the enemy this behavior is attached to.
	* Ins:			    _pEnemy    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CBehavior(CEnemy* _pEnemy );
	// Destructor
	~CBehavior(void);
	/*****************************************************************
	* Update():		Will pass down the updates to its children. 
	* Ins:			    fElapedTime    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	virtual void Update(float _fElapedTime);


	/***************
	*  Accessors
	***************/
	int GetType()  {return  m_nBehaviorType  ;}
	CEnemy* GetEnemy(){ return m_pEnemy;}
	/***************
	* Mutators
	***************/
	void SetType(int _nType)  {  m_nBehaviorType = _nType;	}
};

#endif