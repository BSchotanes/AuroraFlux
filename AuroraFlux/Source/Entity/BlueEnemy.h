/***********************************************
* Filename:  		BlueEnemy.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will hold all the Blue enemy entities unique data.
************************************************/
#ifndef	CBLUEENEMY_H
#define CBLUEENEMY_H

#include "Enemy.h"
#include "../AI/AIHelper.h"

class CObjectManager;
class CBlueEnemy : public CEnemy
{
	// How long until the next missile can be fired
	float m_fMissileTimer;

	// on if the enemy can fire a missile
	bool m_bCanFire;

	// on while this enemy is locked onto the player
	bool m_bLockingOn;

	// Shield BV
	CCollOBJ* m_pExtraBV;

	// Show Shield or not
	bool m_bShieldVisible;

	// Shield Mesh IDs
	int m_nVisibleShieldMesh;
	int m_nInvisibleShieldMesh;
	CAIHelper *s_AIHelper;

	CEffectSystem m_esEngineTrail;
	CEffectSystem m_esDamage;
	CEffectSystem m_esArrival;

	

public:
	/*****************************************************************
	* CBlueEnemy():	Default Constructor will set its type and all of his internal data.
	* Ins:			    None    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CBlueEnemy(void);
	/*****************************************************************
	* CBlueEnemy():	Overload Constructor will set its type and all of his
	*					internal data as well as set its target to the passed in one.
	* Ins:			    _target    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CBlueEnemy(CEntity* _pTarget,CAIHelper *_AIHelper,CWaypoint * _pWaypoin, CObjectManager* _pObjManager);
	/*Destructor*/
	~CBlueEnemy(void);
	/*****************************************************************
	* Update():		Will update its respective shooting and movement behaviors.
	* Ins:			    fElapedTime    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Update(float _fElapedTime);
	/*****************************************************************
	* Render():		Draw effects or whatnot.
	* Ins:			    fElapedTime    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Render();

	void CreateArrivalEffect();

	/*Accessors*///////////////////////////////////////////////////////////////////
	float GetMissileTimer() {return m_fMissileTimer; }
	bool GetCanFire() {return m_bCanFire; }
	bool GetLockingOn() {return m_bLockingOn; }
	
	int GetVisibleShieldMesh() const { return m_nVisibleShieldMesh; }
	int GetInvisibleShieldMesh() const { return m_nInvisibleShieldMesh; }
	CAIHelper * GetHelper(){return s_AIHelper;}
	void UpdateHelperAI(float _elapsedTime);
	CEffectSystem* GetTrailEffect() {return &m_esEngineTrail; }
	CEffectSystem* GetDamagedEffect() { return &m_esDamage; }
	////////////////////////////////////////////////////////////////////////////////

	/*Mutators*////////////////////////////////////////////////////////////////////
	void SetCanFire(bool _bFire) { m_bCanFire = _bFire; }
	void SetLockingOn(bool _bLockOn) { m_bLockingOn = _bLockOn; }
	void SetMissileTimer(float _fTimer) { m_fMissileTimer = _fTimer; }
	void SetShieldBV(CCollOBJ* _pBV) { m_pExtraBV = _pBV; }
	void SetVisibleShieldMesh(int _nID) { m_nVisibleShieldMesh = _nID; }
	void SetInvisibleShieldMesh(int _nID) { m_nInvisibleShieldMesh = _nID; }
	////////////////////////////////////////////////////////////////////////////////

};

#endif