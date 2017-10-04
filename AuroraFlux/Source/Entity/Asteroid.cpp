/***********************************************
* Filename:  		Asteroid.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will hold all of the data for the asteroids.				
************************************************/
#include "../StdAfx.h"
#include "Asteroid.h"
#include "../Collision/Physics.h"
/*****************************************************************
* CAsteroid():  Default Constructor will set its type. Initilizes asteroid unique data.
* Ins:			    None    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CAsteroid::CAsteroid(float _fRadius) : m_nWaveIndex(-1), isMoving(false)
{
	m_nObjectType   = eASTEROID;	
	m_nAsteroidType = eASTEROID_GRASS; 
	if (_fRadius > 3.5f)
	{
		m_nObjectType = eLARGE_ASTEROID;
		m_nAsteroidType = eLARGE_ASTEROID;
	}
	m_nMainMenuType = 1;
	m_nHealth       = int(10 * _fRadius);
	m_nShields      = 0;
	m_nVelocityModifier = 4;
	m_nDamage		= 300;
	m_bIsHit        = false;
	m_bActive       = true;
	
	
	SetBV(CCollOBJ::Create(eSPH, D3DXVECTOR3(0,0,0), _fRadius, _fRadius));

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

	SetEntityMass(LARGE_ASTEROID_MASS);
}
void CAsteroid::SetRadius(float _rad)
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
		((CSphere*)m_pBV)->SetFrustumRadius(m_fRadius * 15);; // WU here, mess with this for me to get collision to work, change the 9.0 cause its too large, make sure it dont mess up the small ones too
	}
	else
	{
		Scale(_rad, _rad, _rad);  
		((CSphere*)m_pBV)->SetRadius(m_fRadius * 2.95f);; // WU here, mess with this for me to get collision to work, change the 9.0 cause its too large, make sure it dont mess up the small ones too
	}

	Rotate( float(rand() % 300), rotationDirectionX,rotationDirectionY,rotationDirectionZ);
}
CAsteroid::~CAsteroid(void)
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
void CAsteroid::Update(float _fElapedTime)
{
	CEnemy::Update(_fElapedTime);

	if (m_pCurrMoveBehavior)
	{
		m_pCurrMoveBehavior->Update(_fElapedTime);
	}

// 	if (isMoving)
// 	{
// 		///// ////////////////////Foward Movement
// 		D3DXMATRIX d3dMat;
// 		D3DXMatrixIdentity( &d3dMat );
// 		d3dMat._43 = _fElapedTime * m_nVelocityModifier; 
// 		SetMatrix( &( d3dMat * ( *GetMatrix() ) ) );
// 		//////////////////////////
// 	}
// 	else
	//{
		Rotate(_fElapedTime * rotationamount * 1.99f, rotationDirectionX,rotationDirectionY,rotationDirectionZ);
// 		if(abs(D3DXVec3Dot(&m_d3dVelocity, &m_d3dVelocity)) > EPSILON * 4)
// 		{
// 			D3DXMATRIX d3dTransMat;
// 
// 			D3DXVECTOR3 d3dLocalZ; D3DXVec3Normalize(&d3dLocalZ, &CPhysics::GetMat4Vec(eZAXIS, GetMatrix()));
// 			D3DXVECTOR3 _d3dLocalY; D3DXVec3Normalize(&_d3dLocalY, &CPhysics::GetMat4Vec(eYAXIS, GetMatrix()));
// 			D3DXVECTOR3 d3dLocalX; D3DXVec3Normalize(&d3dLocalX, &CPhysics::GetMat4Vec(eXAXIS, GetMatrix()));
// 
// 
// 			float fTmpLen = abs(D3DXVec3Length(&m_d3dVelocity));
// 
// 			if(m_d3dVelocity.x > EPSILON*2)
// 			{
// 				m_d3dVelocity.x = max(0, m_d3dVelocity.x - fTmpLen * _fElapedTime);
// 			}
// 			else if(m_d3dVelocity.x < -EPSILON*2)
// 			{
// 				m_d3dVelocity.x = min(0, m_d3dVelocity.x + fTmpLen * _fElapedTime);
// 			}
// 
// 			if(m_d3dVelocity.y > EPSILON*2)
// 			{
// 				m_d3dVelocity.y = max(0, m_d3dVelocity.y - fTmpLen * _fElapedTime);
// 			}
// 			else if(m_d3dVelocity.y < -EPSILON*2)
// 			{
// 				m_d3dVelocity.y = min(0, m_d3dVelocity.y + fTmpLen * _fElapedTime);
// 			}
// 
// 			if(m_d3dVelocity.z > EPSILON*2)
// 			{
// 				m_d3dVelocity.z = max(0, m_d3dVelocity.z - fTmpLen * _fElapedTime);
// 			}
// 			else if(m_d3dVelocity.z < -EPSILON*2)
// 			{
// 				m_d3dVelocity.z = min(0, m_d3dVelocity.z + fTmpLen * _fElapedTime);
// 			}
// 
// 			D3DXMatrixTranslation(&d3dTransMat, m_d3dVelocity.x * _fElapedTime, m_d3dVelocity.y * _fElapedTime, m_d3dVelocity.z * _fElapedTime);

			//	Rotate(_fElapedTime * rotationamount * D3DXVec3Dot(&m_d3dVelocity, &D3DXVECTOR3(0.3f,0.3f,0.3f)),rotationDirectionX,rotationDirectionY,rotationDirectionZ);

			//SetMatrix(&(*GetMatrix() * d3dTransMat));
	//	}
	//}
}
/*****************************************************************
* Render():		Draws the updated asteroid and details with asteroids.
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CAsteroid::Render() 
{

}

void CAsteroid::DeathVFX()
{
	WwiseNS::SoundEvent(eAsteroidExplosion, ePlay, false, this);
	EffectInformation::CreateEffect(eASTEROID_DEATH, &m_esDeath, this, false, true);
}
