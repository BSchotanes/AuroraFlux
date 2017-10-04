/***********************************************
* Filename:  		CMainMenuState.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AG
* Author:    		Andrew Goossens
* Purpose:   		This will display the main menu screen
*						when the player first enters the game
*					
************************************************/
#include "../StdAfx.h"
#include "MainMenuState.h"
#include "GamePlayState.h"
#include "LoadingScreenState.h"
#include "../Definitions.h"
#include "OptionsState.h"
#include "CreditsState.h"
#include "ConfirmationState.h"
/*****************************************************************
* CMainMenuState():  Default Constructor. Will set its type and behaviors.
* Ins:			    None    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
CMainMenuState::CMainMenuState(void) : m_pObjectManager(nullptr)
{
}

/*****************************************************************
* CMainMenuState():  Overloaded Constructor. Will be used to create new states.
* Ins:			    _pGame    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
CMainMenuState::CMainMenuState(CGame * _pGame) : m_bAlphaIn(true), m_nAlphaValue(0), m_pObjectManager(nullptr)
{
	m_pGame = _pGame;

	m_nButtonHighlighted = eMM_NONE;
}

/*****************************************************************
* ~CMainMenuState():  Virtual destructor used when shutting down the main menu state
* Ins:			    none    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
CMainMenuState::~CMainMenuState(void)
{
}

/*****************************************************************
* Enter(): Enters the main menu state. Will set any variables needed for the main menu state
* Ins:			    none    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
******************************************************************/
void CMainMenuState::Enter()
{	
	CInput::SetCursorLock(true);
	m_nButtonHighlighted = eMM_NONE;
	WwiseNS::SoundEvent(eBGM_MainMenuMusic, ePlay);

	if(m_pGame->GetResolutionChanged() == false)
	{
		m_bAlphaIn = true;
	}

	m_bChangeState = false;
	m_nAlphaValue = 0;

	m_bMouseHasPriority = false;

	D3DXMatrixIdentity(&m_ReticleMat);
	m_pGame->SeenBlue(false);
	m_pGame->SeenYellow(false);
	m_pGame->SeenRed(false);
	m_nPrevMouseXPos = m_pGame->GetCursorPosition().x;
	m_nPrevMouseYPos = m_pGame->GetCursorPosition().y;
	m_nSwitchState = eMM_NONE;
	s_AIHelper = new CAIHelper();
	m_pRenderer = m_pGame->GetRenderer();
	m_pCamera = m_pGame->GetCamera();
	CAssetManager* pAssetManager = m_pGame->GetRenderer()->GetAssetManager();

	m_pObjectManager = new CObjectManager(m_pRenderer, m_pCamera, s_AIHelper);
	EffectInformation::LoadAllEffects("AllEffects.txt", pAssetManager, m_pObjectManager);

	m_pPlayer = new CPlayer(100,100,100,100);
	m_pPlayer->SetPosition(0,50, -1000);
	m_pPlayer->SetVelocityModifier(0);
	m_pPlayer->SetIsVisable(false);
	m_pPlayer->SetCamera(m_pCamera);
	m_pPlayer->SetMainMenuType(0);
	m_pObjectManager->AddObject(m_pPlayer);
	m_pRenderer->SetPlayer(m_pPlayer);

	m_pScene = new CSceneryManager(m_pObjectManager, true);
	m_pScene->CreateScene();

	static bool doOnce = false;

	if(doOnce == false)
	{
		m_bPSTCOn = true;
		doOnce = true;
	}
	else
	{
		m_bPSTCOn = false;
	}

	m_nBlinkAlpha = 0;
	m_bBlinkUp = false;

	m_pCamera->SetInMainMenu(true);

	D3DXMATRIX m;
	D3DXMatrixRotationY(&m, 45);
	*m_pCamera->GetCurrentMatrix() =  m * *m_pCamera->GetCurrentMatrix();

}

/*****************************************************************
* Exit(): Exits the main menu state. Will set any variables used to null values
* Ins:			    none    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CMainMenuState::Exit()
{
	m_pCamera->SetInMainMenu(false);
	m_pCamera->Init(m_pGame->GetWidth(), m_pGame->GetHeight(), m_pGame->GetHWND());
	if(m_pObjectManager != nullptr)
	{
		m_pObjectManager->Shutdown();
		delete m_pObjectManager;
		m_pObjectManager = nullptr;
	}

	if (m_pScene)
	{
		delete m_pScene;
		m_pScene = nullptr;
	}

	if (s_AIHelper)
	{
		delete s_AIHelper;
		s_AIHelper = nullptr;
	}

}

/*****************************************************************
* Input(): Grabs any input made by the player
* Ins:			    none    		      
* Outs:				None
* Returns:		    bool
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
bool CMainMenuState::Input()
{
	LONG lCursorXPos = m_pGame->GetCursorPosition().x;
	LONG lCursorYPos = m_pGame->GetCursorPosition().y;

	if(CInput::BufferedButtonPress(eEsc, true))
	{
		m_nButtonHighlighted = eMM_EXIT;
	}

	if(m_bPSTCOn == true)
	{
		if((CInput::BufferedButtonPress(eLeftMouseClick, true) || CInput::BufferedButtonPress(eStart, true)) && m_bAlphaIn == true)
		{
			m_bAlphaIn = false;
		}
	}
	else
	{
		if(m_bChangeState == true)
		{
			switch(m_nSwitchState)
			{
			case eMM_PLAY:
				{
					m_pGame->switchState(new CLoadingScreenState(m_pGame));
					return true;
				}
				break;

			case eMM_OPTIONS:
				{
					m_pGame->PushState(new COptionsState(m_pGame, eMAIN_MENU));
					return true;
				}
				break;

			case eMM_CREDITS:
				{
					m_pGame->PushState(new CCreditsState(m_pGame));
					return true;
				}
				break;

			case eMM_EXIT:
				{
					m_pGame->PushState(new CConfirmationState(m_pGame, eEXIT_GAME));
					return true;
				}
				break;
			}
		}

		if(CInput::BufferedButtonPress(eStart, true) && m_bAlphaIn == true)
		{
			if(m_nButtonHighlighted == eMM_PLAY)
			{
				m_bAlphaIn = false;
				WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
				m_nSwitchState = eMM_PLAY;
			}
			else if(m_nButtonHighlighted == eMM_OPTIONS)
			{
				m_bAlphaIn = false;
				WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
				m_nSwitchState = eMM_OPTIONS;
			}
			else if(m_nButtonHighlighted == eMM_CREDITS)
			{
				WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
				m_bAlphaIn = false;
				m_nSwitchState = eMM_CREDITS;
			}
			else if(m_nButtonHighlighted == eMM_EXIT)
			{
				WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
				m_bAlphaIn = false;
				m_nSwitchState = eMM_EXIT;
			}
		}

		if(m_nButtonHighlighted == eMM_NONE)
		{
			if(CInput::BufferedButtonPress(eStart, true) || CInput::BufferedButtonPress(eUp, true) || CInput::BufferedButtonPress(eUpArrow, true) || 
				CInput::BufferedButtonPress(eLeft, true) || CInput::BufferedButtonPress(eLeftArrow, true) || CInput::BufferedButtonPress(eDown, true) || 
				CInput::BufferedButtonPress(eDownArrow, true) || CInput::BufferedButtonPress(eRight, true) || CInput::BufferedButtonPress(eRightArrow, true)
				|| CInput::DIKBufferedButtonCheck(DIK_SPACE, true))
			{
				//SetCursorPos(0, 0);
				m_nButtonHighlighted = eMM_PLAY;
			}
		}

		if((CInput::BufferedButtonPress(eUp) || CInput::BufferedButtonPress(eUpArrow) || CInput::BufferedButtonPress(eLeft) || CInput::BufferedButtonPress(eLeftArrow)) && m_bAlphaIn == true)
		{
			m_bMouseHasPriority = false;

			WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);

			//SetCursorPos(0, 0);
			if(m_nButtonHighlighted == eMM_PLAY)
			{
				m_nButtonHighlighted = eMM_EXIT;
			}
			else if(m_nButtonHighlighted == eMM_EXIT)
			{
				m_nButtonHighlighted = eMM_CREDITS;
			}
			else if(m_nButtonHighlighted == eMM_CREDITS)
			{
				m_nButtonHighlighted = eMM_OPTIONS;
			}
			else if(m_nButtonHighlighted == eMM_OPTIONS)
			{
				m_nButtonHighlighted = eMM_PLAY;
			}
		}
		else if((CInput::BufferedButtonPress(eDown) || CInput::BufferedButtonPress(eDownArrow) || CInput::BufferedButtonPress(eRight) || CInput::BufferedButtonPress(eRightArrow)) && m_bAlphaIn == true)
		{
			m_bMouseHasPriority = false;

			WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);

			//SetCursorPos(0, 0);
			if(m_nButtonHighlighted == eMM_PLAY)
			{
				m_nButtonHighlighted = eMM_OPTIONS;
			}
			else if(m_nButtonHighlighted == eMM_OPTIONS)
			{
				m_nButtonHighlighted = eMM_CREDITS;
			}
			else if(m_nButtonHighlighted == eMM_CREDITS)
			{
				m_nButtonHighlighted = eMM_EXIT;
			}
			else if(m_nButtonHighlighted == eMM_EXIT)
			{
				m_nButtonHighlighted = eMM_PLAY;
			}
		}

		// EXIT: Exit the game if exit button is pressed
		if(lCursorXPos > s_rExitButtonRect.left && lCursorXPos < s_rExitButtonRect.right &&	lCursorYPos > s_rExitButtonRect.top && lCursorYPos < s_rExitButtonRect.bottom)
		{
			if(m_nButtonHighlighted != eMM_EXIT && m_bAlphaIn == true && m_bMouseHasPriority == true)
			{
				WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);
			}

			if(m_bAlphaIn == true && m_bMouseHasPriority == true)
			{
				m_nButtonHighlighted = eMM_EXIT;
			}

			if(CInput::BufferedButtonPress(eLeftMouseClick, true) && m_bAlphaIn == true)
			{
				WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
				m_bAlphaIn = false;
				m_nSwitchState = eMM_EXIT;
			}


		}
		// CREDITS: Go to the credits state if credits button is pressed
		if(lCursorXPos > s_rCreditsButtonRect.left && lCursorXPos < s_rCreditsButtonRect.right && lCursorYPos > s_rCreditsButtonRect.top && lCursorYPos < s_rCreditsButtonRect.bottom)
		{
			if(m_nButtonHighlighted != eMM_CREDITS && m_bAlphaIn == true && m_bMouseHasPriority == true)
			{
				WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);
			}

			if(m_bAlphaIn == true && m_bMouseHasPriority == true)
			{
				m_nButtonHighlighted = eMM_CREDITS;
			}

			if(CInput::BufferedButtonPress(eLeftMouseClick) && m_bAlphaIn == true)
			{
				WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
				m_bAlphaIn = false;
				m_nSwitchState = eMM_CREDITS;
			}

		}

		// OPTIONS: Go to the options state if button is pressed
		if(lCursorXPos > s_rOptionsButtonRect.left && lCursorXPos < s_rOptionsButtonRect.right && lCursorYPos > s_rOptionsButtonRect.top && lCursorYPos < s_rOptionsButtonRect.bottom)
		{
			if(m_nButtonHighlighted != eMM_OPTIONS && m_bAlphaIn == true && m_bMouseHasPriority == true)
			{
				WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);
			}

			if(m_bAlphaIn == true && m_bMouseHasPriority == true)
			{
				m_nButtonHighlighted = eMM_OPTIONS;
			}

			if(CInput::BufferedButtonPress(eLeftMouseClick) && m_bAlphaIn == true)
			{
				WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
				m_bAlphaIn = false;
				m_nSwitchState = eMM_OPTIONS;
			}
		}

		// PLAY: Enter the game state if play is pressed
		if(lCursorXPos > s_rPlayButtonRect.left && lCursorXPos < s_rPlayButtonRect.right &&	lCursorYPos > s_rPlayButtonRect.top && lCursorYPos < s_rPlayButtonRect.bottom)
		{
			if(m_nButtonHighlighted != eMM_PLAY && m_bAlphaIn == true && m_bMouseHasPriority == true)
			{
				WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);
			}


			if(m_bAlphaIn == true && m_bMouseHasPriority == true)
			{
				m_nButtonHighlighted = eMM_PLAY;
			}

			if(CInput::BufferedButtonPress(eLeftMouseClick) && m_bAlphaIn == true)
			{
				WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
				m_bAlphaIn = false;
				m_nSwitchState = eMM_PLAY;
			}
		}
	}

	return true;
}

/*****************************************************************
* Update(): Updates the main menu state based on the change in time
* Ins:			    fElapsedTime    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CMainMenuState::Update(float _fElapsedTime)
{ 
	m_pObjectManager->UpdateMainMenuObjects(_fElapsedTime);

	D3DXMATRIX m;
	D3DXMatrixRotationY(&m, _fElapsedTime * 0.06f);
	*m_pCamera->GetCurrentMatrix() =  m * *m_pCamera->GetCurrentMatrix();
	m_pScene->Update(_fElapsedTime);
	if(m_bAlphaIn == true)
	{
		if(m_nAlphaValue < 255) 
		{
			m_nAlphaValue += 15;
			m_nBlinkAlpha = m_nAlphaValue;
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
			m_nBlinkAlpha = m_nAlphaValue;
		}
		else if(m_nAlphaValue <= 0)
		{
			m_nAlphaValue = 0;

			if(m_bPSTCOn == true)
			{
				m_bPSTCOn = false;
				m_bAlphaIn = true;
			}
			else
			{
				m_bChangeState = true;
			}
		}
	}

	if(m_bBlinkUp == true)
	{
		m_nBlinkAlpha += 5;

		if(m_nBlinkAlpha >= 255)
		{
			m_nBlinkAlpha = 255;
			m_bBlinkUp = false;
		}
	}
	else if(m_bBlinkUp == false)
	{
		m_nBlinkAlpha -= 5;

		if(m_nBlinkAlpha <= 0)
		{
			m_nBlinkAlpha = 0;
			m_bBlinkUp = true;
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
* Render(): Renders the game main menu by calling the renderer classes render function
* Ins:			    none    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CMainMenuState::Render()
{

	float xRatio = m_pGame->GetRenderer()->GetXRatio();
	float yRatio = m_pGame->GetRenderer()->GetYRatio();
	m_pObjectManager->RenderMainMenuObjects();
	//m_pGame->GetRenderer()->Render2d(m_pGame->GetMenuTexture(0), m_nAlphaValue / 255.0f);

	m_pGame->GetRenderer()->Render2d(m_pGame->GetAuroraFluxLogo(), m_nAlphaValue / 255.0f);

	if(m_bPSTCOn == true)
	{
		m_pGame->GetRenderer()->Render2d(m_pGame->GetPSTCTexture(), m_nBlinkAlpha / 255.0f);
	}
	else if(m_bPSTCOn == false)
	{
		m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetMMBorderXPos(), (float)m_pGame->GetGuiHelper()->GetMMBorderYPos(), false, m_pGame->GetGuiHelper()->GetMMBorderXScale(), m_pGame->GetGuiHelper()->GetMMBorderYScale(), false, 0, 512, 1024, 1075, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));

		if(m_nButtonHighlighted == eMM_PLAY && m_bAlphaIn == false)
		{
			m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetMMPlayXPos(),(float)m_pGame->GetGuiHelper()->GetMMPlayYPos(),eBUTTON_MM_PLAY, m_pGame->GetGuiHelper()->GetMMPlayXScale(), m_pGame->GetGuiHelper()->GetMMPlayYScale(), true, 0, 0, 512, 128, D3DCOLOR_ARGB(m_nAlphaValue, 127, 127, 127));
		}
		else if(m_nButtonHighlighted == eMM_PLAY)
		{
			m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetMMPlayXPos(),(float)m_pGame->GetGuiHelper()->GetMMPlayYPos(),eBUTTON_MM_PLAY, m_pGame->GetGuiHelper()->GetMMPlayXScale(), m_pGame->GetGuiHelper()->GetMMPlayYScale(), true, 0, 0, 512, 128, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
		}
		else
		{
			m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetMMPlayXPos(),(float)m_pGame->GetGuiHelper()->GetMMPlayYPos(),eBUTTON_MM_PLAY, m_pGame->GetGuiHelper()->GetMMPlayXScale(), m_pGame->GetGuiHelper()->GetMMPlayYScale(), false, 0, 0, 512, 128, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
		}

		if(m_nButtonHighlighted == eMM_OPTIONS && m_bAlphaIn == false)
		{
			m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetMMOptionsXPos(),(float)m_pGame->GetGuiHelper()->GetMMOptionsYPos(),eBUTTON_MM_OPTIONS, m_pGame->GetGuiHelper()->GetMMOptionsXScale(), m_pGame->GetGuiHelper()->GetMMOptionsYScale(), true, 0, 128, 512, 256, D3DCOLOR_ARGB(m_nAlphaValue, 127, 127, 127));
		}
		else if(m_nButtonHighlighted == eMM_OPTIONS)
		{
			m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetMMOptionsXPos(),(float)m_pGame->GetGuiHelper()->GetMMOptionsYPos(),eBUTTON_MM_OPTIONS, m_pGame->GetGuiHelper()->GetMMOptionsXScale(), m_pGame->GetGuiHelper()->GetMMOptionsYScale(), true, 0, 128, 512, 256, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
		}
		else
		{
			m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetMMOptionsXPos(),(float)m_pGame->GetGuiHelper()->GetMMOptionsYPos(),eBUTTON_MM_OPTIONS, m_pGame->GetGuiHelper()->GetMMOptionsXScale(), m_pGame->GetGuiHelper()->GetMMOptionsYScale(), false, 0, 128, 512, 256, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
		}

		if(m_nButtonHighlighted == eMM_CREDITS && m_bAlphaIn == false)
		{
			m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetMMCreditsXPos(),(float)m_pGame->GetGuiHelper()->GetMMCreditsYPos(),eBUTTON_MM_CREDITS, m_pGame->GetGuiHelper()->GetMMCreditsXScale(), m_pGame->GetGuiHelper()->GetMMCreditsYScale(), true, 0, 256, 512, 384, D3DCOLOR_ARGB(m_nAlphaValue, 127, 127, 127));
		}
		else if(m_nButtonHighlighted == eMM_CREDITS)
		{
			m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetMMCreditsXPos(),(float)m_pGame->GetGuiHelper()->GetMMCreditsYPos(),eBUTTON_MM_CREDITS, m_pGame->GetGuiHelper()->GetMMCreditsXScale(), m_pGame->GetGuiHelper()->GetMMCreditsYScale(), true, 0, 256, 512, 384, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
		}
		else
		{
			m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetMMCreditsXPos(),(float)m_pGame->GetGuiHelper()->GetMMCreditsYPos(),eBUTTON_MM_CREDITS, m_pGame->GetGuiHelper()->GetMMCreditsXScale(), m_pGame->GetGuiHelper()->GetMMCreditsYScale(), false, 0, 256, 512, 384, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
		}

		if(m_nButtonHighlighted == eMM_EXIT && m_bAlphaIn == false)
		{
			m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetMMExitXPos(),(float)m_pGame->GetGuiHelper()->GetMMExitYPos(),eBUTTON_MM_EXIT, m_pGame->GetGuiHelper()->GetMMExitXScale(), m_pGame->GetGuiHelper()->GetMMExitYScale(), true, 0, 384, 512, 512, D3DCOLOR_ARGB(m_nAlphaValue, 127, 127, 127));
		}
		else if(m_nButtonHighlighted == eMM_EXIT)
		{
			m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetMMExitXPos(),(float)m_pGame->GetGuiHelper()->GetMMExitYPos(),eBUTTON_MM_EXIT, m_pGame->GetGuiHelper()->GetMMExitXScale(), m_pGame->GetGuiHelper()->GetMMExitYScale(), true, 0, 384, 512, 512, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
		}
		else
		{
			m_pGame->GetRenderer()->RenderButton((float)m_pGame->GetGuiHelper()->GetMMExitXPos(),(float)m_pGame->GetGuiHelper()->GetMMExitYPos(),eBUTTON_MM_EXIT, m_pGame->GetGuiHelper()->GetMMExitXScale(), m_pGame->GetGuiHelper()->GetMMExitYScale(), false, 0, 384, 512, 512, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
		}

		if(m_nButtonHighlighted == eMM_PLAY && m_bAlphaIn == false)
		{
			m_pGame->GetRenderer()->RenderText("PLAY", m_pGame->GetGuiHelper()->GetMMPlayTextXPos(), m_pGame->GetGuiHelper()->GetMMPlayTextYPos(), m_pGame->GetGuiHelper()->GetMMPlayTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255, 100, 100));
		}
		else
		{
			m_pGame->GetRenderer()->RenderText("PLAY", m_pGame->GetGuiHelper()->GetMMPlayTextXPos(), m_pGame->GetGuiHelper()->GetMMPlayTextYPos(), m_pGame->GetGuiHelper()->GetMMPlayTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
		}

		if(m_nButtonHighlighted == eMM_OPTIONS && m_bAlphaIn == false)
		{
			m_pGame->GetRenderer()->RenderText("OPTIONS", m_pGame->GetGuiHelper()->GetMMOptionsTextXPos(), m_pGame->GetGuiHelper()->GetMMOptionsTextYPos(), m_pGame->GetGuiHelper()->GetMMOptionsTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255, 100, 100));
		}
		else
		{
			m_pGame->GetRenderer()->RenderText("OPTIONS", m_pGame->GetGuiHelper()->GetMMOptionsTextXPos(), m_pGame->GetGuiHelper()->GetMMOptionsTextYPos(), m_pGame->GetGuiHelper()->GetMMOptionsTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
		}

		if(m_nButtonHighlighted == eMM_CREDITS && m_bAlphaIn == false)
		{
			m_pGame->GetRenderer()->RenderText("CREDITS", m_pGame->GetGuiHelper()->GetMMCreditsTextXPos(), m_pGame->GetGuiHelper()->GetMMCreditsTextYPos(), m_pGame->GetGuiHelper()->GetMMCreditsTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255, 100, 100));
		}
		else
		{
			m_pGame->GetRenderer()->RenderText("CREDITS", m_pGame->GetGuiHelper()->GetMMCreditsTextXPos(), m_pGame->GetGuiHelper()->GetMMCreditsTextYPos(), m_pGame->GetGuiHelper()->GetMMCreditsTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
		}

		if(m_nButtonHighlighted == eMM_EXIT && m_bAlphaIn == false)
		{
			m_pGame->GetRenderer()->RenderText("EXIT", m_pGame->GetGuiHelper()->GetMMExitTextXPos(), m_pGame->GetGuiHelper()->GetMMExitTextYPos(), m_pGame->GetGuiHelper()->GetMMExitTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255, 100, 100));
		}
		else
		{
			m_pGame->GetRenderer()->RenderText("EXIT", m_pGame->GetGuiHelper()->GetMMExitTextXPos(), m_pGame->GetGuiHelper()->GetMMExitTextYPos(), m_pGame->GetGuiHelper()->GetMMExitTextScale(), 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
		}

		D3DXMATRIX d3dTmpMat;	D3DXMatrixTranslation(&d3dTmpMat, 810* xRatio, 700 * yRatio, 0);
		RECT rArrowRect = {0,0,64,32};

		m_pGame->GetRenderer()->RenderArrowAndrewR(m_pGame->GetArrowKeyTexture(), &rArrowRect, d3dTmpMat, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255), true, false);

		m_pGame->GetRenderer()->RenderText("Change",    861, 691, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
		m_pGame->GetRenderer()->RenderText("Change",    860, 690, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
		m_pGame->GetRenderer()->RenderText("Selection", 861, 705, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
		m_pGame->GetRenderer()->RenderText("Selection", 860, 704, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));

		D3DXMATRIX d3dTmpMat2;	D3DXMatrixTranslation(&d3dTmpMat2, 810* xRatio, 740 * yRatio, 0);
		RECT rEnterRect = {0,0,64,32};
		m_pGame->GetRenderer()->RenderArrowAndrewR(m_pGame->GetEnterKeyTexture(), &rEnterRect, d3dTmpMat2, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255),true, false);
		m_pGame->GetRenderer()->RenderText("Confirm",   861, 731, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
		m_pGame->GetRenderer()->RenderText("Confirm",   860, 730, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
		m_pGame->GetRenderer()->RenderText("Selection", 861, 745, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
		m_pGame->GetRenderer()->RenderText("Selection", 860, 746, 0.8f, 0, 0, 0,  D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));

		RECT m_rArrowRect = {0, 0, 64, 64};
		if (m_bAlphaIn)
		{
			m_pGame->GetRenderer()->RenderArrowAndrewR(m_pGame->GetMenuCursorTexture(), &m_rArrowRect,  m_ReticleMat,D3DCOLOR_ARGB(255, 255,0,0) , true, false);
		}
	}
}