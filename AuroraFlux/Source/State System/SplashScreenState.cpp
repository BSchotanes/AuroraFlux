/***********************************************
* Filename:  		CSplashScreenState.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AG
* Author:    		Andrew Goossens
* Purpose:   		This will display the splash screen 
*						while the game is loading assets
*						when the game is first loaded
*					
************************************************/
#include "../StdAfx.h"
#include "MainMenuState.h"
#include "GamePlayState.h"
#include "SplashScreenState.h"
#include "../Input/Input.h"

/*****************************************************************
* CSplashScreenState():  Default Constructor. Will set its type and behaviors.
* Ins:			    None    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
CSplashScreenState::CSplashScreenState(void) : m_fTime(0.0f), m_nCurrentSplashScreen(0),m_fAlpha(0.0f)
{

}

/*****************************************************************
* ~CSplashScreenState():  Virtual destructor used when shutting down the splash screen state
* Ins:			    none    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
CSplashScreenState::~CSplashScreenState(void)
{
}

/*****************************************************************
* Enter(): Enters the splash screen state. Will set any variables needed for the splash screen state
* Ins:			    none    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CSplashScreenState::Enter()
{
	m_d3dpSplashScreens.resize(5);
	D3DXCreateTextureFromFile(m_pGame->GetRenderer()->GetDevice(),"Resources/Textures/GPG_Splash.dds",&m_d3dpSplashScreens[0]);
	D3DXCreateTextureFromFile(m_pGame->GetRenderer()->GetDevice(),"Resources/Textures/ThreePieceDinner.png",&m_d3dpSplashScreens[1]);
	D3DXCreateTextureFromFile(m_pGame->GetRenderer()->GetDevice(),"Resources/Textures/RT_LOGO.png",&m_d3dpSplashScreens[2]);
	D3DXCreateTextureFromFile(m_pGame->GetRenderer()->GetDevice(),"Resources/Textures/RT_TitleScreen_PH.png",&m_d3dpSplashScreens[3]);
	D3DXCreateTextureFromFile(m_pGame->GetRenderer()->GetDevice(),"Resources/Textures/RT_TitleScreen_PH.png",&m_d3dpSplashScreens[4]);

	D3DXCreateTextureFromFile(m_pGame->GetRenderer()->GetDevice(),"Resources/Textures/RT_TitleScreen_PSTC_PH.png", &m_nPSTCTexture);



	m_bChangeState = false;
	m_nCurrentSplashScreen = 0;
	m_fTime = 0;
	m_bShowMouse = false;
	WwiseNS::SoundEvent(eBGM_LevelOneMusic, ePlay);
	m_fTextAlpha = 0;

	m_bRenderPSTC = false;
	m_bRenderLoading = false;

}

/*****************************************************************
* Exit(): Exits the splash screen state. Will set any variables used to null values
* Ins:			    none    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CSplashScreenState::Exit()
{
	if(m_bShowMouse == false)
	{
		ShowCursor(true);
	}

	m_bShowMouse = true;

	for(unsigned int nEachScreen = 0; nEachScreen < m_d3dpSplashScreens.size(); nEachScreen++)
	{
		SAFE_RELEASE(m_d3dpSplashScreens[nEachScreen]);	
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
bool CSplashScreenState::Input()
{
	if((CInput::BufferedButtonPress(eLeftMouseClick, true) || CInput::DIKBufferedButtonCheck(DIK_SPACE, true) || 
		CInput::BufferedButtonPress(eRightMouseClick, true)  || CInput::BufferedButtonPress(eStart, true)) 
		&& m_nCurrentSplashScreen == m_d3dpSplashScreens.size()-1)
	{
		m_bShowMouse = false;
		m_bChangeState = true;
	}

	return true;
}

/*****************************************************************
* Update(): Updates the splash screen state based on the change in time
* Ins:			    fElapsedTime    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CSplashScreenState::Update(float _fElapsedTime)
{
	m_fTime += _fElapsedTime;

	if(m_nCurrentSplashScreen < m_d3dpSplashScreens.size()-1)
	{
		if(m_fTime < 0.50f)
		{
			m_fAlpha = m_fTime + 0.05f;
		}
		else if(m_fTime > 0.50f && m_fTime < 2.0f)
		{
			m_fAlpha = 1.0f;
		}
		else if(m_fTime > 2.0f && m_fTime < 2.5f && m_nCurrentSplashScreen < m_d3dpSplashScreens.size()-2)
		{
			m_fAlpha = 2.5f - m_fTime;
		}
		else if(m_fTime > 2.5f && m_fTime < 3.0f && m_nCurrentSplashScreen < m_d3dpSplashScreens.size()-2)
		{
			m_fAlpha = 0.0f;
		}
		else if(m_fTime > 4.0f && m_nCurrentSplashScreen < m_d3dpSplashScreens.size()-2)
		{
			m_fTime = 0.0f;

			m_nCurrentSplashScreen++;
		}
		if(m_nCurrentSplashScreen == m_d3dpSplashScreens.size()-2 && m_pGame->GetLoadingDone())
		{
			//m_fAlpha = 8.0f - m_fTime;
			m_fAlpha -= _fElapsedTime;
			m_fTextAlpha = int(m_fAlpha * 255);
			if(m_fAlpha <= 0.0f)
			{
				m_pGame->SetStartMainMenu(true);

// 				m_nCurrentSplashScreen = m_d3dpSplashScreens.size()-1;
// 
// 				m_fAlpha = 0.0f;
// 				m_fTime = 0.0f;
			}
		}
	}
	//else if(m_nCurrentSplashScreen == m_d3dpSplashScreens.size()-1)
	//{
	//	if(m_fTime < 1.0f)
	//	{
	//		m_fAlpha = m_fTime + 0.1f;
	//	}
	//	else if(m_fTime > 1.0f)
	//	{
	//		m_fAlpha = 1.0f;
	//	}
	//}

	if(m_pGame->GetLoadingDone() && m_nCurrentSplashScreen == m_d3dpSplashScreens.size()-1 && m_bChangeState == false)
	{
		if(m_fTime < 1.0f)
		{
			m_fAlpha += _fElapsedTime;
		}
		else
		{
			m_fAlpha = 1.0f;
		}
	}

	if(m_bChangeState == true)
	{
		if(m_fAlpha > 0.0f)
		{
			m_fAlpha -= _fElapsedTime;
			m_fTextAlpha = int(m_fAlpha * 255);
		}
		else if(m_fAlpha <= 0.0f)
		{
			m_fAlpha = 0.0f;
		}
	}

	if(m_nCurrentSplashScreen == m_d3dpSplashScreens.size()-2)
	{
		//static float s_LoadingTimer = 0.0f;
		//
		//if(s_LoadingTimer > 0.5f)
		//{
		//	m_bRenderLoading = !m_bRenderLoading;
		//
		//	s_LoadingTimer = 0.0f;
		//}
		//
		//s_LoadingTimer += _fElapsedTime;

		if(m_bRenderLoading == true)
		{
			m_fTextAlpha += 5;

			if(m_fTextAlpha >= 255)
			{
				m_fTextAlpha = 255;
				m_bRenderLoading = false;
			}
		}
		else
		{
			m_fTextAlpha -= 5;

			if(m_fTextAlpha <= 0)
			{
				m_fTextAlpha = 0;
				m_bRenderLoading = true;
			}
		}
	}

	if(m_nCurrentSplashScreen == m_d3dpSplashScreens.size()-1)
	{
		//static float s_PSTCTimer = 0.0f;
		//
		//if(s_PSTCTimer > 0.5f)
		//{
		//	m_bRenderPSTC = !m_bRenderPSTC;
		//
		//	s_PSTCTimer = 0.0f;
		//}
		//
		//s_PSTCTimer += _fElapsedTime;

		if(m_bRenderPSTC == true)
		{
			m_fTextAlpha += 5;

			if(m_fTextAlpha >= 255)
			{
				m_fTextAlpha = 255;
				m_bRenderPSTC = false;
			}
		}
		else
		{
			m_fTextAlpha -= 5;

			if(m_fTextAlpha <= 0)
			{
				m_fTextAlpha = 0;
				m_bRenderPSTC = true;
			}
		}
	}
}

/*****************************************************************
* Render(): Renders the splash screen state by calling the renderer classes render function
* Ins:			    none    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CSplashScreenState::Render()
{
	EnterCriticalSection(&m_pGame->GetCriticalSection());
	{	
		m_pGame->GetRenderer()->Render2d(m_d3dpSplashScreens[m_nCurrentSplashScreen],m_fAlpha);

		if(m_nCurrentSplashScreen == m_d3dpSplashScreens.size()-2)
		{
			m_pGame->GetRenderer()->RenderText("LOADING", 450, 650, 1.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_fTextAlpha, 255, 255, 255));		
		}

		if(m_nCurrentSplashScreen == m_d3dpSplashScreens.size()-1)
		{
			m_pGame->GetRenderer()->Render2d(m_nPSTCTexture, m_fTextAlpha / 255.0f);
		}
	}
	LeaveCriticalSection(&m_pGame->GetCriticalSection());
}