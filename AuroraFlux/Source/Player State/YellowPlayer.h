/***********************************************
* Filename:  		YellowPlayer.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will hold all the yellow player entities data.
************************************************/
#ifndef	CYELLOWPLAYER_H
#define CYELLOWPLAYER_H

#include "PlayerBehavior.h"

class CYellowPlayer : public CPlayerBehavior
{
	// How long the player is allowed to stay in stealth mode
	float m_fStealthTimer;

	// on if the player needs to be shifted out of stealth mode
	bool m_bTimerDone;

	CPlayer* m_pPlayer;

	// Integers for max health and shield
	int m_nMaxHealth;
	int m_nMaxShield;
public:
	/*****************************************************************
	* CYellowPlayer():	Default Constructor will set up his type and timers
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CYellowPlayer(void);
	CYellowPlayer(CPlayer* _pPlayer);

	/*****************************************************************
	* ~CYellowPlayer():	Default Destructor will set up his type and timers
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	~CYellowPlayer(void);

	/*****************************************************************
	* Update():		Will update its respective data, shields, and health.
	* Ins:			    fElapedTime    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Update(float _fElapedTime);
	/*****************************************************************
	* Render():		Draw effects or whatnot.
	* Ins:			    None    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Render();

	/* ACCESSORS */
	int GetMaxShield() {return m_nMaxShield;}
	int GetMaxHealth() {return m_nMaxHealth;}
};

#endif