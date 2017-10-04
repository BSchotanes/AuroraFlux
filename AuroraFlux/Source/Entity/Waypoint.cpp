#include "../StdAfx.h"
#include "Waypoint.h"


/*****************************************************************
* CBluePlayer():		Default Constructor will set up his type and timers
* Ins:			    None    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
CWaypoint::CWaypoint() : m_nID(0), m_bTrigger(false), fyDiff(0.0f), fScaleTimer(0.0f), fRotateChange(0.0f), m_bScaleDown(false), m_bShouldScale(false)
{

	SetBV(CCollOBJ::Create(eSPH, D3DXVECTOR3(0,0,0), WAYPOINT_RADIUS));
	m_nObjectType = eWAYPOINT;
	SetIsHit(false);
	
	SetActive(true);

	m_bHasAlreadyTriggered = false;

	m_bShouldTrigger = false;

	m_bParticlesExploded = false;
	m_chTransitionValue = 1.0f;
}

/*****************************************************************
* ~CWaypoint():	Will set all variables to a null value upon leaving the class
* Ins:			    none    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
CWaypoint::~CWaypoint(void)
{
}

void CWaypoint::ResetTriggerParticle()
{
	/*
	m_esTriggeredParticles.Init();
	vector<char32_t> vec;
	for( int i(0); i<3; ++i )
	{
		vec.push_back(D3DCOLOR_ARGB(85*i, 0, 0, 255));
	}

	m_esTriggeredParticles.SetPlay(false);
	for( unsigned int i(0); i < m_esTriggeredParticles.GetParticleSystem()->size(); ++i )
	{
		for( unsigned int j(0); j < m_esTriggeredParticles.GetParticleSystem()[i].size(); ++j )
		{
			m_esTriggeredParticles.GetParticleSystem()[i][j].SetAllColorValues(&vec);
			m_esTriggeredParticles.GetParticleSystem()[i][j].SetNumberOfParticles(251);
		}
	}
	m_bHasAlreadyTriggered = false;

	m_bShouldTrigger = false;

	m_bParticlesExploded = false;

	fExplosionTimer = 0.0f;
	*/
}

/*****************************************************************
* Update():		Will pass down the update calls to its children.
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		12/01/2012
* Mod. Initials:	AG
*****************************************************************/
void CWaypoint::Update(float _fElapedTime)
{

	Rotate(_fElapedTime, 1,0,1);

	static bool doOnce = false;
	m_bIsTransparent = true;

 	if(m_bShouldScale == true)
 	{

 		if(m_bScaleDown == true)
 		{
 			float s_fWaypointDownScale = 0.01f;
 			s_fWaypointDownScale = 1 - (2.0f * _fElapedTime);

			m_chTransitionValue -= .035f;
			if (m_chTransitionValue <= 0.0f)
			{
				m_chTransitionValue = 0.0f;
			}

 			//Scale(s_fWaypointDownScale,s_fWaypointDownScale,s_fWaypointDownScale);
 
 			if(this->m_d3dMatrix._11 < 0.01f)
 			{
 				m_bShouldScale = false;
 				m_bIsVisable = false;
 			}
 		}
 		else
 		{
 			
 			if(doOnce == false)
 			{
 				D3DXVECTOR3 d3dPrevPosition = GetPosition();
 				D3DXMatrixIdentity(GetMatrix());
 				D3DXMatrixTranslation(&m_d3dMatrix, d3dPrevPosition.x, d3dPrevPosition.y, d3dPrevPosition.z );
 				doOnce = true;
				m_chTransitionValue = 1.0f;
 				m_bIsVisable = true;
 			}
 
 			float s_fWaypointUpScale = 1.1f;
 			s_fWaypointUpScale = 1.01f * (2.0f - _fElapedTime);
 			if(s_fWaypointUpScale <= 1.0f)
 			{
 				s_fWaypointUpScale = 1.01f;
 			}
 
 			Scale(s_fWaypointUpScale,s_fWaypointUpScale,s_fWaypointUpScale);
 			
 			if(this->m_d3dMatrix._11 > 12.0f )
 			{
 				m_bShouldScale = false;
 				doOnce = false;
 			}
 		}
 	}
	//m_esTriggeredParticles.SetPlay(m_bIsVisable);

// 	if( m_bShouldTrigger )
// 	{
// 		fExplosionTimer += _fElapedTime;
// 		if( !m_bHasAlreadyTriggered )
// 		{
// 		//	m_esTriggeredParticles.Init();
// 		//	vector<char32_t> vec;
// // 			for( int i(0); i<3; ++i )
// // 			{
// // 				vec.push_back(D3DCOLOR_ARGB(85*i, 255, 0, 0));
// // 			}
// 
// 		//	for( unsigned int i(0); i < m_esTriggeredParticles.GetParticleSystem()->size(); ++i )
// 		//	{
// 		//		for( unsigned int j(0); j < m_esTriggeredParticles.GetParticleSystem()[i].size(); ++j )
// 		//		{
// 		//			m_esTriggeredParticles.GetParticleSystem()[i][j].SetAllColorValues(&vec);
// 		//			m_esTriggeredParticles.GetParticleSystem()[i][j].SetNumberOfParticles(251);
// 		//		}
// 		//	}
// 			m_bHasAlreadyTriggered = true;
// 		}
// // 		if( !m_bParticlesExploded && fExplosionTimer > 0.01f )
// // 		{
// // 			m_esTriggeredParticles.Init();
// // 			for( unsigned int i(0); i < m_esTriggeredParticles.GetParticleSystem()->size(); ++i )
// // 			{
// // 				for( unsigned int j(0); j < m_esTriggeredParticles.GetParticleSystem()[i].size(); ++j )
// // 				{
// // 					m_esTriggeredParticles.GetParticleSystem()[i][j].SetNumberOfParticles(249);
// // 				}
// // 			}
// // 			m_bParticlesExploded = true;
// // 		}
// 	}
}

void CWaypoint::SetTrigger(bool _nVal)
{
	 m_bTrigger = _nVal;
}

/*****************************************************************
* Render():		Will pass down the draw calls to its children.
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		12/01/2012
* Mod. Initials:	AG
*****************************************************************/
void CWaypoint::Render()
{

}

void CWaypoint::SetIsVisible(bool _bIsVisible)
{
	m_bIsVisable = _bIsVisible;
	//m_esTriggeredParticles.SetPlay(_bIsVisible);	
}
