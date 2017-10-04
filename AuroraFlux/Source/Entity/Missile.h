/***********************************************
 * Filename:  		Missile.h
 * Date:      		11/27/2012
 * Mod. Date: 		11/29/2012
 * Mod. Initials:	AR
 * Author:    		Andrew A Rosser
 * Purpose:   		This class will hold all the missile entities data.
 ************************************************/
#ifndef CMISSILE_H
#define	CMISSILE_H

#include "Entity.h"
#include "Enemy.h"
#include "../AI/AIHelper.h"
#include "../Entity/EnvironmentObjects.h"

class CMissile : public CEnemy
{
	// How much longer the missile has to live
	float m_fLifetime;
	int m_nTurn;

	// where this missile came from
	CEntity * m_pOwner;

	//only update the missile speed boost once
	 bool m_bDoOnce;

	 float m_fMissileDistance;
	CAIHelper *s_AIHelper;

	CEffectSystem m_Effect;
	CEffectSystem m_Effect2;
	CEffectSystem m_esAsteroidDust;
	CEffectSystem m_ESMissileTrail;
	CEffectSystem m_ESMissileThruster;

	CEnvironmentObject* m_pSpaceDibris;

	int m_nLockOnWarningRedTex;
	int m_nLockOnWarningYellowTex;
	int m_nLockOnNoWarningGreenTex;
	bool m_bShouldMakeExplosionSound;

	bool isThrusted;
	bool m_bNearExplosion;
public:
	/*****************************************************************
 * CMissile():		Default Constructor will set up his data members
 * Ins:			    None    		      
 * Outs:		    None
 * Returns:		    None
 * Mod. Date:		11/29/2012
 * Mod. Initials:	AR
 *****************************************************************/
	CMissile(void);
/*****************************************************************
 * CMissile():		overload Constructor will take in what the missile will chase as well as where it came from.
 * Ins:			    _target, _owner  		      
 * Outs:		    None
 * Returns:		    None
 * Mod. Date:		11/29/2012
 * Mod. Initials:	AR
 *****************************************************************/
	CMissile(CEntity* _pTarget, CEntity* _pOwner,CAIHelper *_AIHelper);
/*Destructor*/
	~CMissile(void);
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
 * Render():		Draw stuff
 * Ins:			    fElapedTime    		      
 * Outs:		    None
 * Returns:		    void
 * Mod. Date:		11/29/2012
 * Mod. Initials:	AR
 *****************************************************************/
	void Render(); 

	//Accessors
	float GetLifetime() {return m_fLifetime; }
	bool GetDoOnce() {return m_bDoOnce; }
	CEntity * GetOwner() {return m_pOwner; }
	CAIHelper * GetHelper(){return s_AIHelper;}

	//Mutators
	void SetOwner(CEntity * _pOwner, CEnvironmentObject* _pSpaceDibris = NULL);
	void SetDoOnce(bool _bDo) { m_bDoOnce = _bDo; }
	void SetLifetime(float _fLifetime);

	float GetMissileDistance() {return m_fMissileDistance;}
	void CreateEffect(Effects eType);
	void AsteroidDust();
	void DisableEffects();
	void MissileFlash();
	void SetSpaceDibris(CEnvironmentObject* _dibris) {m_pSpaceDibris = _dibris;}
	bool GetIsThrusted() {return isThrusted;}
	void SetIsThrusted(bool _th) {isThrusted = _th;}

	bool GetNearExplosion() {return m_bNearExplosion;}
	void SetNearExplosion(bool _nNearExplosion) {m_bNearExplosion = _nNearExplosion;}
};

#endif