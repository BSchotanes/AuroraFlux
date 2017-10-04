/***********************************************
* Filename:  		CConfirmationState.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AG
* Author:    		Andrew Goossens
* Purpose:   		This will display the confirmation screen
*						when the player attempts to leave the gameplay state
*					
************************************************/
#include "../StdAfx.h"
#include "MainMenuState.h"
#include "GamePlayState.h"
#include "ConfirmationState.h"
#include "../Input/Input.h"
#include "OptionsState.h"

/*****************************************************************
* CCreditsState():  Default Constructor. Will set its type and behaviors.
* Ins:			    None    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
CConfirmationState::CConfirmationState(void)
{

}

/*****************************************************************
* CConfirmationState():  Overloaded Constructor. Will be used to create new states.
* Ins:			    _pGame
_eStates    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
CConfirmationState::CConfirmationState(CGame * _pGame, GameStates _eState)
{ 
	m_pGame = _pGame; 
	m_nGameState = _eState;

	m_nAlphaValue = 0;
	m_bChangeState = false;
	m_bAlphaIn = true;
}
/*****************************************************************
* ~CCreditsState():  Virtual destructor used when shutting down the confirmation state
* Ins:			    none    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
CConfirmationState::~CConfirmationState(void)
{
}

/*****************************************************************
* Enter(): Enters the confirmation state. Will set any variables needed for the confirmation state
* Ins:			    none    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CConfirmationState::Enter()
{
	m_nAlphaValue = 0;
	m_bChangeState = false;
	m_bAlphaIn = true;
	m_bPressedEscape = false;

	D3DXMatrixIdentity(&m_ReticleMat);

	m_bMouseHasPriority = true;
	m_nPrevMouseXPos = 0;
	m_nPrevMouseYPos = 0;
	m_nButtonHighlighted = eCS_NONE;
}

/*****************************************************************
* Exit(): Exits the confirmation state. Will set any variables used to null values
* Ins:			    none    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CConfirmationState::Exit()
{
}

/*****************************************************************
* Input(): Grabs any input made by the player
* Ins:			    none    		      
* Outs:				None
* Returns:		    bool
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
bool CConfirmationState::Input()
{
	LONG lCursorXPos = m_pGame->GetCursorPosition().x;
	LONG lCursorYPos = m_pGame->GetCursorPosition().y;

	if(m_nButtonHighlighted == eCS_NONE)
	{
		if(CInput::BufferedButtonPress(eStart, true) || CInput::BufferedButtonPress(eUp, true) || CInput::BufferedButtonPress(eUpArrow, true) || 
			CInput::BufferedButtonPress(eLeft, true) || CInput::BufferedButtonPress(eLeftArrow, true) || CInput::BufferedButtonPress(eDown, true) || 
			CInput::BufferedButtonPress(eDownArrow, true) || CInput::BufferedButtonPress(eRight, true) || CInput::BufferedButtonPress(eRightArrow, true)
			|| CInput::DIKBufferedButtonCheck(DIK_SPACE, true))
		{
			//SetCursorPos(0, 0);
			m_nButtonHighlighted = eCS_YES;
		}
	}

	if(m_bChangeState == true)
	{
		if(m_nSwitchState == eCS_YES)
		{
			int nStateSize = m_pGame->GetStateSize();

			if(m_nGameState != eOPTIONS_APPLY && m_nGameState != eOPTIONS_CANCEL && m_nGameState != eOPTIONS_DEFAULT)
			{
				for(int i = 0; i < nStateSize - 1; ++i)
				{
					m_pGame->PopBottom();
				}
			}

			if(m_nGameState == eMAIN_MENU)
			{
				m_pGame->GetCamera()->ResetCamera();
				m_pGame->switchState(new CMainMenuState(m_pGame));
				return true;
			}
			else if(m_nGameState == eGAME_PLAY_RESTART)
			{
				m_pGame->GetCamera()->ResetCameraToCheckPoint();
				GamePlayState* pNewState = new GamePlayState(m_pGame, false); 
				pNewState->FirstEnter();
				pNewState->SecondEnter();
				pNewState->ThirdEnter();
				pNewState->FourthEnter();
				CGame* pGame = m_pGame;
				m_pGame->switchState(pNewState);
				pNewState->GetScriptManager()->LoadCheckpoint();
				return true;
			}
			else if(m_nGameState == eNEW_GAME_PLAY)
			{
				GamePlayState* pNewState = new GamePlayState(m_pGame, false);
				pNewState->FirstEnter();
				pNewState->SecondEnter();
				pNewState->ThirdEnter();
				pNewState->FourthEnter();
				m_pGame->ResetCurrentStats();
				m_pGame->SaveCurrentStats();
				m_pGame->SetCheckPoint(0);
				m_pGame->GetCamera()->ResetCamera();
				m_pGame->switchState(pNewState);
				return true;
			}
			else if(m_nGameState == eEXIT_GAME)
			{

				return false;
			}
			else if(m_nGameState == eOPTIONS_APPLY)
			{
				m_pGame->SetOptionsApply(true);
				m_pGame->PopState();
				return true;
			}
			else if(m_nGameState == eOPTIONS_CANCEL)
			{
				m_pGame->SetOptionsCancel(true);
				m_pGame->PopState();
				return true;
			}
			else if(m_nGameState == eOPTIONS_DEFAULT)
			{
				m_pGame->SetOptionsDefault(true);
				m_pGame->PopState();
				return true;
			}

		}
		else if(m_nSwitchState == eCS_NO)
		{
			if(m_nGameState == eEXIT_GAME)
			{
				m_pGame->GetState(0)->Enter();
				//	m_pGame->SetPressStartChecked(false);
				//	m_pGame->switchState(new CMainMenuState(m_pGame));
				//	return true;
			}
			//else
			//{
			//m_pGame->GetState(0)->Enter();
			m_pGame->PopState();
			//	return true;
			//}
			//
			return true;
		}
	}

	if(CInput::BufferedButtonPress(eEsc, true) && m_bPressedEscape == false)
	{
		WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
		m_bAlphaIn = false;
		m_bPressedEscape = true;
	}
	else if(m_bChangeState == true && m_bPressedEscape == true)
	{
		//if(m_nGameState == eEXIT_GAME)
		//{
		//	//m_pGame->GetState(0)->Enter();
		//
		//	m_pGame->switchState(new CMainMenuState(m_pGame));
		//	return true;
		//}
		//else
		//{
		m_pGame->GetState(0)->Enter();
		m_pGame->PopState();
		//	return true;
		//}

		return true;
	}

	if(CInput::BufferedButtonPress(eStart, true) && m_bAlphaIn == true)
	{
		if(m_nButtonHighlighted == eCS_YES)
		{
			WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
			m_bAlphaIn = false;
			m_nSwitchState = eCS_YES;
		}	
		else if(m_nButtonHighlighted == eCS_NO)
		{
			WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
			m_bAlphaIn = false;
			m_nSwitchState = eCS_NO;
		}
	}


	if((CInput::BufferedButtonPress(eUp) || CInput::BufferedButtonPress(eUpArrow) || CInput::BufferedButtonPress(eLeft) || CInput::BufferedButtonPress(eLeftArrow)) && m_bAlphaIn == true)
	{
		//SetCursorPos(0, 0);
		m_bMouseHasPriority = false;
		WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);

		if(m_nButtonHighlighted == eCS_YES)
		{
			m_nButtonHighlighted = eCS_NO;
		}
		else if(m_nButtonHighlighted == eCS_NO)
		{
			m_nButtonHighlighted = eCS_YES;
		}
	}
	else if((CInput::BufferedButtonPress(eDown) || CInput::BufferedButtonPress(eDownArrow) || CInput::BufferedButtonPress(eRight) || CInput::BufferedButtonPress(eRightArrow)) && m_bAlphaIn == true)
	{
		//SetCursorPos(0, 0);
		m_bMouseHasPriority = false;
		WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);

		if(m_nButtonHighlighted == eCS_YES)
		{
			m_nButtonHighlighted = eCS_NO;
		}
		else if(m_nButtonHighlighted == eCS_NO)
		{
			m_nButtonHighlighted = eCS_YES;
		}
	}

	// NO:
	if(lCursorXPos > s_rNoButtonRect.left && lCursorXPos < s_rNoButtonRect.right &&	lCursorYPos > s_rNoButtonRect.top && lCursorYPos < s_rNoButtonRect.bottom)
	{
		if(m_nButtonHighlighted != eCS_NO && m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);
		}

		if(m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			m_nButtonHighlighted = eCS_NO;
		}

		if(CInput::BufferedButtonPress(eLeftMouseClick, true) && m_bAlphaIn == true)
		{
			WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
			m_bAlphaIn = false;
			m_nSwitchState = eCS_NO;
		}
	}

	// YES:
	if(lCursorXPos > s_rYesButtonRect.left && lCursorXPos < s_rYesButtonRect.right &&	lCursorYPos > s_rYesButtonRect.top && lCursorYPos < s_rYesButtonRect.bottom)
	{
		if(m_nButtonHighlighted != eCS_YES && m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);
		}

		if(m_bAlphaIn == true && m_bMouseHasPriority == true)
		{
			m_nButtonHighlighted = eCS_YES;
		}

		if(CInput::BufferedButtonPress(eLeftMouseClick, true) && m_bAlphaIn == true)
		{
			WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
			m_bAlphaIn = false;
			m_nSwitchState = eCS_YES;
		}
	}

	return true;
}

/*****************************************************************
* Update(): Updates the confirmation state based on the change in time
* Ins:			    fElapsedTime    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CConfirmationState::Update(float _fElapsedTime)
{
	if(m_pGame->GetFromMainMenu() == true)
	{
		m_pGame->GetState(0)->Update(_fElapsedTime);
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
* Render(): Renders the confirmation state by calling the renderer classes render function
* Ins:			    none    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CConfirmationState::Render()
{
	float xRatio = m_pGame->GetRenderer()->GetXRatio();
	float yRatio = m_pGame->GetRenderer()->GetYRatio();

	m_pGame->GetState(0)->Render();

	RECT rBlackRect = {0, 0, 100, 100};
	D3DXMATRIX d3dBlackMat;
	D3DXMATRIX d3dBlackScale;
	D3DXMatrixIdentity(&d3dBlackMat);
	D3DXMatrixIdentity(&d3dBlackScale);

	d3dBlackMat._41 = 537.0f * xRatio;
	d3dBlackMat._42 = 379.0f * yRatio;

	D3DXMatrixScaling(&d3dBlackScale, 8.5f * xRatio, 7.0f * yRatio, 2.0f);

	m_pGame->GetRenderer()->RenderArrowAndrewR(m_pGame->GetBlackTexture(), &rBlackRect,  (d3dBlackScale * d3dBlackMat), D3DCOLOR_ARGB(200,255,255,255), true, false);




	m_pGame->GetRenderer()->RenderText("ARE YOU SURE?", 300, 50, 3.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));

	//m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetCSBorderXPos(), (float)m_pGame->GetGuiHelper()->GetCSBorderYPos(), false, m_pGame->GetGuiHelper()->GetCSBorderXScale(), m_pGame->GetGuiHelper()->GetCSBorderYScale(), false, 0, 512, 1024, 1075, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));

	if(m_nButtonHighlighted == eCS_YES && m_bAlphaIn == false)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetCSYesXPos(),(float)m_pGame->GetGuiHelper()->GetCSYesYPos(),eBUTTON_CF_YES, m_pGame->GetGuiHelper()->GetCSYesXScale(), m_pGame->GetGuiHelper()->GetCSYesYScale(), true, 0, 128, 512, 256, D3DCOLOR_ARGB(m_nAlphaValue, 127, 127, 127));
	}
	else if(m_nButtonHighlighted == eCS_YES)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetCSYesXPos(),(float)m_pGame->GetGuiHelper()->GetCSYesYPos(),eBUTTON_CF_YES, m_pGame->GetGuiHelper()->GetCSYesXScale(), m_pGame->GetGuiHelper()->GetCSYesYScale(), true, 0, 128, 512, 256, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}
	else
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetCSYesXPos(),(float)m_pGame->GetGuiHelper()->GetCSYesYPos(),eBUTTON_CF_YES, m_pGame->GetGuiHelper()->GetCSYesXScale(), m_pGame->GetGuiHelper()->GetCSYesYScale(), false, 0, 128, 512, 256, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}

	if(m_nButtonHighlighted == eCS_NO && m_bAlphaIn == false)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetCSNoXPos(),(float)m_pGame->GetGuiHelper()->GetCSNoYPos(),eBUTTON_CF_NO, m_pGame->GetGuiHelper()->GetCSNoXScale(), m_pGame->GetGuiHelper()->GetCSNoYScale(), true, 0, 256, 512, 384, D3DCOLOR_ARGB(m_nAlphaValue, 127, 127, 127));
	}
	else if(m_nButtonHighlighted == eCS_NO)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetCSNoXPos(),(float)m_pGame->GetGuiHelper()->GetCSNoYPos(),eBUTTON_CF_NO, m_pGame->GetGuiHelper()->GetCSNoXScale(), m_pGame->GetGuiHelper()->GetCSNoYScale(), true, 0, 256, 512, 384, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}
	else
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetCSNoXPos(),(float)m_pGame->GetGuiHelper()->GetCSNoYPos(),eBUTTON_CF_NO, m_pGame->GetGuiHelper()->GetCSNoXScale(), m_pGame->GetGuiHelper()->GetCSNoYScale(), false, 0, 256, 512, 384, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	}

	if(m_nButtonHighlighted == eCS_YES && m_bAlphaIn == false)
	{
		m_pGame->GetRenderer()->RenderText("YES", m_pGame->GetGuiHelper()->GetCSYesTextXPos(), m_pGame->GetGuiHelper()->GetCSYesTextYPos(), m_pGame->GetGuiHelper()->GetCSYesTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255, 100, 100));
	}
	else
	{
		m_pGame->GetRenderer()->RenderText("YES", m_pGame->GetGuiHelper()->GetCSYesTextXPos(), m_pGame->GetGuiHelper()->GetCSYesTextYPos(), m_pGame->GetGuiHelper()->GetCSYesTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
	}

	if(m_nButtonHighlighted == eCS_NO && m_bAlphaIn == false)
	{
		m_pGame->GetRenderer()->RenderText("NO", m_pGame->GetGuiHelper()->GetCSNoTextXPos(), m_pGame->GetGuiHelper()->GetCSNoTextYPos(), m_pGame->GetGuiHelper()->GetCSNoTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255, 100, 100));
	}
	else
	{
		m_pGame->GetRenderer()->RenderText("NO", m_pGame->GetGuiHelper()->GetCSNoTextXPos(), m_pGame->GetGuiHelper()->GetCSNoTextYPos(), m_pGame->GetGuiHelper()->GetCSNoTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
	}

	if (m_bAlphaIn)
	{
		D3DXMATRIX tmpMat;	D3DXMatrixTranslation(&tmpMat, 810 * xRatio, 690 * yRatio, 0);
		RECT tmpR = {0,0,64,32};
		m_pGame->GetRenderer()->RenderArrowAndrewR(m_pGame->GetArrow306Texture(), &tmpR, tmpMat,  D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255), true, false);
		m_pGame->GetRenderer()->RenderText("Change",    871, 676, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
		m_pGame->GetRenderer()->RenderText("Change",    870, 675, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
		m_pGame->GetRenderer()->RenderText("Selection", 871, 690, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
		m_pGame->GetRenderer()->RenderText("Selection", 870, 689, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));


		D3DXMATRIX tmpMat2;	D3DXMatrixTranslation(&tmpMat2, 810 * xRatio, 740 * yRatio, 0);
		RECT tmpR2 = {0,0,64,32};
		m_pGame->GetRenderer()->RenderArrowAndrewR(m_pGame->GetEnterKeyTexture(), &tmpR2, tmpMat2,  D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255), true, false);
		m_pGame->GetRenderer()->RenderText("Confirm",   871, 726, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
		m_pGame->GetRenderer()->RenderText("Confirm",   870, 725, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
		m_pGame->GetRenderer()->RenderText("Selection", 871, 740, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
		m_pGame->GetRenderer()->RenderText("Selection", 870, 739, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));

		RECT m_rArrowRect = {0, 0, 64, 64};

		m_pGame->GetRenderer()->RenderArrowAndrewR(m_pGame->GetMenuCursorTexture(), &m_rArrowRect,  m_ReticleMat,D3DCOLOR_ARGB(255, 255,0,0) , true, false);
	}

}