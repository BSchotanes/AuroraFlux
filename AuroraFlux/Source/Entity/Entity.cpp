/***********************************************
* Filename:  		Entity.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will be the base class all entities will inherit from.
************************************************/
#include "../StdAfx.h"
#include "Entity.h"
#include "../Entity/Shield.h"
/*****************************************************************
* CEntity():		Default Constructor will pass down to its respective chlid.
* Ins:			    None    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CEntity::CEntity(void)
	: m_bActive(false), m_pTarget(nullptr), m_nObjectType(-1), m_pBV(nullptr), m_cFlags(0), m_nMesh(-1), m_bIsHit(0), m_bShouldRun(false)
	,m_bFasterIsHit(false),m_bIsVisable(true), m_pEnemyShield(0), m_bShieldHit(false), m_chTransitionValue(-1)
{
	///////CHANGED////////////
	D3DXMatrixIdentity(&m_d3dMatrix); isScenery = false;
	m_bIsTransparent = false;
}
/*****************************************************************
* CEntity():		Overload Constructor Default Constructor will pass down to its 
*					respective chlid and set the entites target to the passed in one.
* Ins:			    _target      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CEntity::CEntity(CEntity* _target)
	: m_bActive(false), m_pTarget(_target), m_nObjectType(-1), m_pBV(0), m_cFlags(0), m_bIsHit(0), m_bShouldRun(false),
	m_bFasterIsHit(false),m_bIsVisable(true), m_pEnemyShield(0), m_bShieldHit(false), m_chTransitionValue(-1)
{
	isScenery = false;
}
/*Destructor*/
CEntity::~CEntity(void)
{
	if(m_pBV != nullptr)
	{
		delete m_pBV;
		m_pBV = nullptr; 
	}
}
/*****************************************************************
* Update():		Pass update to its children still.
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CEntity::Update(float _fElapsedTime)
{
	//SetPosition(GetPosition() + m_d3dVelocity * _fElapsedTime);
	
	static float s_fFasterIsHitTimer = 0.0f;
	if (m_bFasterIsHit == true)
	{
		m_bShouldRun = true;
		s_fFasterIsHitTimer += _fElapsedTime;
		if (s_fFasterIsHitTimer >= .02f)
		{
			m_bIsHit = false;
			m_bFasterIsHit = false;
			s_fFasterIsHitTimer = 0;
		}
	}

	static float s_fIsHitTimer = 0.0f;
	if (m_bIsHit == true)
	{
		m_bShouldRun = true;
		s_fIsHitTimer += _fElapsedTime;
		if (s_fIsHitTimer >= .22f)
		{
			m_bIsHit = false;
			s_fIsHitTimer = 0;
		}
	}
	static float s_fShieldIsHitTimer = 0.0f;
	if(m_bShieldHit == true)
	{
		s_fShieldIsHitTimer += _fElapsedTime;
		if (s_fShieldIsHitTimer >= 1.0f)
		{
			m_bShieldHit = false;
			s_fShieldIsHitTimer = 0;
		}
	}

}
/*****************************************************************
* Render():		Pass render to its children still.
* Ins:			    None    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CEntity::Render()  
{

}
D3DXVECTOR3 CEntity::GetPosition()
{
	D3DXVECTOR3 d3dTempPosition;
	d3dTempPosition.x = m_d3dMatrix._41;
	d3dTempPosition.y = m_d3dMatrix._42;
	d3dTempPosition.z = m_d3dMatrix._43;

	return d3dTempPosition;
}
void CEntity::SetPosition(D3DXVECTOR3 _d3dPosition)
{
	m_d3dMatrix._41 =_d3dPosition.x;
	m_d3dMatrix._42 = _d3dPosition.y;
	m_d3dMatrix._43 = _d3dPosition.z;
}
void CEntity::SetPosition(float _fX, float _fY, float _fZ)
{
	m_d3dMatrix._41 = _fX;
	m_d3dMatrix._42 = _fY;
	m_d3dMatrix._43 = _fZ;
}
/*****************************************************************
* Rotate():		Will create a rotation matrix based on the degrees passed in and what axis you want.
0 will not rotate that axis. 1 will rotate that axis.
* Ins:			    _degrees, _x, _y, _z    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CEntity::Rotate( float _degrees, int _x, int _y, int _z  )
{
	D3DXMATRIX d3dRotateXMatrix;
	D3DXMATRIX d3dRotateYMatrix;
	D3DXMATRIX d3dRotateZMatrix;
	D3DXMatrixIdentity(&d3dRotateXMatrix);
	D3DXMatrixIdentity(&d3dRotateYMatrix);
	D3DXMatrixIdentity(&d3dRotateZMatrix);

	if ( _x >= 1 )
	{  
		d3dRotateXMatrix._22 = cos(_degrees);
		d3dRotateXMatrix._32 = -sin(_degrees);
		d3dRotateXMatrix._23 = sin(_degrees);
		d3dRotateXMatrix._33 = cos(_degrees);

		m_d3dMatrix = d3dRotateXMatrix * m_d3dMatrix;
	}
	if ( _y >= 1 )
	{  
		d3dRotateYMatrix._11 = cos(_degrees);
		d3dRotateYMatrix._31 = sin(_degrees);
		d3dRotateYMatrix._13 = -sin(_degrees);
		d3dRotateYMatrix._33 = cos(_degrees);
		m_d3dMatrix = d3dRotateYMatrix * m_d3dMatrix;

	}
	if (  _z >= 1 )
	{  
		d3dRotateZMatrix._11 = cos(_degrees);
		d3dRotateZMatrix._21 = -sin(_degrees);
		d3dRotateZMatrix._12 = sin(_degrees);
		d3dRotateZMatrix._22 = cos(_degrees);
		m_d3dMatrix = d3dRotateZMatrix * m_d3dMatrix;
	}
}
/*****************************************************************
* Scale():			Will create a scale matrix based on the scale passed in 
and will scale every axis by that much.
* Ins:			    _scale    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CEntity::Scale( D3DXVECTOR3 _scale )
{
	D3DXMATRIX d3dScaleMatrix;
	D3DXMatrixIdentity(&d3dScaleMatrix);

	d3dScaleMatrix._11 = _scale.x;
	d3dScaleMatrix._22 = _scale.y;
	d3dScaleMatrix._33 = _scale.z;
	d3dScaleMatrix._44 = 1;


	m_d3dMatrix = d3dScaleMatrix * m_d3dMatrix;

}
/*****************************************************************
* Scale():			Will create a scale matrix based on the scale passed in 
and will scale each axis by that much.
* Ins:			    _x, _y, _z    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CEntity::Scale( float _x, float _y, float _z )
{
	D3DXMATRIX d3dScaleMatrix;
	D3DXMatrixIdentity(&d3dScaleMatrix);
	d3dScaleMatrix._11 *= _x;
	d3dScaleMatrix._22 *= _y;
	d3dScaleMatrix._33 *= _z;
	d3dScaleMatrix._44 = 1;

	m_d3dMatrix = d3dScaleMatrix * m_d3dMatrix;
}
