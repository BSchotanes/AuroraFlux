/***********************************************
* Filename:  		GeometrySystem.h
* Date:      		01/09/2013
* Mod. Date: 		01/09/2013
* Mod. Initials:	SW
* Author:    		Stephen Warren
* Purpose:   		This is the class that the Effect System will be usbiing for geometry		
************************************************/

#include "GeometrySystem.h"
#include "../Collision/Physics.h"

CGeometrySystem::CGeometrySystem() : m_fLifetime(0.0f), m_fTimeOffset(0.0f), m_fOriginalOffset(0.0f), m_bLooping(false), m_bAlive(true),
	m_d3dFirstRotation(0.0f, 0.0f, 0.0f), m_d3dSecondRotation(0.0f, 0.0f, 0.0f), m_d3dThirdRotation(0.0f, 0.0f, 0.0f),
	m_d3dFirstScale(0.0f, 0.0f, 0.0f), m_d3dSecondScale(0.0f, 0.0f, 0.0f), m_d3dThirdScale(0.0f, 0.0f, 0.0f),
	m_uiFirstColor(0), m_uiSecondColor(0), m_uiThirdColor(0), m_uiNumberOfGeometry(0), m_nMeshID(-1), m_nTextureID(-1), m_fGravity(0.0f),
	m_bRepeating(false), m_d3dPosition(0.0f, 0.0f, 0.0f), m_fEmitterRate(0.0f), m_fCurrEmitterTime(0.0f), m_uiNextGeometry(0), m_uiDestinationBlendModes(5),
	m_uiSourceBlendModes(7)
{

}

CGeometrySystem::~CGeometrySystem()
{

}

void CGeometrySystem::Update(float _fDeltaTime, D3DXMATRIX * _d3dCurrPosition, int _eType, CEntity* _pOwner, float _dist, float _infrontDist, D3DXVECTOR3* _dir)
{
	m_fCurrEmitterTime -= _fDeltaTime;

	if(m_bRepeating && m_fCurrEmitterTime < 0.0f && m_vtGeometry.size() > 0 )
	{
		m_fCurrEmitterTime = m_fEmitterRate;

		if(m_uiNextGeometry >= m_uiNumberOfGeometry)
		{
			m_uiNextGeometry = 0;
		}

		ResetGeometry(&m_vtGeometry[m_uiNextGeometry++], _d3dCurrPosition);

		if( _eType == eWARP_DUST && _dist != 0.0f)
		{		

			float x, y, z;
			x = (float)(rand()%550-275)*_dist;
			y = (float)(rand()%550-275)*_dist;
			z = (float)(rand()%550-275)*_dist;

			if( x < 100.0f && x > -100.0f )
			{
				if( x < 0 )
				{
					x -= 100.0f;
				}
				else
				{
					x += 100.0f;
				}
			}
			if( y < 100.0f && y > -100.0f )
			{
				if( y < 0 )
				{
					y -= 100.0f;
				}
				else
				{
					y += 100.0f;
				}
			}
			if( z < 100.0f && z > -100.0f )
			{
				if( z < 0 )
				{
					z -= 100.0f;
				}
				else
				{
					z += 100.0f;
				}
			}

			m_vtGeometry[m_uiNextGeometry-1].m_d3dPosition.x += x;
			m_vtGeometry[m_uiNextGeometry-1].m_d3dPosition.y += y;
			m_vtGeometry[m_uiNextGeometry-1].m_d3dPosition.z += z;

			m_vtGeometry[m_uiNextGeometry-1].m_d3dPosition += (*_dir)*_infrontDist;
		}
	}

	unsigned int uiTotalGeometry = m_vtGeometry.size();
	for(unsigned int uiGeometryIndex = 0; uiGeometryIndex < uiTotalGeometry; ++uiGeometryIndex)
	{
		tGeometry* tCurrGeometry = &m_vtGeometry[uiGeometryIndex];

		tCurrGeometry->m_fLifetime += _fDeltaTime;

		m_fTimeOffset -= _fDeltaTime;

		if(m_fTimeOffset > 0.0f)
		{
			return;
		}


		if(tCurrGeometry->m_fLifetime > m_fLifetime + m_fOriginalOffset)
		{
			continue;
		}

		//Scale 
		float fSecondTime = (m_fMiddleScaleTime / m_fEndScaleTime) * m_fLifetime; 
		float fFirstTime  = (m_fStartScaleTime / m_fEndScaleTime) * m_fLifetime;
		float percentLife = 0.0f;

		if(tCurrGeometry->m_fLifetime < fSecondTime && tCurrGeometry->m_fLifetime > fFirstTime)
		{			
			percentLife =  (tCurrGeometry->m_fLifetime / fSecondTime); 
			tCurrGeometry->m_d3dScale = m_d3dFirstScale + ( (m_d3dSecondScale - m_d3dFirstScale) * percentLife  );
		}
		else if(tCurrGeometry->m_fLifetime < m_fLifetime && tCurrGeometry->m_fLifetime > fSecondTime)
		{
			/*percentLife =  ((tCurrGeometry->m_fLifetime / m_fLifetime) - (fSecondTime / m_fLifetime));
			percentLife *=  (1 / (fSecondTime / m_fLifetime)); */

			percentLife = (tCurrGeometry->m_fLifetime - fSecondTime) / (m_fEndColorTime - fSecondTime);
			tCurrGeometry->m_d3dScale = m_d3dSecondScale + ( (m_d3dThirdScale - m_d3dSecondScale) * percentLife  );

		}


		//Rotation
		fSecondTime = (m_fMiddleRotationTime / m_fEndRotationTime) * m_fLifetime; 
		fFirstTime  =  (m_fStartRotationTime / m_fEndRotationTime) * m_fLifetime;

		if(tCurrGeometry->m_fLifetime < fSecondTime && tCurrGeometry->m_fLifetime > fFirstTime)
		{			
			percentLife = (tCurrGeometry->m_fLifetime / fSecondTime); 
			tCurrGeometry->m_d3dRotation = m_d3dFirstRotation + ( (m_d3dSecondRotation - m_d3dFirstRotation) * percentLife  );
		}
		else
		{
			/*percentLife =  ((tCurrGeometry->m_fLifetime / m_fLifetime) - (fSecondTime / m_fLifetime));
			percentLife *=  (1 / (fSecondTime / m_fLifetime)); */
			percentLife = (tCurrGeometry->m_fLifetime - fSecondTime) / (m_fEndColorTime - fSecondTime);
			tCurrGeometry->m_d3dRotation = m_d3dSecondRotation + ( (m_d3dThirdRotation - m_d3dSecondRotation) * percentLife );
		}


		//Velocity
		fSecondTime = (m_fMiddleVelocityTime / m_fEndVelocityTime) * m_fLifetime; 
		fFirstTime  = ( m_fStartVelocityTime / m_fEndVelocityTime) * m_fLifetime;


		if(tCurrGeometry->m_fLifetime < fSecondTime && tCurrGeometry->m_fLifetime > fFirstTime)
		{			
			percentLife = (tCurrGeometry->m_fLifetime / fSecondTime); 
			tCurrGeometry->m_d3dVelocity = m_d3dFirstVelocity + ( (m_d3dSecondVelocity - m_d3dFirstVelocity) * percentLife  );
		}
		else
		{
			/*percentLife =  ((tCurrGeometry->m_fLifetime / m_fLifetime) - (fSecondTime / m_fLifetime));
			percentLife *=  (1 / (fSecondTime / m_fLifetime));*/
			percentLife = (tCurrGeometry->m_fLifetime - fSecondTime) / (m_fEndColorTime - fSecondTime);
			tCurrGeometry->m_d3dVelocity = m_d3dSecondVelocity + ( (m_d3dThirdVelocity - m_d3dSecondVelocity) * percentLife );
		}


		//Color
		UpdateColor(tCurrGeometry);
		
		if( _eType == eWARP_DUST && _dist != 0.0f && _pOwner )
		{			
			D3DXVECTOR3 dir = D3DXVECTOR3(*_dir);
			dir.x = -dir.x;
			dir.y = -dir.y;
			dir.z = -dir.z;						
			tCurrGeometry->m_d3dVelocity = dir*4000;

		//	tCurrGeometry->m_d3dScale = dir*20.0f;
		}

		//Position3
		tCurrGeometry->m_d3dPosition += tCurrGeometry->m_d3dVelocity * _fDeltaTime;  
		tCurrGeometry->m_d3dPosition.y -= m_fGravity * _fDeltaTime;

	}
}

void CGeometrySystem::Init(D3DXVECTOR3* position)
{

	if(m_vtGeometry.size() > 0)
	{
		return;
	}

	for( unsigned int uiIndex = 0; uiIndex < m_uiNumberOfGeometry; ++uiIndex)
	{
		tGeometry tempGeom;
		tempGeom.m_d3dPosition = *position + m_d3dPositionOffset;
		tempGeom.m_d3dScale    = m_d3dFirstScale;
		tempGeom.m_fLifetime   = 0.0f;
		tempGeom.m_d3dVelocity = m_d3dFirstVelocity;
		tempGeom.m_d3dRotation = m_d3dFirstRotation;
		tempGeom.m_uiColor     = m_uiFirstColor;

		m_vtGeometry.push_back(tempGeom);
	}
}

void CGeometrySystem::ResetGeometry(tGeometry * _tGeometry, D3DXMATRIX* _d3dCurPos)
{
	_tGeometry->m_fLifetime	    = 0.0f;
	_tGeometry->m_d3dScale	    = m_d3dFirstScale;
	_tGeometry->m_d3dRotation   = m_d3dFirstRotation;
	_tGeometry->m_uiColor	    = m_uiFirstColor;
	_tGeometry->m_d3dVelocity   = m_d3dFirstVelocity;

	//TODO SW Set Position to emitter + offset...check if fixed
	_tGeometry->m_d3dPosition   = CPhysics::GetMat4Vec(ePOS, _d3dCurPos) + CPhysics::GetMat4Vec(eXAXIS, _d3dCurPos)*m_d3dPositionOffset.x +
		CPhysics::GetMat4Vec(eYAXIS, _d3dCurPos)*m_d3dPositionOffset.y + CPhysics::GetMat4Vec(eZAXIS, _d3dCurPos)*m_d3dPositionOffset.z;

}

void CGeometrySystem::SetAllRotationValues( std::vector<D3DXVECTOR3>* _vpRotations )
{
	if(_vpRotations->size() < 3 )
		return;

	m_d3dFirstRotation  = (*_vpRotations)[0];
	m_d3dSecondRotation = (*_vpRotations)[1];
	m_d3dThirdRotation  = (*_vpRotations)[2];
}

void CGeometrySystem::SetAllRotationTimes(std::vector<float>* _vfpRotationTimes)
{
	if(_vfpRotationTimes->size() < 3 )
		return;

	m_fStartRotationTime  = (*_vfpRotationTimes)[0];
	m_fMiddleRotationTime = (*_vfpRotationTimes)[1];
	m_fEndRotationTime  = (*_vfpRotationTimes)[2];

	CheckValidValue(m_fMiddleRotationTime);
	CheckValidValue(m_fEndRotationTime);
}

void CGeometrySystem::SetAllScaleValues(std::vector<D3DXVECTOR3>* _vd3dpScales)
{
	if(_vd3dpScales->size() < 3 )
		return;

	m_d3dFirstScale  = (*_vd3dpScales)[0];
	m_d3dSecondScale = (*_vd3dpScales)[1];
	m_d3dThirdScale  = (*_vd3dpScales)[2];
}

void CGeometrySystem::SetAllScaleTimes(std::vector<float>* _vfpScaleTimes)
{
	if(_vfpScaleTimes->size() < 3 )
		return;

	m_fStartScaleTime  = (*_vfpScaleTimes)[0];
	m_fMiddleScaleTime = (*_vfpScaleTimes)[1];
	m_fEndScaleTime    = (*_vfpScaleTimes)[2];

	CheckValidValue(m_fMiddleScaleTime);
	CheckValidValue(m_fEndScaleTime);
}

void CGeometrySystem::SetAllColorValues(std::vector<unsigned int>* _vuipColors)
{
	if(_vuipColors->size() < 3 )
		return;

	m_uiFirstColor  = (*_vuipColors)[0];
	m_uiSecondColor = (*_vuipColors)[1];
	m_uiThirdColor  = (*_vuipColors)[2];
}

void CGeometrySystem::SetAllColorTimes(std::vector<float>* _vuipColorTimes)
{
	if(_vuipColorTimes->size() < 3 )
		return;

	m_fStartColorTime   = (*_vuipColorTimes)[0];
	m_fMiddleColorTime  = (*_vuipColorTimes)[1];
	m_fEndColorTime     = (*_vuipColorTimes)[2];


	CheckValidValue(m_fMiddleColorTime);
	CheckValidValue(m_fEndColorTime);

}

void CGeometrySystem::UpdateColor(tGeometry* _tCurrGeometry)
{
	float fSecondTime = (m_fMiddleColorTime / m_fEndColorTime) * m_fLifetime; 
	float fFirstTime  = ( m_fStartColorTime / m_fEndColorTime) * m_fLifetime;
	float fPercentLife = 0.0f;

	if(_tCurrGeometry->m_fLifetime < fSecondTime && _tCurrGeometry->m_fLifetime > fFirstTime)
	{			
		fPercentLife = (_tCurrGeometry->m_fLifetime / fSecondTime); 


		//Updating the current color of the particle using the same formula
		float fSA = (float)(m_uiFirstColor >> 24),  fSR = (float)( m_uiFirstColor << 8 >> 24),  fSG = (float)( m_uiFirstColor << 16 >> 24),  fSB = (float)( m_uiFirstColor << 24 >> 24);
		float fEA = (float)(m_uiSecondColor >> 24), fER = (float)( m_uiSecondColor << 8 >> 24), fEG = (float)( m_uiSecondColor << 16 >> 24), fEB = (float)( m_uiSecondColor << 24 >> 24);

		_tCurrGeometry->m_uiColor = 0;
		_tCurrGeometry->m_uiColor |= (unsigned int)(fSA + (fEA - fSA) * fPercentLife) << 24;
		_tCurrGeometry->m_uiColor |= (unsigned int)(fSR + (fER - fSR) * fPercentLife) << 16;
		_tCurrGeometry->m_uiColor |= (unsigned int)(fSG + (fEG - fSG) * fPercentLife) << 8;
		_tCurrGeometry->m_uiColor |= (unsigned int)(fSB + (fEB - fSB) * fPercentLife);

	}
	else
	{
		/*fPercentLife =  ((_tCurrGeometry->m_fLifetime / m_fLifetime) - (fSecondTime / m_fLifetime));
		fPercentLife *=  (1 / (fSecondTime / m_fLifetime)); */

		fPercentLife = (_tCurrGeometry->m_fLifetime - fSecondTime) / (m_fEndColorTime - fSecondTime);


		//Updating the current color of the particle using the same formula
		float fSA = (float)(m_uiSecondColor >> 24), fSR = (float)( m_uiSecondColor << 8 >> 24), fSG = (float)( m_uiSecondColor << 16 >> 24), fSB = (float)( m_uiSecondColor << 24 >> 24);
		float fEA = (float)(m_uiThirdColor >> 24),  fER = (float)( m_uiThirdColor << 8 >> 24),  fEG = (float)( m_uiThirdColor << 16 >> 24),  fEB = (float)( m_uiThirdColor << 24 >> 24);

		_tCurrGeometry->m_uiColor = 0;
		_tCurrGeometry->m_uiColor |= (unsigned int)(fSA + (fEA - fSA) * fPercentLife) << 24;
		_tCurrGeometry->m_uiColor |= (unsigned int)(fSR + (fER - fSR) * fPercentLife) << 16;
		_tCurrGeometry->m_uiColor |= (unsigned int)(fSG + (fEG - fSG) * fPercentLife) << 8;
		_tCurrGeometry->m_uiColor |= (unsigned int)(fSB + (fEB - fSB) * fPercentLife);

	}


}


void CGeometrySystem::SetAllVelocityValues(std::vector<D3DXVECTOR3>* _vd3dpVelocities)
{
	if(_vd3dpVelocities->size() < 3 )
		return;

	m_d3dFirstVelocity  = (*_vd3dpVelocities)[0];
	m_d3dSecondVelocity = (*_vd3dpVelocities)[1];
	m_d3dThirdVelocity  = (*_vd3dpVelocities)[2];
}

void CGeometrySystem::SetAllVelocityTimes(std::vector<float>* _vfpVelocityTimes)
{
	if(_vfpVelocityTimes->size() < 3 )
		return;

	m_fStartVelocityTime  = (*_vfpVelocityTimes)[0];
	m_fMiddleVelocityTime = (*_vfpVelocityTimes)[1];
	m_fEndVelocityTime    = (*_vfpVelocityTimes)[2];

	CheckValidValue(m_fMiddleVelocityTime);
	CheckValidValue(m_fEndVelocityTime);
}

void CGeometrySystem::ResetPosition( D3DXVECTOR3* _position )
{
	m_d3dPosition  = *_position;

	for( unsigned int uiIndex = 0; uiIndex < m_uiNumberOfGeometry; ++uiIndex)
	{
		m_vtGeometry[uiIndex].m_d3dPosition = *_position;
	}
}
void CGeometrySystem::ResetPosition( D3DXMATRIX _Matrix )
{
	D3DXMATRIX tmpMat;
	D3DXMatrixIdentity(&tmpMat);

	tmpMat._41 = m_d3dPositionOffset.x;
	tmpMat._42 = m_d3dPositionOffset.y;
	tmpMat._43 = m_d3dPositionOffset.z;

	D3DXMatrixMultiply(&_Matrix, &tmpMat, &_Matrix);

	m_d3dPosition  = D3DXVECTOR3(_Matrix._41, _Matrix._42, _Matrix._43);

	for( unsigned int uiIndex = 0; uiIndex < m_uiNumberOfGeometry; ++uiIndex)
	{
		m_vtGeometry[uiIndex].m_d3dPosition = m_d3dPosition;
	}
}


void CGeometrySystem::CheckValidValue(float& _fValue)
{
	if(_fValue > -EPSILON && _fValue < EPSILON)
	{
		_fValue = EPSILON;
	}
}
