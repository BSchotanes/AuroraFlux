/***********************************************
* Filename:  		PlayerEnterWarp.h
* Date:      		1/19/13
* Mod. Date: 		1/19/13
* Mod. Initials:	AQ
* Author:    		Alexander Garcia de Quevedo
* Purpose:   		This class will serve as a state for the 
*					player to fly off into the distance.
************************************************/
#ifndef	CWARPPLAYER_H
#define CWARPPLAYER_H

#include "PlayerBehavior.h"
#include "../Particle System/EffectSystem.h"

class CPlayerEnterWarp : public CPlayerBehavior
{
	CPlayer* m_pPlayer;

	// how long until the player can roll
	float m_fRollTimer;
	bool m_bCanRoll;

	int m_nMaxHealth;
	int m_nMaxShield;

	bool m_bSoundPlay;


public:
	/*****************************************************************
	* CRedPlayer():		Default Constructor will set up his type and timers
	* Ins:			    None    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CPlayerEnterWarp(void);
	CPlayerEnterWarp(CPlayer* _pPlayer);
	
	/*****************************************************************
	* ~CPlayerEnterWarp():	Default Destructor
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	~CPlayerEnterWarp(void);

	/*****************************************************************
	* Update():		Will update its respective data, shields, and health.
	* Ins:			    fElapedTime    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Update(float _fElapsedTime);

	/*****************************************************************
	* Render():		Draw effects or whatnot.
	* Ins:			    None    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Render();

	/*****************************************************************
	* DoRoll():		If a missile is incoming, dodge it if the timer is ready.
	* Ins:			    None    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void DoRoll();

	/* ACCESSORS */
	int GetMaxShield() {return m_nMaxShield;}
	int GetMaxHealth() {return m_nMaxHealth;}
	void SetPlayer( CPlayer* _pPlayer );


};

#endif

