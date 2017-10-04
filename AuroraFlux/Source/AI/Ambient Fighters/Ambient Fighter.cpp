#include "Ambient Fighter.h"
#include "../../Collision/Physics.h"
#include "../AmbientFlockingMovement.h" 
#include "../../Entity/Missile.h"
#include "../../Object Manager/ObjectManager.h"
#include "AmbientBoundingBox.h" 
#include "../../Camera/Camera.h"

CAmbientFighter::CAmbientFighter(CAmbientBoundingBox* _pContainingBox, D3DXMATRIX _d3dMatrix, int _flockID)
{
	SetDefaults(); 
	D3DXMatrixIdentity(&m_d3dMatrix);
	D3DXVECTOR3 _d3dObjPos = CPhysics::GetMat4Vec(ePOS, &_d3dMatrix);
	m_d3dMatrix._41 = _d3dObjPos.x;
	m_d3dMatrix._42 = _d3dObjPos.y;
	m_d3dMatrix._43 = _d3dObjPos.z;  
	m_pContainingBox = _pContainingBox;
	m_nFlockID = _flockID;
	PushBehavior(new CAmbientFlockingMovement((CAmbientFighter*)this));
	m_bIsInIntroduction = false;
	m_bHasAlreadyPassedPlayer = false;
	m_bHasAlreadyShakenCamera = false;
}  
CAmbientFighter::CAmbientFighter(D3DXMATRIX _d3dWorldPos, CCamera* _pCamera)
{
	SetDefaults();
	m_d3dMatrix = _d3dWorldPos;
	m_pCamera = _pCamera;
	m_bIsInIntroduction = true;
	m_nObjectType = eRED_ENEMY;
	m_nFlockID = -1;
	m_bHasAlreadyPassedPlayer = false;
	EffectInformation::CreateEffect(eRED_ENEMY_THRUSTER, &m_esEngineTrail, this,  true, false);
	CPhysics::LookAt(&m_d3dMatrix, _pCamera->GetCurrentMatrix());
}
bool CAmbientFighter::IntroUpdate(float fTime) 
{

	if(!m_esEngineTrail.GetPlay())
	{
		m_esEngineTrail.ResetPosition();
		m_esEngineTrail.SetPlay(true);
	}
	////
	//move the ambient fighter forward
	D3DXMATRIX d3dMat;
	D3DXMatrixIdentity(&d3dMat);
	D3DXMatrixTranslation(&d3dMat,	0.0f, 0.0f, 300.0f*fTime);
	m_d3dMatrix =  m_d3dMatrix * d3dMat;
	CPhysics::TurnTo(&m_d3dMatrix, &CPhysics::GetMat4Vec(ePOS, m_pCamera->GetCurrentMatrix()), fTime, 0.001f);
	D3DXVECTOR3 _d3dZAxis = CPhysics::GetMat4Vec(eZAXIS, m_pCamera->GetCurrentMatrix());
	D3DXVec3Normalize(&_d3dZAxis, &_d3dZAxis);
	if(m_bHasAlreadyPassedPlayer == false)
	{
		D3DXVECTOR3 _fghterPos = CPhysics::GetMat4Vec(ePOS, GetMatrix());
		D3DXVECTOR3 _camPos = CPhysics::GetMat4Vec(ePOS, m_pCamera->GetCurrentMatrix());
		D3DXVECTOR3 _lngthVec = _camPos - _fghterPos;
		float distToTar = D3DXVec3Length(&_lngthVec);

		if(distToTar < 1500 && m_bHasPlayedWarpIn == false)
		{
			EffectInformation::CreateEffect(eRED_ENEMY_ARRIVAL, &m_esArrival, this, false, true);
			m_bHasPlayedWarpIn = true;
		}

		if(distToTar < 70.0f && m_bHasAlreadyShakenCamera == false)
		{
			WwiseNS::SoundEvent(eEnemyThrust, ePlay, false, this);
			m_pCamera->Shake(30, 60, 0.4f); 
			m_bHasAlreadyShakenCamera = true;
		}

		if( distToTar > 140 && HalfSpaceTest(_d3dZAxis, CPhysics::GetMat4Vec(ePOS, m_pCamera->GetCurrentMatrix()), GetPosition()))
		{
			m_bHasAlreadyPassedPlayer = true;
			return true;
		}
	}
	return false;
}
bool CAmbientFighter::HalfSpaceTest(const D3DXVECTOR3& planeNorm, const D3DXVECTOR3& pointOnPlane, const D3DXVECTOR3& objPoint)
{
        D3DXVECTOR3 v = objPoint - pointOnPlane;
        float dot = D3DXVec3Dot(&planeNorm, &v);
        if(dot > 0)
                return false;
        else
                return true;
}
bool CAmbientFighter::Update(float fTime, bool _boss)
{
	if(m_bIsInIntroduction)
	{
		return IntroUpdate(fTime);
	}
	CEnemy::Update(fTime);
	if (m_pCurrMoveBehavior)
	{
		m_pCurrMoveBehavior->Update(fTime);
	}
	if(_boss)
	{
		return false;
	}
	if (bobmTimer >= 40+timerOffset && !m_bFirstExplosion && !m_bSecondExplosion && !m_bThirdExplosion)
	{
		m_bFirstExplosion = true;
	}
	else 
	{
		bobmTimer += fTime;
	}
	static float m_fExplosionTimer = 0.0f;
	if(m_bFirstExplosion)
	{ 
		if(m_fExplosionTimer > 2.0f)
		{
			CMissile* tempMissile = (CMissile*)(m_pContainingBox->GetObjectManager()->ReturnArmamentToMasterList(eMISSILE));
			if(tempMissile != NULL)
			{
				D3DXMATRIX	d3dMissileMat;
				D3DXMatrixTranslation(&d3dMissileMat, (float)(rand()%60-30), (float)(rand()%20-10), (float)(rand()%75-37));
				tempMissile->SetTarget(this); 
				tempMissile->SetMatrix(&( (*GetMatrix()) * d3dMissileMat ));
				tempMissile->SetOwner(this);
				tempMissile->SetActive(true); 
				tempMissile->SetLifetime(0.01f);
				tempMissile->SetDoOnce(true);
				tempMissile->SetHealth(1);
				tempMissile->SetVelocityModifier(0.0f);
				tempMissile->SetDamage(0);
				tempMissile->SetIsVisable(false);
				tempMissile->SetIsScenery(true);
				for (unsigned int i = 0; i < tempMissile->GetMovementBehaviorStack().size(); i++)
				{
					tempMissile->PopBehavior();
				}
			}
			m_fExplosionTimer = 0.0f;
			m_bFirstExplosion = false;
			m_bSecondExplosion = true;
		}
		else
		{
			m_fExplosionTimer += fTime;
		}
	}
	else if(m_bSecondExplosion)
	{
		if(m_fExplosionTimer > 2.0f)
		{
			CMissile* tempMissile = (CMissile*)(m_pContainingBox->GetObjectManager()->ReturnArmamentToMasterList(eMISSILE));
			if(tempMissile != NULL)
			{
				D3DXMATRIX	d3dMissileMat;
				D3DXMatrixTranslation(&d3dMissileMat, (float)(rand()%60-30), (float)(rand()%20-10), (float)(rand()%75-37));
				tempMissile->SetTarget(this); 
				tempMissile->SetMatrix(&( (*GetMatrix()) * d3dMissileMat ));
				tempMissile->SetOwner(this);
				tempMissile->SetActive(true); 
				tempMissile->SetLifetime(0.01f);
				tempMissile->SetDoOnce(true);
				tempMissile->SetHealth(1);
				tempMissile->SetVelocityModifier(0.0f);
				tempMissile->SetDamage(0);
				tempMissile->SetIsVisable(false);
				tempMissile->SetIsScenery(true);
				for (unsigned int i = 0; i < tempMissile->GetMovementBehaviorStack().size(); i++)
				{
					tempMissile->PopBehavior();
				}
			}
			m_fExplosionTimer = 0.0f;
			m_bSecondExplosion = false;
			m_bThirdExplosion = true;
		}
		else
		{
			m_fExplosionTimer += fTime;
		}
	}
	else if(m_bThirdExplosion)
	{
		if(m_fExplosionTimer > 2.0f)
		{
			CMissile* tempMissile = (CMissile*)(m_pContainingBox->GetObjectManager()->ReturnArmamentToMasterList(eMISSILE));
			if(tempMissile != NULL)
			{
				D3DXMATRIX	d3dMissileMat;
				D3DXMatrixTranslation(&d3dMissileMat, (float)(rand()%60-30), (float)(rand()%20-10), (float)(rand()%75-37));
				tempMissile->SetTarget(this); 
				tempMissile->SetMatrix(&( (*GetMatrix()) * d3dMissileMat ));
				tempMissile->SetOwner(this);
				tempMissile->SetActive(true); 
				tempMissile->SetLifetime(0.01f);
				tempMissile->SetDoOnce(true);
				tempMissile->SetHealth(1);
				tempMissile->SetVelocityModifier(0.0f);
				tempMissile->SetDamage(0);
				tempMissile->SetIsVisable(false);
				tempMissile->SetIsScenery(true);
				for (unsigned int i = 0; i < tempMissile->GetMovementBehaviorStack().size(); i++)
				{
					tempMissile->PopBehavior();
				}
			}
			m_fExplosionTimer = 0.0f;
			bobmTimer = 0.0f;
			m_bThirdExplosion = false;
		}
		else
		{
			m_fExplosionTimer += fTime;
		}
	}
	 

	//firing behavior
	if(_fAmbientFighterFiringBucket > 15.0f + _nRandomFiringOffset)
	{
		SetFlags(GetFlags() | eFIRE_LASER);
		_fAmbientFighterFiringBucket = 0.0f;
	} 
	else
	{
		_fAmbientFighterFiringBucket += fTime;
	}

	return false;
}

void CAmbientFighter::SetDefaults()
{
	m_nObjectType = eAMBIENT_ENEMY;
	m_nHealth = 1;
	m_pPlayer = nullptr;
	m_nShields = 0;
	m_nDamage = 0;
	m_nVelocityModifier = 400.0f;
	m_pCurrMoveBehavior = nullptr;
	m_pShootBehavior = nullptr;
	m_fStunPower = 0;
	m_pWaypoint = nullptr;
	SetBV(CCollOBJ::Create(eSPH, D3DXVECTOR3(0,0,0), 10.0));
	m_bFirstExplosion = false;
	m_bSecondExplosion = false;
	m_bThirdExplosion = false;
	bobmTimer = 0.0f;
	timerOffset = rand()%20;
	_nRandomFiringOffset = rand()%10;
	_fAmbientFighterFiringBucket = 0.0f;
	m_bHasAlreadyShakenCamera = false;
	m_bHasPlayedWarpIn = false;
}