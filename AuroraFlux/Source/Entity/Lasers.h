/***********************************************
* Filename:  		Laser.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will hold all the laser entities data.
************************************************/
#ifndef	CLASER_H
#define CLASER_H

#include "Entity.h"
#include "Enemy.h"
#include "../AI/AIHelper.h"

class CLaser : public CEnemy
{
	// how much longer the laser has to live
	float m_fLifetime;

	// where this laser came from
	CEntity * m_pOwner;
	CAIHelper *s_AIHelper;

	CEffectSystem m_Effect;
	CEffectSystem m_MuzzleEffect;
	CEffectSystem m_esAsteroidDust;
	CEffectSystem m_ESLaserSpark;

	bool m_bShouldBounce;
public:
	void SetShouldBounce(bool _bounce){m_bShouldBounce = _bounce; }
	bool GetShouldBounce( ){ return m_bShouldBounce; }
	/*****************************************************************
	* CLaser():		Default Constructor will set up his data members
	* Ins:			    None    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CLaser(void);
	/*****************************************************************
	* CLaser():		Default Constructor will set up his data members
	* Ins:			    None    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CLaser(CEntity* _pOwner,CAIHelper *_AIHelper);
	/*Destructor*/
	~CLaser(void);
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
	* Render():		Draw Updates
	* Ins:			    None    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Render() ;

	//Accessors
	float GetLifetime() {return m_fLifetime; }
	CEntity* GetOwner() {return m_pOwner;}
	CAIHelper * GetHelper(){return s_AIHelper;}

	//Mutators
	void SetLifetime(float _fLifetime) { m_fLifetime = _fLifetime; }
	void SetOwner(CEntity* _pOwner) { m_pOwner = _pOwner;}
	void CreateEffect(Effects eType);
	void MuzzleEffect();
	void AsteroidDust();
	void SparkEffect();
};

#endif
