/***********************************************
* Filename:  		BossTurret.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will hold all of the data for the asteroids.				
************************************************/
#ifndef	CBOSSTURRET_H
#define CBOSSTURRET_H

#include "IEntity.h"
#include "Enemy.h"

enum TurretTypes{LEFT, TOP, RIGHT, BOTTOM, FRONT, BACK};

class CBossTurret : public CEnemy
{
	CEntity * m_pOwner;

	int		 m_nTurretType;

	int		 m_nWeaponType;

	float m_fZModifier;
	bool corretSide;
	float m_fCounter;

public:
	/*****************************************************************
	* CGenerator():  Default Constructor will set its type. Initilizes asteroid unique data.
	* Ins:			    None    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CBossTurret(CEntity * _pOwner, CEntity * _pTarget, int _type, int _weaponType, float _zModifier);
	/*Destructor*/
	~CBossTurret(void);
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

	bool TopTurret	 (float _fElapsedTime);
	bool BottomTurret(float _fElapsedTime);
	bool LeftTurret	 (float _fElapsedTime);
	bool RightTurret (float _fElapsedTime);
	bool FrontTurret (float _fElapsedTime);
	bool BackTurret  (float _fElapsedTime);
	CEntity * GetOwner(){return m_pOwner;}
	int GetTurretType() {return m_nTurretType;}
	bool GetCorrectSide(){return corretSide;}
};

#endif
