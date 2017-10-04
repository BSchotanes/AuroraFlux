#include "../StdAfx.h"
#include "MainMenuState.h"
#include "GameOverState.h"
#include "GamePlayState.h"
#include "../Input/Input.h"
#include "ConfirmationState.h"

/*****************************************************************
* CGameOverState():  Default Constructor. Will set its type and behaviors.
* Ins:			    None    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*******************************************************************/
CGameOverState::CGameOverState(void)
{
	m_pGame = nullptr;	
}

/*****************************************************************
* CGameOverState():  Overloaded Constructor. Will be used to create new states.
* Ins:			    _pGame    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
CGameOverState::CGameOverState(CGame* _pGame) 
{
	m_pGame = _pGame;
}

/*****************************************************************
* ~CGameOverState():  Virtual destructor used when shutting down the game state
* Ins:			    None    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
CGameOverState::~CGameOverState(void)
{
}

/*****************************************************************
* Enter(): Enters the game over state. Will set any variables needed for the main menu state
* Ins:			    None    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CGameOverState::Enter()
{
	CInput::SetCursorLock(false);
	WwiseNS::SoundEvent(ePause, ePause);

	m_pGame->RevertToSavedStats();
	m_pGame->GetCurrentStats()->DiedStats();
	m_pGame->SaveCurrentStats();

	m_nAlphaValue = 0;
	m_bChangeState = false;
	m_bAlphaIn = true;

	D3DXMatrixIdentity(&m_ReticleMat);

	m_bMouseHasPriority = true;
	m_nPrevMouseXPos = 0;
	m_nPrevMouseYPos = 0;
	m_nSwitchState = eGO_NONE;
}

/*****************************************************************
* Exit(): Exits the game over state. Will set any variables used to null values
* Ins:			    None    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CGameOverState::Exit()
{
}

/*****************************************************************
* Input(): Grabs any input made by the player
* Ins:			    None    		      
* Outs:				None
* Returns:		    bool
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
bool CGameOverState::Input()
{
	LONG lCursorXPos = m_pGame->GetCursorPosition().x;
	LONG lCursorYPos = m_pGame->GetCursorPosition().y;

	if(m_nButtonHighlighted == eGO_NONE)
	{
		if(CInput::BufferedButtonPress(eStart, true) || CInput::BufferedButtonPress(eUp, true) || CInput::BufferedButtonPress(eUpArrow, true) || 
			CInput::BufferedButtonPress(eLeft, true) || CInput::BufferedButtonPress(eLeftArrow, true) || CInput::BufferedButtonPress(eDown, true) || 
			CInput::BufferedButtonPress(eDownArrow, true) || CInput::BufferedButtonPress(eRight, true) || CInput::BufferedButtonPress(eRightArrow, true)
			|| CInput::DIKBufferedButtonCheck(DIK_SPACE, true))
		{
			//SetCursorPos(0, 0);
			m_nButtonHighlighted = eGO_CONTINUE;
		}
	}

	if(m_bChangeState == true)
	{
		switch(m_nSwitchState)
		{
		case eGO_CONTINUE:
			{
				m_nAlphaValue = 0;
				m_bChangeState = false;
				m_bAlphaIn = true;

				m_pGame->GetCamera()->ResetCameraToCheckPoint();
				GamePlayState* pNewState = new GamePlayState(m_pGame, true); 
				pNewState->FirstEnter();
				pNewState->SecondEnter();
				pNewState->ThirdEnter();
				pNewState->FourthEnter();

				CGame* _pGame = m_pGame;
				m_pGame->switchState(pNewState);
				pNewState->GetScriptManager()->LoadCheckpoint();
				return true;
			}
			break;

		case eGO_RESTART:
			{
				m_nAlphaValue = 0;
				m_bChangeState = false;
				m_bAlphaIn = true;

				WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
				m_pGame->PushState(new CConfirmationState(m_pGame, eNEW_GAME_PLAY));
				return true;
			}
			break;

		case eGO_MAINMENU:
			{
				m_nAlphaValue = 0;
				m_bChangeState = false;
				m_bAlphaIn = true;

				m_pGame->PushState(new CConfirmationState(m_pGame, eMAIN_MENU));
				return true;
			}
			break;
		}
	}

	if(CInput::BufferedButtonPress(eStart, true) && m_bAlphaIn == true)
	{
		if(m_nButtonHighlighted == eGO_CONTINUE)
		{
			WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
			m_bAlphaIn = false;
			m_nSwitchState = eGO_CONTINUE;
		}
		else if(m_nButtonHighlighted == eGO_RESTART)
		{
			if(CInput::BufferedButtonPress(eStart, true) && m_bAlphaIn == true)
			{
				WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
				m_bAlphaIn = false;
				m_nSwitchState = eGO_RESTART;
			}
		}
		else if(m_nButtonHighlighted == eGO_MAINMENU)
		{
			if(CInput::BufferedButtonPress(eStart, true) && m_bAlphaIn == true)
			{
				WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
				m_bAlphaIn = false;
				m_nSwitchState = eGO_RESTART;
			}
		}
	}



	if((CInput::BufferedButtonPress(eUp) || CInput::BufferedButtonPress(eUpArrow) || CInput::BufferedButtonPress(eLeft) || CInput::BufferedButtonPress(eLeftArrow)) && m_bAlphaIn ==true)
	{
		m_bMouseHasPriority = false;
		//SetCursorPos(0, 0);
		WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);

		if(m_nButtonHighlighted == eGO_CONTINUE)
		{
			m_nButtonHighlighted = eGO_MAINMENU;
		}
		else if(m_nButtonHighlighted == eGO_RESTART)
		{
			m_nButtonHighlighted = eGO_CONTINUE;
		}
		else if(m_nButtonHighlighted == eGO_MAINMENU)
		{
			m_nButtonHighlighted = eGO_RESTART;
		}
	}
	else if((CInput::BufferedButtonPress(eDown) || CInput::BufferedButtonPress(eDownArrow) || CInput::BufferedButtonPress(eRight) || CInput::BufferedButtonPress(eRightArrow)) && m_bAlphaIn ==true)
	{
		m_bMouseHasPriority = false;
		//SetCursorPos(0, 0);
		WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);

		if(m_nButtonHighlighted == eGO_CONTINUE)
		{
			m_nButtonHighlighted = eGO_RESTART;
		}
		else if(m_nButtonHighlighted == eGO_RESTART)
		{
			m_nButtonHighlighted = eGO_MAINMENU;
		}
		else if(m_nButtonHighlighted == eGO_MAINMENU)
		{
			m_nButtonHighlighted = eGO_CONTINUE;
		}
	}


	// Main Menu: Go to the main menu state if button is pressed
	if(lCursorXPos > s_rGOExitButtonRect.left && lCursorXPos < s_rGOExitButtonRect.right &&	lCursorYPos > s_rGOExitButtonRect.top && lCursorYPos < s_rGOExitButtonRect.bottom)
	{
		if(m_nButtonHighlighted != eGO_MAINMENU && m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);
		}

		if(m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			m_nButtonHighlighted = eGO_MAINMENU;
		}

		if(CInput::BufferedButtonPress(eLeftMouseClick, true) && m_bAlphaIn == true)
		{
			WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
			m_bAlphaIn = false;
			m_nSwitchState = eGO_MAINMENU;
		}
	}

	// RESTART: Start a new game from the beginning
	if(lCursorXPos > s_rGOMainMenuRect.left && lCursorXPos < s_rGOMainMenuRect.right &&	lCursorYPos > s_rGOMainMenuRect.top && lCursorYPos < s_rGOMainMenuRect.bottom)
	{
		if(m_nButtonHighlighted != eGO_RESTART && m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);
		}

		if(m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			m_nButtonHighlighted = eGO_RESTART;
		}

		if(CInput::BufferedButtonPress(eLeftMouseClick, true) && m_bAlphaIn == true)
		{
			WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
			m_bAlphaIn = false;
			m_nSwitchState = eGO_RESTART;
		}
	}

	//Continue: Retry from the last checkpoint
	if(lCursorXPos > s_rGOContinueButtonRect.left && lCursorXPos < s_rGOContinueButtonRect.right &&	lCursorYPos > s_rGOContinueButtonRect.top && lCursorYPos < s_rGOContinueButtonRect.bottom)
	{
		if(m_nButtonHighlighted != eGO_CONTINUE && m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);
		}

		if(m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			m_nButtonHighlighted = eGO_CONTINUE;
		}

		if(CInput::BufferedButtonPress(eLeftMouseClick, true) && m_bAlphaIn == true)
		{
			WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
			m_bAlphaIn = false;
			m_nSwitchState = eGO_CONTINUE;
		}
	}

	return true;
}

/*****************************************************************
* Update(): Updates the game over state based on the change in time
* Ins:			    _fElapsedTime    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CGameOverState::Update(float _fElapsedTime)
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

	m_nPrevMouseXPos = m_pGame->GetCursorPosition().x;
	m_nPrevMouseYPos = m_pGame->GetCursorPosition().y;

	m_ReticleMat._41 = float(m_pGame->GetCursorPosition().x);
	m_ReticleMat._42 = float(m_pGame->GetCursorPosition().y);
}

/*****************************************************************
* Render(): Renders the game over state by calling the renderer classes render function
* Ins:			    none    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CGameOverState::Render()
{

	float xRatio = m_pGame->GetRenderer()->GetXRatio();
	float yRatio = m_pGame->GetRenderer()->GetYRatio();
	m_pGame->GetRenderer()->Render2d(m_pGame->GetMenuTexture(0), m_nAlphaValue / 255.0f);

	m_pGame->GetRenderer()->RenderText("GAME OVER", 300, 50, 3.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));

	m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetGOBorderXPos(), (float)m_pGame->GetGuiHelper()->GetGOBorderYPos(), false, m_pGame->GetGuiHelper()->GetGOBorderXScale(), m_pGame->GetGuiHelper()->GetGOBorderYScale(), false, 0, 512, 1024, 1075, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));


	if(m_nButtonHighlighted == eGO_CONTINUE && m_bAlphaIn == false)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetGOContinueXPos(),(float)m_pGame->GetGuiHelper()->GetGOContinueYPos(), eBUTTON_GO_RETRY, m_pGame->GetGuiHelper()->GetGOContinueXScale(), m_pGame->GetGuiHelper()->GetGOContinueYScale(), true, 0, 0, 512, 128, D3DCOLOR_ARGB(m_nAlphaValue, 127, 127, 127));
	}
	else if(m_nButtonHighlighted == eGO_CONTINUE)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetGOContinueXPos(),(float)m_pGame->GetGuiHelper()->GetGOContinueYPos(), eBUTTON_GO_RETRY, m_pGame->GetGuiHelper()->GetGOContinueXScale(), m_pGame->GetGuiHelper()->GetGOContinueYScale(), true, 0, 0, 512, 128, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}
	else
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetGOContinueXPos(),(float)m_pGame->GetGuiHelper()->GetGOContinueYPos(), eBUTTON_GO_RETRY, m_pGame->GetGuiHelper()->GetGOContinueXScale(), m_pGame->GetGuiHelper()->GetGOContinueYScale(), false, 0, 0, 512, 128, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}

	if(m_nButtonHighlighted == eGO_RESTART && m_bAlphaIn == false)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetGORestartXPos(),(float)m_pGame->GetGuiHelper()->GetGORestartYPos(), eBUTTON_GO_RESTART, m_pGame->GetGuiHelper()->GetGORestartXScale(), m_pGame->GetGuiHelper()->GetGORestartYScale(), true, 0, 128, 512, 256, D3DCOLOR_ARGB(m_nAlphaValue, 127, 127, 127));
	}
	else if(m_nButtonHighlighted == eGO_RESTART)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetGORestartXPos(),(float)m_pGame->GetGuiHelper()->GetGORestartYPos(), eBUTTON_GO_RESTART, m_pGame->GetGuiHelper()->GetGORestartXScale(), m_pGame->GetGuiHelper()->GetGORestartYScale(), true, 0, 128, 512, 256, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}
	else
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetGORestartXPos(),(float)m_pGame->GetGuiHelper()->GetGORestartYPos(), eBUTTON_GO_RESTART, m_pGame->GetGuiHelper()->GetGORestartXScale(), m_pGame->GetGuiHelper()->GetGORestartYScale(), false, 0, 128, 512, 256, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}

	if(m_nButtonHighlighted == eGO_MAINMENU && m_bAlphaIn == false)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetGOMainMenuXPos(),(float)m_pGame->GetGuiHelper()->GetGOMainMenuYPos(), eBUTTON_GO_MAINMENU, m_pGame->GetGuiHelper()->GetGOMainMenuXScale(), m_pGame->GetGuiHelper()->GetGOMainMenuYScale(), true, 0, 256, 512, 384, D3DCOLOR_ARGB(m_nAlphaValue, 127, 127, 127));
	}
	else if(m_nButtonHighlighted == eGO_MAINMENU)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetGOMainMenuXPos(),(float)m_pGame->GetGuiHelper()->GetGOMainMenuYPos(), eBUTTON_GO_MAINMENU, m_pGame->GetGuiHelper()->GetGOMainMenuXScale(), m_pGame->GetGuiHelper()->GetGOMainMenuYScale(), true, 0, 256, 512, 384, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}
	else
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetGOMainMenuXPos(),(float)m_pGame->GetGuiHelper()->GetGOMainMenuYPos(), eBUTTON_GO_MAINMENU, m_pGame->GetGuiHelper()->GetGOMainMenuXScale(), m_pGame->GetGuiHelper()->GetGOMainMenuYScale(), false, 0, 256, 512, 384, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}


	if(m_nButtonHighlighted == eGO_CONTINUE && m_bAlphaIn == false)
	{
		m_pGame->GetRenderer()->RenderText("CONTINUE", m_pGame->GetGuiHelper()->GetGOContinueTextXPos(), m_pGame->GetGuiHelper()->GetGOContinueTextYPos(), m_pGame->GetGuiHelper()->GetGOContinueTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255, 100, 100));
	}
	else
	{
		m_pGame->GetRenderer()->RenderText("CONTINUE", m_pGame->GetGuiHelper()->GetGOContinueTextXPos(), m_pGame->GetGuiHelper()->GetGOContinueTextYPos(), m_pGame->GetGuiHelper()->GetGOContinueTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
	}
	
	if(m_nButtonHighlighted == eGO_RESTART && m_bAlphaIn == false)
	{
		m_pGame->GetRenderer()->RenderText("RESTART", m_pGame->GetGuiHelper()->GetGORestartTextXPos(), m_pGame->GetGuiHelper()->GetGORestartTextYPos(), m_pGame->GetGuiHelper()->GetGORestartTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255, 100, 100));
	}
	else
	{
		m_pGame->GetRenderer()->RenderText("RESTART", m_pGame->GetGuiHelper()->GetGORestartTextXPos(), m_pGame->GetGuiHelper()->GetGORestartTextYPos(), m_pGame->GetGuiHelper()->GetGORestartTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
	}
	
	if(m_nButtonHighlighted == eGO_MAINMENU && m_bAlphaIn == false)
	{
		m_pGame->GetRenderer()->RenderText("MAIN MENU", m_pGame->GetGuiHelper()->GetGOMainMenuTextXPos(), m_pGame->GetGuiHelper()->GetGOMainMenuTextYPos(), m_pGame->GetGuiHelper()->GetGOMainMenuTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255, 100, 100));
	}
	else
	{
		m_pGame->GetRenderer()->RenderText("MAIN MENU", m_pGame->GetGuiHelper()->GetGOMainMenuTextXPos(), m_pGame->GetGuiHelper()->GetGOMainMenuTextYPos(), m_pGame->GetGuiHelper()->GetGOMainMenuTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
	}

	
	D3DXMATRIX tmpMat;	D3DXMatrixTranslation(&tmpMat, 810 * xRatio, 690 * yRatio, 0);
	RECT tmpR = {0,0,64,32};
	m_pGame->GetRenderer()->RenderArrowAndrewR(m_pGame->GetArrow306Texture(), &tmpR, tmpMat,  D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255), true, false);
	m_pGame->GetRenderer()->RenderText("Change",    871, 676, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
	m_pGame->GetRenderer()->RenderText("Change",    870, 675, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	m_pGame->GetRenderer()->RenderText("Selection", 871, 690, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
	m_pGame->GetRenderer()->RenderText("Selection", 870, 689, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));


	D3DXMATRIX tmpMat2;	D3DXMatrixTranslation(&tmpMat2, 810 * xRatio, 735 * yRatio, 0);
	RECT tmpR2 = {0,0,64,32};
	m_pGame->GetRenderer()->RenderArrowAndrewR(m_pGame->GetEnterKeyTexture(), &tmpR2, tmpMat2,  D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255),true, false);
	m_pGame->GetRenderer()->RenderText("Confirm",    871, 726, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
	m_pGame->GetRenderer()->RenderText("Confirm",    870, 725, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	m_pGame->GetRenderer()->RenderText("Selection",  871, 740, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
	m_pGame->GetRenderer()->RenderText("Selection",  870, 739, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));

	RECT m_rArrowRect = {0, 0, 64, 64};
	if (m_bAlphaIn)
	{
		m_pGame->GetRenderer()->RenderArrowAndrewR(m_pGame->GetMenuCursorTexture(), &m_rArrowRect,  m_ReticleMat,D3DCOLOR_ARGB(255, 255,0,0) );
	}

}