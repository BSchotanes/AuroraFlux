/***********************************************
* Filename:  		BossTurret.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will hold all of the data for the asteroids.				
************************************************/
#ifndef	CBOSSWINTURRET_H
#define CBOSSWINTURRET_H

#include "Enemy.h"
#include "../Entity/BossTurret.h"
#include "../Particle System/EffectInformation.h"

class CBossBigLazer;
class CBoss;

class CBossWinTurret : public CEnemy 
{
	CEntity * m_pOwner;
	bool shouldRecharce;
	int		 m_nTurretType;

	int		 m_nWeaponType;

	float m_fZModifier;

	float m_fRechargeTimer;

	//CBossBigLazer* m_pBigLazer;
	bool m_bStartRecharging;
	bool m_bHasPlayedLazerChargingSound;

	CEffectSystem m_esChargeParticles;

	CEffectSystem m_esLaserParticles;

	bool m_bFireAtPlanet;

	float m_fCDTime;

	bool m_bFireMegaLaser;
	bool corretSide;
	
	float fLargeLazerChargeTimer;

	bool m_bFireLaser;

	CBoss* m_pBoss;
public:
	/*****************************************************************
	* CGenerator():  Default Constructor will set its type. Initilizes asteroid unique data.
	* Ins:			    None    		      
	* Outs:		    None 
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CBossWinTurret(CEntity * _pOwner, CEntity * _pTarget, int _type, int _weaponType, float _zModifier, CObjectManager* _objManager);
	/*Destructor*/
	~CBossWinTurret(void);
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
	bool GetCorrectSide()	 {return corretSide;}
	void SetRechargeTimer(float _time) { m_fRechargeTimer = _time;}
	bool TopTurret	 (float _fElapsedTime);
	bool BottomTurret(float _fElapsedTime);
	bool LeftTurret	 (float _fElapsedTime);
	bool RightTurret (float _fElapsedTime);
	bool FrontTurret (float _fElapsedTime);
	bool BackTurret  (float _fElapsedTime);
	CEntity * GetOwner(){return m_pOwner;}
	void SetZModifier(float _z){ m_fZModifier = _z;}
	float GetZModifier(void){ return m_fZModifier;}
	bool GetRecharging(void){return m_bStartRecharging;}
	bool GetShouldFireAtPlanet() {return m_bFireAtPlanet;}
	void SetShouldFireAtPlanet(bool _bShouldDestroyPlanet) {m_bFireAtPlanet = _bShouldDestroyPlanet;}
	float GetChargingTimer() { return fLargeLazerChargeTimer; }
	bool GetFireLaser() {return m_bFireMegaLaser;}
	bool GetFireLaserTowardPlanet() {return m_bFireLaser;}
	void SetFireLaserTowardPlanet(bool _bFireLaser) {m_bFireLaser = _bFireLaser;}
};

#endif
