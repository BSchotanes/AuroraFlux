#include "BossBigLazer.h"
#include "BossWinTurret.h"
#include "../Collision/Physics.h"
CBossBigLazer::CBossBigLazer() : CEnemy()
{  
	m_nObjectType = eBARRIER; 
	SetBV(CCollOBJ::Create(eCAP, D3DXVECTOR3(0,0,0.0f), D3DXVECTOR3(0,0,1000.0f), 7.0f));
	SetTarget(NULL);
	m_bIsVisable = false;
} 
void CBossBigLazer::Update(float fTime)
{
	if(m_pOwner != NULL) 
	{
		D3DXMATRIX _d3dWorld = *m_pOwner->GetMatrix();
		D3DXVECTOR3 _forwardVec = CPhysics::GetMat4Vec(eZAXIS, &_d3dWorld);
		D3DXVec3Normalize(&_forwardVec, &_forwardVec);
		_forwardVec *= 520.0f;
		D3DXMATRIX tranMat;
		D3DXMatrixIdentity(&tranMat);
		D3DXMatrixTranslation(&tranMat, _forwardVec.x,_forwardVec.y , _forwardVec.z);
		_d3dWorld = _d3dWorld * tranMat;
		m_d3dMatrix = _d3dWorld;
	}
}