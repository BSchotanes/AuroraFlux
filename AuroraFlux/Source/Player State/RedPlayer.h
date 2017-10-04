/***********************************************
* Filename:  		RedPlayer.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will hold all the red player entities data.
************************************************/
#ifndef	CREDPLAYER_H
#define CREDPLAYER_H

#include "PlayerBehavior.h"

class CRedPlayer : public CPlayerBehavior
{
	CPlayer* m_pPlayer;

	// how long until the player can roll
	float m_fRollTimer;

	//on if the player has the ability to roll available
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
	CRedPlayer(void);
	CRedPlayer(CPlayer* _pPlayer);

	/*****************************************************************
	* ~CRedPlayer():	Default Destructor
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	~CRedPlayer(void);

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
};

#endif

