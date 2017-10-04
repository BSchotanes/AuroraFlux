#include "AmbientFlockingMovement.h"
#include "../AI/Ambient Fighters/Ambient Fighter.h"
#include "../AI/Ambient Fighters/AmbientBoundingBox.h"
#include "../Collision/Physics.h"
#include "../Collision/CollisionLib.h" 
CAmbientFlockingMovement::CAmbientFlockingMovement(CAmbientFighter* _pEnemy, float _fSeperation, float _fCohesion, float _fAlignment): CMovementBehavior(_pEnemy)
{
	m_pOwner = NULL;
	m_pOwner = _pEnemy; 
	m_fSeperationWeight = _fSeperation;
	m_fAlignmentWeight = _fAlignment;
	m_fCohesionWeight = _fCohesion;
} 
void CAmbientFlockingMovement::Update(float fTime)
{
	if(OwnerTooFar())
	{
		D3DXVECTOR3 _d3dCenterPoint = D3DXVECTOR3(0,0,0);
		_d3dCenterPoint = CPhysics::GetMat4Vec(ePOS, &m_pOwner->GetBoundingBox()->GetWorldMatrix());
		_d3dCenterPoint.x += m_pOwner->GetBoundingBox()->GetBoundingWidth() * 0.5f;
		_d3dCenterPoint.y += m_pOwner->GetBoundingBox()->GetBoundingHeight() * 0.5f;
		_d3dCenterPoint.z += m_pOwner->GetBoundingBox()->GetBoundingDepth() * 0.5f;
		CPhysics::TurnTo(m_pOwner->GetMatrix(), &_d3dCenterPoint, fTime, 0.001f);
		D3DXMATRIX _d3dWorld = *m_pOwner->GetMatrix(); 
		D3DXVECTOR3 _forwardVec = CPhysics::GetMat4Vec(eZAXIS, &_d3dWorld);
		D3DXVec3Normalize(&_forwardVec, &_forwardVec);
		_forwardVec *= m_pOwner->GetVelocityModifier() * fTime;
		D3DXMATRIX tranMat;
		D3DXMatrixIdentity(&tranMat);
		D3DXMatrixTranslation(&tranMat, _forwardVec.x,_forwardVec.y , _forwardVec.z);
		_d3dWorld = _d3dWorld * tranMat;
		m_pOwner->SetMatrix(&_d3dWorld);
		return;
	}
	D3DXVECTOR3 direction(0,0,0);
	CalculateAverages();
	direction += Alignment();
	direction += Seperation();
	direction += Cohesion();
	D3DXVec3Normalize(&direction, &direction);
	MoveBoid(direction, fTime);
}
bool CAmbientFlockingMovement::OwnerTooFar()
{
	CAmbientBoundingBox* _pBB = m_pOwner->GetBoundingBox();
	D3DXVECTOR3 _d3dBoundPos = CPhysics::GetMat4Vec(ePOS, &_pBB->GetWorldMatrix());
	D3DXVECTOR3 _d3dOwnerPos = m_pOwner->GetPosition();

	if(_d3dOwnerPos.x < _d3dBoundPos.x)
	{
		return true;
	}
	else if(_d3dOwnerPos.x > _d3dBoundPos.x + _pBB->GetBoundingWidth())
	{
		return true;
	}
	else if(_d3dOwnerPos.y < _d3dBoundPos.y)
	{
		return true;
	}
	else if(_d3dOwnerPos.y > _d3dBoundPos.y + _pBB->GetBoundingHeight())
	{
		return true; 
	}
	else if(_d3dOwnerPos.z < _d3dBoundPos.z)
	{
		return true;
	}
	else if(_d3dOwnerPos.z > _d3dBoundPos.z + _pBB->GetBoundingDepth())
	{
		return true;
	}
	return false;
}
void CAmbientFlockingMovement::MoveBoid(D3DXVECTOR3 _d3dTurnDir, float fTime)
{
	_d3dTurnDir *= m_pOwner->GetVelocityModifier();
	D3DXVECTOR3 _d3dHeading = CPhysics::GetMat4Vec(ePOS, m_pOwner->GetMatrix());
	D3DXVec3Normalize(&_d3dHeading, &_d3dHeading);
	D3DXVECTOR3 _turnVec = D3DXVECTOR3(0,0,0);
	_turnVec = _d3dTurnDir + _d3dHeading;
	CPhysics::TurnTo(m_pOwner->GetMatrix(), &_turnVec, fTime, 0.01f);
	//TurnTo(*m_pOwner->GetMatrix(), _turnVec, fTime);
	D3DXMATRIX _d3dWorld = *m_pOwner->GetMatrix();
	D3DXVECTOR3 _forwardVec = CPhysics::GetMat4Vec(eZAXIS, &_d3dWorld);
	D3DXVec3Normalize(&_forwardVec, &_forwardVec);
	_forwardVec *= m_pOwner->GetVelocityModifier() * fTime;
	D3DXMATRIX tranMat;
	D3DXMatrixIdentity(&tranMat);
	D3DXMatrixTranslation(&tranMat, _forwardVec.x,_forwardVec.y , _forwardVec.z);
	_d3dWorld = _d3dWorld * tranMat;
	m_pOwner->SetMatrix(&_d3dWorld);
}
D3DXVECTOR3 CAmbientFlockingMovement::Cohesion()
{
	D3DXVECTOR3 direction(0,0,0);
	direction = m_dAveragePosition - m_pOwner->GetPosition();
	float directionMagnitude = D3DXVec3LengthSq(&direction);
	if(directionMagnitude)
		direction /= directionMagnitude;
	D3DXVECTOR3 velocity = D3DXVECTOR3(m_pOwner->GetMatrix()->_11, m_pOwner->GetMatrix()->_12, m_pOwner->GetMatrix()->_13);
	float velocityMagnitude = D3DXVec3Length(&velocity);
	if (velocityMagnitude < m_fFlockRadius)
	{
		direction *= velocityMagnitude / m_fFlockRadius;
	}
	return direction *m_fCohesionWeight; 
}
D3DXVECTOR3 CAmbientFlockingMovement::Seperation()
{ 
	D3DXVECTOR3 direction(0,0,0);
	for(unsigned int i(0); i < m_vFlock.size(); ++i)
	{
		if(m_pOwner == m_vFlock[i])
			continue;
		D3DXVECTOR3 toOwner = m_pOwner->GetPosition() - m_vFlock[i]->GetPosition();
		//float distBetween = sqrt((pow(toOwner.x, 2) + pow(toOwner.y, 2) + pow(toOwner.z, 2)));
		float distBetween = D3DXVec3Length(&toOwner);
		float _fObjRadius = 0;
		if(m_vFlock[i]->GetBV()->GetObjType() == eSPH)
		{
			_fObjRadius = ((CSphere*)m_vFlock[i]->GetBV())->GetRadius();
		}
		else if(m_vFlock[i]->GetBV()->GetObjType() == eCAP)
		{
			_fObjRadius = ((CCapsule*)m_vFlock[i]->GetBV())->GetRadius();
		}
		if(distBetween < _fObjRadius)
		{
			if(distBetween)
				toOwner /= distBetween;
			else
				toOwner = D3DXVECTOR3(rand()%100*0.01f, rand()%100*0.01f, rand()%100*0.01f);
			toOwner *= (_fObjRadius - distBetween) / _fObjRadius;
			direction += toOwner;
		}
	}
	float directionMagnitude = D3DXVec3Length(&direction);
	if(directionMagnitude > 1.0f)
		direction /= directionMagnitude;
	return direction * m_fSeperationWeight;
}
D3DXVECTOR3 CAmbientFlockingMovement::Alignment()
{
	D3DXVECTOR3 direction(0,0,0);
	direction.x = m_dAverageHeading.x / m_pOwner->GetVelocityModifier();
	direction.y = m_dAverageHeading.y / m_pOwner->GetVelocityModifier();
	direction.z = m_dAverageHeading.z / m_pOwner->GetVelocityModifier();
	float directionMagnitude = D3DXVec3Length(&direction);
	if(directionMagnitude > 1)
		direction /= directionMagnitude;
	return direction * m_fAlignmentWeight;
}
void CAmbientFlockingMovement::CalculateAverages()
{
	D3DXVECTOR3 tempPos(0,0,0);
	m_dAveragePosition = tempPos;
	AcquireFlock();
	unsigned int _nFlockSize = m_vFlock.size();
	if(_nFlockSize)
	{
		for(unsigned int i(0); i < _nFlockSize; ++i)
		{
			m_dAveragePosition += m_vFlock[i]->GetPosition();
			D3DXVECTOR3 velocity = D3DXVECTOR3(m_vFlock[i]->GetMatrix()->_11, m_vFlock[i]->GetMatrix()->_12, m_vFlock[i]->GetMatrix()->_13);
			m_dAverageHeading += velocity;
		}
		tempPos.x = m_dAveragePosition.x / _nFlockSize;
		tempPos.y = m_dAveragePosition.y / _nFlockSize;
		tempPos.z = m_dAveragePosition.z / _nFlockSize;
		m_dAveragePosition = tempPos;
		tempPos.x = m_dAverageHeading.x / _nFlockSize;
		tempPos.y = m_dAverageHeading.y / _nFlockSize;
		tempPos.z = m_dAverageHeading.z / _nFlockSize;
	}
	m_dAverageHeading = tempPos;
}
void CAmbientFlockingMovement::AcquireFlock()
{
	if(m_pOwner == NULL)
		return;
	if(m_vFlock.size() > 0)
		return;
	unsigned int nTotalSize = m_pOwner->GetBoundingBox()->GetFighters().size();
	for(unsigned int i(0); i < nTotalSize; ++i)
	{
		if(m_pOwner->GetBoundingBox()->GetFighters()[i]->GetFlockID() == m_pOwner->GetFlockID())
		{
			m_vFlock.push_back(m_pOwner->GetBoundingBox()->GetFighters()[i]);
		}
	} 
}
CAmbientFlockingMovement::~CAmbientFlockingMovement()
{
	m_vFlock.clear(); 
}

void CAmbientFlockingMovement::TurnTo(D3DXMATRIX& objMatrix, D3DXVECTOR3 targetPos, float deltaTime)
{
	D3DXVECTOR3 vec = CPhysics::GetMat4Vec(ePOS, &objMatrix);
	objMatrix._41 = 0.0f; 
	objMatrix._42 = 0.0f;
	objMatrix._43 = 0.0f;

	D3DXVECTOR3 vecToTarget = targetPos - vec;
	D3DXVec3Normalize(&vecToTarget, &vecToTarget);

	float xDot = D3DXVec3Dot(&CPhysics::GetMat4Vec(eXAXIS, &objMatrix),&vecToTarget);
	D3DXMATRIX rotMat;
	D3DXMatrixIdentity(&rotMat);
	D3DXMatrixRotationY(&rotMat, xDot*deltaTime);
	objMatrix *= rotMat;

	float yDot = D3DXVec3Dot(&CPhysics::GetMat4Vec(eYAXIS, &objMatrix),&vecToTarget);
	D3DXMatrixIdentity(&rotMat);
	D3DXMatrixRotationX(&rotMat, (yDot)*deltaTime);
	objMatrix *= rotMat;

	D3DXVECTOR3 zAxis =CPhysics::GetMat4Vec(eZAXIS, &objMatrix);

	D3DXVECTOR3 Worldy;
	Worldy = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVec3Normalize(&zAxis, &zAxis);

	D3DXVECTOR3 xAxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVec3Cross(&xAxis, &Worldy, &zAxis);
	D3DXVec3Normalize(&xAxis, &xAxis);
	D3DXVECTOR3 yAxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVec3Cross(&yAxis, &zAxis, &xAxis);
	D3DXVec3Normalize(&yAxis, &yAxis);
	objMatrix._11 = xAxis.x;
	objMatrix._12 = xAxis.y;
	objMatrix._13 = xAxis.z;
	objMatrix._21 = yAxis.x;
	objMatrix._22 = yAxis.y;
	objMatrix._23 = yAxis.z;
	objMatrix._31 = zAxis.x;
	objMatrix._32 = zAxis.y;
	objMatrix._33 = zAxis.z;
	objMatrix._41 = vec.x;
	objMatrix._42 = vec.y;
	objMatrix._43 = vec.z;
}