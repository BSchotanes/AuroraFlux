/***********************************************
* Filename:  		EffectSystem.cpp
* Date:      		01/09/2013
* Mod. Date: 		01/09/2013
* Mod. Initials:	SW
* Author:    		Stephen Warren
* Purpose:   		This is the class that will control all the Effects in the game			
************************************************/
#include "EffectSystem.h"
#include "../Collision/Physics.h"
#include "../Entity/Player.h"
#include "../Entity/Boss.h"

CEffectSystem::CEffectSystem() : m_fLifetime(0.0f), m_pAssetManager(nullptr), m_fCurrLife(0.0f), m_bIsAlive(true), m_bRepeatable(false), m_bPlay(true), m_nEffectType(-1)
{

}

CEffectSystem::~CEffectSystem()
{

}

void CEffectSystem::Update(float _fDeltaTime)
{

	if(m_bPlay == false)
	{
		return;
	}

	m_fCurrLife += _fDeltaTime;

	unsigned int uiVectorSize = m_ParticleSystem.size();
	unsigned int uiCurrVectorIndex = 0;
	for(; uiCurrVectorIndex < uiVectorSize; ++uiCurrVectorIndex)
	{
		if( m_pOwner )
		{
			switch( m_nEffectType )
			{
			case eSPACE_DUST:
				{
					float dist = 1.25f;
					D3DXVECTOR3 dir;
					CCamera *pCamera = ((CPlayer*)m_pOwner)->GetCamera();
					if( m_nEffectType == eSPACE_DUST && pCamera->GetIsDetached() )
					{
						dir = CPhysics::GetMat4Vec(eZAXIS, pCamera->GetCurrentMatrix());
					}
					else
					{
						dir = CPhysics::GetMat4Vec(eZAXIS, m_pOwner->GetMatrix());
					}

					float infrontdist = 500.0f;
	
						
					if( m_nEffectType == eSPACE_DUST )
					{		
						if( ((CPlayer*)m_pOwner)->GetVelocityModifier() > 100 )
						{
							dist = 3.0f;
							m_ParticleSystem[uiCurrVectorIndex].SetNumberOfParticles(39);
							m_ParticleSystem[uiCurrVectorIndex].SetEmitterRate(0.005f);
							m_ParticleSystem[uiCurrVectorIndex].SetSecondScale(1.30f, 1.30f);
							infrontdist = 2000.0f;
						}
						else if( ((CPlayer*)m_pOwner)->GetVelocityModifier() > 40 )
						{
							dist = 2.0f;
							m_ParticleSystem[uiCurrVectorIndex].SetNumberOfParticles(39);
							m_ParticleSystem[uiCurrVectorIndex].SetEmitterRate(0.09f);
							m_ParticleSystem[uiCurrVectorIndex].SetSecondScale(0.75f, 0.75f);
							infrontdist = 1000.0f;
						}
						else
						{
							dist = 1.0f;
							m_ParticleSystem[uiCurrVectorIndex].SetNumberOfParticles(0);
							m_ParticleSystem[uiCurrVectorIndex].SetSecondScale(0.0f, 0.0f);
							infrontdist = -2000.0f;
						}

						D3DXVec3Normalize(&dir, &dir);
						if( pCamera->GetIsDetached())
						{
							m_ParticleSystem[uiCurrVectorIndex].Update( _fDeltaTime, pCamera->GetCurrentMatrix(), m_nEffectType, NULL, dist, infrontdist, &dir );
						}
						else
						{
							m_ParticleSystem[uiCurrVectorIndex].Update( _fDeltaTime, m_pOwner->GetMatrix(), m_nEffectType, m_pOwner, dist, infrontdist, &dir );
						}
					}
				}
				break;
			case eWIN_LASER_CHARGE:
				{
					m_ParticleSystem[uiCurrVectorIndex].Update( _fDeltaTime, m_pOwner->GetMatrix(), m_nEffectType, m_pOwner, 4.0f);		
				}
				break;
			case eBOSS_EXPLOSION:
				{
					float dt = 1.0f;
					if( _fDeltaTime < 0.001f )
					{
						dt = 1000.0f;
					}
					if( m_ParticleSystem[uiCurrVectorIndex].GetFrameOffset() < 1.6f )
					{
						float offset = (rand()%20) * 0.1f;
						m_ParticleSystem[uiCurrVectorIndex].SetFrameOffset( m_ParticleSystem[uiCurrVectorIndex].GetFrameOffset() + offset);
					}
					m_ParticleSystem[uiCurrVectorIndex].Update(_fDeltaTime * dt, m_pOwner->GetMatrix(), m_nEffectType, m_pOwner);
				}
				break;
			case eBOSS_WARPIN:
				{
					if( m_pOwner->GetType() == eBOSS )
					{
						if( ((CBoss*)(m_pOwner))->GetPhase() == eCINEMATIC_PHASE )
						{
							m_ParticleSystem[uiCurrVectorIndex].Update(_fDeltaTime * 1000.0f, m_pOwner->GetMatrix(), m_nEffectType, m_pOwner);
						}
					}
				}
				break;
			default:
				{
					m_ParticleSystem[uiCurrVectorIndex].Update(_fDeltaTime, m_pOwner->GetMatrix(), m_nEffectType, m_pOwner);
				}
				break;
			};
		}
		else
		{
			D3DXMATRIX iden;
			D3DXMatrixIdentity(&iden);
			m_ParticleSystem[uiCurrVectorIndex].Update(_fDeltaTime, &iden, m_nEffectType, m_pOwner);
		}
	}

	uiVectorSize = m_GeometrySystem.size();
	uiCurrVectorIndex = 0;
	for(; uiCurrVectorIndex < uiVectorSize; ++uiCurrVectorIndex)
	{
		if( m_pOwner )
		{
			if( m_nEffectType == eWARP_DUST )
			{
				float dist = 5.0f;
				float infrontdist = 5000.0f;

				D3DXVECTOR3 dir;
				CCamera *pCamera = ((CPlayer*)m_pOwner)->GetCamera();
				dir = CPhysics::GetMat4Vec(eZAXIS, pCamera->GetCurrentMatrix());
				
				D3DXVec3Normalize(&dir, &dir);
				if( pCamera->GetIsDetached())
				{
					m_GeometrySystem[uiCurrVectorIndex].Update( _fDeltaTime, pCamera->GetCurrentMatrix(), m_nEffectType, NULL, dist, infrontdist, &dir );
				}
				else
				{
					m_GeometrySystem[uiCurrVectorIndex].Update( _fDeltaTime, m_pOwner->GetMatrix(), m_nEffectType, m_pOwner, dist, infrontdist, &dir );
				}
			}
			else
			{
				m_GeometrySystem[uiCurrVectorIndex].Update(_fDeltaTime, m_pOwner->GetMatrix());
			}
		}
		else
		{
			D3DXMATRIX iden;
			D3DXMatrixIdentity(&iden);
			m_GeometrySystem[uiCurrVectorIndex].Update(_fDeltaTime, &iden);
		}
	}	

	if(m_fCurrLife > m_fLifetime)
	{
		if( !m_bRepeatable)
		{
			m_bIsAlive = false;
		}
		else
		{
			m_fCurrLife = 0.0f;
		}
	}

}


void CEffectSystem::CreateEffect(tEffectSystemCreationInfo* _pEffectInfo, bool _bRepeatable, bool _bPlay )
{

	int nNumberofParticleSystems = _pEffectInfo->m_nNumberOfParticleSystems;
	m_ParticleSystem.clear();

	m_fLifetime = _pEffectInfo->m_fLifetime;
	m_bIsAlive  = true;
	m_fCurrLife = 0.0f;
	m_bPlay		= _bPlay;
	m_bRepeatable = _bRepeatable;

	for(int nParticleIndex = 0; nParticleIndex < nNumberofParticleSystems; ++nParticleIndex)
	{
		CParticleSystem ps;
		

		//Frame
		ps.SetFrameOffset(_pEffectInfo->m_vParticleGeneralInfo[nParticleIndex].m_fFrameOffset);

		//Face Player
		ps.SetFacePlayer(_pEffectInfo->m_vParticleGeneralInfo[nParticleIndex].m_bFacePlayer);
		ps.SetNoRotation(_pEffectInfo->m_vParticleGeneralInfo[nParticleIndex].m_bNoRotation);
		//Lifetime
		ps.SetLifeTime(_pEffectInfo->m_vParticleGeneralInfo[nParticleIndex].m_fLifetime); 

		//Scale
		ps.SetAllScaleValues(&_pEffectInfo->m_vParticleScaleInfo[nParticleIndex].m_d3dValues);
		ps.SetAllScaleTimes(&_pEffectInfo->m_vParticleScaleInfo[nParticleIndex].m_fTime);

		//Rotation
		ps.SetAllRotationValues(&_pEffectInfo->m_vParticleRotationInfo[nParticleIndex].m_fValues);
		ps.SetAllRotationTimes(&_pEffectInfo->m_vParticleRotationInfo[nParticleIndex].m_fTime);


		//Velocity
		ps.SetAllVelocityValues(&_pEffectInfo->m_vParticleVelocityInfo[nParticleIndex].m_d3dValues);
		ps.SetAllVelocityTimes(&_pEffectInfo->m_vParticleVelocityInfo[nParticleIndex].m_fTime);

		//Color
		ps.SetAllColorValues(&_pEffectInfo->m_vParticleColorInfo[nParticleIndex].m_uiValues);
		ps.SetAllColorTimes(&_pEffectInfo->m_vParticleColorInfo[nParticleIndex].m_fTime);

		//Gravity
		ps.SetGravity(_pEffectInfo->m_vParticleGeneralInfo[nParticleIndex].m_fGravity);

		//position offset
		ps.SetPositionOffset(_pEffectInfo->m_vParticleGeneralInfo[nParticleIndex].m_d3dPositionOffset);

		//Blend Mode
		ps.SetBlendModes(_pEffectInfo->m_vParticleGeneralInfo[nParticleIndex].m_uiSourceBlend, _pEffectInfo->m_vParticleGeneralInfo[nParticleIndex].m_uiDestinationBlend );

		//number of particles
		ps.SetNumberOfParticles(_pEffectInfo->m_vParticleGeneralInfo[nParticleIndex].m_nNumberOfParticles);

		//Emitter Rate
		ps.SetEmitterRate(_pEffectInfo->m_vParticleGeneralInfo[nParticleIndex].m_fEmitterRate);

		//texture ID
		ps.SetTexture(_pEffectInfo->m_vParticleGeneralInfo[nParticleIndex].m_nTextureID );

		//Repeatable
		ps.SetRepeating(_bRepeatable);

		//Init
		if( m_pOwner )
		{
			ps.Init(&m_pOwner->GetPosition());
		}
		else
		{
			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			ps.Init(&pos);
		}

		m_ParticleSystem.push_back(ps);
	}


	int nNumberofGeometrySystems = _pEffectInfo->m_nNumberOfGeometrySystems;
	m_GeometrySystem.clear();

	for(int nGeometryIndex = 0; nGeometryIndex < nNumberofGeometrySystems; ++nGeometryIndex)
	{
		CGeometrySystem gs;
		
		//Frame
		gs.SetTimeOffset(_pEffectInfo->m_vGeometryGeneralInfo[nGeometryIndex].m_fTimeOffset);

		//Lifetime
		gs.SetLifeTime(_pEffectInfo->m_vGeometryGeneralInfo[nGeometryIndex].m_fLifetime); 

		//Scale
		gs.SetAllScaleValues(&_pEffectInfo->m_vGeometryScaleInfo[nGeometryIndex].m_d3dValues);
		gs.SetAllScaleTimes(&_pEffectInfo->m_vGeometryScaleInfo[nGeometryIndex].m_fTime);

		//Rotation
		gs.SetAllRotationValues(&_pEffectInfo->m_vGeometryRotationInfo[nGeometryIndex].m_d3dValues);
		gs.SetAllRotationTimes(&_pEffectInfo->m_vGeometryRotationInfo[nGeometryIndex].m_fTime);


		//Velocity
		gs.SetAllVelocityValues(&_pEffectInfo->m_vGeometryVelocityInfo[nGeometryIndex].m_d3dValues);
		gs.SetAllVelocityTimes(&_pEffectInfo->m_vGeometryVelocityInfo[nGeometryIndex].m_fTime);

		//Color
		gs.SetAllColorValues(&_pEffectInfo->m_vGeometryColorInfo[nGeometryIndex].m_uiValues);
		gs.SetAllColorTimes(&_pEffectInfo->m_vGeometryColorInfo[nGeometryIndex].m_fTime);

		//Gravity
		gs.SetGravity(_pEffectInfo->m_vGeometryGeneralInfo[nGeometryIndex].m_fGravity);

		//position offset
		gs.SetPositionOffset(_pEffectInfo->m_vGeometryGeneralInfo[nGeometryIndex].m_d3dPositionOffset);

		//Source Blend, Destination Blend
		gs.SetBlendModes(_pEffectInfo->m_vGeometryGeneralInfo[nGeometryIndex].m_uiSourceBlend, _pEffectInfo->m_vGeometryGeneralInfo[nGeometryIndex].m_uiDestinationBlend );

		//number of particles
		gs.SetNumberOfGeometry(_pEffectInfo->m_vGeometryGeneralInfo[nGeometryIndex].m_uiNumberOfGeometry);

		//Emitter Rate
		gs.SetEmitterRate(_pEffectInfo->m_vGeometryGeneralInfo[nGeometryIndex].m_fEmitterRate);

		//texture ID
		gs.SetTexture(_pEffectInfo->m_vGeometryGeneralInfo[nGeometryIndex].m_nTextureID );

		//Mesh ID
		gs.SetMesh(_pEffectInfo->m_vGeometryGeneralInfo[nGeometryIndex].m_nMeshID );

		//Repeatable
		gs.SetRepeating(_bRepeatable);

		//Init
		gs.Init(&m_pOwner->GetPosition());

		m_GeometrySystem.push_back(gs);
	}


}

void CEffectSystem::Init()
{
	unsigned int uiParticleSize = m_ParticleSystem.size();
	for(unsigned int uiParticleSystemIndex = 0; uiParticleSystemIndex < uiParticleSize; ++uiParticleSystemIndex)
	{
		m_ParticleSystem[uiParticleSystemIndex].Init(&m_pOwner->GetPosition());
	}

	unsigned int uiGeometrySize = m_GeometrySystem.size();
	for(unsigned int uiGeometrySystemIndex = 0; uiGeometrySystemIndex < uiGeometrySize; ++uiGeometrySystemIndex)
	{
		m_GeometrySystem[uiGeometrySystemIndex].Init(&m_pOwner->GetPosition());
	}

}

void CEffectSystem::ResetPosition()
{
	unsigned int uiParticleSize = m_ParticleSystem.size();
	for(unsigned int uiParticleSystemIndex = 0; uiParticleSystemIndex < uiParticleSize; ++uiParticleSystemIndex)
	{
		m_ParticleSystem[uiParticleSystemIndex].ResetPosition(*m_pOwner->GetMatrix());
	}

	unsigned int uiGeometrySize = m_GeometrySystem.size();
	for(unsigned int uiGeometrySystemIndex = 0; uiGeometrySystemIndex < uiGeometrySize; ++uiGeometrySystemIndex)
	{
		m_GeometrySystem[uiGeometrySystemIndex].ResetPosition(*m_pOwner->GetMatrix());
	}
}


void CEffectSystem::SetRepeatible(bool _bRepeat)
{	
	m_bRepeatable = _bRepeat;

	unsigned int uiParticleSize = m_ParticleSystem.size();
	for(unsigned int uiParticleSystemIndex = 0; uiParticleSystemIndex < uiParticleSize; ++uiParticleSystemIndex)
	{
		m_ParticleSystem[uiParticleSystemIndex].SetRepeating(true);
	}

	unsigned int uiGeometrySize = m_GeometrySystem.size();
	for(unsigned int uiGeometrySystemIndex = 0; uiGeometrySystemIndex < uiGeometrySize; ++uiGeometrySystemIndex)
	{
		m_GeometrySystem[uiGeometrySystemIndex].SetRepeating(true);
	}
}

void CEffectSystem::SetEmitterRate( float _fRate )
{
	unsigned int uiParticleSize = m_ParticleSystem.size();
	for(unsigned int uiParticleSystemIndex = 0; uiParticleSystemIndex < uiParticleSize; ++uiParticleSystemIndex)
	{
		m_ParticleSystem[uiParticleSystemIndex].SetEmitterRate(_fRate);
	}

	unsigned int uiGeometrySize = m_GeometrySystem.size();
	for(unsigned int uiGeometrySystemIndex = 0; uiGeometrySystemIndex < uiGeometrySize; ++uiGeometrySystemIndex)
	{
		m_GeometrySystem[uiGeometrySystemIndex].SetEmitterRate(_fRate);
	}
}
