/***********************************************
* Filename:  		BluePlayer.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will hold all the blue player entities data.
************************************************/
#ifndef	CBLUEPLAYER_H
#define CBLUEPLAYER_H

#include "PlayerBehavior.h"

class CBluePlayer : public CPlayerBehavior
{
	CPlayer* m_pPlayer;
	// How long until a missile can be fired
	float m_fMissileTimer;

	// on if missile is ready to fire
	bool m_bCanFire;

	int m_nMaxHealth;
	int m_nMaxShield;
public:
	/*****************************************************************
	* CBluePlayer():	Default Constructor will set up his type and timers
	* Ins:			    None    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CBluePlayer(void);

	/*****************************************************************
	* CBluePlayer():	Overload Constructor will set its type and all of his
	*					internal data as well as set its target to the passed in one.
	* Ins:			    _missileTimer, _target  		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CBluePlayer(float _fMissileTimer, int _nTarget);
	CBluePlayer(CPlayer* _pPlayer);

	/*****************************************************************
	* ~CBluePlayer():	Default Destructor
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	~CBluePlayer(void);

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

	/*****************************************************************
	* FireMissile():		Creates a missile with its target
	* Ins:			    None    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	bool FireMissile(CEntity* _pTarget);

	/* ACCESSORS */
	int GetMaxShield() {return m_nMaxShield;}
	int GetMaxHealth() {return m_nMaxHealth;}
};
#endif