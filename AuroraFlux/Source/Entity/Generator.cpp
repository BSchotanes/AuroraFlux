/***********************************************
* Filename:  		Asteroid.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will hold all of the data for the asteroids.				
************************************************/
#include "../StdAfx.h"
#include "Generator.h"
#include "..\Collision\Physics.h"
/*****************************************************************
* CAsteroid():  Default Constructor will set its type. Initilizes asteroid unique data.
* Ins:	 		    None    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CGenerator::CGenerator(CEntity * _pOwner, int _index) : yMod(0)
{
	doOnce = true;
	index = _index;
	m_nObjectType   = eGENERATOR;
	m_nHealth       = 600;
	m_nShields      = 400;
	m_nVelocityModifier = 20;
	m_nDamage = 0;
	Scale(2,2,2);
	m_bIsHit        = false;
	m_bActive       = true;
	m_pOwner = _pOwner;
	SetBV(CCollOBJ::Create(eSPH, D3DXVECTOR3(0,0,0), 15.5f));
//	EffectInformation::CreateEffect(eGENERATORS_CONNECTION, &m_esShieldConnectionParticles, this, true, false);
}

/*Destructor*/
CGenerator::~CGenerator(void)
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
void CGenerator::Update(float _fElapedTime)
{
	CEnemy::Update(_fElapedTime);
	if(m_nHealth <= 0)
		WwiseNS::SoundEvent(eEnemyExplosion, ePlay, false, this);

	SetPosition( GetPosition().x, GetPosition().y, m_pOwner->GetPosition().z);
	//right
	if (index == 0)
	{

		if(doOnce)
		{
			Rotate(1.57f, 0,0,1);
			doOnce = false;
		}
	
		//Rotate(_fElapedTime / 3.1f , 0,0,1);
 
// 		D3DXMATRIX d;
// 		D3DXMatrixIdentity(&d);
// 		D3DXMatrixTranslation(&d, .12f,0, 0);
// 		*GetMatrix() = d * *GetMatrix();


	}
	//left
	else if (index == 1)
	{
		if(doOnce)
		{
			Rotate(-1.57f, 0,0,1);
			doOnce = false;
		}

// 		if(doOnce)
// 		{
// 			SetPosition(GetPosition().x,GetPosition().y, m_pOwner->GetPosition().z);
// 			doOnce = false;
// 		}

// 		Rotate(_fElapedTime / 3.1f , 1,0,0);

// 		D3DXMATRIX d;
// 		D3DXMatrixIdentity(&d);
// 		D3DXMatrixTranslation(&d, 0,0, .17f);
// 		*GetMatrix() = d * *GetMatrix();

	}
	//toop mid
	if (index == 2)
	{
	
// 		if(doOnce)
// 		{
// 			SetPosition(m_pOwner->GetPosition().x, GetPosition().y, m_pOwner->GetPosition().z);
// 			doOnce = false;
// 		}

// 		Rotate(_fElapedTime / 3.1f , 0,1,0);

// 		D3DXMATRIX d;
// 		D3DXMatrixIdentity(&d);
// 		D3DXMatrixTranslation(&d, 0,0, .15f);
// 		*GetMatrix() = d * *GetMatrix();
	}

	if (m_pCurrMoveBehavior)
	{
		m_pCurrMoveBehavior->Update(_fElapedTime);
	}

// 	if( !m_bIsVisable )
// 	{
// 		m_esShieldConnectionParticles.SetPlay(false);
// 	}
	else
	{
		vector<D3DXVECTOR3> vels;
		D3DXVECTOR3 toboss = m_pOwner->GetPosition() - GetPosition();
		D3DXVec3Normalize(&toboss, &toboss);
		for( int i(0); i < 3; ++i )
		{
			vels.push_back(D3DXVECTOR3(20*toboss.x * (rand()%20-10*0.1f), toboss.y*20 * (rand()%20-10*0.1f), toboss.z*20 * (rand()%20-10*0.1f))); 
		}

// 		for( unsigned int i(0); i < m_esShieldConnectionParticles.GetGeometrySystem()->size(); ++i )
// 		{
// 			for( unsigned int j(0); j < m_esShieldConnectionParticles.GetGeometrySystem()[i].size(); ++j )
// 			{
// 				m_esShieldConnectionParticles.GetGeometrySystem()[i][j].SetAllVelocityValues(&vels);
// 			}
// 		}
	}

// 	if(currHealthPercentage < .95f )
// 	{
// 		m_esDamage.SetEmitterRate(currHealthPercentage);
// 		m_esDamage.SetPlay(true);
// 
// 	}
}
/*****************************************************************
* Render():		Draws the updated asteroid and details with asteroids.
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CGenerator::Render() 
{

}

void CGenerator::SetActive(bool _isActive)
{	
	m_bActive = _isActive; 
// 	if( !_isActive )
// 	{
// 		m_esShieldConnectionParticles.SetPlay(_isActive);
// 	}
}