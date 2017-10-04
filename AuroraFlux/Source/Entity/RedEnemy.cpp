/***********************************************
* Filename:  		RedEnemy.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This is what will contain the 
*					Red enemys personal data.				
************************************************/
#include "../StdAfx.h"
#include "../AI/RedShooting.h"
#include "RedEnemy.h"
#include "../AI/ThinkingMovement.h"
#include "../AI/MovementBehavior.h"
#include "BlueEnemy.h"
#include "Missile.h"
#include "../AI/RedShooting.h"
#include "../AI/AIHelper.h"
#include "../Object Manager/ObjectManager.h"
#include "../Collision/Physics.h"
/*****************************************************************
* CRedEnemy():  Default Constructor. Will set its type and behaviors.
* Ins:			    None    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CRedEnemy::CRedEnemy(void) : m_fDodgeTimer(0), m_bCanDodge(false), m_nNumpartners(0), m_nFlockID(0), m_nFlockCounter(0)
{
	m_fRollTimer = 0.0f;
	/*m_nObjectType		= eRED_ENEMY;
	m_nHealth			= ENEMY_HEALTH;
	m_nShields			= RED_ENEMY_SHIELD;
	m_nDamage			= ENEMY_LASER_DAMAGE;
	m_nVelocityModifier = RED_VELOCITY_MODIFIER;
	m_nTurnRate			= RED_TURN_RATE;*/
	isRolling = false;
	m_pTarget			= nullptr;
	m_bActive			= true;
	SetIsHit(false);
	m_bIsStunned = (false);
	m_cFlags |= eAVOIDANCE;

	m_pPartners.clear();

	SetBV(CCollOBJ::Create(eSPH, D3DXVECTOR3(0,0,0), 10.0));

	switchShootState(new CRedShooting(this));

	m_d3dVelocity = D3DXVECTOR3(0,0,0);
	EffectInformation::CreateEffect(eRED_ENEMY_THRUSTER, &m_esEngineTrail, this,  true, false);
	EffectInformation::CreateEffect(eBLUE_ENEMY_DAMAGED, &m_esDamage, this, true, false);
	
	SetEntityMass(RED_ENEMY_MASS);

}
/*****************************************************************
* CRedEnemy():  Overload Constructor. Will set its type, target, and behaviors. 
*					Sets the target parameter to its target member.
* Ins:			    _target    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CRedEnemy::CRedEnemy(CEntity* _pTarget,CAIHelper *_AIHelper, CWaypoint* _pCurrWaypoint, CObjectManager* _pObjManager) : m_fDodgeTimer(0), m_bCanDodge(false), m_nNumpartners(0), m_nFlockID(0), m_nFlockCounter(0)
{
	s_AIHelper = _AIHelper;
	m_nObjectType			= eRED_ENEMY;
	m_nHealth			= _AIHelper->GetHealth();
	m_nShields			= _AIHelper->GetredShield();
	m_nDamage			= _AIHelper->GetlaserDamage();
	m_nVelocityModifier		= _AIHelper->GetredVelocityModifier();
	m_nTurnRate			= _AIHelper->GetredTurnRate();
	m_pTarget			= _pTarget;
	m_bActive			= true;
	m_pPlayer			= _pTarget;
	m_pWaypoint = _pCurrWaypoint;
	m_pPartners.clear();
	SetIsHit(false);
	m_bIsStunned = (false);
	m_cFlags |= eAVOIDANCE;
	s_AIHelper = _AIHelper;
	m_bShouldDodge = false;
	SetBV(CCollOBJ::Create(eSPH, D3DXVECTOR3(0,0,0), 6.0));
	isRolling = false;
	switchShootState(new CRedShooting(this));

	EffectInformation::CreateEffect(eRED_ENEMY_THRUSTER, &m_esEngineTrail, this, true, false);
	EffectInformation::CreateEffect(eRED_ENEMY_DAMAGED, &m_esDamage, this, true, false);

	m_d3dVelocity = D3DXVECTOR3(0,0,0);
	SetEntityMass(RED_ENEMY_MASS);

	m_pObjectManager = _pObjManager;

}
/*****************************************************************
* ~CRedEnemy(): Destructor, probably doing nothing
* Ins:			    None    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CRedEnemy::~CRedEnemy(void)
{

}
void CRedEnemy::UpdateHelperAI(float _elapsedTime)
{
	this->m_nVelocityModifier = s_AIHelper->GetredVelocityModifier();
	this->m_nTurnRate = s_AIHelper->GetredTurnRate();


}

/*****************************************************************
* Update():		Will update the enemys internal timers and update the behaviors 
*					based on the elapsed time.
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CRedEnemy::Update(float _fElapedTime)
{

	CEnemy::Update(_fElapedTime);

	//TODO: REMOVE THIS WHEN AI IS FINISHED
	int nNumActivePartners = 0;
	//count the active partners left
	for (unsigned int nPartnersIndex = 0; nPartnersIndex < m_pPartners.size(); nPartnersIndex++ )
	{
		if (m_pPartners[nPartnersIndex] && m_pPartners[nPartnersIndex]->GetActive() == true)
		{
			nNumActivePartners++;
		}
	}
	//fix the partner list if someone joined or left
	if(	m_nNumpartners != nNumActivePartners )
	{
		vector<CEntity*> vPartners;
		for (unsigned int uiPartner = 0; uiPartner < m_pPartners.size(); uiPartner++ )
		{
			if (m_pPartners[uiPartner] && m_pPartners[uiPartner]->GetActive() == true)
			{
				vPartners.push_back(m_pPartners[uiPartner]);
			}
		}
		m_pPartners = vPartners;
		m_nNumpartners = m_pPartners.size();
	}
	//if a buddy is hit get that mother fu*#er that hit him
// 	for (unsigned int nPartnersIndex = 0; nPartnersIndex < m_pPartners.size(); nPartnersIndex++)
// 	{
// 		if (m_pPartners[nPartnersIndex]->GetIsHit())
// 		{
// 			//m_pCurrMoveBehavior->SetForceChasebool(true);
// 		}
// 	}
	m_fRollTimer += _fElapedTime;
	if (m_fRollTimer >= 2.5f)
	{
		m_bCanRoll = true;
		m_fRollTimer = 0.0f;
	}
//   	if(m_bCanRoll)
//   	{
//  		std::vector<CEntity *> _vMasterList = *m_pObjectManager->GetActiveEntityList();
//  		unsigned int masterListSize = _vMasterList.size();
//  		for(unsigned int i = 0; i < masterListSize; ++i)
//  		{
//  			CEntity * pMasterList = _vMasterList[i];
//  			int _type = pMasterList->GetType();
//  			if (m_nObjectType == eRED_ENEMY && pMasterList->GetType() == eMISSILE &&
//  				(((CMissile*)pMasterList)->GetOwner()->GetType() == eRED_PLAYER || ((CMissile*)pMasterList)->GetOwner()->GetType() == eBLUE_PLAYER   ) )
//  			{
//  				
//  					D3DXVECTOR3 _d3dVecToObj = pMasterList->GetPosition() - GetPosition();
//  					float len = D3DXVec3Length(&_d3dVecToObj);
//  					if (len <= 50)
//  					{
//  						m_bShouldDodge = true;
//  						pMasterList->SetTarget(nullptr);
//  						((CMissile*)pMasterList)->SetLifetime(0.86f);
//  						m_bCanRoll = false;
//  					}
//  				
//  			}
//  
//  		}
 		
 //	}

 	

	if (m_pCurrMoveBehavior)
	{
		m_pCurrMoveBehavior->Update(_fElapedTime);
	}
	if (m_pShootBehavior)
	{
		m_pShootBehavior->Update(_fElapedTime);
	}

// 	if(abs(D3DXVec3Dot(&m_d3dVelocity, &m_d3dVelocity)) > EPSILON * 4)
// 	{
// 		D3DXMATRIX d3dTransMat;
// 
// 		D3DXVECTOR3 d3dLocalZ; D3DXVec3Normalize(&d3dLocalZ, &CPhysics::GetMat4Vec(eZAXIS, GetMatrix()));
// 		D3DXVECTOR3 _d3dLocalY; D3DXVec3Normalize(&_d3dLocalY, &CPhysics::GetMat4Vec(eYAXIS, GetMatrix()));
// 		D3DXVECTOR3 d3dLocalX; D3DXVec3Normalize(&d3dLocalX, &CPhysics::GetMat4Vec(eXAXIS, GetMatrix()));
// 
// 
// 		float fTmpLen = abs(D3DXVec3Length(&m_d3dVelocity));
// 
// 		if(m_d3dVelocity.x > EPSILON*2)
// 		{
// 			m_d3dVelocity.x = max(0, m_d3dVelocity.x - fTmpLen * _fElapedTime);
// 		}
// 		else if(m_d3dVelocity.x < -EPSILON*2)
// 		{
// 			m_d3dVelocity.x = min(0, m_d3dVelocity.x + fTmpLen * _fElapedTime);
// 		}
// 
// 		if(m_d3dVelocity.y > EPSILON*2)
// 		{
// 			m_d3dVelocity.y = max(0, m_d3dVelocity.y - fTmpLen * _fElapedTime);
// 		}
// 		else if(m_d3dVelocity.y < -EPSILON*2)
// 		{
// 			m_d3dVelocity.y = min(0, m_d3dVelocity.y + fTmpLen * _fElapedTime);
// 		}
// 
// 		if(m_d3dVelocity.z > EPSILON*2)
// 		{
// 			m_d3dVelocity.z = max(0, m_d3dVelocity.z - fTmpLen * _fElapedTime);
// 		}
// 		else if(m_d3dVelocity.z < -EPSILON*2)
// 		{
// 			m_d3dVelocity.z = min(0, m_d3dVelocity.z + fTmpLen * _fElapedTime);
// 		}
// 
// 		D3DXMatrixTranslation(&d3dTransMat, m_d3dVelocity.x * _fElapedTime, m_d3dVelocity.y * _fElapedTime, m_d3dVelocity.z * _fElapedTime);
// 
// 		 SetMatrix(&(*GetMatrix() * d3dTransMat));
// 
// 	}



	if(!m_esEngineTrail.GetPlay())
	{
		m_esEngineTrail.ResetPosition();
		m_esEngineTrail.SetPlay(true);
	}


	float currHealthPercentage = (float)m_nHealth / (float)s_AIHelper->GetHealth();


	if(currHealthPercentage < .95f )
	{
		m_esDamage.SetEmitterRate(currHealthPercentage);
		m_esDamage.SetPlay(true);

// 		if(currHealthPercentage < .50f)
// 		{
// 			m_esDamage.SetEmitterRate(.3f);
// 
// 			if(currHealthPercentage < .25f)
// 			{
// 				m_esDamage.SetEmitterRate(.1f);
// 			}
// 
// 		}
	}

	//if(GetAsyncKeyState(VK_NUMPAD8))
	//{
	//	cout << GetVelocityModifier() << " " << m_d3dVelocity.x << " " << m_d3dVelocity.y << " " << m_d3dVelocity.z << endl;
	//}

}
/*****************************************************************
* Render():		Draws the enemies.				
* Ins:			    None    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CRedEnemy::Render() 
{

}
/*****************************************************************
* AddPartner():		Adds a enemy to influence his current flock			
* Ins:			    _add    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CRedEnemy::AddPartner(CEntity* _pAdd)
{
	if (_pAdd != nullptr)
	{
		m_pPartners.push_back(_pAdd);
	}

}
/*****************************************************************
* RemoveAllPartners():		removes all partners from his current flock			
* Ins:			    None    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void  CRedEnemy::RemoveAllPartners()
{
	m_pPartners.clear();
}
/*****************************************************************
* isPartner():		returns true if the paassed in entity is already in his partner list			
* Ins:			    _red    		      
* Outs:		    None
* Returns:		    bool
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
bool CRedEnemy::isPartner(CRedEnemy * _pRed)
{
	for (unsigned int nNumPartners = 0; nNumPartners < m_pPartners.size(); nNumPartners++)
	{
		if(m_pPartners[nNumPartners] == _pRed)
		{
			return true;
		}
	}
	return false;
}

void CRedEnemy::CreateArrivalEffect()
{
	EffectInformation::CreateEffect(eRED_ENEMY_ARRIVAL, &m_esArrival, this, false, true);
}
 
void CRedEnemy::CreateDeathEffect()
{
	EffectInformation::CreateEffect(eRED_ENEMY_DEATH, &m_esArrival, this, false, true);
}