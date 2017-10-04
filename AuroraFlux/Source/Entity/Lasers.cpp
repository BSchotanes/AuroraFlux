/***********************************************
* Filename:  		Laser.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will hold all the laser entities data.
************************************************/
#include "../StdAfx.h"
#include "Lasers.h"
#include "../AI/AIHelper.h"
#include "../Collision/Physics.h"

/*****************************************************************
* CLaser():		Default Constructor will set up his data members
* Ins:			    None    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CLaser::CLaser(void) : m_fLifetime(0)
{
	m_pOwner = 0;
	m_nObjectType       = eLASER;
	SetBV(CCollOBJ::Create(eCAP, D3DXVECTOR3(0,0,-2), D3DXVECTOR3(0,0,2), 1.0));
	m_pTarget = NULL;
	m_Effect.SetOwner(this);
	m_ESLaserSpark.SetOwner(this);
	m_MuzzleEffect.SetOwner(this);
	m_esAsteroidDust.SetOwner(this);
	Scale(1.0f, 1.0f, 6.0f);
	m_bShouldBounce = false;
}

/*****************************************************************
* CLaser():		Overloaded Constructor will set up his data members
* Ins:			    None    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CLaser::CLaser(CEntity* _pOwner, CAIHelper *_AIHelper) : m_fLifetime(0), s_AIHelper(_AIHelper)
{
	m_pOwner      = _pOwner;
	m_nObjectType = eLASER;
	SetBV(CCollOBJ::Create(eCAP, D3DXVECTOR3(0,0,-2), D3DXVECTOR3(0,0,2), 1.0));
	m_nTurnRate   =  s_AIHelper->GetlaserTurnRate();
	m_nVelocityModifier = 0;// s_AIHelper->GetlaserVelocityModifier();
	m_Effect.SetOwner(this);
	m_ESLaserSpark.SetOwner(this);
	m_MuzzleEffect.SetOwner(this);
	m_esAsteroidDust.SetOwner(this);
	Scale(1.0f, 1.0f, 6.0f);
	/*	SetIsVisable(false);*/
	m_bShouldBounce = false;
}

/*Destructor*/
CLaser::~CLaser(void)
{

}
/*****************************************************************
* Update():		Will update its respective data, shields, and health.
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CLaser::Update(float _fElapedTime)
{
	CEnemy::Update(_fElapedTime);
	m_fLifetime -= _fElapedTime*2;
	if (m_fLifetime <= 0)
	{
		SetActive(false);
		SetTarget(NULL);
	}
	if (m_bShouldBounce == false)
	{
	if (GetActive())
	{
		D3DXVECTOR3 d3dTranslate(GetMatrix()->_31, GetMatrix()->_32,GetMatrix()->_33);
		D3DXVec3Normalize(&d3dTranslate, &d3dTranslate);

		float laserVel =  s_AIHelper->GetlaserVelocityModifier();
		GetMatrix()->_41 += d3dTranslate.x * 1000.0f * _fElapedTime;
		GetMatrix()->_42 += d3dTranslate.y * 1000.0f * _fElapedTime;
		GetMatrix()->_43 += d3dTranslate.z * 1000.0f * _fElapedTime;

	}

	if( m_pTarget != NULL && D3DXVec3Length( &(GetPosition() - m_pTarget->GetPosition() )) <= 22)
	{
		m_pTarget = 0;
	}

	if( m_pOwner->GetType() == eRED_ENEMY && m_pTarget != NULL && m_pTarget->GetActive() )
	{
		CPhysics::TurnTo(this, m_pTarget, _fElapedTime,.047f);
	}
	

		if(m_pOwner->GetType() == eRED_PLAYER  && m_pTarget != NULL && m_pTarget->GetActive() == true)
		{

			if(  D3DXVec3Length( &(GetPosition() - m_pTarget->GetPosition() )) >= 250.0f)
			{
				m_nTurnRate = 0.001f;
			}
			else
			{
				m_nTurnRate = s_AIHelper->GetlaserTurnRate();
			}
			//////////////////////////////////////////////////////////////////////////
			CPhysics::TurnTo(this, m_pTarget, _fElapedTime, 0.25f);
			//////////////////////////////////////////////////////////////////////////
		}

		if((m_pOwner->GetType() == eRED_ENEMY || m_pOwner->GetType() == eYELLOW_ENEMY) && m_pTarget != NULL && m_pTarget->GetActive() == true)
		{
			D3DXVECTOR3 d3dZVector = GetPosition() - m_pTarget->GetPosition();
			D3DXVec3Normalize(&d3dZVector, &d3dZVector);

			float fZDotResult = D3DXVec3Dot(&d3dZVector, &CPhysics::GetMat4Vec(eZAXIS, m_pTarget->GetMatrix()));

			if(fZDotResult <= -0.1f)
			{
				float fDistance = D3DXVec3Length( &(GetPosition() - m_pTarget->GetPosition() ));

				if(fDistance < 50)
				{
					SetTarget(NULL);	
				} 

			}
		}


	}
	else
	{
		if (m_pCurrMoveBehavior)
		{
			m_pCurrMoveBehavior->Update(_fElapedTime);
		}
	}


}

/*****************************************************************
* Render():		Draw Updates
* Ins:			    None    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CLaser::Render() 
{

}

void CLaser::CreateEffect(Effects eType)
{
	EffectInformation::CreateEffect(eType, &m_Effect, this, false, true);
}

void CLaser::MuzzleEffect()
{

	EffectInformation::CreateEffect(eRED_ENEMY_FIRE_LASER, &m_MuzzleEffect, this, false, true);
}

void CLaser::AsteroidDust()
{
	EffectInformation::CreateEffect(eASTEROID_COLLISION_LASER, &m_esAsteroidDust, this, false, true);
}

void CLaser::SparkEffect()
{
	//	EffectInformation::CreateEffect(eLASER_SPARK, &m_ESLaserSpark, this, false, true);

	//D3DXVECTOR3 d3dOrigin = this->GetOwner()->GetPosition(), d3dDest = this->GetPosition(), d3dDir;
	//float fMagnitude = D3DXVec3Length(&(d3dDest - d3dOrigin));
	//d3dDir = (d3dDest - d3dOrigin) / fMagnitude;
	//
	// 	//D3DXMATRIX tmp1, tmp2;
	// 	//D3DXMatrixIdentity(&tmp1);
	// 	//D3DXMatrixTranslation(&tmp2, d3dDest.x, d3dDest.y, d3dDest.z);
	//
	////CPhysics::LookAt(&tmp1, &tmp2);
	//vector<D3DXVECTOR3> tmpVecs, tmpVecs2;
	//tmpVecs.reserve(3);	tmpVecs2.reserve(3);
	//for(int j = 0; j<3; j++)
	//{
	//	tmpVecs.push_back(d3dDir * min(2000, fMagnitude*1)/* D3DXVECTOR3(0.15f,0.15f,15)*/);
	//	//tmpVecs2.push_back(CPhysics::GetMat4Vec(eZAXIS, GetOwner()->GetMatrix()) * D3DXToRadian(260) );
	//}
	//
	//for(unsigned i=0; i<m_ESLaserSpark.GetParticleSystem()->size(); i++)
	//{
	//	(*m_ESLaserSpark.GetParticleSystem())[i].ResetPosition(&d3dOrigin);
	//	(*m_ESLaserSpark.GetParticleSystem())[i].SetAllVelocityValues(&tmpVecs);
	//	//(*m_ESLaserSpark.GetParticleSystem())[i].SetAllRotationValues(&tmpVecs2);
	//	//(*m_ESLaserSpark.GetParticleSystem())[i].SetAllScaleValues(&tmpVecs);
	//	/*(*m_ESLaserSpark.GetGeometrySystem())[i].SetAllScaleValues(&tmpVecs2);*/
	//}


}