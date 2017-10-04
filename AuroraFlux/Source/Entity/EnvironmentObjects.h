/***********************************************
* Filename:  		Generator.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will hold all of the data for the asteroids.				
************************************************/
#ifndef	CENVIRONMENTOBJECT_H
#define CENVIRONMENTOBJECT_H

#include "IEntity.h"
#include "Enemy.h"
#include "../Particle System/EffectInformation.h"

#define NUM_SEMI_EXPLOSIONS 4

class CEnvironmentObject : public CEnemy
{
	bool doOnce;
	int index;
	bool isOrbit;
	bool m_bShrink;
	float m_fScale;
	CEntity * m_pOwner;
	int asteroidType;
	D3DXVECTOR3 m_d3dOffset;
	bool ShouldUpdate;
	D3DXVECTOR3 m_d3dvVelocity;
	D3DXVECTOR3 m_d3dvRotation;
	float m_fDegree;
	bool m_bDuringShield;
	CEffectSystem m_esSemiExplosions[NUM_SEMI_EXPLOSIONS];
	CEffectSystem m_esDeathParticle;
	bool m_bIsBossHelper;
	CEffectSystem m_Effect;

public:
	void CreateEffect(Effects eType);

	bool GetBossHelper(){return m_bIsBossHelper;}
	/*****************************************************************
	* CGenerator():  Default Constructor will set its type. Initilizes asteroid unique data.
	* Ins:			    None    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CEnvironmentObject(int _index, D3DXVECTOR3 _position, D3DXVECTOR3 _velocity = D3DXVECTOR3(0,0,0), bool _boss = false, float _collisionRadius = 0.0f);
	/*Destructor*/
	~CEnvironmentObject(void);
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
	bool GetDuringShield() {return m_bDuringShield; }
	void  SetDuringShield(bool _shield) {m_bDuringShield = _shield; }
	CEntity * GetOwner(void){return m_pOwner;}
	void SetOwner(CEntity * _Owner){m_pOwner = _Owner;}
	bool GetIsOrbit(void){return isOrbit;}
	void SetIsOrbit(bool _orbit){isOrbit = _orbit;}
	int GetAsteroidType(){return asteroidType;}
	D3DXVECTOR3 GetOffset() const { return m_d3dOffset; }
	void SetOffset(D3DXVECTOR3 val) { m_d3dOffset = val; }
	void SetShouldUpdate(bool _updating) {ShouldUpdate = _updating;}
	void SetRotation(D3DXVECTOR3 _rotate, float _degree) {m_d3dvRotation = _rotate; m_fDegree = _degree;}
	void Destroy();
	void PlaySemiExplosion(int num);
};

#endif
