/***********************************************
* Filename:  		GamePlayState.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AG
* Author:    		Andrew Goossens
* Purpose:   		This will display the game state for
*						the game. 
*					
************************************************/
#include "../StdAfx.h"
#include "../Entity/Lasers.h"
#include "../Entity/Missile.h"
#include "../Entity/YellowEnemy.h"
#include "../Entity/BlueEnemy.h"
#include "../Entity/RedEnemy.h"
#include "../Entity/Shield.h"
#include "../Entity/Generator.h"
#include "../AI/YellowShooting.h"
#include "../AI/WanderingMovement.h"
#include "MainMenuState.h"
#include "GamePlayState.h"
#include "PauseState.h"	
#include "../Timer/CTimer.h"
#include "../Collision/Sphere.h"

#include "../Collision/Physics.h"
#include "../Entity/Asteroid.h"
#include "GameOverState.h"
#include "../Script Manager/ScriptManager.h"
#include "WinState.h"
#include "../Entity/Boss.h"
#include "../AI/AIHelper.h"
#include "../Particle System/EffectInformation.h"
#include "../Player State/InactivePlayer.h"


/*****************************************************************
* CCreditsState():  Default Constructor. Will set its type and behaviors.
* Ins:			    None    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/


GamePlayState::GamePlayState(void) : m_pGame(nullptr), 
	m_pPlayer(nullptr), m_pObjectManager(nullptr),  m_nDamageDirectionVertical(-1), m_fHelpScreenAlpha(0), m_nHelpScreenTextureID(-1), m_bHelpScreenToggleOn(false),
	m_nDamageDirectionHorizontal(-1), m_nRedEnemyIconTex(-1), m_nBlueEnemyIconTex(-1), m_bEnemyBehind(false), m_pPlayerBehavior(nullptr), m_bLaserSide(true), m_bIsAlive(true), m_bFirstTime(true)
{
}

/*****************************************************************
* ~CCreditsState():  Virtual destructor used when shutting down the game state
* Ins:			    none    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
GamePlayState::~GamePlayState(void)
{
}

/*****************************************************************
* Enter(): Enters the game state. Will set any variables needed for the game state
* Ins:			    none    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void GamePlayState::Enter()
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//first load
	//FirstEnter();
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//second load
	//SecondEnter();
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//third load
	//ThirdEnter();
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//fourth load
	//FourthEnter();
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	WwiseNS::SoundEvent(eBGM_LevelOneMusic, ePlay);
	if(m_pPlayer)
		m_pPlayer->SetInactive(false);

	m_pGame->RevertToSavedStats();

	m_bHelpScreenToggleOn = false;
	m_fCompletedBucket = 0.0f;
	m_bRenderIndicatorBox = false;
	m_bSlowDodgeEnemy = false;
	m_bNearExplodingMissiles = false;
	m_pObjectManager->SetPlayerLose(false);
	//m_pGame->GetCamera()->ResetCamera();

	static bool doOnceIntro = false;
	if(doOnceIntro == false)
	{
		doOnceIntro = true;
	}
	else
	{
		m_pCinematicCamera->SetAlreadySeenIntro(true);
	}
}

void GamePlayState::FirstEnter()
{
	m_bPlaySoundCruiserEntrance = true;
	m_bPlaySoundMarauderEntrance = true;
	m_bPlaySoundSeekerEntrance = true;
	m_bPlaySoundHydraTimerRanOut = true;
	m_bPlaySoundIncommingWave = true;
	m_bPlaySoundWaveComplete = true;
	m_bPlaySoundCheckpointArrival = true;
	m_fPlaySoundBucketBuffer = 0.0f;
	m_pScriptManager = nullptr;
	m_d3dvPlayerWinPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fTeachXLocation = -100;
	m_bShowRedData = false;
	m_bShowBlueData = false;
	m_bShowYellowData = false;
	m_bShowBossData = false;
	m_fSkipTextAlpha = 255;
	startTimer = 0.0f;
	m_nMissilesToFire = 0;
	m_fMissileDelay = 0.5f;
	bIsLooping = false;
	m_fBehindSound = 0.0f;
	m_bShowHealth = true;
	m_bShowShields = true;
	m_bTooFar = false;
	m_bTurnAround = false;
	gameJustStarted = true;
	m_bInCombat = false;
	CInput::SetCursorLock(true);
	CInput::SetHasDodged(false);
	m_fBossAsteroidSpawnTimer = 0.0f;
	m_bHasDodgedMissile = false;
	m_bLockedOn = false;
	m_bWithinRange = false;
	m_fTimeToLockOn = 0.8f;
	m_bSpawnYellowEnemy = false;
	m_bChangeTurnAroundCam = false;
	m_bReturnToCamera = false;
	m_fLockOnTimer = 0.0f;
	m_fMissileReloadTimer = 1.3f;
	m_bShowWaypoint = true;
	spawnBoss = (false);
	bossFight = false;
	s_AIHelper = new CAIHelper();
	MakebossOnce = true;
	m_nFlashAlpha = 155;
	m_bFlashHealth = false;
	whichWay = false;
	m_pCamera = m_pGame->GetCamera();
	m_pCamera->SetWaitForGame(true);
	m_pRenderer = m_pGame->GetRenderer();
	m_pObjectManager = new CObjectManager(m_pRenderer, m_pCamera, s_AIHelper);
	m_nExplosionFade = m_pRenderer->GetAssetManager()->LoadTexture("FullScreen.png");
	//	m_nBlackTexture = m_pRenderer->GetAssetManager()->LoadTexture("black.png");
	m_nRedTexture = m_pRenderer->GetAssetManager()->LoadTexture("red.png");
	m_nHelpScreenTextureID = m_pGame->GetLoadingScreenTexture(0);
	m_bFlashed = false;
	m_fExplosionFadeAlpha = 255;
	m_pCinematicCamera = new CCameraObject(m_pCamera, m_pObjectManager);
	m_pObjectManager->AddObject(m_pCinematicCamera);
	//EffectInformation::resetEffects();
	EffectInformation::LoadAllEffects("AllEffects.txt", m_pRenderer->GetAssetManager(), m_pObjectManager);
	m_pPlayer = new CPlayer();
	m_pPlayer->SetCamera(m_pGame->GetCamera());
	m_fFadeToWhiteAlpha = 0;
	m_pPlayer->SetInvisibleShieldMesh(m_pRenderer->GetAssetManager()->LoadMeshSphere("PlayerShieldInvis",0));
	m_pPlayer->SetVisibleShieldMesh(m_pRenderer->GetAssetManager()->LoadMeshSphere("PlayerShieldVis",((CSphere*)(m_pPlayer->GetBV()))->GetRadius()));
	m_pObjectManager->AddObject(m_pPlayer);

	m_bNeedToProceed = false;
	m_fProceedTimer = 0.0f;

	CAssetManager* pAssetManager = m_pGame->GetRenderer()->GetAssetManager();

	pAssetManager->LoadModel("RT_3D_CruiserShape.mesh");

	//BLUE_ENEMY
	pAssetManager->LoadModel("RT_3D_BossShape.mesh");

	//YELLOW_ENEMY
	pAssetManager->LoadModel("GruntEnemyShape.mesh");

	//RED_PLAYER
	pAssetManager->LoadModel("PlayerShipShape.mesh");

	//BLUE_PLAYER
	pAssetManager->LoadModel("RT_3D_BluePlayer_PH.mesh");

	//YELLOW_PLAYER
	pAssetManager->LoadModel("RT_3D_YellowPlayer_PH.mesh");

	//MISSILE
	pAssetManager->LoadModel("MissileShape.mesh");
	//LASER
	pAssetManager->LoadModel("RT_3D_Laser_PH.mesh");

	//ASTEROID
	pAssetManager->LoadModel("RT_3D_Alt_Asteroid_Shape1.mesh");
}
void GamePlayState::SecondEnter()
{
	m_bTooFar = false;
	m_bTurnAround = false;

	m_pPlayer->SetForceReturn(false);
	m_pCamera->SetForceReturn(false);

	m_pScriptManager = new CScriptManager(m_pObjectManager, m_pPlayer, m_pGame, s_AIHelper, m_pRenderer, m_pGame->GetDialogSys() );

	m_pScriptManager->LoadTextScript("FinalLevel.txt");
	//m_pScriptManager->LoadTextScript("Test2.txt");
//	m_pScriptManager->LoadTextScript("BossTest.txt");
	m_pScriptManager->SetCinematicCamera(m_pCinematicCamera);
	m_pCamera->SetInMainMenu(true);

	if( m_pScriptManager->GetCurrentWaypoint() )
	{
		m_d3dPrevWaypointPosition = m_pScriptManager->GetCurrentWaypoint()->GetPosition();
	}
	else
	{
		m_d3dPrevWaypointPosition = D3DXVECTOR3(-1,-1,-1);
	}
	for(int nNumYellowEnemies = 0; nNumYellowEnemies < 20; nNumYellowEnemies++)
	{
		CYellowEnemy* pYellowEnemy = new CYellowEnemy(m_pPlayer, s_AIHelper,  m_pScriptManager->GetCurrentWaypoint(), m_pObjectManager) ;
		m_pObjectManager->AddToInactiveEnemy(pYellowEnemy);
	}
	for(int nEachBullet = 0; nEachBullet < 20; nEachBullet++)
	{
		CRedEnemy* pRedEnemy = new CRedEnemy(m_pPlayer, s_AIHelper,m_pScriptManager->GetCurrentWaypoint(), m_pObjectManager );
		m_pObjectManager->AddToInactiveEnemy(pRedEnemy);
	}
	for(int nEachBullet = 0; nEachBullet < 20; nEachBullet++)
	{
		CBlueEnemy* pBlueEnemy = new CBlueEnemy(m_pPlayer, s_AIHelper,m_pScriptManager->GetCurrentWaypoint(), m_pObjectManager);
		m_pObjectManager->AddObject(pBlueEnemy->GetShield());
		m_pObjectManager->AddToInactiveEnemy(pBlueEnemy);
	}
	m_pRedPlayer = new CRedPlayer(m_pPlayer);
	m_pBluePlayer = new CBluePlayer(m_pPlayer);
	m_pYellowPlayer = new CYellowPlayer(m_pPlayer);
	m_pWarpPlayer = new CPlayerEnterWarp(m_pPlayer);
	m_pInactivePlayer = new CInactivePlayer(m_pPlayer);
	m_pPrevPlayerBehavior = m_pPlayerBehavior = m_pRedPlayer;

	CAssetManager* pAssetManager = m_pGame->GetRenderer()->GetAssetManager();

	//WAYPOINT
	pAssetManager->LoadModel("RT_3D_Waypoint.mesh");

	//ShieldSphere and Quad
	pAssetManager->LoadModel("ShieldSphere.mesh");
	pAssetManager->LoadModel("ShieldQuad.mesh");

	//BOSS
	pAssetManager->LoadModel("LargeCarrierShape.mesh");
	pAssetManager->LoadModel("RT_3D_ShieldGenerator_ModelShape.mesh");
	pAssetManager->LoadModel("LargeCarrier_gunShape.mesh");

	pAssetManager->LoadModel("RT_3D_Boss2TurretShape.mesh");
	pAssetManager->LoadModel("RT_3D_BossTurretShape.mesh");
	pAssetManager->LoadModel("RT_3D_SUPERTURRET_PH.mesh");

}
void GamePlayState::ThirdEnter()
{
	for(int nEachBullet = 0; nEachBullet < 200; nEachBullet++)
	{
		CLaser* pLaserObj = new CLaser(m_pPlayer, s_AIHelper);
		m_pObjectManager->AddToInactiveArmament(pLaserObj);
	}

	for(int nEachMissile = 0; nEachMissile < 200; ++nEachMissile)
	{
		CMissile* pMissileObj = new CMissile(0,m_pPlayer, s_AIHelper);
		m_pObjectManager->AddToInactiveArmament(pMissileObj);
	}

	for(int nEachAsteroid = 0; nEachAsteroid < 200; ++nEachAsteroid)
	{
		CAsteroid* pAsteroid2 = new CAsteroid(float(1.0f));
		m_pObjectManager->AddToInactiveAsteroid(pAsteroid2);
	}

	for (unsigned int i = 0; i < m_pAsteroidList.size(); i++)
	{
		if (m_pAsteroidList[i])
		{
			m_pAsteroidList[i] = nullptr;
		}
	}
	//spread them out more, have too 1000
	for (int i = 5; i < 20; i++)
	{
		CAsteroid* pAsteroid2 = (CAsteroid*)m_pObjectManager->ReturnAsteroidToMasterList(i / 6.0f);
		//pAsteroid2->SetRadius( i / 10.0f);
		pAsteroid2->SetPosition(float((rand () % 18) - 12) * i * 3, float(i * ((rand () % 25) - 8)) , 150.0f + ((rand () % 15) + 5) *( i));
		m_pAsteroidList.push_back(pAsteroid2);
	}
	for (int i = 5; i < 20; i++)
	{
		CAsteroid* pAsteroid2 = (CAsteroid*)m_pObjectManager->ReturnAsteroidToMasterList(i / 6.0f);
		//pAsteroid2->SetRadius( i / 10.0f);
		pAsteroid2->SetPosition(float((rand () % 18) - 12) * i * 3, float(i * ((rand () % 25) - 8)) , 550.0f + ((rand () % 15) + 5) *( i));
		m_pAsteroidList.push_back(pAsteroid2);
	}
	for (int i = 5; i < 20; i++)
	{
		CAsteroid* pAsteroid2 = (CAsteroid*)m_pObjectManager->ReturnAsteroidToMasterList(i / 6.0f);
		//pAsteroid2->SetRadius( i / 10.0f);
		pAsteroid2->SetPosition(float((rand () % 18) - 12) * i * 3, float(i * ((rand () % 25) - 8)) , 350.0f + ((rand () % 15) + 5) *( i));
		m_pBossAsteroidList.push_back(pAsteroid2);
	}
	for (int i = 5; i < 20; i++)
	{
		CAsteroid* pAsteroid2 = (CAsteroid*)m_pObjectManager->ReturnAsteroidToMasterList(i / 6.0f);
		//pAsteroid2->SetRadius( i / 10.0f);
		pAsteroid2->SetPosition(float((rand () % 18) - 12) * i * 3, float(i * ((rand () % 25) - 8)) , 950.0f + ((rand () % 15) + 5) *( i));
		m_pBossAsteroidList.push_back(pAsteroid2);
	}
	m_fWinTimer = 0;
	m_pGame->GetCamera()->DetachCamera(false);

	CAssetManager* pAssetManager = m_pGame->GetRenderer()->GetAssetManager();

	pAssetManager->LoadModel("RT_3D_Alt_Asteroid_Shape2.mesh");

	pAssetManager->LoadModel("BossShieldSphere.mesh");

	pAssetManager->LoadModel("RT_3D_Planet_Model_SurfaceShape.mesh"); //  RT_3D_Planet_Model_SurfaceShape

	pAssetManager->LoadModel("Planet3Shape.mesh"); //  RT_3D_Planet_Model_SurfaceShape
	pAssetManager->LoadModel("RT_3D_Planet4_ModelShape.mesh");

	pAssetManager->LoadModel("RT_3D_Station2_ModelShape.mesh");
	pAssetManager->LoadModel("SpaceDebrisShape4.mesh");
	pAssetManager->LoadModel("Space_Debris_2.mesh");
	pAssetManager->LoadModel("SpaceDebris_0Shape1.mesh");
}

void GamePlayState::FourthEnter()
{
	m_bLaserSide = true;
	float xRatio = m_pGame->GetRenderer()->GetXRatio();
	float yRatio = m_pGame->GetRenderer()->GetYRatio();
	//m_nGreenArrowText = m_pRenderer->GetAssetManager()->LoadTexture("GreenArrow.png");
	//m_nRedArrowText   = m_pRenderer->GetAssetManager()->LoadTexture("RedArrow.png");
	//m_nBlueArrowText = 	m_pRenderer->GetAssetManager()->LoadTexture("BlueArrow.png");
	//m_nYellowArrowText = 	m_pRenderer->GetAssetManager()->LoadTexture("YellowArrow.png");
	//m_nHUDHealthIcon = m_pRenderer->GetAssetManager()->LoadTexture("RT_2D_HealthIcon.png");
	//m_nHUDShieldIcon = m_pRenderer->GetAssetManager()->LoadTexture("RT_2D_ShieldIcon.png");
	//m_nBlueEnemyShieldIcon = 	m_pRenderer->GetAssetManager()->LoadTexture("RT_2D_ShieldIconTwo.png");

	m_nReticleTex		=	m_pRenderer->GetAssetManager()->LoadTexture("target.png");
	D3DXMatrixIdentity(&m_ReticleMat);
	D3DXMatrixIdentity(&m_TargetMat);
	m_TargetMat = *m_pGame->GetCamera()->GetViewMatrix();

	m_rArrowRect.top	= 0;
	m_rArrowRect.left   = 0;
	m_rArrowRect.right  = 64;
	m_rArrowRect.bottom = 64;

	m_rHorizontalDamage.top	= 0;
	m_rHorizontalDamage.left   = 0;
	m_rHorizontalDamage.right  = 1024;
	m_rHorizontalDamage.bottom = 256;

	m_rVerticalDamage.top	= 0;
	m_rVerticalDamage.left   = 0;
	m_rVerticalDamage.right  = 256;
	m_rVerticalDamage.bottom = 1024;

	D3DXMATRIX tmpScale;
	D3DXMatrixIdentity(&tmpScale);
	D3DXMatrixScaling(&tmpScale, m_pRenderer->GetXRatio(), m_pRenderer->GetYRatio(), 1.0f);

	D3DXMatrixIdentity(&m_d3dTopSideDamage);
	D3DXMatrixMultiply(&m_d3dTopSideDamage, &m_d3dTopSideDamage, &tmpScale);
	m_d3dTopSideDamage._41 = m_pGame->GetWidth()/2.0f;
	m_d3dTopSideDamage._42 = m_pGame->GetHeight()/6.0f;

	D3DXMatrixIdentity(&m_d3dBotSideDamage);
	D3DXMatrixRotationZ(&m_d3dBotSideDamage, D3DXToRadian(180));
	D3DXMatrixMultiply(&m_d3dBotSideDamage, &m_d3dBotSideDamage, &tmpScale);
	m_d3dBotSideDamage._41 = m_pGame->GetWidth()/2.0f;
	m_d3dBotSideDamage._42 = m_pGame->GetHeight() * (5.0f/6.0f);

	D3DXMatrixIdentity(&m_d3dLeftSideDamage);
	D3DXMatrixMultiply(&m_d3dLeftSideDamage, &m_d3dLeftSideDamage, &tmpScale);
	m_d3dLeftSideDamage._41 = m_pGame->GetWidth()/8.0f;
	m_d3dLeftSideDamage._42 = m_pGame->GetHeight()/2.0f;

	D3DXMatrixIdentity(&m_d3dRightSideDamage);
	D3DXMatrixRotationZ(&m_d3dRightSideDamage, D3DXToRadian(180));
	D3DXMatrixMultiply(&m_d3dRightSideDamage, &m_d3dRightSideDamage, &tmpScale);
	m_d3dRightSideDamage._41 = m_pGame->GetWidth() * (7.0f/8.0f);
	m_d3dRightSideDamage._42 = m_pGame->GetHeight()/2.0f;


	m_rEnemyIconRect.top = m_rEnemyIconRect.left = 0;
	m_rEnemyIconRect.bottom = m_rEnemyIconRect.right = 64;

	D3DXMatrixIdentity(&m_d3dEnemyIconPos);


	m_nYellowLockOnTex = m_pRenderer->GetAssetManager()->LoadTexture("lockOnProcess.png");
	m_nRedLockOnTex = m_pRenderer->GetAssetManager()->LoadTexture("lockOnConfirm.png");
	m_nDamageDirectionVertical = m_pRenderer->GetAssetManager()->LoadTexture("Vertical Damage Effect.png");
	m_nDamageDirectionHorizontal = m_pRenderer->GetAssetManager()->LoadTexture("Horizontal Damage Effect.png");
	m_nRedEnemyIconTex = m_pRenderer->GetAssetManager()->LoadTexture("Red Icon.png");
	m_nBlueEnemyIconTex = m_pRenderer->GetAssetManager()->LoadTexture("Blue Icon.png");
	m_nYellowEnemyIconTex = m_pRenderer->GetAssetManager()->LoadTexture("Yellow Icon.png");

	m_nLockOnWarningRedTex = m_pRenderer->GetAssetManager()->LoadTexture("RT_MissileLockonWarning_Red.png");
	m_nLockOnWarningYellowTex = m_pRenderer->GetAssetManager()->LoadTexture("RT_MissileLockonWarning_Yellow.png");
	m_nLockOnNoWarningGreenTex = m_pRenderer->GetAssetManager()->LoadTexture("RT_MissileLockonNoWarning_Green.png");

	m_rWarningRect.top	= 0;
	m_rWarningRect.left   = 0;
	m_rWarningRect.right  = 64;
	m_rWarningRect.bottom = 64;
	D3DXMatrixIdentity(&m_d3dWarningMat);
	D3DXMatrixTranslation(&m_d3dWarningMat, 530, 334,0);

	m_pGame->GetDialogSys()->Initialize("", m_pRenderer);
	//m_pGame->GetDialogSys()->ChangeSection(eDialog_Start);
	//	BlackTexture	=	m_pRenderer->GetAssetManager()->LoadTexture("black.png");
	m_rBlack.left = 0;
	m_rBlack.top = 0;
	m_rBlack.right = 100;
	m_rBlack.bottom = 100;

	D3DXMatrixIdentity(&m_BlackMat);
	D3DXMATRIX scale;
	D3DXMatrixScaling(&scale, 4.2f * xRatio, 3.6f * yRatio, 2.0f);
	m_BlackMat = m_BlackMat * scale;
	m_BlackMat._41 = 138 * xRatio;
	m_BlackMat._42 = 565 * yRatio;


	D3DXMatrixIdentity(&m_HealthBox);
	D3DXMatrixScaling(&scale, 5.0f * xRatio, 1.0f * yRatio, 2.0f);
	m_HealthBox = m_HealthBox * scale;
	m_HealthBox._41 = 0 *xRatio;
	m_HealthBox._42 = 680 * yRatio;

	D3DXMatrixIdentity(&m_DistanceBox);
	D3DXMatrixScaling(&scale, 2.0f * xRatio, 0.5f * yRatio, 2.0f);
	m_DistanceBox = m_DistanceBox * scale;
	m_DistanceBox._41 = 900 * xRatio;
	m_DistanceBox._42 = 615 * yRatio;

	D3DXMatrixIdentity(&m_EnemyBox);
	D3DXMatrixScaling(&scale, 2.5f * xRatio, 2.0f * yRatio, 2.0f);
	m_EnemyBox = m_EnemyBox * scale;
	m_EnemyBox._41 = 900 * xRatio;
	m_EnemyBox._42 = 670 * yRatio;

	D3DXMatrixIdentity(&m_ArrowMat);
	D3DXMATRIX rotateeee;
	D3DXMatrixRotationZ(&rotateeee, 1.57f);
	m_ArrowMat = m_ArrowMat * rotateeee;

	m_ArrowMat._41 = -75 *xRatio;
	m_ArrowMat._42 = 325 * yRatio;

	D3DXMATRIX scale3;
	D3DXMatrixScaling(&scale3, 0.75f * xRatio, 0.75f * yRatio, 1);
	m_ArrowMat = m_ArrowMat * scale3;

	D3DXMatrixIdentity(&m_ArrowMat2);

	m_ArrowMat2._41 = -75 * xRatio;
	m_ArrowMat2._42 = 325 * yRatio;

	D3DXMATRIX scale4;
	D3DXMatrixScaling(&scale4, 0.50f * xRatio, 0.50f * yRatio,1);
	m_ArrowMat2 = m_ArrowMat2 * scale4;

	m_pRenderer->SetPlayer(m_pPlayer);

	m_bIsAlive = true;
	m_pDeadPlayer.SetPlayer(m_pPlayer);

	m_pObjectManager->SetBossFight(false);

	RECT rect; 

	GetWindowRect(m_pGame->GetHWND(), &rect);
	SetCursorPos(int(rect.left) + int(m_pGame->GetWidth()*0.5f), int(rect.top) + int(m_pGame->GetHeight()*0.5f));

	if( !m_bContinued )
	{
		m_bInCinematic = true;
		WwiseNS::SetVolumeRTPC( AK::GAME_PARAMETERS::MX_VOLUME, m_pGame->GetMusicVolume() - 10);
		m_pCamera->DetachCamera(true);
		m_pPlayer->SetIsVisable(false);
		D3DXMATRIX mat; 
		D3DXMatrixTranslation(&mat, 0.0f, 0.0f, 4000.0f);
		*m_pCamera->GetCurrentMatrix() = mat * *m_pCamera->GetCurrentMatrix();
		m_pCinematicCamera->SetPlayer(m_pPlayer);	
		CPhysics::LookAt(m_pCamera->GetCurrentMatrix(), m_pPlayer->GetMatrix());
		m_pObjectManager->SetInCinematic(true);
	}
	else
	{
		m_bInCinematic = false;
		WwiseNS::SetVolumeRTPC( AK::GAME_PARAMETERS::MX_VOLUME, m_pGame->GetMusicVolume());
		m_pCamera->DetachCamera(false);
		m_pCinematicCamera->SetIsIntroCinematic(false);
	}

	EffectInformation::CreateEffect(eSPACE_DUST, &m_esSpaceDust, m_pPlayer, true, true);
	EffectInformation::CreateEffect(eWARP_DUST, &m_esWarpDust, m_pPlayer, true, false);

	CreateAmbientFighters();

	m_pSceneryManager = new CSceneryManager(m_pObjectManager);
	m_pSceneryManager->CreateScene();
	m_bPlayCinematic = false;
	m_fWaitForCinematicBucket = 0.0f;

	CAssetManager* pAssetManager = m_pGame->GetRenderer()->GetAssetManager();
	pAssetManager->LoadModel("RT_3D_Barrier_ModelShape.mesh");
	pAssetManager->LoadModel("StationShape.mesh");
	pAssetManager->LoadModel("RT_3D_Planet_ModelShape.mesh");

	pAssetManager->LoadModel("RT_3D_Satellite.mesh");
	pAssetManager->LoadModel("RT_3D_RedPlayer_PH.mesh");
	pAssetManager->LoadModel("RT_3D_SpaceTraveler_ModelShape_Large.mesh");

	pAssetManager->LoadModel("BossLaser.mesh");
	pAssetManager->LoadModel("RT_3D_GrassAsteroid_PH.mesh");
	pAssetManager->LoadModel("RT_3D_IceAsteroid_PH.mesh");

	if(m_pGame->GetCheckPoint()->m_unCurrEvent == 0)
	{
		D3DXMATRIX d3dTempPlayer; //= m_pThePlayer->GetMatrixCopy();
		D3DXMatrixIdentity(&d3dTempPlayer);
		m_pGame->GetCamera()->SetCheckpointCamera(d3dTempPlayer);
	}
}
void GamePlayState::CreateAmbientFighters()
{
	D3DXMATRIX _d3dCenterPointMatrix = *m_pScriptManager->GetCurrentWaypoint()->GetMatrix();
	D3DXMATRIX _d3dAmbientWorldMatrix = _d3dCenterPointMatrix;
	float _fBoundingBoxSpacing = 200;
	short _sEnemyCount = 4;

	//AddSceneryObject(eSPACESTATION1, D3DXVECTOR3(-4150, 900, 2950),0, 3 );;
	_d3dAmbientWorldMatrix._41 = -4150 - (_fBoundingBoxSpacing);
	_d3dAmbientWorldMatrix._42 = 1200 + (_fBoundingBoxSpacing);
	_d3dAmbientWorldMatrix._43 = 2950 + (_fBoundingBoxSpacing);
	m_pObjectManager->AddAmbientFight(_d3dAmbientWorldMatrix, _sEnemyCount, _fBoundingBoxSpacing, _fBoundingBoxSpacing, _fBoundingBoxSpacing);

	//AddSceneryObject(eSPACE_STATION, D3DXVECTOR3(-14000,-10000,-10000) , 6, .025f,0,D3DXVECTOR3(0,0,0),0, 0);	
	_d3dAmbientWorldMatrix._41 = -14000 - (_fBoundingBoxSpacing);
	_d3dAmbientWorldMatrix._42 = -10000 - (_fBoundingBoxSpacing);
	_d3dAmbientWorldMatrix._43 = -10000 - (_fBoundingBoxSpacing);
	m_pObjectManager->AddAmbientFight(_d3dAmbientWorldMatrix, _sEnemyCount, _fBoundingBoxSpacing, _fBoundingBoxSpacing, _fBoundingBoxSpacing);

	////AddSceneryObject(eSPACE_STATION, D3DXVECTOR3(-6100,9000,1900), 10 );
	_d3dAmbientWorldMatrix._41 = -6100 - (_fBoundingBoxSpacing);
	_d3dAmbientWorldMatrix._42 = 9000 + (_fBoundingBoxSpacing);
	_d3dAmbientWorldMatrix._43 = 1900 + (_fBoundingBoxSpacing);
	m_pObjectManager->AddAmbientFight(_d3dAmbientWorldMatrix, _sEnemyCount, _fBoundingBoxSpacing, _fBoundingBoxSpacing, _fBoundingBoxSpacing);

	////AddSceneryObject(eSPACE_STATION, D3DXVECTOR3( 6100,0,10900) , 1, 2);
	_d3dAmbientWorldMatrix._41 = 6100;
	_d3dAmbientWorldMatrix._42 = 0;
	_d3dAmbientWorldMatrix._43 = 10900;
	m_pObjectManager->AddAmbientFight(_d3dAmbientWorldMatrix, _sEnemyCount, _fBoundingBoxSpacing, _fBoundingBoxSpacing, _fBoundingBoxSpacing);

	////AddSceneryObject(eSPACESTATION1, D3DXVECTOR3(-20000,-15000,-13000) , 2, .45f);	
	_d3dAmbientWorldMatrix._41 = -20000;
	_d3dAmbientWorldMatrix._42 = -15000;
	_d3dAmbientWorldMatrix._43 = -13000;
	m_pObjectManager->AddAmbientFight(_d3dAmbientWorldMatrix, _sEnemyCount, _fBoundingBoxSpacing, _fBoundingBoxSpacing, _fBoundingBoxSpacing);

	////AddSceneryObject(eSPACESTATION1, D3DXVECTOR3(4150, 4900, 2950),0, 4 );
	_d3dAmbientWorldMatrix._41 = 4150;
	_d3dAmbientWorldMatrix._42 = 4900;
	_d3dAmbientWorldMatrix._43 = 2950;
	m_pObjectManager->AddAmbientFight(_d3dAmbientWorldMatrix, _sEnemyCount, _fBoundingBoxSpacing, _fBoundingBoxSpacing, _fBoundingBoxSpacing);

	_d3dAmbientWorldMatrix._41 = -200;
	_d3dAmbientWorldMatrix._42 = 200;
	_d3dAmbientWorldMatrix._43 = -4000;
	m_pObjectManager->AddAmbientFight(_d3dAmbientWorldMatrix, _sEnemyCount, _fBoundingBoxSpacing * 1.5f, _fBoundingBoxSpacing * 1.5f, _fBoundingBoxSpacing * 0.5f);

	m_pObjectManager->ToggleToShowAmbientDogFights();
}

/*****************************************************************
* Exit(): Exits the game state. Will set any variables used to null values
* Ins:			    none    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void GamePlayState::Exit()
{
	if (s_AIHelper)
	{
		delete s_AIHelper;
		s_AIHelper = nullptr;
	}

	if(m_pObjectManager != nullptr)
	{
		m_pObjectManager->Shutdown();
		delete m_pObjectManager;
		m_pObjectManager = nullptr;
	}

	if(m_pRedPlayer != nullptr)
	{
		delete m_pRedPlayer;
		m_pRedPlayer = nullptr;
	}

	if(m_pBluePlayer != nullptr)
	{
		delete m_pBluePlayer;
		m_pBluePlayer = nullptr;
	}

	if(m_pYellowPlayer != nullptr)
	{
		delete m_pYellowPlayer;
		m_pYellowPlayer = nullptr;
	}

	if(m_pInactivePlayer != nullptr)
	{
		delete m_pInactivePlayer;
		m_pInactivePlayer = nullptr;
	}

	if(m_pWarpPlayer != nullptr)
	{
		delete m_pWarpPlayer;
		m_pWarpPlayer = nullptr;
	}
	if (m_pSceneryManager)
	{
		delete m_pSceneryManager;
		m_pSceneryManager = nullptr;
	}

	m_pPlayer = nullptr;

	if(m_pCamera != nullptr)
	{
		m_pCamera->Shake(1,1,0.0f);
		m_pCamera = nullptr;
	}

	if(m_pScriptManager != nullptr)
	{
		delete m_pScriptManager;
		m_pScriptManager = nullptr;
	}




}

/*****************************************************************
* Input(): Grabs any input made by the player
* Ins:			    none    		      
* Outs:				None
* Returns:		    bool
* Mod. Date:		11/29/2012
* Mod. Initials:	AG*
****************************************************************/
bool GamePlayState::Input()
{
	if(m_fWinTimer > 0)
	{
		return true;
	}

	if(CInput::DIKBufferedButtonCheck(DIK_F1) && m_bHelpScreenToggleOn == false)
	{
		WwiseNS::SoundEvent(ePause, ePause);
		m_bHelpScreenToggleOn = true;
		m_fHelpScreenAlpha = 1;
	}
	if(m_bHelpScreenToggleOn && (CInput::BufferedButtonPress(eEsc) || CInput::BufferedButtonPress(eStart) || CInput::BufferedButtonPress(eLeftMouseClick)))
	{
		WwiseNS::SoundEvent(eResume, eResume);
		m_bHelpScreenToggleOn = false;
	}
	if(m_fHelpScreenAlpha > 0)
		return true;

	if(CInput::BufferedButtonPress(eEsc, true))
	{
		m_pGame->PushState(new CPauseState(m_pGame));
		return true;
	}
#if _DEBUG

// 	if(GetAsyncKeyState(VK_OEM_5))
// 	{
// 		static float s_fSlowMPHPrint = 0.5f;
// 		s_fSlowMPHPrint += m_pGame->GetTimer()->GetDeltaTime();
// 		if(s_fSlowMPHPrint >= 0.5f)
// 		{
// 			if(m_pPlayer->GetMoveFaster())
// 				cout << "MPH:" << m_pPlayer->GetVelocityModifier() * 2 << endl;
// 			else
// 				cout << "MPH:" << m_pPlayer->GetVelocityModifier() << endl;
// 
// 			s_fSlowMPHPrint = 0;
// 		}
// 	}
// 	if(GetAsyncKeyState('I') & 0x1)
// 	{
// 		std::cout << "Player Pos:  X- " << m_pPlayer->GetPosition().x << "     Y- " << m_pPlayer->GetPosition().y << "     Z- " << m_pPlayer->GetPosition().z << endl;
// 	}
// 	if (GetAsyncKeyState('R') & 1)
// 	{
// 		s_AIHelper->ReadFile("AIHelper.txt");
// 	}
	//#endif

	
	//#if _DEBUG
	//TODO Remove this cheat
	

// 	if(GetAsyncKeyState('C') & 0x1)
// 	{
// 		m_pGame->PushState(new CWinState(m_pGame));
// 		m_pObjectManager->SetRemoveMiscItems(true);
// 		m_pCamera->SetInWinScreen(true);
// 		m_esWarpDust.SetPlay(true);
// 		return true;
// 	}


// 	if(GetAsyncKeyState('P') & 0x1)
// 	{
// 		EffectInformation::resetEffects();
// 		EffectInformation::LoadAllEffects("AllEffects.txt", m_pRenderer->GetAssetManager(), m_pObjectManager);
// 	}
#endif
// 
// 	if(GetAsyncKeyState('X') & 0x1)
// 	{
// 		m_pObjectManager->ToggleToShowAmbientDogFights();
// 	}
	// 	static float s_fMissileTimer = 0.0f;
	// 	static bool s_bCanfire = false;
	// 	s_fMissileTimer += m_pGame->GetTimer()->GetDeltaTime();

	static float s_fModeTimer = 2.0f;
	static bool s_bCanChange = false;
	s_fModeTimer += m_pGame->GetTimer()->GetDeltaTime();

	// 	if (s_fMissileTimer >= 1.250f)
	// 	{
	// 		s_fMissileTimer = 0.0f;
	// 		s_bCanfire = true;
	// 	}
	if (s_fModeTimer >= 2.0f)
	{
		s_fModeTimer = 0.0f;
		s_bCanChange = true;
	}

	if (m_bIsAlive == true && m_pPlayerBehavior != m_pInactivePlayer)
	{
		//TODO::Jayson - Moving reticle 
		D3DXVECTOR3 _vLocalZ(m_pGame->GetCamera()->GetViewMatrix()->_31, m_pGame->GetCamera()->GetViewMatrix()->_32, m_pGame->GetCamera()->GetViewMatrix()->_33);
		D3DXVec3Normalize(&_vLocalZ, &_vLocalZ);

		D3DXVECTOR3 _vLocalY(m_pGame->GetCamera()->GetViewMatrix()->_21, m_pGame->GetCamera()->GetViewMatrix()->_22, m_pGame->GetCamera()->GetViewMatrix()->_23);
		D3DXVec3Normalize(&_vLocalY, &_vLocalY);

		D3DXVECTOR3 _vLocalX(m_pGame->GetCamera()->GetViewMatrix()->_11, m_pGame->GetCamera()->GetViewMatrix()->_12, m_pGame->GetCamera()->GetViewMatrix()->_13);
		D3DXVec3Normalize(&_vLocalX, &_vLocalX);

		//TODO SW Remove to player state
		if(CInput::TimeBufferedButtonHeld(eLeftMouseClick, 0.095f) && m_pCamera->GetWaitForGame() == false /*|| CInput::TimeBufferedButtonHeld(eRightMouseClick, 0.15f)*/ )
		{
			if(m_pPlayer->GetType() == eRED_PLAYER)
			{
				CLaser* pTempLaser = (CLaser*)(m_pObjectManager->ReturnArmamentToMasterList(eLASER));

				if(pTempLaser != NULL)
				{
					pTempLaser->SetOwner(m_pPlayer);

					CEntity d3dTmpE; //tmpE.SetMatrix(m_pGame->GetCamera()->GetViewMatrix());
					D3DXMATRIX LaserSpawnMat(*m_pPlayer->GetMatrix());

					//This is here to make the red enemy evade when the player looks
					//at him because thats what the enemyLockOnTarget function does when it selects a target
					D3DXVECTOR3 d3dTmpN;	
					//D3DXVec3Normalize(&d3dTmpN, &CPhysics::GetMat4Vec(eZAXIS, m_pGame->GetCamera()->GetViewMatrix()));
					D3DXVec3Normalize(&d3dTmpN, &( CPhysics::GetMat4Vec(ePOS, &m_TargetMat) - CPhysics::GetMat4Vec(ePOS, m_pPlayer->GetMatrix())));
					CEntity* CMissileTarget = m_pObjectManager->EnemyLockOnTarget(CPhysics::GetMat4Vec(ePOS, m_pGame->GetCamera()->GetViewMatrix()), d3dTmpN, 2.5f,m_pGame->GetTimer()->GetDeltaTime() );

					d3dTmpE.SetMatrix(&m_TargetMat);

					//////////////////////////////////////////////////////////////////////////////////////////////////////////////
					// Calculation of the laser spawn
					D3DXVECTOR3 d3dPlayerZ(m_pPlayer->GetMatrix()->_31, m_pPlayer->GetMatrix()->_32, m_pPlayer->GetMatrix()->_33);
					D3DXVec3Normalize(&d3dPlayerZ, &d3dPlayerZ);

					D3DXVECTOR3 d3dPlayerY(m_pPlayer->GetMatrix()->_21, m_pPlayer->GetMatrix()->_22, m_pPlayer->GetMatrix()->_23);
					D3DXVec3Normalize(&d3dPlayerY, &d3dPlayerY);

					D3DXVECTOR3 d3dPlayerX(m_pPlayer->GetMatrix()->_11, m_pPlayer->GetMatrix()->_12, m_pPlayer->GetMatrix()->_13);
					D3DXVec3Normalize(&d3dPlayerX, &d3dPlayerX);

					if(m_bLaserSide == true)
					{
						// Use Negative on PlayerX to move laserspawn to the left, Positive to the right
						LaserSpawnMat._41 += (d3dPlayerX.x * -7.25f) + (d3dPlayerY.x * 1.125f) + (d3dPlayerZ.x * 2.5f);
						LaserSpawnMat._42 += (d3dPlayerX.y * -7.25f) + (d3dPlayerY.y * 1.125f) + (d3dPlayerZ.y * 2.5f);
						LaserSpawnMat._43 += (d3dPlayerX.z * -7.25f) + (d3dPlayerY.z * 1.125f) + (d3dPlayerZ.z * 2.5f);

						pTempLaser->SetMatrix(&LaserSpawnMat);

						m_bLaserSide = !m_bLaserSide;
					}
					else
					{					
						// Use Negative on PlayerX to move laserspawn to the left, Positive to the right
						LaserSpawnMat._41 += (d3dPlayerX.x * 7.25f) + (d3dPlayerY.x * 1.125f) + (d3dPlayerZ.x * 2.5f);
						LaserSpawnMat._42 += (d3dPlayerX.y * 7.25f) + (d3dPlayerY.y * 1.125f) + (d3dPlayerZ.y * 2.5f);
						LaserSpawnMat._43 += (d3dPlayerX.z * 7.25f) + (d3dPlayerY.z * 1.125f) + (d3dPlayerZ.z * 2.5f);

						pTempLaser->SetMatrix(&LaserSpawnMat);

						m_bLaserSide = !m_bLaserSide;
					}
					if(CMissileTarget)
					{
						if(D3DXVec3Dot(&CPhysics::GetMat4Vec(eZAXIS, m_pCamera->GetViewMatrix()), &(CMissileTarget->GetPosition() - m_pPlayer->GetPosition())) > 0.7 )
						{
							CPhysics::LookAt(pTempLaser, CMissileTarget);
							pTempLaser->SetTarget(CMissileTarget);
						}
						else
						{
							CPhysics::LookAt(pTempLaser, &d3dTmpE);
						}
					}	
					else
					{
						CPhysics::LookAt(pTempLaser, &d3dTmpE);
					}

					WwiseNS::SoundEvent(ePlayerLaserFire, ePlay);
					m_pGame->GetCurrentStats()->LaserFired();

					pTempLaser->SetActive(true);
					pTempLaser->SetLifetime(LASER_LIFETIME);
					pTempLaser->SetHealth(10);
					pTempLaser->SetVelocityModifier(5.0f);
					pTempLaser->SetDamage(PLAYER_LASER_DAMAGE);
					pTempLaser->SetIsScenery(false);
					pTempLaser->SetShouldBounce(false);

					pTempLaser->SparkEffect();
					pTempLaser->MuzzleEffect();

					pTempLaser->Scale(1.0f, 1.0f, 6.0f);


					//pTempLaser->SetOwner(m_pPlayer);
					//
					//CEntity d3dTmpE; //tmpE.SetMatrix(m_pGame->GetCamera()->GetViewMatrix());
					//D3DXMATRIX LaserSpawnMat(*m_pPlayer->GetMatrix());
					//
					////This is here to make the red enemy evade when the player looks
					////at him because thats what the enemyLockOnTarget function does when it selects a target
					//
					//D3DXMATRIX d3dIdentity;
					//D3DVIEWPORT9 d3dViewPort;
					//D3DXMATRIX d3dCameraInverse;
					//
					//D3DXMatrixIdentity(&d3dIdentity);		
					//m_pGame->GetRenderer()->GetDevice()->GetViewport(&d3dViewPort);
					//D3DXMatrixInverse(&d3dCameraInverse, 0, m_pCamera->GetViewMatrix());
					//
					//POINT tMousePosition = CInput::GetMouseScreenPos();
					//
					//if(CInput::GetInvertY() == true)
					//{
					//	tMousePosition.y = int(-tMousePosition.y + WINDOW_HEIGHT); 
					//}
					//if(CInput::GetInvertX() == true)
					//{
					//	tMousePosition.x = int(-tMousePosition.x + WINDOW_WIDTH);
					//}
					//
					//D3DXVECTOR3 d3dTmpN, d3dBegin, d3dEnd = D3DXVECTOR3(m_TargetMat._41, m_TargetMat._42, m_TargetMat._43);	
					//D3DXVec3Unproject(&d3dBegin, &D3DXVECTOR3((float)tMousePosition.x, (float)tMousePosition.y, 0.0f), &d3dViewPort, m_pCamera->GetProjectionMatrix(), &d3dCameraInverse, &d3dIdentity );
					////D3DXVec3Normalize(&d3dTmpN, &CPhysics::GetMat4Vec(eZAXIS, m_pGame->GetCamera()->GetViewMatrix()));
					//D3DXVec3Normalize(&d3dTmpN, &( CPhysics::GetMat4Vec(ePOS, &m_TargetMat) - d3dBegin));
					//CEntity* CMissileTarget = 0;//m_pObjectManager->GetReticuleTarget(d3dBegin, d3dEnd);
					//
					//D3DXVECTOR3 d3dLaserPos = m_pObjectManager->LaserCast(d3dBegin, d3dTmpN, true);
					//
					//d3dTmpE.SetMatrix(&m_TargetMat);
					//
					////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//// Calculation of the laser spawn
					//D3DXVECTOR3 d3dPlayerZ(m_pPlayer->GetMatrix()->_31, m_pPlayer->GetMatrix()->_32, m_pPlayer->GetMatrix()->_33);
					//D3DXVec3Normalize(&d3dPlayerZ, &d3dPlayerZ);
					//
					//D3DXVECTOR3 d3dPlayerY(m_pPlayer->GetMatrix()->_21, m_pPlayer->GetMatrix()->_22, m_pPlayer->GetMatrix()->_23);
					//D3DXVec3Normalize(&d3dPlayerY, &d3dPlayerY);
					//
					//D3DXVECTOR3 d3dPlayerX(m_pPlayer->GetMatrix()->_11, m_pPlayer->GetMatrix()->_12, m_pPlayer->GetMatrix()->_13);
					//D3DXVec3Normalize(&d3dPlayerX, &d3dPlayerX);
					//
					//if(m_bLaserSide == true)
					//{
					//	// Use Negative on PlayerX to move laserspawn to the left, Positive to the right
					//	LaserSpawnMat._41 += (d3dPlayerX.x * -7.25f) + (d3dPlayerY.x * 1.125f) + (d3dPlayerZ.x * 2.5f);
					//	LaserSpawnMat._42 += (d3dPlayerX.y * -7.25f) + (d3dPlayerY.y * 1.125f) + (d3dPlayerZ.y * 2.5f);
					//	LaserSpawnMat._43 += (d3dPlayerX.z * -7.25f) + (d3dPlayerY.z * 1.125f) + (d3dPlayerZ.z * 2.5f);
					//
					//	pTempLaser->SetMatrix(&LaserSpawnMat);
					//
					//	m_bLaserSide = !m_bLaserSide;
					//}
					//else
					//{					
					//	// Use Negative on PlayerX to move laserspawn to the left, Positive to the right
					//	LaserSpawnMat._41 += (d3dPlayerX.x * 7.25f) + (d3dPlayerY.x * 1.125f) + (d3dPlayerZ.x * 2.5f);
					//	LaserSpawnMat._42 += (d3dPlayerX.y * 7.25f) + (d3dPlayerY.y * 1.125f) + (d3dPlayerZ.y * 2.5f);
					//	LaserSpawnMat._43 += (d3dPlayerX.z * 7.25f) + (d3dPlayerY.z * 1.125f) + (d3dPlayerZ.z * 2.5f);
					//
					//	pTempLaser->SetMatrix(&LaserSpawnMat);
					//
					//	m_bLaserSide = !m_bLaserSide;
					//}
					//
					//
					//
					//
					//
					//
					//// 					if(CMissileTarget)
					//// 					{
					//// 						if(D3DXVec3Dot(&CPhysics::GetMat4Vec(eZAXIS, m_pCamera->GetViewMatrix()), &(CMissileTarget->GetPosition() - m_pPlayer->GetPosition()))
					//// 			> 0.7 )
					//// 						{
					//// 							CPhysics::LookAt(pTempLaser, CMissileTarget);
					//// 							pTempLaser->SetTarget(CMissileTarget);
					//// 						}
					//// 						else
					//// 						{
					//// 							CPhysics::LookAt(pTempLaser, &d3dTmpE);
					//// 						}
					//// 					}	
					//// 					else
					//// 					{
					//// 						CPhysics::LookAt(pTempLaser, &d3dTmpE);
					//// 					}D3DXMATRIX d3dPH = *pTempLaser->GetOwner()->GetMatrix();
					//D3DXMATRIX d3dPH = *pTempLaser->GetOwner()->GetMatrix();
					//pTempLaser->GetOwner()->SetMatrix(pTempLaser->GetMatrix());
					//pTempLaser->MuzzleEffect();
					//
					//if(d3dLaserPos == D3DXVECTOR3(FLT_MAX, -FLT_MAX, FLT_MAX))
					//{
					////	pTempLaser->SetPosition(CPhysics::GetMat4Vec(ePOS, &m_TargetMat));
					//}
					//else
					//{
					//	pTempLaser->SetPosition(d3dLaserPos);
					//}
					//
					//
					//WwiseNS::SoundEvent(ePlayerLaserFire, ePlay);
					//
					//pTempLaser->SetActive(true);
					//pTempLaser->SetLifetime(LASER_LIFETIME);
					//pTempLaser->SetHealth(10);
					//pTempLaser->SetVelocityModifier(5.0f);
					//pTempLaser->SetDamage(PLAYER_LASER_DAMAGE);
					//
					//pTempLaser->SparkEffect();
					//pTempLaser->GetOwner()->SetMatrix(&d3dPH);
				}
				/*
				if(m_pPlayer->GetType() == eRED_PLAYER)
				{
				CLaser* pTempLaser = (CLaser*)(m_pObjectManager->ReturnArmamentToMasterList(eLASER));

				if(pTempLaser != NULL)
				{

				pTempLaser->SetOwner(m_pPlayer);

				CEntity d3dTmpE; //tmpE.SetMatrix(m_pGame->GetCamera()->GetViewMatrix());
				D3DXMATRIX lookAtMat(*m_pGame->GetCamera()->GetViewMatrix());
				D3DXMATRIX LaserSpawnMat(*m_pPlayer->GetMatrix());

				// Calculation of the Reticle
				D3DXVECTOR3 _vLocalZ(m_pGame->GetCamera()->GetViewMatrix()->_31, m_pGame->GetCamera()->GetViewMatrix()->_32, m_pGame->GetCamera()->GetViewMatrix()->_33);
				D3DXVec3Normalize(&_vLocalZ, &_vLocalZ);

				D3DXVECTOR3 _vLocalY(m_pGame->GetCamera()->GetViewMatrix()->_21, m_pGame->GetCamera()->GetViewMatrix()->_22, m_pGame->GetCamera()->GetViewMatrix()->_23);
				D3DXVec3Normalize(&_vLocalY, &_vLocalY);

				D3DXVECTOR3 _vLocalX(m_pGame->GetCamera()->GetViewMatrix()->_11, m_pGame->GetCamera()->GetViewMatrix()->_12, m_pGame->GetCamera()->GetViewMatrix()->_13);
				D3DXVec3Normalize(&_vLocalX, &_vLocalX);

				lookAtMat._41 += (_vLocalY.x * 7.0f) + (_vLocalZ.x * 250);
				lookAtMat._42 += (_vLocalY.y * 7.0f) + (_vLocalZ.y * 250);
				lookAtMat._43 += (_vLocalY.z * 7.0f) + (_vLocalZ.z * 250);

				d3dTmpE.SetMatrix(&lookAtMat);

				//////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// Calculation of the laser spawn
				D3DXVECTOR3 d3dPlayerZ(m_pPlayer->GetMatrix()->_31, m_pPlayer->GetMatrix()->_32, m_pPlayer->GetMatrix()->_33);
				D3DXVec3Normalize(&d3dPlayerZ, &d3dPlayerZ);

				D3DXVECTOR3 d3dPlayerY(m_pPlayer->GetMatrix()->_21, m_pPlayer->GetMatrix()->_22, m_pPlayer->GetMatrix()->_23);
				D3DXVec3Normalize(&d3dPlayerY, &d3dPlayerY);

				D3DXVECTOR3 d3dPlayerX(m_pPlayer->GetMatrix()->_11, m_pPlayer->GetMatrix()->_12, m_pPlayer->GetMatrix()->_13);
				D3DXVec3Normalize(&d3dPlayerX, &d3dPlayerX);

				if(m_bLaserSide == true)
				{
				// Use Negative on PlayerX to move laserspawn to the left, Positive to the right
				LaserSpawnMat._41 += (d3dPlayerX.x * -5.0f) + (d3dPlayerY.x * 1.125f) + (d3dPlayerZ.x * 2);
				LaserSpawnMat._42 += (d3dPlayerX.y * -5.0f) + (d3dPlayerY.y * 1.125f) + (d3dPlayerZ.y * 2);
				LaserSpawnMat._43 += (d3dPlayerX.z * -5.0f) + (d3dPlayerY.z * 1.125f) + (d3dPlayerZ.z * 2);

				pTempLaser->SetMatrix(&LaserSpawnMat);

				m_bLaserSide = !m_bLaserSide;
				}
				else
				{					
				// Use Negative on PlayerX to move laserspawn to the left, Positive to the right
				LaserSpawnMat._41 += (d3dPlayerX.x * 5.0f) + (d3dPlayerY.x * 1.125f) + (d3dPlayerZ.x * 2);
				LaserSpawnMat._42 += (d3dPlayerX.y * 5.0f) + (d3dPlayerY.y * 1.125f) + (d3dPlayerZ.y * 2);
				LaserSpawnMat._43 += (d3dPlayerX.z * 5.0f) + (d3dPlayerY.z * 1.125f) + (d3dPlayerZ.z * 2);

				pTempLaser->SetMatrix(&LaserSpawnMat);

				m_bLaserSide = !m_bLaserSide;
				}
				CPhysics::LookAt(pTempLaser, &d3dTmpE);

				WwiseNS::SoundEvent(ePlayerLaserFire, ePlay);

				pTempLaser->SetActive(true);
				pTempLaser->SetLifetime(.75f);
				pTempLaser->SetHealth(10);
				pTempLaser->SetVelocityModifier(5.0f);
				pTempLaser->SetDamage(PLAYER_LASER_DAMAGE);
				}
				}	*/
			}
			else if(m_pPlayer->GetType() == eBLUE_PLAYER)
			{
				if(m_fMissileReloadTimer>= 0.8f && bIsLooping == false)
				{	
					m_nMissilesToFire = 3;
					m_fMissileDelay = 0.5f;
				}
			}
			else if(m_pPlayer->GetType() == eYELLOW_PLAYER)
			{
				float playerShields = m_pPlayer->GetShields() / 100.0f;
				if (m_pPlayer->GetShields() > 1)
				{
					vector<CEntity*> currEnemies = m_pScriptManager->GetCurrentEnemies();
					vector<CEntity*> currYellowEnemies = m_pScriptManager->GetCurrentYellowEnemies();
					for (unsigned int eachEnemy = 0; eachEnemy < currEnemies.size(); ++eachEnemy)
					{
						//use shield amount to decide how far and how much
						float distanceToTEnemy = D3DXVec3Length(&(((CEnemy*)currEnemies[eachEnemy])->GetPosition() - m_pPlayer->GetPosition()));
						if (distanceToTEnemy * playerShields < 150)
						{
							((CEnemy*)currEnemies[eachEnemy])->SetIsStunned(true, playerShields);
						}
					}
					for (unsigned int eachEnemy = 0; eachEnemy < currYellowEnemies.size(); ++eachEnemy)
					{
						//use shield amount to decide how far and how much
						float distanceToTEnemy = D3DXVec3Length(&(((CEnemy*)currYellowEnemies[eachEnemy])->GetPosition() - m_pPlayer->GetPosition()));
						if (distanceToTEnemy * playerShields < 150)
						{
							((CEnemy*)currYellowEnemies[eachEnemy])->SetIsStunned(true, playerShields);
						}
					}
					m_pPlayer->SetShields(0);
				}
				m_pPlayer->SetCamera(m_pGame->GetCamera());
			}
		} 
		if (s_bCanChange)
		{

			if(/*(CInput::BufferedButtonPress(eNum1, true) ||*/ CInput::BufferedButtonPress(eRightMouseClick) && m_pPlayer->GetType() != eRED_PLAYER && m_pCamera->GetWaitForGame() == false /*&& m_pPlayer->GetHasDodged() == false*/)
			{
				m_pPlayer->ChangeModeEffect(false);
				m_pPlayerBehavior = m_pRedPlayer;
				m_pPlayer->SetCamera(m_pGame->GetCamera());
				WwiseNS::SoundEvent(eModeChange, ePlay);
				s_bCanChange = false;
			}

			if(/*(CInput::BufferedButtonPress(eNum2, true) || */CInput::BufferedButtonPress(eRightMouseClick) && m_pPlayer->GetType() != eBLUE_PLAYER && m_pCamera->GetWaitForGame() == false /*&& m_pPlayer->GetHasDodged() == false*/)
			{
				m_pPlayer->ChangeModeEffect();
				m_pPlayerBehavior = m_pBluePlayer;
				m_pPlayer->SetCamera(m_pGame->GetCamera());
				WwiseNS::SoundEvent(eModeChange, ePlay);
				s_bCanChange = false;			
			}

			//if(CInput::BufferedButtonPress(eTurnAround, true) && CInput::GetHasDodge() == false && m_pCamera->GetWaitForGame() == false && m_bTurnAround == false)
			//{
			//	CInput::SetHasDodged(true);
			//
			//	D3DXVECTOR3 negZaxis = -CPhysics::GetMat4Vec(eZAXIS, m_pPlayer->GetMatrix());
			//	negZaxis *= 500;
			//
			//	newZaxisDirection = negZaxis + m_pPlayer->GetPosition();
			//	//newZaxisDirection.x += 10;
			//
			//
			//
			//}

			//
			//if(m_pPlayerBehavior == m_pDodgePlayer && CInput::GetHasDodge() == false)
			//{
			//	m_pPlayerBehavior = m_pPrevPlayerBehavior;
			//}

			/*if(CInput::BufferedButtonPress(eNum3, true))
			{
			m_pPlayerBehavior = m_pYellowPlayer;
			m_pPlayer->SetCamera(m_pGame->GetCamera());
			WwiseNS::SoundEvent(eModeChange, ePlay);
			s_bCanChange = false;
			}*/
		}
	}


	return true;
}

/*****************************************************************
* Update(): Updates the game state based on the change in time
* Ins:			    fElapsedTime    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void GamePlayState::Update(float _fElapsedTime)
{
	float fTime = _fElapsedTime;

	if(m_bNeedToProceed)
	{
		m_fProceedTimer += _fElapsedTime;

		if(m_fProceedTimer > 2.0f)
		{
			m_fProceedTimer = 0.0f;
			m_bNeedToProceed = false;
			WwiseNS::SoundEvent(eProceedToWaypoint, ePlay);
		}
	}

	if( m_pCamera->GetInWinScreen() )
	{
		m_pCamera->Update(_fElapsedTime);
		m_pObjectManager->UpdateObjects(_fElapsedTime);
		m_pObjectManager->SetInCinematic(true);
		return;
	}

	if(m_pCinematicCamera->GetIsIntroCinematic() == false)
	{
		m_pCamera->SetInMainMenu(false);
	}
// 
// #ifdef _DEBUG
// 	if( GetAsyncKeyState(VK_CONTROL) & 1 )
// 	{
// 		m_pObjectManager->SetRemoveMiscItems(true);
// 	}
// #endif

	if(m_bHelpScreenToggleOn == true)
	{
		if(m_fHelpScreenAlpha < 255)
			m_fHelpScreenAlpha += 255 * _fElapsedTime;
		return;
	}
	else if(m_bHelpScreenToggleOn == false && m_fHelpScreenAlpha > 0)
	{
		m_fHelpScreenAlpha -= 255 * _fElapsedTime;
		if(m_fHelpScreenAlpha < 0)
			m_fHelpScreenAlpha = 0;
		return;
	}

	if(m_bHasDodgedMissile == true || m_bSlowDodgeEnemy == true)
	{
		_fElapsedTime *= 0.25f;
	}

	int nNumMissiles = m_pObjectManager->GetMissileList()->size();

	static bool doOnceLockOn = false;

	if(nNumMissiles > 0)
	{
		CEntity* MissileList = (*m_pObjectManager->GetMissileList())[0];

		if(MissileList->GetTarget() != NULL)
		{
			int nPlayerType = MissileList->GetTarget()->GetType();

			if(nPlayerType == eRED_PLAYER || nPlayerType == eBLUE_PLAYER)
			{
				m_bLockedOn = true;

				if(doOnceLockOn == false)
				{
					WwiseNS::SoundEvent(eLockOnWarning, ePlay);

					doOnceLockOn = true;
				}

				m_fMissileDistance = D3DXVec3Length(&(MissileList->GetPosition() - m_pPlayer->GetPosition()));

				if(m_fMissileDistance >= 75)
				{
					m_bWithinRange = false;
				}
				else if(m_fMissileDistance < 75 && m_bHasDodgedMissile == false)
				{
					if(m_pPlayer->GetHasDodged() == DODGE_LEFT || m_pPlayer->GetHasDodged() == DODGE_RIGHT)
					{
						m_bHasDodgedMissile = true;
						m_pGame->GetCurrentStats()->SlowDodged();
						MissileList->SetTarget(nullptr);
						MissileList->SetIsScenery(true);
						m_pPlayer->SetIsScenery(true);
						doOnceLockOn = false;
					}
					else
					{
						m_bWithinRange = true;							
					}						
				}
			}
		}
	}
	else
	{
		m_bLockedOn = false;
		doOnceLockOn = false;
	}

	//if(m_pPlayer->GetHasDodged() == 1 || m_pPlayer->GetHasDodged() == 2)
	//{
	//	std::vector<CEntity*> vEnemyList =  *m_pObjectManager->GetIndicatorList();
	//	std::vector<CEntity*>::iterator iIndicator = vEnemyList.begin();
	//	CEntity* pIter = nullptr;
	//
	//	for(;iIndicator != vEnemyList.end(); ++iIndicator)
	//	{
	//		pIter = (*iIndicator);
	//
	//		if(pIter->GetType() == eWAYPOINT)
	//		{
	//			continue;
	//		}
	//
	//		if(pIter->GetDistanceFromPlayer() <= 100)
	//		{
	//			D3DXVECTOR3 d3dToPlayerVector = m_pPlayer->GetPosition() - pIter->GetPosition();
	//			D3DXVECTOR3 d3dFromPlayerVector = pIter->GetPosition() - m_pPlayer->GetPosition();
	//			D3DXVec3Normalize(&d3dToPlayerVector, &d3dToPlayerVector);
	//			D3DXVec3Normalize(&d3dFromPlayerVector, &d3dFromPlayerVector);
	//
	//			float fToPlayerZDotResult = D3DXVec3Dot(&d3dToPlayerVector, &CPhysics::GetMat4Vec(eZAXIS, pIter->GetMatrix()));
	//			float fFromPlayerZDotResult = D3DXVec3Dot(&d3dFromPlayerVector, &CPhysics::GetMat4Vec(eZAXIS, m_pPlayer->GetMatrix()));
	//
	//			if(fToPlayerZDotResult > 0 && fFromPlayerZDotResult > 0)
	//			{
	//				m_pPlayer->SetIsScenery(true);
	//				m_bSlowDodgeEnemy = true;
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	m_bSlowDodgeEnemy = false;
	//}

	if(m_pScriptManager->GetCurrentEnemies().size() == 0.0f && !m_bCompleted)
	{
		m_fCompletedBucket += _fElapsedTime;
	}

	// Send the player position to wwise for 3D sounds
	WwiseNS::SetPlayerPos(*m_pPlayer->GetMatrix());
	if((bossFight && m_pScriptManager->GetTheBoss()->GetIsDisabled() == true && m_bInCinematic))
		WwiseNS::SetListenerPos(*m_pScriptManager->GetTheBoss()->GetMatrix());
	else
		WwiseNS::SetListenerPos(*m_pCamera->GetCurrentMatrix());

	m_pSceneryManager->Update(_fElapsedTime);


	if( m_bPlayCinematic )
	{
		m_fWaitForCinematicBucket += _fElapsedTime;
	}

	if(m_fWinTimer == 0)
	{
		m_pCamera->Update(_fElapsedTime);
	}

	if( m_bInCinematic )
	{		
		if( m_pCinematicCamera->GetHasFinishedCinematic() )
		{
			if( m_pScriptManager->GetTheBoss() )
			{
				if( !m_pScriptManager->GetTheBoss()->GetIsDisabled() )
				{
					m_pPlayerBehavior = m_pPrevPlayerBehavior;
					m_bInCinematic = false;
					WwiseNS::SetVolumeRTPC( AK::GAME_PARAMETERS::MX_VOLUME, m_pGame->GetMusicVolume());
					m_pObjectManager->SetInCinematic(false);
					m_pPlayer->SetInactive(false);
				}
			}
			else
			{
				m_pPlayerBehavior = m_pPrevPlayerBehavior;
				m_bInCinematic = false;
				WwiseNS::SetVolumeRTPC( AK::GAME_PARAMETERS::MX_VOLUME, m_pGame->GetMusicVolume());
				m_pObjectManager->SetInCinematic(false);
				m_pPlayer->SetInactive(false);
			}
		}
		else if( m_pScriptManager->GetTheBoss() )
		{
			if( m_pScriptManager->GetTheBoss()->GetPhase() != eCINEMATIC_PHASE )
			{
				if( !m_pScriptManager->GetTheBoss()->GetIsDisabled() )
				{
					_fElapsedTime = 0.0f;
				}
			}
			else 
			{
				_fElapsedTime *= 0.001f;
			}
		}
		else
		{
			_fElapsedTime = 0.0f;
		}
	}

	if( bossFight && m_pScriptManager->GetTheBoss()->GetDeathWaitTimer() > 11.0f )
	{
		m_fFadeToWhiteAlpha -= fTime*100.0f;
		if( m_fFadeToWhiteAlpha < 0 )
		{
			m_fFadeToWhiteAlpha = 0;
		}
	}
	else if( bossFight && m_pScriptManager->GetTheBoss()->GetDeathWaitTimer() > 7.5f )
	{
		m_fFadeToWhiteAlpha += fTime*200.0f;
		if( m_fFadeToWhiteAlpha >= 255 )
		{
			m_fFadeToWhiteAlpha = 254;
		}
	}

	m_fBehindSound += _fElapsedTime;

	if(m_pCamera->GetWaitForGame() == true)
	{
		static float s_fGameWaitTimer = 0.0f;

		if(s_fGameWaitTimer > 1.5f)
		{
			m_pCamera->SetWaitForGame(false);
			s_fGameWaitTimer = 0.0f;
		}
		s_fGameWaitTimer += _fElapsedTime;
	}

	startTimer += _fElapsedTime;
	if (startTimer >= 10.0f )
	{
		gameJustStarted = false;
		startTimer = 0.0f;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////
	CWaypoint * pWaypoint = m_pScriptManager->GetCurrentWaypoint();
	D3DXVECTOR3 playerPos = m_pPlayer->GetPosition();
	D3DXVECTOR3 waypointPos = pWaypoint->GetPosition();
	m_fDistanceFromWaypoint = sqrt((( waypointPos.x - playerPos.x) * (waypointPos.x - playerPos.x)) + 
		((waypointPos.y - playerPos.y) * (waypointPos.y - playerPos.y)) + 
		((waypointPos.z - playerPos.z) * (waypointPos.z - playerPos.z)));


	m_fMissileDelay += _fElapsedTime;
	//for (int nMoreMissiles = 0; nMoreMissiles < 3; nMoreMissiles++)
	if(m_nMissilesToFire > 0 && m_fMissileDelay >= 0.18f)
	{
		m_fMissileDelay = 0.0f;
		m_nMissilesToFire--;

		bIsLooping = true;

		CMissile* cTempMissile = (CMissile*)(m_pObjectManager->ReturnArmamentToMasterList(eMISSILE));

		if(cTempMissile != NULL)
		{
			cTempMissile->SetOwner(m_pPlayer);
			if(m_fLockOnTimer > m_fTimeToLockOn)
			{
				cTempMissile->SetTarget(m_pPlayer->GetTarget());
			}
			else
			{
				cTempMissile->SetTarget(0);
			}

			D3DXMATRIX	d3dMissileMat(* m_pPlayer->GetMatrix());

			if (m_nMissilesToFire == 2)
			{
				//	d3dMissileMat._41 += 6;
				cTempMissile->SetVelocity(D3DXVECTOR3(-1, -45, -7));
			}
			else if (m_nMissilesToFire == 1)
			{
				//d3dMissileMat._42 -= 2;
				cTempMissile->SetVelocity(D3DXVECTOR3(0, -45, -7));
			}
			else
			{
				//d3dMissileMat._41 -= 1;
				cTempMissile->SetVelocity(D3DXVECTOR3(1, -45, -7));
			}

			WwiseNS::SoundEvent(ePlayerMissileFire, ePlay, false, cTempMissile);
			WwiseNS::SoundEvent(eMissileTravel, ePlay, false, cTempMissile);
			m_pGame->GetCurrentStats()->MissileFired();

			cTempMissile->SetMatrix(&d3dMissileMat);
			cTempMissile->MissileFlash();
			cTempMissile->SetActive(true);
			cTempMissile->SetLifetime(6);
			cTempMissile->SetDoOnce(true);
			cTempMissile->SetHealth(10);
			cTempMissile->SetIsVisable(true);
			cTempMissile->SetIsScenery(false);
			cTempMissile->SetVelocityModifier(m_pPlayer->GetVelocityModifier() + s_AIHelper->GetmissileVelocityModifier());
			cTempMissile->SetDamage(s_AIHelper->GetmissileDamage());
			for (unsigned int i = 0; i < cTempMissile->GetMovementBehaviorStack().size(); i++)
			{
				cTempMissile->PopBehavior();
			}
			if (m_nMissilesToFire == 0)
			{
				bIsLooping = false;
				m_fMissileReloadTimer = 0.0f;
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////

	if(m_pScriptManager->GetCurrentEnemies().size() > 0 && m_bInCombat == false)
	{
		m_bInCombat = true;
		WwiseNS::SoundEvent(eBGM_BattleMusic, ePlay);
	}
	if(m_pScriptManager->GetCurrentEnemies().size() == 0 && m_bInCombat == true)
	{
		m_bInCombat = false;
		WwiseNS::SoundEvent(eBGM_LevelOneMusic, ePlay);
	}


	m_pGame->GetDialogSys()->Update(_fElapsedTime);


	if(CInput::GetHasDodge() == true)  
	{	
		static bool firstTime = true;
		if(firstTime)
		{
			WwiseNS::SoundEvent(eHyperSpace, ePlay);
			firstTime = false;
		}
		CPhysics::ReverseDirection(m_pCamera->GetViewMatrix(), &newZaxisDirection, _fElapsedTime, 0.9f);	

		static float s_fTurnAroundTime = 0.0f;

		if(s_fTurnAroundTime > 1.0f)
		{
			CInput::SetHasDodged(false);
			s_fTurnAroundTime = 0.0f;
			firstTime = true;
		}

		s_fTurnAroundTime += _fElapsedTime;
	}

	//if(CInput::GetHasDodge() == true && m_bReturnToCamera == false)  
	//{	
	//
	//	static bool tempReturn = false;
	//
	//	if(tempReturn == false)
	//	{
	//		m_pCamera->DetachCamera(true);
	//
	//		D3DXVECTOR3 d3dCamPos(m_pCamera->GetCurrentMatrix()->_41, m_pCamera->GetCurrentMatrix()->_42, m_pCamera->GetCurrentMatrix()->_43);
	//
	//		float fSquaredDistance = D3DXVec3Length(&(d3dCamPos - m_pPlayer->GetPosition()));
	//		static bool temptemp = false;
	//
	//		if(fSquaredDistance > 100.0f)
	//		{
	//			temptemp = true;
	//		}
	//
	//		if(temptemp == true)
	//		{
	//			CPhysics::DodgeReverseDirection(m_pCamera->GetViewMatrix(), &newZaxisDirection, _fElapsedTime, 0.9f);	
	//
	//			D3DXVECTOR3 d3dPlayerZVector = d3dCamPos - m_pPlayer->GetPosition();
	//			D3DXVECTOR3 d3dCameraZVector = m_pPlayer->GetPosition() - d3dCamPos;
	//			D3DXVec3Normalize(&d3dPlayerZVector, &d3dPlayerZVector);
	//			D3DXVec3Normalize(&d3dCameraZVector, &d3dCameraZVector);
	//
	//			float fPlayerZDotResult = D3DXVec3Dot(&d3dPlayerZVector, &CPhysics::GetMat4Vec(eZAXIS, m_pPlayer->GetMatrix()));
	//			float fCameraZDotResult = D3DXVec3Dot(&d3dCameraZVector, &CPhysics::GetMat4Vec(eZAXIS, m_pCamera->GetCurrentMatrix()));
	//
	//			if(fPlayerZDotResult > 0 && fCameraZDotResult > 0)
	//			{
	//				tempReturn = true;
	//				temptemp = false;
	//			}
	//		}
	//	}	
	//	else if(tempReturn == true)
	//	{
	//		D3DXMATRIX tmpCamera = *m_pCamera->GetCurrentMatrix();
	//
	//		D3DXMATRIX transMat;
	//		D3DXMatrixIdentity(&transMat);
	//
	//		D3DXVECTOR3 campos = D3DXVECTOR3(tmpCamera._41, tmpCamera._42, tmpCamera._43);
	//		D3DXVECTOR3 targetpos = D3DXVECTOR3( m_pPlayer->GetMatrix()->_41, m_pPlayer->GetMatrix()->_42, m_pPlayer->GetMatrix()->_43);
	//		D3DXVECTOR3 diff = targetpos - campos;
	//		D3DXMatrixTranslation(&transMat, 100.0f *_fElapsedTime, 0, 0);	
	//
	//		m_pCamera->SetCurrentCameraMatrix(transMat * *m_pCamera->GetCurrentMatrix());
	//		CPhysics::LookAt(m_pCamera->GetCurrentMatrix(), m_pPlayer->GetMatrix());
	//
	//		m_pCamera->SetCurrentCameraMatrix(*m_pCamera->GetCurrentMatrix());
	//
	//		static float s_fTurnAroundTime = 0.0f;
	//
	//		if(s_fTurnAroundTime > 1.0f)
	//		{
	//			m_bReturnToCamera = true;
	//			s_fTurnAroundTime = 0.0f;
	//			tempReturn = false;
	//		}
	//
	//		s_fTurnAroundTime += _fElapsedTime;
	//	}
	//}
	//
	//if(m_bReturnToCamera == true)
	//{
	//	D3DXMATRIX tmpCamera = *m_pCamera->GetCurrentMatrix();
	//	D3DXMATRIX transMat;
	//	D3DXMatrixIdentity(&transMat);
	//
	//	D3DXVECTOR3 campos = D3DXVECTOR3(tmpCamera._41, tmpCamera._42, tmpCamera._43);
	//	D3DXVECTOR3 targetpos = D3DXVECTOR3( m_pPlayer->GetMatrix()->_41, m_pPlayer->GetMatrix()->_42, m_pPlayer->GetMatrix()->_43);
	//	D3DXVECTOR3 diff = targetpos - campos;
	//	D3DXMatrixTranslation(&transMat, 0, 0, 100.0f *_fElapsedTime);	
	//
	//	m_pCamera->SetCurrentCameraMatrix(transMat * *m_pCamera->GetCurrentMatrix());
	//	CPhysics::LookAt(m_pCamera->GetCurrentMatrix(), m_pPlayer->GetMatrix());
	//
	//	D3DXVECTOR3 d3dCamPos(m_pCamera->GetCurrentMatrix()->_41, m_pCamera->GetCurrentMatrix()->_42, m_pCamera->GetCurrentMatrix()->_43);
	//
	//	float fSquaredDistance = D3DXVec3Length(&(d3dCamPos - m_pPlayer->GetPosition()));
	//
	//	if(fSquaredDistance > 15.0f)
	//	{
	//		m_pCamera->SetCurrentCameraMatrix(*m_pCamera->GetCurrentMatrix());
	//	}
	//	else
	//	{
	//		m_pCamera->DetachCamera(false);
	//		m_bReturnToCamera = false;
	//		CInput::SetHasDodged(false);
	//	}
	//}

	if(m_bTurnAround == true && CInput::GetHasDodge() == false)
	{
		D3DXVECTOR3 d3dWaypointVector(m_pScriptManager->GetCurrentWaypoint()->GetMatrix()->_41, m_pScriptManager->GetCurrentWaypoint()->GetMatrix()->_42, m_pScriptManager->GetCurrentWaypoint()->GetMatrix()->_43);

		D3DXMATRIX d3dNewViewMat = *m_pCamera->GetViewMatrix();

		CPhysics::DodgeReverseDirection(&d3dNewViewMat, &d3dWaypointVector, _fElapsedTime, 0.1f);

		m_pCamera->SetViewMatrix(&d3dNewViewMat);

		//m_pCamera->DetachCamera(true);
		//
		//D3DXMATRIX tmpCamera = *m_pCamera->GetCurrentMatrix();
		//
		//D3DXMATRIX transMat;
		//D3DXMatrixIdentity(&transMat);
		//
		//D3DXVECTOR3 campos = D3DXVECTOR3(tmpCamera._41, tmpCamera._42, tmpCamera._43);
		//D3DXVECTOR3 targetpos = D3DXVECTOR3( m_pPlayer->GetMatrix()->_41, m_pPlayer->GetMatrix()->_42, m_pPlayer->GetMatrix()->_43);
		//D3DXVECTOR3 diff = targetpos - campos;
		//D3DXMatrixTranslation(&transMat, 100.0f *_fElapsedTime, 0, 0);	
		//
		//m_pCamera->SetCurrentCameraMatrix(transMat * *m_pCamera->GetCurrentMatrix());
		//CPhysics::LookAt(m_pCamera->GetCurrentMatrix(), m_pPlayer->GetMatrix());
		//
		//m_pCamera->SetCurrentCameraMatrix(*m_pCamera->GetCurrentMatrix());
	}

	//if(CInput::GetHasDodge() == true)
	//{
	//	D3DXMATRIX d3dNewCamMat = *m_pCamera->GetViewMatrix();
	//	static bool doOnce = false;
	//	if(doOnce = false)
	//	{
	//		if(true)
	//		{
	//			m_d3dDodgeVector.x = d3dNewCamMat._41 + 0;
	//			m_d3dDodgeVector.y = d3dNewCamMat._42 + 200;
	//			m_d3dDodgeVector.z = d3dNewCamMat._43 + 0;
	//		}	
	//		//else if(m_pCamera->GetViewMatrix()->_43 < 0)
	//		//{
	//		//	m_d3dDodgeVector.x = 0;
	//		//	m_d3dDodgeVector.y = 0;
	//		//	m_d3dDodgeVector.z = -200.0f;
	//		//}
	//	}
	//
	//	D3DXVECTOR3 d3dWaypointVector(m_pScriptManager->GetCurrentWaypoint()->GetMatrix()->_41, m_pScriptManager->GetCurrentWaypoint()->GetMatrix()->_42, m_pScriptManager->GetCurrentWaypoint()->GetMatrix()->_43);
	//
	//
	//
	//	CPhysics::ReverseDirection(&d3dNewCamMat, &m_d3dDodgeVector, _fElapsedTime, 2.0f);
	//
	//	m_pCamera->SetViewMatrix(&d3dNewCamMat);
	//
	//
	//	D3DXVECTOR3 d3dZVector = m_d3dDodgeVector - D3DXVECTOR3(d3dNewCamMat._41, d3dNewCamMat._42, d3dNewCamMat._43);
	//	D3DXVec3Normalize(&d3dZVector, &d3dZVector);
	//
	//	float fZDotResult = D3DXVec3Dot(&d3dZVector, &CPhysics::GetMat4Vec(eZAXIS, &d3dNewCamMat));
	//
	//	if(fZDotResult > 0.95)
	//	{
	//		CInput::SetHasDodged(false);
	//		doOnce = false;
	//	}
	//}


	D3DXVECTOR3 d3dOut;
	D3DXMATRIX d3dIdentity;
	D3DVIEWPORT9 d3dViewPort;
	D3DXMATRIX d3dCameraInverse;

	D3DXMatrixIdentity(&d3dIdentity);		
	m_pRenderer->GetDevice()->GetViewport(&d3dViewPort);
	D3DXMatrixInverse(&d3dCameraInverse, 0, m_pCamera->GetViewMatrix());

	POINT tMousePosition = CInput::GetMouseScreenPos();

	if(CInput::GetInvertY() == true)
	{
		tMousePosition.y = int(-tMousePosition.y + m_pGame->GetHeight()); 
	}
	if(CInput::GetInvertX() == true)
	{
		tMousePosition.x = int(-tMousePosition.x + m_pGame->GetWidth());
	}

	D3DXVec3Unproject(&d3dOut, &D3DXVECTOR3((float)tMousePosition.x, (float)tMousePosition.y, 1.0f), &d3dViewPort, m_pCamera->GetProjectionMatrix(), &d3dCameraInverse, &d3dIdentity );

	m_TargetMat._41 = d3dOut.x;
	m_TargetMat._42 = d3dOut.y;
	m_TargetMat._43 = d3dOut.z;

	D3DXVec3Project(&d3dOut, &D3DXVECTOR3(m_TargetMat._41, m_TargetMat._42, m_TargetMat._43), &d3dViewPort, m_pCamera->GetProjectionMatrix(), &d3dCameraInverse, &d3dIdentity);

	m_ReticleMat._41 = d3dOut.x;
	m_ReticleMat._42 = d3dOut.y;
	if(m_pScriptManager->Update(_fElapsedTime))
	{
		spawnBoss = true;
	}

	if( m_pScriptManager->GetSpawnTimer() > 0.0f )
	{
		m_pScriptManager->GetCurrentWaypoint()->SetShouldTrigger(true);
	}

	if( m_pScriptManager->GetSpawnedEnemyBool() && m_pPlayer && m_pPlayer->GetHealth() > 0 )
	{
		m_pScriptManager->SetSpawnedEnemyBool(false);
		m_bPlayCinematic = true;
	}
	if( m_bPlayCinematic && m_fWaitForCinematicBucket > 1.0f )
	{
		/****************************/
		m_bPlayCinematic = false;
		m_fWaitForCinematicBucket = 0.0f;
		if( !m_bInCinematic )
		{
			if(m_pScriptManager->GetCurrentEnemies().size() > 0)
			{
				int index = -1;
				for( unsigned int i(0); i < m_pScriptManager->GetCurrentEnemies().size(); ++i )
				{
					if( !m_pGame->GetHasSeenYellow() )
					{
						if( m_pScriptManager->GetCurrentEnemies()[i]->GetType() == eYELLOW_ENEMY )
						{
							m_pGame->SeenYellow(true);
							m_bShowYellowData = true;
							index = i;
							break;
						}
					}
					if( !m_pGame->GetHasSeenRed() )
					{
						if( m_pScriptManager->GetCurrentEnemies()[i]->GetType() == eRED_ENEMY )
						{
							m_pGame->SeenRed(true);
							m_bShowRedData = true;
							index = i;
							break;
						}
					}
					if( !m_pGame->GetHasSeenBlue() )
					{
						if( m_pScriptManager->GetCurrentEnemies()[i]->GetType() == eBLUE_ENEMY )
						{
							m_pGame->SeenBlue(true);
							m_bShowBlueData = true;

							index = i;
							break;
						}
					}

				}

				if( index > -1 )
				{
					m_pPrevPlayerBehavior = m_pPlayerBehavior;
					m_pPlayerBehavior = m_pInactivePlayer;
					m_pPlayer->SetCamera(m_pGame->GetCamera());
					m_pCinematicCamera->SetPlayer(m_pPlayer);
					m_pCinematicCamera->SetPlayerPos(*m_pPlayer->GetMatrix());
					m_pGame->GetCamera()->DetachCamera(true);

					D3DXMATRIX d3dLookMat;
					D3DXMatrixIdentity(&d3dLookMat);
					d3dLookMat._41 = m_pScriptManager->GetCurrentEnemies()[index]->GetMatrix()->_41;
					d3dLookMat._42 = m_pScriptManager->GetCurrentEnemies()[index]->GetMatrix()->_42;
					d3dLookMat._43 = m_pScriptManager->GetCurrentEnemies()[index]->GetMatrix()->_43;

					D3DXVECTOR3 ahead = D3DXVECTOR3( m_pScriptManager->GetCurrentEnemies()[index]->GetMatrix()->_31,
						m_pScriptManager->GetCurrentEnemies()[index]->GetMatrix()->_32, 
						m_pScriptManager->GetCurrentEnemies()[index]->GetMatrix()->_33);

					D3DXVec3Normalize(&ahead, &ahead);
					D3DXMATRIX d3dMat;

					float speed = 0.0f;

					switch( m_pScriptManager->GetCurrentEnemies()[index]->GetType() )
					{
					case eRED_ENEMY:
						{
							D3DXMatrixTranslation(&d3dMat, 10.0f * ahead.x, 10.0f * ahead.y, 10.0f * ahead.z);
							speed = 2.5f;
						}
						break;
					case eYELLOW_ENEMY:
						{
							D3DXMatrixTranslation(&d3dMat, 5.0f * ahead.x, 5.0f * ahead.y, 5.0f * ahead.z);
							speed = 2.5f;
						}
						break;
					case eBLUE_ENEMY:
						{
							D3DXMatrixTranslation(&d3dMat, 40.0f * ahead.x, 40.0f * ahead.y, 40.0f * ahead.z);
							speed = 5.0f;
						}
						break;
					}

					d3dMat = *m_pScriptManager->GetCurrentEnemies()[index]->GetMatrix() * d3dMat;

					D3DXMATRIX mat;
					D3DXMatrixTranslation(&mat, 0.0f, 5.0f, 0.0f);

					d3dMat = d3dMat * mat;

					m_pCinematicCamera->SetDestination(d3dMat, d3dLookMat, 15.0f);
					m_pCinematicCamera->SetRotation(speed, d3dLookMat, 10.0f);
					m_pCinematicCamera->MoveToDestination();

					m_bInCinematic = true;
					WwiseNS::SetVolumeRTPC( AK::GAME_PARAMETERS::MX_VOLUME, m_pGame->GetMusicVolume() - 10);
					m_pObjectManager->SetInCinematic(true);
					return;
				}
			}
		}		
		/****************************/
	}
	if (spawnBoss && MakebossOnce && m_pScriptManager->GetSpawnBoss())
	{
		MakebossOnce = false;
		spawnBoss = false;
		bossFight = true;
		m_pObjectManager->SetBossFight(true);

		// Cinematic Camera
		/****************************/
		if( !m_bInCinematic )
		{
			m_pPrevPlayerBehavior = m_pPlayerBehavior;
			m_pPlayerBehavior = m_pInactivePlayer;
			m_pPlayer->SetCamera(m_pGame->GetCamera());
			m_pCinematicCamera->SetPlayer(m_pPlayer);
			m_pCinematicCamera->SetPlayerPos(*m_pPlayer->GetMatrix());
			m_pGame->GetCamera()->DetachCamera(true);

			CBoss * pTheBoss = m_pScriptManager->GetTheBoss();
			D3DXVECTOR3 bossDestPos = pTheBoss->GetDestinationPos();
			D3DXMATRIX *playerMatrix = m_pPlayer->GetMatrix();
			//D3DXVECTOR3 toboss = D3DXVECTOR3( bossDestPos.x - playerMatrix->_41,
			//									bossDestPos.y - playerMatrix->_42,
			//									bossDestPos.z - playerMatrix->_43);

			D3DXMATRIX d3dLookMat;
			D3DXMatrixIdentity(&d3dLookMat);
			d3dLookMat._41 = m_pScriptManager->GetTheBoss()->GetDestinationPos().x;
			d3dLookMat._42 = m_pScriptManager->GetTheBoss()->GetDestinationPos().y;
			d3dLookMat._43 = m_pScriptManager->GetTheBoss()->GetDestinationPos().z + 150.0f;

			D3DXMATRIX d3dMat;
			D3DXMatrixTranslation(&d3dMat, -450.0f, 600, -500.0f);
			d3dMat = d3dLookMat * d3dMat;

			//	CEffectSystem es;			
			//	EffectInformation::CreateEffect(eRED_ENEMY_ARRIVAL, &es, m_pPlayer, true, true);


			m_pCinematicCamera->SetDestination(d3dMat, d3dLookMat, 15.0f);
			m_pCinematicCamera->SetRotation(50.0f, d3dLookMat, 7.0f);
			m_pCinematicCamera->MoveToDestination();

			m_pCinematicCamera->SetIsBossFight(true, m_pScriptManager->GetTheBoss());

			m_bInCinematic = true;
			WwiseNS::SetVolumeRTPC( AK::GAME_PARAMETERS::MX_VOLUME, m_pGame->GetMusicVolume() - 10);
			m_pObjectManager->SetInCinematic(true);
			WwiseNS::SoundEvent(eHydraEntrance, ePlay);
			WwiseNS::SoundEvent(eHydraThrusters, ePlay);

			D3DXMATRIX m, mpos;
			D3DXMatrixTranslation(&m, m_pScriptManager->GetTheBoss()->GetDestinationPos().x + 1000.0f,
				m_pScriptManager->GetTheBoss()->GetDestinationPos().y + 300.0f, 
				m_pScriptManager->GetTheBoss()->GetDestinationPos().z - 750.0f);

			D3DXMatrixTranslation(&mpos, m_pScriptManager->GetTheBoss()->GetDestinationPos().x,
				m_pScriptManager->GetTheBoss()->GetDestinationPos().y, 
				m_pScriptManager->GetTheBoss()->GetDestinationPos().z);
			CPhysics::LookAt(&m, &mpos);

			m_pCamera->SetReturnPos(m);
		}		
		/****************************/

		//tempBoss = (CBlueEnemy*)m_pObjectManager->ReturnEnemyToMasterList(eBLUE_ENEMY);
		/*	tempBoss = new CBlueEnemy(m_pPlayer, s_AIHelper);
		tempBoss->SetActive(true);
		D3DXVECTOR3 tempPos = m_pPlayer->GetPosition();

		D3DXMATRIX d3dNewPlayerPos(*m_pCamera->GetViewMatrix());


		D3DXVECTOR3 d3dLocalZ(m_pCamera->GetViewMatrix()->_31, m_pCamera->GetViewMatrix()->_32,m_pCamera->GetViewMatrix()->_33);
		D3DXVec3Normalize(&d3dLocalZ, &d3dLocalZ);

		D3DXVECTOR3 _d3dLocalY(m_pCamera->GetViewMatrix()->_21, m_pCamera->GetViewMatrix()->_22, m_pCamera->GetViewMatrix()->_23);
		D3DXVec3Normalize(&_d3dLocalY, &_d3dLocalY);

		D3DXVECTOR3 d3dLocalX(m_pCamera->GetViewMatrix()->_11, m_pCamera->GetViewMatrix()->_12, m_pCamera->GetViewMatrix()->_13);
		D3DXVec3Normalize(&d3dLocalX, &d3dLocalX);

		tempPos.x += d3dLocalZ.x * m_pPlayer->GetVelocityModifier()* 10;
		tempPos.y += d3dLocalZ.y * m_pPlayer->GetVelocityModifier()*10;
		tempPos.z += d3dLocalZ.z * m_pPlayer->GetVelocityModifier()*10;

		tempBoss->SetPosition( tempPos );

		tempBoss->SetTarget(m_pPlayer);
		tempBoss->SetHealth(s_AIHelper->GetHealth() * 2);
		tempBoss->SetShields(s_AIHelper->GetblueShield() * 2);
		tempBoss->SetIsStunned(false, 0.0f);
		tempBoss->GetShield()->SetActive(true);

		if (tempBoss->GetMovementBehavior()) 
		{
		tempBoss->GetMovementBehavior()->SetForceChasebool(false);
		}


		m_pObjectManager->AddObject(tempBoss);*/
	}

	if (bossFight && m_pScriptManager->GetTheBoss()->GetPlayerLose())
	{
		// TODO DESTROY WORLD HERE
		m_pObjectManager->SetPlayerLose(true);
		m_fFadeToWhiteAlpha = 1;
		m_pGame->switchState(new CGameOverState(m_pGame));

		return;
	}
	else if( bossFight && m_pScriptManager->GetTheBoss()->GetBattleTimer() <= 0.0f && m_pScriptManager->GetTheBoss()->GetPhase() != eSEEK_AND_DESTROY )
	{
		float timer = m_pScriptManager->GetTheBoss()->GetDeathWaitTimer();
		static bool bPlayTimeOutSound = true;

		if(bPlayTimeOutSound)
		{
			if(timer > 10.0f)
			{
				WwiseNS::SoundEvent(eAITimerRanOut, ePlay);
				bPlayTimeOutSound = false;
			}
		}
		/*if( timer > 26.0f )
		{			
		m_fFadeToWhiteAlpha += fTime * 50.0f;
		if( m_fFadeToWhiteAlpha > 255.0f )
		{
		m_fFadeToWhiteAlpha = 254.0f;
		}
		}
		else*/ if( timer > 14.5f && timer < 16.5f )
		{
			m_pCamera->Shake(1, 3, 0.25f);
		}

		if( m_pScriptManager->GetTheBoss()->GetWinTurret()->GetChargingTimer() < 3.0f )
		{
			//	m_pCamera->Shake( (int)(timer*0.5f), (int)(timer*0.5f), 0.5f);
		}

		// Cinematic Camera
		/****************************/
		if( !m_bInCinematic )
		{
			CBoss * pTheBoss = m_pScriptManager->GetTheBoss();
			m_pPrevPlayerBehavior = m_pPlayerBehavior;
			m_pPlayerBehavior = m_pInactivePlayer;
			m_pPlayer->SetCamera(m_pGame->GetCamera());
			m_pCinematicCamera->SetPlayer(pTheBoss->GetPlanet());
			m_pCinematicCamera->SetPlayerPos(*pTheBoss->GetPlanet()->GetMatrix());
			m_pGame->GetCamera()->DetachCamera(true);

			D3DXMATRIX mat;
			D3DXMatrixTranslation(&mat, 0, -400, -750);
			mat = *m_pScriptManager->GetTheBoss()->GetMatrix() * mat;
			m_pCinematicCamera->SetCameraPositionAfterEntrance(mat);

			D3DXVECTOR3 planettoboss = D3DXVECTOR3( pTheBoss->GetPosition().x - pTheBoss->GetPlanet()->GetPosition().x,
				pTheBoss->GetPosition().y - pTheBoss->GetPlanet()->GetPosition().y,
				pTheBoss->GetPosition().z - pTheBoss->GetPlanet()->GetPosition().z);
			D3DXVec3Normalize(&planettoboss, &planettoboss);

			D3DXMATRIX d3dLookMat;
			D3DXMatrixIdentity(&d3dLookMat);
			d3dLookMat._41 = pTheBoss->GetPlanet()->GetPosition().x;
			d3dLookMat._42 = pTheBoss->GetPlanet()->GetPosition().y;
			d3dLookMat._43 = pTheBoss->GetPlanet()->GetPosition().z;

			D3DXMATRIX d3dMat;
			D3DXMatrixTranslation(&d3dMat, 400.0f, -200, planettoboss.z * 250.0f);
			d3dMat = *pTheBoss->GetWinTurret()->GetMatrix() * d3dMat;

			//	CEffectSystem es;			
			//	EffectInformation::CreateEffect(eRED_ENEMY_ARRIVAL, &es, m_pPlayer, true, true);


			m_pCinematicCamera->SetDestination(d3dMat, *pTheBoss->GetWinTurret()->GetMatrix(), 15.0f, 3.0f);
			m_pCinematicCamera->SetRotation(0.0f, d3dLookMat, 15.0f, false, false);
			m_pCinematicCamera->MoveToDestination(false);

			m_pCinematicCamera->SetIsBossFight(true, m_pScriptManager->GetTheBoss());

			m_bInCinematic = true;
			WwiseNS::SetVolumeRTPC( AK::GAME_PARAMETERS::MX_VOLUME, m_pGame->GetMusicVolume() - 10);
			m_pObjectManager->SetInCinematic(true);
		}		
		/****************************/
	}

	if (bossFight && m_pScriptManager->GetTheBoss()->GetIsDisabled() == true )//temp boss dead )
	{				
		static bool doOnceDepart = false;
		static bool doOnceDepart1 = false;

		if( !m_bInCinematic && m_pScriptManager->GetTheBoss()->GetIsVisable() && m_fWinTimer < 1.5f )
		{
			D3DXMATRIX mat;
			m_pPrevPlayerBehavior = m_pPlayerBehavior;
			m_pPlayerBehavior = m_pInactivePlayer;

			D3DXMatrixTranslation(&mat, 350, 400, 1500);
			mat = *m_pScriptManager->GetTheBoss()->GetMatrix() * mat;

			m_pCamera->DetachCamera(true);

			CPhysics::LookAt(&mat, m_pScriptManager->GetTheBoss()->GetMatrix());
			m_pCamera->SetCurrentCameraMatrix(mat);

			m_pCinematicCamera->SetDestination(mat, *m_pScriptManager->GetTheBoss()->GetMatrix(), 15.0f);
			m_pCinematicCamera->SetCameraPositionAfterEntrance(mat);
			m_pCinematicCamera->SetRotation(0.0f, *m_pScriptManager->GetTheBoss()->GetMatrix(), 12.0f);
			m_pCinematicCamera->MoveToDestination(false);

			m_bInCinematic = true;
			WwiseNS::SetVolumeRTPC( AK::GAME_PARAMETERS::MX_VOLUME, m_pGame->GetMusicVolume() - 10);
			m_pObjectManager->SetInCinematic(true);
		}
		else if( m_pCinematicCamera->GetHasReachedDestination() && !m_pPlayer->GetIsFinalCinematic() )
		{
			m_pPlayer->SetIsFinalCinematic(true);

			D3DXMATRIX mat2;
			D3DXVECTOR3 vec = D3DXVECTOR3(m_pCinematicCamera->GetMatrix()->_31, m_pCinematicCamera->GetMatrix()->_32, m_pCinematicCamera->GetMatrix()->_33);
			D3DXVec3Normalize(&vec, &vec);
			D3DXMatrixTranslation(&mat2, -900, 375, 1000);
			mat2 = *m_pScriptManager->GetTheBoss()->GetMatrix() * mat2;
			m_pPlayer->SetMatrix(&mat2);

			D3DXMATRIX mat;
			D3DXMatrixTranslation(&mat, 400, 400, 1500);
			mat = *m_pScriptManager->GetTheBoss()->GetMatrix() * mat;
			CPhysics::LookAt(&mat, m_pScriptManager->GetTheBoss()->GetMatrix());
			D3DXVECTOR3 nvec = D3DXVECTOR3(mat._41, mat._42, mat._43);
			m_pPlayer->SetDestination( nvec ); 
		}

		m_fWinTimer += _fElapsedTime;

		if(m_fWinTimer > 16.0f)
		{
			if(doOnceDepart1 == false)
			{
				m_pPlayer->GetDepartEffect();
				//	m_pPlayer->SetIsVisable(false);
				m_pPlayer->StopAllEffects();
				doOnceDepart1 = true;
				m_esWarpDust.SetPlay(true);
				m_pObjectManager->SetRemoveMiscItems(true);
				m_pGame->PushState(new CWinState(m_pGame));
				m_pCamera->SetInWinScreen(true);
				return;
			}
		}
		else if(m_fWinTimer > 14.5f)
		{
			doOnceDepart = false;
			doOnceDepart1 = false;
			WwiseNS::RemoveAllSoundObjs();
			/*
			m_pCamera->SetWaitForGame(true);

			if(doOnceDepart == false)
			{
			m_pPlayer->SetVelocityModifier(m_pPlayer->GetVelocityModifier() + 300);
			doOnceDepart = true;
			}
			*/
		}
		else if( m_fWinTimer > 12.0f )
		{
			m_pPlayerBehavior = m_pPrevPlayerBehavior;
			//m_pCamera->DetachCamera(false);
			m_pCamera->SetWaitForGame(true);
			m_pObjectManager->SetInCinematic(true);
			//m_bInCinematic = false;
			// Only do it on the first time in this loop because setting the player to inactive over and over will cause unbuffered sounds to play.
			if(m_fWinTimer < 12.0f + _fElapsedTime)
				m_pPlayer->SetInactive(false);
		}


		if( m_fWinTimer > 5.0f && m_fWinTimer < 6.45f )
		{
			m_pCamera->Shake(1, 15, 0.25f);
		}
		else if( m_fWinTimer > 6.45f && m_fWinTimer < 6.6f )
		{
			m_pCamera->Shake(145, 145, 0.2f);
		}
		else if( m_fWinTimer > 8.0f && m_fWinTimer < 10.0f )
		{
			m_pCamera->Shake(45, 45, 0.2f);
		}
		else if( m_fWinTimer > 9.9f && m_fWinTimer < 11.0f )
		{
			int val = 3 - (int)( m_fWinTimer - 9.0f );
			m_pCamera->Shake(1, val*4, 0.25f );
		}
		static bool shouldfade = false;
		if( m_fWinTimer > 9.0f )
		{
			m_fExplosionFadeAlpha -= 150.0f * _fElapsedTime;
			if( m_fExplosionFadeAlpha < 0 )
			{
				m_fExplosionFadeAlpha = 0;
				shouldfade = false;
			}
		}
		else if( m_fWinTimer > 8.5f && m_fExplosionFadeAlpha == 255 )
		{
			shouldfade = true;
			m_fExplosionFadeAlpha = 100.0f;
		}
		else if( shouldfade )
		{
			m_fExplosionFadeAlpha += 500.0f * _fElapsedTime;
		}

		/* UN-comment for following the player
		if( m_pCinematicCamera->GetHasFinishedCinematic() )
		{
		D3DXMATRIX mat;
		D3DXMatrixTranslation(&mat, 0.0f, 10.0f, -100.0f);
		mat = mat * *m_pPlayer->GetMatrix();
		m_pCamera->SetViewMatrix( &mat );
		CPhysics::LookAt(m_pCamera->GetCurrentMatrix(), m_pPlayer->GetMatrix()); 
		}
		*/
	}

	/*if( m_fFadeToWhiteAlpha > 0 )
	{
	m_fFadeToWhiteAlpha += 50 * _fElapsedTime;
	if( m_fFadeToWhiteAlpha > 255 )
	{
	m_fFadeToWhiteAlpha = 255;
	}
	}*/

	if( !m_bInCinematic )
	{
		m_pObjectManager->UpdateObjects(_fElapsedTime);
		m_pObjectManager->CheckCollision(false, m_pGame);
	}
	else if(m_pScriptManager->GetTheBoss() )
	{
		if( m_pScriptManager->GetTheBoss()->GetPhase() == eCINEMATIC_PHASE)
		{
			m_pObjectManager->UpdateObjects(_fElapsedTime, fTime);
		}
		else if( m_pScriptManager->GetTheBoss()->GetBattleTimer() <= 0.0f )
		{
			m_pObjectManager->UpdateObjects(fTime);
		}
		else
		{
			m_pObjectManager->UpdateObjects(_fElapsedTime);
		}
	}
	else if( m_pCinematicCamera->GetIsIntroCinematic() )
	{
		m_pObjectManager->UpdateObjects(fTime);
	}
	else
	{
		m_pObjectManager->UpdateObjects(_fElapsedTime);
	}

	m_pPlayerBehavior->Update(_fElapsedTime);


	static float s_fYellowSpawnTimer = 0.0f;

	if(m_bSpawnYellowEnemy == true && m_pScriptManager->GetSpawnYellowEnemies() == true)
	{
		CYellowEnemy * pNewEnemy = (CYellowEnemy*)m_pObjectManager->ReturnEnemyToMasterList( eYELLOW_ENEMY );

		if (pNewEnemy != nullptr)
		{
			pNewEnemy->SetTarget(m_pPlayer);
			D3DXVECTOR3 tempPos = m_pPlayer->GetPosition();
			pNewEnemy->SetNeedAnArrow(false);

			D3DXMATRIX d3dNewPlayerPos(*m_pCamera->GetViewMatrix());


			D3DXVECTOR3 d3dLocalZ(m_pCamera->GetViewMatrix()->_31, m_pCamera->GetViewMatrix()->_32,m_pCamera->GetViewMatrix()->_33);
			D3DXVec3Normalize(&d3dLocalZ, &d3dLocalZ);

			D3DXVECTOR3 _d3dLocalY(m_pCamera->GetViewMatrix()->_21, m_pCamera->GetViewMatrix()->_22, m_pCamera->GetViewMatrix()->_23);
			D3DXVec3Normalize(&_d3dLocalY, &_d3dLocalY);

			D3DXVECTOR3 d3dLocalX(m_pCamera->GetViewMatrix()->_11, m_pCamera->GetViewMatrix()->_12, m_pCamera->GetViewMatrix()->_13);
			D3DXVec3Normalize(&d3dLocalX, &d3dLocalX);

			tempPos.x += d3dLocalZ.x * 100.0f * 10;
			tempPos.y += d3dLocalZ.y * 100.0f * 10;
			tempPos.z += d3dLocalZ.z * 100.0f * 10;

			pNewEnemy->SetPosition( tempPos );
			pNewEnemy->SetHealth(s_AIHelper->GetyellowShield());
			//pNewEnemy->SetShields(s_AIHelper->GetyellowShield());
			pNewEnemy->SetIsStunned(false, 0.0f);

			if (pNewEnemy->GetMovementBehavior())
			{
				pNewEnemy->GetMovementBehavior()->SetForceChasebool(false);
			}

			for (unsigned int i = 0; i < pNewEnemy->GetMovementBehaviorStack().size(); i++)
			{
				pNewEnemy->PopBehavior();
			}

			CPhysics::LookAt(pNewEnemy, m_pPlayer);
			m_pObjectManager->SetNumYellowEnemies(m_pObjectManager->GetNumYellowEnemies() + 1);
			pNewEnemy->CreateArrivalEffect();

			s_fYellowSpawnTimer = 0.0f;
			m_bSpawnYellowEnemy = false;
		}
	}

	if (!bossFight && m_pPlayer->GetAlive() == true)
	{
		// 		if(m_pScriptManager->GetCurrentEnemies().size() == 0)
		// 		{
		// 			if(m_pObjectManager->GetNumYellowEnemies() < 3)
		// 			{
		// 				if(s_fYellowSpawnTimer > 15.0f)
		// 				{
		// 					m_bSpawnYellowEnemy = true;
		// 				}
		// 				s_fYellowSpawnTimer += _fElapsedTime;
		// 			}
		// 		}
		// 		else
		// 		{
		// 			if(m_pObjectManager->GetNumYellowEnemies() < 4)
		// 			{
		// 				if(s_fYellowSpawnTimer > 5.0f)
		// 				{
		// 					m_bSpawnYellowEnemy = true;
		// 				}
		// 
		// 				s_fYellowSpawnTimer += _fElapsedTime;
		// 
		// 			}
		// 		}
	}
	if(m_bTooFar == true)
	{		
		if(D3DXVec3Length(&(m_pPlayer->GetPosition() - m_pScriptManager->GetCurrentWaypoint()->GetPosition())) > 2200)
		{
			WwiseNS::SoundEvent(eOutOfBounds, ePlay);
			m_bTooFar = false;
			m_bTurnAround = true;
			m_pCamera->SetForceReturn(true);
		}
		else if(D3DXVec3Length(&(m_pPlayer->GetPosition() - m_pScriptManager->GetCurrentWaypoint()->GetPosition())) < 2000)
		{
			m_bTooFar = false;
		}
	}
	else if(m_bTooFar == false && m_bTurnAround == false)
	{
		if(D3DXVec3Length(&(m_pPlayer->GetPosition() - m_pScriptManager->GetCurrentWaypoint()->GetPosition())) > 2000)
		{
			m_bTooFar = true;
			m_bShowWaypoint = true;
		}
	}
	else if(m_bTurnAround == true || m_bTooFar == true)
	{
		if(D3DXVec3Length(&(m_pPlayer->GetPosition() - m_pScriptManager->GetCurrentWaypoint()->GetPosition())) < 2000)
		{
			m_bShowWaypoint = false;
			m_bTooFar = false;
			m_bTurnAround = false;
			m_pCamera->SetForceReturn(false);
			m_pCamera->DetachCamera(false);
			m_bChangeTurnAroundCam = false;
		}
	}

	if (!m_bTooFar)
	{
		if(m_bShowWaypoint)
		{
			if(m_pScriptManager->GetCurrentWaypoint()->GetShouldScale() == false)
			{
				if(m_fDistanceFromWaypoint < 150.0f && m_pScriptManager->GetCurrentWaypoint()->GetScaleDown() == false)
				{
					m_pScriptManager->GetCurrentWaypoint()->SetShouldScale(true);
					m_pScriptManager->GetCurrentWaypoint()->SetScaleDown(true);
				}
			}
			if(m_pScriptManager->GetCurrentEnemies().size())
			{
				m_pScriptManager->GetCurrentWaypoint()->SetShouldScale(true);
				m_pScriptManager->GetCurrentWaypoint()->SetScaleDown(true);
				//m_pScriptManager->GetCurrentWaypoint()->SetIsVisable(false);

				m_bShowWaypoint = false;
				m_pPlayer->SetMoveFaster(false);
			}
		}
		else
		{
			if(m_pScriptManager->GetCurrentEnemies().size() == 0)
			{
				if( m_d3dPrevWaypointPosition != m_pScriptManager->GetCurrentWaypoint()->GetPosition() )
				{
					//Save Stats here
					m_pGame->SaveCurrentStats();
					m_bNeedToProceed = true;
					m_pScriptManager->GetCurrentWaypoint()->SetShouldScale(true);
					m_pScriptManager->GetCurrentWaypoint()->SetScaleDown(false);
					m_d3dPrevWaypointPosition = m_pScriptManager->GetCurrentWaypoint()->GetPosition();
					m_pScriptManager->GetCurrentWaypoint()->SetIsVisable(true);
					m_bShowWaypoint = true;
					m_pPlayer->SetMoveFaster(true);
				}
				if(m_pScriptManager->GetIsInTutorial() == true)
				{
					m_bShowWaypoint = true;
					m_pPlayer->SetMoveFaster(true);
				}
			}
		}
	}

	D3DXVECTOR3 d3dBegin, d3dEnd = D3DXVECTOR3(m_TargetMat._41, m_TargetMat._42, m_TargetMat._43);	
	D3DXVec3Unproject(&d3dBegin, &D3DXVECTOR3((float)tMousePosition.x, (float)tMousePosition.y, 0.0f), &d3dViewPort, m_pCamera->GetProjectionMatrix(), &d3dCameraInverse, &d3dIdentity );

	//D3DXMATRIX tmpMat = *m_pGame->GetCamera()->GetViewMatrix();
	//if (m_pPlayer->GetType() == eRED_PLAYER)
	//{
	//	//This is here to make the red enemy evade when the player looks
	//	//at him because thats what the enemyLockOnTarget function does when it selects a target
	//	D3DXVECTOR3 d3dTmpN;	
	//	D3DXVec3Normalize(&d3dTmpN, &CPhysics::GetMat4Vec(eZAXIS, m_pGame->GetCamera()->GetViewMatrix()));
	//	m_pObjectManager->GetReticuleTarget(d3dBegin, d3dEnd, 2.5f);
	//	CEntity* CMissileTarget = m_pObjectManager->EnemyLockOnTarget(CPhysics::GetMat4Vec(ePOS, m_pCamera->GetViewMatrix()), d3dTmpN, 2.5f);
	//
	//}

	if (m_pPlayer->GetType() == eRED_PLAYER)
	{
		//This is here to make the red enemy evade when the player looks
		//at him because thats what the enemyLockOnTarget function does when it selects a target
		D3DXVECTOR3 d3dTmpN;	
		D3DXVec3Normalize(&d3dTmpN, &CPhysics::GetMat4Vec(eZAXIS, m_pGame->GetCamera()->GetViewMatrix()));
		CEntity* CMissileTarget = m_pObjectManager->EnemyLockOnTarget(CPhysics::GetMat4Vec(ePOS, m_pGame->GetCamera()->GetViewMatrix()), d3dTmpN, 6.25f, _fElapsedTime);

	}

	if (m_pPlayer->GetType() == eBLUE_PLAYER)
	{
		D3DXVECTOR3 d3dTmpN;	
		D3DXVec3Normalize(&d3dTmpN, &CPhysics::GetMat4Vec(eZAXIS, m_pCamera->GetViewMatrix()));
		CEntity* CMissileTarget = 0, *pTmpTarget = 0;
		for(float LockOnCounter = 1; LockOnCounter < 8; LockOnCounter++)
		{
			pTmpTarget = m_pObjectManager->EnemyLockOnTarget(CPhysics::GetMat4Vec(ePOS, m_pCamera->GetViewMatrix()), d3dTmpN, LockOnCounter, _fElapsedTime);

			if(pTmpTarget)
			{
				D3DXVECTOR3 d3dTowardsVec; D3DXVec3Normalize(&d3dTowardsVec,  &(pTmpTarget->GetPosition() - m_pPlayer->GetPosition()));

				if((D3DXVec3Dot(&d3dTmpN,&d3dTowardsVec) > 0.7))
				{
					if (pTmpTarget && (D3DXVec3Length(&(pTmpTarget->GetPosition() - m_pPlayer->GetPosition())) <= 650  ) )
					{
						CMissileTarget = pTmpTarget;
						break;
					}
				}
			}
		}

		if (CMissileTarget)
		{

			int nType = CMissileTarget->GetType();
			if(nType == eRED_ENEMY || nType == eBLUE_ENEMY || nType == eYELLOW_ENEMY)
			{
				((CEnemy*)(CMissileTarget))->SetShouldRun(true);
			}
		}

		if(m_fMissileReloadTimer < 1)
		{
			m_fMissileReloadTimer += _fElapsedTime;
			if(m_fMissileReloadTimer >= 1)
			{
				WwiseNS::SoundEvent(eMissileReload, ePlay);
			}
		}

		if(CMissileTarget/* && m_fMissileReloadTimer > 1*/ && m_fLockOnTimer < m_fTimeToLockOn)
		{
			if(m_pPlayer->GetTarget() == CMissileTarget)
			{
				m_fLockOnTimer += _fElapsedTime;
				if(m_fLockOnTimer == _fElapsedTime && m_bInCinematic == false) 
				{
					WwiseNS::SoundEvent(eMissileLockOn, ePlay);
				}

				if(m_fLockOnTimer >= m_fTimeToLockOn)
				{
					WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);
				}
			}
			else
			{
				m_pPlayer->SetTarget(CMissileTarget);
				m_fLockOnTimer = 0.0f;
			}
		}
		else if(m_fLockOnTimer > m_fTimeToLockOn)
		{
			D3DXVECTOR3 d3dVecBetween = m_pPlayer->GetTarget()->GetPosition() - CPhysics::GetMat4Vec(ePOS, m_pCamera->GetViewMatrix());
			D3DXVec3Normalize(&d3dVecBetween, &d3dVecBetween);

			if(D3DXVec3Dot(&d3dVecBetween, &d3dTmpN) < 0.7 || !m_pPlayer->GetTarget()->GetActive())
			{
				m_pPlayer->SetTarget(0);
				m_fLockOnTimer = 0.0f;
			}
		}
		else if(!CMissileTarget)
		{
			m_fLockOnTimer = 0;
		}
	}
	else
	{
		m_fMissileReloadTimer = 1.1f;
		m_fLockOnTimer = 0.0f;
	}


	if (bossFight && !m_bInCinematic)
	{
		if (  m_pScriptManager->GetCurrentWaypoint() )
		{
			m_pScriptManager->GetCurrentWaypoint()->SetIsVisable(false);
			m_pScriptManager->GetCurrentWaypoint()->SetMatrix( m_pScriptManager->GetTheBoss()->GetMatrix());
		}


	}
	//if (m_pPlayer->GetType() == eBLUE_PLAYER)
	//{
	//	D3DXVECTOR3 d3dTmpN;	
	//	D3DXVec3Normalize(&d3dTmpN, &CPhysics::GetMat4Vec(eZAXIS, m_pCamera->GetViewMatrix()));
	//	CEntity* CMissileTarget = 0, *pTmpTarget = 0;
	//	for(float LockOnCounter = 1; LockOnCounter < 8; LockOnCounter++)
	//	{
	//		pTmpTarget = m_pObjectManager->GetReticuleTarget(d3dBegin, d3dEnd, LockOnCounter);
	//
	//		if(pTmpTarget && pTmpTarget->GetType() != eASTEROID && pTmpTarget->GetType() != eLARGE_ASTEROID)
	//		{
	//			D3DXVECTOR3 d3dTowardsVec; D3DXVec3Normalize(&d3dTowardsVec,  &(pTmpTarget->GetPosition() - m_pPlayer->GetPosition()));
	//
	//			if((D3DXVec3Dot(&d3dTmpN,&d3dTowardsVec) > 0.7))
	//			{
	//				CMissileTarget = pTmpTarget;
	//				break;
	//			}
	//		}
	//	}
	//
	//	int nType =0;
	//	if (CMissileTarget)
	//	{
	//
	//		nType = CMissileTarget->GetType();
	//		if(nType == eRED_ENEMY || nType == eBLUE_ENEMY)
	//		{
	//			((CEnemy*)(CMissileTarget))->SetShouldRun(true);
	//		}
	//	}
	//	if(m_fMissileReloadTimer < 1)
	//	{
	//		if(nType == eBLUE_ENEMY)
	//		{
	//			m_fMissileReloadTimer += _fElapsedTime*1.7f;
	//		}
	//		else
	//		{
	//			m_fMissileReloadTimer += _fElapsedTime;
	//		}
	//
	//	}
	//
	//	if(CMissileTarget/* && m_fMissileReloadTimer > 1*/ && m_fLockOnTimer < 1.0f)
	//	{
	//		if(m_pPlayer->GetTarget() == CMissileTarget)
	//		{
	//			m_fLockOnTimer += _fElapsedTime;
	//			if(m_fLockOnTimer == _fElapsedTime)
	//			{
	//				WwiseNS::SoundEvent(eMissileLockOn, ePlay);
	//			}
	//
	//			if(m_fLockOnTimer >= 1.0f)
	//			{
	//				WwiseNS::SoundEvent(eMenuItemSwitch, ePlay);
	//			}
	//		}
	//		else
	//		{
	//			m_pPlayer->SetTarget(CMissileTarget);
	//			m_fLockOnTimer = 0.0f;
	//		}
	//	}
	//	else if(m_fLockOnTimer > 1.0f)
	//	{
	//		D3DXVECTOR3 d3dVecBetween = m_pPlayer->GetTarget()->GetPosition() - CPhysics::GetMat4Vec(ePOS, m_pCamera->GetViewMatrix());
	//		D3DXVec3Normalize(&d3dVecBetween, &d3dVecBetween);
	//
	//		if(D3DXVec3Dot(&d3dVecBetween, &d3dTmpN) < 0.7 || !m_pPlayer->GetTarget()->GetActive())
	//		{
	//			m_pPlayer->SetTarget(0);
	//			m_fLockOnTimer = 0.0f;
	//		}
	//	}
	//	else if(!CMissileTarget)
	//	{
	//		m_fLockOnTimer = 0;
	//	}
	//}
	//else
	//{
	//	m_fMissileReloadTimer = 1.1f;
	//	m_fLockOnTimer = 0.0f;
	//}


	if(m_pPlayer->GetHealth() < 45 && m_pPlayer->GetHealth() > 0)
	{
		static float fHealthSkipTime = 0.0f;
		float fHealthTimer = 0.3f;

		if(m_pPlayer->GetHealth() < 25)
		{
			fHealthTimer = 0.1f;
		}

		if(fHealthSkipTime > fHealthTimer)
		{
			m_bShowHealth = !m_bShowHealth;
			fHealthSkipTime = 0.0f;
		}

		fHealthSkipTime += _fElapsedTime;
	}
	else if(m_pPlayer->GetHealth() >= 50 || m_pPlayer->GetHealth() == 0)
	{
		m_bShowHealth = true;
	}

	if(m_pPlayer->GetShields() < 20 && m_pPlayer->GetShields() > 0)
	{
		static float fShieldSkipTime = 0.0f;
		float fShieldTimer = 0.3f;

		if(m_pPlayer->GetShields() < 10)
		{
			fShieldTimer = 0.1f;
		}

		if(fShieldSkipTime > fShieldTimer)
		{
			m_bShowShields = !m_bShowShields;
			fShieldSkipTime = 0.0f;
		}

		fShieldSkipTime += _fElapsedTime;
	}
	else if(m_pPlayer->GetShields() >= 20 || m_pPlayer->GetShields() == 0)
	{
		m_bShowShields = true;
	}

	//EnemyLocationArrow(&tmpMat, m_pPlayer, m_pPlayer->GetTarget());
	//m_pArrow->SetMatrix(&tmpMat);
	//m_pGame->GetRenderer()->RenderObjects(&tmpVector);


	// 	if (!gameJustStarted && m_pScriptManager->GetCurrentEnemies().size() <= 0 && 
	// 		D3DXVec3Length( &(m_pPlayer->GetPosition() - m_pScriptManager->GetCurrentWaypoint()->GetPosition() )) >= 600)
	// 	{		
	// 		for (unsigned int i = 0; i < m_pAsteroidList.size(); i++)
	// 		{
	// 			if (!m_pAsteroidList[i]->GetActive())
	// 			{
	// 				m_pAsteroidList[i] = (CAsteroid *)m_pObjectManager->ReturnAsteroidToMasterList();
	// 				D3DXVECTOR3 playerPos = m_pPlayer->GetPosition();
	// 				m_pAsteroidList[i]->SetRadius( 1.0f);
	// 				D3DXVECTOR3 tempPos = m_pPlayer->GetPosition();
	// 
	// 				D3DXMATRIX d3dNewPlayerPos(*m_pCamera->GetViewMatrix());
	// 
	// 
	// 				D3DXVECTOR3 d3dLocalZ(m_pCamera->GetViewMatrix()->_31, m_pCamera->GetViewMatrix()->_32,m_pCamera->GetViewMatrix()->_33);
	// 				D3DXVec3Normalize(&d3dLocalZ, &d3dLocalZ);
	// 
	// 				D3DXVECTOR3 _d3dLocalY(m_pCamera->GetViewMatrix()->_21, m_pCamera->GetViewMatrix()->_22, m_pCamera->GetViewMatrix()->_23);
	// 				D3DXVec3Normalize(&_d3dLocalY, &_d3dLocalY);
	// 
	// 				D3DXVECTOR3 d3dLocalX(m_pCamera->GetViewMatrix()->_11, m_pCamera->GetViewMatrix()->_12, m_pCamera->GetViewMatrix()->_13);
	// 				D3DXVec3Normalize(&d3dLocalX, &d3dLocalX);
	// 
	// 				
	// 				tempPos.x += d3dLocalZ.x * 60  *( ((rand() % 18) - 9) + 1);
	// 				tempPos.y += d3dLocalZ.y * 60 * (((rand() % 20) - 10) + 1);
	// 
	// 				tempPos.z += d3dLocalZ.z * (100) * (((rand() % 4) + 1) + 4);
	// 
	// 				m_pAsteroidList[i]->SetPosition( tempPos );
	// 				
	// 				//m_pAsteroidList[i]->SetPosition(playerPos.x + (rand() % 450) - 225, playerPos.y + (rand() % 450) - 225, playerPos.z + 275);
	// 				m_pAsteroidList[i]->SetIsMoving(true);
	// 				CPhysics::LookAt(m_pAsteroidList[i], m_pPlayer);
	// 			}
	// 		}
	// 
	// 	}

	if(m_bHasDodgedMissile == true)
	{
		m_bWithinRange = false;
		m_bLockedOn = false;

		static float s_fDodgedMissileTimer = 0.0f;

		if(s_fDodgedMissileTimer >= 2.0f)
		{
			m_bHasDodgedMissile = false;
			m_pCamera->DetachCamera(false);
			s_fDodgedMissileTimer = 0.0f;
			m_pPlayer->SetIsScenery(false);
		}

		s_fDodgedMissileTimer += fTime;
	}

	m_fBossAsteroidSpawnTimer += _fElapsedTime;
	if (bossFight && m_fBossAsteroidSpawnTimer >= 4.0f)
	{		
		unsigned int listSize = m_pBossAsteroidList.size();
		for (unsigned int i = 0; i < listSize; i++)
		{
			if (!m_pBossAsteroidList[i]->GetActive())
			{
				int leftOrRight = rand() % 4;
				float rad =  (((rand() % 35) + 35.0f) / 10.0f) ;
				m_pBossAsteroidList[i] = (CAsteroid *)m_pObjectManager->ReturnAsteroidToMasterList(rad);
				D3DXVECTOR3 tempPos = m_pScriptManager->GetTheBoss()->GetPosition();
				//right
				if (leftOrRight == 0)
				{
					tempPos.x +=  ((rand() % 200) + 500) ;
					tempPos.y +=   ((rand() % 400) - 200 ) ;
					tempPos.z +=  2200;
				}
				//left
				else if(leftOrRight == 1)
				{
					tempPos.x -=  ((rand() % 200) + 500 );
					tempPos.y +=   ((rand() % 400) - 200 ) ;
					tempPos.z +=  2200;
				}
				//top
				else if(leftOrRight == 2)
				{
					tempPos.x +=  ((rand() % 400) - 200 );
					tempPos.y +=  ((rand() % 150) + 400 ) ;
					tempPos.z +=  2200;
				}
				// bottom
				else 
				{
					tempPos.x +=  ((rand() % 400) - 200 );
					tempPos.y -=  ((rand() % 200) + 300 ) ;
					tempPos.z +=  2200;
				}
				m_pBossAsteroidList[i]->SetPosition( tempPos );

				//m_pAsteroidList[i]->SetPosition(playerPos.x + (rand() % 450) - 225, playerPos.y + (rand() % 450) - 225, playerPos.z + 275);
				m_pBossAsteroidList[i]->SetIsMoving(true);
				m_pBossAsteroidList[i]->Rotate(3.1415926535f, 0,1,0);
				m_fBossAsteroidSpawnTimer = 0.0f;
				break;
			}

		}

	}
	if (bossFight)
	{
		if( m_pScriptManager->GetTheBoss()->GetNewPhase() && !m_bInCinematic )
		{
			m_pPrevPlayerBehavior = m_pPlayerBehavior;
			m_pPlayerBehavior = m_pInactivePlayer;
			m_pPlayer->SetCamera(m_pGame->GetCamera());
			m_pCinematicCamera->SetPlayer(m_pPlayer);
			m_pCinematicCamera->SetPlayerPos(*m_pPlayer->GetMatrix());
			m_pGame->GetCamera()->DetachCamera(true);

			D3DXMATRIX d3dLookMat, d3dMat, d3dCamPos;
			D3DXMatrixIdentity(&d3dLookMat);
			D3DXMatrixTranslation(&d3dCamPos, 0, 150, 300);

			D3DXMATRIX m, mpos;
			bool look = false;

			switch( m_pScriptManager->GetTheBoss()->GetPhase())
			{
			case eLASER_PHASE:
				{	
					D3DXMATRIX mat;
					D3DXMatrixTranslation(&mat, 1000, 300, 750);

					mat = *m_pScriptManager->GetTheBoss()->GetMatrix() * mat;

					m_pCinematicCamera->SetCameraPositionAfterEntrance(mat);

					d3dLookMat._41 = m_pScriptManager->GetTheBoss()->GetAGenerator()->GetMatrix()->_41;
					d3dLookMat._42 = m_pScriptManager->GetTheBoss()->GetAGenerator()->GetMatrix()->_42;
					d3dLookMat._43 = m_pScriptManager->GetTheBoss()->GetAGenerator()->GetMatrix()->_43;

					D3DXMatrixTranslation(&d3dMat, 75, 25, 25);
					d3dMat = *m_pScriptManager->GetTheBoss()->GetAGenerator()->GetMatrix() * d3dMat;
					m_pScriptManager->GetTheBoss()->SetTeachPhase1(true);
					m_pScriptManager->GetTheBoss()->SetTeachPhase2(false);
					m_pScriptManager->GetTheBoss()->SetTeachPhase3(false);
				}
				break;
			case eMISSILE_PHASE:
				{
					/** Player Positioning **/
					D3DXMatrixTranslation(&m, m_pScriptManager->GetTheBoss()->GetMatrix()->_41 - 1000.0f,
						m_pScriptManager->GetTheBoss()->GetMatrix()->_42 + 300.0f, 
						m_pScriptManager->GetTheBoss()->GetMatrix()->_43 - 1000.0f);

					mpos = *m_pScriptManager->GetTheBoss()->GetMatrix();
					look = true;
					/***************************************************/

					D3DXMATRIX mat;
					D3DXMatrixTranslation(&mat, 0, 300, 750);

					mat = *m_pScriptManager->GetTheBoss()->GetMatrix() * mat;

					m_pCinematicCamera->SetCameraPositionAfterEntrance(mat);

					int choice = 0;
					for( unsigned int i(0); i < m_pScriptManager->GetTheBoss()->GetLargeTurrets().size(); ++i)
					{
						if( m_pScriptManager->GetTheBoss()->GetLargeTurrets()[i]->GetType() == eTURRET_MISSILE )
						{
							choice = i;
						}
					}
					d3dLookMat._41 = m_pScriptManager->GetTheBoss()->GetLargeTurrets()[choice]->GetMatrix()->_41;
					d3dLookMat._42 = m_pScriptManager->GetTheBoss()->GetLargeTurrets()[choice]->GetMatrix()->_42;
					d3dLookMat._43 = m_pScriptManager->GetTheBoss()->GetLargeTurrets()[choice]->GetMatrix()->_43;

					D3DXMatrixTranslation(&d3dMat, 25, 25, 20);
					d3dMat = *m_pScriptManager->GetTheBoss()->GetLargeTurrets()[choice]->GetMatrix() * d3dMat;
					m_pScriptManager->GetTheBoss()->SetTeachPhase1(false);
					m_pScriptManager->GetTheBoss()->SetTeachPhase2(true);
					m_pScriptManager->GetTheBoss()->SetTeachPhase3(false);

				}
				break;
			case eSEEK_AND_DESTROY:
				{
					/** Player Positioning **/
					D3DXMatrixTranslation(&m, m_pScriptManager->GetTheBoss()->GetMatrix()->_41 - 100,
						m_pScriptManager->GetTheBoss()->GetMatrix()->_42 - 300.0f, 
						m_pScriptManager->GetTheBoss()->GetMatrix()->_43 - 1100.0f);

					mpos = *m_pScriptManager->GetTheBoss()->GetMatrix();
					look = true;
					/***************************************************/

					D3DXMATRIX mat;
					D3DXMatrixTranslation(&mat, -100, -400, 1500);

					mat = *m_pScriptManager->GetTheBoss()->GetMatrix() * mat;

					m_pCinematicCamera->SetCameraPositionAfterEntrance(mat);

					d3dLookMat._41 = m_pScriptManager->GetTheBoss()->GetWinTurret()->GetMatrix()->_41;
					d3dLookMat._42 = m_pScriptManager->GetTheBoss()->GetWinTurret()->GetMatrix()->_42;
					d3dLookMat._43 = m_pScriptManager->GetTheBoss()->GetWinTurret()->GetMatrix()->_43;

					D3DXMatrixTranslation(&d3dMat, -75, -50, 225);
					d3dMat = *m_pScriptManager->GetTheBoss()->GetWinTurret()->GetMatrix() * d3dMat;
					m_pScriptManager->GetTheBoss()->SetTeachPhase1(false);
					m_pScriptManager->GetTheBoss()->SetTeachPhase2(false);
					m_pScriptManager->GetTheBoss()->SetTeachPhase3(true);

				}
				break;
			};

			m_pScriptManager->GetTheBoss()->SetNewPhase(false);
			m_pCinematicCamera->SetDestination(d3dMat, d3dLookMat, 15.0f);
			m_pCinematicCamera->SetRotation(2.5f, d3dLookMat, 10.0f);
			m_pCinematicCamera->MoveToDestination();

			if( look )
			{
				CPhysics::LookAt(&m, &mpos);
				m_pCamera->SetReturnPos(m);
			}

			m_bInCinematic = true;
			WwiseNS::SetVolumeRTPC( AK::GAME_PARAMETERS::MX_VOLUME, m_pGame->GetMusicVolume() - 10);
			m_pObjectManager->SetInCinematic(true);
		}
		unsigned int AsteroidListSize =  m_pAsteroidList.size();

		for (unsigned int i = 0; i < AsteroidListSize; i++)
		{

			{
				if (m_pAsteroidList[i]->GetActive())
				{
					m_pAsteroidList[i]->SetActive(false);
					m_pBossAsteroidList.push_back(m_pAsteroidList[i]);
				}
			}

		}
		m_pAsteroidList.clear();
		unsigned int bossAsteroidListSize =  m_pBossAsteroidList.size();
		for (unsigned int i = 0; i < bossAsteroidListSize; i++)
		{

			{
				if (m_pBossAsteroidList[i]->GetActive() && D3DXVec3Length( &(m_pPlayer->GetPosition() - m_pBossAsteroidList[i]->GetPosition() ) ) > 2300)
				{
					m_pBossAsteroidList[i]->SetActive(false);
				}
			}

		}
	}
	else
	{
		unsigned int AsteroidListSize =  m_pAsteroidList.size();

		for (unsigned int i = 0; i < AsteroidListSize; i++)
		{

			{
				if (m_pAsteroidList[i]->GetActive() && D3DXVec3Length( &(m_pPlayer->GetPosition() - m_pAsteroidList[i]->GetPosition() ) ) > 2300)
				{
					m_pAsteroidList[i]->SetActive(false);
				}
			}

		}
	}
	if(m_pPlayer->GetAlive() == false)
	{
		m_bTurnAround = false;
		m_bTooFar = false;

		if(m_bIsAlive == true)
		{
			m_bIsAlive = false;
			m_pCamera->DetachCamera(true);
			m_pDeadPlayer.ResetTimer(); 
			m_pDeadPlayer.CreateEffect();
			m_pPlayerBehavior = &m_pDeadPlayer;
		}
		CCamera* tempCamera = m_pCamera;
		CPhysics::LookAt(tempCamera->GetCurrentMatrix(), m_pPlayer->GetMatrix());
		m_pCamera->SetViewMatrix(tempCamera->GetViewMatrix());
	}

	if(m_pPlayer->GetActive() == false)
	{
		m_pCamera->DetachCamera(false);
		m_fFadeToWhiteAlpha = 1;
		m_pGame->switchState(new CGameOverState(m_pGame));
		return;
	}
}

/*****************************************************************
* Render(): Renders the game state by calling the renderer classes and object managers render functions
* Ins:			    none    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void GamePlayState::Render()
{
	float xRatio = m_pGame->GetRenderer()->GetXRatio();
	float yRatio = m_pGame->GetRenderer()->GetYRatio();

	if( m_pCamera->GetInWinScreen() )
	{
		m_pObjectManager->RenderObjects();
		return;
	}

	//static float timer;
	//	timer += m_pGame->GetTimer()->GetDeltaTime();

	/*if(timer > 1.0f)
	{
	timer = 0.0f;
	cout << "CUR/MAX " << ((float)(m_pPlayer->GetShields())/m_pPlayerBehavior->GetMaxShield()) << " SHIELDS " << m_pPlayer->GetShields() << endl;
	}*/

	if(m_nHelpScreenTextureID > 0 && m_fHelpScreenAlpha > 0.0f)
	{
		m_pRenderer->Render2d(m_pGame->GetHelpTexture(), m_fHelpScreenAlpha/255.0f);

		m_pRenderer->RenderText("Objective: Destroy enemy forces", (int)(224.0f * m_pRenderer->GetXRatio()), (int)(644.0f * m_pRenderer->GetYRatio()), 1.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0), false);
		m_pRenderer->RenderText("Objective: Destroy enemy forces", (int)(220.0f * m_pRenderer->GetXRatio()), (int)(640.0f * m_pRenderer->GetYRatio()), 1.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 128, 255, 0), false);
		m_pRenderer->RenderText("Press Esc, Enter, or Left Click to Exit", (int)(204.0f * m_pRenderer->GetXRatio()), (int)(704.0f * m_pRenderer->GetYRatio()), 1.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0), false);
		m_pRenderer->RenderText("Press Esc, Enter, or Left Click to Exit", (int)(200.0f * m_pRenderer->GetXRatio()), (int)(700.0f * m_pRenderer->GetYRatio()), 1.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 255, 255, 255), false);
		return;
	}

	m_pObjectManager->RenderObjects();	
	if( m_pCinematicCamera->GetShouldFadeIn() )
	{
		m_pGame->GetRenderer()->Render2d(m_pGame->GetBlackTexture(), m_pCinematicCamera->GetBlackoutAlpha() );
	}
	if( m_fExplosionFadeAlpha > 0 && m_fExplosionFadeAlpha != 255)
	{
		m_pGame->GetRenderer()->Render2d(m_nExplosionFade, m_fExplosionFadeAlpha/255);
	}
	if( m_fFadeToWhiteAlpha > 0 && m_fFadeToWhiteAlpha != 255 )
	{
		m_pGame->GetRenderer()->Render2d(m_nExplosionFade, m_fFadeToWhiteAlpha/255);
	}
	//m_pGame->GetRenderer()->RenderHUD(((float)(m_pPlayer->GetShields())/m_pPlayerBehavior->GetMaxShield()),((float)(m_pPlayer->GetHealth())/m_pPlayerBehavior->GetMaxHealth()), m_pObjectManager->GetMissileList());
	CBoss *pTheBoss =  m_pScriptManager->GetTheBoss();
	if (bossFight && m_bInCinematic && !pTheBoss->GetTeachPhase1() && !pTheBoss->GetTeachPhase2()
		&& !pTheBoss->GetTeachPhase3() && !pTheBoss->GetIsDisabled())
	{
		if (m_pScriptManager->GetTheBoss()->GetShowTeachBoss() && m_pScriptManager->GetTheBoss()->GetBattleTimer())
		{
			m_pScriptManager->GetTheBoss()->DrawIntro(m_pGame->GetBlackTexture());
		}
	}

	if (bossFight)
	{
		m_pScriptManager->GetTheBoss()->DrawFeedback( m_pGame->GetTimer()->GetDeltaTime(), m_bInCinematic );
	}

	if( !m_bInCinematic )
	{

		m_ArrowMat._41 = -50;
		m_ArrowMat._42 = 325;
		m_ArrowMat2._41 = -50;
		m_ArrowMat2._42 = 325;

		m_bShowRedData = false;
		m_bShowBlueData = false;
		m_bShowYellowData = false;
		m_bShowBossData = false;
		EnemyLocationArrow();

		//if(!bossFight)
		//{
		if(!m_bInCinematic)
		{
			EnemyLockOnIcon();
		}
		//}
		/*		USER FEEDBACK TEXT ON SCREEN		*/

		// 	if(m_fMissileReloadTimer < 1)
		// 	{
		// 		char szBuff1[100];
		// 		sprintf_s(szBuff1, "Reload - %f", m_fMissileReloadTimer);
		// 		m_pGame->GetRenderer()->RenderText(szBuff1,60,15,1.0f,0.0f,0.0f,0.0f,D3DCOLOR_XRGB(255,255,255));
		// 	}
		//if(m_fLockOnTimer < 1 && m_fLockOnTimer > 0.0f)
		//{
		//	char szBuff1[100];
		//	sprintf_s(szBuff1, "LockOn - %f", m_fLockOnTimer);
		//	m_pGame->GetRenderer()->RenderText(szBuff1,60,30,1.0f,0.0f,0.0f,0.0f,D3DCOLOR_XRGB(255,255,255));
		//}
		//else if(m_pPlayer->GetTarget() && m_fLockOnTimer > 0.0f)
		//{
		//	m_pGame->GetRenderer()->RenderText("Locked On!",60,30,1.0f,0.0f,0.0f,0.0f,D3DCOLOR_XRGB(255,255,255));
		//}

		if(m_pScriptManager->GetCurrentEnemies().size() > 0)
		{
			char szBuff[32];

			if (!bossFight && !m_pScriptManager->GetIsInTutorial())
			{
				int nNumBlueEnemies = 0;
				int nNumRedEnemies = 0;
				int nNumYellowEnemies = 0;
				for(unsigned int numEnemies = 0; numEnemies < m_pScriptManager->GetCurrentEnemies().size(); numEnemies++)
				{
					if(m_pScriptManager->GetCurrentEnemies()[numEnemies]->GetType() == eRED_ENEMY)
						nNumRedEnemies += 1;
					if(m_pScriptManager->GetCurrentEnemies()[numEnemies]->GetType() == eBLUE_ENEMY)
						nNumBlueEnemies += 1;
					if(m_pScriptManager->GetCurrentEnemies()[numEnemies]->GetType() == eYELLOW_ENEMY)
						nNumYellowEnemies += 1;

				}
				m_pRenderer->RenderArrowAndrewR(m_pGame->GetBlackTexture(), &m_rBlack, m_EnemyBox, D3DCOLOR_ARGB(150, 255, 255, 255), true, false);

				sprintf_s(szBuff, "Kill Marked Enemies");
				m_pRenderer->RenderText(szBuff, 802, 592, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0,0,0));
				m_pRenderer->RenderText(szBuff, 800, 590, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(253, 81, 63));

				if (nNumYellowEnemies > 0)
				{
					sprintf_s(szBuff, "Seeker: %d", nNumYellowEnemies);
					D3DXMatrixTranslation(&m_d3dEnemyIconPos, 850.0f * xRatio, 628.0f * yRatio, 0.0f );
					m_pRenderer->RenderArrowAndrewR(m_nYellowEnemyIconTex, &m_rEnemyIconRect, m_d3dEnemyIconPos,D3DCOLOR_ARGB(255,255,255,255),true,false);
					m_pRenderer->RenderText(szBuff, 900, 628, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0,0,0));
					m_pRenderer->RenderText(szBuff, 898, 626, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(246, 207, 109));
				}

				if (nNumRedEnemies > 0)
				{
					sprintf_s(szBuff, "Cruiser: %d", nNumRedEnemies);
					D3DXMatrixTranslation(&m_d3dEnemyIconPos, 850.0f * xRatio, 665.0f * yRatio, 0.0f );
					m_pRenderer->RenderArrowAndrewR(m_nRedEnemyIconTex, &m_rEnemyIconRect, m_d3dEnemyIconPos,D3DCOLOR_ARGB(255,255,255,255),true,false);
					m_pRenderer->RenderText(szBuff, 902, 667, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0,0,0));
					m_pRenderer->RenderText(szBuff, 900, 665, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(253, 81, 63));
				}
				if (nNumBlueEnemies > 0)
				{
					sprintf_s(szBuff, "Marauder: %d", nNumBlueEnemies);
					D3DXMatrixTranslation(&m_d3dEnemyIconPos, 850.0f * xRatio, 705.0f * yRatio, 0.0f );
					m_pRenderer->RenderArrowAndrewR(m_nBlueEnemyIconTex, &m_rEnemyIconRect, m_d3dEnemyIconPos,D3DCOLOR_ARGB(255,255,255,255),true,false);
					m_pRenderer->RenderText(szBuff, 900, 707, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0,0,0));
					m_pRenderer->RenderText(szBuff, 898, 705, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(109, 207, 246));
				}

			}

			// 			sprintf_s(szBuff, "Current Knots: %d", (int)m_pPlayer->GetVelocityModifier());
			// 			m_pGame->GetRenderer()->RenderText(szBuff, 852, 692, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0));
			// 			m_pGame->GetRenderer()->RenderText(szBuff, 850, 690, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(240, 100, 50));
		}


		static bool _bCompleted = true;

		float SpawnTime = m_pScriptManager->GetSpawnTimer();

		if( SpawnTime > 0.0f && SpawnTime < 2.0f )
		{			
			char szBuff[100];
			if(m_bPlaySoundIncommingWave)
			{
				WwiseNS::StopAllTutorialVoices(eAIIncommingWave);
				WwiseNS::SoundEvent(eAIIncommingWave, ePlay);
				m_bPlaySoundIncommingWave = false;
				m_bPlaySoundWaveComplete = true;
			}
			sprintf_s(szBuff, "Ambush Detected");
			m_pGame->GetRenderer()->RenderText(szBuff, int(m_pGame->GetWidth() / 2.5f), 212, 1.1f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0,0,0), false);
			m_pGame->GetRenderer()->RenderText(szBuff, int(m_pGame->GetWidth() / 2.5f), 210, 1.1f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(215,60,60), false);
			m_bCompleted = false;

			if(m_fCompletedBucket > 0.45f)
			{		
				WwiseNS::SoundEvent(eCountDown, ePlay);
				m_fCompletedBucket = 0.0f;
			}
		}
		else if(m_pScriptManager->GetCurrentEnemies().size() == 0.0f && !m_bCompleted)
		{	
			//if(m_fCompletedBucket < 0.25f)
			if(m_bPlaySoundCheckpointArrival)
			{
				m_bPlaySoundCheckpointArrival = false;
				WwiseNS::SoundEvent(eCheckpoint, ePlay);
			}
			if( m_fCompletedBucket > 2.0f )
			{
				m_fCompletedBucket = 0.0f;
				m_bCompleted = true;
				m_bPlaySoundCheckpointArrival = true;
			}

			if(m_pScriptManager->GetCurrentWaypoint()->GetIsVisable())
			{
				if(m_pScriptManager->GetIsInTutorial() == false)
				{
					if(m_bPlaySoundWaveComplete)
					{
						WwiseNS::StopAllTutorialVoices(eAIWaveComplete);
						WwiseNS::SoundEvent(eAIWaveComplete, ePlay);
						m_bPlaySoundWaveComplete = false;
						m_bPlaySoundIncommingWave = true;
					}
					m_pGame->GetRenderer()->RenderText("Area Clear!", int(m_pGame->GetWidth() / 2.5f), 212, 1.1f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0,0,0), false);
					m_pGame->GetRenderer()->RenderText("Area Clear!", int(m_pGame->GetWidth() / 2.5f), 210, 1.1f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(200,128,255), false);
				}
			}
			else
			{
				if(m_pScriptManager->GetIsInTutorial() == false)
				{
					if(m_bPlaySoundWaveComplete)
					{
						WwiseNS::StopAllTutorialVoices(eAIWaveComplete);
						WwiseNS::SoundEvent(eAIWaveComplete, ePlay);
						m_bPlaySoundWaveComplete = false;
						m_bPlaySoundIncommingWave = true;
					}
					m_pGame->GetRenderer()->RenderText("Enemies Neutralized", int(m_pGame->GetWidth() / 2.5f), 212, 1.1f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0,0,0), false);
					m_pGame->GetRenderer()->RenderText("Enemies Neutralized", int(m_pGame->GetWidth() / 2.5f), 210, 1.1f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(100,100,255), false);
				}
			}
		}		
		else if (m_pScriptManager->GetCurrentEnemies().size() == 0 && m_pScriptManager->GetCurrentWaypoint()->GetIsVisable())
		{
			if(m_pScriptManager->GetAddedWaypoint())
			{
				m_pRenderer->RenderText("Proceed to Waypoint!", int(m_pGame->GetWidth() / 2.5f), 212,1.1f,0.0f,0.0f,0.0f,D3DCOLOR_XRGB(0,0,0), false);
				m_pRenderer->RenderText("Proceed to Waypoint!", int(m_pGame->GetWidth() / 2.5f), 210,1.1f,0.0f,0.0f,0.0f,D3DCOLOR_XRGB(0,255,0), false);
			}

			//	char szBuff[32];
			//	sprintf_s(szBuff, "Current Knots: %d", (int)(m_pPlayer->GetVelocityModifier()));
			//	m_pGame->GetRenderer()->RenderText(szBuff, 852, 692, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0));
			//	m_pGame->GetRenderer()->RenderText(szBuff, 850, 690, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(240, 100, 50));
		}

		if(m_bTooFar) 
		{
			m_pRenderer->RenderText("Warning: Leaving Battlefield!!!",127,142,1.0f,0.0f,0.0f,0.0f,D3DCOLOR_XRGB(0,0,0));
			m_pRenderer->RenderText("Warning: Leaving Battlefield!!!",125,140,1.0f,0.0f,0.0f,0.0f,D3DCOLOR_XRGB(255,255,0));

		}

		if(m_bTurnAround == true)
		{
			m_pRenderer->RenderText("Returning to the Battlefield!!!",127,142,1.0f,0.0f,0.0f,0.0f,D3DCOLOR_XRGB(0,0,0));
			m_pRenderer->RenderText("Returning to the Battlefield!!!",125,140,1.0f,0.0f,0.0f,0.0f,D3DCOLOR_XRGB(255,255,0));
		}

		/*		****************************		*/
		//if (m_pScriptManager->GetIsInTutorial() == false && !bossFight && m_bShowWaypoint)//m_pScriptManager->GetCurrentEnemies().size() == 0)
		//{
		//	m_pRenderer->RenderArrowAndrewR(m_nBlackTexture, &m_rBlack,m_DistanceBox, D3DCOLOR_ARGB(150, 255, 255, 255), true, false);
		//	char cWaypointDistance[100];
		//	sprintf_s(cWaypointDistance, "Waypoint: %i", (int)m_fDistanceFromWaypoint);
		//	m_pRenderer->RenderText(cWaypointDistance, int(m_pGame->GetWidth() - 185), 610, 0.75f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(70, 236, 70));
		//}

		if(m_bShowHealth == true || m_bShowShields == true)
		{
			if (!m_bFlashHealth)
			{
				m_pRenderer->RenderArrowAndrewR(m_pGame->GetBlackTexture(), &m_rBlack, m_HealthBox, D3DCOLOR_ARGB(140, 255, 255, 255), true, false);

			}
			else
			{
				m_pRenderer->RenderArrowAndrewR(m_nRedTexture, &m_rBlack, m_HealthBox, D3DCOLOR_ARGB(33, 255, 255, 255), true, false);
			}
		}
		int nPlayerHealth = m_pPlayer->GetHealth();
		if (nPlayerHealth <= 45 && nPlayerHealth != 0 )
		{
			m_bFlashHealth = true;
		}
		else 
		{
			m_bFlashHealth = false;
			m_nFlashAlpha = 255;
		}
// 		if(GetAsyncKeyState('E')&1)
// 		{
// 			m_pPlayer->SetHealth(nPlayerHealth - 1);
// 		}
		if (m_bFlashHealth)
		{
			if (whichWay)
			{
				if( nPlayerHealth != 0)
					m_nFlashAlpha += 300 / nPlayerHealth;
				if (m_nFlashAlpha >= 155)
				{
					m_nFlashAlpha = 155;
					whichWay = !whichWay;
				}
			}
			else
			{
				if( nPlayerHealth != 0)
					m_nFlashAlpha -= 300 / nPlayerHealth;
				if (m_nFlashAlpha <= 0)
				{
					m_nFlashAlpha = 0;
					whichWay = !whichWay;
				}
			}
		}

		if(m_bShowHealth == true)
		{
			char szMaxHealthBuff[50];
			sprintf_s(szMaxHealthBuff, "/%i", PLAYER_HEALTH);
			m_pRenderer->RenderText(szMaxHealthBuff, 162, 702, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0)); 
			m_pRenderer->RenderText(szMaxHealthBuff, 160, 700, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 70, 236, 70));
			char szRemainingHealthBuff[50];
			sprintf_s(szRemainingHealthBuff, "%i", m_pPlayer->GetHealth());
			m_pRenderer->RenderText(szRemainingHealthBuff, 107, 702, 1.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
			m_pRenderer->RenderText(szRemainingHealthBuff, 105, 700, 1.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 70, 236, 70));
		}

		char szMaxShieldBuff[50];

		if(m_bShowShields == true)
		{
			char szRemainingShieldBuff[50];

			if(m_pPlayer->GetType() == eRED_PLAYER)
			{
				sprintf_s(szMaxShieldBuff, "/%i", RED_STATE_SHIELD);
				m_pRenderer->RenderText(szMaxShieldBuff, 172, 657, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,0, 0, 0)); 
				m_pRenderer->RenderText(szMaxShieldBuff, 170, 655, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,109, 207, 246));
				sprintf_s(szRemainingShieldBuff, "%i", m_pPlayer->GetShields());
				m_pRenderer->RenderText(szRemainingShieldBuff, 107, 657, 1.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,0, 0, 0));
				m_pRenderer->RenderText(szRemainingShieldBuff, 105, 655, 1.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,109, 207, 246));	
			}
			else if(m_pPlayer->GetType() == eBLUE_PLAYER)
			{
				sprintf_s(szMaxShieldBuff, "/%i", BLUE_STATE_SHIELD);
				sprintf_s(szRemainingShieldBuff, "%i", m_pPlayer->GetShields());
				m_pRenderer->RenderText(szRemainingShieldBuff, 107, 657, 1.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,0, 0, 0));
				m_pRenderer->RenderText(szRemainingShieldBuff, 105, 655, 1.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,109, 207, 246));	

				if(m_pPlayer->GetShields() < 200)
				{
					m_pRenderer->RenderText(szMaxShieldBuff, 172, 657, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0)); 
					m_pRenderer->RenderText(szMaxShieldBuff, 170, 655, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 109, 207, 246));
				}
				else if(m_pPlayer->GetShields() == 200)
				{
					m_pRenderer->RenderText(szMaxShieldBuff, 187, 657, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0)); 
					m_pRenderer->RenderText(szMaxShieldBuff, 185, 655, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 109, 207, 246));
				}
			}
		}
		RECT rHealthIcon = {0,0,64,64};
		RECT rShieldIcon = {0,0,64,64};
		D3DXMATRIX d3dHealthMat;
		D3DXMATRIX d3dShieldMat;
		D3DXMatrixTranslation(&d3dHealthMat, 120.0f * xRatio, 1400.0f * yRatio, 0.0f);
		D3DXMatrixTranslation(&d3dShieldMat, 120.0f * xRatio, 1325.0f * yRatio, 0.0f);
		D3DXMATRIX d3dHealthScale;
		D3DXMATRIX d3dShieldScale;
		// 		D3DXMATRIX UIMat = m_BlackMat2;
		// 		UIMat._41 = 20;
		// 		UIMat._42 = 800;

		//m_pRenderer->RenderArrowAndrewR(BlackTexture, &m_rBlack, UIMat , D3DCOLOR_ARGB(170,255,255,255));

		D3DXMatrixScaling(&d3dHealthScale, 0.5f, 0.5f, 0.0f);
		D3DXMatrixScaling(&d3dShieldScale, 0.5f, 0.5f, 0.0f);
		m_pRenderer->RenderArrowAndrewR(m_pGame->GetHUDHealthIcon(), &rHealthIcon,  d3dHealthMat * d3dHealthScale,D3DCOLOR_ARGB(255,255,255,255),true,false);
		m_pRenderer->RenderArrowAndrewR(m_pGame->GetHUDShieldIcon(), &rShieldIcon,  d3dShieldMat * d3dShieldScale,D3DCOLOR_ARGB(255,255,255,255),true,false);



		if (m_pPlayer->GetType() == eRED_PLAYER)
		{
			m_pRenderer->RenderArrowAndrewR(m_nReticleTex, &m_rArrowRect,  m_ReticleMat,D3DCOLOR_ARGB(255, 255,0,0),true,false);
		}
		else
		{
			m_pRenderer->RenderArrowAndrewR(m_nReticleTex, &m_rArrowRect,  m_ReticleMat,D3DCOLOR_ARGB(255, 109, 207, 246),true,false);
		}

		//static bool s_bDodgeWell = false;
		//static bool s_bEnemyIsLockedOn = false;
		//static float s_fDodgeWellTimer = 0;
		//
		//if(s_bDodgeWell && s_fDodgeWellTimer < 3)
		//{
		//	s_fDodgeWellTimer += 0.01666666666667f;
		//	if(s_fDodgeWellTimer >= 3)
		//	{
		//		s_fDodgeWellTimer = 0;
		//		s_bDodgeWell = false;
		//	}
		//}

		if(m_bLockedOn == true)
		{
			if(m_bWithinRange == true)
			{
				D3DXMatrixTranslation(&m_d3dWarningMat, 602 * xRatio, 336 * yRatio,0);
				m_pRenderer->RenderArrowAndrewR(m_nLockOnWarningYellowTex, &m_rWarningRect, m_d3dWarningMat,D3DCOLOR_ARGB(255,255,255,255),true,false);
			}
			else
			{
				D3DXMatrixTranslation(&m_d3dWarningMat, 602 * xRatio, 336 * yRatio,0);
				m_pRenderer->RenderArrowAndrewR(m_nLockOnWarningRedTex, &m_rWarningRect, m_d3dWarningMat,D3DCOLOR_ARGB(255,255,255,255),true,false);
			}
		}

		if(m_bHasDodgedMissile == true)
		{
			D3DXMatrixTranslation(&m_d3dWarningMat, 602 * xRatio, 336 * yRatio,0);
			m_pRenderer->RenderArrowAndrewR(m_nLockOnNoWarningGreenTex, &m_rWarningRect, m_d3dWarningMat,D3DCOLOR_ARGB(255,255,255,255),true,false);
		}


		//if(m_pObjectManager->GetMissileList()->size())
		//{
		//	float fMissileDist = D3DXVec3Length(&((*m_pObjectManager->GetMissileList())[0]->GetPosition() - m_pPlayer->GetPosition()));
		//
		//	if(s_bEnemyIsLockedOn == true)
		//	{
		//		WwiseNS::SoundEvent(eLockOnWarning, ePlay);
		//		s_bEnemyIsLockedOn = false;
		//	}
		//
		//	if (s_bDodgeWell)
		//	{
		//		D3DXMatrixTranslation(&m_d3dWarningMat, 602, 336,0);
		//		m_pGame->GetRenderer()->RenderArrow(m_nLockOnWarningBlackTex, &m_rWarningRect, m_d3dWarningMat);
		//		D3DXMatrixTranslation(&m_d3dWarningMat, 600, 334,0);
		//		m_pGame->GetRenderer()->RenderArrow(m_nLockOnNoWarningGreenTex, &m_rWarningRect, m_d3dWarningMat);
		//	}
		//	else if(fMissileDist < 240 && fMissileDist > 80)
		//	{
		//		D3DXMatrixTranslation(&m_d3dWarningMat, 602, 336,0);
		//		m_pGame->GetRenderer()->RenderArrow(m_nLockOnWarningBlackTex, &m_rWarningRect, m_d3dWarningMat);
		//		D3DXMatrixTranslation(&m_d3dWarningMat, 600, 334,0);
		//		m_pGame->GetRenderer()->RenderArrow(m_nLockOnWarningYellowTex, &m_rWarningRect, m_d3dWarningMat);
		//
		//		if(m_pPlayer->GetHasDodged() > 0)
		//		{
		//			s_bDodgeWell = true;
		//		}
		//	}
		//	else
		//	{
		//		D3DXMatrixTranslation(&m_d3dWarningMat, 602, 336,0);
		//		m_pGame->GetRenderer()->RenderArrow(m_nLockOnWarningBlackTex, &m_rWarningRect, m_d3dWarningMat);
		//		D3DXMatrixTranslation(&m_d3dWarningMat, 600, 334,0);
		//		m_pGame->GetRenderer()->RenderArrow(m_nLockOnWarningRedTex, &m_rWarningRect, m_d3dWarningMat);
		//	}
		//}
		//else
		//{
		//	WwiseNS::SoundEvent(eLockOnWarning, eStop);
		//	s_bEnemyIsLockedOn = true;
		//	s_bDodgeWell = false;
		//}

		int tmpAlpha = 0;
		if(m_pPlayer->GetHurtRecently(0) < 1.0f)// top
		{
			tmpAlpha = 500 - (int)(m_pPlayer->GetHurtRecently(0) * 500.0f);
			(tmpAlpha > 255)? tmpAlpha = 255 : 0;
			m_pRenderer->RenderArrowAndrewR(m_nDamageDirectionHorizontal, &m_rHorizontalDamage, m_d3dTopSideDamage, D3DCOLOR_ARGB(tmpAlpha, 255, 255, 255),true,false);
			m_pCamera->Shake(1, 5, 1.0f);
		}

		if(m_pPlayer->GetHurtRecently(1) < 1.0f)// bot
		{
			tmpAlpha = 500 - (int)(m_pPlayer->GetHurtRecently(1) * 500.0f);
			(tmpAlpha > 255)? tmpAlpha = 255 : 0;
			m_pRenderer->RenderArrowAndrewR(m_nDamageDirectionHorizontal, &m_rHorizontalDamage, m_d3dBotSideDamage, D3DCOLOR_ARGB(tmpAlpha, 255, 255, 255),true,false);
			m_pCamera->Shake(1, 5, 1.0f);
		}

		if(m_pPlayer->GetHurtRecently(2) < 1.0f)// left
		{
			tmpAlpha = 500 - (int)(m_pPlayer->GetHurtRecently(2) * 500.0f);
			(tmpAlpha > 255)? tmpAlpha = 255 : 0;
			m_pRenderer->RenderArrowAndrewR(m_nDamageDirectionVertical, &m_rVerticalDamage, m_d3dLeftSideDamage, D3DCOLOR_ARGB(tmpAlpha, 255, 255, 255),true,false);
			m_pCamera->Shake(1, 5, 1.0f);
		}
		if(m_pPlayer->GetHurtRecently(3) < 1.0f)// right
		{
			tmpAlpha = 500 - (int)(m_pPlayer->GetHurtRecently(3) * 500.0f);
			(tmpAlpha > 255)? tmpAlpha = 255 : 0;
			m_pRenderer->RenderArrowAndrewR(m_nDamageDirectionVertical, &m_rVerticalDamage, m_d3dRightSideDamage, D3DCOLOR_ARGB(tmpAlpha, 255, 255, 255),true,false);
			m_pCamera->Shake(1, 5, 1.0f);
		}
		m_pGame->GetDialogSys()->Render();



	}
	else if( !m_pScriptManager->GetTheBoss()) 
	{
		static bool whichFade = false;
		if (whichFade)
		{
			m_fSkipTextAlpha -= 10;
			if (m_fSkipTextAlpha <= 0)
			{
				m_fSkipTextAlpha = 0;
				whichFade = !whichFade;
			}
		}
		else
		{
			m_fSkipTextAlpha += 10;
			if (m_fSkipTextAlpha >= 255)
			{
				m_fSkipTextAlpha = 255;
				whichFade = !whichFade;
			}
		}

		char szBuff[32];
		sprintf_s(szBuff, "Press SPACE to Skip");
		m_pRenderer->RenderText(szBuff, 739, SKIP_TEXT_Y_POSITION-1, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_fSkipTextAlpha,0,0,0));
		m_pRenderer->RenderText(szBuff, 740, SKIP_TEXT_Y_POSITION, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_fSkipTextAlpha,245,245,245));
		int nTeachxPos = m_pCinematicCamera->GetTeachXLocation();
		int nTransAlpha = m_pCinematicCamera->GetTransitionAlpha();
		if (m_bShowRedData)
		{
			if(m_bPlaySoundCruiserEntrance)
			{
				m_fPlaySoundBucketBuffer += 0.1f;
				if(m_fPlaySoundBucketBuffer > 10.0f)
				{
					WwiseNS::StopAllTutorialVoices(eAICruiserEntrance);
					WwiseNS::SoundEvent(eAICruiserEntrance, ePlay);
					m_bPlaySoundCruiserEntrance = false;
					m_fPlaySoundBucketBuffer = 0.0f;
				}
			}
			char szBuff[255];
			char szBuff2[255];
			char szBuff3[255];
			char szBuff4[255];
			char szBuff5[255];
			sprintf_s(szBuff2, "CRUISER");
			sprintf_s(szBuff3, "\n\n\n\n\n\nTIPS");
			sprintf_s(szBuff, "\nHealth   : Medium\nVelocity : High\nFire Rate: High\nDamage : Medium\n\n\n    -Cruiser Dodges             .\n    -Use            . \n");
			sprintf_s(szBuff4, "\n\n\n\n\n\n\n                                Missiles");
			sprintf_s(szBuff5, "\n\n\n\n\n\n\n\n            Lasers");
			m_pRenderer->RenderArrowAndrewR(m_pGame->GetBlackTexture(), &m_rBlack, m_BlackMat , D3DCOLOR_ARGB(nTransAlpha,255,255,255), true, false);
			m_ArrowMat._41+=2;
			m_ArrowMat._42 = 325;

			if (m_ArrowMat._41 >= 84)
			{
				m_ArrowMat._41 = 84;
			}

			//cout << "x: " << m_ArrowMat._41 << "y: " << m_ArrowMat._42 << endl;


			//m_pRenderer->RenderArrow(m_pGame->GetRedArrowTexture(), &m_rArrowRect,  m_ArrowMat);

			m_pRenderer->RenderText(szBuff2,nTeachxPos, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,0, 220, 220));
			m_pRenderer->RenderText(szBuff3, nTeachxPos, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,0, 220, 220));
			m_pRenderer->RenderText(szBuff, nTeachxPos, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,245,245,245));
			m_pRenderer->RenderText(szBuff4, nTeachxPos, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 128, 128, 255));
			m_pRenderer->RenderText(szBuff5, nTeachxPos, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 255, 64, 64));

		}
		else if (m_bShowBlueData)
		{
			if(m_bPlaySoundMarauderEntrance)
			{
				m_fPlaySoundBucketBuffer += 0.1f;
				if(m_fPlaySoundBucketBuffer > 10.0f)
				{
					WwiseNS::StopAllTutorialVoices(eAIMarauderEntrance);
					WwiseNS::SoundEvent(eAIMarauderEntrance, ePlay);
					m_bPlaySoundMarauderEntrance = false;
					m_fPlaySoundBucketBuffer = 0.0f;
				}
			}
			char szBuff[255];
			char szBuff2[255];
			char szBuff3[255];
			char szBuff4[255];
			char szBuff5[255];
			sprintf_s(szBuff2, "Marauder");
			sprintf_s(szBuff3, "\n\n\n\n\n\nTIPS");
			sprintf_s(szBuff, "\nHealth   : High\nVelocity : Very Low\nFire Rate: High\nDamage : Very High\n\n\n   -Destroy Shield With           .     \n   -Use              After Shield.");
			sprintf_s(szBuff4, "\n\n\n\n\n\n\n                                      Lasers");
			sprintf_s(szBuff5, "\n\n\n\n\n\n\n\n           Missiles ");

			m_pRenderer->RenderArrowAndrewR(m_pGame->GetBlackTexture(), &m_rBlack, m_BlackMat, D3DCOLOR_ARGB(nTransAlpha,255,255,255), true, false);


			m_ArrowMat._41+=2* xRatio;
			m_ArrowMat2._41+=2* xRatio;
			m_ArrowMat._42 = 325* yRatio;

			if (m_ArrowMat._41 >= 79 * xRatio)
			{
				m_ArrowMat._41 = 79* xRatio;
			}
			if (m_ArrowMat2._41 >= 79* yRatio)
			{
				m_ArrowMat2._41 = 79* yRatio;
			}
			//cout << "x: " << m_ArrowMat2._41 << "y: " << m_ArrowMat2._42 << endl;

			//	m_pRenderer->RenderArrow(m_pGame->GetBlueEnemyShieldIcon(), &m_rArrowRect,  m_ArrowMat2);

			m_ArrowMat._42 = 366 * yRatio;
			//	m_pRenderer->RenderArrow(m_pGame->GetBlueArrowTexture(), &m_rArrowRect,  m_ArrowMat);

			m_pRenderer->RenderText(szBuff2,nTeachxPos, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,0, 220, 220));
			m_pRenderer->RenderText(szBuff3, nTeachxPos, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,0, 220, 220));
			m_pRenderer->RenderText(szBuff, nTeachxPos, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,245,245,245));
			m_pRenderer->RenderText(szBuff5, nTeachxPos, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 128, 128, 255));
			m_pRenderer->RenderText(szBuff4, nTeachxPos, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 255, 64, 64));

		}
		else if (m_bShowYellowData)
		{

			if(m_bPlaySoundSeekerEntrance)
			{
				m_fPlaySoundBucketBuffer += 0.1f;
				if(m_fPlaySoundBucketBuffer > 13.0f)
				{
					WwiseNS::StopAllTutorialVoices(eAISeekerEntrance);
					WwiseNS::SoundEvent(eAISeekerEntrance, ePlay);
					m_bPlaySoundSeekerEntrance = false;
					m_fPlaySoundBucketBuffer = 0.0f;
				}
			}
			char szBuff[255];
			char szBuff2[255];
			char szBuff3[255];
			sprintf_s(szBuff2, "Seeker");
			sprintf_s(szBuff3, "\n\n\n\n\n\nTIPS");
			sprintf_s(szBuff, "\nHealth: Low\nVelocity: Medium\nFire Rate: Medium\nDamage: Low\n\n\n  -Seekers are Deadly in\n     packs.\n  -Seekers are Easily Destroyed.");
			m_pRenderer->RenderArrowAndrewR(m_pGame->GetBlackTexture(), &m_rBlack, m_BlackMat , D3DCOLOR_ARGB(nTransAlpha,255,255,255), true, false);
			m_pRenderer->RenderText(szBuff2,nTeachxPos, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,0, 220, 220));
			m_pRenderer->RenderText(szBuff3, nTeachxPos, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,0,220,220));
			m_pRenderer->RenderText(szBuff, nTeachxPos, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,245,245,245));
		}
	}

	if(CInput::ButtonHeld(eLeftArrow))
	{
		char tmp[20];
		sprintf_s(tmp, "%d", m_pGame->GetTimer()->GetFPS());
		m_pRenderer->RenderText(tmp, 50, 50, 1.0f, 0, 0, 0, D3DCOLOR_ARGB(255,255,255,240));
	}
}

/*****************************************************************
* EnemyLocationArrow(): Function that calculates the placement of the waypoint arrows
* Ins:					none    		      
* Outs:					None
* Returns:				void
* Mod. Date:			12/06/2012
* Mod. Initials:		AG
*****************************************************************/
void GamePlayState::EnemyLocationArrow()
{
	//project the two points, store in vector, normalize, dot.

	std::vector<CEntity*> vEnemyList =  *m_pObjectManager->GetIndicatorList();
	std::vector<CEntity*>::iterator iIndicator = vEnemyList.begin();

	D3DXMATRIX d3dCameraWorldMatrix = *m_pCamera->GetViewMatrix();
	D3DXVECTOR3 CameraPosition(CPhysics::GetMat4Vec(ePOS, &d3dCameraWorldMatrix));
	D3DXVECTOR2 CamPos;

	D3DXMATRIX d3dCameraInverse;
	D3DXMatrixInverse( &d3dCameraInverse, 0, &d3dCameraWorldMatrix);

	IDirect3DSurface9 *sOldDS = 0;
	D3DSURFACE_DESC desc;

	m_pRenderer->GetDevice()->GetRenderTarget(0, &sOldDS);
	sOldDS->GetDesc(&desc);

	D3DVIEWPORT9 d3dViewPort;
	m_pRenderer->GetDevice()->GetViewport(&d3dViewPort);

	D3DXMATRIX d3dIdentity;
	D3DXMatrixIdentity(&d3dIdentity);		

	D3DXMATRIX d3dBlueEnemyShieldMat;
	D3DXMatrixIdentity(&d3dBlueEnemyShieldMat);
	char szDistanceFromPlayerBuffer[100];

	float fDistanceFromPlayer;
	int nDistanceAlpha;

	for( ;iIndicator != vEnemyList.end(); ++iIndicator)
	{
		CEntity * pIndicator = (*iIndicator);
		if(pIndicator->GetIsVisable() == false  )
		{
			continue;
		}

		D3DXMATRIX d3dEnemyMat = *pIndicator->GetMatrix();
		D3DXVECTOR2 d3dPosition;
		D3DXVECTOR3 d3dOut;
		D3DXVECTOR3 d3dEnemyPos = CPhysics::GetMat4Vec(ePOS, &d3dEnemyMat);

		D3DXVec3Project(&d3dOut, &d3dEnemyPos, &d3dViewPort, m_pCamera->GetProjectionMatrix(), &d3dCameraInverse, &d3dIdentity);
		d3dPosition.x = d3dOut.x;
		d3dPosition.y = d3dOut.y;

		D3DXVECTOR3 d3dOut2;
		D3DXVec3Project(&d3dOut2, &CPhysics::GetMat4Vec(ePOS, &m_TargetMat), &d3dViewPort, m_pCamera->GetProjectionMatrix(), &d3dCameraInverse, &d3dIdentity);
		CamPos.x = d3dOut2.x;
		CamPos.y = d3dOut2.y;

		D3DXVECTOR2 d3dPos(d3dPosition);
		D3DXVECTOR2 d3dBlueEnemyShieldVec(d3dPosition);
		float fDiffX = d3dPos.x - CamPos.x;
		float fDiffY = d3dPos.y - CamPos.y;		

		float angle = atan(fDiffY/fDiffX) * 180.0f/3.14f;
		D3DXVECTOR3 d3dVectorBetween = d3dEnemyPos - CameraPosition;
		D3DXVec3Normalize(&d3dVectorBetween, &d3dVectorBetween);

		D3DXMATRIX d3dRot;
		D3DXMatrixIdentity(&m_d3dArrow);
		D3DXMatrixIdentity(&d3dRot);

		//If the Enemy is in front of the player, just make sure everything is within the boundry
		float fInsideFrustumDotResult = D3DXVec3Dot(&d3dVectorBetween, &CPhysics::GetMat4Vec(eZAXIS, &d3dCameraWorldMatrix));
		float fDotLeftOrRight = D3DXVec3Dot(&d3dVectorBetween, &CPhysics::GetMat4Vec(eXAXIS, &d3dCameraWorldMatrix));

		if(fDotLeftOrRight <= 0.0f)
		{
			angle += 180.0f;
		}

		D3DXMatrixRotationZ(&d3dRot, D3DXToRadian(angle));

		float xRatio = m_pGame->GetRenderer()->GetXRatio();
		float yRatio = m_pGame->GetRenderer()->GetYRatio();

		int indicatorType = pIndicator->GetType();

		if(fInsideFrustumDotResult > 0.68f)
		{
			//if(d3dPosition.x > 1000.0f * xRatio)
			//{
			//	if(indicatorType != eRED_ENEMY)
			//	{
			//		d3dPosition.x = 1000.0f * xRatio;
			//	}
			//}
			//else if(d3dPosition.x < 25.0f * xRatio)
			//{
			//	if(indicatorType != eRED_ENEMY)
			//	{
			//		d3dPosition.x = 25.0f * xRatio;
			//	}
			//}
			//if(d3dPosition.y > 732.0f  * yRatio)
			//{
			//	if(indicatorType != eRED_ENEMY)
			//	{
			//		d3dPosition.y = 732.0f * yRatio;
			//	}
			//}
			//else if(d3dPosition.y < 32.0f * yRatio)
			//{
			//	if(indicatorType != eRED_ENEMY)
			//	{
			//		d3dPosition.y = 32.0f * yRatio;
			//	}
			//}
			//if(d3dPosition.x < 1000.0f * xRatio && d3dPosition.x > 25.0f * xRatio && d3dPosition.y * yRatio > 32.0f && d3dPosition.y < 732.0f * yRatio)
			//{

			//D3DXMatrixIdentity(&d3dRot);
			//D3DXMatrixRotationZ(&d3dRot, D3DXToRadian(90));

			if(indicatorType == eBLUE_ENEMY)
			{
				d3dBlueEnemyShieldVec.y -= 40;
			}


			m_bRenderIndicatorBox = true;
			//}
		}
		else// if the enemy is behind the player, lock the waypoint to the side so player can turn to it
		{
			m_bRenderIndicatorBox = false;

			float fLeftRightDotResult = D3DXVec3Dot(&d3dVectorBetween, &CPhysics::GetMat4Vec(eXAXIS, &d3dCameraWorldMatrix));
			float fTopBottomDotResult = D3DXVec3Dot(&d3dVectorBetween, &CPhysics::GetMat4Vec(eYAXIS, &d3dCameraWorldMatrix));

			if(fInsideFrustumDotResult < 0)
			{
				if(fLeftRightDotResult > 0)
				{
					d3dPosition.x = 1000 * xRatio;
					d3dBlueEnemyShieldVec.x = 1000 * xRatio;
				}
				else
				{
					d3dPosition.x = 32;
					d3dBlueEnemyShieldVec.x = 32;
				}

				if(fTopBottomDotResult > 0)
				{
					d3dPosition.y = 32;
					d3dBlueEnemyShieldVec.y = 32;
				}
				else
				{
					d3dPosition.y = 736 * yRatio;
					d3dBlueEnemyShieldVec.y = 736 * yRatio;
				}
			}
			else
			{
				if(fLeftRightDotResult > 0)
				{
					if(d3dPosition.x > 1000)
					{
						d3dPosition.x = 1000 * xRatio;
						d3dBlueEnemyShieldVec.x = 1000 * xRatio;
					}
				}
				else
				{
					if(d3dPosition.x < 32)
					{
						d3dPosition.x = 32;
						d3dBlueEnemyShieldVec.x = 32;
					}
				}

				if(fTopBottomDotResult < 0)
				{
					if(d3dPosition.y > 736)
					{
						d3dPosition.y = 736 * yRatio;
						d3dBlueEnemyShieldVec.y = 736 * yRatio;
					}
				}
				else
				{
					if(d3dPosition.y < 32)
					{
						d3dPosition.y = 32;
						d3dBlueEnemyShieldVec.y = 32;
					}
				}
			}
			//if(d3dPosition.y > 700)
			//{
			//	d3dPosition.y = 700;
			//}
			//else if(d3dPosition.y < 64)
			//{
			//	d3dPosition.y = 64;
			//}

			//if(d3dPosition.x > 1000.0f * xRatio)
			//{
			//	d3dPosition.x = 1000.0f * xRatio;
			//}
			//else if(d3dPosition.x < 25.0f * xRatio)
			//{
			//	d3dPosition.x = 25.0f * xRatio;
			//}
			//if(d3dPosition.y > 732.0f * yRatio)
			//{
			//	d3dPosition.y = 732.0f * yRatio;
			//}
			//else if(d3dPosition.y < 32.0f * yRatio)
			//{
			//	d3dPosition.y = 32.0f * yRatio;
			//}
			//
			//if(fDiffY >= fDiffX)
			//{
			//	if(d3dPosition.y >= 366.0f * yRatio)
			//	{
			//		d3dPosition.y = 32.0f * yRatio;
			//	}
			//	else if(d3dPosition.y < 366.0f * yRatio)
			//	{
			//		d3dPosition.y = 732.0f * yRatio;
			//	}
			//}
			//else
			//{
			//	if(d3dPosition.x <= 500.0f * xRatio)
			//	{
			//		d3dPosition.x = 1000.0f * xRatio;
			//	}
			//	else if(d3dPosition.x > 500.0f * xRatio)
			//	{
			//		d3dPosition.x = 25.0f * xRatio;
			//	}	
			//}
			////Correction for corners
			//if(d3dPosition.y < 32.1f * yRatio)
			//{
			//	if(d3dPosition.x > 999.9f * xRatio)
			//	{
			//		angle = -45.0f;
			//		D3DXMatrixIdentity(&d3dRot);
			//		D3DXMatrixRotationZ(&d3dRot, D3DXToRadian(angle));
			//	}
			//	else if(d3dPosition.x < 25.1f * xRatio)
			//	{
			//		angle = 225.0f;
			//		D3DXMatrixIdentity(&d3dRot);
			//		D3DXMatrixRotationZ(&d3dRot, D3DXToRadian(angle));
			//	}
			//}
			//else if(d3dPosition.y > 731.9f * yRatio)
			//{
			//	if(d3dPosition.x > 999.9f * xRatio)
			//	{
			//		angle = 45.0f;
			//		D3DXMatrixIdentity(&d3dRot);
			//		D3DXMatrixRotationZ(&d3dRot, D3DXToRadian(angle));
			//	}
			//	else if(d3dPosition.x < 25.1f * xRatio)
			//	{
			//		angle = 135.0f;
			//		D3DXMatrixIdentity(&d3dRot);
			//		D3DXMatrixRotationZ(&d3dRot, D3DXToRadian(angle));
			//	}
			//}
		}



		if (indicatorType == eBLUE_ENEMY && ((CBlueEnemy*)pIndicator)->GetShields() > 0)
		{
			D3DXMATRIX scaleMat;
			D3DXMatrixScaling(&scaleMat, .80f,.80f,.80f);
			d3dBlueEnemyShieldMat = d3dBlueEnemyShieldMat * scaleMat;		
			d3dBlueEnemyShieldMat._41 = d3dBlueEnemyShieldVec.x;
			d3dBlueEnemyShieldMat._42 = d3dBlueEnemyShieldVec.y - 6;

			m_d3dArrow = m_d3dArrow * scaleMat;		
			m_d3dArrow._41 = d3dPosition.x;
			m_d3dArrow._42 = d3dPosition.y;
		}
		else
		{
			//if(indicatorType != eRED_ENEMY)
			//{
			if(m_bRenderIndicatorBox == false)
			{
				m_d3dArrow = m_d3dArrow * d3dRot;		
				m_d3dArrow._41 = d3dPosition.x;
				m_d3dArrow._42 = d3dPosition.y;
			}
			else
			{
				//m_d3dArrow = m_d3dArrow;		
				m_d3dArrow._41 = d3dPosition.x;
				m_d3dArrow._42 = d3dPosition.y;
			}
			//}
		}

		fDistanceFromPlayer = pIndicator->GetDistanceFromPlayer();
		sprintf_s(szDistanceFromPlayerBuffer, "%i", int(fDistanceFromPlayer));

		if(fDistanceFromPlayer <= 400)
		{
			nDistanceAlpha = (int)((255 * fDistanceFromPlayer) / 400);
		}
		else
		{
			nDistanceAlpha = 255;
		}

		if(indicatorType == eWAYPOINT)
		{
			if(m_bShowWaypoint)
			{
				if(m_bRenderIndicatorBox == false)
				{
					m_pRenderer->RenderArrow(m_pGame->GetGreenArrowTexture(), &m_rArrowRect,  m_d3dArrow);
				}
				else
				{
					m_pGame->GetRenderer()->RenderText(szDistanceFromPlayerBuffer, int(d3dPosition.x + 38), int(d3dPosition.y - 19), 0.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(nDistanceAlpha, 70, 236, 70), false);
					m_pRenderer->RenderArrowAndrewR(m_pGame->GetGreenBoxTexture(), &m_rArrowRect,  m_d3dArrow, D3DCOLOR_ARGB(nDistanceAlpha, 255, 255, 255), true, false);
				}
			}
		}
		else if(indicatorType == eRED_ENEMY && m_pScriptManager->GetSpawnBoss() == false)
		{
			if(m_bRenderIndicatorBox == false)
			{
				m_pRenderer->RenderArrow(m_pGame->GetRedArrowTexture(), &m_rArrowRect,  m_d3dArrow);
			}
			else
			{
				m_pGame->GetRenderer()->RenderText(szDistanceFromPlayerBuffer, int(d3dPosition.x + 38), int(d3dPosition.y - 19), 0.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(nDistanceAlpha, 215, 100, 100), false);
				m_pRenderer->RenderArrowAndrewR(m_pGame->GetRedBoxTexture(), &m_rArrowRect,  m_d3dArrow, D3DCOLOR_ARGB(nDistanceAlpha, 255, 255, 255), true, false);
			}
		}
		else if(indicatorType == eBLUE_ENEMY && m_pScriptManager->GetSpawnBoss() == false)
		{
			if ( ((CBlueEnemy*)pIndicator)->GetShields() > 0 )
			{
				m_pRenderer->RenderArrow(m_pGame->GetBlueEnemyShieldIcon(), &m_rArrowRect,  d3dBlueEnemyShieldMat);

				if(m_bRenderIndicatorBox == true)
				{
					m_pRenderer->RenderArrowAndrewR(m_pGame->GetRedBoxTexture(), &m_rArrowRect,  m_d3dArrow, D3DCOLOR_ARGB(nDistanceAlpha, 255, 255, 255), true, false);
					m_pGame->GetRenderer()->RenderText(szDistanceFromPlayerBuffer, int(d3dPosition.x + 38), int(d3dPosition.y - 19), 0.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(nDistanceAlpha, 215, 100, 100), false);
				}
			}
			else
			{
				if(m_bRenderIndicatorBox == false)
				{
					m_pRenderer->RenderArrow(m_pGame->GetBlueArrowTexture(), &m_rArrowRect,  m_d3dArrow);
				}
				else
				{
					m_pRenderer->RenderArrowAndrewR(m_pGame->GetBlueBoxTexture(), &m_rArrowRect,  m_d3dArrow, D3DCOLOR_ARGB(nDistanceAlpha, 255, 255, 255), true, false);
					m_pGame->GetRenderer()->RenderText(szDistanceFromPlayerBuffer, int(d3dPosition.x + 38), int(d3dPosition.y - 19), 0.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(nDistanceAlpha, 109, 207, 246), false);
				}
			}
		}
		else if(indicatorType == eHanger  && m_pScriptManager->GetSpawnBoss() == true)
		{
			if ( ((CEnemy*)pIndicator)->GetShields() > 0 )
			{
				m_ArrowMat2._41 = m_d3dArrow._41;
				m_ArrowMat2._42 = m_d3dArrow._42 - 5;
				if ( ((CBossWinTurret*)pIndicator)->GetCorrectSide() )
				{
					m_pRenderer->RenderArrowAndrewR(m_pGame->GetBlueEnemyShieldIcon(), &m_rArrowRect,  m_ArrowMat2, D3DCOLOR_ARGB(255, 255,255,255), true, false);
				}
				else
				{
					m_pRenderer->RenderArrowAndrewR(m_pGame->GetBlueEnemyShieldIcon(), &m_rArrowRect,  m_ArrowMat2, D3DCOLOR_ARGB(100, 255,255,255), true, false);
				}
			}
			else
			{
				if ( ((CBossWinTurret*)pIndicator)->GetCorrectSide() )
				{
					if(m_bRenderIndicatorBox == false)
					{
						m_pRenderer->RenderArrowAndrewR(m_pGame->GetBlueArrowTexture(), &m_rArrowRect,  m_d3dArrow, D3DCOLOR_ARGB(255, 255,255,255), true, false);
					}
					else
					{
						m_pRenderer->RenderArrowAndrewR(m_pGame->GetBlueBoxTexture(), &m_rArrowRect,  m_d3dArrow, D3DCOLOR_ARGB(255, 255,255,255), true, false);
						m_pGame->GetRenderer()->RenderText(szDistanceFromPlayerBuffer, int(d3dPosition.x + 38), int(d3dPosition.y - 19), 0.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(nDistanceAlpha, 109, 207, 246), false);
					}
				}
				else
				{
					if(m_bRenderIndicatorBox == false)
					{
						m_pRenderer->RenderArrowAndrewR(m_pGame->GetBlueArrowTexture(), &m_rArrowRect,  m_d3dArrow, D3DCOLOR_ARGB(100, 255,255,255), true, false);
					}
					else
					{
						m_pRenderer->RenderArrowAndrewR(m_pGame->GetBlueBoxTexture(), &m_rArrowRect,  m_d3dArrow, D3DCOLOR_ARGB(255, 255,255,255), true, false);
						m_pGame->GetRenderer()->RenderText(szDistanceFromPlayerBuffer, int(d3dPosition.x + 38), int(d3dPosition.y - 19), 0.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(nDistanceAlpha, 109, 207, 246), false);
					}
				}
			}
		}
		else if(indicatorType == eGENERATOR && m_pScriptManager->GetSpawnBoss() == true)
		{
			if(m_bRenderIndicatorBox == false)
			{
				m_pRenderer->RenderArrow(m_pGame->GetRedArrowTexture(), &m_rArrowRect,  m_d3dArrow);
			}
			else
			{
				m_pRenderer->RenderArrowAndrewR(m_pGame->GetRedBoxTexture(), &m_rArrowRect,  m_d3dArrow, D3DCOLOR_ARGB(nDistanceAlpha, 255, 255, 255), true, false);
				m_pGame->GetRenderer()->RenderText(szDistanceFromPlayerBuffer, int(d3dPosition.x + 38), int(d3dPosition.y - 19), 0.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(nDistanceAlpha, 246, 60, 60), false);
			}
		}		
		else if(indicatorType == eYELLOW_ENEMY && (((CYellowEnemy*)pIndicator)->GetNeedAnArrow() == true || m_pScriptManager->GetIsInTutorial() == true)) 
		{
			if(m_bRenderIndicatorBox == false)
			{
				m_pRenderer->RenderArrow(m_pGame->GetYellowArrowTexture(), &m_rArrowRect,  m_d3dArrow);
			}
			else
			{
				m_pRenderer->RenderArrowAndrewR(m_pGame->GetYellowBoxTexture(), &m_rArrowRect,  m_d3dArrow, D3DCOLOR_ARGB(nDistanceAlpha, 255, 255, 255), true, false);
				m_pGame->GetRenderer()->RenderText(szDistanceFromPlayerBuffer, int(d3dPosition.x + 38), int(d3dPosition.y - 19), 0.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(nDistanceAlpha, 255, 255, 83), false);
			}
		}
		else if(indicatorType == eTURRET_LASER && m_pScriptManager->GetSpawnBoss() == true)
		{
			if (  !((CBossLargerTurret*)pIndicator)->GetCorrectSide())
			{
				if(m_bRenderIndicatorBox == false)
				{
					m_pRenderer->RenderArrowAndrewR(m_pGame->GetRedArrowTexture(), &m_rArrowRect,  m_d3dArrow, D3DCOLOR_ARGB(100, 255,255,255), true, false);
				}
				else
				{
					m_pRenderer->RenderArrowAndrewR(m_pGame->GetRedBoxTexture(), &m_rArrowRect,  m_d3dArrow, D3DCOLOR_ARGB(100, 255,255,255), true, false);
					m_pGame->GetRenderer()->RenderText(szDistanceFromPlayerBuffer, int(d3dPosition.x + 38), int(d3dPosition.y - 19), 0.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(nDistanceAlpha, 215, 100, 100), false);
				}

			}
			else
			{
				if(m_bRenderIndicatorBox == false)
				{
					m_pRenderer->RenderArrowAndrewR(m_pGame->GetRedArrowTexture(), &m_rArrowRect,  m_d3dArrow,  D3DCOLOR_ARGB(255, 255,255,255), true, false);
				}
				else
				{
					m_pRenderer->RenderArrowAndrewR(m_pGame->GetRedBoxTexture(), &m_rArrowRect,  m_d3dArrow,  D3DCOLOR_ARGB(255, 255,255,255), true, false);
					m_pGame->GetRenderer()->RenderText(szDistanceFromPlayerBuffer, int(d3dPosition.x + 38), int(d3dPosition.y - 19), 0.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(nDistanceAlpha, 215, 100, 100), false);
				}
			}
		}

		//if(fDotResult < -.95f && indicatorType != eWAYPOINT && !bossFight)
		//{
		//	D3DXVECTOR3 d3dZVector = CameraPosition - d3dEnemyPos;
		//	D3DXVec3Normalize(&d3dZVector, &d3dZVector);
		//
		//	float fZDotResult = D3DXVec3Dot(&d3dZVector, &CPhysics::GetMat4Vec(eZAXIS, pIndicator->GetMatrix()));
		//
		//	if(fDotResult <= -0.75 && fZDotResult >= 0.75)
		//	{
		//		//char szEnemyBehind[100];
		//		//sprintf_s(szEnemyBehind, "Bogey on your six!!!");
		//		//m_pRenderer->RenderText(szEnemyBehind, 412, 432, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0));
		//		//m_pRenderer->RenderText(szEnemyBehind, 410, 430, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 255));
		//
		//		if(m_fBehindSound > 5.0f)
		//		{
		//			WwiseNS::SoundEvent(eOnYourSix, ePlay);
		//			m_fBehindSound = 0.0f;
		//		}
		//
		//	}
		//
		//}
		//D3DXVECTOR3 d3dZVector = CameraPosition - d3dEnemyPos;
		//float sqDistance = D3DXVec3Length(&d3dZVector);
		//if (fDotResult <= -0.75 &&indicatorType != eWAYPOINT && sqDistance > 275) //check squared ditance and force to chase if too far away and behind maybe
		//{
		//	((CEnemy*)pIndicator)->SetTurnRate(s_AIHelper->GetredBehindTurnRate());
		//	if(((CRedEnemy*)pIndicator)->GetMovementBehavior())
		//	{
		//		//((CRedEnemy*)(*iIndicator))->GetMovementBehavior()->SetForceChasebool(true);
		//	}
		//}
		//else
		//{
		//	((CEnemy*)pIndicator)->SetTurnRate(s_AIHelper->GetredTurnRate());
		//	if(((CRedEnemy*)pIndicator)->GetMovementBehavior())
		//	{
		//		//B((CRedEnemy*)(*iIndicator))->GetMovementBehavior()->SetForceChasebool(false);
		//	}
		//}
	}
}

void GamePlayState::EnemyLockOnIcon()
{
	if (m_fLockOnTimer > 0)
	{
		D3DXMATRIX d3dCameraInverse;
		D3DXMatrixInverse( &d3dCameraInverse, 0, m_pCamera->GetViewMatrix( ) );

		IDirect3DSurface9 *sOldDS = 0;
		D3DSURFACE_DESC desc;

		m_pRenderer->GetDevice()->GetRenderTarget(0, &sOldDS);
		sOldDS->GetDesc(&desc);

		D3DVIEWPORT9 d3dViewPort;
		m_pRenderer->GetDevice()->GetViewport(&d3dViewPort);

		D3DXMATRIX d3dIdentity;
		D3DXMatrixIdentity(&d3dIdentity);


		D3DXMATRIX d3dEnemyMat = m_pPlayer->GetTarget()->GetMatrixCopy();
		D3DXVECTOR2 d3dPosition;
		D3DXVECTOR3 d3dOut;
		D3DXVECTOR3 d3dEnemyPos = CPhysics::GetMat4Vec(ePOS, &d3dEnemyMat);

		D3DXVec3Project(&d3dOut, &d3dEnemyPos, &d3dViewPort, m_pCamera->GetProjectionMatrix(), &d3dCameraInverse, &d3dIdentity);

		d3dPosition.x = d3dOut.x;
		d3dPosition.y = d3dOut.y;

		D3DXMATRIX d3dRot, d3dScale;

		D3DXMatrixIdentity(&m_d3dArrow);
		D3DXMatrixIdentity(&d3dRot);
		D3DXMatrixIdentity(&d3dScale);

		float fTmpLen = D3DXVec3Length(&(m_pPlayer->GetPosition() - d3dEnemyPos));
		if(fTmpLen > 50)
			D3DXMatrixScaling(&d3dScale, 3 - 2*m_fLockOnTimer, 3 - 2*m_fLockOnTimer, 1);
		else
			D3DXMatrixScaling(&d3dScale, (3 - 2*m_fLockOnTimer)*(6 - 0.1f*fTmpLen), (3 - 2*m_fLockOnTimer)*(6 - 0.1f*fTmpLen), 1);

		D3DXMatrixRotationZ(&d3dRot, m_fLockOnTimer * 90);

		m_d3dArrow = m_d3dArrow * d3dRot * d3dScale;		

		m_d3dArrow._41 = d3dPosition.x;
		m_d3dArrow._42 = d3dPosition.y;

		if(m_pPlayer->GetTarget()->GetType() == eBLUE_ENEMY)
		{
			m_fTimeToLockOn = 0.4f;
		}
		else
		{
			m_fTimeToLockOn = 0.8f;
		}

		if(m_fLockOnTimer > m_fTimeToLockOn)
			m_pRenderer->RenderArrow(m_nRedLockOnTex, &m_rArrowRect,  m_d3dArrow);
		else
			m_pRenderer->RenderArrow(m_nYellowLockOnTex, &m_rArrowRect,  m_d3dArrow);

	}
}
