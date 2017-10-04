/***********************************************
* Filename:  		Player.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will hold all the player entities data.
************************************************/
#include "../StdAfx.h"
#include "Player.h"
#include "../Collision/Physics.h"
#include "../Input/Input.h"
#include "../State System/Game.h"

/*****************************************************************
* CPlayer():		Default Constructor will set up his data members
* Ins:			    None    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CPlayer::CPlayer(void) : m_nHealth(100), m_nShields(100), m_nDamage(PLAYER_LASER_DAMAGE), m_fVelocityModifier(25), m_bAlive(true),
	m_fShieldRechargeRate(0), m_fHealthRechargeRate(0), m_fForwardPosition(27), m_fMaxForwardPosition(70), m_fMaxBackPosition(36),
	m_fYawDegree(0), m_fPitchDegree(0), m_fTranslateX(0), m_fTranslateY(0), m_fBufferZone(20), m_fYawReturnTime(1), m_bYawLeft(false),
	m_bYawRight(false), m_fDegreeReturnTime(1), m_bPosDegree(false), m_bNegDegree(false), m_bHasDodged(0), m_fCameraLevel(-14.0f), m_fMaxXTranslation(0),
	m_fMaxYTranslation(0), m_bMoveFaster(false), m_bPlayHullHit(false), m_bPlayShieldHit(false), m_bDisableInput(false), m_bLoadingComplete(false), m_bIsInactive(false),
	m_bForceReturn(false), m_fSubtractFromRotation(1.0f)
{
	//	Messes with the matrix
	SetBV(CCollOBJ::Create(eSPH, D3DXVECTOR3(0,0,0), 5.0));
	SetShieldBV(CCollOBJ::Create(eSPH, D3DXVECTOR3(0,0,0), 6.0));
	SetActive(true);
	m_fReturnFromInactiveBucket = 0.0f;
	m_d3dVelocity = D3DXVECTOR3(0,0,0);

	m_bHealthStateChanged = false;
	m_bShouldWarpOut = false;
	m_bShieldStateChanged = false;

	m_bFinalCinematic = false;

	startMovingL = false;
	startMovingR = false;
	doOnceR = true;
	doOnceL = true;

	m_fModeChangeTimer = 1.1f;
	m_cCruiseControl = 0;

	for(int eachSide = 0; eachSide < 4; eachSide++)
	{
		m_arrfHurtRecentTimer[eachSide] = 1.0f;
	}

	SetType(eRED_PLAYER); //Twas only temporary
	SetPosition(0.0f, 0.0f, 0.0f);

	//WwiseNS::SoundEvent(ePlayerIdle, ePlay);

	EffectInformation::CreateEffect(ePLAYER_IDLE, &m_ESPlayerIdle, this, true, true);
	EffectInformation::CreateEffect(ePLAYER_CHANGE_BLUE, &m_ESPlayerModeChange, this, false, true);
	EffectInformation::CreateEffect(ePLAYER_CHANGE_RED, &m_ESPlayerModeChange, this, false, true);
	EffectInformation::CreateEffect(ePLAYER_THRUSTERS, &m_ESPlayerThruster, this, true, false);
	EffectInformation::CreateEffect(ePLAYER_DAMAGED, &m_ESPlayerDamaged, this, true, false);
	EffectInformation::CreateEffect(ePLAYER_MILDLY_DAMAGED, &m_ESPlayerMildlyDamaged, this, true, true);
	//EffectInformation::CreateEffect(ePLAYER_HEAVILY_DAMAGED, &m_ESPlayerHeavilyDamaged, this, true, true);
	//EffectInformation::CreateEffect(ePLAYER_JET_STREAMERS, &m_ESPlayerJetStreamers, this, true, true);

	m_ESPlayerMildlyDamaged.SetPlay(false);



	SetEntityMass(PLAYER_MASS);


	////////////////////////////////////////
	m_bHasPlayedCriticalHealthSound = false;
	m_bHasPlayedLossOfShieldSound = false;
	m_bHasPlayedPlayerDeath = false;
	m_bHasPlayedSafetyEjection = false;

	m_fHealthAlarmDelay = 1.0f;
	m_fShieldAlarmDelay = 1.0f;
}
/*****************************************************************
* CPlayer():		overload Constructor will set up his data members
* Ins:			    _health, _shields, _damage, _velMod  		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CPlayer::CPlayer(int _nHealth, int _nShields, int _nDamage, float _fVelMod) 
	: m_nHealth(_nHealth), m_nShields(_nShields), m_nDamage(_nDamage), m_fVelocityModifier(_fVelMod), m_fForwardPosition(23), m_fMaxForwardPosition(28), m_fMaxBackPosition(18), m_bMoveFaster(false),
	m_bPlayHullHit(false), m_bPlayShieldHit(false), m_bDisableInput(false), m_bIsInactive(false)
{
	SetBV(CCollOBJ::Create(eSPH, D3DXVECTOR3(0,0,0), 5.0));
	SetShieldBV(CCollOBJ::Create(eSPH, D3DXVECTOR3(0,0,0), 6.0));
	m_fReturnFromInactiveBucket = 0.0f;
	m_d3dVelocity = D3DXVECTOR3(0,0,0);

	for(int eachSide = 0; eachSide < 4; eachSide++)
	{
		m_arrfHurtRecentTimer[eachSide] = 1.0f;
	}

	m_bHealthStateChanged = false;
	m_bShouldWarpOut = false;
	m_bShieldStateChanged = false;

	m_fModeChangeTimer = 1.1f;
	m_cCruiseControl = 0;
	SetEntityMass(PLAYER_MASS);
	//m_ESPlayerIdle.SetPlay(true);
	////////////////////////////////////////
	m_bHasPlayedCriticalHealthSound = false;
	m_bHasPlayedLossOfShieldSound = false;
	m_bHasPlayedPlayerDeath = false;
	m_bHasPlayedSafetyEjection = false;

	m_fHealthAlarmDelay = 1.0f;
	m_fShieldAlarmDelay = 1.0f;
	m_bFinalCinematic = false;
}
/*Destructor*/
CPlayer::~CPlayer(void)
{
	if(m_pExtraBV != NULL)
	{
		delete m_pExtraBV;
	}
	m_pExtraBV = NULL;
}

void CPlayer::SetDestination(D3DXVECTOR3 vec)
{
	m_vDestination = vec;
}

/*****************************************************************
* Update():		Will update its respective data, shields, and health.
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CPlayer::Update(float _fElapsedTime)
{
	if( m_pCamera->GetInWinScreen() )
	{				
		D3DXMATRIX mat;
		D3DXMatrixTranslation(&mat, 0.0f, -20.0f, 75.0f);
		m_d3dMatrix = mat * *m_pCamera->GetCurrentMatrix();
		return;
	}
	else if( m_bFinalCinematic )
	{
		D3DXVECTOR3 to = D3DXVECTOR3(GetPosition().x - m_vDestination.x, 
									 GetPosition().y - m_vDestination.y, 
									 GetPosition().z - m_vDestination.z);

		if( D3DXVec3Length(&to) < 10.0f )
		{
			m_bShouldWarpOut = true;
			m_bFinalCinematic = false;
			return;
		}

		CPhysics::LookAt(this, m_vDestination);
		D3DXMATRIX mat;
		D3DXMatrixTranslation(&mat, 0.0f, 0.0f, 150.0f * _fElapsedTime);

		m_d3dMatrix = mat * m_d3dMatrix;
		return;
	}

	if(m_fModeChangeTimer <= 1.0f)
	{
		m_fModeChangeTimer += _fElapsedTime;
		m_ESPlayerModeChange.ResetPosition();
	}
	
#ifdef _DEBUG
	if(CInput::DIKBufferedButtonCheck(DIK_B))
		m_nHealth = 0;
#endif

	for(int eachSide = 0; eachSide < 4; eachSide++)
	{
		m_arrfHurtRecentTimer[eachSide] += _fElapsedTime;
	}

	CEntity::Update(_fElapsedTime);
	
	if(m_bIsInactive)// || m_fReturnFromInactiveBucket > 0.0f) 
	{ 
		//m_fReturnFromInactiveBucket -= _fElapsedTime;
		return; 
	} 
	else if(GetType() == eRED_PLAYER) 
	{ 
		if(GetTransitionValue() >= -1.0f) 
			SetTransitionValue((GetTransitionValue()) - (_fElapsedTime * 2.0f)); 
		else 
			SetTransitionValue(-1.0f); 
	} 
	else if(GetType() == eBLUE_PLAYER) 
	{ 
		if(GetTransitionValue() <= 1.0f) 
			SetTransitionValue((GetTransitionValue()) + (_fElapsedTime * 2.0f)); 
		else 
			SetTransitionValue(1.0f); 
	}

	if(m_nHealth > 100)
	{
		WwiseNS::SoundEvent(ePlayerHealthFull, ePlay);
		m_nHealth = 100;
	}
	else if(m_nHealth < 0)
	{
		m_nHealth = 0;
		WwiseNS::SetPreventPlaying(true);
	}

	float fPlayerHealthRatio = (float)m_nHealth / PLAYER_HEALTH;

	
		//m_ESPlayerDamaged.SetEmitterRate(fPlayerHealthRatio / 10);
		//m_ESPlayerDamaged.SetPlay(true);
		m_ESPlayerDamaged.ResetPosition();

		if(fPlayerHealthRatio < 0.5f)
		{
			m_ESPlayerMildlyDamaged.SetEmitterRate(fPlayerHealthRatio / 100);
			m_ESPlayerMildlyDamaged.SetPlay(true);
			//m_ESPlayerMildlyDamaged.ResetPosition();
		}
		else
		{
			m_ESPlayerMildlyDamaged.SetPlay(false);
		}


	//m_ESPlayerMildlyDamaged.SetPlay(true);
	//m_ESPlayerMildlyDamaged.ResetPosition();

	//
	//m_ESPlayerHeavilyDamaged.SetPlay(true);
	//m_ESPlayerMildlyDamaged.ResetPosition();

	//if(m_nHealth < 99 && m_nHealth > 0	&& m_ESPlayerDamaged.GetPlay() == false)
	//{
	//	m_ESPlayerDamaged.SetPlay(true);
	//	m_ESPlayerDamaged.ResetPosition();
	//}
	//else if(m_nHealth >= 99)
	//{
	//	m_ESPlayerDamaged.SetPlay(false);
	//}
	//
	//if(m_nHealth < 33 && m_nHealth > 0 && m_ESPlayerMildlyDamaged.GetPlay() == false)
	//{
	//	WwiseNS::SoundEvent(ePlayerLowHealthWarning, ePlay);
	//	m_ESPlayerExtremelyDamaged.SetPlay(true);
	//	m_ESPlayerExtremelyDamaged.ResetPosition();
	//}
	//else if(m_nHealth >= 33 && m_ESPlayerExtremelyDamaged.GetPlay() == true)
	//{
	//	m_ESPlayerExtremelyDamaged.SetPlay(false);
	//	WwiseNS::SoundEvent(ePlayerLowHealthWarning, eStop);
	//}

	if(m_nHealth <= 0)
	{
		WwiseNS::SetPreventPlaying(true);
		m_nHealth = 0;
	}

	if(m_nShields > 200)
	{
		m_nShields = 200;
	}

	m_fHealthAlarmDelay += _fElapsedTime;
	m_fShieldAlarmDelay += _fElapsedTime;
	///////////////////////////////////////////////////////////////////////
	//Checks to determine if the player needs to be audibly alerted 
	if(m_nShields < 0)
	{
		WwiseNS::SoundEvent(ePlayerShieldBroken, ePlay);
		m_nShields = 0;
		
	}
	//else if(m_nShields <= 50 && m_nShields > 0 && m_fShieldAlarmDelay > 0.5f)
	//{  
	//	WwiseNS::SoundEvent(ePlayerLowShields, ePlay);
	//	m_fShieldAlarmDelay = 0.0f;
	//}
	if(m_bHasPlayedLossOfShieldSound == false && m_nShields == 0)
	{
		WwiseNS::StopAllTutorialVoices(eAIPlayerShieldsDown);
		WwiseNS::SoundEvent(eAIPlayerShieldsDown, ePlay);
		m_bHasPlayedLossOfShieldSound = true;
	}
	if(m_nShields >= 100)
	{
		m_bHasPlayedLossOfShieldSound = false;
	}

	if(m_nHealth <= 50 && m_bHasPlayedCriticalHealthSound == false)
	{
		WwiseNS::SoundEvent(eAIPlayerShieldsDown, eStop);
		WwiseNS::SoundEvent(eAIPlayerCriticalHealth, ePlay);
		m_bHasPlayedCriticalHealthSound = true;
	}
	if(m_nHealth <= 50 && m_nHealth > 0 && m_fHealthAlarmDelay > 1.4f)
	{  
		WwiseNS::SoundEvent(ePlayerLowHealthWarning, ePlay);
		m_fHealthAlarmDelay = 0.0f;
	}
	if(m_nHealth >= 100)
	{
		m_bHasPlayedCriticalHealthSound = false;
	}
	///////////////////////////////////////////////////////////////////////



	// Determine the players position on the screen
	D3DXMATRIX d3dNewPlayerPos(*m_pCamera->GetViewMatrix());
	D3DXMATRIX d3dYawRotate;
	D3DXMATRIX d3dPitchRotate;
	D3DXMATRIX d3dTranslate;
	D3DXMatrixIdentity(&d3dYawRotate);
	D3DXMatrixIdentity(&d3dPitchRotate);
	D3DXMatrixIdentity(&d3dTranslate);

	if(!m_bAlive)
	{
		m_pCamera->SetWaitForGame(true);
	}

	D3DXVECTOR3 d3dLocalZ(m_pCamera->GetViewMatrix(true)->_31, m_pCamera->GetViewMatrix(true)->_32,m_pCamera->GetViewMatrix(true)->_33);
	D3DXVec3Normalize(&d3dLocalZ, &d3dLocalZ);

	D3DXVECTOR3 _d3dLocalY(m_pCamera->GetViewMatrix(true)->_21, m_pCamera->GetViewMatrix(true)->_22, m_pCamera->GetViewMatrix(true)->_23);
	D3DXVec3Normalize(&_d3dLocalY, &_d3dLocalY);

	D3DXVECTOR3 d3dLocalX(m_pCamera->GetViewMatrix(true)->_11, m_pCamera->GetViewMatrix(true)->_12, m_pCamera->GetViewMatrix(true)->_13);
	D3DXVec3Normalize(&d3dLocalX, &d3dLocalX);

	if(abs(D3DXVec3Dot(&m_d3dVelocity, &m_d3dVelocity)) > EPSILON * 4)
	{
		float fTmpLen = abs(D3DXVec3Length(&m_d3dVelocity));

		if(m_d3dVelocity.x > EPSILON*2)
		{
			m_d3dVelocity.x = max(0, m_d3dVelocity.x - fTmpLen * _fElapsedTime);
		}
		else if(m_d3dVelocity.x < -EPSILON*2)
		{
			m_d3dVelocity.x = min(0, m_d3dVelocity.x + fTmpLen * _fElapsedTime);
		}

		if(m_d3dVelocity.y > EPSILON*2)
		{
			m_d3dVelocity.y = max(0, m_d3dVelocity.y - fTmpLen * _fElapsedTime);
		}
		else if(m_d3dVelocity.y < -EPSILON*2)
		{
			m_d3dVelocity.y = min(0, m_d3dVelocity.y + fTmpLen * _fElapsedTime);
		}

		if(m_d3dVelocity.z > EPSILON*2)
		{
			m_d3dVelocity.z = max(0, m_d3dVelocity.z - fTmpLen * _fElapsedTime);
		}
		else if(m_d3dVelocity.z < -EPSILON*2)
		{
			m_d3dVelocity.z = min(0, m_d3dVelocity.z + fTmpLen * _fElapsedTime);
		}

		d3dNewPlayerPos._41 += d3dLocalZ.x * GetVelocityModifier() * _fElapsedTime + (m_d3dVelocity.x * _fElapsedTime); //(d3dLocalZ.x * m_d3dVelocity.z * _fElapsedTime) + (_d3dLocalY.x * m_d3dVelocity.y * _fElapsedTime) + (d3dLocalX.x * m_d3dVelocity.x * _fElapsedTime);
		d3dNewPlayerPos._42 += d3dLocalZ.y * GetVelocityModifier() * _fElapsedTime + (m_d3dVelocity.y * _fElapsedTime);//(d3dLocalZ.y * m_d3dVelocity.z * _fElapsedTime) + (_d3dLocalY.y * m_d3dVelocity.y * _fElapsedTime) + (d3dLocalX.y * m_d3dVelocity.x * _fElapsedTime);
		d3dNewPlayerPos._43 += d3dLocalZ.z * GetVelocityModifier() * _fElapsedTime + (m_d3dVelocity.z * _fElapsedTime);//(d3dLocalZ.z * m_d3dVelocity.z * _fElapsedTime) + (_d3dLocalY.z * m_d3dVelocity.y * _fElapsedTime) + (d3dLocalX.z * m_d3dVelocity.x * _fElapsedTime);
	}
	else
	{
		d3dNewPlayerPos._41 += d3dLocalZ.x * GetVelocityModifier() * _fElapsedTime ;
		d3dNewPlayerPos._42 += d3dLocalZ.y * GetVelocityModifier() * _fElapsedTime ;
		d3dNewPlayerPos._43 += d3dLocalZ.z * GetVelocityModifier() * _fElapsedTime ;
	}

	if(CInput::GetHasDodge() == false)
	{
		if (startMovingR)
		{
			if (m_fYawDegree > -360)
			{
				d3dNewPlayerPos._41 += (d3dLocalX.x * 75.0f) * _fElapsedTime;
				d3dNewPlayerPos._42 += (d3dLocalX.y * 75.0f) * _fElapsedTime;
				d3dNewPlayerPos._43 += (d3dLocalX.z * 75.0f) * _fElapsedTime;
			}
		}

		if (startMovingL)
		{
			if (m_fYawDegree < 360)
			{
				d3dNewPlayerPos._41 += -(d3dLocalX.x * 75.0f) * _fElapsedTime;
				d3dNewPlayerPos._42 += -(d3dLocalX.y * 75.0f) * _fElapsedTime;
				d3dNewPlayerPos._43 += -(d3dLocalX.z * 75.0f) * _fElapsedTime;
			}
		}
	}

	m_pCamera->SetViewMatrix(&d3dNewPlayerPos);

	//When you press W and thrusters particle is not playeing, 
	if(CInput::BufferedButtonPress(eUp, false) && m_ESPlayerThruster.GetPlay() == false)
	{
		m_ESPlayerThruster.SetPlay(true);
		m_ESPlayerThruster.ResetPosition();
	}
	//When you let go of W and thrusters particles are playeing, 
	else if(CInput::BufferedButtonPress(eUp, true) && m_ESPlayerThruster.GetPlay() == true)
	{
		m_ESPlayerThruster.SetPlay(false);	
	}


	if (GetType() == eRED_PLAYER)
	{
		if (m_fVelocityModifier > RED_STATE_MAX_VELOCITY )
		{
			m_fVelocityModifier -= _fElapsedTime * (float)(m_fVelocityModifier - RED_STATE_MAX_VELOCITY);
			WwiseNS::SetVolumeRTPC(AK::GAME_PARAMETERS::SHIPSPEED, 100);
		}
		else
		{
			float tmpSpeed = (m_fVelocityModifier/(float)RED_STATE_MAX_VELOCITY) * 100.0f;
			if(tmpSpeed > 100.0f)
				tmpSpeed = 100.0f;
			WwiseNS::SetVolumeRTPC(AK::GAME_PARAMETERS::SHIPSPEED, tmpSpeed);
		}
	}
	else if(GetType() == eBLUE_PLAYER)
	{
		if (m_fVelocityModifier > BLUE_STATE_MAX_VELOCITY )
		{
			m_fVelocityModifier -= _fElapsedTime * (float)(m_fVelocityModifier - BLUE_STATE_MAX_VELOCITY);
			WwiseNS::SetVolumeRTPC(AK::GAME_PARAMETERS::SHIPSPEED, 100);
		}
		else
		{
			float tmpSpeed = (m_fVelocityModifier/(float)BLUE_STATE_MAX_VELOCITY) * 100.0f;
			if(tmpSpeed > 100.0f)
				tmpSpeed = 100.0f;
			WwiseNS::SetVolumeRTPC(AK::GAME_PARAMETERS::SHIPSPEED, tmpSpeed);
		}
	}
	m_fForwardPosition = m_fVelocityModifier / 1.5f;

	if(m_fForwardPosition >= m_fMaxForwardPosition)
	{
		m_fForwardPosition = m_fMaxForwardPosition;
	}
	if(m_fForwardPosition <= m_fMaxBackPosition)
	{
		m_fForwardPosition = m_fMaxBackPosition;
	}


	d3dNewPlayerPos._41 += (_d3dLocalY.x * m_fCameraLevel) + (d3dLocalZ.x * m_fForwardPosition);
	d3dNewPlayerPos._42 += (_d3dLocalY.y * m_fCameraLevel) + (d3dLocalZ.y * m_fForwardPosition);
	d3dNewPlayerPos._43 += (_d3dLocalY.z * m_fCameraLevel) + (d3dLocalZ.z * m_fForwardPosition);

	if (m_bAlive)
	{

		if(m_nHealth <= 0)
		{
			m_bAlive = false;
			//m_bActive = false;
		}

		if(m_bShieldVisible)
		{
			if(GetShields() <= 0)
			{
				m_bShieldVisible = false;
				//			SetMesh(m_nInvisibleShieldMesh);
			}
		}
		else
		{
			if(GetShields() > 0)
			{
				m_bShieldVisible = true;
				//			SetMesh(m_nVisibleShieldMesh);
			}
		}

		// Store the mouse change in variables for optimization
		float fMouseChangeX = CInput::GetMouseChangeX();
		float fMouseChangeY = CInput::GetMouseChangeY();

		if(m_pCamera->GetForceReturn() == false)
		{
			if(m_fMaxXTranslation >= -15.0f && m_fMaxXTranslation <= 15.0f)
			{
				if(fMouseChangeX > -(m_pCamera->GetWidth() / 2) && fMouseChangeX < (m_pCamera->GetWidth() / 2))
				{
					if(m_bHasDodged == 0)
					{
						m_fMaxXTranslation = -((fMouseChangeX * 14.9f) / (m_pCamera->GetWidth()/ 2));
						//m_fYawDegree = -((fMouseChangeX * 45.0f) / (g_fWINDOW_WIDTH / 2));
					}
				}

			}

			if(m_fMaxXTranslation > 15.0f)
			{
				m_fMaxXTranslation = 15.0f;
			}

			if(m_fMaxXTranslation < -15.0f)
			{
				m_fMaxXTranslation = -15.0f;
			}

			if(m_fMaxYTranslation >= -2.0f && m_fMaxYTranslation <= 5.0f)
			{
				if(fMouseChangeY > -(m_pCamera->GetHeight() / 2) && fMouseChangeY < (m_pCamera->GetHeight() / 2))
				{
					if(m_bHasDodged == 0)
					{
						m_fMaxYTranslation = ((fMouseChangeY * 5.0f) / (m_pCamera->GetHeight() / 2));
						//m_fYawDegree = -((fMouseChangeX * 45.0f) / (g_fWINDOW_WIDTH / 2));
					}
				}
			}

			if(m_fMaxYTranslation > 5.0f)
			{
				m_fMaxYTranslation = 5.0f;
			}

			if(m_fMaxYTranslation < -2.0f)
			{
				m_fMaxYTranslation = -2.0f;
			}

			static float s_fCurrentYawDegree = 0.0f;
			static float sfRollTimer = 0.0f;
			sfRollTimer += _fElapsedTime;

			if(m_bHasDodged == 0 &&  sfRollTimer >= 1.0f  && GetType() != eBLUE_PLAYER && m_pCamera->GetWaitForGame() == false)
			{
				if(CInput::BufferedButtonPress(eRight))
				{
					WwiseNS::SoundEvent(ePlayerDodge, ePlay);
					s_fCurrentYawDegree = m_fYawDegree;
					startMovingR = true;
					m_bHasDodged = DODGE_RIGHT;
					sfRollTimer = 0.0f;
				}
				else if(CInput::BufferedButtonPress(eLeft))
				{
					WwiseNS::SoundEvent(ePlayerDodge, ePlay);
					s_fCurrentYawDegree = m_fYawDegree;
					startMovingL = true;
					m_bHasDodged = DODGE_LEFT;
					sfRollTimer = 0.0f;
				}

			}

			if (startMovingR)
			{
				if (m_fYawDegree > s_fCurrentYawDegree - 360)
				{
					if(m_fYawDegree > (s_fCurrentYawDegree - 360) + 270)
						m_fYawDegree -= 900.0f * _fElapsedTime; //-DODGE_ROTATION_RATE;
					else if(m_fYawDegree > (s_fCurrentYawDegree - 360) + 180)
						m_fYawDegree -= 720.0f * _fElapsedTime; //-DODGE_ROTATION_RATE;
					else if(m_fYawDegree > (s_fCurrentYawDegree - 360) + 90)
						m_fYawDegree -= 540.0f * _fElapsedTime; //-DODGE_ROTATION_RATE;
					else
						m_fYawDegree -= 360.0f * _fElapsedTime; //-DODGE_ROTATION_RATE;

					//m_fSubtractFromRotation = ((m_fYawDegree * 1.0f) / (s_fCurrentYawDegree - 360));
				}
				else
				{
					m_fYawDegree = s_fCurrentYawDegree;
					doOnceR = true;
					startMovingR = false;
					m_bHasDodged = 0;
					m_fSubtractFromRotation = 1.0f;

				}
			}
			if (startMovingL)
			{
				if (m_fYawDegree < s_fCurrentYawDegree + 360)
				{
					if(m_fYawDegree < (s_fCurrentYawDegree + 360) - 270)
						m_fYawDegree += 900.0f * _fElapsedTime; //-DODGE_ROTATION_RATE;
					else if(m_fYawDegree < (s_fCurrentYawDegree + 360) - 180)
						m_fYawDegree += 720.0f * _fElapsedTime; //-DODGE_ROTATION_RATE;
					else if(m_fYawDegree < (s_fCurrentYawDegree + 360) - 90)
						m_fYawDegree += 540.0f * _fElapsedTime; //-DODGE_ROTATION_RATE;
					else
						m_fYawDegree += 360.0f * _fElapsedTime; //-DODGE_ROTATION_RATE;


					//m_fSubtractFromRotation;
				}
				else
				{
					m_fYawDegree = s_fCurrentYawDegree;
					doOnceL = true;
					startMovingL = false;
					m_bHasDodged = 0;
					m_fSubtractFromRotation = 1.0f;
				}

			}
		}
	}

	if(m_pCamera->GetForceReturn() == false && m_pCamera->GetWaitForGame() == false)
	{
		if(startMovingL == false && startMovingR == false && m_pCamera->GetInDeadZone() == false)
		{

			if(m_fTranslateX < m_fMaxXTranslation)
			{
				m_fTranslateX += 10.0f * _fElapsedTime;
				m_fYawDegree += 35.0f * _fElapsedTime;
			}
			else if(m_fTranslateX > m_fMaxXTranslation)
			{
				m_fTranslateX -= 10.0f * _fElapsedTime;
				m_fYawDegree -= 35.0f * _fElapsedTime;
			}

			if(m_fTranslateX > m_fMaxXTranslation && m_fTranslateX > 0.0f)
			{
				m_fTranslateX -= 10.0f * _fElapsedTime;
				m_fYawDegree -= 35.0f * _fElapsedTime;
			}
			else if(m_fTranslateX < m_fMaxXTranslation && m_fTranslateX < 0.0f)
			{
				m_fTranslateX += 10.0f * _fElapsedTime;
				m_fYawDegree += 35.0f * _fElapsedTime;
			}

			if(m_fTranslateY < m_fMaxYTranslation)
			{
				m_fTranslateY += 5.0f * _fElapsedTime;
				m_fPitchDegree += 7.0f * _fElapsedTime;
			}
			else if(m_fTranslateY > m_fMaxYTranslation)
			{
				m_fTranslateY -= 5.0f * _fElapsedTime;
				m_fPitchDegree -= 7.0f * _fElapsedTime;
			}

			if(m_fTranslateY < m_fMaxYTranslation && m_fTranslateY < 0.0f)
			{
				m_fTranslateY += 5.0f * _fElapsedTime;
				m_fPitchDegree += 7.0f * _fElapsedTime;
			}
			else if(m_fTranslateY > m_fMaxYTranslation && m_fTranslateY > 0.0f)
			{
				m_fTranslateY -= 5.0f * _fElapsedTime;
				m_fPitchDegree -= 7.0f * _fElapsedTime;
			}

			//if(m_fTranslateX > -1.5f && m_fTranslateX < 1.5)
			//{
			//	if(m_fYawDegree > 2.0f)
			//	{
			//		m_fYawDegree -= 35.0f * _fElapsedTime;
			//	}
			//	else if(m_fYawDegree < -2.0f)
			//	{
			//		m_fYawDegree += 35.0f * _fElapsedTime;
			//	}
			//}
		}

		if(m_pCamera->GetInDeadZone() == true)
		{
			if(m_fTranslateX > 0.3f)
			{
				m_fTranslateX -= 1.0f * _fElapsedTime;
			}
			else if(m_fTranslateX < -0.3f)
			{
				m_fTranslateX += 1.0f * _fElapsedTime;
			}

			if(m_fTranslateY > 0.3f)
			{
				m_fTranslateY -= 1.0f * _fElapsedTime;
			}
			else if(m_fTranslateY < -0.3f)
			{
				m_fTranslateY += 1.0f * _fElapsedTime;
			}

			if(m_fYawDegree > 0.3f)
			{
				m_fYawDegree -= 5.0f * _fElapsedTime;
			}
			else if(m_fYawDegree < -0.3f)
			{
				m_fYawDegree += 5.0f * _fElapsedTime;
			}
		}
	}

	if(m_bForceReturn == true)
	{
		if(m_fYawDegree < 360)
		{
			m_fYawDegree += (2.0f * m_fVelocityModifier) * _fElapsedTime;
		}

		if(m_fYawDegree > 360)
		{
			m_bForceReturn = false;
		}
	}
	else if(m_bForceReturn == false && m_pCamera->GetForceReturn() == true)
	{
		m_fYawDegree = 0;
	}


	D3DXMatrixTranslation(&d3dTranslate, m_fTranslateX, m_fTranslateY, 0);

	if (m_bAlive)
	{
		D3DXMatrixRotationZ(&d3dYawRotate, D3DXToRadian(m_fYawDegree));
		D3DXMatrixRotationX(&d3dPitchRotate, D3DXToRadian(m_fPitchDegree));
	}

	// Set the players matrix
	SetMatrix(&((d3dYawRotate * d3dPitchRotate) * d3dTranslate * d3dNewPlayerPos));



	static float fShieldTimer = .5f;
	static float fHullTimer = .5f;

	fShieldTimer += _fElapsedTime;
	fHullTimer += _fElapsedTime;

	if(m_bPlayShieldHit && fShieldTimer > 1.0f)
	{
		WwiseNS::SoundEvent(ePlayerShieldHit, ePlay);
		m_bPlayShieldHit = false;
		fShieldTimer = 0;
	}
	if(m_bPlayHullHit && fHullTimer > 0.25f)
	{
		WwiseNS::SoundEvent(ePlayerHullHit, ePlay);
		m_bPlayHullHit = false;
		fHullTimer = 0;
	}
	//cout << 70+(GetVelocityModifier()/10)-1.5f << endl;
	//GetCamera()->SetFoV(D3DXToRadian(65.0f+(GetVelocityModifier()/5.0f)-1.5f));

	//}
}
/*****************************************************************
* Render():		Draw effects or whatnot.
* Ins:			    None    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CPlayer::Render() 
{

}

CCollOBJ* CPlayer::GetBV()
{
	if(m_nShields) 
	{
		return m_pExtraBV;
	}

	return m_pBV;
}


void CPlayer::StopAllEffects()
{
	WwiseNS::SoundEvent(ePlayerThrust, eStop);
	WwiseNS::SoundEvent(ePlayerIdle, eStop);
	m_ESPlayerModeChange.SetPlay(false);
	m_ESPlayerIdle.SetPlay(false);
	m_ESPlayerThruster.SetPlay(false);
	m_ESPlayerDamaged.SetPlay(false);
	m_ESPlayerMildlyDamaged.SetPlay(false);
	m_ESPlayerHeavilyDamaged.SetPlay(false);
	//m_ESPlayerJetStreamers.SetPlay(false);
}

void CPlayer::GetDepartEffect()
{
	EffectInformation::CreateEffect(eRED_ENEMY_ARRIVAL, &m_esPlayerDepart, this, false, true);
}

void CPlayer::ChangeModeEffect(bool ToBlue)
{
	m_fModeChangeTimer = 0.0f;
	if(ToBlue)
	{
		EffectInformation::CreateEffect(ePLAYER_CHANGE_BLUE, &m_ESPlayerModeChange, this, false, true);
	}
	else
	{
		EffectInformation::CreateEffect(ePLAYER_CHANGE_RED, &m_ESPlayerModeChange, this, false, true);
	}
}

void CPlayer::SetInactive(bool _bIsInactive)
{
	m_bIsInactive = _bIsInactive;
	if( !_bIsInactive )
	{
 		WwiseNS::SoundEvent(ePlayerIdle, ePlay);
		m_ESPlayerIdle.Init();
		m_ESPlayerModeChange.Init();
		m_ESPlayerThruster.Init();
		m_ESPlayerDamaged.Init();
		m_ESPlayerMildlyDamaged.Init();
		m_ESPlayerHeavilyDamaged.Init();
		//m_ESPlayerJetStreamers.Init();

		m_ESPlayerIdle.ResetPosition();
		m_ESPlayerModeChange.ResetPosition();
		m_ESPlayerThruster.ResetPosition();
		m_ESPlayerDamaged.ResetPosition();
		m_ESPlayerMildlyDamaged.ResetPosition();
		m_ESPlayerHeavilyDamaged.ResetPosition();
		//m_ESPlayerJetStreamers.ResetPosition();
	}
	else
	{
		WwiseNS::SoundEvent(ePlayerIdle, eStop);
	}
}

int CPlayer::CalculateDamageDirection(D3DXVECTOR3 _HarmfulSubstance)
{
	D3DXVECTOR3 LocalX(m_d3dMatrix._11, m_d3dMatrix._12, m_d3dMatrix._13);
	D3DXVECTOR3 LocalZ(m_d3dMatrix._31, m_d3dMatrix._32, m_d3dMatrix._33);

	float SideX = D3DXVec3Dot(&LocalX, &(_HarmfulSubstance - GetPosition()));
	float SideZ = D3DXVec3Dot(&LocalZ, &(_HarmfulSubstance - GetPosition()));

	//if the side is closer than the front or back
	if(abs(SideX) > abs(SideZ))
	{
		// On the right
		if(SideX > 0.0f)
		{
			return 3;
		}
		else
		{
			return 2;
		}
	}

	else // if the front and back collision is closer than the sides
	{
		if(SideZ > 0.0f)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}

	return -1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Mutator
//////////////////////////////////////////////////////////////////////////////////////////////////
//modified so that if the sheilds were to be depleted from an outside source, it would still play the shield break sound

void CPlayer::SetHealth(int _nHealth, int DamageDirection, CGame* _pGame)
{
	if(m_nHealth > _nHealth)
	{
		if(_pGame != NULL)
			_pGame->GetCurrentStats()->IncreaseDamageTaken(m_nHealth - _nHealth);

		if(DamageDirection >= 0 && DamageDirection < 4)
		{
			m_nDamageDirction = DamageDirection;
			m_arrfHurtRecentTimer[DamageDirection] = 0.0f;
		}
	}

	m_nHealth = _nHealth; 
}

void CPlayer::SetShields(int _nShields, int DamageDirection, CGame* _pGame)
{ 
	if(m_nShields > _nShields)
	{
		if(DamageDirection >= 0 && DamageDirection < 4)
		{
			if((GetType() == eRED_PLAYER && m_nShields < RED_STATE_SHIELD) 
				|| (GetType() == eBLUE_PLAYER && m_nShields < BLUE_STATE_SHIELD))
			{
				m_nDamageDirction = DamageDirection;
				m_arrfHurtRecentTimer[DamageDirection] = 0.0f;
				if(_pGame != NULL)
					_pGame->GetCurrentStats()->IncreaseDamageTaken(m_nShields - _nShields);
			}
		}
	}

	if(m_bShieldStateChanged == false && _nShields < 33)
	{
		m_bShieldStateChanged = true;
		WwiseNS::SoundEvent(ePlayerLowShields, ePlay);
	}

	if(m_bShieldStateChanged == true && _nShields > 33)
	{
		m_bShieldStateChanged = false;
	}
	if(_nShields == 0 && m_nShields > 0)
	{		
		WwiseNS::SoundEvent(ePlayerShieldBroken, ePlay);
	}
	m_nShields = _nShields; 
}