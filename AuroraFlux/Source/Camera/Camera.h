/***********************************************
* Filename:  		Camera.h
* Date:      		11/27/2012
* Mod. Date: 		11/30/2012
* Mod. Initials:	SW
* Author:    		Bryan Schotanes
* Purpose:   		This is the camera for everything that 
*					needs to be rendered
************************************************/
#ifndef	CAMERA_H
#define CAMERA_H

#include "../Collision/CollisionLib.h"

class CCamera
{
public:
	CCamera() {};
	~CCamera();

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
	void Init(float _fScreenWidth, float _fScreenHeight, HWND _hWnd);

	/*****************************************************************
	* Update(float time):	Updates the camera and the matricies it uses
	*
	* Ins:			float time	- Time value passed into update for time based input
	*
	* Outs:		     N/A
	*
	* Returns:		N/A
	*
	* Mod. Date:		      11/30/2012
	* Mod. Initials:	      JW
	*****************************************************************/
	void Update(float _fTime);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Accessors
	//////////////////////////////////////////////////////////////////////////////////////////////////////

	/*****************************************************************
	* GetWorldMatrix(float time):	Gets the Camera's Matrix
	*
	* Ins:			None
	*
	* Outs:		    None 
	*
	* Returns:		D3DXMATRIX* The World Matrix
	*
	* Mod. Date:		   11/30/2012  
	* Mod. Initials:	   SW 
	*****************************************************************/
	D3DXMATRIX * GetViewMatrix(bool _isPlayer = false);

	/*****************************************************************
	* GetProjectionMatrix(float time):	Gets the Camera's Projection Matrix
	*
	* Ins:			None
	*
	* Outs:		    None 
	*
	* Returns:		D3DXMATRIX* The Projection Matrix
	*
	* Mod. Date:		   12/1/2012  
	* Mod. Initials:	   SW 
	*****************************************************************/
	D3DXMATRIX * GetProjectionMatrix() { return &m_d3dProjectionMatrix; }

	/*****************************************************************
	* ReorientCamera():	Reorients the camera on the y axis
	*
	* Ins:			float:  used for change in time
	*
	* Outs:		     N/A
	*
	* Returns:		N/A
	*
	* Mod. Date:		      11/30/2012
	* Mod. Initials:	      SW
	*****************************************************************/
	void ReorientCamera(float _fDeltaTime);
	void ReorientDetatchedCamera(float _fDeltaTime);

	/*****************************************************************
	* SetViewMatrix():	Sets the Camera's Matrix
	*
	* Ins:			_newMat
	*
	* Outs:		    None 
	*
	* Returns:		
	*
	* Mod. Date:		   12/4/2012  
	* Mod. Initials:	   AQ
	*****************************************************************/
	void SetViewMatrix(D3DXMATRIX* _pNewMat) { m_d3dCameraViewMatrix = *_pNewMat; }
	void SetViewMatrixPos(D3DXVECTOR3* _pNewVec);

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
	void ResetCamera();

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
	void ResetCameraToCheckPoint();

	/*****************************************************************
	* SetCheckpointCamera():	Sets the CheckPoint's Camera's Matrix to the Checkpoint
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
	void SetCheckpointCamera(D3DXMATRIX _d3dCheckPointCamera)  { m_d3dCheckPointCamera = _d3dCheckPointCamera; }

	/*****************************************************************
	* Shake():	Shakes the camera in a random direction between two values for a certain amount of time.
	*
	* Ins:		- _fSmallShakeValue	-	used with rand() to determine the smallest shake the camera could have.
	*			- _fBigShakeValue	-	used with rand() to determine the largest shake the camera could have.
	*			- _fDurration		-	the amount of time the shake will occur for.
	*
	* Outs:		None
	*
	* Returns:	None
	******************************************************************/
	void Shake(int _fSmallShakeValue, int _fBigShakeValue, float _fDurration);

	float GetHiX() {return m_fChangeMouseX;}
	float GetHiY() {return m_fChangeMouseY;}
	tFrustum* GetFrustum() { return &m_tFrustum;}
	bool GetForceReturn() {return m_bForceReturn;}
	void SetForceReturn(bool _forceReturn) {m_bForceReturn = _forceReturn;}
	D3DXVECTOR3 GetPosition() {return D3DXVECTOR3(m_d3dDetachCameraMatrix._41, m_d3dDetachCameraMatrix._42, m_d3dDetachCameraMatrix._43);}

	D3DXVECTOR3 GetCurrentPosition() {return D3DXVECTOR3(m_pCurrCamera->_41, m_pCurrCamera->_42, m_pCurrCamera->_43);}

	void SetDetachedCamera(D3DXVECTOR3 _detachedCamera) {m_d3dDetachCameraMatrix._41 = _detachedCamera.x; m_d3dDetachCameraMatrix._42 = _detachedCamera.y; m_d3dDetachCameraMatrix._43 = _detachedCamera.z;}

	D3DXMATRIX * GetCurrentMatrix() { return m_pCurrCamera; }
	void DetachCamera(bool _bDetach); 
	bool GetInDeadZone() {return m_bInDeadZoneBool;}

	float GetYawSpeed() {return m_fYawSpeed;}
	float GetPitchSpeed() {return m_fPitchSpeed;}
	bool GetWaitForGame() {return m_bWaitForGame;}
	float GetWidth() {return m_fWidth;}
	float GetHeight() {return m_fHeight;}
	void SetYawSpeed(float _fYawSpeed) {m_fYawSpeed = _fYawSpeed;}
	void SetPitchSpeed(float _fPitchSpeed) {m_fPitchSpeed = _fPitchSpeed;}
	void SetWaitForGame(bool _shouldWait) {m_bWaitForGame = _shouldWait;}
	void SetInWinScreen(bool _bInWinScreen) {m_bInWinScreen = _bInWinScreen;}
	
	void SetCameraMatrix(D3DXMATRIX _mat) {(*m_pCurrCamera) = _mat;}

	D3DXMATRIX GetReturnPos() {return m_d3dReturnPos;}

	void SetReturnPos(D3DXMATRIX mat) {m_d3dReturnPos = mat;}
	bool GetIsDetached() {return m_bDetached;}
	D3DXMATRIX GetPreviousProjectionMatrix() const { return m_d3dPreviousProjectionMatrix; }
	void SetPreviousProjectionMatrix(D3DXMATRIX val) { m_d3dPreviousProjectionMatrix = val; }
	void SetCurrentCameraMatrix(D3DXMATRIX mat) {*m_pCurrCamera = mat;}
	//void SetFoV(float _fFoV);
	void SetWidthHeight(float _Width, float _Height);
	D3DXMATRIX GetDetachedPos() {return m_d3dDetachCameraMatrix;}
	bool GetShouldShake() { return m_bShouldShake; }
	bool GetInWinScreen() {return m_bInWinScreen;}

	void ShakeTheCamera();
	void UpdateShakeVariables(float _fTime);

	bool GetInMainMenu() {return m_bInMainMenu;}
	void SetInMainMenu(bool _bInMainMenu) {m_bInMainMenu = _bInMainMenu;}
private:

	D3DXMATRIX m_d3dCameraViewMatrix;
	D3DXMATRIX m_d3dProjectionMatrix;
	D3DXMATRIX m_d3dPreviousProjectionMatrix;
	
	D3DXVECTOR3 m_d3dvWorldXAxis;
	D3DXVECTOR3 m_d3dvWorldYAxis;
	D3DXVECTOR3 m_d3dvWorldZAxis;
		
	D3DXMATRIX m_d3dReturnPos;
	D3DXMATRIX m_d3dCheckPointCamera;

	float m_fFieldOfView;
	float m_fAspectRatio;
	float m_fNear;
	float m_fFar;
	float m_fWidth;
	float m_fHeight;

	tFrustum m_tFrustum;

	bool m_bMoving;
	float m_fYawSpeed;
	float m_fPitchSpeed;
	float m_fRollSpeed;

	bool m_bHasDodgedBool;
	bool m_bForceReturn;
	bool m_bInDeadZoneBool;
	float m_fPauseBucket;

	float m_fChangeMouseX;
	float m_fChangeMouseY;

	HWND m_hWnd;

	D3DXMATRIX  m_d3dDetachCameraMatrix;
	D3DXMATRIX* m_pCurrCamera;
	D3DXMATRIX d3dNewCamMat;

	bool m_bDetach;
	bool m_bDetached;
	bool m_bWaitForGame;	

	bool m_bCreatedRand,
		 m_bShouldShake;
	float m_fShakeDuration,
		  m_nCurrRotation,
		  m_fShakeBucket;
	int	  m_nSmallShakeVal,
		  m_nRotationAmount,
		  m_nBigShakeVal;

	bool m_bInWinScreen;
	bool m_bInMainMenu;
};

#endif