/***********************************************
* Filename:  		CPauseState.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AG
* Author:    		Andrew Goossens
* Purpose:   		This will display the pause screen
*						when the player pauses the game
*					
************************************************/
#include "../StdAfx.h"
#include "MainMenuState.h"
#include "GamePlayState.h"
#include "PauseState.h"
#include "../Input/Input.h"
#include "ConfirmationState.h"
#include "OptionsState.h"

/*****************************************************************
* CPauseState():  Default Constructor. Will set its type and behaviors.
* Ins:			    None    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
CPauseState::CPauseState(void) : m_pGame(nullptr)
{

}

/*****************************************************************
* ~CPauseState():  Virtual destructor used when shutting down the pause state
* Ins:			    none    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
CPauseState::~CPauseState(void)
{
}

/*****************************************************************
* Enter(): Enters the pause state. Will set any variables needed for the pause state
* Ins:			    none    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CPauseState::Enter()
{	
	CInput::SetCursorLock(false);
	m_pGame->GetRenderer()->SetPaused(true);
	WwiseNS::SoundEvent( ePause, ePause);
	WwiseNS::SoundEvent(eBGM_LevelTwoMusic, ePlay);
	m_nButtonHighlighted = ePS_NONE;
	m_bMouseHasPriority = true;
	m_nSwitchState = ePS_NONE;
	m_nAlphaValue = 0;
	m_bChangeState = false;
	m_bAlphaIn = true;
	m_bPressedEscape = false;

	m_nPrevMouseXPos = 0;
	m_nPrevMouseYPos = 0;

	D3DXMatrixIdentity(&m_ReticleMat);

}

/*****************************************************************
* Exit(): Exits the pause state. Will set any variables used to null values
* Ins:			    none    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CPauseState::Exit()
{
	m_pGame->GetRenderer()->SetPaused(false);
}

/*****************************************************************
* Input(): Grabs any input made by the player
* Ins:			    none    		      
* Outs:				None
* Returns:		    bool
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
bool CPauseState::Input()
{
	if(CInput::BufferedButtonPress(eEsc, true) && m_bPressedEscape == false)
	{
		m_bAlphaIn = false;
		m_bPressedEscape = true;
		WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
		WwiseNS::SoundEvent(eBGM_LevelTwoMusic, eStop);
	}
	else if(m_bChangeState == true && m_bPressedEscape == true)
	{
		CInput::SetCursorLock(true);
		WwiseNS::SoundEvent(eResume, eResume);
		WwiseNS::SoundEvent(eBGM_LevelTwoMusic, eStop);
		m_pGame->PopState();
		return true;
	}

	LONG lCursorXPos = m_pGame->GetCursorPosition().x;
	LONG lCursorYPos = m_pGame->GetCursorPosition().y;

	if(m_nButtonHighlighted == ePS_NONE)
	{
		if(CInput::BufferedButtonPress(eStart, true) || CInput::BufferedButtonPress(eUp, true) || CInput::BufferedButtonPress(eUpArrow, true) || 
			CInput::BufferedButtonPress(eLeft, true) || CInput::BufferedButtonPress(eLeftArrow, true) || CInput::BufferedButtonPress(eDown, true) || 
			CInput::BufferedButtonPress(eDownArrow, true) || CInput::BufferedButtonPress(eRight, true) || CInput::BufferedButtonPress(eRightArrow, true)
			|| CInput::DIKBufferedButtonCheck(DIK_SPACE, true))
		{
			//SetCursorPos(0, 0);
			m_nButtonHighlighted = ePS_RESUME;
		}
	}

	if(m_bChangeState == true)
	{
		switch(m_nSwitchState)
		{
		case ePS_RESUME:
			{
				WwiseNS::SoundEvent(eBGM_LevelTwoMusic, eStop);
				WwiseNS::SoundEvent(eResume, eResume);
				CInput::SetCursorLock(true);
				m_pGame->PopState();
				return true;
			}
			break;

		case ePS_OPTIONS:
			{
				m_nAlphaValue = 0;
				m_bChangeState = false;
				m_bAlphaIn = true;
				m_bPressedEscape = false;

				m_pGame->PushState(new COptionsState(m_pGame, eGAME_PLAY_OPTION));
				return true;
			}
			break;

		case ePS_RESTART:
			{
				m_nAlphaValue = 0;
				m_bChangeState = false;
				m_bAlphaIn = true;
				m_bPressedEscape = false;

				m_pGame->PushState(new CConfirmationState(m_pGame, eGAME_PLAY_RESTART));
				return true;
			}
			break;

		case ePS_MAINMENU:
			{
				m_nAlphaValue = 0;
				m_bChangeState = false;
				m_bAlphaIn = true;
				m_bPressedEscape = false;

				m_pGame->PushState(new CConfirmationState(m_pGame, eMAIN_MENU));
				return true;
			}
			break;
		}
	}

	if(CInput::BufferedButtonPress(eStart, true) && m_bAlphaIn == true)
	{
		if(m_nButtonHighlighted == ePS_RESUME)
		{
			WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
			m_bAlphaIn = false;
			m_nSwitchState = ePS_RESUME;
		}
		else if(m_nButtonHighlighted == ePS_OPTIONS)
		{
			WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
			m_bAlphaIn = false;
			m_nSwitchState = ePS_OPTIONS;
		}
		else if(m_nButtonHighlighted == ePS_RESTART)
		{
			WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
			m_bAlphaIn = false;
			m_nSwitchState = ePS_RESTART;
		}
		else if(m_nButtonHighlighted == ePS_MAINMENU)
		{

			WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
			m_bAlphaIn = false;
			m_nSwitchState = ePS_MAINMENU;

		}
	}

	if((CInput::BufferedButtonPress(eUp) || CInput::BufferedButtonPress(eUpArrow) || CInput::BufferedButtonPress(eLeft) || CInput::BufferedButtonPress(eLeftArrow)) && m_bAlphaIn == true)
	{
		WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);

		m_bMouseHasPriority = false;

		if(m_nButtonHighlighted == ePS_RESUME)
		{
			m_nButtonHighlighted = ePS_MAINMENU;
		}
		else if(m_nButtonHighlighted == ePS_MAINMENU)
		{
			m_nButtonHighlighted = ePS_RESTART;
		}
		else if(m_nButtonHighlighted == ePS_RESTART)
		{
			m_nButtonHighlighted = ePS_OPTIONS;
		}
		else if(m_nButtonHighlighted == ePS_OPTIONS)
		{
			m_nButtonHighlighted = ePS_RESUME;
		}
	}
	else if((CInput::BufferedButtonPress(eDown) || CInput::BufferedButtonPress(eDownArrow) || CInput::BufferedButtonPress(eRight) || CInput::BufferedButtonPress(eRightArrow)) && m_bAlphaIn == true)
	{
		WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);

		m_bMouseHasPriority = false;

		if(m_nButtonHighlighted == ePS_RESUME)
		{
			m_nButtonHighlighted = ePS_OPTIONS;
		}
		else if(m_nButtonHighlighted == ePS_OPTIONS)
		{
			m_nButtonHighlighted = ePS_RESTART;
		}
		else if(m_nButtonHighlighted == ePS_RESTART)
		{
			m_nButtonHighlighted = ePS_MAINMENU;
		}
		else if(m_nButtonHighlighted == ePS_MAINMENU)
		{
			m_nButtonHighlighted = ePS_RESUME;
		}
	}

	// EXIT: Exit the game if exit button is pressed
	if(lCursorXPos > s_rExitHighlightedRect.left && lCursorXPos < s_rExitHighlightedRect.right && lCursorYPos > s_rExitHighlightedRect.top && lCursorYPos < s_rExitHighlightedRect.bottom)
	{
		if(m_nButtonHighlighted != ePS_MAINMENU && m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);
		}

		if(m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			m_nButtonHighlighted = ePS_MAINMENU;
		}

		if(CInput::BufferedButtonPress(eLeftMouseClick) && m_bAlphaIn == true)
		{
			WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
			m_bAlphaIn = false;
			m_nSwitchState = ePS_MAINMENU;
		}
	}

	// Restart Checkpoint
	if(lCursorXPos > s_rMainMenuHighlightedRect.left && lCursorXPos < s_rMainMenuHighlightedRect.right && lCursorYPos > s_rMainMenuHighlightedRect.top && lCursorYPos < s_rMainMenuHighlightedRect.bottom)
	{
		if(m_nButtonHighlighted != ePS_RESTART && m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);
		}

		if(m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			m_nButtonHighlighted = ePS_RESTART;
		}

		if(CInput::BufferedButtonPress(eLeftMouseClick, true) && m_bAlphaIn == true)
		{
			WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
			m_bAlphaIn = false;
			m_nSwitchState = ePS_RESTART;
		}
	}

	// OPTIONS: Go to the options state if button is pressed
	if(lCursorXPos > s_rOptionsHighlighedRect.left && lCursorXPos < s_rOptionsHighlighedRect.right && lCursorYPos > s_rOptionsHighlighedRect.top && lCursorYPos < s_rOptionsHighlighedRect.bottom)
	{
		if(m_nButtonHighlighted != ePS_OPTIONS && m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);
		}

		if(m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			m_nButtonHighlighted = ePS_OPTIONS;
		}

		if(CInput::BufferedButtonPress(eLeftMouseClick, true) && m_bAlphaIn == true)
		{
			WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
			m_bAlphaIn = false;
			m_nSwitchState = ePS_OPTIONS;
		}
	}

	// RESUME: Resume the game state if it is pressed
	if(lCursorXPos > s_rResumeHighlightedRect.left && lCursorXPos < s_rResumeHighlightedRect.right && lCursorYPos > s_rResumeHighlightedRect.top && lCursorYPos < s_rResumeHighlightedRect.bottom)
	{
		if(m_nButtonHighlighted != ePS_RESUME && m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);
		}

		if(m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			m_nButtonHighlighted = ePS_RESUME;
		}

		if(CInput::BufferedButtonPress(eLeftMouseClick, true) && m_bAlphaIn == true)
		{
			WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
			m_bAlphaIn = false;
			m_nSwitchState = ePS_RESUME;
		}
	}

	return true;
}

/*****************************************************************
* Update(): Updates the pause state based on the change in time
* Ins:			    fElapsedTime    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CPauseState::Update(float _fElapsedTime)
{
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
* Render(): Renders the pause state by calling the renderer classes render function
* Ins:			    none    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CPauseState::Render()
{
	//m_pGame->GetRenderer()->Render2d( m_pGame->GetMenuTexture( 0 ), m_nAlphaValue / 255.0f);

	m_pGame->GetState(0)->Render();
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

	m_pGame->GetRenderer()->RenderText("GAME PAUSED", 300, 50, 3.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));

	m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetPSBorderXPos(), (float)m_pGame->GetGuiHelper()->GetPSBorderYPos(), false, m_pGame->GetGuiHelper()->GetPSBorderXScale(), m_pGame->GetGuiHelper()->GetPSBorderYScale(), false, 0, 512, 1024, 1075, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));

	if(m_nButtonHighlighted == ePS_RESUME && m_bAlphaIn == false)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetPSResumeXPos(), (float)m_pGame->GetGuiHelper()->GetPSResumeYPos(), eBUTTON_PS_RESUME, m_pGame->GetGuiHelper()->GetPSResumeXScale(), m_pGame->GetGuiHelper()->GetPSResumeYScale(), true, 0, 0, 512, 128, D3DCOLOR_ARGB(m_nAlphaValue, 127, 127, 127));
	}
	else if(m_nButtonHighlighted == ePS_RESUME)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetPSResumeXPos(), (float)m_pGame->GetGuiHelper()->GetPSResumeYPos(), eBUTTON_PS_RESUME, m_pGame->GetGuiHelper()->GetPSResumeXScale(), m_pGame->GetGuiHelper()->GetPSResumeYScale(), true, 0, 0, 512, 128, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}
	else
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetPSResumeXPos(), (float)m_pGame->GetGuiHelper()->GetPSResumeYPos(), eBUTTON_PS_RESUME, m_pGame->GetGuiHelper()->GetPSResumeXScale(), m_pGame->GetGuiHelper()->GetPSResumeYScale(), false, 0, 0, 512, 128, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}

	if(m_nButtonHighlighted == ePS_OPTIONS && m_bAlphaIn == false)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetPSOptionsXPos(), (float)m_pGame->GetGuiHelper()->GetPSOptionsYPos(), eBUTTON_PS_OPTIONS, m_pGame->GetGuiHelper()->GetPSOptionsXScale(), m_pGame->GetGuiHelper()->GetPSOptionsYScale(), true, 0, 128, 512, 256, D3DCOLOR_ARGB(m_nAlphaValue, 127, 127, 127));
	}
	else if(m_nButtonHighlighted == ePS_OPTIONS)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetPSOptionsXPos(), (float)m_pGame->GetGuiHelper()->GetPSOptionsYPos(), eBUTTON_PS_OPTIONS, m_pGame->GetGuiHelper()->GetPSOptionsXScale(), m_pGame->GetGuiHelper()->GetPSOptionsYScale(), true, 0, 128, 512, 256, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}
	else
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetPSOptionsXPos(), (float)m_pGame->GetGuiHelper()->GetPSOptionsYPos(), eBUTTON_PS_OPTIONS, m_pGame->GetGuiHelper()->GetPSOptionsXScale(), m_pGame->GetGuiHelper()->GetPSOptionsYScale(), false, 0, 128, 512, 256, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}

	if(m_nButtonHighlighted == ePS_RESTART && m_bAlphaIn == false)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetPSRestartXPos(), (float)m_pGame->GetGuiHelper()->GetPSRestartYPos(), eBUTTON_PS_RESTART, m_pGame->GetGuiHelper()->GetPSRestartXScale(), m_pGame->GetGuiHelper()->GetPSRestartYScale(), true, 0, 256, 512, 384, D3DCOLOR_ARGB(m_nAlphaValue, 127, 127, 127));
	}
	else if(m_nButtonHighlighted == ePS_RESTART)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetPSRestartXPos(), (float)m_pGame->GetGuiHelper()->GetPSRestartYPos(), eBUTTON_PS_RESTART, m_pGame->GetGuiHelper()->GetPSRestartXScale(), m_pGame->GetGuiHelper()->GetPSRestartYScale(), true, 0, 256, 512, 384, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}
	else
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetPSRestartXPos(), (float)m_pGame->GetGuiHelper()->GetPSRestartYPos(), eBUTTON_PS_RESTART, m_pGame->GetGuiHelper()->GetPSRestartXScale(), m_pGame->GetGuiHelper()->GetPSRestartYScale(), false, 0, 256, 512, 384, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}

	if(m_nButtonHighlighted == ePS_MAINMENU && m_bAlphaIn == false)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetPSMainMenuXPos(), (float)m_pGame->GetGuiHelper()->GetPSMainMenuYPos(), eBUTTON_PS_MAINMENU, m_pGame->GetGuiHelper()->GetPSMainMenuXScale(), m_pGame->GetGuiHelper()->GetPSMainMenuYScale(), true, 0, 384, 512, 512, D3DCOLOR_ARGB(m_nAlphaValue, 127, 127, 127));
	}
	else if(m_nButtonHighlighted == ePS_MAINMENU)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetPSMainMenuXPos(), (float)m_pGame->GetGuiHelper()->GetPSMainMenuYPos(), eBUTTON_PS_MAINMENU, m_pGame->GetGuiHelper()->GetPSMainMenuXScale(), m_pGame->GetGuiHelper()->GetPSMainMenuYScale(), true, 0, 384, 512, 512, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}
	else
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetPSMainMenuXPos(), (float)m_pGame->GetGuiHelper()->GetPSMainMenuYPos(), eBUTTON_PS_MAINMENU, m_pGame->GetGuiHelper()->GetPSMainMenuXScale(), m_pGame->GetGuiHelper()->GetPSMainMenuYScale(), false, 0, 384, 512, 512, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}


	if(m_nButtonHighlighted == ePS_RESUME && m_bAlphaIn == false)
	{
		m_pGame->GetRenderer()->RenderText("RESUME", m_pGame->GetGuiHelper()->GetPSResumeTextXPos(), m_pGame->GetGuiHelper()->GetPSResumeTextYPos(), m_pGame->GetGuiHelper()->GetPSResumeTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255, 100, 100));
	}
	else
	{
		m_pGame->GetRenderer()->RenderText("RESUME", m_pGame->GetGuiHelper()->GetPSResumeTextXPos(), m_pGame->GetGuiHelper()->GetPSResumeTextYPos(), m_pGame->GetGuiHelper()->GetPSResumeTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
	}

	if(m_nButtonHighlighted == ePS_OPTIONS && m_bAlphaIn == false)
	{
		m_pGame->GetRenderer()->RenderText("OPTIONS", m_pGame->GetGuiHelper()->GetPSOptionsTextXPos(), m_pGame->GetGuiHelper()->GetPSOptionsTextYPos(), m_pGame->GetGuiHelper()->GetPSOptionsTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255, 100, 100));
	}
	else
	{
		m_pGame->GetRenderer()->RenderText("OPTIONS", m_pGame->GetGuiHelper()->GetPSOptionsTextXPos(), m_pGame->GetGuiHelper()->GetPSOptionsTextYPos(), m_pGame->GetGuiHelper()->GetPSOptionsTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
	}

	if(m_nButtonHighlighted == ePS_RESTART && m_bAlphaIn == false)
	{
		m_pGame->GetRenderer()->RenderText("RESTART", m_pGame->GetGuiHelper()->GetPSRestartTextXPos(), m_pGame->GetGuiHelper()->GetPSRestartTextYPos(), m_pGame->GetGuiHelper()->GetPSRestartTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255, 100, 100));
	}
	else
	{
		m_pGame->GetRenderer()->RenderText("RESTART", m_pGame->GetGuiHelper()->GetPSRestartTextXPos(), m_pGame->GetGuiHelper()->GetPSRestartTextYPos(), m_pGame->GetGuiHelper()->GetPSRestartTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
	}

	if(m_nButtonHighlighted == ePS_MAINMENU && m_bAlphaIn == false)
	{
		m_pGame->GetRenderer()->RenderText("MAIN MENU", m_pGame->GetGuiHelper()->GetPSMainMenuTextXPos(), m_pGame->GetGuiHelper()->GetPSMainMenuTextYPos(), m_pGame->GetGuiHelper()->GetPSMainMenuTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255, 100, 100));
	}
	else
	{
		m_pGame->GetRenderer()->RenderText("MAIN MENU", m_pGame->GetGuiHelper()->GetPSMainMenuTextXPos(), m_pGame->GetGuiHelper()->GetPSMainMenuTextYPos(), m_pGame->GetGuiHelper()->GetPSMainMenuTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
	}


	D3DXMATRIX d3dTmpMat;	
	D3DXMatrixTranslation(&d3dTmpMat, 810 * xRatio, 690 * yRatio, 0);
	RECT rArrowKeyRect = {0,0,64,32};
	m_pGame->GetRenderer()->RenderArrowAndrewR(m_pGame->GetArrow306Texture(), &rArrowKeyRect, d3dTmpMat,D3DCOLOR_ARGB(255,255,255,255),true,false);
	m_pGame->GetRenderer()->RenderText("Change",    871, 676, 0.8f, 0, 0, 0, 0xff000000);
	m_pGame->GetRenderer()->RenderText("Change",    870, 675, 0.8f, 0, 0, 0, 0xfffffffF);
	m_pGame->GetRenderer()->RenderText("Selection", 871, 690, 0.8f, 0, 0, 0, 0xff000000);
	m_pGame->GetRenderer()->RenderText("Selection", 870, 689, 0.8f, 0, 0, 0, 0xfffffffF);


	D3DXMATRIX d3dTmpMat2;
	D3DXMatrixTranslation(&d3dTmpMat2, 810 * xRatio, 740 * yRatio, 0);
	RECT rTempEnterRect = {0,0,64,32};
	m_pGame->GetRenderer()->RenderArrowAndrewR(m_pGame->GetEnterKeyTexture(), &rTempEnterRect, d3dTmpMat2,D3DCOLOR_ARGB(255,255,255,255),true,false);
	m_pGame->GetRenderer()->RenderText("Confirm",   871, 726, 0.8f, 0, 0, 0, 0xff000000);
	m_pGame->GetRenderer()->RenderText("Confirm",   870, 725, 0.8f, 0, 0, 0, 0xfffffffF);
	m_pGame->GetRenderer()->RenderText("Selection", 871, 740, 0.8f, 0, 0, 0, 0xff000000);
	m_pGame->GetRenderer()->RenderText("Selection", 870, 739, 0.8f, 0, 0, 0, 0xfffffffF);

	RECT m_rArrowRect = {0, 0, 64, 64};
	if (m_bAlphaIn)
	{

		m_pGame->GetRenderer()->RenderArrowAndrewR(m_pGame->GetMenuCursorTexture(), &m_rArrowRect,  m_ReticleMat,D3DCOLOR_ARGB(255, 255,0,0) , true, false);
	}

}
