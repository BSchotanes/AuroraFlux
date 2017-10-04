#include "AmbientBoundingBox.h"
#include <vector>
#include <d3dx9math.h>
#include "../../Collision/Physics.h"
#include "../../Object Manager/ObjectManager.h"
#include "../../Camera/Camera.h"
CAmbientBoundingBox::CAmbientBoundingBox(D3DXMATRIX _worldMat, int nFighterCount, float _fHeight, float _fDepth, float _fWidth, CObjectManager* _pObjManager)
{
	m_pObjectManager = _pObjManager;
	m_nCounter = 0;
	m_WorldMatrix = _worldMat;  
	m_fBoundingHeight = _fHeight;   
	m_fBoundingDepth = _fDepth;
	m_fBoundingWidth = _fWidth;
	D3DXVECTOR3 _d3dRandPos = D3DXVECTOR3(0,0,0);
	D3DXVECTOR3 _d3dTopLeftPos = CPhysics::GetMat4Vec(ePOS, &m_WorldMatrix);
	D3DXMATRIX _d3dObjMat = D3DXMATRIX();
	int _nFlockID = -1; 
	m_bIsIntroFighters = false;
	for(int i(0); i < nFighterCount; ++i)
	{
		_d3dObjMat = m_WorldMatrix; 
		_d3dRandPos = _d3dTopLeftPos;
		_d3dRandPos.x += rand()%(int)m_fBoundingWidth;
		_d3dRandPos.y += rand()%(int)m_fBoundingHeight;
		_d3dRandPos.z += rand()%(int)m_fBoundingDepth;
		_d3dObjMat._41 = _d3dRandPos.x;
		_d3dObjMat._42 = _d3dRandPos.y;
		_d3dObjMat._43 = _d3dRandPos.z;
		_nFlockID = 0;
		CAmbientFighter* pFighter = new CAmbientFighter(this, _d3dObjMat, 0);
		m_vContainedFighters.push_back(pFighter);
		
	}
}
CAmbientBoundingBox::CAmbientBoundingBox( CCamera* _pCamera)
{
	m_pCamera = _pCamera;
	m_bIsIntroFighters = true;
	m_nIntroCounter = 0;
}
bool CAmbientBoundingBox::IntroIsDone()
{
	return (unsigned int)m_nIntroCounter >= m_vContainedFighters.size();
}
void CAmbientBoundingBox::Update(float fTime, bool _boss)
{ 
	if(m_bIsIntroFighters)
	{
		for(unsigned int i(0); i < m_vContainedFighters.size(); ++i)
		{
			if(m_vContainedFighters[i]->IntroUpdate(fTime))
			{
				++m_nIntroCounter;
			}
		}
		return;
	}
	m_nCounter++;
	if(m_nCounter > 0)
	{
		for(unsigned int i(0); i < m_vContainedFighters.size(); ++i)
		{
			m_vContainedFighters[i]->Update(fTime, _boss);
			m_pObjectManager->SpawnEntity(m_vContainedFighters[i], fTime);
		}
		m_nCounter = 0;
	}
};

CAmbientBoundingBox::~CAmbientBoundingBox()
{
	for(unsigned int i(0); i < m_vContainedFighters.size(); ++i)
	{
		delete m_vContainedFighters[i];
		m_vContainedFighters[i] = NULL;
	}
	m_vContainedFighters.clear();
}