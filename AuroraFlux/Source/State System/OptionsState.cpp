/***********************************************
* Filename:  		COptionsState.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AG
* Author:    		Andrew Goossens
* Purpose:   		This will display the options screen
*						and allow the player to change game settings
*					
************************************************/
#include "../StdAfx.h"
#include "MainMenuState.h"
#include "CreditsState.h"
#include "OptionsState.h"
#include "ConfirmationState.h"
#include "GamePlayState.h"

/*****************************************************************
* COptionsState():  Default Constructor. Will set its type and behaviors.
* Ins:			    None    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
COptionsState::COptionsState(void)
{
}

COptionsState::COptionsState(CGame * _pGame, int _nPrevState)
{
	m_pGame =  _pGame;
	m_nPrevState = _nPrevState;

	m_fDefaultVolume = DEFAULT_VOLUME;
	m_fDefaultSFX = DEFAULT_SFX_VOLUME;
	m_fDefaultGamma = 0.0f;
	m_fDefaultYaw = CAMERA_YAW_SPEED;
	m_fDefaultPitch = CAMERA_PITCH_SPEED;
	m_bDefaultInvertY = false;
	m_bDefaultInvertX = false;	
	m_nDefaultResolution = eOP_1024x768;

	m_nToWhichState = -1;

	m_bApplyInvertX = false;
	m_bApplyInvertY = false;

	m_bMouseHasPriority = false;
	m_nPrevMouseXPos = 0;
	m_nPrevMouseYPos = 0;
}

/*****************************************************************
* ~COptionsState():  Virtual destructor used when shutting down the options state
* Ins:			    none    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
COptionsState::~COptionsState(void)
{
}

/*****************************************************************
* Enter(): Enters the options state. Will set any variables needed for the options state
* Ins:			    none    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void COptionsState::Enter()
{
	m_nAlphaValue = 0;
	m_bChangeState = false;
	m_bAlphaIn = true;
	m_bPressedEscape = false;
	m_bOnInvert = false;

	m_pPause = nullptr;
	m_pOptions = nullptr;

	m_fApplyVolume = m_fPrevVolume = m_pGame->GetMusicVolume();
	m_fApplySFX = m_fPrevSFX = m_pGame->GetSFXVolume();
	m_fApplyGamma = m_fPrevGamma = m_pGame->GetRenderer()->GetGamma();
	m_fApplyYaw = m_fPrevYaw = m_pGame->GetCamera()->GetYawSpeed();
	m_fApplyPitch = m_fPrevPitch = m_pGame->GetCamera()->GetPitchSpeed();
	m_bApplyInvertY = m_bPrevInvertY = CInput::GetInvertY();
	m_bApplyInvertX = m_bPrevInvertX = CInput::GetInvertX();
	m_nApplyResolution = m_nPrevResolution = m_pGame->GetCurrentResolution();

	D3DXMatrixIdentity(&m_ReticleMat);

	m_bMouseHasPriority = false;
	m_bMouseButtonDown = false;
	m_nPrevMouseXPos = 0;
	m_nPrevMouseYPos = 0;
	m_nMouseButtonDownOnState = eOP_NONE;
	m_nButtonHighlighted = eOP_NONE;
	m_bButtonPressedTimer = false;


	if(m_pGame->GetCurrentResolution() == 1) 
	{ 
		m_nResolutionSetting = eOP_1024x768;
	} 
	else if(m_pGame->GetCurrentResolution() == 2) 
	{ 
		m_nResolutionSetting = eOP_1680x1050;
	} 
	else if(m_pGame->GetCurrentResolution() == 0) 
	{
		m_nResolutionSetting = eOP_1920x1080;
	}

	/*
	1 = 1024 x 768
	0 = 1920 x 1080
	2 = 1280 x 720
	*/
	m_bChangeResolution = false;

}

/*****************************************************************
* Exit(): Exits the options state. Will set any variables used to null values
* Ins:			    none    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void COptionsState::Exit()
{
	WwiseNS::SoundEvent(eMenuItemCancel, ePlay);
}

/*****************************************************************
* Input(): Grabs any input made by the player
* Ins:			    none    		      
* Outs:				None
* Returns:		    bool
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
bool COptionsState::Input()
{
	/*
	if(m_pGame->GetCurrentResolution() == 0) 
	{ 
	m_nResolutionSetting = eOP_1024x768;

	m_pGame->ChangeResolution(1024,768); 
	m_pGame->SetCurrentResolution(1); 
	} 
	else if(m_pGame->GetCurrentResolution() == 1) 
	{ 
	m_nResolutionSetting = eOP_1680x1050;

	m_pGame->ChangeResolution(WINDOW_WIDTH_720p,WINDOW_HEIGHT_720p); 
	m_pGame->SetCurrentResolution(2); 
	} 
	else
	{
	m_nResolutionSetting = eOP_1920x1080;

	m_pGame->ChangeResolution(WINDOW_WIDTH_1080p,WINDOW_HEIGHT_1080p); 
	m_pGame->SetCurrentResolution(0);
	}
	*/

	LONG lCursorXPos = m_pGame->GetCursorPosition().x;
	LONG lCursorYPos = m_pGame->GetCursorPosition().y;

	if(m_nButtonHighlighted == eOP_NONE)
	{
		if(CInput::BufferedButtonPress(eStart, true) || CInput::BufferedButtonPress(eUp, true) || CInput::BufferedButtonPress(eUpArrow, true) || 
			CInput::BufferedButtonPress(eLeft, true) || CInput::BufferedButtonPress(eLeftArrow, true) || CInput::BufferedButtonPress(eDown, true) || 
			CInput::BufferedButtonPress(eDownArrow, true) || CInput::BufferedButtonPress(eRight, true) || CInput::BufferedButtonPress(eRightArrow, true)
			|| CInput::DIKBufferedButtonCheck(DIK_SPACE, true))
		{
			//SetCursorPos(0, 0);
			m_nButtonHighlighted = eOP_MUSIC;
		}
	}

	if(CInput::BufferedButtonPress(eEsc, true) && m_bPressedEscape == false)
	{
		WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);

		m_pGame->SetOptionsCancel(false);
		m_fApplyVolume = m_fPrevVolume;
		m_fApplySFX = m_fPrevSFX;
		m_fApplyGamma = m_fPrevGamma;
		m_fApplyYaw = m_fPrevYaw;
		m_fApplyPitch = m_fPrevPitch;
		m_bApplyInvertX = m_bPrevInvertX;
		m_bApplyInvertY = m_bPrevInvertY;

		if(m_nPrevResolution == 1)
		{
			if(m_nPrevResolution != m_pGame->GetCurrentResolution())
			{
				m_pGame->GetState(0)->Exit();
				m_pGame->ChangeResolution(1024,768); 
				m_pGame->SetCurrentResolution(1);
			}
		}
		else if(m_nPrevResolution == 2)
		{
			if(m_nPrevResolution != m_pGame->GetCurrentResolution())
			{
				m_pGame->GetState(0)->Exit();
				m_pGame->ChangeResolution(WINDOW_WIDTH_720p,WINDOW_HEIGHT_720p); 
				m_pGame->SetCurrentResolution(2); 
			}
		}
		else if(m_nPrevResolution == 0)
		{
			if(m_nPrevResolution != m_pGame->GetCurrentResolution())
			{
				m_pGame->GetState(0)->Exit();
				m_pGame->ChangeResolution(WINDOW_WIDTH_1080p,WINDOW_HEIGHT_1080p); 
				m_pGame->SetCurrentResolution(0);
			}
		}

		m_bAlphaIn = false;

		m_pGame->SetResolutionChanged(false);
		m_pGame->GetState(0)->Enter();

		m_pGame->SetFromMainMenu(false);

		m_nToWhichState = m_nPrevState;
	}
	else if(m_bChangeState == true && m_bPressedEscape == true)
	{
		if(m_nPrevState == eMAIN_MENU)
		{
			//m_pGame->GetState(0)->Enter();
			m_pGame->PopState();
			//m_pGame->switchState(new CMainMenuState(m_pGame));
			return true;
		}
		else if(m_nPrevState == eGAME_PLAY_OPTION)
		{
			m_pGame->PopState();
			return true;
		}
	}

	if(CInput::BufferedButtonPress(eStart, true))
	{
		if(m_nButtonHighlighted == eOP_CANCEL)
		{
			m_bButtonPressed = true;

			WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);

			m_pGame->PushState(new CConfirmationState(m_pGame, eOPTIONS_CANCEL));
			return true;
		}

		if(m_nButtonHighlighted == eOP_DEFAULT)
		{
			m_bButtonPressed = true;

			WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);

			m_pGame->PushState(new CConfirmationState(m_pGame, eOPTIONS_DEFAULT));
			return true;
		}

		if(m_nButtonHighlighted == eOP_APPLY)
		{
			m_bButtonPressed = true;

			WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);

			//m_pGame->PushState(new CConfirmationState(m_pGame));
			m_pGame->PushState(new CConfirmationState(m_pGame, eOPTIONS_APPLY));
			return true;
		}
	}

	if((CInput::BufferedButtonPress(eUp) || CInput::BufferedButtonPress(eUpArrow)) && m_bAlphaIn == true && m_nMouseButtonDownOnState == eOP_NONE)
	{
		//SetCursorPos(0, 0);
		m_bMouseHasPriority = false;
		WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);

		if(m_nButtonHighlighted == eOP_MUSIC)
		{
			m_nButtonHighlighted = eOP_CANCEL;
		}
		else if(m_nButtonHighlighted == eOP_SFX)
		{
			m_nButtonHighlighted = eOP_MUSIC;
		}
		else if(m_nButtonHighlighted == eOP_GAMMA)
		{
			m_nButtonHighlighted = eOP_SFX;
		}
		else if(m_nButtonHighlighted == eOP_YAW)
		{
			m_nButtonHighlighted = eOP_GAMMA;
		}
		else if(m_nButtonHighlighted == eOP_PITCH)
		{
			m_nButtonHighlighted = eOP_YAW;
		}
		else if(m_nButtonHighlighted == eOP_INVERTX)
		{
			m_nButtonHighlighted = eOP_PITCH;
		}
		else if(m_nButtonHighlighted == eOP_INVERTY)
		{
			m_nButtonHighlighted = eOP_INVERTX;
		}
		else if(m_nButtonHighlighted == eOP_RESOLUTION_LEFT)
		{
			m_nButtonHighlighted = eOP_INVERTY;
		}
		else if(m_nButtonHighlighted == eOP_RESOLUTION_RIGHT)
		{
			m_nButtonHighlighted = eOP_RESOLUTION_LEFT;
		}
		else if(m_nButtonHighlighted == eOP_APPLY)
		{
			m_nButtonHighlighted = eOP_RESOLUTION_RIGHT;
		}
		else if(m_nButtonHighlighted == eOP_DEFAULT)
		{
			m_nButtonHighlighted = eOP_APPLY;
		}
		else if(m_nButtonHighlighted == eOP_CANCEL)
		{
			m_nButtonHighlighted = eOP_DEFAULT;
		}
	}
	else if((CInput::BufferedButtonPress(eDown) || CInput::BufferedButtonPress(eDownArrow)) && m_bAlphaIn == true && m_nMouseButtonDownOnState == eOP_NONE)
	{
		m_bMouseHasPriority = false;
		WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);

		if(m_nButtonHighlighted == eOP_MUSIC)
		{
			m_nButtonHighlighted = eOP_SFX;
		}
		else if(m_nButtonHighlighted == eOP_SFX)
		{
			m_nButtonHighlighted = eOP_GAMMA;
		}
		else if(m_nButtonHighlighted == eOP_GAMMA)
		{
			m_nButtonHighlighted = eOP_YAW;
		}
		else if(m_nButtonHighlighted == eOP_YAW)
		{
			m_nButtonHighlighted = eOP_PITCH;
		}
		else if(m_nButtonHighlighted == eOP_PITCH)
		{
			m_nButtonHighlighted = eOP_INVERTX;
		}
		else if(m_nButtonHighlighted == eOP_INVERTX)
		{
			m_nButtonHighlighted = eOP_INVERTY;
		}
		else if(m_nButtonHighlighted == eOP_INVERTY)
		{
			m_nButtonHighlighted = eOP_RESOLUTION_LEFT;
		}
		else if(m_nButtonHighlighted == eOP_RESOLUTION_LEFT)
		{
			m_nButtonHighlighted = eOP_RESOLUTION_RIGHT;
		}
		else if(m_nButtonHighlighted == eOP_RESOLUTION_RIGHT)
		{
			m_nButtonHighlighted = eOP_APPLY;
		}
		else if(m_nButtonHighlighted == eOP_APPLY)
		{
			m_nButtonHighlighted = eOP_DEFAULT;
		}
		else if(m_nButtonHighlighted == eOP_DEFAULT)
		{
			m_nButtonHighlighted = eOP_CANCEL;
		}
		else if(m_nButtonHighlighted == eOP_CANCEL)
		{
			m_nButtonHighlighted = eOP_MUSIC;
		}
	}

	if((CInput::ButtonHeld(eRight) || CInput::ButtonHeld(eRightArrow)) && m_bAlphaIn == true && m_nMouseButtonDownOnState == eOP_NONE)
	{
		if(m_nButtonHighlighted == eOP_MUSIC)
		{
			if(m_fApplyVolume < 50.0f)
			{
				m_fApplyVolume += 1.0f;
			}
		}
		else if(m_nButtonHighlighted == eOP_SFX)
		{
			if(m_fApplySFX < 50.0f)
			{
				m_fApplySFX += 1.0f;
			}
		}
		else if(m_nButtonHighlighted == eOP_GAMMA)
		{
			if(m_fApplyGamma < 0.55f)
			{
				m_fApplyGamma += 0.05f;
			}
		}
		else if(m_nButtonHighlighted == eOP_YAW)
		{
			if(m_fApplyYaw < 0.25f)
			{
				m_fApplyYaw += 0.01f;
			}
		}
		else if(m_nButtonHighlighted == eOP_PITCH)
		{
			if(m_fApplyPitch < 0.25f)
			{
				m_fApplyPitch += 0.01f;
			}
		}
	}
	else if((CInput::ButtonHeld(eLeft) || CInput::ButtonHeld(eLeftArrow)) && m_bAlphaIn == true && m_nMouseButtonDownOnState == eOP_NONE)
	{
		if(m_nButtonHighlighted == eOP_MUSIC)
		{
			if(m_fApplyVolume > 0.0f)
			{
				m_fApplyVolume -= 1.0f;
			}
		}
		else if(m_nButtonHighlighted == eOP_SFX)
		{
			if(m_fApplySFX > 0.0f)
			{
				m_fApplySFX -= 1.0f;
			}	
		}
		else if(m_nButtonHighlighted == eOP_GAMMA)
		{
			if(m_fApplyGamma > -0.35f)
			{
				m_fApplyGamma -= 0.05f;
			}	
		}
		else if(m_nButtonHighlighted == eOP_YAW)
		{
			if(m_fApplyYaw > 0.05f)
			{
				m_fApplyYaw -= 0.01f;
			}

			if(m_fApplyYaw < 0.05f)
			{
				m_fApplyYaw = 0.05f;
			}
		}
		else if(m_nButtonHighlighted == eOP_PITCH)
		{
			if(m_fApplyPitch > 0.05f)
			{
				m_fApplyPitch -= 0.01f;
			}

			if(m_fApplyPitch < 0.05f)
			{
				m_fApplyPitch = 0.05f;
			}
		}
	}

	if(m_nButtonHighlighted == eOP_RESOLUTION_LEFT || m_nButtonHighlighted == eOP_RESOLUTION_RIGHT)
	{
		if((CInput::BufferedButtonPress(eLeftArrow, true) || CInput::BufferedButtonPress(eLeft, true)) && m_bAlphaIn == true && m_nMouseButtonDownOnState == eOP_NONE)
		{
			m_nButtonHighlighted = eOP_RESOLUTION_LEFT;

			m_bChangeResolution = true;

			if(m_nResolutionSetting == eOP_1024x768)
			{
				m_nResolutionSetting = eOP_1920x1080;
			}
			else if(m_nResolutionSetting == eOP_1680x1050)
			{
				m_nResolutionSetting = eOP_1024x768;
			}
			else if(m_nResolutionSetting == eOP_1920x1080)
			{
				m_nResolutionSetting = eOP_1680x1050;
			}
		}
		else if((CInput::BufferedButtonPress(eRightArrow, true) || CInput::BufferedButtonPress(eRight, true)) && m_bAlphaIn == true && m_nMouseButtonDownOnState == eOP_NONE)
		{
			m_nButtonHighlighted = eOP_RESOLUTION_RIGHT;

			if(m_nResolutionSetting == eOP_1024x768)
			{
				m_nResolutionSetting = eOP_1680x1050;
			}
			else if(m_nResolutionSetting == eOP_1680x1050)
			{
				m_nResolutionSetting = eOP_1920x1080;
			}
			else if(m_nResolutionSetting == eOP_1920x1080)
			{
				m_nResolutionSetting = eOP_1024x768;
			}

			m_bChangeResolution = true;
		}
	}
	//cout << "X: " << m_pGame->GetCursorPosition().x << " Y: " << m_pGame->GetCursorPosition().y << '\n';

	RECT rMusic =	{312, 261, 800, 306};
	RECT rSFX =		{312, 306, 800, 349};
	RECT rGamma =	{312, 349, 800, 392};
	RECT rYaw =		{312, 392, 800, 449};
	RECT rPitch =	{312, 449, 800, 506};

	RECT rInvertY = {456, 506, 479, 535};
	RECT rInvertX = {748, 506, 771, 535};

	RECT rResolution = {312, 545, 771, 572};

	RECT rApply =   {301, 587, 453, 654};
	RECT rDefault = {454, 600, 647, 683};
	RECT rCancel =  {656, 586, 797, 652};

	RECT rMusicBall = {509, 272, 845, 298};
	RECT rSFXBall =   {509, 314, 845, 342};
	RECT rGammaBall = {509, 355, 845, 383};
	RECT rYawBall =   {509, 406, 845, 435};
	RECT rPitchBall = {509, 467, 845, 494};

	RECT rLeftArrow = {521, 544, 548, 570};
	RECT rRightArrow = {672, 544, 698, 570};

	// CANCEL
	if(lCursorXPos > rCancel.left && lCursorXPos < rCancel.right &&	lCursorYPos > rCancel.top && lCursorYPos < rCancel.bottom)
	{
		m_bOnInvert = false;

		if(m_nButtonHighlighted != eOP_CANCEL && m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);
		}

		if(m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			m_nButtonHighlighted = eOP_CANCEL;

		}

		if(CInput::BufferedButtonPress(eLeftMouseClick, true) && m_bAlphaIn == true)
		{
			WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
			m_bButtonPressed = true;
			m_pGame->PushState(new CConfirmationState(m_pGame, eOPTIONS_CANCEL));
			return true;

			//m_fApplyVolume = m_fPrevVolume;
			//m_fApplySFX = m_fPrevSFX;
			//m_fApplyGamma = m_fPrevGamma;
			//m_fApplyYaw = m_fPrevYaw;
			//m_fApplyPitch = m_fPrevPitch;
			//m_bApplyInvertX = m_bPrevInvertX;
			//m_bApplyInvertY = m_bPrevInvertY;
			//
			//m_bAlphaIn = false;
			//
			//m_nToWhichState = m_nPrevState;
		}		
	}

	// DEFAULT
	if(lCursorXPos > rDefault.left && lCursorXPos < rDefault.right && lCursorYPos > rDefault.top && lCursorYPos < rDefault.bottom)
	{
		m_bOnInvert = false;

		if(m_nButtonHighlighted != eOP_DEFAULT && m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);
		}

		if(m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			m_nButtonHighlighted = eOP_DEFAULT;
		}	

		if(CInput::BufferedButtonPress(eLeftMouseClick, true) && m_bAlphaIn == true)
		{
			WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
			m_bButtonPressed = true;

			m_pGame->PushState(new CConfirmationState(m_pGame, eOPTIONS_DEFAULT));
			return true;

			//m_fApplyVolume = m_fDefaultVolume;
			//m_fApplySFX = m_fDefaultSFX;
			//m_fApplyGamma = m_fDefaultGamma;
			//m_fApplyYaw = m_fDefaultYaw;
			//m_fApplyPitch = m_fDefaultPitch;
			//m_bApplyInvertX = m_bDefaultInvertX;
			//m_bApplyInvertY = m_bDefaultInvertY;
		}
	}

	// APPLY
	if(lCursorXPos > rApply.left && lCursorXPos < rApply.right &&	lCursorYPos > rApply.top && lCursorYPos < rApply.bottom)
	{
		m_bOnInvert = false;

		if(m_nButtonHighlighted != eOP_APPLY && m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);
		}

		if(m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			m_nButtonHighlighted = eOP_APPLY;
		}

		if(CInput::BufferedButtonPress(eLeftMouseClick, true) && m_bAlphaIn == true)
		{
			WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
			m_bButtonPressed = true;
			m_pGame->PushState(new CConfirmationState(m_pGame, eOPTIONS_APPLY));
			return true;

			//m_pGame->SetMusicVolume(m_fApplyVolume);
			//m_pGame->SetSFXVolume(m_fApplySFX);
			//m_pGame->GetRenderer()->SetGamma(m_fApplyGamma);
			//m_pGame->GetCamera()->SetYawSpeed(m_fApplyYaw);
			//m_pGame->GetCamera()->SetPitchSpeed(m_fApplyPitch);
			//CInput::SetInvertX(m_bApplyInvertX);
			//CInput::SetInvertY(m_bApplyInvertY);
			//
			//WwiseNS::SetVolumeRTPC(AK::GAME_PARAMETERS::MX_VOLUME, m_fApplyVolume);
			//WwiseNS::SetVolumeRTPC(AK::GAME_PARAMETERS::FX_VOLUME, m_fApplySFX);
			//WwiseNS::SetVolumeRTPC(AK::GAME_PARAMETERS::DX_VOLUME, m_fApplySFX);
			//
			//m_bAlphaIn = false;
			//
			//m_nToWhichState = m_nPrevState;
		}
	}

	// RIGHT ARROW
	if(lCursorXPos > rRightArrow.left && lCursorXPos < rRightArrow.right &&	lCursorYPos > rRightArrow.top && lCursorYPos < rRightArrow.bottom)
	{
		m_bOnInvert = false;

		if(m_nButtonHighlighted != eOP_RESOLUTION_RIGHT && m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);
		}

		if(m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			m_nButtonHighlighted = eOP_RESOLUTION_RIGHT;
		}

		if(CInput::BufferedButtonPress(eLeftMouseClick, true) && m_bAlphaIn == true)
		{
			// 12 = 1024x768
			// 13 = 1280x720
			// 14 = 1920x1080

			m_nResolutionSetting += 1;

			if(m_nResolutionSetting > eOP_1920x1080)
			{
				m_nResolutionSetting = eOP_1024x768;
			}

			m_bChangeResolution = true;
		}
	}


	// LEFT ARROW
	if(lCursorXPos > rLeftArrow.left && lCursorXPos < rLeftArrow.right &&	lCursorYPos > rLeftArrow.top && lCursorYPos < rLeftArrow.bottom)
	{
		m_bOnInvert = false;

		if(m_nButtonHighlighted != eOP_RESOLUTION_LEFT && m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);
		}

		if(m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			m_nButtonHighlighted = eOP_RESOLUTION_LEFT;
		}

		if(CInput::BufferedButtonPress(eLeftMouseClick, true) && m_bAlphaIn == true)
		{			
			// 12 = 1024x768
			// 13 = 1280x720
			// 14 = 1920x1080

			m_nResolutionSetting -= 1;

			if(m_nResolutionSetting < eOP_1024x768)
			{
				m_nResolutionSetting = eOP_1920x1080;
			}

			m_bChangeResolution = true;
		}
	}

	// INVERT X
	if(lCursorXPos > rInvertX.left && lCursorXPos < rInvertX.right && lCursorYPos > rInvertX.top && lCursorYPos < rInvertX.bottom)
	{
		m_bOnInvert = true;

		if(m_nButtonHighlighted != eOP_INVERTX && m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);
		}

		if(m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			m_nButtonHighlighted = eOP_INVERTX;
		}

		if(CInput::BufferedButtonPress(eLeftMouseClick, true) && m_bAlphaIn == true)
		{
			WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
			m_bApplyInvertX = !m_bApplyInvertX;
		}		
	}

	// INVERT Y
	if(lCursorXPos > rInvertY.left && lCursorXPos < rInvertY.right &&	lCursorYPos > rInvertY.top && lCursorYPos < rInvertY.bottom)
	{
		m_bOnInvert = true;

		if(m_nButtonHighlighted != eOP_INVERTY && m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);
		}

		if(m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			m_nButtonHighlighted = eOP_INVERTY;
		}

		if(CInput::BufferedButtonPress(eLeftMouseClick, true) && m_bAlphaIn == true)
		{			
			WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
			m_bApplyInvertY = !m_bApplyInvertY;			
		}
	}

	// PITCH
	if(lCursorXPos > rPitch.left && lCursorXPos < rPitch.right && lCursorYPos > rPitch.top && lCursorYPos < rPitch.bottom)
	{
		m_bOnInvert = false;

		if(m_nButtonHighlighted != eOP_PITCH && m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);
		}

		if(m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			m_nButtonHighlighted = eOP_PITCH;
		}
	}

	//	if(CInput::ButtonHeld(eLeftMouseClick) && m_bAlphaIn == true)
	//	{
	//		if(m_bMouseButtonDown == false)
	//		{
	//			m_bMouseButtonDown = true;
	//
	//			m_nClickedState = eOP_PITCH;
	//		}			
	//		else
	//		{
	//			if(m_nClickedState == eOP_PITCH)
	//			{
	//				m_bMouseButtonDown = false;
	//
	//				int nPoint = 0;
	//
	//				if(m_pGame->GetCursorPosition().x < 496)
	//				{
	//					nPoint = 496;
	//				}
	//				else if(m_pGame->GetCursorPosition().x > 796)
	//				{
	//					nPoint = 796;
	//				}
	//				else
	//				{
	//					nPoint = m_pGame->GetCursorPosition().x;
	//				}
	//
	//				m_fApplyPitch = (((nPoint - 496) * 0.2f) / 296.0f) + 0.05f;
	//
	//				if(m_fApplyPitch > 0.25f)
	//				{
	//					m_fApplyPitch = 0.25f;
	//				}
	//				else if(m_fApplyPitch < 0.05f)
	//				{
	//					m_fApplyPitch = 0.05f;
	//				}
	//			}
	//		}
	//	}
	//}

	// YAW
	if(lCursorXPos > rYaw.left && lCursorXPos < rYaw.right && lCursorYPos > rYaw.top && lCursorYPos < rYaw.bottom)
	{
		m_bOnInvert = false;

		if(m_nButtonHighlighted != eOP_YAW && m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);
		}

		if(m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			m_nButtonHighlighted = eOP_YAW;
		}
	}

	//	if(CInput::ButtonHeld(eLeftMouseClick) && m_bAlphaIn == true)
	//	{
	//		if(m_bMouseButtonDown == false)
	//		{
	//			m_bMouseButtonDown = true;
	//
	//			m_nClickedState = eOP_YAW;
	//		}
	//		else
	//		{
	//			if(m_nClickedState == eOP_YAW)
	//			{
	//				int nPoint = 0;
	//
	//				if(m_pGame->GetCursorPosition().x < 496)
	//				{
	//					nPoint = 496;
	//				}
	//				else if(m_pGame->GetCursorPosition().x > 796)
	//				{
	//					nPoint = 796;
	//				}
	//				else
	//				{
	//					nPoint = m_pGame->GetCursorPosition().x;
	//				}
	//
	//				m_fApplyYaw = (((nPoint - 496) * 0.2f) / 296.0f) + 0.05f;
	//
	//				if(m_fApplyYaw > 0.25f)
	//				{
	//					m_fApplyYaw = 0.25f;
	//				}
	//				else if(m_fApplyYaw < 0.05f)
	//				{
	//					m_fApplyYaw = 0.05f;
	//				}
	//			}
	//		}
	//	}
	//}

	// GAMMA
	if(lCursorXPos > rGamma.left && lCursorXPos < rGamma.right && lCursorYPos > rGamma.top && lCursorYPos < rGamma.bottom)
	{
		m_bOnInvert = false;

		if(m_nButtonHighlighted != eOP_GAMMA && m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);
		}

		if(m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			m_nButtonHighlighted = eOP_GAMMA;
		}
	}

	//	if(CInput::ButtonHeld(eLeftMouseClick) && m_bAlphaIn == true)
	//	{
	//		if(m_bMouseButtonDown == false)
	//		{
	//			m_bMouseButtonDown = true;
	//
	//			int nPoint = 0;
	//
	//			if(m_pGame->GetCursorPosition().x < 496)
	//			{
	//				nPoint = 496;
	//			}
	//			else if(m_pGame->GetCursorPosition().x > 796)
	//			{
	//				nPoint = 796;
	//			}
	//			else
	//			{
	//				nPoint = m_pGame->GetCursorPosition().x;
	//			}
	//
	//			m_fApplyGamma = (((nPoint - 496) * 0.9f) / 296.0f) - 0.35f;
	//
	//			if(m_fApplyGamma > 0.55f)
	//			{
	//				m_fApplyGamma = 0.55f;
	//			}
	//			else if(m_fApplyGamma < -0.35f)
	//			{
	//				m_fApplyGamma = -0.35f;
	//			}
	//		}
	//	}
	//	else
	//	{
	//		m_bMouseButtonDown = false;
	//	}
	//}

	// SFX
	if(lCursorXPos > rSFX.left && lCursorXPos < rSFX.right && lCursorYPos > rSFX.top && lCursorYPos < rSFX.bottom)
	{
		m_bOnInvert = false;

		if(m_nButtonHighlighted != eOP_SFX && m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);
		}

		if(m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			m_nButtonHighlighted = eOP_SFX;
		}

	}
	//	if(CInput::ButtonHeld(eLeftMouseClick) && m_bAlphaIn == true)
	//	{
	//		if(m_bMouseButtonDown == false)
	//		{
	//			m_bMouseButtonDown = true;
	//
	//			int nPoint = 0;
	//
	//			if(m_pGame->GetCursorPosition().x < 496)
	//			{
	//				nPoint = 496;
	//			}
	//			else if(m_pGame->GetCursorPosition().x > 796)
	//			{
	//				nPoint = 796;
	//			}
	//			else
	//			{
	//				nPoint = m_pGame->GetCursorPosition().x;
	//			}
	//
	//			m_fApplySFX = ((nPoint - 496) * 50) / 296.0f;
	//
	//			if(m_fApplySFX > 50.0f)
	//			{
	//				m_fApplySFX = 50.0f;
	//			}
	//		}
	//	}
	//	else
	//	{
	//		m_bMouseButtonDown = false;
	//	}
	//
	//}

	// MUSIC
	if(lCursorXPos > rMusic.left && lCursorXPos < rMusic.right && lCursorYPos > rMusic.top && lCursorYPos < rMusic.bottom)
	{
		m_bOnInvert = false;

		if(m_nButtonHighlighted != eOP_MUSIC && m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);
		}

		if(m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			m_nButtonHighlighted = eOP_MUSIC;
		}
	}

	if(CInput::ButtonHeld(eLeftMouseClick) && m_bAlphaIn == true)
	{
		switch(m_nButtonHighlighted)
		{
		case eOP_MUSIC:
			{
				if(m_nMouseButtonDownOnState == eOP_NONE || m_nMouseButtonDownOnState == eOP_MUSIC)
				{
					m_nMouseButtonDownOnState = eOP_MUSIC;

					int nPoint = 0;

					if(m_pGame->GetCursorPosition().x < 496)
					{
						nPoint = 496;
					}
					else if(m_pGame->GetCursorPosition().x > 796)
					{
						nPoint = 796;
					}
					else
					{
						nPoint = m_pGame->GetCursorPosition().x;
					}

					m_fApplyVolume = ((nPoint - 496) * 50) / 296.0f;

					if(m_fApplyVolume > 50.0f)
					{
						m_fApplyVolume = 50.0f;
					}
				}
			}
			break;

		case eOP_SFX:
			{
				if(m_nMouseButtonDownOnState == eOP_NONE || m_nMouseButtonDownOnState == eOP_SFX)
				{
					m_nMouseButtonDownOnState = eOP_SFX;

					int nPoint = 0;

					if(m_pGame->GetCursorPosition().x < 496)
					{
						nPoint = 496;
					}
					else if(m_pGame->GetCursorPosition().x > 796)
					{
						nPoint = 796;
					}
					else
					{
						nPoint = m_pGame->GetCursorPosition().x;
					}

					m_fApplySFX = ((nPoint - 496) * 50) / 296.0f;

					if(m_fApplySFX > 50.0f)
					{
						m_fApplySFX = 50.0f;
					}
				}
			}
			break;

		case eOP_GAMMA:
			{
				if(m_nMouseButtonDownOnState == eOP_NONE || m_nMouseButtonDownOnState == eOP_GAMMA)
				{
					m_nMouseButtonDownOnState = eOP_GAMMA;

					int nPoint = 0;

					if(m_pGame->GetCursorPosition().x < 496)
					{
						nPoint = 496;
					}
					else if(m_pGame->GetCursorPosition().x > 796)
					{
						nPoint = 796;
					}
					else
					{
						nPoint = m_pGame->GetCursorPosition().x;
					}

					m_fApplyGamma = (((nPoint - 496) * 0.9f) / 296.0f) - 0.35f;

					if(m_fApplyGamma > 0.55f)
					{
						m_fApplyGamma = 0.55f;
					}
					else if(m_fApplyGamma < -0.35f)
					{
						m_fApplyGamma = -0.35f;
					}
				}
			}
			break;

		case eOP_PITCH:
			{
				if(m_nMouseButtonDownOnState == eOP_NONE || m_nMouseButtonDownOnState == eOP_PITCH)
				{
					m_nMouseButtonDownOnState = eOP_PITCH;

					int nPoint = 0;

					if(m_pGame->GetCursorPosition().x < 496)
					{
						nPoint = 496;
					}
					else if(m_pGame->GetCursorPosition().x > 796)
					{
						nPoint = 796;
					}
					else
					{
						nPoint = m_pGame->GetCursorPosition().x;
					}

					m_fApplyPitch = (((nPoint - 496) * 0.2f) / 296.0f) + 0.05f;

					if(m_fApplyPitch > 0.25f)
					{
						m_fApplyPitch = 0.25f;
					}
					else if(m_fApplyPitch < 0.05f)
					{
						m_fApplyPitch = 0.05f;
					}
				}
			}
			break;

		case eOP_YAW:
			{
				if(m_nMouseButtonDownOnState == eOP_NONE || m_nMouseButtonDownOnState == eOP_YAW)
				{
					m_nMouseButtonDownOnState = eOP_YAW;

					int nPoint = 0;

					if(m_pGame->GetCursorPosition().x < 496)
					{
						nPoint = 496;
					}
					else if(m_pGame->GetCursorPosition().x > 796)
					{
						nPoint = 796;
					}
					else
					{
						nPoint = m_pGame->GetCursorPosition().x;
					}

					m_fApplyYaw = (((nPoint - 496) * 0.2f) / 296.0f) + 0.05f;

					if(m_fApplyYaw > 0.25f)
					{
						m_fApplyYaw = 0.25f;
					}
					else if(m_fApplyYaw < 0.05f)
					{
						m_fApplyYaw = 0.05f;
					}
				}
			}
			break;
		}

	}
	else
	{
		m_nMouseButtonDownOnState = eOP_NONE;
	}


	return true;
}

/*****************************************************************
* Update(): Updates the options state based on the change in time
* Ins:			    fElapsedTime    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void COptionsState::Update(float _fElapsedTime)
{
	//cout << m_pGame->GetCursorPosition().x << " " << m_pGame->GetCursorPosition().y << '\n';
	if(m_nPrevState == eMAIN_MENU)
	{
		m_pGame->GetState(0)->Update(_fElapsedTime);
		m_pGame->SetFromMainMenu(true);
	}


	if(m_pGame->GetOptionsApply() == true)
	{
		if(m_nPrevState == eMAIN_MENU)
		{
			m_pGame->GetState(0)->Exit();
		}

		m_pGame->SetOptionsApply(false);
		m_pGame->SetMusicVolume(m_fApplyVolume);
		m_pGame->SetSFXVolume(m_fApplySFX);
		m_pGame->GetRenderer()->SetGamma(m_fApplyGamma);
		m_pGame->GetCamera()->SetYawSpeed(m_fApplyYaw);
		m_pGame->GetCamera()->SetPitchSpeed(m_fApplyPitch);
		CInput::SetInvertX(m_bApplyInvertX);
		CInput::SetInvertY(m_bApplyInvertY);

		WwiseNS::SetVolumeRTPC(AK::GAME_PARAMETERS::MX_VOLUME, m_fApplyVolume);
		WwiseNS::SetVolumeRTPC(AK::GAME_PARAMETERS::FX_VOLUME, m_fApplySFX);
		WwiseNS::SetVolumeRTPC(AK::GAME_PARAMETERS::DX_VOLUME, m_fApplySFX);

		m_bAlphaIn = false;

		m_pGame->SetResolutionChanged(false);
		
		if(m_nPrevState == eMAIN_MENU)
		{
			m_pGame->GetState(0)->Enter();
		}

		m_pGame->SetFromMainMenu(false);

		m_nToWhichState = m_nPrevState;
	}
	else if(m_pGame->GetOptionsCancel() == true)
	{
		if(m_nPrevState == eMAIN_MENU)
		{
			m_pGame->GetState(0)->Exit();
		}

		m_pGame->SetOptionsCancel(false);
		m_fApplyVolume = m_fPrevVolume;
		m_fApplySFX = m_fPrevSFX;
		m_fApplyGamma = m_fPrevGamma;
		m_fApplyYaw = m_fPrevYaw;
		m_fApplyPitch = m_fPrevPitch;
		m_bApplyInvertX = m_bPrevInvertX;
		m_bApplyInvertY = m_bPrevInvertY;

		if(m_nPrevResolution == 1)
		{
			if(m_nPrevResolution != m_pGame->GetCurrentResolution())
			{
				m_pGame->ChangeResolution(1024,768); 
				m_pGame->SetCurrentResolution(1);
			}
		}
		else if(m_nPrevResolution == 2)
		{
			if(m_nPrevResolution != m_pGame->GetCurrentResolution())
			{
				m_pGame->ChangeResolution(WINDOW_WIDTH_720p,WINDOW_HEIGHT_720p); 
				m_pGame->SetCurrentResolution(2); 
			}
		}
		else if(m_nPrevResolution == 0)
		{
			if(m_nPrevResolution != m_pGame->GetCurrentResolution())
			{
				m_pGame->ChangeResolution(WINDOW_WIDTH_1080p,WINDOW_HEIGHT_1080p); 
				m_pGame->SetCurrentResolution(0);
			}
		}

		m_bAlphaIn = false;

		m_pGame->SetResolutionChanged(false);
		
		if(m_nPrevState == eMAIN_MENU)
		{
			m_pGame->GetState(0)->Enter();
		}

		m_pGame->SetFromMainMenu(false);

		m_nToWhichState = m_nPrevState;
	}
	else if(m_pGame->GetOptionsDefault() == true)
	{
		m_pGame->GetState(0)->Exit();
		m_pGame->SetOptionsDefault(false);
		m_fApplyVolume = m_fDefaultVolume;
		m_fApplySFX = m_fDefaultSFX;
		m_fApplyGamma = m_fDefaultGamma;
		m_fApplyYaw = m_fDefaultYaw;
		m_fApplyPitch = m_fDefaultPitch;
		m_bApplyInvertX = m_bDefaultInvertX;
		m_bApplyInvertY = m_bDefaultInvertY;

		if(m_nDefaultResolution == eOP_1024x768)
		{
			m_pGame->SetResolutionChanged(true);
			m_pGame->ChangeResolution(1024,768); 
			m_pGame->SetCurrentResolution(1);

			m_nResolutionSetting = eOP_1024x768;
		}
		else if(m_nDefaultResolution == eOP_1680x1050)
		{
			m_pGame->SetResolutionChanged(true);
			m_pGame->ChangeResolution(WINDOW_WIDTH_720p,WINDOW_HEIGHT_720p); 
			m_pGame->SetCurrentResolution(2); 

			m_nResolutionSetting = eOP_1680x1050;
		}
		else if(m_nDefaultResolution == eOP_1920x1080)
		{
			m_pGame->SetResolutionChanged(true);
			m_pGame->ChangeResolution(WINDOW_WIDTH_1080p,WINDOW_HEIGHT_1080p); 
			m_pGame->SetCurrentResolution(0);

			m_nResolutionSetting = eOP_1920x1080;
		}
	}

	if(m_bChangeResolution == true)
	{
		//	1 = 1024 x 768
		//	0 = 1920 x 1080
		//	2 = 1280 x 720

		if(m_nResolutionSetting == eOP_1024x768)
		{
			m_pGame->SetResolutionChanged(true);
			m_pGame->ChangeResolution(1024,768); 
			m_pGame->SetCurrentResolution(1);
		}
		else if(m_nResolutionSetting == eOP_1680x1050)
		{
			m_pGame->SetResolutionChanged(true);
			m_pGame->ChangeResolution(WINDOW_WIDTH_720p,WINDOW_HEIGHT_720p); 
			m_pGame->SetCurrentResolution(2);
		}
		else if(m_nResolutionSetting == eOP_1920x1080)
		{
			m_pGame->SetResolutionChanged(true);
			m_pGame->ChangeResolution(WINDOW_WIDTH_1080p,WINDOW_HEIGHT_1080p); 
			m_pGame->SetCurrentResolution(0);
		}

		m_bChangeResolution = false;
	}

	if(m_bChangeState == true)
	{
		if(m_nToWhichState == eMAIN_MENU)
		{
			//m_pGame->GetState(0)->Enter();
			m_pGame->PopState();
		}
		else if(m_nToWhichState == eGAME_PLAY_OPTION)
		{
			m_pGame->PopState();
		}

		return;
	}

	if(m_bAlphaIn == true)
	{
		if(m_nAlphaValue < 255)
		{
			m_nAlphaValue += 15;
		}
		else if(m_nAlphaValue > 255)
		{
			m_nAlphaValue = 255;
		}
	}
	else
	{
		if(m_nAlphaValue > 0)
		{
			m_nAlphaValue -= 15;
		}
		else if(m_nAlphaValue <= 0)
		{
			m_nAlphaValue = 0;
			m_bChangeState = true;
		}
	}

	if(m_bButtonPressedTimer == true)
	{
		static float fButtonPressedTimer = 0.0f;

		if(fButtonPressedTimer > 0.2f)
		{
			m_bButtonPressedTimer = false;
			m_bButtonPressed = false;
			fButtonPressedTimer = 0.0f;
		}

		fButtonPressedTimer += _fElapsedTime;
	}

	if(m_pGame->GetGuiHelper()->GetOPVolumeBallXPos() < 760)
	{
		m_pGame->GetGuiHelper()->SetVBallXPos(760);
	}
	else if(m_pGame->GetGuiHelper()->GetOPVolumeBallXPos() > 1185)
	{
		m_pGame->GetGuiHelper()->SetVBallXPos(1185);
	}
	else
	{
		m_nMusicBallXPos = int(((m_fApplyVolume * (1185 - 760)) / (DEFAULT_VOLUME * 2)) + 760);

		m_pGame->GetGuiHelper()->SetVBallXPos(m_nMusicBallXPos);
	}

	if(m_pGame->GetGuiHelper()->GetOPSFXXPos() < 760)
	{
		m_pGame->GetGuiHelper()->SetSBallXPos(760);
	}
	else if(m_pGame->GetGuiHelper()->GetOPSFXXPos() > 1185)
	{
		m_pGame->GetGuiHelper()->SetSBallXPos(1185);
	}
	else
	{
		m_nSFXBallXPos = int(((m_fApplySFX * (1185 - 760)) / (DEFAULT_VOLUME * 2)) + 760);

		m_pGame->GetGuiHelper()->SetSBallXPos(m_nSFXBallXPos);
	}

	if(m_pGame->GetGuiHelper()->GetOPGammaXPos() < 760)
	{
		m_pGame->GetGuiHelper()->SetGBallXPos(760);
	}
	else if(m_pGame->GetGuiHelper()->GetOPGammaXPos() > 1185)
	{
		m_pGame->GetGuiHelper()->SetGBallXPos(1185);
	}
	else
	{
		m_nGammaBallXPos = int((((m_fApplyGamma + 0.35f) * 425) / 0.9f) + 760);

		m_pGame->GetGuiHelper()->SetGBallXPos(m_nGammaBallXPos);
	}

	if(m_pGame->GetGuiHelper()->GetOPYawXPos() < 760)
	{
		m_pGame->GetGuiHelper()->SetYBallXPos(760);
	}
	else if(m_pGame->GetGuiHelper()->GetOPYawXPos() > 1185)
	{
		m_pGame->GetGuiHelper()->SetYBallXPos(1185);
	}
	else
	{
		m_nYawBallXPos = int((((m_fApplyYaw - 0.05f) * 425) / 0.20f) + 760);

		m_pGame->GetGuiHelper()->SetYBallXPos(m_nYawBallXPos);
	}

	if(m_pGame->GetGuiHelper()->GetOPPitchXPos() < 760)
	{
		m_pGame->GetGuiHelper()->SetPBallXPos(760);
	}
	else if(m_pGame->GetGuiHelper()->GetOPPitchXPos() > 1185)
	{
		m_pGame->GetGuiHelper()->SetPBallXPos(1185);
	}
	else
	{
		m_nPitchBallXPos = int((((m_fApplyPitch - 0.05f) * 425) / 0.20f) + 760);

		m_pGame->GetGuiHelper()->SetPBallXPos(m_nPitchBallXPos);
	}

	m_pGame->SetMusicVolume(m_fApplyVolume);
	m_pGame->SetSFXVolume(m_fApplySFX);
	m_pGame->GetRenderer()->SetGamma(m_fApplyGamma);
	m_pGame->GetCamera()->SetYawSpeed(m_fApplyYaw);
	m_pGame->GetCamera()->SetPitchSpeed(m_fApplyPitch);
	CInput::SetInvertX(m_bApplyInvertX);
	CInput::SetInvertY(m_bApplyInvertY);

	WwiseNS::SetVolumeRTPC(AK::GAME_PARAMETERS::MX_VOLUME, m_fApplyVolume);
	WwiseNS::SetVolumeRTPC(AK::GAME_PARAMETERS::FX_VOLUME, m_fApplySFX);
	WwiseNS::SetVolumeRTPC(AK::GAME_PARAMETERS::DX_VOLUME, m_fApplySFX);

	if(m_nPrevMouseXPos != m_pGame->GetCursorPosition().x || m_nPrevMouseYPos != m_pGame->GetCursorPosition().y)
	{
		m_bMouseHasPriority = true;
	}

	float xRatio = m_pGame->GetRenderer()->GetXRatio();
	float yRatio = m_pGame->GetRenderer()->GetYRatio();

	m_nPrevMouseXPos = m_pGame->GetCursorPosition().x;
	m_nPrevMouseYPos = m_pGame->GetCursorPosition().y;

	m_ReticleMat._41 = float(m_pGame->GetCursorPosition().x) * xRatio;
	m_ReticleMat._42 = float(m_pGame->GetCursorPosition().y)* yRatio;
}

/*****************************************************************
* Render(): Renders the options state by calling the renderer classes render function
* Ins:			    none    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void COptionsState::Render()
{
	float xRatio = m_pGame->GetRenderer()->GetXRatio();
	float yRatio = m_pGame->GetRenderer()->GetYRatio();



	RECT rBlackRect = {0, 0, 100, 100};
	D3DXMATRIX d3dBlackMat;
	D3DXMATRIX d3dBlackScale;
	D3DXMatrixIdentity(&d3dBlackMat);
	D3DXMatrixIdentity(&d3dBlackScale);

	d3dBlackMat._41 = 537.0f * xRatio;
	d3dBlackMat._42 = 379.0f * yRatio;

	D3DXMatrixScaling(&d3dBlackScale, 8.5f * xRatio, 7.0f * yRatio, 2.0f);

	m_pGame->GetState(0)->Render();
	m_pGame->GetRenderer()->RenderArrowAndrewR(m_pGame->GetBlackTexture(), &rBlackRect,  (d3dBlackScale * d3dBlackMat), D3DCOLOR_ARGB(200,255,255,255), true, false);


	m_pGame->GetRenderer()->RenderText("OPTIONS", 450, 100, 3.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));

	m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPLeftBorderXPos(),(float)m_pGame->GetGuiHelper()->GetOPLeftBorderYPos(),false, m_pGame->GetGuiHelper()->GetOPLeftBorderXScale(),m_pGame->GetGuiHelper()->GetOPLeftBorderYScale(), false, 160, 545, 348, 1066, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPRightBorderXPos(),(float)m_pGame->GetGuiHelper()->GetOPRightBorderYPos(),false, m_pGame->GetGuiHelper()->GetOPRightBorderXScale(),m_pGame->GetGuiHelper()->GetOPRightBorderYScale(), false, 678, 545, 864, 1066, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));

	m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPMainXPos(),(float)m_pGame->GetGuiHelper()->GetOPMainYPos(),false, m_pGame->GetGuiHelper()->GetOPMainXScale(),m_pGame->GetGuiHelper()->GetOPMainYScale(), false, 160, 1066, 864, 1437, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPInvertYXPos() ,(float)m_pGame->GetGuiHelper()->GetOPInvertYYPos() ,false, m_pGame->GetGuiHelper()->GetOPInvertYXScale(), m_pGame->GetGuiHelper()->GetOPInvertYYScale(), false, 160, 1437, 348, 1506, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPInvertXXPos(),(float)m_pGame->GetGuiHelper()->GetOPInvertXYPos(),false, m_pGame->GetGuiHelper()->GetOPInvertXXScale(), m_pGame->GetGuiHelper()->GetOPInvertXYScale(), false, 564, 1437, 742, 1506, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPResolutionXPos(),(float)m_pGame->GetGuiHelper()->GetOPResolutionYPos(),false, m_pGame->GetGuiHelper()->GetOPResolutionXScale(), m_pGame->GetGuiHelper()->GetOPResolutionYScale(), false, 160, 1506, 388, 1556, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));

	if(m_nButtonHighlighted == eOP_APPLY && m_bButtonPressed == true)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPCancelXPos() ,(float)m_pGame->GetGuiHelper()->GetOPCancelYPos() ,false, m_pGame->GetGuiHelper()->GetOPCancelXScale(), m_pGame->GetGuiHelper()->GetOPCancelYScale(), false, 0, 1591, 512, 1743, D3DCOLOR_ARGB(m_nAlphaValue, 127, 127, 127));
	}
	else if(m_nButtonHighlighted == eOP_APPLY)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPCancelXPos() ,(float)m_pGame->GetGuiHelper()->GetOPCancelYPos() ,false, m_pGame->GetGuiHelper()->GetOPCancelXScale(), m_pGame->GetGuiHelper()->GetOPCancelYScale(), true, 0, 1591, 512, 1743, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}
	else
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPCancelXPos() ,(float)m_pGame->GetGuiHelper()->GetOPCancelYPos() ,false, m_pGame->GetGuiHelper()->GetOPCancelXScale(), m_pGame->GetGuiHelper()->GetOPCancelYScale(), false, 0, 1591, 512, 1743, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}

	if(m_nButtonHighlighted == eOP_DEFAULT && m_bButtonPressed == true)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPDefaultXPos(),(float)m_pGame->GetGuiHelper()->GetOPDefaultYPos(),false, m_pGame->GetGuiHelper()->GetOPDefaultXScale(), m_pGame->GetGuiHelper()->GetOPDefaultYScale(), false, 0, 1743, 512, 1875, D3DCOLOR_ARGB(m_nAlphaValue, 127, 127, 127));
	}
	else if(m_nButtonHighlighted == eOP_DEFAULT)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPDefaultXPos(),(float)m_pGame->GetGuiHelper()->GetOPDefaultYPos(),false, m_pGame->GetGuiHelper()->GetOPDefaultXScale(), m_pGame->GetGuiHelper()->GetOPDefaultYScale(), true, 0, 1743, 512, 1875, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}
	else
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPDefaultXPos(),(float)m_pGame->GetGuiHelper()->GetOPDefaultYPos(),false, m_pGame->GetGuiHelper()->GetOPDefaultXScale(), m_pGame->GetGuiHelper()->GetOPDefaultYScale(), false, 0, 1743, 512, 1875, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}

	if(m_nButtonHighlighted == eOP_CANCEL && m_bButtonPressed == true)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPApplyXPos(),(float)m_pGame->GetGuiHelper()->GetOPApplyYPos(),false, m_pGame->GetGuiHelper()->GetOPApplyXScale(), m_pGame->GetGuiHelper()->GetOPApplyYScale(), false, 0, 1875, 512, 2006, D3DCOLOR_ARGB(m_nAlphaValue, 127, 127, 127));
	}
	else if(m_nButtonHighlighted == eOP_CANCEL)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPApplyXPos(),(float)m_pGame->GetGuiHelper()->GetOPApplyYPos(),false, m_pGame->GetGuiHelper()->GetOPApplyXScale(), m_pGame->GetGuiHelper()->GetOPApplyYScale(), true, 0, 1875, 512, 2006, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}
	else
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPApplyXPos(),(float)m_pGame->GetGuiHelper()->GetOPApplyYPos(),false, m_pGame->GetGuiHelper()->GetOPApplyXScale(), m_pGame->GetGuiHelper()->GetOPApplyYScale(), false, 0, 1875, 512, 2006, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}

	if(m_bApplyInvertX == true)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPInvertXBoxXPos(),(float)m_pGame->GetGuiHelper()->GetOPInvertXBoxYPos(), false, m_pGame->GetGuiHelper()->GetOPInvertXBoxXScale(), m_pGame->GetGuiHelper()->GetOPInvertXBoxYScale(), false, 809, 1456, 846, 1494, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}
	else
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPInvertXBoxXPos(),(float)m_pGame->GetGuiHelper()->GetOPInvertXBoxYPos(), false, m_pGame->GetGuiHelper()->GetOPInvertXBoxXScale(), m_pGame->GetGuiHelper()->GetOPInvertXBoxYScale(), false, 432, 1456, 469, 1494, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}

	if(m_bApplyInvertY == true)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPInvertYBoxXPos(),(float)m_pGame->GetGuiHelper()->GetOPInvertYBoxYPos(), false, m_pGame->GetGuiHelper()->GetOPInvertYBoxXScale(), m_pGame->GetGuiHelper()->GetOPInvertYBoxYScale(), false, 809, 1456, 846, 1494, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}
	else
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPInvertYBoxXPos(),(float)m_pGame->GetGuiHelper()->GetOPInvertYBoxYPos(), false, m_pGame->GetGuiHelper()->GetOPInvertYBoxXScale(), m_pGame->GetGuiHelper()->GetOPInvertYBoxYScale(), false, 432, 1456, 469, 1494, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}

	if(m_nButtonHighlighted != eOP_MUSIC)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPVolumeBallXPos(),(float)m_pGame->GetGuiHelper()->GetOPVolumeBallYPos(), false, m_pGame->GetGuiHelper()->GetOPVolumeBallXScale(), m_pGame->GetGuiHelper()->GetOPVolumeBallYScale(), false, 432, 1547, 469, 1586, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}
	else
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPVolumeBallXPos(),(float)m_pGame->GetGuiHelper()->GetOPVolumeBallYPos(), false, m_pGame->GetGuiHelper()->GetOPVolumeBallXScale(), m_pGame->GetGuiHelper()->GetOPVolumeBallYScale(), false, 809, 1547, 846, 1586, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}

	if(m_nButtonHighlighted != eOP_SFX)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPSFXXPos(),(float)m_pGame->GetGuiHelper()->GetOPSFXYPos(), false, m_pGame->GetGuiHelper()->GetOPSFXXScale(), m_pGame->GetGuiHelper()->GetOPSFXYScale(), false, 432, 1547, 469, 1586, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}
	else
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPSFXXPos(),(float)m_pGame->GetGuiHelper()->GetOPSFXYPos(), false, m_pGame->GetGuiHelper()->GetOPSFXXScale(), m_pGame->GetGuiHelper()->GetOPSFXYScale(), false, 809, 1547, 846, 1586, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}

	if(m_nButtonHighlighted != eOP_GAMMA)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPGammaXPos(),(float)m_pGame->GetGuiHelper()->GetOPGammaYPos(), false, m_pGame->GetGuiHelper()->GetOPGammaXScale(), m_pGame->GetGuiHelper()->GetOPGammaYScale(), false, 432, 1547, 469, 1586, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}
	else
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPGammaXPos(),(float)m_pGame->GetGuiHelper()->GetOPGammaYPos(), false, m_pGame->GetGuiHelper()->GetOPGammaXScale(), m_pGame->GetGuiHelper()->GetOPGammaYScale(), false, 809, 1547, 846, 1586, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}

	if(m_nButtonHighlighted != eOP_YAW)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPYawXPos(),(float)m_pGame->GetGuiHelper()->GetOPYawYPos(), false, m_pGame->GetGuiHelper()->GetOPYawXScale(), m_pGame->GetGuiHelper()->GetOPYawYScale(), false, 432, 1547, 469, 1586, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}
	else
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPYawXPos(),(float)m_pGame->GetGuiHelper()->GetOPYawYPos(), false, m_pGame->GetGuiHelper()->GetOPYawXScale(), m_pGame->GetGuiHelper()->GetOPYawYScale(), false, 809, 1547, 846, 1586, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}

	if(m_nButtonHighlighted != eOP_PITCH)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPPitchXPos(),(float)m_pGame->GetGuiHelper()->GetOPPitchYPos(), false, m_pGame->GetGuiHelper()->GetOPPitchXScale(), m_pGame->GetGuiHelper()->GetOPPitchYScale(), false, 432, 1547, 469, 1586, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}
	else
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPPitchXPos(),(float)m_pGame->GetGuiHelper()->GetOPPitchYPos(), false, m_pGame->GetGuiHelper()->GetOPPitchXScale(), m_pGame->GetGuiHelper()->GetOPPitchYScale(), false, 809, 1547, 846, 1586, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}

	if(m_nResolutionSetting == eOP_1024x768)
	{
		m_pGame->GetRenderer()->RenderText("1024 x 768", m_pGame->GetGuiHelper()->Get1024x768XPos(), m_pGame->GetGuiHelper()->Get1024x768YPos(), m_pGame->GetGuiHelper()->Get1024x768Scale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}
	else if(m_nResolutionSetting == eOP_1680x1050)
	{
		m_pGame->GetRenderer()->RenderText("1280 x 720", m_pGame->GetGuiHelper()->Get1680x1050XPos(), m_pGame->GetGuiHelper()->Get1680x1050YPos(), m_pGame->GetGuiHelper()->Get1680x1050Scale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}
	else if(m_nResolutionSetting == eOP_1920x1080)
	{
		m_pGame->GetRenderer()->RenderText("1920 x 1080", m_pGame->GetGuiHelper()->Get1920x1080XPos(), m_pGame->GetGuiHelper()->Get1920x1080YPos(), m_pGame->GetGuiHelper()->Get1920x1080Scale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}

	if(m_nButtonHighlighted != eOP_RESOLUTION_LEFT)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPResArrowOneXPos(), (float)m_pGame->GetGuiHelper()->GetOPResArrowOneYPos(), false, m_pGame->GetGuiHelper()->GetOPResArrowOneXScale(), m_pGame->GetGuiHelper()->GetOPResArrowOneYScale(), false, 510, 1548, 547, 1585, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}
	else
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPResArrowOneXPos(), (float)m_pGame->GetGuiHelper()->GetOPResArrowOneYPos(), false, m_pGame->GetGuiHelper()->GetOPResArrowOneXScale(), m_pGame->GetGuiHelper()->GetOPResArrowOneYScale(), false, 580, 1548, 618, 1585, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}

	if(m_nButtonHighlighted != eOP_RESOLUTION_RIGHT)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPResArrowTwoXPos(), (float)m_pGame->GetGuiHelper()->GetOPResArrowTwoYPos(), false, m_pGame->GetGuiHelper()->GetOPResArrowTwoXScale(), m_pGame->GetGuiHelper()->GetOPResArrowTwoYScale(), false, 661, 1548, 699, 1585, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}
	else
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetOPResArrowTwoXPos(), (float)m_pGame->GetGuiHelper()->GetOPResArrowTwoYPos(), false, m_pGame->GetGuiHelper()->GetOPResArrowTwoXScale(), m_pGame->GetGuiHelper()->GetOPResArrowTwoYScale(), false, 732, 1548, 769, 1585, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}

	//if(/*m_nButtonHighlighted == eOP_APPLY && m_bButtonPressed == true*/true == true)
	//{
	//	m_pGame->GetRenderer()->RenderText("1024 x 768", m_pGame->GetGuiHelper()->Get1024x768XPos(), m_pGame->GetGuiHelper()->Get1024x768YPos(), m_pGame->GetGuiHelper()->Get1024x768Scale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255, 0, 0));
	//	m_bButtonPressedTimer = true;
	//}
	//else
	//{
	//	m_pGame->GetRenderer()->RenderText("APPLY", m_pGame->GetGuiHelper()->GetOPApplyTextXPos(), m_pGame->GetGuiHelper()->GetOPApplyTextYPos(), m_pGame->GetGuiHelper()->GetOPApplyTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	//}
	//
	//if(/*m_nButtonHighlighted == eOP_APPLY && m_bButtonPressed == true*/true == true)
	//{
	//	m_pGame->GetRenderer()->RenderText("1680 x 1050", m_pGame->GetGuiHelper()->Get1680x1050XPos(), m_pGame->GetGuiHelper()->Get1680x1050YPos(), m_pGame->GetGuiHelper()->Get1680x1050Scale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255, 0, 0));
	//	m_bButtonPressedTimer = true;
	//}
	//else
	//{
	//	m_pGame->GetRenderer()->RenderText("APPLY", m_pGame->GetGuiHelper()->GetOPApplyTextXPos(), m_pGame->GetGuiHelper()->GetOPApplyTextYPos(), m_pGame->GetGuiHelper()->GetOPApplyTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	//}
	//
	//if(/*m_nButtonHighlighted == eOP_APPLY && m_bButtonPressed == true*/true == true)
	//{
	//	m_pGame->GetRenderer()->RenderText("1920 x 1080", m_pGame->GetGuiHelper()->Get1920x1080XPos(), m_pGame->GetGuiHelper()->Get1920x1080YPos(), m_pGame->GetGuiHelper()->Get1920x1080Scale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255, 0, 0));
	//	m_bButtonPressedTimer = true;
	//}
	//else
	//{
	//	m_pGame->GetRenderer()->RenderText("APPLY", m_pGame->GetGuiHelper()->GetOPApplyTextXPos(), m_pGame->GetGuiHelper()->GetOPApplyTextYPos(), m_pGame->GetGuiHelper()->GetOPApplyTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	//}

	if(m_nButtonHighlighted == eOP_APPLY && m_bButtonPressed == true)
	{
		m_pGame->GetRenderer()->RenderText("APPLY", m_pGame->GetGuiHelper()->GetOPApplyTextXPos(), m_pGame->GetGuiHelper()->GetOPApplyTextYPos(), m_pGame->GetGuiHelper()->GetOPApplyTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255, 0, 0));
		m_bButtonPressedTimer = true;
	}
	else
	{
		m_pGame->GetRenderer()->RenderText("APPLY", m_pGame->GetGuiHelper()->GetOPApplyTextXPos(), m_pGame->GetGuiHelper()->GetOPApplyTextYPos(), m_pGame->GetGuiHelper()->GetOPApplyTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
	}

	if(m_nButtonHighlighted == eOP_DEFAULT && m_bButtonPressed == true)
	{
		m_pGame->GetRenderer()->RenderText("DEFAULT", m_pGame->GetGuiHelper()->GetOPDefaultTextXPos(), m_pGame->GetGuiHelper()->GetOPDefaultTextYPos(), m_pGame->GetGuiHelper()->GetOPDefaultTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255, 0, 0));
		m_bButtonPressedTimer = true;
	}
	else
	{
		m_pGame->GetRenderer()->RenderText("DEFAULT", m_pGame->GetGuiHelper()->GetOPDefaultTextXPos(), m_pGame->GetGuiHelper()->GetOPDefaultTextYPos(), m_pGame->GetGuiHelper()->GetOPDefaultTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
	}

	if(m_nButtonHighlighted == eOP_CANCEL && m_bButtonPressed == true)
	{
		m_pGame->GetRenderer()->RenderText("CANCEL", m_pGame->GetGuiHelper()->GetOPCancelTextXPos(), m_pGame->GetGuiHelper()->GetOPCancelTextYPos(), m_pGame->GetGuiHelper()->GetOPCancelTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255, 0, 0));
		m_bButtonPressedTimer = true;
	}
	else
	{
		m_pGame->GetRenderer()->RenderText("CANCEL", m_pGame->GetGuiHelper()->GetOPCancelTextXPos(), m_pGame->GetGuiHelper()->GetOPCancelTextYPos(), m_pGame->GetGuiHelper()->GetOPCancelTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
	}


	D3DXMATRIX d3dTmpMat;	D3DXMatrixTranslation(&d3dTmpMat, 810* xRatio, 700* yRatio, 0);
	RECT rArrowRect = {0,0,64,32};
	m_pGame->GetRenderer()->RenderArrowAndrewR(m_pGame->GetArrowKeyTexture(), &rArrowRect, d3dTmpMat,  D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255), true, false);
	m_pGame->GetRenderer()->RenderText("Change",    861, 691, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
	m_pGame->GetRenderer()->RenderText("Change",    860, 690, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	m_pGame->GetRenderer()->RenderText("Selection", 861, 705, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
	m_pGame->GetRenderer()->RenderText("Selection", 860, 704, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));


	if(m_nButtonHighlighted == eOP_INVERTX || m_nButtonHighlighted == eOP_INVERTY)
	{
		D3DXMATRIX d3dTmpMat2;	D3DXMatrixTranslation(&d3dTmpMat2, 810* xRatio, 740* yRatio, 0);
		RECT rEnterRect = {0,0,64,32};
		m_pGame->GetRenderer()->RenderArrowAndrewR(m_pGame->GetEnterKeyTexture(), &rEnterRect, d3dTmpMat2,  D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255), true, false);
		m_pGame->GetRenderer()->RenderText("Confirm",   861, 756, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
		m_pGame->GetRenderer()->RenderText("Confirm",   860, 755, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
		m_pGame->GetRenderer()->RenderText("Selection", 861, 770, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
		m_pGame->GetRenderer()->RenderText("Selection", 860, 769, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}
	else
	{

		D3DXMATRIX d3dTmpMat2;	D3DXMatrixTranslation(&d3dTmpMat2, 810* xRatio, 740* yRatio, 0);
		RECT rArrowLRRect = {0,0,64,32};
		m_pGame->GetRenderer()->RenderArrowAndrewR(m_pGame->GetArrowKeyLRTexture(), &rArrowLRRect, d3dTmpMat2,  D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255), true, false);
		m_pGame->GetRenderer()->RenderText("Move Slider",  861, 736, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
		m_pGame->GetRenderer()->RenderText("Move Slider",  860, 735, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}

	RECT m_rArrowRect = {0, 0, 64, 64};
	if (m_bAlphaIn)
	{
		m_pGame->GetRenderer()->RenderArrowAndrewR(m_pGame->GetMenuCursorTexture(), &m_rArrowRect,  m_ReticleMat,D3DCOLOR_ARGB(255, 255,0,0), true, false );
	}

}


