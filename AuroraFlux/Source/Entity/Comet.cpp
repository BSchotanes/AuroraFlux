/***********************************************
* Filename:  		Asteroid.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will hold all of the data for the asteroids.				
************************************************/
#include "../StdAfx.h"
#include "Comet.h"
#include "../Collision/Physics.h"
/*****************************************************************
* CAsteroid():  Default Constructor will set its type. Initilizes asteroid unique data.
* Ins:			    None    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CComet::CComet(float _fRadius) : m_nWaveIndex(-1)
{
	m_nObjectType   = eLARGE_ASTEROID;	
	
	m_nHealth       = int(10 * _fRadius);
	m_nShields      = 0;
	m_nVelocityModifier = 4;
	m_nDamage		= 300;
	m_bIsHit        = false;
	m_bActive       = true;

	SetBV(CCollOBJ::Create(eSPH, D3DXVECTOR3(0,0,0), _fRadius));
	this->isScenery = true;
	m_fRadius = _fRadius;

	rotationamount = ( 0.10f / _fRadius );

	randomNegative = rand() % 2;

	m_d3dVelocity = D3DXVECTOR3(0,0,0);

	rotationDirectionX = rand() % 2; 
	rotationDirectionY = rand() % 2; 
	rotationDirectionZ = rand() % 2; 
	if (rotationDirectionX == 0 && rotationDirectionY == 0 && rotationDirectionZ == 0)
	{
		rotationDirectionX = 1; rotationDirectionZ = 1; 
	}

	//randomScale = ((rand() % 585) + 5) * 0.1f;
	randomScale = _fRadius;
	Scale(randomScale, randomScale, randomScale);

	m_esDeath.SetOwner(this);
	EffectInformation::CreateEffect(eBLUE_ENEMY_THRUSTER, &m_esEngineTrail, this,  true, false);
	SetEntityMass(LARGE_ASTEROID_MASS);
	m_fLifeTime = 0.0f;
}
void CComet::SetRadius(float _rad)
{
	D3DXMatrixIdentity(&m_d3dMatrix);
	m_fRadius = _rad;

	rotationamount = (  0.10f / _rad ); 
	((CSphere*)m_pBV)->SetRadius(_rad* ((CSphere*)m_pBV)->GetRadius());
	m_nHealth       = int(10 * _rad);
	randomNegative = rand() % 2;
	m_d3dVelocity = D3DXVECTOR3(0,0,0);
	if (randomNegative == 1)
	{
		rotationamount *= -1;
	}
	m_nVelocityModifier = 80/ _rad;
	if (_rad > 4.0f)
	{
		m_nObjectType = eLARGE_ASTEROID;
		Scale(_rad * 2, _rad * 2, _rad * 2);  
		((CSphere*)m_pBV)->SetRadius(m_fRadius * 7.5f);; // WU here, mess with this for me to get collision to work, change the 9.0 cause its too large, make sure it dont mess up the small ones too
	}
	else
	{
		Scale(_rad, _rad, _rad);  
		((CSphere*)m_pBV)->SetRadius(m_fRadius * 2.95f);; // WU here, mess with this for me to get collision to work, change the 9.0 cause its too large, make sure it dont mess up the small ones too
	}

	Rotate( float(rand() % 300), rotationDirectionX,rotationDirectionY,rotationDirectionZ);
}
CComet::~CComet(void)
{

}
/*****************************************************************
* Update():		Will update its internal timers and behaviors.
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CComet::Update(float _fElapedTime)
{
	CEnemy::Update(_fElapedTime);

	if (m_pCurrMoveBehavior)
	{
		m_pCurrMoveBehavior->Update(_fElapedTime);
	}

	if(!m_esEngineTrail.GetPlay())
	{
		m_esEngineTrail.ResetPosition();
		m_esEngineTrail.SetPlay(true);
	}

	m_fLifeTime += _fElapedTime;

	if (m_fLifeTime >= 18.0f)
	{
		m_fLifeTime = 0.0f;
		m_esEngineTrail.ResetPosition();
		m_esEngineTrail.SetPlay(false);
		m_bActive = false;
	}
	
		///// ////////////////////Foward Movement
		D3DXMATRIX d3dMat;
		D3DXMatrixIdentity( &d3dMat );
		d3dMat._43 = _fElapedTime * m_nVelocityModifier * 100 ; 
		SetMatrix( &( d3dMat * ( *GetMatrix() ) ) );
		//////////////////////////
	
// 		if (m_pTarget)
// 		{
// 			CPhysics::TurnTo(this, m_pTarget, _fElapedTime, 0.001f);
// 		}
	
}
/*****************************************************************
* Render():		Draws the updated asteroid and details with asteroids.
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CComet::Render() 
{

}

void CComet::DeathVFX()
{
	WwiseNS::SoundEvent(eAsteroidExplosion, ePlay, false, this);
	EffectInformation::CreateEffect(eASTEROID_DEATH, &m_esDeath, this, false, true);
}
