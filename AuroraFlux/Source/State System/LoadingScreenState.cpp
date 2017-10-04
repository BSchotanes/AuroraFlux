/***********************************************
* Filename:  		LoadingScreenState.cpp
* Date:      		1/14/2013
* Mod. Date: 		1/14/2013
* Mod. Initials:	AQ
* Author:    		Alexander Garcia de Quevedo
* Purpose:   		This will display the loading screen
*					when the player attempts to play the game
*					
************************************************/
#include "../Definitions.h"
#include "LoadingScreenState.h"
#include "MainMenuState.h"
#include "GamePlayState.h"
#include "../Input/Input.h"
#include "../Renderer/CRenderer.h"

/*****************************************************************
* CCreditsState():  Default Constructor. Will set its type and behaviors.
* Ins:			    None    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
CLoadingScreenState::CLoadingScreenState(void) : m_bDoneLoading(false), m_nTime(0)
{
}

/*****************************************************************
* ~CCreditsState():  Virtual destructor used when shutting down the confirmation state
* Ins:			    none    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
CLoadingScreenState::~CLoadingScreenState(void)
{
}

/*****************************************************************
* Enter(): Enters the Loading Screen state. Will set any variables needed for the Loading Screen state
* Ins:			    none    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CLoadingScreenState::Enter()
{
	m_bDoneLoading = false;
	m_nTime = 0;
	m_nAlphaValue = 0;
	m_bChangeState = false;
	m_bAlphaIn = true;
	m_nWhichScreen = 0;
	loadingBlink = 0;
	loadingBlinkBool = false;
	float xRatio = m_pGame->GetRenderer()->GetXRatio();
	float yRatio = m_pGame->GetRenderer()->GetYRatio();

	for(int i = 0; i < 7; ++i)
	{
		m_bShouldRender[i] = false;
	}

	m_nWhichScreen = rand() % 7;
	
	m_bShouldRender[m_nWhichScreen] = true;

	D3DXMatrixIdentity(&m_ReticleMat);
	//m_bShouldRender[0] = true;
	//m_pGame->GetRenderer()->Render2d(m_pGame->GetLoadingScreenTexture(0), 100);

	m_pGamePlayState = new GamePlayState(m_pGame, false);

	m_pRenderer = m_pGame->GetRenderer();
	m_fLoadingPercentage = 0.0f;
	m_rLoadingBarRect.left = 0;
	m_rLoadingBarRect.top = 0;
	m_rLoadingBarRect.right = 0;
	m_rLoadingBarRect.bottom = 8; 
	D3DXMatrixIdentity(&m_d3dLoadingBarMatrix);


	D3DXMATRIX _d3dScaler; 
	D3DXMatrixIdentity(&_d3dScaler);
	D3DXMatrixScaling(&_d3dScaler, 8.0f *xRatio, 4.0f*yRatio, 1.0f);

	m_d3dLoadingBarMatrix = m_d3dLoadingBarMatrix *_d3dScaler ;
	float _fHeight = 691;
	m_d3dLoadingBarMatrix._41 = 204 * xRatio;
	//#if !_DEBUG
	//_fHeight = 521; 
	//#endif
	m_d3dLoadingBarMatrix._42 = _fHeight * yRatio;

	m_d3dLoadingBarColor = D3DCOLOR_ARGB( 150, 0, int( 255 * m_fLoadingPercentage ), 0 );
	m_nLoadingStage = 0;
	m_fInitialLoadingBucket = 0.0f;
}

/*****************************************************************
* Exit(): Exits the Loading Screen state. Will set any variables used to null values
* Ins:			    none    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CLoadingScreenState::Exit()
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
bool CLoadingScreenState::Input()
{
	if(CInput::BufferedButtonPress(eExtraButton))
	{
		m_bAlphaIn = false;
	}
	else if(m_bChangeState == true)
	{
		m_nTime = 111111111;
	}

	if(m_bDoneLoading)
	{
		if(CInput::BufferedButtonPress(eLeftMouseClick) || CInput::BufferedButtonPress(eStart))
		{
			m_bAlphaIn = false;
		}
		else if(m_bChangeState == true)
		{
			POINT tCursorPosition;
			tCursorPosition.x = LONG(m_pGame->GetWidth() / 2);
			tCursorPosition.y = LONG(m_pGame->GetWidth() / 2);
			m_pGame->SetCursorPosition(tCursorPosition);

			m_pGame->switchState(m_pGamePlayState);
		}
	}

	return true;
}

/*****************************************************************
* Update(): Updates the Loading Screen state based on the change in time
* Ins:			    fElapsedTime    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CLoadingScreenState::Update(float _fElapsedTime)
{
	if(m_fInitialLoadingBucket > 2.0f)
	{
		if (m_fLoadingPercentage >= .90f)
		{
			if(m_nLoadingStage == 3)
			{
				m_bShouldRender[1] = true;
				++m_nLoadingStage;
				m_pGame->ResetCurrentStats();
				m_pGame->SaveCurrentStats();
				m_pGamePlayState->FourthEnter();
				m_bShouldRender[2] = true;
			}
			m_d3dLoadingBarColor = D3DCOLOR_ARGB( m_nAlphaValue, 0, 255, 0 );
		}
		else if (m_fLoadingPercentage >= .25f)
		{
			if(m_nLoadingStage == 1)
			{
				m_bShouldRender[3] = true;
				++m_nLoadingStage;
				m_pGamePlayState->SecondEnter();
				m_bShouldRender[4] = true;
			}
			else if(m_nLoadingStage == 2)
			{
				m_bShouldRender[5] = true;
				++m_nLoadingStage;
				m_pGamePlayState->ThirdEnter();
				m_bShouldRender[6] = true;
			}
			m_d3dLoadingBarColor = D3DCOLOR_ARGB( m_nAlphaValue, 255, 255, 0 );
		}
		else
		{
			if(!m_nLoadingStage)
			{
				++m_nLoadingStage;
				m_pGamePlayState->FirstEnter();
				m_bShouldRender[7] = true;
			}
			m_d3dLoadingBarColor = D3DCOLOR_ARGB( m_nAlphaValue, 255, 20, 20 );
		}

		if(rand() % 1 == 0)
		{
			if(m_fLoadingPercentage < 1.0f)
			{
				m_fLoadingPercentage += 0.01f;
				m_rLoadingBarRect.right = -LONG( m_fLoadingPercentage * 75);
			}
		}

		if(m_fLoadingPercentage >= 1.0f)
		{
			m_bDoneLoading = true;
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
	}
	else
	{
		m_fInitialLoadingBucket += _fElapsedTime;
	}
	if (loadingBlinkBool)
	{
		loadingBlink += 15;
		if (loadingBlink >= 255)
		{
			loadingBlink = 255;
			loadingBlinkBool = !loadingBlinkBool;
		}
	}
	else
	{
		loadingBlink -= 15;
		if (loadingBlink <= 0)
		{
			loadingBlink = 0;
			loadingBlinkBool = !loadingBlinkBool;
		}
	}
	

	m_ReticleMat._41 = float(m_pGame->GetCursorPosition().x);
	m_ReticleMat._42 = float(m_pGame->GetCursorPosition().y);
}

/*****************************************************************
* Render(): Renders the Loading Screen state by calling the renderer classes render function
* Ins:			    none    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CLoadingScreenState::Render()
{
	RECT rLoadingScreenRect = {0,0,1024,512};
	RECT rSmallLogoRect = {0,0,512,320};
	D3DXMATRIX d3dLoadingScreenMat;
	D3DXMatrixIdentity(&d3dLoadingScreenMat);

	//m_pGame->GetRenderer()->RenderArrowAndrewR(m_pGame->GetMenuTexture(0), &rLoadingScreenRect, d3dLoadingScreenMat, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255), true, true);

	static float xPos1 = 0;
	static float yPos1 = 0;

	D3DXMatrixScaling(&d3dLoadingScreenMat, m_pGame->GetRenderer()->GetXRatio(), m_pGame->GetRenderer()->GetYRatio(), 1.0f);

	d3dLoadingScreenMat._41 = 512;
	d3dLoadingScreenMat._42 = 308;
			
	m_pGame->GetRenderer()->RenderArrowAndrewR(m_pGame->GetLoadingScreenTexture(m_nWhichScreen), &rLoadingScreenRect, d3dLoadingScreenMat, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255), true, true);
	
	d3dLoadingScreenMat._41 = 1042;
	d3dLoadingScreenMat._42 = 722;
			
	m_pGame->GetRenderer()->RenderArrowAndrewR(m_pGame->GetSmallAuroraFluxLogo(), &rSmallLogoRect, d3dLoadingScreenMat, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255), true, true);

	if(m_bDoneLoading)
	{
		m_pGame->GetRenderer()->RenderText("Click Mouse/Press Enter to Continue...", 302, 702, 1.0f, 0, 0, 0, D3DCOLOR_ARGB(loadingBlink, 0,0,0) );
		m_pGame->GetRenderer()->RenderText("Click Mouse/Press Enter to Continue...", 300, 700, 1.0f, 0, 0, 0, D3DCOLOR_ARGB(loadingBlink, 255,255,255) );
	}
	else
	{

		m_pGame->GetRenderer()->RenderText("LOADING...", 302, 702, 1.0f, 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 0,0,0) );
		m_pGame->GetRenderer()->RenderText("LOADING...", 300, 700, 1.0f, 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255,255,255) );

		//char szbuff[100];
		//sprintf_s(szbuff, "%f", bossHealthPercentage);
		//m_pGame->GetRenderer()->RenderText(szbuff, 617, 702, 1.0f, 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 0,0,0) );
		//m_pGame->GetRenderer()->RenderText(szbuff, 615, 700, 1.0f, 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255,255,255) );
	}

	RECT m_rArrowRect = {0, 0, 64, 64};

	//m_pGame->GetRenderer()->RenderArrowAndrewR(m_pGame->GetMenuCursorTexture(), &m_rArrowRect,  m_ReticleMat,D3DCOLOR_ARGB(255, 255,0,0) );


	m_pRenderer->RenderArrowAndrewR(m_pRenderer->GetAssetManager()->LoadTexture("RT_2D_HealthIcon#1.png"), &m_rLoadingBarRect, m_d3dLoadingBarMatrix , m_d3dLoadingBarColor,false, false );
}

