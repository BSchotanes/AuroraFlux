/***********************************************
* Filename:  		Camera.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/30/2012
* Mod. Initials:	SW
* Author:    		Bryan Schotanes
* Purpose:   		This is the camera for everything that 
*					needs to be rendered
************************************************/


#include "Camera.h"
#include "../Input/Input.h"
#include "../Collision/Physics.h"
#include "../Definitions.h"

#include "../Collision/CollisionLib.h"

CCamera::~CCamera()
{

}

/*****************************************************************
* Init(float screenWidth, float screenHeight):	Initializes the camera class. Takes in 
*							screen width and screen height.
*
* Ins:			float screenWidth	- Width of the window
*				float screenHeight	- Height of the window
*
* Outs:		     N/A
*
* Returns:		N/A
*
* Mod. Date:		      11/30/2012
* Mod. Initials:	      JW
*****************************************************************/
void CCamera::Init( float _fScreenWidth, float _fScreenHeight, HWND _hWnd ) 
{
	m_bInMainMenu = false;

	m_d3dvWorldXAxis = D3DXVECTOR3(1.0f,0.0f,0.0f);
	m_d3dvWorldYAxis = D3DXVECTOR3(0.0f,1.0f,0.0f);
	m_d3dvWorldZAxis = D3DXVECTOR3(0.0f,0.0f,1.0f);

	D3DXMatrixIdentity(&m_d3dCameraViewMatrix);
	D3DXMatrixIdentity(&m_d3dCheckPointCamera);

	D3DXVECTOR3 Eye = D3DXVECTOR3(0,0,0);
	D3DXVECTOR3	At = D3DXVECTOR3(0,0,1);
	D3DXVECTOR3 Up = D3DXVECTOR3(0,1,0);

	D3DXMatrixLookAtLH(&m_d3dCameraViewMatrix,&Eye,&At,&Up);

	m_fHeight = _fScreenHeight;
	m_fWidth = _fScreenWidth;

	m_fFieldOfView = FIELD_OF_VIEW;	
	m_fAspectRatio = m_fWidth / m_fHeight;

	m_fNear = CAM_NEAR;
	m_fFar  = CAM_FAR;

	m_hWnd = _hWnd;

	D3DXMatrixPerspectiveFovLH(&m_d3dProjectionMatrix, m_fFieldOfView ,m_fAspectRatio,m_fNear,m_fFar);
	m_bMoving = false;

	m_fYawSpeed   = CAMERA_YAW_SPEED;
	m_fPitchSpeed = CAMERA_PITCH_SPEED;
	m_fRollSpeed  = CAMERA_ROLL_SPEED;

	CCollisionLib::BuildFrustum(m_tFrustum, m_fFieldOfView, m_fNear, m_fFar, m_fAspectRatio, m_d3dCameraViewMatrix);

	m_bHasDodgedBool = false;
	m_bForceReturn = false;
	m_bInDeadZoneBool = false;

	m_pCurrCamera = &m_d3dCameraViewMatrix;
	m_bDetached = false;
	m_bDetach = true;
	m_bWaitForGame = true;
	D3DXMatrixIdentity(&d3dNewCamMat);
	m_fPauseBucket = 0.0f;

	m_bInWinScreen = false;

	m_d3dPreviousProjectionMatrix = m_d3dProjectionMatrix;

	m_bCreatedRand = false;
	m_bShouldShake = false;
	m_fShakeDuration = 0.0f;
	m_nCurrRotation = 0.0f;
	m_fShakeBucket = 0.0f;
	m_nSmallShakeVal = 0;
	m_nRotationAmount = 0;
	m_nBigShakeVal = 0;
}

/*****************************************************************
* Update(float _deltaTime):	Updates the camera and the matricies it uses
*
* Ins:			float _deltaTime	- _deltaTime value passed into update for _deltaTime based input
*
* Outs:		     N/A
*
* Returns:		N/A
*
* Mod. Date:		      11/30/2012
* Mod. Initials:	      JW
*****************************************************************/
void CCamera::Update( float _fTime )
{
	m_d3dPreviousProjectionMatrix = m_d3dProjectionMatrix;

	if( m_bInWinScreen )
	{
		if( m_pCurrCamera->_43 < 12000 )
		{
			D3DXMATRIX mat;
			D3DXMatrixTranslation(&mat, 0.0f, 0.0f, 200.0f * _fTime);
			*m_pCurrCamera = mat * *m_pCurrCamera;
			m_pCurrCamera->_41 = 0.0f;
			m_pCurrCamera->_42 = 0.0f;
		
			D3DXMATRIX mat2;
			D3DXMatrixTranslation(&mat2, 0.0f, 0.0f, 75.0f);
			mat2 = mat2 * *m_pCurrCamera;
			mat2._41 = 0.0f;
			mat2._42 = 0.0f;
			CPhysics::LookAt(m_pCurrCamera, &mat2);
		}
		return;
	}
	/*
	if( m_bMoveToDestination )
	{
	CinematicCamera(_fTime);
	}
	else
	{*/
	D3DXMATRIX  d3dmRotation;
	D3DXVECTOR4 d3dvPosition(m_d3dCameraViewMatrix._41, m_d3dCameraViewMatrix._42, m_d3dCameraViewMatrix._43, m_d3dCameraViewMatrix._44);
	m_d3dCameraViewMatrix._41 = m_d3dCameraViewMatrix._42 = m_d3dCameraViewMatrix._43 = 0.0f; 

	D3DXVECTOR3 d3dXAxis = CPhysics::GetMat4Vec(eXAXIS, &m_d3dCameraViewMatrix);
	D3DXVECTOR3 d3dZAx = CPhysics::GetMat4Vec(eZAXIS, &m_d3dCameraViewMatrix);

	D3DXVECTOR3 d3dYAxis = CPhysics::GetMat4Vec(eYAXIS, &m_d3dCameraViewMatrix);

	m_fChangeMouseX = CInput::GetMouseChangeX();
	m_fChangeMouseY = CInput::GetMouseChangeY();

	m_bInDeadZoneBool = CInput::GetInDeadZone();
	m_bHasDodgedBool = CInput::GetHasDodge();

	if(m_bInDeadZoneBool == false && m_bHasDodgedBool == false && m_bForceReturn == false && m_bWaitForGame == false)
	{
		m_bMoving = true;
		D3DXVECTOR3 d3dLocalX =  CPhysics::GetMat4Vec(eXAXIS, &m_d3dCameraViewMatrix); 

		D3DXMatrixRotationAxis(&d3dmRotation, &d3dLocalX, D3DXToRadian(m_fChangeMouseY * _fTime * m_fPitchSpeed * 2.0f));
		D3DXMatrixMultiply(&m_d3dCameraViewMatrix, &m_d3dCameraViewMatrix, &d3dmRotation);

		CPhysics::SetAxis(eXAXIS, m_d3dCameraViewMatrix, d3dXAxis);

		m_d3dCameraViewMatrix._41 = m_d3dCameraViewMatrix._42 = m_d3dCameraViewMatrix._43 = 0.0f; 

		d3dYAxis = CPhysics::GetMat4Vec(eYAXIS, &m_d3dCameraViewMatrix);

		D3DXVECTOR3 d3dLocalY(m_d3dCameraViewMatrix._21, m_d3dCameraViewMatrix._22, m_d3dCameraViewMatrix._23);
		D3DXMatrixRotationAxis(&d3dmRotation, &d3dLocalY, D3DXToRadian(m_fChangeMouseX * _fTime * m_fYawSpeed * 2.0f));
		D3DXMatrixMultiply(&m_d3dCameraViewMatrix, &m_d3dCameraViewMatrix, &d3dmRotation);

		CPhysics::SetAxis(eYAXIS, m_d3dCameraViewMatrix, d3dYAxis);

		m_d3dCameraViewMatrix._41 = m_d3dCameraViewMatrix._42 = m_d3dCameraViewMatrix._43 = 0.0f;
	}

	m_d3dCameraViewMatrix._41 = d3dvPosition.x; 
	m_d3dCameraViewMatrix._42 = d3dvPosition.y;
	m_d3dCameraViewMatrix._43 = d3dvPosition.z;
	m_d3dCameraViewMatrix._44 = d3dvPosition.w;

	if(!m_bMoving && CInput::GetHasDodge() == false)
		ReorientCamera(_fTime);

	m_bMoving = false;

#if _DEBUG
		if( GetAsyncKeyState( VK_UP ) && m_bDetach )
	{
		D3DXMATRIX mat;
		D3DXMatrixTranslation(&mat, 0.0f, 0.0f, 10.0f * _fTime);
		*m_pCurrCamera = mat * *m_pCurrCamera;
	}
	if( GetAsyncKeyState( VK_DOWN ) && m_bDetach )
	{
		D3DXMATRIX mat;
		D3DXMatrixTranslation(&mat, 0.0f, 0.0f, -10.0f * _fTime);
		*m_pCurrCamera = mat * *m_pCurrCamera;
	}
	if( GetAsyncKeyState( VK_LEFT ) && m_bDetach )
	{
		D3DXMATRIX mat;
		D3DXMatrixTranslation(&mat, -10.0f * _fTime, 0.0f, 0.0f);
		*m_pCurrCamera = mat * *m_pCurrCamera;
	}
	if( GetAsyncKeyState( VK_RIGHT ) && m_bDetach )
	{
		D3DXMATRIX mat;
		D3DXMatrixTranslation(&mat, 10.0f * _fTime, 0.0f, 0.0f);
		*m_pCurrCamera = mat * *m_pCurrCamera;
	}
	
#endif

	/*
	if( m_bShouldShake )
	{
		UpdateShakeVariables(_fTime);
	}
	*/

	CCollisionLib::BuildFrustum(m_tFrustum, m_fFieldOfView, m_fNear, m_fFar, m_fAspectRatio, m_d3dCameraViewMatrix);
	//	}
}

/*****************************************************************
* ReorientCamera(float time):	Reorients the camera on the y axis
*
* Ins:			None
*
* Outs:		     N/A
*
* Returns:		N/A
*
* Mod. Date:		      11/30/2012
* Mod. Initials:	      SW
*****************************************************************/
void CCamera::ReorientCamera( float _fDeltaTime )
{
	D3DXVECTOR3 d3dZAxis = CPhysics::GetMat4Vec(eZAXIS, &m_d3dCameraViewMatrix);
	D3DXVec3Normalize(&d3dZAxis, &d3dZAxis);

	//See which Direction we are going +Z or -Z
	float fZResult = D3DXVec3Dot(&d3dZAxis, &m_d3dvWorldZAxis);

	D3DXVECTOR3 d3dYAxis = CPhysics::GetMat4Vec(eYAXIS, &m_d3dCameraViewMatrix);
	D3DXVec3Normalize(&d3dYAxis, &d3dYAxis);

	float fYResult = 0;

	//Depending on the direction, get the Rotation rate
	if( fZResult > 0 )
	{
		fYResult = D3DXVec3Dot(&d3dYAxis, &m_d3dvWorldXAxis);
	}
	else if(fZResult < 0)
	{
		fYResult = D3DXVec3Dot(&d3dYAxis, &-m_d3dvWorldXAxis);
	}

	if( fYResult )
	{
		D3DXMATRIX  d3dRotationMat;

		if( fYResult < 0)
		{
			D3DXMatrixRotationAxis(&d3dRotationMat, &m_d3dvWorldZAxis, D3DXToRadian(fYResult * m_fRollSpeed * _fDeltaTime));
			D3DXMatrixMultiply(&m_d3dCameraViewMatrix, &d3dRotationMat, &m_d3dCameraViewMatrix);
		}
		else
		{
			D3DXMatrixRotationAxis(&d3dRotationMat, &m_d3dvWorldZAxis, D3DXToRadian(fYResult * m_fRollSpeed * _fDeltaTime));
			D3DXMatrixMultiply(&m_d3dCameraViewMatrix, &d3dRotationMat, &m_d3dCameraViewMatrix);
		}
	}
}

void CCamera::UpdateShakeVariables(float _fTime)
{
	m_fShakeBucket += _fTime;
	if( m_fShakeBucket >= m_fShakeDuration )
	{
		m_fShakeBucket = m_fShakeDuration = 0.0f;
		m_bShouldShake = false;
	}

	ShakeTheCamera();
	m_nCurrRotation += _fTime;
	if( m_nCurrRotation - abs(m_nRotationAmount) < 0.001f )
	{
		m_bCreatedRand = false;
	}
	if( !m_bCreatedRand )
	{
		m_nRotationAmount = rand()%m_nBigShakeVal;
		if( m_nRotationAmount < m_nSmallShakeVal )
		{
			m_nRotationAmount = m_nSmallShakeVal;
		}
		if( rand()%2 == 1 )
		{
			m_nRotationAmount *= -1;
		}
		m_bCreatedRand = true;
	}
}

void CCamera::ReorientDetatchedCamera(float _fDeltaTime)
{
	D3DXVECTOR3 d3dZAxis = CPhysics::GetMat4Vec(eZAXIS, &m_d3dDetachCameraMatrix);
	D3DXVec3Normalize(&d3dZAxis, &d3dZAxis);

	//See which Direction we are going +Z or -Z
	float fZResult = D3DXVec3Dot(&d3dZAxis, &m_d3dvWorldZAxis);

	D3DXVECTOR3 d3dYAxis = CPhysics::GetMat4Vec(eYAXIS, &m_d3dDetachCameraMatrix);
	D3DXVec3Normalize(&d3dYAxis, &d3dYAxis);

	float fYResult = 0;

	//Depending on the direction, get the Rotation rate
	if( fZResult > 0 )
	{
		fYResult = D3DXVec3Dot(&d3dYAxis, &m_d3dvWorldXAxis);
	}
	else if(fZResult < 0)
	{
		fYResult = D3DXVec3Dot(&d3dYAxis, &-m_d3dvWorldXAxis);
	}

	if( fYResult )
	{
		D3DXMATRIX  d3dRotationMat;

		if( fYResult < 0)
		{
			D3DXMatrixRotationAxis(&d3dRotationMat, &m_d3dvWorldZAxis, D3DXToRadian(fYResult * m_fRollSpeed * _fDeltaTime));
			D3DXMatrixMultiply(&m_d3dDetachCameraMatrix, &d3dRotationMat, &m_d3dDetachCameraMatrix);
		}
		else
		{
			D3DXMatrixRotationAxis(&d3dRotationMat, &m_d3dvWorldZAxis, D3DXToRadian(fYResult * m_fRollSpeed * _fDeltaTime));
			D3DXMatrixMultiply(&m_d3dDetachCameraMatrix, &d3dRotationMat, &m_d3dDetachCameraMatrix);
		}
	}
}

/*****************************************************************
* ResetCamera():	Sets the Camera's Matrix to Identity
*
* Ins:			None
*
* Outs:		    None 
*
* Returns:		
*
* Mod. Date:		   12/7/2012  
* Mod. Initials:	   SW
*****************************************************************/
void CCamera::ResetCamera()
{
	D3DXMatrixIdentity(&m_d3dCameraViewMatrix);
}

/*****************************************************************
* ResetCameraToCheckPoint():	Sets the Camera's Matrix to the Checkpoint
*
* Ins:			None
*
* Outs:		    None 
*
* Returns:		
*
* Mod. Date:		   12/7/2012  
* Mod. Initials:	   SW
*****************************************************************/
void CCamera::ResetCameraToCheckPoint()
{
	m_d3dCameraViewMatrix = m_d3dCheckPointCamera;
}

void CCamera::SetViewMatrixPos(D3DXVECTOR3* _pNewVec)
{
	//CCollisionLib::BuildFrustum(m_tFrustum, m_fFieldOfView, m_fNear, m_fFar, m_fAspectRatio, m_d3dCameraViewMatrix);
	CPhysics::SetAxis(ePOS, m_d3dCameraViewMatrix, *_pNewVec);
}

void CCamera::DetachCamera(bool _bDetach)
{
	m_bDetach = _bDetach; 

	if(m_bDetach)
	{
		if(m_bDetached)
		{
			return;
		}
		else
		{
			m_bDetached = true;
			m_d3dReturnPos = m_d3dCameraViewMatrix; 
			m_d3dDetachCameraMatrix = m_d3dCameraViewMatrix;
			m_pCurrCamera = &m_d3dDetachCameraMatrix;
		}
	}
	else
	{
		m_bDetached = false;
		m_pCurrCamera = &m_d3dCameraViewMatrix;
	}
}

D3DXMATRIX* CCamera::GetViewMatrix(bool _isPlayer) 
{
	if( _isPlayer && m_bShouldShake)
	{
		D3DXMATRIX returnMat = m_d3dCameraViewMatrix;
		D3DXMATRIX rot;
		float r = 0.25f * m_nCurrRotation;

		if( rand()%2 == 1 )
		{
			r *= -1;
		}

		D3DXMatrixRotationY(&rot, D3DXToRadian( r ) );
		return &(rot * m_d3dCameraViewMatrix);
	}
	return &m_d3dCameraViewMatrix;
}

//void CCamera::SetFoV(float _fFoV)
//{
//	D3DXMatrixPerspectiveFovLH(&m_d3dProjectionMatrix, _fFoV ,m_fAspectRatio,m_fNear,m_fFar);
//}
void CCamera::SetWidthHeight(float _fScreenWidth,float _fScreenHeight)
{
	m_fWidth = _fScreenWidth;
	m_fHeight = _fScreenHeight;

	m_fFieldOfView = FIELD_OF_VIEW;	
	m_fAspectRatio = m_fWidth / m_fHeight;

	m_fNear = CAM_NEAR;
	m_fFar  = CAM_FAR;

	D3DXMatrixPerspectiveFovLH(&m_d3dProjectionMatrix, m_fFieldOfView ,m_fAspectRatio,m_fNear,m_fFar);
}

void CCamera::Shake(int _nSmallShakeValue, int _nBigShakeValue, float _fDurration)
{
	if( !m_bShouldShake )
	{
		m_nCurrRotation = 0.0f;
	}
	m_bShouldShake = true;
	m_fShakeDuration = _fDurration;
	m_nSmallShakeVal = _nSmallShakeValue;
	m_nBigShakeVal = _nBigShakeValue;

	if( m_nBigShakeVal < 1 )
	{
		m_nBigShakeVal = 1;
	}
}

void CCamera::ShakeTheCamera()
{
	D3DXMATRIX rot;
	if( m_nRotationAmount < 0 )
	{
		D3DXMatrixRotationY(&rot, D3DXToRadian( -m_nCurrRotation ) );
	}
	else
	{
		D3DXMatrixRotationY(&rot, D3DXToRadian( m_nCurrRotation ) );
	}

	*m_pCurrCamera = rot * *m_pCurrCamera;
}