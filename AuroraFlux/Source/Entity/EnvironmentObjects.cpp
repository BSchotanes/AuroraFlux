/***********************************************
* Filename:  		Asteroid.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will hold all of the data for the asteroids.				
************************************************/
#include "../StdAfx.h"
#include "EnvironmentObjects.h"

/*****************************************************************
* CAsteroid():  Default Constructor will set its type. Initilizes asteroid unique data.
* Ins:	 		    None    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CEnvironmentObject::CEnvironmentObject(int _ObjectType, D3DXVECTOR3 _position, D3DXVECTOR3 _velocity, bool _boss, float _collisionRadius) : ShouldUpdate(false), m_d3dvVelocity(_velocity)
{
	m_bShrink = false;
	m_fScale = 1.0f;
	m_pOwner = NULL;
	if (_ObjectType != eSPACE_DEBRIS ||_ObjectType != eSPACE_DEBRIS2  || _ObjectType != eSPACE_DEBRIS3 ||  _ObjectType != eBARRIER )
	{
		SetIsScenery(true);
	}
	if (_ObjectType == eSPACE_STATION || _ObjectType == eASTEROID || _ObjectType == ePLANET4 ||  _ObjectType == eBARRIER
		|| _ObjectType == ePLANET3 || _ObjectType == ePLANET1  || _ObjectType == eSPACE_DEBRIS || _ObjectType == eSPACE_DEBRIS2 || _ObjectType == eSPACE_DEBRIS3 )
	{
		ShouldUpdate = true;
	}
	m_bIsBossHelper = _boss;
	doOnce = true;
	asteroidType = eASTEROID_DIRT;
	m_nObjectType = _ObjectType;
	m_nHealth       = 40;
	m_nShields      = 0;
	m_nVelocityModifier = 6;
	m_nTurnRate = 0.004f;
	m_nDamage = 0;
	m_bIsHit        = false;
	m_bActive       = true;
	isOrbit = false;
	
	if(_ObjectType == ePLANET1 || _ObjectType == ePLANET2 || _ObjectType == ePLANET3)
	{
		SetBV(CCollOBJ::Create(eSPH, D3DXVECTOR3(0,0,0), 20000.0f));
	}
	else if(_ObjectType == ePLANET4)
	{
		SetBV(CCollOBJ::Create(eSPH, D3DXVECTOR3(0,0,0), 10000.0f));
	}
	else if(_ObjectType == eSPACE_DEBRIS && _boss)
	{
		SetBV(CCollOBJ::Create(eSPH, D3DXVECTOR3(0,0,0), _collisionRadius));
	}
	else
	{
		SetBV(CCollOBJ::Create(eSPH, D3DXVECTOR3(0,0,0), 12.5f));
	}

	m_fDegree = 0.0f;
	m_d3dvRotation = D3DXVECTOR3(0,0,0);
	SetPosition(_position);

	EffectInformation::CreateEffect(ePLANET_EXPLOSION, &m_esDeathParticle, this, false, false);

	for( int i(0); i < NUM_SEMI_EXPLOSIONS; ++i )
	{
	//	EffectInformation::CreateEffect(ePLANET_SEMI_EXPLOSION, &m_esSemiExplosions[i], this, false, false);
	}
}

/*Destructor*/
CEnvironmentObject::~CEnvironmentObject(void)
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
void CEnvironmentObject::Update(float _fElapedTime)
{
	if( GetIsVisable() )
	{
		CEnemy::Update(_fElapedTime);
		if( m_bShrink )
		{
			Scale(m_fScale,m_fScale,m_fScale);
			m_fScale -= _fElapedTime;
		}

		if (ShouldUpdate)
		{
			switch (m_nObjectType)
			{
			case eSPACE_DEBRIS2:
			case eSPACE_DEBRIS3:
			case eSPACE_DEBRIS:
				{
					//Antone do code here:
					if( m_pOwner )
					{
						if( m_pOwner->GetType() == eMISSILE )
						{
							int x = 0;
						}
					}
					SetPosition(GetPosition() + m_d3dvVelocity * _fElapedTime);
					Rotate(m_fDegree, (int)(m_d3dvRotation.x), (int)(m_d3dvRotation.y), (int)(m_d3dvRotation.z));
				}
				break;
			case eSPACE_STATION:
				{
					Rotate(.15f * _fElapedTime, 0,1,0);

				}
				break;
			case eASTEROID:
				{
					Rotate(.01f * _fElapedTime, 1 ,1,1);
				}
				break;
			case ePLANET1:
			case ePLANET4:
				{
					Rotate(_fElapedTime * .0175f, 0,1,0);
				}
				break;
			}

			if(m_nObjectType == ePLANET4 && /*shouldOrbit*/ 0 ) // TODO add a check there so not all planets orbit
			{
				if(D3DXVec3Length(&m_d3dOffset) > 0)
				{
					D3DXVECTOR3 norm;	D3DXVec3Normalize(&norm, &m_d3dOffset);
					Rotate(_fElapedTime*20, (norm.x > 0.5f ? 1 : 0), (norm.y > 0.5f ? 1 : 0), (norm.z > 0.5f ? 1 : 0));
					D3DXMATRIX transMat;	D3DXMatrixTranslation(&transMat, m_d3dOffset.x, m_d3dOffset.y, m_d3dOffset.z);
					SetMatrix(&(transMat * *GetMatrix()));
				}
			}

			if (m_pCurrMoveBehavior)
			{
				m_pCurrMoveBehavior->Update(_fElapedTime);
			}
		}
	}
}
/*****************************************************************
* Render():		Draws the updated asteroid and details with
asteroids.
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CEnvironmentObject::Render() 
{

}

void CEnvironmentObject::CreateEffect(Effects eType)
{
	EffectInformation::CreateEffect(eType, &m_Effect, this, false, true);
}

void CEnvironmentObject::Destroy()
{
	if( GetIsVisable() )
	{
		WwiseNS::SoundEvent(eHydraPlanetDestroyed, ePlay);
		m_esDeathParticle.SetPlay(true);
		m_bShrink = true;
		m_bIsVisable = false;
	}
}

void CEnvironmentObject::PlaySemiExplosion(int num)
{
	if( num < NUM_SEMI_EXPLOSIONS && num > -1 )
	{
		if( m_esSemiExplosions[num].GetPlay() )
		{
			m_esSemiExplosions[num].Init();
		}
		else
		{
			m_esSemiExplosions[num].SetPlay(true);
		}
	}
}