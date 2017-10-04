#include "../StdAfx.h"
#include "Game.h"
#include "MainMenuState.h"
#include "../Input/Input.h"
#include "SplashScreenState.h"
#include "GamePlayState.h"
HANDLE serverThread = 0;

void StatTrack::InitStats()
{
	m_nDamageTaken = 0;
	m_nLasersFired = 0;
	m_nLasersHit = 0;
	m_nMissilesFired = 0;
	m_nMissilesHit = 0;
	m_nMissilesDodged = 0;
	m_nDeaths = 0;
}

unsigned __stdcall  LoadModels(void* _lpParameter)
{
	CGame* pGame = (CGame*)(_lpParameter);

	EnterCriticalSection(&pGame->GetCriticalSection());
	{
		CAssetManager* pAssetManager = pGame->GetRenderer()->GetAssetManager();

		///////////////////////////////////////////////////////////////////////
		//																	 //
		//					***** Loading New Textures *****				 //
		//																	 //
		///////////////////////////////////////////////////////////////////////
		pGame->SetMenuTexture(pAssetManager->LoadTexture("RT_2D_Menu_Background.png"));

		pGame->SetAuroraFluxLogo(pAssetManager->LoadTexture("Aurora_Flux_Logo.png"));
		pGame->SetSmallAuroraFluxLogo(pAssetManager->LoadTexture("Aurora_Flux_Logo_2.png"));

		pGame->SetMenuCursor(pAssetManager->LoadTexture("target.png"));
		pGame->SetPSTCTexture(pAssetManager->LoadTexture("RT_TitleScreen_PSTC_PH.png"));
		pGame->SetGreenArrowTexture(pAssetManager->LoadTexture("GreenArrow.png"));
		pGame->SetGreenBoxTexture(pAssetManager->LoadTexture("GreenArrow2.png"));
		pGame->SetRedArrowTexture(pAssetManager->LoadTexture("RedArrow.png"));
		pGame->SetRedBoxTexture(pAssetManager->LoadTexture("RedArrow2.png"));
		pGame->SetBlueArrowTexture(pAssetManager->LoadTexture("BlueArrow.png"));
		pGame->SetBlueBoxTexture(pAssetManager->LoadTexture("BlueArrow2.png"));
		pGame->SetYellowArrowTexture(pAssetManager->LoadTexture("YellowArrow.png"));
		pGame->SetYellowBoxTexture(pAssetManager->LoadTexture("YellowArrow2.png"));
		pGame->SetHUDHealthIcon(pAssetManager->LoadTexture("RT_2D_HealthIcon.png"));
		pGame->SetHUDShieldIcon(pAssetManager->LoadTexture("RT_2D_ShieldIcon.png"));
		pGame->SetBlueEnemyShieldIcon(pAssetManager->LoadTexture("RT_2D_ShieldIconTwo.png"));

		pGame->SetLoadingScreenTextures(pAssetManager->LoadTexture("Player_Complete_1_1.png"), 0);
		pGame->SetLoadingScreenTextures(pAssetManager->LoadTexture("Player_Complete_2_2.png"), 1);
		pGame->SetLoadingScreenTextures(pAssetManager->LoadTexture("Player_Complete_3_3.png"), 2);
		pGame->SetLoadingScreenTextures(pAssetManager->LoadTexture("Player_Complete_4_4.png"), 3);
		pGame->SetLoadingScreenTextures(pAssetManager->LoadTexture("Player_Complete_5_5.png"), 4);
		pGame->SetLoadingScreenTextures(pAssetManager->LoadTexture("Player_Complete_6_6.png"), 5);
		pGame->SetLoadingScreenTextures(pAssetManager->LoadTexture("Player_Complete_7_7.png"), 6);
				
		pGame->SetHelpTexture(pAssetManager->LoadTexture("Loading Screen Controls.png"));

		pGame->SetBlackTexture(pAssetManager->LoadTexture("black.png"));

		pGame->SetArrowKeyTexture(pAssetManager->LoadTexture("dec-arrow-keys-UD.png"));
		pGame->SetEnterKeyTexture(pAssetManager->LoadTexture("enter_key.png"));
		pGame->SetArrowKeyLRTexture(pAssetManager->LoadTexture("dec-arrow-keys-LR.png"));
		pGame->SetArrow306Texture(pAssetManager->LoadTexture("dec-arrow-keys-300x206.png"));

		//RED_PLAYER
		pAssetManager->LoadMainMenuModel("PlayerShipShape.mesh");
		//ASTEROID
		pAssetManager->LoadMainMenuModel("RT_3D_Alt_Asteroid_Shape1.mesh");
		pAssetManager->LoadMainMenuModel("RT_3D_Alt_Asteroid_Shape2.mesh");

		pAssetManager->LoadMainMenuModel("RT_3D_Planet_Model_SurfaceShape.mesh"); //  RT_3D_Planet_Model_SurfaceShape

		pAssetManager->LoadMainMenuModel("Planet3Shape.mesh"); //  RT_3D_Planet_Model_SurfaceShape
		pAssetManager->LoadMainMenuModel("RT_3D_Planet4_ModelShape.mesh");
		//6
		pAssetManager->LoadMainMenuModel("RT_3D_Station2_ModelShape.mesh");
		pAssetManager->LoadMainMenuModel("SpaceDebrisShape4.mesh");
		pAssetManager->LoadMainMenuModel("Space_Debris_2.mesh");
		pAssetManager->LoadMainMenuModel("SpaceDebris_0Shape1.mesh");
		pAssetManager->LoadMainMenuModel("ShieldQuad.mesh");


		pGame->SetLoadingDone(true);
	}
	LeaveCriticalSection(&pGame->GetCriticalSection());
	return 0;
}

/*
HANDLE GetProcessHandle(LPSTR szExeName)
{
#include<tlhelp32.h>
PROCESSENTRY32 Pc = { sizeof(PROCESSENTRY32) };volume

HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
if(Process32First(hSnapshot, &Pc)){

do{
if(!strcmp(Pc.szExeFile, szExeName)) {

return OpenProcess(PROCESS_ALL_ACCESS, TRUE, Pc.th32ProcessID);

}

}while(Process32Next(hSnapshot, &Pc));

}

return NULL;

}

*/


DWORD WINAPI createServer(LPVOID _data)
{
	system("AITool.exe");

	return 0;
}

CGame::CGame(void)
{
	//Window
	m_fScreenWidth = 0.0f;
	m_fScreenHeight = 0.0f;

	m_fMX_Volume = DEFAULT_VOLUME;
	m_fFX_Volume = DEFAULT_SFX_VOLUME;
	m_fDX_Volume = DEFAULT_SFX_VOLUME;

	m_bIsWindowed = false;
	m_bLockCursorToScreen = true;

	//State
	m_pCurrState = nullptr;
	m_bLoadingDone = false;
	m_bStartMainMenu = false;

	m_nMenuTextureID = -1;
	m_nPauseTextureScreens = -1;
	m_nGameOverTextures = -1;
	m_nVictoryTextures = -1;
	m_nConfirmationTextures = -1;
	//m_nLoadingTextures = -1;
	m_nOptionsTextures = -1;
	m_nButtonTexture = -1;
	m_nCreditTexture = -1;

	m_bSeenBlueEnemy = false;
	m_bSeenYellowEnemy = false;
	m_bSeenRedEnemy = false;

	m_bBeenInGeneratorPhase = false;
	m_bBeenInLaserPhase = false;
	m_bBeenInBigLaserPhase = false;

	m_nCurrentResolution = 1;
}


CGame::~CGame(void)
{
}

bool CGame::Init(HWND _hWnd, HINSTANCE _hInstance, float _nScreenWidth, float _nScreenHeight, bool _bIsWindowed)
{	

	m_hWnd = _hWnd;
	m_bIsWindowed = _bIsWindowed;
	m_fScreenWidth = _nScreenWidth;
	m_fScreenHeight = _nScreenHeight;

	m_bOptionsApply = false;
	m_bOptionsDefault = false;
	m_bOptionsCancel = false;
	m_bFromMainMenu = false;
	m_bResolutionChanged = false;

#if _DEBUG
	//ChangeResolution(WINDOW_WIDTH_720p,WINDOW_HEIGHT_720p);
	//m_fScreenWidth = WINDOW_WIDTH_720p;
	//m_fScreenHeight = WINDOW_HEIGHT_720p;
	//
	//int windowWidth	= (int)m_fScreenWidth - 0;
	//int windowHeight = (int)m_fScreenHeight  - 0;
	//
	//SetWindowPos(m_hWnd,NULL,(GetSystemMetrics(SM_CXSCREEN) >> 1) - (windowWidth >> 1), (GetSystemMetrics(SM_CYSCREEN) >> 1) - (windowHeight >> 1),(int)m_fScreenWidth,(int)m_fScreenHeight,NULL);
	//SetWindowLong(m_hWnd,GWL_STYLE, WS_TILEDWINDOW);
	//ShowWindow(m_hWnd,SW_SHOW);
#else
	int nWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int nHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	SetWindowPos(m_hWnd,NULL,0,0,nWidth,nHeight,NULL);
	SetWindowLong(m_hWnd,GWL_STYLE, WS_POPUP);
	ShowWindow(m_hWnd,SW_MAXIMIZE);
	m_bIsWindowed = false;
#endif


	// Init Renderer and Camera
	m_pCamera = new CCamera;
	m_pCamera->Init(m_fScreenWidth,m_fScreenHeight, _hWnd);
	m_pAssetManager = new CAssetManager;

	m_cRenderer.Init(_hWnd,m_fScreenWidth,m_fScreenHeight,true,true,m_pCamera,m_pAssetManager);
	m_cTimer.Reset();
	m_cRenderer.SetTime(&m_cTimer);

	m_cTimer.Reset();
	WwiseNS::InitSoundEngine();
	CInput::InitDInput(_hWnd, _hInstance,m_fScreenWidth,m_fScreenHeight);

	WwiseNS::SetVolumeRTPC(AK::GAME_PARAMETERS::MX_VOLUME, m_fMX_Volume);
	WwiseNS::SetVolumeRTPC(AK::GAME_PARAMETERS::FX_VOLUME, m_fFX_Volume);
	WwiseNS::SetVolumeRTPC(AK::GAME_PARAMETERS::DX_VOLUME, m_fDX_Volume);

	PushState(new CSplashScreenState(this));

	InitializeCriticalSection( &m_CS );
	m_hThreadHandle = (HANDLE)_beginthreadex(0, 0, &LoadModels, this, 0, &m_unThreadID);

	//m_dwTime = GetTickCount();

	m_pGuiHelper = new CGuiHelper();

	m_bSeenBlueEnemy = false;
	m_bSeenYellowEnemy = false;
	m_bSeenRedEnemy = false;

	m_tCurrentStats.InitStats();
	m_tSavedStats.InitStats();

	return true;
}
bool CGame::Main(void)
{
	if(GetFocus() == m_hWnd)
	{
		if(Input() == false)
		{
			return false;
		}

		Update();

		Render();
	}

	return true;
}
bool CGame::Input(void)
{
	if(m_pCurrState == nullptr)
	{
		return false;
	}

	if((CInput::DIKBufferedButtonCheck(DIK_RETURN) && (CInput::DIKButtonCheck(DIK_LALT) || CInput::DIKButtonCheck(DIK_RALT))) ||
		(CInput::DIKButtonCheck(DIK_RETURN) && (CInput::DIKBufferedButtonCheck(DIK_LALT) || CInput::DIKBufferedButtonCheck(DIK_RALT))))
	{

		if(m_bIsWindowed)
		{
			int nWidth = GetSystemMetrics(SM_CXFULLSCREEN);
			int nHeight = GetSystemMetrics(SM_CYFULLSCREEN);

			SetWindowPos(m_hWnd,NULL,0,0,nWidth,nHeight,NULL);
			SetWindowLong(m_hWnd,GWL_STYLE, WS_POPUP);
			ShowWindow(m_hWnd,SW_MAXIMIZE);

			m_bIsWindowed = false;
		}
		else
		{
			int windowWidth	= (int)m_fScreenWidth - 0;
			int windowHeight = (int)m_fScreenHeight - 0;

			SetWindowPos(m_hWnd,NULL,(GetSystemMetrics(SM_CXSCREEN) >> 1) - (windowWidth >> 1), (GetSystemMetrics(SM_CYSCREEN) >> 1) - (windowHeight >> 1),(int)m_fScreenWidth,(int)m_fScreenHeight,NULL);
			SetWindowLong(m_hWnd,GWL_STYLE, WS_TILEDWINDOW);
			ShowWindow(m_hWnd,SW_SHOW);

			m_bIsWindowed = true;
		}

		//m_cRenderer.ChangeDisplayParam(g_fWINDOW_WIDTH,g_fWINDOW_HEIGHT,m_bIsWindowed);
	}

	return m_pCurrState->Input();
}

HWND temp = 0;

void CGame::Update(void)
{


	EnterCriticalSection(&m_CS);
	if(GetLoadingDone() && GetStartMainMenu())
	{
		m_bLoadingDone = false;
		switchState( new CMainMenuState(this));
		return;
	}
	LeaveCriticalSection(&m_CS);


	GetCursorPos(&m_tPoint);
	ScreenToClient(m_hWnd, &m_tPoint);
	m_tCursorPosition.x = (LONG)m_tPoint.x;
	m_tCursorPosition.y = (LONG)m_tPoint.y;

	m_cTimer.Update();
	WwiseNS::Update();

#if _DEBUG
	
	if(GetAsyncKeyState(VK_OEM_3) & 0x1)
	{
		m_bLockCursorToScreen = !m_bLockCursorToScreen;
	}

	if(m_bLockCursorToScreen == true)
	{
		RECT rWindowRect;
		GetWindowRect(m_hWnd, &rWindowRect);
		rWindowRect.bottom -= 10;
		rWindowRect.left += 10;
		rWindowRect.top += 30;
		rWindowRect.right -= 10;
		ClipCursor(&rWindowRect);
	}
	else
	{
		ClipCursor(0);
	}
#endif

	//DWORD ulNow = GetTickCount();
	float fElapsed = m_cTimer.GetDeltaTime();
	//float fElapsed2 = (ulNow - m_dwTime) / 1000.0f;

	//m_dwTime = ulNow;
	if(fElapsed > 1.0f)
	{
		fElapsed = 1.0f;
	}

	m_fDT = fElapsed;
	CInput::Update(fElapsed);

// #if _DEBUG
// 
// 	if(GetAsyncKeyState('F'))
// 	{
// 		fElapsed *= 10;
// 	}
// #endif

	if (m_pCurrState)
	{
		m_pCurrState->Update(fElapsed);
	}
}
void CGame::Render(void)
{
	if (m_pCurrState)
	{
		if(m_cRenderer.GetDevice() == NULL)
		{
			return;
		}

		m_cRenderer.Clear(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,0,0,0);
		m_cRenderer.GetDevice()->BeginScene();

		m_pCurrState->Render();

		m_cRenderer.GetDevice()->EndScene();
		m_cRenderer.GetDevice()->Present(0,0,0,0);
	}
}
void CGame::Shutdown(void)
{
	if (m_hThreadHandle)
	{
		CloseHandle(m_hThreadHandle);
		m_hThreadHandle = 0;
	}

	if ( &m_CS )
	{        
		DeleteCriticalSection( &m_CS );
	}

	for(int states = m_vStates.size(); states >= 0; --states)
	{
		PopState();
	}

	m_pCurrState = nullptr;

	if(m_pCamera != nullptr)
	{
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	if(m_pGuiHelper != nullptr)
	{
		delete m_pGuiHelper;
		m_pGuiHelper = nullptr;
	}

	m_cRenderer.Shutdown();

	if(m_pAssetManager != nullptr)
	{
		delete m_pAssetManager;
		m_pAssetManager = nullptr;
	}

	WwiseNS::TermSoundEngine();
}

void CGame::PushState(IGameState *_state)
{
	/* Ensure the invoker isn't a dumb. */
	if(_state == nullptr)
	{
		return;
	}

	// updating to avoid cascading
	CInput::Update(0.0f);

	/* Store the new state as the current state. */
	m_pCurrState = _state;
	m_pCurrState->Enter();

	/* Add the state to the stack. */
	m_vStates.push_back(_state);
}

void CGame::PopState()
{
	/* Ensure the invoker isn't a dumb. */
	if(m_vStates.empty())
	{
		return;
	}

	// updating to avoid cascading
	CInput::Update(0.0f);

	/* Get the back (top) of the stack. */
	IGameState *pBack = m_vStates.back();

	if(pBack != NULL)
	{
		pBack->Exit();
	}

	/* Free the state. */
	delete pBack;

	/* Remove the state. */
	m_vStates.pop_back();

	/* Store the new current state. */
	m_pCurrState = m_vStates.empty() ? nullptr : m_vStates.back();
}

void CGame::PopBottom()
{
	/* Ensure the invoker isn't a dumb. */
	if(m_vStates.empty())
	{
		return;
	}

	// updating to avoid cascading
	CInput::Update(0.0f);

	/* Get the bottom of the stack. */
	IGameState *pFront = m_vStates.front();

	pFront->Exit();

	/* Free the state. */
	delete pFront;

	/* Remove the state. */
	m_vStates.erase(m_vStates.begin());
	/* Store the new current state. */
	m_pCurrState = m_vStates.empty() ? nullptr : m_vStates.back();
}

//void CGame::RevertToSavedStats()
//{
	//m_tCurrentStats.m_fDamageRecovered	=	m_tSavedStats.m_fDamageRecovered;
	//m_tCurrentStats.m_fDamageTaken		=	m_tSavedStats.m_fDamageTaken;
	//m_tCurrentStats.m_nLasersFired		=	m_tSavedStats.m_nLasersFired;
	//m_tCurrentStats.m_nLasersHit		=	m_tSavedStats.m_nLasersHit;
	//m_tCurrentStats.m_nMissilesFired	=	m_tSavedStats.m_nMissilesFired;
	//m_tCurrentStats.m_nMissilesHit		=	m_tSavedStats.m_nMissilesHit;
	//m_tCurrentStats.m_nMissilesDodged	=	m_tSavedStats.m_nMissilesDodged;
	//m_tCurrentStats.m_nDeaths			=	m_tSavedStats.m_nDeaths;
//}

void CGame::ResetCurrentStats()
{
	m_tCurrentStats.m_nDamageTaken = 0;
	m_tCurrentStats.m_nLasersFired = 0;
	m_tCurrentStats.m_nLasersHit = 0;
	m_tCurrentStats.m_nMissilesFired = 0;
	m_tCurrentStats.m_nMissilesHit = 0;
	m_tCurrentStats.m_nMissilesDodged = 0;
	m_tCurrentStats.m_nDeaths = 0;
}

void CGame::switchState(IGameState *_state)
{
	/* Determine if there is a current state. */
	if(m_pCurrState)
	{
		// Exit the current state
		m_pCurrState->Exit();
		// Remove/Reinit all Sound objects used
		WwiseNS::RemoveAllSoundObjs();

		/* Delete the current state. */
		delete m_pCurrState;

		/* Remove the state. */
		m_vStates.pop_back();
	}

	// updating to avoid cascading
	CInput::Update(0.0f);

	/* Store the newly entered state. */
	m_pCurrState = _state;
	m_pCurrState->Enter();
	/* Push the new state. */
	m_vStates.push_back(m_pCurrState);
}

void CGame::SetCheckPoint(unsigned int _unCurrEvent ) 
{
	m_tPlayerCheckPoint.m_unCurrEvent = _unCurrEvent;
}

void CGame::ChangeResolution(float _Width,float _Height)
{
	if(m_vStates.size() <= 2)
	{
		m_fScreenWidth = _Width;
		m_fScreenHeight = _Height;


		if(m_bIsWindowed)
		{
			int windowWidth	= (int)m_fScreenWidth - 0;
			int windowHeight = (int)m_fScreenHeight - 0;

			SetWindowPos(m_hWnd,NULL,(GetSystemMetrics(SM_CXSCREEN) >> 1) - (windowWidth >> 1), (GetSystemMetrics(SM_CYSCREEN) >> 1) - (windowHeight >> 1),(int)m_fScreenWidth,(int)m_fScreenHeight,NULL);
			SetWindowLong(m_hWnd,GWL_STYLE, WS_TILEDWINDOW);
			ShowWindow(m_hWnd,SW_SHOW);
		}
		else
		{
			int nWidth = GetSystemMetrics(SM_CXFULLSCREEN);
			int nHeight = GetSystemMetrics(SM_CYFULLSCREEN);

			m_fScreenWidth = (float)nWidth;
			m_fScreenHeight = (float)nHeight;

			SetWindowPos(m_hWnd,NULL,0,0,nWidth,nHeight,NULL);
			SetWindowLong(m_hWnd,GWL_STYLE, WS_POPUP);
			ShowWindow(m_hWnd,SW_MAXIMIZE);
		}

		if(m_pCamera)
		{
			delete m_pCamera;
			m_pCamera = nullptr;
			m_pCamera = new CCamera();
			m_pCamera->Init(_Width,_Height, m_hWnd);
		}

		CInput::SetWidthHeight(m_fScreenWidth,m_fScreenHeight);
		m_pCamera->SetWidthHeight(_Width,_Height);

		m_cRenderer.Shutdown();
		m_pAssetManager->Shutdown();

		if(m_pAssetManager != nullptr)
		{
			delete m_pAssetManager;
			m_pAssetManager = nullptr;
		}

		m_pAssetManager = new CAssetManager;
		m_cRenderer.Init(m_hWnd,m_fScreenWidth,m_fScreenHeight,true,true,m_pCamera, m_pAssetManager);


		SetMenuTexture(m_pAssetManager->LoadTexture("RT_2D_Menu_Background.png"));

		SetAuroraFluxLogo(m_pAssetManager->LoadTexture("Aurora_Flux_Logo.png"));
		SetSmallAuroraFluxLogo(m_pAssetManager->LoadTexture("Aurora_Flux_Logo_2.png"));

		SetMenuCursor(m_pAssetManager->LoadTexture("target.png"));

		SetPSTCTexture(m_pAssetManager->LoadTexture("RT_TitleScreen_PSTC_PH.png"));
		SetGreenArrowTexture(m_pAssetManager->LoadTexture("GreenArrow.png"));
		SetGreenBoxTexture(m_pAssetManager->LoadTexture("GreenArrow2.png"));
		SetRedArrowTexture(m_pAssetManager->LoadTexture("RedArrow.png"));
		SetRedBoxTexture(m_pAssetManager->LoadTexture("RedArrow2.png"));
		SetBlueArrowTexture(m_pAssetManager->LoadTexture("BlueArrow.png"));
		SetBlueBoxTexture(m_pAssetManager->LoadTexture("BlueArrow2.png"));
		SetYellowArrowTexture(m_pAssetManager->LoadTexture("YellowArrow.png"));
		SetYellowBoxTexture(m_pAssetManager->LoadTexture("YellowArrow2.png"));
		SetHUDHealthIcon(m_pAssetManager->LoadTexture("RT_2D_HealthIcon.png"));
		SetHUDShieldIcon(m_pAssetManager->LoadTexture("RT_2D_ShieldIcon.png"));
		SetBlueEnemyShieldIcon(m_pAssetManager->LoadTexture("RT_2D_ShieldIconTwo.png"));

		SetLoadingScreenTextures(m_pAssetManager->LoadTexture("Player_Complete_1_1.png"), 0);
		SetLoadingScreenTextures(m_pAssetManager->LoadTexture("Player_Complete_2_2.png"), 1);
		SetLoadingScreenTextures(m_pAssetManager->LoadTexture("Player_Complete_3_3.png"), 2);
		SetLoadingScreenTextures(m_pAssetManager->LoadTexture("Player_Complete_4_4.png"), 3);
		SetLoadingScreenTextures(m_pAssetManager->LoadTexture("Player_Complete_5_5.png"), 4);
		SetLoadingScreenTextures(m_pAssetManager->LoadTexture("Player_Complete_6_6.png"), 5);
		SetLoadingScreenTextures(m_pAssetManager->LoadTexture("Player_Complete_7_7.png"), 6);

		SetHelpTexture(m_pAssetManager->LoadTexture("Loading Screen Controls.png"));

		SetBlackTexture(m_pAssetManager->LoadTexture("black.png"));

		SetArrowKeyTexture(m_pAssetManager->LoadTexture("dec-arrow-keys-UD.png"));
		SetEnterKeyTexture(m_pAssetManager->LoadTexture("enter_key.png"));
		SetArrowKeyLRTexture(m_pAssetManager->LoadTexture("dec-arrow-keys-LR.png"));
		SetArrow306Texture(m_pAssetManager->LoadTexture("dec-arrow-keys-300x206.png"));

		EffectInformation::resetEffects();

		//RED_PLAYER
		m_pAssetManager->LoadMainMenuModel("PlayerShipShape.mesh");
		m_pAssetManager->LoadMainMenuModel("RT_3D_Alt_Asteroid_Shape1.mesh");
		m_pAssetManager->LoadMainMenuModel("RT_3D_Alt_Asteroid_Shape2.mesh");
		m_pAssetManager->LoadMainMenuModel("RT_3D_Planet_Model_SurfaceShape.mesh"); //  RT_3D_Planet_Model_SurfaceShape
		m_pAssetManager->LoadMainMenuModel("Planet3Shape.mesh"); //  RT_3D_Planet_Model_SurfaceShape
		m_pAssetManager->LoadMainMenuModel("RT_3D_Planet4_ModelShape.mesh");
		m_pAssetManager->LoadMainMenuModel("RT_3D_Station2_ModelShape.mesh");
		m_pAssetManager->LoadMainMenuModel("SpaceDebrisShape4.mesh");
		m_pAssetManager->LoadMainMenuModel("Space_Debris_2.mesh");
		m_pAssetManager->LoadMainMenuModel("SpaceDebris_0Shape1.mesh");
		m_pAssetManager->LoadMainMenuModel("ShieldQuad.mesh");
		m_vStates[0]->Enter();
	}	

	//if(m_vStates.size() > 1)
	//{
	//	m_pAssetManager->LoadModel("RT_3D_CruiserShape.mesh");
	//	
	//	//BLUE_ENEMY
	//	m_pAssetManager->LoadModel("RT_3D_BossShape.mesh");
	//	
	//	//YELLOW_ENEMY
	//	m_pAssetManager->LoadModel("GruntEnemyShape.mesh");
	//	
	//	//RED_PLAYER
	//	m_pAssetManager->LoadModel("PlayerShipShape.mesh");
	//	
	//	//BLUE_PLAYER
	//	m_pAssetManager->LoadModel("RT_3D_BluePlayer_PH.mesh");
	//	
	//	//YELLOW_PLAYER
	//	m_pAssetManager->LoadModel("RT_3D_YellowPlayer_PH.mesh");
	//	
	//	//MISSILE
	//	m_pAssetManager->LoadModel("MissileShape.mesh");
	//	//LASER
	//	m_pAssetManager->LoadModel("RT_3D_Laser_PH.mesh");
	//	
	//	//ASTEROID
	//	m_pAssetManager->LoadModel("RT_3D_Alt_Asteroid_Shape1.mesh");
	//	
	//	//WAYPOINT
	//	m_pAssetManager->LoadModel("RT_3D_Waypoint.mesh");
	//	
	//	//ShieldSphere and Quad
	//	m_pAssetManager->LoadModel("ShieldSphere.mesh");
	//	m_pAssetManager->LoadModel("ShieldQuad.mesh");
	//	
	//	//BOSS
	//	m_pAssetManager->LoadModel("LargeCarrierShape.mesh");
	//	m_pAssetManager->LoadModel("RT_3D_ShieldGenerator_ModelShape.mesh");
	//	m_pAssetManager->LoadModel("Carrier_gunShape.mesh");
	//	
	//	m_pAssetManager->LoadModel("RT_3D_LARGETURRET_PH.mesh");
	//	m_pAssetManager->LoadModel("RT_3D_BossTurretShape.mesh");
	//	m_pAssetManager->LoadModel("RT_3D_SUPERTURRET_PH.mesh");
	//	
	//	m_pAssetManager->LoadModel("RT_3D_Alt_Asteroid_Shape2.mesh");
	//	
	//	m_pAssetManager->LoadModel("BossShieldSphere.mesh");
	//	
	//	m_pAssetManager->LoadModel("RT_3D_Planet_Model_SurfaceShape.mesh"); //  RT_3D_Planet_Model_SurfaceShape
	//	
	//	m_pAssetManager->LoadModel("Planet3Shape.mesh"); //  RT_3D_Planet_Model_SurfaceShape
	//	m_pAssetManager->LoadModel("RT_3D_Planet4_ModelShape.mesh");
	//	
	//	m_pAssetManager->LoadModel("RT_3D_Station2_ModelShape.mesh");
	//	m_pAssetManager->LoadModel("SpaceDebrisShape4.mesh");
	//	m_pAssetManager->LoadModel("Space_Debris_2.mesh");
	//	m_pAssetManager->LoadModel("SpaceDebris_0Shape1.mesh");
	//	
	//	m_pAssetManager->LoadModel("RT_3D_Barrier_ModelShape.mesh");
	//	m_pAssetManager->LoadModel("StationShape.mesh");
	//	m_pAssetManager->LoadModel("RT_3D_Planet_ModelShape.mesh");
	//	
	//	m_pAssetManager->LoadModel("RT_3D_Satellite.mesh");
	//	m_pAssetManager->LoadModel("RT_3D_RedPlayer_PH.mesh");
	//	m_pAssetManager->LoadModel("RT_3D_CruiserShapeLarge.mesh");
	//	
	//	m_pAssetManager->LoadModel("RT_3D_GrassAsteroid_PH.mesh");
	//	m_pAssetManager->LoadModel("RT_3D_IceAsteroid_PH.mesh");
	//}

}
