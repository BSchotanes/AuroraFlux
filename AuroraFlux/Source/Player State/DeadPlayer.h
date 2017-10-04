/***********************************************
* Filename:  		RedPlayer.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will hold all the red player entities data.
************************************************/
#ifndef	CDEADPLAYER_H
#define CDEADPLAYER_H

#include "PlayerBehavior.h"
#include "../Particle System/EffectSystem.h"

class CDeadPlayer : public CPlayerBehavior
{
	CPlayer* m_pPlayer;

	// how long until the player can roll
	float m_fRollTimer;

	//on if the player has the ability to roll available
	bool m_bCanRoll;

	int m_nMaxHealth;
	int m_nMaxShield;

	bool m_bSoundPlay;
	bool m_bDoWhenStart;

	float m_fDeathTime;

	CEffectSystem m_DeathInitialExplosion;
	CEffectSystem m_DeathImplosion;
public:
	/*****************************************************************
	* CRedPlayer():		Default Constructor will set up his type and timers
	* Ins:			    None    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CDeadPlayer(void);
	CDeadPlayer(CPlayer* _pPlayer);

	/*****************************************************************
	* ~CDeadPlayer():Default Destructor
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	~CDeadPlayer(void);

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
	void ResetTimer() {m_fDeathTime = 5.0f;}
	void SetPlayer( CPlayer* _pPlayer );
	void CreateEffect();
};
#endif