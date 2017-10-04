#include "CameraObject.h"
#include "../Collision/Physics.h"
#include "../State System/Game.h"
#include "../State System/Game.h"
#include "../Entity/comet.h"
#include "../AI/Ambient Fighters/Ambient Fighter.h"
#include "../AI/Ambient Fighters/AmbientBoundingBox.h"

CCameraObject::CCameraObject(CCamera* _pCurrentCamera, CObjectManager* _pObjMan) : m_fTeachXLocation(-300), m_ntransitionAlpha(0)
{
	m_szFadeFlag = DONT_FADE;
	m_pCurrentCamera = _pCurrentCamera;
	m_pObjectManager = _pObjMan;
	m_nObjectType = eCAM_OBJ;
	m_nHealth = 1;
	m_bActive = true;
	m_bReachedDestination = false;
	m_bRotate = false;
	m_bReturn = false;
	m_bOnlyRotate = false;
	m_fRotation = 0.0f;
	m_fWaitTimer = 0.0f;
	m_nTotalDistance = 0;
	m_fSpeed = 10.0f;
	m_fRotationBucket = 0.0f;
	m_bIsBossFight = false;
	m_pBoss = NULL;
	m_dwTime = 0.0f;
	m_tPrevMousePos.x = -1;
	m_tPrevMousePos.y = -1;
	m_bReturning = true;
	m_bCinematicEnter = false;
	m_fTransition = 0.0f;
	m_fBlackoutAlpha = 0.0f;
	m_bHasPostPosition = false;
	m_bShouldFadeIn = false;
	m_fWaitToReturn = 0.0f;
	m_bHasSkipped = false;
	m_bCanBeSkipped = true;
	m_bHasFinishedCinematic = false;
	m_fIntroReturnBucket = 0.0f;
	m_nCurrentAsteroidTarget = 0;

	m_bIsIntroCinematic = true;
	m_szIntroStage = eINTRO_AVOID_ASTEROIDS;

	SetBV(NULL);
	m_bFirstPass = true;
	m_bFirstFighterPassed = false;
	m_bSecondFigherPassed = false;
	m_bHasPlayedIntroduction = false;
	m_bHasPlayedHomePlanet = false;
	m_bHasPlayedDontWorry = false;
	m_fIntroSoundBuffer = 0.0f;
	m_fDontWorryBuffer = 0.0f;
	m_nIntroCameraWaypointIter = 0;
	SetUpIntroWaypoints();
	m_fTimeGoingToWaypoint = 0.0f;

	m_bAlreadySeen = false;
}
void CCameraObject::SetMatPos(D3DXVECTOR3 _p, D3DXMATRIX& _m)
{
	_m._41 = _p.x;
	_m._42 = _p.y;
	_m._43 = _p.z;
}
void CCameraObject::SetUpIntroWaypoints()
{
	D3DXMATRIX tempMat;
	D3DXMatrixIdentity(&tempMat);
	D3DXVECTOR3 tempVec;
	//to first asteroid
	tempVec = D3DXVECTOR3(75, 75, 3500);
	SetMatPos(tempVec, tempMat);
	m_d3dIntroCameraWaypoints.push_back(tempMat);
	//start turning around asteroid
	tempVec = D3DXVECTOR3(200, 100, 3250);
	SetMatPos(tempVec, tempMat);
	m_d3dIntroCameraWaypoints.push_back(tempMat);
	tempVec = D3DXVECTOR3(175, 100, 3000);
	SetMatPos(tempVec, tempMat);
	m_d3dIntroCameraWaypoints.push_back(tempMat);

	//go under second asteroid
	tempVec = D3DXVECTOR3(250, -120, 2750);
	SetMatPos(tempVec, tempMat); 
	m_d3dIntroCameraWaypoints.push_back(tempMat);
	tempVec = D3DXVECTOR3(230, -300, 2650);
	SetMatPos(tempVec, tempMat);
	m_d3dIntroCameraWaypoints.push_back(tempMat);
	tempVec = D3DXVECTOR3(175, 50, 2450);
	SetMatPos(tempVec, tempMat);
	m_d3dIntroCameraWaypoints.push_back(tempMat);


	//move back the way you came a bit to give room for the fighters (because of weird culling)

	tempVec = D3DXVECTOR3(75, 30, 2650);
	SetMatPos(tempVec, tempMat);
	m_d3dIntroCameraWaypoints.push_back(tempMat);
	tempVec = D3DXVECTOR3(-50, 0, 3200);
	SetMatPos(tempVec, tempMat);
	m_d3dIntroCameraWaypoints.push_back(tempMat);
	//tempVec = D3DXVECTOR3(100, 0, 3400);
	//SetMatPos(tempVec, tempMat);
	//m_d3dIntroCameraWaypoints.push_back(tempMat);
}

CCameraObject::~CCameraObject()
{
}

void CCameraObject::SetDestination(D3DXMATRIX _d3dDestMat, D3DXMATRIX _d3dLookAtMat, float _fDist, float _fWait )
{
	m_d3dDestination = _d3dDestMat; 
	m_d3dDestLookAtMat = _d3dLookAtMat; 
	m_fDistanceFromDestination = _fDist;
	m_fDestWaitTime = _fWait;
}

void CCameraObject::Update(float _fElapsedTime)
{
	/*
	m_bMoveToDestination = true; 
	m_bHasFinishedCinematic = false;
	m_bReturn = true;
	m_bCinematicEnter = true;*/
	
	float ulNow = (float)(GetTickCount());
	float fElapsed = (ulNow - m_dwTime) / 1000.0f;
	m_dwTime = ulNow;
	if( fElapsed > 0.1f )
	{
		fElapsed= 0.1f;
	}

	if( m_fWaitToReturn > 0.0f )
	{
		m_fWaitToReturn -= fElapsed;
		if( m_fWaitToReturn < 0.0f )
		{
			m_fWaitToReturn = 0.0f;
			StopCinematic();	
			if( m_pBoss )
			{
				if( !m_pBoss->GetIsDisabled() )
				{
					m_pBoss->SetIsVisable(true);
					if( m_pBoss->GetDestinationPos().z > m_pBoss->GetMatrix()->_43 )
					{
						m_pBoss->SetPosition(m_pBoss->GetDestinationPos());
					}					
				}
			}
		}
	}

	if( m_szFadeFlag == FADE_OUT )
	{
		m_fBlackoutAlpha += fElapsed;
		if( m_fBlackoutAlpha > 1.0f )
		{
			m_fBlackoutAlpha = 1.0f;
			m_szFadeFlag = FADE_IN;
			if( m_pBoss && (m_pBoss->GetPhase() == eCINEMATIC_PHASE))
			{
				m_pObjectManager->SetRemoveMiscItems(true);
			}
		}
	}
	else if( m_szFadeFlag == FADE_IN )
	{
		m_fBlackoutAlpha -= fElapsed;
		if( m_fBlackoutAlpha < 0.0f )
		{
			m_fBlackoutAlpha = 0.0f;
			m_szFadeFlag = DONT_FADE;
			m_bShouldFadeIn = false;
		}
	}

	if( m_pCurrentCamera )
	{
		if( m_bIsIntroCinematic)
		{
			if( (CInput::DIKBufferedButtonCheck(DIK_SPACE) && m_bCanBeSkipped) || m_bAlreadySeen == true )
			{
				WwiseNS::StopAllTutorialVoices(-1);
				WwiseNS::SoundEvent(eProceedToWaypoint, ePlay);
				m_fWaitToReturn = 1.0f;
				m_szFadeFlag = FADE_OUT;
				m_bShouldFadeIn = true;
				m_bIsIntroCinematic = false;	
				m_bReturning = true;
				m_szIntroStage = eINTRO_AVOID_ASTEROIDS;
				m_pPlayer->SetIsVisable(true);
				m_pPlayer->SetIsScenery(false);
				return;
			}
			PlayIntroCinematic(fElapsed);
		
			if( m_pCurrentCamera->GetShouldShake() )
			{
				m_pCurrentCamera->UpdateShakeVariables(_fElapsedTime);
				m_pCurrentCamera->ShakeTheCamera();
			}
			return;
		}

		if( m_bReachedDestination )
		{
			Return(fElapsed);
		}
		else if( m_bRotate )
		{			
			if( CInput::DIKBufferedButtonCheck(DIK_SPACE) && m_bCanBeSkipped )
			{
				if( !m_bHasSkipped )
				{
					WwiseNS::StopAllTutorialVoices(-1);
					m_fWaitToReturn = 1.0f;
					m_szFadeFlag = FADE_OUT;
					m_bShouldFadeIn = true;
					m_bHasSkipped = true;
					return;
				}
			}
			Rotate(fElapsed);
		} 
		else if( m_bReturn )
		{
			if( m_tPrevMousePos.x == -1 && m_tPrevMousePos.y == -1 )
			{
				m_tPrevMousePos = CInput::GetMouseScreenPos();
			}
			if(CInput::DIKBufferedButtonCheck(DIK_SPACE) && m_bCanBeSkipped)
			{
				if( !m_bHasSkipped ) 
				{
					WwiseNS::StopAllTutorialVoices(-1);
					m_fWaitToReturn = 1.0f;
					m_szFadeFlag = FADE_OUT;
					m_bShouldFadeIn = true;
					m_bHasSkipped = true;
					return;
				}
			}
			GoToDestination(fElapsed);
		}
		else if( m_bCinematicEnter )
		{
			if( m_tPrevMousePos.x == -1 && m_tPrevMousePos.y == -1 )
			{
				m_tPrevMousePos = CInput::GetMouseScreenPos();
			}
			if(CInput::DIKBufferedButtonCheck(DIK_SPACE) && m_bCanBeSkipped)
			{
				if( !m_bHasSkipped )
				{
					WwiseNS::StopAllTutorialVoices(-1);
					m_fWaitToReturn = 1.0f;
					m_szFadeFlag = FADE_OUT;
					m_bShouldFadeIn = true;
					m_bHasSkipped = true;
					return;
				}
			}
			Entrance(fElapsed);
		}
		
		if( m_pCurrentCamera->GetShouldShake() )
		{
			m_pCurrentCamera->UpdateShakeVariables(_fElapsedTime);
			m_pCurrentCamera->ShakeTheCamera();
		}
	}
}
void CCameraObject::Render()
{
}
int CCameraObject::GetType()
{
	return m_nObjectType;
}
D3DXMATRIX* CCameraObject::GetMatrix()
{
	return m_pCurrentCamera->GetCurrentMatrix();
}

void CCameraObject::SetRotation(float _fRotationAmount, D3DXMATRIX _lookAtMat, float _fRotationDuration, bool _bOnlyRotate, bool _bReturning)
{ 
	m_fRotationAmount = _fRotationAmount; 
	m_d3dLookAtMat = _lookAtMat;
	m_fRotation = _fRotationDuration;
	m_bRotate = _bOnlyRotate;
	m_bOnlyRotate = _bOnlyRotate;
	m_bReturning = _bReturning;
}

void CCameraObject::GoToDestination(float _fDt)
{	
	// do a LookAt at the destination position, then move forward	
	D3DXMATRIX d3dMat;
	D3DXMatrixIdentity(&d3dMat);

	D3DXVECTOR3 d3dPos;
	d3dPos.x = m_d3dDestination._41 - m_pCurrentCamera->GetCurrentMatrix()->_41;
	d3dPos.y = m_d3dDestination._42 - m_pCurrentCamera->GetCurrentMatrix()->_42;
	d3dPos.z = m_d3dDestination._43 - m_pCurrentCamera->GetCurrentMatrix()->_43;

	float dist = D3DXVec3Length(&d3dPos);

	if( m_nTotalDistance <= 0 )
	{
		m_nTotalDistance = (int)dist;
		m_fSpeed = 10.0f;
	}

	if( dist < m_fDistanceFromDestination )
	{
		m_fWaitTimer += _fDt;
		if( m_fWaitTimer > m_fDestWaitTime )
		{
			m_bRotate = true;
			m_bReturn = false;
			m_nTotalDistance = 0;
			m_fWaitTimer = 0.0f;
		}
		else if( m_fWaitTimer > 1.5f )
		{
			D3DXVECTOR3 destpos = D3DXVECTOR3(m_d3dLookAtMat._41, m_d3dLookAtMat._42, m_d3dLookAtMat._43);
			CPhysics::TurnTo(m_pCurrentCamera->GetCurrentMatrix(), &destpos, _fDt, 0.00004f);
			//CPhysics::TurnTo(m_pCurrentCamera->GetCurrentMatrix(),  &m_pPlayer->GetPosition(), _fDt, 0.002f);
		}
		return;
	}

	D3DXVec3Normalize(&d3dPos, &d3dPos);

	if( dist < 20.0f * m_nTotalDistance * 0.01f )
	{
		m_fSpeed -= _fDt * 400.0f;
	}
	else if( dist > 50.0f * m_nTotalDistance * 0.01f )
	{
		m_fSpeed += _fDt * 150.0f;
	}
	else 
		m_fSpeed = 400.0f;

	if( m_fSpeed < 150.0f )
		m_fSpeed = 150.0f;

	D3DXMatrixTranslation(&d3dMat,	d3dPos.x *m_fSpeed*_fDt, 
									d3dPos.y *m_fSpeed*_fDt, 
									d3dPos.z *m_fSpeed*_fDt);

	*m_pCurrentCamera->GetCurrentMatrix() =  (*m_pCurrentCamera->GetCurrentMatrix()) * d3dMat;
	if( m_pBoss )
	{
		if( m_pBoss->GetPhase() == eCINEMATIC_PHASE )
		{
			CPhysics::LookAt(m_pCurrentCamera->GetCurrentMatrix(), m_pBoss->GetMatrix());	
		}
		else
		{
			CPhysics::LookAt(m_pCurrentCamera->GetCurrentMatrix(), &m_d3dDestLookAtMat);	
		}
	}
	else
	{
		CPhysics::LookAt(m_pCurrentCamera->GetCurrentMatrix(), &m_d3dDestLookAtMat);	
	}
	m_fTeachXLocation+=6;
	m_ntransitionAlpha += 3;
	if (m_ntransitionAlpha >= 150)
	{
		m_ntransitionAlpha = 150;
	}
	if (m_fTeachXLocation >= 55)
	{
		m_fTeachXLocation = 55;
	}

#ifdef _DEBUG
	if( GetAsyncKeyState( 'G' ) )
	{
		CPhysics::LookAt(m_pCurrentCamera->GetCurrentMatrix(), m_pPlayer->GetMatrix());
	}
#endif
}
void CCameraObject::Rotate(float _fDt)
{
	m_fRotationBucket += _fDt;

	if( m_fRotationBucket >= m_fRotation )
	{
		m_fWaitTimer += _fDt;
		m_fTeachXLocation+=6;
		if (m_fTeachXLocation >= 55)
		{
			m_fTeachXLocation = 55;
		}
		m_ntransitionAlpha += 3;
		if (m_ntransitionAlpha >= 150)
		{
			m_ntransitionAlpha = 150;
		}
		if( m_fWaitTimer > 1.25f )
		{
			m_szFadeFlag = FADE_OUT;			
			m_bShouldFadeIn = true;
		}
		if( m_fWaitTimer > 3.0f )
		{
			if( m_bOnlyRotate )
			{
				if( m_tPrevMousePos.x != -1 && m_tPrevMousePos.y != -1 )
				{
					SetCursorPos( m_tPrevMousePos.x, m_tPrevMousePos.y );
					CInput::SetMouseScreenPos(m_tPrevMousePos.x, m_tPrevMousePos.y);
					m_tPrevMousePos.x = -1;
					m_tPrevMousePos.y = -1;
				}

				m_bMoveToDestination = false;
				m_bReachedDestination = false;
				m_bRotate = false; 
				m_bReturn = false;
				m_bIsBossFight = false;
				m_bOnlyRotate = false;
				m_bHasFinishedCinematic = true;
				m_fWaitTimer = 0.0f;
				m_fSpeed = 10.0f;
				m_nTotalDistance = 0;
				m_fRotationBucket = 0.0f;
				m_bShouldFadeIn = true;
				if( m_pBoss )
				{
					m_pBoss->SetInCinematic(false);
				}
				m_pCurrentCamera->DetachCamera(!m_bReturning);
				
			}
			else
			{
				//////////////////////////////////////////////
				// Skip Return Cinematic
				StopCinematic();
				//////////////////////////////////////////////
				/*
				// Return Cinematic
				m_bReachedDestination = true;
				m_bRotate = false;
				m_fRotationBucket = 0.0f;
				m_fWaitTimer = 0.0f;
				m_fSpeed = 10.0f;
				*/
			}
		}
		return;
	}
	
	//D3DXMATRIX d3dMat;
	//D3DXMatrixIdentity(&d3dMat);
	//D3DXMatrixTranslation(&d3dMat, 1000*_fTime, 0.0f, 0.0f);
	//*m_pCurrCamera = d3dMat * (*m_pCurrCamera);
	D3DXMATRIX transMat;
	D3DXMatrixIdentity(&transMat);

	D3DXVECTOR3 campos = D3DXVECTOR3(m_pCurrentCamera->GetCurrentMatrix()->_41, m_pCurrentCamera->GetCurrentMatrix()->_42, m_pCurrentCamera->GetCurrentMatrix()->_43);
	D3DXVECTOR3 targetpos = D3DXVECTOR3( m_d3dLookAtMat._41, m_d3dLookAtMat._42, m_d3dLookAtMat._43);
	D3DXVECTOR3 diff = targetpos - campos;
	D3DXMatrixTranslation(&transMat, m_fRotationAmount *_fDt, 0, 0);	

	(*m_pCurrentCamera->GetCurrentMatrix()) = transMat * (*m_pCurrentCamera->GetCurrentMatrix());
	
	if( m_pBoss )
	{
		if( m_pBoss->GetPhase() == eCINEMATIC_PHASE )
		{
			CPhysics::LookAt(m_pCurrentCamera->GetCurrentMatrix(), m_pBoss->GetMatrix());	
		}
		else
		{
			CPhysics::LookAt(m_pCurrentCamera->GetCurrentMatrix(), &m_d3dLookAtMat);	
		}
	}
	else
	{
		CPhysics::LookAt(m_pCurrentCamera->GetCurrentMatrix(), &m_d3dLookAtMat);	
	}

	SetMatrix(m_pCurrentCamera->GetCurrentMatrix());
	m_fTeachXLocation+=4;
	if (m_fTeachXLocation >= 55)
	{
		m_fTeachXLocation = 55;
	}
	m_ntransitionAlpha += 3;
	if (m_ntransitionAlpha >= 150)
	{
		m_ntransitionAlpha = 150;
	}
	
// 
// #ifdef _DEBUG
// 	if( GetAsyncKeyState( 'G' ) )
// 	{
// 		CPhysics::LookAt(m_pCurrentCamera->GetCurrentMatrix(), m_pPlayer->GetMatrix());
// 	}
// #endif
}
void CCameraObject::Return(float _fDt)
{
	if (m_pBoss)
	{
		m_pBoss->SetTeachBoss(false);
	}

	m_ntransitionAlpha -= 4;
	if (m_ntransitionAlpha <= 0)
	{
		m_ntransitionAlpha = 0;
	}
	// move back to the player
	D3DXMATRIX d3dMat;
	D3DXMatrixIdentity(&d3dMat);			

	D3DXVECTOR3 d3dPos;
	d3dPos.x = m_pCurrentCamera->GetReturnPos()._41 - m_pCurrentCamera->GetCurrentMatrix()->_41;
	d3dPos.y = m_pCurrentCamera->GetReturnPos()._42 - m_pCurrentCamera->GetCurrentMatrix()->_42;
	d3dPos.z = m_pCurrentCamera->GetReturnPos()._43 - m_pCurrentCamera->GetCurrentMatrix()->_43;
	
	float dist = D3DXVec3Length(&d3dPos);

	if( m_nTotalDistance <= 0 )
	{
		m_nTotalDistance = (int)dist;
		m_fSpeed = 10.0f;
	}

	if( dist < 25.0f )
	{
		if( m_fWaitTimer < 0.01f )
		{
			m_pCurrentCamera->SetCameraMatrix(m_pCurrentCamera->GetReturnPos());
		}
		else
		{
			CPhysics::TurnTo(m_pCurrentCamera->GetCurrentMatrix(),  &m_pPlayer->GetPosition(), _fDt, 0.01f);
		}

		m_fWaitTimer += _fDt;
		if( m_fWaitTimer > 1.0f )
		{
			StopCinematic();
		}
		return;
	}
	
	if( dist < 20.0f * m_nTotalDistance * 0.01f )
	{
		m_fSpeed -= _fDt * 400.0f;
	}
	else if( dist > 50.0f * m_nTotalDistance * 0.01f )
	{
		m_fSpeed += _fDt * 150.0f;
	}
	else 
		m_fSpeed = 400.0f;

	if( m_fSpeed < 150.0f )
		m_fSpeed = 150.0f;

	D3DXVec3Normalize(&d3dPos, &d3dPos);
	D3DXMatrixTranslation(&d3dMat,  d3dPos.x * m_fSpeed * _fDt, 
									d3dPos.y * m_fSpeed * _fDt, 
									d3dPos.z * m_fSpeed * _fDt);

	m_pCurrentCamera->SetCameraMatrix( (*m_pCurrentCamera->GetCurrentMatrix()) * d3dMat );	

	if( m_pPlayer )
	{
		CPhysics::LookAt(m_pCurrentCamera->GetCurrentMatrix(), m_pPlayer->GetMatrix());
	}
}

void CCameraObject::Entrance(float _fDt)
{
	m_bShouldFadeIn = true;
	m_fTransition += _fDt;
	/*
	if( m_fTransition < 0.75f )
	{
		m_fBlackoutAlpha += _fDt;
		if( m_fBlackoutAlpha > 1.0f )
		{
			m_fBlackoutAlpha = 1.0f;
		}
	}
	else
	{
		m_fBlackoutAlpha -= _fDt;
		if( m_fBlackoutAlpha < 0.0f )
		{
			m_fBlackoutAlpha = 0.0f;
		}
	}
	*/
	m_szFadeFlag = FADE_OUT;
	m_bShouldFadeIn = true;
	/*if( m_fTransition < 2.0f )
	{
		D3DXMATRIX mat;
		D3DXMatrixTranslation(&mat, 0.0f, 1.5f, 0.0f);

		m_pCurrentCamera->SetCameraMatrix(mat * *m_pCurrentCamera->GetCurrentMatrix());
	}
	else */
	if( m_fTransition > 1.0f )
	{
		m_bReturn = true;
		m_bCinematicEnter = false;
		m_fTransition = 0.0f; 
	}
	
	if( m_bHasPostPosition && m_fBlackoutAlpha > 0.95f )
	{
		m_pCurrentCamera->SetCameraMatrix(m_d3dPostEntrancePosition);
		m_bHasPostPosition = false;
	}
}

void CCameraObject::SetCameraPositionAfterEntrance(D3DXMATRIX mat) 
{ 
	m_d3dPostEntrancePosition = mat; 
	m_bHasPostPosition = true;
}

void CCameraObject::StopCinematic()
{
	if( m_tPrevMousePos.x != -1 && m_tPrevMousePos.y != -1 )
	{
		SetCursorPos( m_tPrevMousePos.x, m_tPrevMousePos.y );
		CInput::SetMouseScreenPos(m_tPrevMousePos.x, m_tPrevMousePos.y);
		m_tPrevMousePos.x = -1;
		m_tPrevMousePos.y = -1;
	}
	
	m_bCinematicEnter = false;
	m_fTeachXLocation = -300;
	m_bMoveToDestination = false;
	m_bReachedDestination = false;
	m_bRotate = false; 
	m_bReturn = false;
	m_bIsBossFight = false;
	m_pCurrentCamera->SetViewMatrix(&m_pCurrentCamera->GetReturnPos());
	m_pCurrentCamera->DetachCamera(!m_bReturning);
	m_bHasFinishedCinematic = true;
	m_fWaitTimer = 0.0f;
	m_fSpeed = 10.0f;
	m_nTotalDistance = 0;
	m_fRotationBucket = 0.0f;
	m_bHasSkipped = false;
	
	if( m_szFadeFlag == DONT_FADE )
	{
		m_bShouldFadeIn = true;
		m_szFadeFlag = FADE_OUT;
	}			

	/// For Testing!!!
	/*
	D3DXVECTOR3 vec1, vec2, vec3, vec4, vec5, vec6, vec7, vec8;
	vec1 = D3DXVECTOR3( m_pCurrentCamera->GetCurrentMatrix()->_11,  m_pCurrentCamera->GetCurrentMatrix()->_12,  m_pCurrentCamera->GetCurrentMatrix()->_13);
	vec2 = D3DXVECTOR3( m_pCurrentCamera->GetCurrentMatrix()->_21,  m_pCurrentCamera->GetCurrentMatrix()->_22,  m_pCurrentCamera->GetCurrentMatrix()->_23);
	vec3 = D3DXVECTOR3( m_pCurrentCamera->GetCurrentMatrix()->_31,  m_pCurrentCamera->GetCurrentMatrix()->_32,  m_pCurrentCamera->GetCurrentMatrix()->_33);
	vec4 = D3DXVECTOR3( m_pCurrentCamera->GetCurrentMatrix()->_41,  m_pCurrentCamera->GetCurrentMatrix()->_42,  m_pCurrentCamera->GetCurrentMatrix()->_43);
	
	vec5 = D3DXVECTOR3( m_pCurrentCamera->GetReturnPos()._11,  m_pCurrentCamera->GetReturnPos()._12,  m_pCurrentCamera->GetReturnPos()._13);
	vec6 = D3DXVECTOR3( m_pCurrentCamera->GetReturnPos()._21,  m_pCurrentCamera->GetReturnPos()._22,  m_pCurrentCamera->GetReturnPos()._23);
	vec7 = D3DXVECTOR3( m_pCurrentCamera->GetReturnPos()._31,a  m_pCurrentCamera->GetReturnPos()._32,  m_pCurrentCamera->GetReturnPos()._33);
	vec8 = D3DXVECTOR3( m_pCurrentCamera->GetReturnPos()._41,  m_pCurrentCamera->GetReturnPos()._42,  m_pCurrentCamera->GetReturnPos()._43);

	if( vec1 == vec5 )
	{
		int x = 0;
	}
	if( vec2 == vec6 )
	{
		int x = 0;
	}
	if( vec3 == vec7 )
	{
		int x = 0;
	}
	if( vec4 == vec8 )
	{
		int x = 0;
	}
	*/

	/*
	m_pCurrentCamera->GetCurrentMatrix()->_41 = m_d3dReturnMatrix._41;
	m_pCurrentCamera->GetCurrentMatrix()->_42 = m_d3dReturnMatrix._42;
	m_pCurrentCamera->GetCurrentMatrix()->_43 = m_d3dReturnMatrix._43;	
	if( m_pPlayer )
	{
		CPhysics::LookAt(m_pCurrentCamera->GetCurrentMatrix(), m_pPlayer->GetMatrix());
	}
	else
	{
		m_pCurrentCamera->SetCameraMatrix(m_d3dReturnMatrix);
	}
	*/
	if( m_pBoss )
	{
		m_pBoss->SetInCinematic(false);
	}
}

void CCameraObject::MoveToDestination(bool _bCanBeSkipped)		
{
	m_bMoveToDestination = true; 
	m_bHasFinishedCinematic = false;
	m_bCinematicEnter = true;
	m_d3dReturnMatrix = *m_pCurrentCamera->GetViewMatrix();
	m_pCurrentCamera->Shake(1,1,0.0f);
	m_bCanBeSkipped = _bCanBeSkipped;
	m_pObjectManager->RemoveLasers();
}

void CCameraObject::PlayIntroCinematic(float _fDt)
{
	if(m_nIntroCameraWaypointIter >= m_d3dIntroCameraWaypoints.size())
	{
		if(m_szIntroStage != eINTRO_PAST_FIGHTERS && m_szIntroStage != eINTRO_MOVE_TO_PLAYER)
			m_szIntroStage = eINTRO_THROUGH_METEORS;
	}
	else 
	{
		if(m_nIntroCameraWaypointIter == 4)
		{
			//yo antone
			//number one
			static int count = 0;
			float x, y, z;
			CComet *comet = (CComet*) m_pObjectManager->ReturnArmamentToMasterList(eLARGE_ASTEROID);
			if (comet)
			{
				count++;
				//cout << count << endl;
				comet->SetPosition( m_pCurrentCamera->GetCurrentMatrix()->_41,  m_pCurrentCamera->GetCurrentMatrix()->_42,  m_pCurrentCamera->GetCurrentMatrix()->_43);
				D3DXMATRIX mat;
				x = (float)(rand()%1000-500);
				y = (float)(rand()%1000-500);
				z = (float)(rand()%1000-500);
				D3DXMatrixTranslation(&mat, 300.0f + x, 300.0f + y, 1000.0f + z);
				mat = *m_pCurrentCamera->GetCurrentMatrix() * mat;
				comet->SetMatrix(&mat);

				D3DXMATRIX mat2;
				D3DXMatrixTranslation(&mat2, -300.0f + x, -100.0f + y, -1000.0f + z);
				mat2 = *m_pCurrentCamera->GetCurrentMatrix() * mat2;

				CPhysics::LookAt(comet, mat2);
			}
		}
		D3DXVECTOR3 facing = D3DXVECTOR3(m_pCurrentCamera->GetCurrentMatrix()->_31, m_pCurrentCamera->GetCurrentMatrix()->_32, m_pCurrentCamera->GetCurrentMatrix()->_33); 
		D3DXVec3Normalize(&facing, &facing);
		D3DXMATRIX mat;
		D3DXMatrixTranslation(&mat, 70.0f * facing.x * _fDt, 70.0f * facing.y * _fDt, 70.0f * facing.z * _fDt);
		*m_pCurrentCamera->GetCurrentMatrix() =  (*m_pCurrentCamera->GetCurrentMatrix()) * mat;
		CPhysics::TurnTo(m_pCurrentCamera->GetCurrentMatrix(), &CPhysics::GetMat4Vec(ePOS, &m_d3dIntroCameraWaypoints[m_nIntroCameraWaypointIter]), _fDt, 0.003f);


		D3DXVECTOR3 vecToWaypoint = CPhysics::GetMat4Vec(ePOS, m_pCurrentCamera->GetCurrentMatrix()) - CPhysics::GetMat4Vec(ePOS, &m_d3dIntroCameraWaypoints[m_nIntroCameraWaypointIter]);
		float distToWaypoint = D3DXVec3Length(&vecToWaypoint);
		if(distToWaypoint < 90.0f || m_fTimeGoingToWaypoint > 7.5f)
		{
			//std::cout << "Reached Waypoint!\n" << m_nIntroCameraWaypointIter << endl;
			m_fTimeGoingToWaypoint = 0.0f;
			m_nIntroCameraWaypointIter += 1;
		}
		else
			m_fTimeGoingToWaypoint += _fDt;
		return;
	}




	m_pPlayer->SetIsScenery(true);
	switch( m_szIntroStage )
	{
	case eINTRO_AVOID_ASTEROIDS:
		{
			AvoidAsteroids(_fDt);
		}
		break;
	case eINTRO_THROUGH_METEORS:
		{
			ThroughMeteors(_fDt);
		}
		break;
	case eINTRO_PAST_FIGHTERS:
		{
			PastFighters(_fDt);
		}
		break;
	case eINTRO_MOVE_TO_PLAYER:
		{
			GoToPlayer(_fDt);
		}
		break;
	}
}

void CCameraObject::AvoidAsteroids(float _fDt)
{	
	if( (int)(m_vd3dAsteroidDestinations.size()) <= m_nCurrentAsteroidTarget )
	{
		m_szIntroStage = eINTRO_THROUGH_METEORS;
		return;
	}

	D3DXMATRIX d3dMat;
	D3DXMatrixIdentity(&d3dMat);

	/*
	D3DXVECTOR3 d3dPos;
	d3dPos.x = m_vd3dAsteroidDestinations[m_nCurrentAsteroidTarget]._41 - m_pCurrentCamera->GetCurrentMatrix()->_41;
	d3dPos.y = m_vd3dAsteroidDestinations[m_nCurrentAsteroidTarget]._42 - m_pCurrentCamera->GetCurrentMatrix()->_42;
	d3dPos.z = m_vd3dAsteroidDestinations[m_nCurrentAsteroidTarget]._43 - m_pCurrentCamera->GetCurrentMatrix()->_43;
	*/

	float dist = m_pCurrentCamera->GetCurrentMatrix()->_43 - m_vd3dAsteroidDestinations[m_nCurrentAsteroidTarget]._43;

	static int numTimes = 0;
		
	if( dist < 250.0f )
	{		
		numTimes++;
		if( numTimes == 5 )
		{
			m_szIntroStage = eINTRO_THROUGH_METEORS;
			numTimes = 0;
			return;
		}

		SelectNewAsteroid();
	}

	//D3DXVec3Normalize(&d3dPos, &d3dPos);
	//D3DXMatrixTranslation(&d3dMat,	d3dPos.x *75.0f*_fDt, d3dPos.y *75.0f*_fDt, d3dPos.z *75.0f*_fDt);
	D3DXMatrixTranslation(&d3dMat,	0.0f, 0.0f, -100.0f*_fDt);
	
	*m_pCurrentCamera->GetCurrentMatrix() =  (*m_pCurrentCamera->GetCurrentMatrix()) * d3dMat;
//	D3DXVECTOR3 vector = D3DXVECTOR3(m_vd3dAsteroidDestinations[m_nCurrentAsteroidTarget]._41, m_vd3dAsteroidDestinations[m_nCurrentAsteroidTarget]._42, m_vd3dAsteroidDestinations[m_nCurrentAsteroidTarget]._43);
	CPhysics::TurnTo(m_pCurrentCamera->GetCurrentMatrix(), &m_vAsteroidsInScene[m_nCurrentAsteroidTarget]->GetPosition(), _fDt, 0.00075f);	
}

void CCameraObject::ThroughMeteors(float _fDt)
{
	
	static int create = 0;
	static float creationbucket = 0.0f;

	m_fIntroReturnBucket += _fDt;
	if( m_fIntroReturnBucket > 5.0f )
	{
		m_fIntroReturnBucket = 0.0f;
		m_szIntroStage = eINTRO_PAST_FIGHTERS;
		create = true;
		creationbucket = 0.0f;
	}
	
	
	creationbucket += _fDt;
	if( create < 5 && creationbucket > 0.75f)
	{
		create++;
		creationbucket = 0.0f;

		CComet *poop = (CComet*) m_pObjectManager->ReturnArmamentToMasterList(eLARGE_ASTEROID);
		if (poop)
		{
			poop->SetPosition( m_pCurrentCamera->GetCurrentMatrix()->_41,  m_pCurrentCamera->GetCurrentMatrix()->_42,  m_pCurrentCamera->GetCurrentMatrix()->_43);
			D3DXMATRIX mat;
			D3DXMatrixTranslation(&mat, 1750.0f, 1000.0f, -2000.0f);
			mat = *m_pCurrentCamera->GetCurrentMatrix() * mat;
			poop->SetMatrix(&mat);

			D3DXMATRIX mat2;
			D3DXMatrixTranslation(&mat2, -1000.0f, -1000.0f, -1500.0f);
			mat2 = *m_pCurrentCamera->GetCurrentMatrix() * mat2;

			CPhysics::LookAt(poop, mat2);
		}
	}

	D3DXVECTOR3 facing = D3DXVECTOR3(m_pCurrentCamera->GetCurrentMatrix()->_31, m_pCurrentCamera->GetCurrentMatrix()->_32, m_pCurrentCamera->GetCurrentMatrix()->_33); 
	D3DXVec3Normalize(&facing, &facing);
	D3DXMATRIX mat;
	D3DXMatrixTranslation(&mat, 150.0f * facing.x * _fDt, 150.0f * facing.y * _fDt, 150.0f * facing.z * _fDt);
	*m_pCurrentCamera->GetCurrentMatrix() =  (*m_pCurrentCamera->GetCurrentMatrix()) * mat;
	CPhysics::TurnTo(m_pCurrentCamera->GetCurrentMatrix(), &m_pPlayer->GetPosition(), _fDt, 0.00125f);
}

void CCameraObject::PastFighters(float _fDt) 
{
	if(m_bFirstPass == true)
	{
		_abb = new CAmbientBoundingBox(m_pCurrentCamera);
		D3DXMATRIX worldMat = *m_pCurrentCamera->GetCurrentMatrix();
		D3DXMATRIX d3dMat;
		D3DXMatrixIdentity(&d3dMat);
		D3DXMatrixTranslation(&d3dMat,	90.0f, 10.0f, -1800.0f);
		worldMat =  worldMat * d3dMat;
		CAmbientFighter* m_pRightSideAmbientFighter = new CAmbientFighter(worldMat, m_pCurrentCamera);
		_abb->AddFighter(m_pRightSideAmbientFighter);

		worldMat = *m_pCurrentCamera->GetCurrentMatrix();
		D3DXMatrixIdentity(&d3dMat);
		D3DXMatrixTranslation(&d3dMat,	30.0f, 30.0f, -1400.0f); 
		worldMat =  worldMat * d3dMat;
		m_pRightSideAmbientFighter = new CAmbientFighter(worldMat, m_pCurrentCamera);
		_abb->AddFighter(m_pRightSideAmbientFighter);

		worldMat = *m_pCurrentCamera->GetCurrentMatrix();
		D3DXMatrixIdentity(&d3dMat);
		D3DXMatrixTranslation(&d3dMat, 50.0f, 50.0f, -1800.0f);
		worldMat = worldMat * d3dMat;
		m_pRightSideAmbientFighter = new CAmbientFighter(worldMat, m_pCurrentCamera);
		_abb->AddFighter(m_pRightSideAmbientFighter);

		worldMat = *m_pCurrentCamera->GetCurrentMatrix();
		D3DXMatrixIdentity(&d3dMat);
		D3DXMatrixTranslation(&d3dMat, 50.0f, 25.0f, -2200.0f);
		worldMat = worldMat * d3dMat;
		m_pRightSideAmbientFighter = new CAmbientFighter(worldMat, m_pCurrentCamera);
		_abb->AddFighter(m_pRightSideAmbientFighter);

		worldMat = *m_pCurrentCamera->GetCurrentMatrix();
		D3DXMatrixIdentity(&d3dMat);
		D3DXMatrixTranslation(&d3dMat, 35.0f, 40.0f, -2600.0f);
		worldMat = worldMat * d3dMat;
		m_pRightSideAmbientFighter = new CAmbientFighter(worldMat, m_pCurrentCamera);
		_abb->AddFighter(m_pRightSideAmbientFighter);

		m_pObjectManager->AddIntroAmbientFighters(_abb);
		m_bFirstPass = false;
	}
	if(_abb)
	{
		if(_abb->IntroIsDone())
		{
			m_pObjectManager->RemoveAmbientFighters();
			if(_abb)
			{
				delete _abb;
				_abb = NULL;
			}
		}
	}
	m_szIntroStage = eINTRO_MOVE_TO_PLAYER;
	m_pPlayer->SetIsVisable(true);
}

void CCameraObject::GoToPlayer(float _fDt)
{
	static bool reachedZdist = false;
	if( reachedZdist )
	{		
		//D3DXVECTOR3 facing = D3DXVECTOR3(m_pCurrentCamera->GetCurrentMatrix()->_31, m_pCurrentCamera->GetCurrentMatrix()->_32, m_pCurrentCamera->GetCurrentMatrix()->_33); 
		//D3DXVec3Normalize(&facing, &facing);
		//D3DXMATRIX mat;
		//D3DXMatrixTranslation(&mat, 150.0f * facing.x * _fDt, 150.0f * facing.y * _fDt, 150.0f * facing.z * _fDt);
		//*m_pCurrentCamera->GetCurrentMatrix() =  (*m_pCurrentCamera->GetCurrentMatrix()) * mat;
		

		static float turnBucket = 0.0f;
		turnBucket += _fDt;
	
		if( turnBucket >= 5.0f )
		{
			turnBucket = 0.0f;
			reachedZdist = false;
			m_bIsIntroCinematic = false;	
			m_bReturning = true;
			m_szIntroStage = eINTRO_AVOID_ASTEROIDS;
			m_pPlayer->SetIsScenery(false);
			m_fWaitToReturn = 1.0f;
			m_szFadeFlag = FADE_OUT;
			m_bShouldFadeIn = true;
		}
		else if( turnBucket > 3.5f )
		{
			D3DXMATRIX mat;
			D3DXMatrixTranslation(&mat, 0.0f, 0.0f, 25.0f * _fDt);
			*m_pCurrentCamera->GetCurrentMatrix() = mat * *m_pCurrentCamera->GetCurrentMatrix();
		}
		else
		{
			CPhysics::TurnTo(m_pCurrentCamera->GetCurrentMatrix(), &m_pPlayer->GetPosition(), _fDt, 0.008f);
		}
	}
	else
	{
		D3DXMATRIX matrix;
		D3DXMatrixTranslation(&matrix, 15.0f, 0.0f, -50.0f);
		matrix = matrix * *m_pPlayer->GetMatrix();
		
		D3DXVECTOR3 facing = D3DXVECTOR3( matrix._41 - m_pCurrentCamera->GetCurrentMatrix()->_41, matrix._42 - m_pCurrentCamera->GetCurrentMatrix()->_42, matrix._43 - m_pCurrentCamera->GetCurrentMatrix()->_43 ); 
		
		float dist = D3DXVec3Length(&facing);
		float speed = 150.0f;

		if( dist < 15.0f )
		{
			reachedZdist = true;
			return;
		}
		else if( dist < 300.0f )
		{
			speed = 50.0f;
		}
		
		D3DXVec3Normalize(&facing, &facing);
		D3DXMATRIX mat;
		D3DXMatrixTranslation(&mat, speed * facing.x * _fDt, 
									speed * facing.y * _fDt, 
									speed * facing.z * _fDt);

		*m_pCurrentCamera->GetCurrentMatrix() =  (*m_pCurrentCamera->GetCurrentMatrix()) * mat;
		//CPhysics::TurnTo(m_pCurrentCamera->GetCurrentMatrix(), &CPhysics::GetMat4Vec(ePOS, m_pPlayer->GetMatrix()), _fDt, 0.001f);
		CPhysics::LookAt(m_pCurrentCamera->GetCurrentMatrix(), m_pPlayer->GetMatrix());

	}
}

void CCameraObject::AddAsteroid( CAsteroid* _roid )
{
	m_szFadeFlag = FADE_IN;
	m_fBlackoutAlpha = 1.0f;
	m_bShouldFadeIn = true;
	
	if( _roid->GetType() == eASTEROID )
	{
		return;
	}

	m_vAsteroidsInScene.push_back(_roid);
	D3DXMATRIX mat;
	D3DXMatrixTranslation(&mat, 120, 0, 0);
	m_vd3dAsteroidDestinations.push_back(*_roid->GetMatrix() * mat);

	if( m_vAsteroidsInScene.size() == 1 )
	{
		m_nCurrentAsteroidTarget = 0;
	}
	else
	{
		D3DXVECTOR3 vec2 = D3DXVECTOR3( _roid->GetPosition().x - m_pCurrentCamera->GetCurrentMatrix()->_41,
									    _roid->GetPosition().y - m_pCurrentCamera->GetCurrentMatrix()->_42,
									    _roid->GetPosition().z - m_pCurrentCamera->GetCurrentMatrix()->_43);

		D3DXVECTOR3 vec = D3DXVECTOR3( m_vAsteroidsInScene[m_nCurrentAsteroidTarget]->GetPosition().x - m_pCurrentCamera->GetCurrentMatrix()->_41,
									   m_vAsteroidsInScene[m_nCurrentAsteroidTarget]->GetPosition().y - m_pCurrentCamera->GetCurrentMatrix()->_42,
									   m_vAsteroidsInScene[m_nCurrentAsteroidTarget]->GetPosition().z - m_pCurrentCamera->GetCurrentMatrix()->_43);

		float length = D3DXVec3Length(&vec);
		float length2 = D3DXVec3Length(&vec2);

		if( length2 < length )
		{
			m_nCurrentAsteroidTarget = m_vAsteroidsInScene.size()-1;

		}

	}
}

void CCameraObject::SelectNewAsteroid()
{
	if( m_vAsteroidsInScene.size() == 1 )
	{
		m_nCurrentAsteroidTarget = 0;
	}
	else
	{
		m_vAlreadyBeen.push_back(m_nCurrentAsteroidTarget);
		int x = 0;
		for( unsigned int i(1); i < m_vAsteroidsInScene.size(); ++i )
		{
			if( i == m_nCurrentAsteroidTarget )
			{
				continue;
			}

			bool been = false;
			for( unsigned int j(0); j < m_vAlreadyBeen.size(); ++j)
			{
				if( i == m_vAlreadyBeen[j] )
				{
					been = true;
					break;
				}
			}

			if( been )
			{
				continue;
			}

			D3DXVECTOR3 vec2 = D3DXVECTOR3( m_vAsteroidsInScene[i]->GetPosition().x - m_pCurrentCamera->GetCurrentMatrix()->_41,
											m_vAsteroidsInScene[i]->GetPosition().y - m_pCurrentCamera->GetCurrentMatrix()->_42,
											m_vAsteroidsInScene[i]->GetPosition().z - m_pCurrentCamera->GetCurrentMatrix()->_43);

			D3DXVECTOR3 vec = D3DXVECTOR3( m_vAsteroidsInScene[x]->GetPosition().x - m_pCurrentCamera->GetCurrentMatrix()->_41,
										   m_vAsteroidsInScene[x]->GetPosition().y - m_pCurrentCamera->GetCurrentMatrix()->_42,
										   m_vAsteroidsInScene[x]->GetPosition().z - m_pCurrentCamera->GetCurrentMatrix()->_43);

			float length = D3DXVec3Length(&vec);
			float length2 = D3DXVec3Length(&vec2);

			if( length2 < length )
			{
				x = i;
			}
		}
		m_nCurrentAsteroidTarget = x;
	}
}
