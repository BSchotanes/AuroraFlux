/***********************************************
* Filename:  		BossHanger.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will hold all of the data for the asteroids.				
************************************************/
#ifndef	CBOSSHANGER_H
#define CBOSSHANGER_H

#include "IEntity.h"
#include "Enemy.h"

enum hangerTypes{PHASE_ONE_HANGER, PHASE_TWO_HANGER, PHASE_THREE_HANGER};
class CObjectManager;

class CBossHanger : public CEnemy
{
	CEntity * m_pOwner;

	int		 m_nHangerType;

	float	 n_fTimeToSpawnNext;

	CObjectManager * m_pObjectManager;

public:
	/*****************************************************************
	* CBossHanger():  Default Constructor will set its type. Initilizes asteroid unique data.
	* Ins:			    None    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CBossHanger(CEntity * _pOwner, int _type,CObjectManager * _objectManager);
	/*Destructor*/
	~CBossHanger(void);
	/*****************************************************************
	* Update():		Will update its internal timers and behaviors.
	* Ins:			    fElapedTime    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Update(float _fElapedTime);
	/*****************************************************************
	* Render():		Draws the updated asteroid and details with asteroids.
	* Ins:			    fElapedTime    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Render();


	bool RedHanger();
	bool BlueHanger();
	bool YellowHanger();
	CEntity * GetOwner(){return m_pOwner;}
};

#endif
