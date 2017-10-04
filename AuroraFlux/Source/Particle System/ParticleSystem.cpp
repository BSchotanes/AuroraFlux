/***********************************************
* Filename:  		ParticleSystem.h
* Date:      		01/08/2013
* Mod. Date: 		01/08/2013
* Mod. Initials:	SW
* Author:    		Stephen Warren
* Purpose:   		This is the class that the Effect System will be using for particles			
************************************************/

#include "ParticleSystem.h"
#include "EffectSystem.h"
#include "../Collision/Physics.h"
#include "../Entity/Player.h"

/*****************************************************************
* CParticleSystem():		Default Constructor
*
* Ins:				
*
* Outs:			
*
* Returns:			
*
* Mod. Date:		      01/08/2013
* Mod. Initials:	      SW
*****************************************************************/
CParticleSystem::CParticleSystem(void) : m_fFrameOffset(0), m_fLifeTime(0.0f), m_fGravity(0.0f), m_uiSourceBlendModes(0), m_uiDestinationBlendModes(0),
	m_fFirstScale(0.0f, 0.0f), m_fSecondScale(0.0f, 0.0f),
	m_fThirdScale(0.0f, 0.0f), m_fFirstRotation(0.0f), m_fSecondRotation(0.0f), m_fThirdRotation(0.0f), m_uiFirstColor(0), m_uiSecondColor(0), m_uiThirdColor(0),
	m_fMiddleScaleTime(0.0f), m_fMiddleRotationTime(0.0f), m_fMiddleVelocityTime(0.0f), m_fMiddleColorTime(0.0f), m_fStartScaleTime(0.0f), m_fEndScaleTime(0.0f),
	m_fStartRotationTime(0.0f), m_fEndRotationTime(0.0f), m_fStartVelocityTime(0.0f), m_fEndVelocityTime(0.0f), m_fStartColorTime(0.0f), m_fEndColorTime(0.0f),
	m_d3dPositionOffset(0.0f, 0.0f, 0.0f), m_d3dFirstVelocity(0.0f, 0.0f, 0.0f), m_d3dSecondVelocity(0.0f, 0.0f, 0.0f), m_d3dThirdVelocity(0.0f, 0.0f, 0.0f),
	m_nTextureID(-1), m_bRepeating(false), fEmitterRate(0.0f), XYVelocity(D3DXVECTOR2(0.0f,0.0f)), fCurrEmitterTime(0.0f), uiNextParticle(0)
{



}

/*****************************************************************
* ~CParticleSystem():		Default Destructor
*
* Ins:				
*
* Outs:			
*
* Returns:			
*
* Mod. Date:		      01/08/2013
* Mod. Initials:	      SW
*****************************************************************/
CParticleSystem::~CParticleSystem(void)
{

}

/*****************************************************************
* Update():		      Update Each particle in this particle system
*
* Ins:			      float 	
*
* Outs:			
*
* Returns:			
*
* Mod. Date:		      01/08/2013
* Mod. Initials:	      SW
*****************************************************************/
void CParticleSystem::Update( float _fDeltaTime, D3DXMATRIX * _d3dCurrPosition, int _eType, CEntity* _pOwner, float _dist, float _infrontDist, D3DXVECTOR3* _d3dDir )
{
	if( _fDeltaTime > -EPSILON && _fDeltaTime < EPSILON )
		_fDeltaTime = EPSILON;

	fCurrEmitterTime -= _fDeltaTime;

	m_fFrameOffset -= _fDeltaTime;

	if(m_fFrameOffset > 0.0f)
	{
		return;
	}
	else if( m_fFrameOffset < 0.0 )
	{
		m_fFrameOffset = 0.0f;
	}

	if(m_bRepeating && fCurrEmitterTime <= 0.0f && m_vtParticles.size() > 0 )
	{
		fCurrEmitterTime = fEmitterRate;

		if(uiNextParticle >= m_uiNumberOfParticles)
		{
			uiNextParticle = 0;
		}

		if( _eType == ePLAYER_THRUSTERS || _eType == ePLAYER_IDLE || _eType == ePLAYER_JET_STREAMERS)
		{
			D3DXMATRIX ZeroMatrix;
			D3DXMatrixIdentity(&ZeroMatrix);

			ResetParticle(&m_vtParticles[uiNextParticle++], &ZeroMatrix);
		}
		else
		{
			ResetParticle(&m_vtParticles[uiNextParticle++], _d3dCurrPosition);
		}

		if( _eType == eSPACE_DUST && _dist != 0.0f)
		{		
			m_vtParticles[uiNextParticle-1].m_d3dPosition.x += (float)(rand()%550-275)*_dist;
			m_vtParticles[uiNextParticle-1].m_d3dPosition.y += (float)(rand()%550-275)*_dist;
			m_vtParticles[uiNextParticle-1].m_d3dPosition.z += (float)(rand()%550-275)*_dist;

			m_vtParticles[uiNextParticle-1].m_d3dPosition += (*_d3dDir)*_infrontDist;
		}
		else if( _eType == eWIN_LASER_CHARGE )	
		{
			m_vtParticles[uiNextParticle-1].m_d3dPosition.x += (float)(rand()%50-25)*_dist;
			m_vtParticles[uiNextParticle-1].m_d3dPosition.y += (float)(rand()%50-25)*_dist;
			m_vtParticles[uiNextParticle-1].m_d3dPosition.z += (float)(rand()%50-25)*_dist;
		}
	}


	unsigned int uiTotalParticles = m_vtParticles.size();
	for(unsigned int uiParticleIndex = 0; uiParticleIndex < uiTotalParticles; ++uiParticleIndex)
	{
		tParticles* tCurrParticle = &m_vtParticles[uiParticleIndex];
		if( !tCurrParticle->m_bAlive )
		{
			continue;
		}

		tCurrParticle->m_fLifeTime += _fDeltaTime;

		if(tCurrParticle->m_fLifeTime > m_fLifeTime)
		{
			tCurrParticle->m_bAlive = false;
			continue;
		}


		//Scale 
		float fSecondTime = (m_fMiddleScaleTime / m_fEndScaleTime) * m_fLifeTime; 
		float fFirstTime  = (m_fStartScaleTime / m_fEndScaleTime) * m_fLifeTime;
		float percentLife = 0.0f;

		if(tCurrParticle->m_fLifeTime < fSecondTime && tCurrParticle->m_fLifeTime > fFirstTime)
		{			
			percentLife =  (tCurrParticle->m_fLifeTime / fSecondTime); 
			tCurrParticle->m_d3dScale = m_fFirstScale + ( (m_fSecondScale - m_fFirstScale) * percentLife  );
		}
		else if(tCurrParticle->m_fLifeTime < m_fLifeTime && tCurrParticle->m_fLifeTime > fSecondTime)
		{
			/*	percentLife =  ((tCurrParticle->m_fLifeTime / m_fLifeTime) - (fSecondTime / m_fLifeTime));
			percentLife *=  (1 / (fSecondTime / m_fLifeTime)); */

			percentLife = (tCurrParticle->m_fLifeTime - fSecondTime) / (m_fEndColorTime - fSecondTime);

			tCurrParticle->m_d3dScale = m_fSecondScale + ( (m_fThirdScale - m_fSecondScale) * percentLife  );

		}


		//Rotation
		fSecondTime = (m_fMiddleRotationTime / m_fEndRotationTime) * m_fLifeTime; 
		fFirstTime  =  (m_fStartRotationTime / m_fEndRotationTime) * m_fLifeTime;

		if(tCurrParticle->m_fLifeTime < fSecondTime && tCurrParticle->m_fLifeTime > fFirstTime)
		{			
			percentLife = (tCurrParticle->m_fLifeTime / fSecondTime); 
			tCurrParticle->m_fRotation = m_fFirstRotation + ( (m_fSecondRotation - m_fFirstRotation) * percentLife  );
		}
		else
		{
			/*percentLife =  ((tCurrParticle->m_fLifeTime / m_fLifeTime) - (fSecondTime / m_fLifeTime));
			percentLife *=  (1 / (fSecondTime / m_fLifeTime)); */
			percentLife = (tCurrParticle->m_fLifeTime - fSecondTime) / (m_fEndColorTime - fSecondTime);

			tCurrParticle->m_fRotation = m_fSecondRotation + ( (m_fThirdRotation - m_fSecondRotation) * percentLife );
		}


		//Velocity
		fSecondTime = (m_fMiddleVelocityTime / m_fEndVelocityTime) * m_fLifeTime; 
		fFirstTime  = ( m_fStartVelocityTime / m_fEndVelocityTime) * m_fLifeTime;

		if(tCurrParticle->m_fLifeTime < fSecondTime && tCurrParticle->m_fLifeTime > fFirstTime)
		{			
			percentLife = (tCurrParticle->m_fLifeTime / fSecondTime); 
			tCurrParticle->m_d3dVelocity = m_d3dFirstVelocity + ( (m_d3dSecondVelocity - m_d3dFirstVelocity) * percentLife  );
		}
		else
		{
			/*percentLife =  ((tCurrParticle->m_fLifeTime / m_fLifeTime) - (fSecondTime / m_fLifeTime));
			percentLife *=  (1 / (fSecondTime / m_fLifeTime)); */

			percentLife = (tCurrParticle->m_fLifeTime - fSecondTime) / (m_fEndColorTime - fSecondTime);

			tCurrParticle->m_d3dVelocity = m_d3dSecondVelocity + ( (m_d3dThirdVelocity - m_d3dSecondVelocity) * percentLife );
		}

		if( _eType == eSPACE_DUST && _dist != 0.0f && _pOwner )
		{
			
			D3DXVECTOR3 dir = D3DXVECTOR3(*_d3dDir);
			dir.x = -dir.x;
			dir.y = -dir.y;
			dir.z = -dir.z;
			tCurrParticle->m_d3dVelocity = dir*100;
			float vel = ((CPlayer*)_pOwner)->GetVelocityModifier();
			float velMod = vel * vel * 0.255f;
						
			tCurrParticle->m_d3dVelocity = dir*velMod;

		}


		//Color
		UpdateColor(tCurrParticle);


		//Position
		if(_pOwner && (_eType == ePLAYER_CHANGE_RED || _eType == ePLAYER_CHANGE_BLUE))
		{
			D3DXMATRIX* _d3dCurPos = _pOwner->GetMatrix();
			D3DXVECTOR3 LocalX(_d3dCurPos->_11, _d3dCurPos->_12, _d3dCurPos->_13);
			D3DXVec3Normalize(&LocalX, &LocalX);
			D3DXVECTOR3 LocalY(_d3dCurPos->_21, _d3dCurPos->_22, _d3dCurPos->_23);
			D3DXVec3Normalize(&LocalY, &LocalY);
			D3DXVECTOR3 LocalZ(_d3dCurPos->_31, _d3dCurPos->_32, _d3dCurPos->_33);
			D3DXVec3Normalize(&LocalZ, &LocalZ);
			
			tCurrParticle->m_d3dPosition.x = _pOwner->GetPosition().x + LocalX.x * m_d3dPositionOffset.x + LocalY.x * m_d3dPositionOffset.y + LocalZ.x * m_d3dPositionOffset.z;
			tCurrParticle->m_d3dPosition.y = _pOwner->GetPosition().y + LocalX.y * m_d3dPositionOffset.x + LocalY.y * m_d3dPositionOffset.y + LocalZ.y * m_d3dPositionOffset.z;
			tCurrParticle->m_d3dPosition.z = _pOwner->GetPosition().z + LocalX.z * m_d3dPositionOffset.x + LocalY.z * m_d3dPositionOffset.y + LocalZ.z * m_d3dPositionOffset.z;
			//tCurrParticle->m_d3dPosition = m_d3dPositionOffset.x + m_d3dPositionOffset.y + m_d3dPositionOffset.z;
		}
		else
			tCurrParticle->m_d3dPosition = tCurrParticle->m_d3dPosition + tCurrParticle->m_d3dVelocity * _fDeltaTime;	

		tCurrParticle->m_d3dPosition.y -= m_fGravity * _fDeltaTime;

		if( _eType == ePLAYER_THRUSTERS || _eType == ePLAYER_IDLE || _eType == ePLAYER_JET_STREAMERS)
		{
			float tmpX = CInput::GetMouseChangeX();
			float tmpY = CInput::GetMouseChangeY();

			if(tmpX > 10.0f)
			{
				tmpX = -0.1f;
				tCurrParticle->m_d3dPosition.x += tmpX;
			}
			else if(tmpX < -10.0f)
			{
				tmpX = 0.1f;
				tCurrParticle->m_d3dPosition.x += tmpX;
			}

			if(tmpY > 10.0f)
			{
				tmpY = 0.1f;
				tCurrParticle->m_d3dPosition.y += tmpY;
			}
			else if(tmpY < -10.0f)
			{
				tmpY = -0.1f;
				tCurrParticle->m_d3dPosition.y += tmpY;
			}
		}

		if(_eType == eWIN_LASER_CHARGE)
		{
			D3DXVECTOR3 toOwner = _pOwner->GetPosition() - tCurrParticle->m_d3dPosition;
			D3DXVec3Normalize(&toOwner, &toOwner);
			tCurrParticle->m_d3dPosition += toOwner * _fDeltaTime * 100.0f;
		}
	}
}

void CParticleSystem::ResetParticle( tParticles * _tParticle, D3DXMATRIX* _d3dCurPos )
{
	_tParticle->m_fLifeTime	    = 0.0f;
	_tParticle->m_d3dScale		= m_fFirstScale;
	_tParticle->m_fRotation	    = m_fFirstRotation;
	_tParticle->m_uiColor		= m_uiFirstColor;
	_tParticle->m_d3dVelocity	= m_d3dFirstVelocity;
	_tParticle->m_bAlive		= true;
	
	_tParticle->m_d3dPosition	=  CPhysics::GetMat4Vec(ePOS, _d3dCurPos) + CPhysics::GetMat4Vec(eXAXIS, _d3dCurPos)*m_d3dPositionOffset.x +
						CPhysics::GetMat4Vec(eYAXIS, _d3dCurPos)*m_d3dPositionOffset.y + CPhysics::GetMat4Vec(eZAXIS, _d3dCurPos)*m_d3dPositionOffset.z;
	
}

void CParticleSystem::UpdateColor(tParticles* _tCurrParticle)
{
	float fSecondTime = (m_fMiddleColorTime / m_fEndColorTime) * m_fLifeTime; 
	float fFirstTime  = ( m_fStartColorTime / m_fEndColorTime) * m_fLifeTime;
	float fPercentLife = 0.0f;

	if(_tCurrParticle->m_fLifeTime < fSecondTime && _tCurrParticle->m_fLifeTime > fFirstTime)
	{			
		fPercentLife = (_tCurrParticle->m_fLifeTime / fSecondTime); 

		//Updating the current color of the particle using the same formula
		float fSA = (float)(m_uiFirstColor >> 24),  fSR = (float)( m_uiFirstColor << 8 >> 24),  fSG = (float)( m_uiFirstColor << 16 >> 24),  fSB = (float)( m_uiFirstColor << 24 >> 24);
		float fEA = (float)(m_uiSecondColor >> 24), fER = (float)( m_uiSecondColor << 8 >> 24), fEG = (float)( m_uiSecondColor << 16 >> 24), fEB = (float)( m_uiSecondColor << 24 >> 24);

		_tCurrParticle->m_uiColor = 0;
		_tCurrParticle->m_uiColor |= (unsigned int)(fSA + (fEA - fSA) * fPercentLife) << 24;
		_tCurrParticle->m_uiColor |= (unsigned int)(fSR + (fER - fSR) * fPercentLife) << 16;
		_tCurrParticle->m_uiColor |= (unsigned int)(fSG + (fEG - fSG) * fPercentLife) << 8;
		_tCurrParticle->m_uiColor |= (unsigned int)(fSB + (fEB - fSB) * fPercentLife);

	}
	else
	{

		fPercentLife = (_tCurrParticle->m_fLifeTime - fSecondTime) / (m_fEndColorTime - fSecondTime);

		//Updating the current color of the particle using the same formula
		float fSA = (float)(m_uiSecondColor >> 24), fSR = (float)( m_uiSecondColor << 8 >> 24), fSG = (float)( m_uiSecondColor << 16 >> 24), fSB = (float)( m_uiSecondColor << 24 >> 24);
		float fEA = (float)(m_uiThirdColor >> 24),  fER = (float)( m_uiThirdColor << 8 >> 24),  fEG = (float)( m_uiThirdColor << 16 >> 24),  fEB = (float)( m_uiThirdColor << 24 >> 24);

		_tCurrParticle->m_uiColor = 0;
		_tCurrParticle->m_uiColor |= (unsigned int)(fSA + (fEA - fSA) * fPercentLife) << 24;
		_tCurrParticle->m_uiColor |= (unsigned int)(fSR + (fER - fSR) * fPercentLife) << 16;
		_tCurrParticle->m_uiColor |= (unsigned int)(fSG + (fEG - fSG) * fPercentLife) << 8;
		_tCurrParticle->m_uiColor |= (unsigned int)(fSB + (fEB - fSB) * fPercentLife);

	}
}

void CParticleSystem::SetAllScaleValues(std::vector<D3DXVECTOR2>* _vfpScales)
{
	//Check for curretn system, will change if it is all vector
	if(_vfpScales->size() < 3 )
		return;

	m_fFirstScale  = (*_vfpScales)[0];
	m_fSecondScale = (*_vfpScales)[1];
	m_fThirdScale  = (*_vfpScales)[2];

}

void CParticleSystem::SetAllScaleTimes(std::vector<float>* _vfpScaleTimes)
{
	if(_vfpScaleTimes->size() < 3 )
		return;

	m_fStartScaleTime  = (*_vfpScaleTimes)[0];
	m_fMiddleScaleTime = (*_vfpScaleTimes)[1];
	m_fEndScaleTime    = (*_vfpScaleTimes)[2];

}

void CParticleSystem::SetAllRotationValues(std::vector<float>* _vfpRotations)
{
	if(_vfpRotations->size() < 3 )
		return;

	m_fFirstRotation  = (*_vfpRotations)[0];
	m_fSecondRotation = (*_vfpRotations)[1];
	m_fThirdRotation  = (*_vfpRotations)[2];
}

void CParticleSystem::SetAllRotationTimes(std::vector<float>* _vfpRotationTimes)
{
	if(_vfpRotationTimes->size() < 3 )
		return;

	m_fStartRotationTime  = (*_vfpRotationTimes)[0];
	m_fMiddleRotationTime = (*_vfpRotationTimes)[1];
	m_fEndRotationTime  = (*_vfpRotationTimes)[2];
}


void CParticleSystem::SetAllVelocityValues(std::vector<D3DXVECTOR3>* _vd3dpVelocities)
{
	if(_vd3dpVelocities->size() < 3 )
		return;

	m_d3dFirstVelocity  = (*_vd3dpVelocities)[0];
	m_d3dSecondVelocity = (*_vd3dpVelocities)[1];
	m_d3dThirdVelocity  = (*_vd3dpVelocities)[2];
}

void CParticleSystem::SetAllVelocityTimes(std::vector<float>* _vfpVelocityTimes)
{
	if(_vfpVelocityTimes->size() < 3 )
		return;

	m_fStartVelocityTime  = (*_vfpVelocityTimes)[0];
	m_fMiddleVelocityTime = (*_vfpVelocityTimes)[1];
	m_fEndVelocityTime    = (*_vfpVelocityTimes)[2];
}

void CParticleSystem::SetAllColorValues(std::vector<unsigned int>* _vuipColors)
{
	if(_vuipColors->size() < 3 )
		return;

	m_uiFirstColor  = (*_vuipColors)[0];
	m_uiSecondColor = (*_vuipColors)[1];
	m_uiThirdColor  = (*_vuipColors)[2];
}


void CParticleSystem::SetTexture(int _textID)
{
	m_nTextureID = _textID; 
}


void CParticleSystem::SetAllColorTimes(std::vector<float>* _vuipColorTimes)
{
	if(_vuipColorTimes->size() < 3 )
		return;

	m_fStartColorTime   = (*_vuipColorTimes)[0];
	m_fMiddleColorTime  = (*_vuipColorTimes)[1];
	m_fEndColorTime     = (*_vuipColorTimes)[2];
}


void CParticleSystem::Init(D3DXVECTOR3* _position)
{
	if(m_vtParticles.size() > 0)
	{
		return;
	}


	for( unsigned int uiIndex = 0; uiIndex < m_uiNumberOfParticles; ++uiIndex)
	{
		tParticles tempPart;
		tempPart.m_d3dPosition  = *_position + m_d3dPositionOffset;
		tempPart.m_d3dScale     = m_fFirstScale;
		tempPart.m_fLifeTime    = 0.0f;
		tempPart.m_d3dVelocity  = m_d3dFirstVelocity;
		tempPart.m_fRotation    = m_fStartRotationTime;
		tempPart.m_uiColor      = m_uiFirstColor;
		tempPart.m_bFacePlayer  = m_bFacePlayer;
		tempPart.m_bNoRotation  = m_bNoRotation;
		if(!m_bRepeating)
		{
			tempPart.m_bAlive = true;
		}

		m_vtParticles.push_back(tempPart);
	}
}

void CParticleSystem::ResetPosition( D3DXVECTOR3* _position )
{
	position  = *_position + m_d3dPositionOffset;

	for( unsigned int uiIndex = 0; uiIndex < m_uiNumberOfParticles; ++uiIndex)
	{
		m_vtParticles[uiIndex].m_d3dPosition = *_position;
	}
}
void CParticleSystem::ResetPosition( D3DXMATRIX _Matrix )
{
	D3DXMATRIX tmpMat;
	D3DXMatrixIdentity(&tmpMat);

	tmpMat._41 = m_d3dPositionOffset.x;
	tmpMat._42 = m_d3dPositionOffset.y;
	tmpMat._43 = m_d3dPositionOffset.z;

	D3DXMatrixMultiply(&_Matrix, &tmpMat, &_Matrix);

	position  = D3DXVECTOR3(_Matrix._41, _Matrix._42, _Matrix._43);

	for( unsigned int uiIndex = 0; uiIndex < m_uiNumberOfParticles; ++uiIndex)
	{
		m_vtParticles[uiIndex].m_d3dPosition = position;
	}
}