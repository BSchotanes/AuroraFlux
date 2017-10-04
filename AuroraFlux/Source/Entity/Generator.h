/***********************************************
* Filename:  		Generator.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will hold all of the data for the asteroids.				
************************************************/
#ifndef	CGENERATOR_H
#define CGENERATOR_H

#include "IEntity.h"
#include "Enemy.h"
#include "../Particle System/EffectSystem.h"

class CGenerator : public CEnemy
{
	CEntity * m_pOwner;
	bool doOnce;
	int index;
	int yMod;
	//CEffectSystem m_esShieldConnectionParticles;
public:
	/*****************************************************************
	* CGenerator():  Default Constructor will set its type. Initilizes asteroid unique data.
	* Ins:			    None    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CGenerator(CEntity * _pOwner, int _index);
	/*Destructor*/
	~CGenerator(void);
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

//	void Activate() {m_esShieldConnectionParticles.SetPlay(true);}

	void SetActive(bool _isActive);

	//void StartParticles() {m_esShieldConnectionParticles.SetPlay(true);}
};

#endif
