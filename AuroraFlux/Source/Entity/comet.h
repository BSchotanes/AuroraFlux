/***********************************************
* Filename:  		Asteroid.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will hold all of the data for the asteroids.				
************************************************/
#ifndef	CCOMET_H
#define CCOMET_H

#include "IEntity.h"
#include "Enemy.h"
#include "../Collision/Sphere.h"
class CComet : public CEnemy
{
	int rotationDirectionX;
	int rotationDirectionY;
	int rotationDirectionZ;

	float rotationamount;

	float randomScale;

	int randomNegative;

	float m_fRadius;

	int m_nWaveIndex;

	CEffectSystem m_esDeath;

	float m_fLifeTime;


	CEffectSystem m_esEngineTrail;

public:
	/*****************************************************************
	* CAsteroid():  Default Constructor will set its type. Initilizes asteroid unique data.
	* Ins:			    None    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CComet(float _fRadius);
	/*Destructor*/
	~CComet(void);
	/*****************************************************************
	* Update():		Will update its internal timers and behaviors.
	* Ins:			    fElapedTime    		      
	* Outs:				None
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
	float GetRadius(){return m_fRadius ;}
	void SetRadius(float _rad);

	int GetWaveIndex(){return m_nWaveIndex ;}
	void SetWaveIndex(int _nWaveIndex){m_nWaveIndex = _nWaveIndex; }

	void DeathVFX();
	CEffectSystem* GetTrailEffect() {return &m_esEngineTrail; }
};
#endif
