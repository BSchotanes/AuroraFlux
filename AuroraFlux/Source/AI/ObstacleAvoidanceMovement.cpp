#include "ObstacleAvoidanceMovement.h"
#include "../Entity/Enemy.h"
#include "../Object Manager/ObjectManager.h"
#include "../Collision/Physics.h"
CObstacleAvoidanceMovement::CObstacleAvoidanceMovement(CEnemy* _pEnemy) : CMovementBehavior(_pEnemy)
{

	avoidFlags.push_back(eRED_ENEMY); avoidFlags.push_back(eBLUE_ENEMY); avoidFlags.push_back(eYELLOW_ENEMY); avoidFlags.push_back(eRED_PLAYER); avoidFlags.push_back(eBLUE_PLAYER);
	avoidFlags.push_back(eASTEROID); avoidFlags.push_back(eLARGE_ASTEROID);
}
/*Destructor*/
CObstacleAvoidanceMovement::~CObstacleAvoidanceMovement(void)
{

}
void CObstacleAvoidanceMovement::Update(float _fElapedTime)
{ 
	D3DXVECTOR3 _d3dVelocity = D3DXVECTOR3(0,0,0);
	_d3dVelocity += m_pEnemy->GetObjectManager()->AdjustToAvoidCollision(m_pEnemy, avoidFlags);
	float _fVelLength = D3DXVec3Length(&_d3dVelocity);
	if( _fVelLength > 0.1f)
	{
		_d3dVelocity *= m_pEnemy->GetVelocityModifier(); 
		D3DXVECTOR3 _d3dHeading = CPhysics::GetMat4Vec(ePOS, m_pEnemy->GetMatrix());
		D3DXVECTOR3 _turnVec = D3DXVECTOR3(0,0,0);
		_turnVec = _d3dVelocity + _d3dHeading;
		CPhysics::TurnTo(m_pEnemy->GetMatrix(), &_turnVec, _fElapedTime, 0.01f);

		D3DXMATRIX _d3dWorld = *m_pEnemy->GetMatrix();
		D3DXVECTOR3 _forwardVec = CPhysics::GetMat4Vec(eZAXIS, &_d3dWorld);
		D3DXVec3Normalize(&_forwardVec, &_forwardVec);
		_forwardVec *= m_pEnemy->GetVelocityModifier() * _fElapedTime;
		D3DXMATRIX tranMat;
		D3DXMatrixIdentity(&tranMat);
		D3DXMatrixTranslation(&tranMat, _forwardVec.x,_forwardVec.y , _forwardVec.z);
		_d3dWorld = _d3dWorld * tranMat;
		m_pEnemy->SetMatrix(&_d3dWorld);
	}
	else
	{
		m_pEnemy->PopBehavior();
		return;
	}
}