/***********************************************
* Filename:  		Boss.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This is what will contain the 
*					boss enemys personal data.				
************************************************/
#include "../StdAfx.h"
#include "Boss.h"
#include "Player.h"
#include "../Entity/Generator.h"
#include "../Object Manager/ObjectManager.h"
#include "../Entity/BossHanger.h"
#include "../Entity/BossWinTurret.h"
#include "../Entity/BossThruster.h"

/*****************************************************************
* CBoss():  Default Constructor. Will set its type and behaviors.
* Ins:			    None    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CBoss::CBoss(CPlayer * _pPlayer,CObjectManager * _objectManager , CRenderer * _pRenderer, D3DXVECTOR3 _pos, CEnvironmentObject * _pPlanet, CEnvironmentObject* _pLight1, CEnvironmentObject* _pLight2, CEnvironmentObject* _pLight3, CEnvironmentObject* _pLight4)
	: m_nPhase(eCINEMATIC_PHASE), m_bCanFire(false), m_fmissileTimer(0), m_fSeekTimer(0),  m_nNumGenerators(3), m_nShieldStatus(eSHIELDS_UP),
	m_bIsDisabled(false), numAttachments(0), m_nNumHangers(0), m_nNumTurrets(0), m_pAsteroidTarget(nullptr), m_fBattleTimer(300.0f), m_bTeachBoss(false), xPos(-100), xPoScrolls(-200)
{
	m_bBigExplosion = false;
	transitionTimer = 0.0f;
	transitionTimer2 = 0.0f;
	m_bPlayedBigExplosion = false;
	float xRatio = _pRenderer->GetXRatio();
	float yRatio = _pRenderer->GetYRatio();
	m_fDeathWaitTime = 0.0f;
	m_bDrawWarning = false;
	m_bFirst = true;
	m_fSkipTextAlpha = 255;
	whichFade = true;
	alphaTeach = 5;
	BlackTexture	=	_pRenderer->GetAssetManager()->LoadTexture("black.png");
	m_rBlack.left = 0;
	m_rBlack.top = 0;
	m_rBlack.right = 75;
	m_rBlack.bottom = 60;
	m_tTimerPosition.x  = 820;
	m_tTimerPosition.y = 700;
	m_fseconds = 0.0f;
	m_fTimerLowScale = 0.0f;
	DisplayteachPhase1= false;
	DisplayteachPhase2= false;
	DisplayteachPhase3= false;
	m_bWarped = false;
	
	m_bIsOnTop = false;
	m_bIsOnLeft = false;
	m_bIsOnRight = false;

	D3DXMatrixIdentity(&m_BlackMat);
	D3DXMATRIX scale;
	D3DXMatrixScaling(&scale, 4.2f * xRatio, 3.6f * yRatio, 2.0f);
	m_BlackMat = m_BlackMat * scale;
	m_BlackMat._41 = 138 * xRatio;
	m_BlackMat._42 = 565 * yRatio;


	m_bNewPhase = true;
	teachPhase1 = false;
	teachPhase2 = false;
	teachPhase3 = false;
	startBoss = false;
	m_bReachedZScale = false;
	m_bTransitionPhaseText1To2 = false;
	m_bTransitionPhaseText2To3 = false;
	m_fPhaseTransitionTimer = 0.0f;
	m_bPlayerLose = false;
	alpha = 255;
	m_fScaleTransition = 3.0f;
	m_fTranslationTransition = D3DXVECTOR3(352,50,0);
	m_fShadowTranslationTransition = D3DXVECTOR3(350,50,0 );
	generatorHealth = 0;
	m_nMaxHealth = 0;
	m_bMaxHealth = true;
	m_nNumMissilesFired = 0;
	SetActive(true);
	SetType(eBOSS);
	SetShields(0);
	SetDamage(500);
	SetVelocityModifier(10);
	m_pTarget = _pPlayer;
	//SetBV(CCollOBJ::Create(eAABB, D3DXVECTOR3(-300,-100,-633), D3DXVECTOR3(300,100,965)/*, 230.0f*/));
// 	SetBV(CCollOBJ::Create(eCAP, D3DXVECTOR3(0,-12,-283), D3DXVECTOR3(0,-12,425), 180.0f));
// 	m_pShieldBV = CCollOBJ::Create(eCAP, D3DXVECTOR3(0,0,-603), D3DXVECTOR3(0,0,625), 320.0f);

	SetBV(CCollOBJ::Create(eSPH,  D3DXVECTOR3(0,-12,425), 0.0f));
	m_pShieldBV = CCollOBJ::Create(eSPH, D3DXVECTOR3(0,0,625), 0.0f);
	m_pObjectManager = _objectManager;

	CreateCollisionSpheres();

	
	m_pPlayer = _pPlayer;
	m_pRenderer = _pRenderer;
	BossHealthTexture	=	m_pRenderer->GetAssetManager()->LoadTexture("RT_2D_HealthIcon#1.png");

	GeneratorTexture = m_pRenderer->GetAssetManager()->LoadTexture("Generator.png");
	TurretTexture = m_pRenderer->GetAssetManager()->LoadTexture("Turret.png");
	BigTurretTexture = m_pRenderer->GetAssetManager()->LoadTexture("BossTurret.png");

	m_rAttachmentRect.left = 0;
	m_rAttachmentRect.top = 0;
	m_rAttachmentRect.right = 64;
	m_rAttachmentRect.bottom = 64;

	D3DXMatrixIdentity(&m_AttachmentMat);
	m_AttachmentMat._41 = 200;
	m_AttachmentMat._42 = GENERATOR_BAR_Y;

	m_rBossHealth.left = 0;
	m_rBossHealth.top = 0;
	m_rBossHealth.right = 128;
	m_rBossHealth.bottom = 8;

	D3DXMatrixIdentity(&m_HealthMat);
	m_HealthMat._41 = 100;
	m_HealthMat._42 = GENERATOR_BAR_Y;

	D3DXMATRIX scale2;
	D3DXMatrixScaling(&scale2, 3.1f*xRatio,3.0f*yRatio,2);
	m_HealthMat = m_HealthMat * scale2;
	m_bHasPlayedAIVictorySpeech = false;
	//m_rGen1Health.left = 0;
	//m_rGen1Health.top = 0;
	//m_rGen1Health.right = 128;
	//m_rGen1Health.bottom = 8;
	//
	//D3DXMatrixIdentity(&m_Gen1Mat);
	//m_Gen1Mat._41 = 260;
	//m_Gen1Mat._42 = GENERATOR_BAR_Y;
	//
	//D3DXMatrixIdentity(&scale2);
	//D3DXMatrixScaling(&scale2, 0.9f,3.0f,2);
	//m_Gen1Mat = m_Gen1Mat * scale2;
	//
	//m_rGen2Health.left = 0;
	//m_rGen2Health.top = 0;
	//m_rGen2Health.right = 128;
	//m_rGen2Health.bottom = 8;
	//
	//D3DXMatrixIdentity(&m_Gen2Mat);
	//m_Gen2Mat._41 = 484;
	//m_Gen2Mat._42 = GENERATOR_BAR_Y;
	//
	//D3DXMatrixIdentity(&scale);
	//D3DXMatrixScaling(&scale, 0.9f,3.0f,2);
	//m_Gen2Mat = m_Gen2Mat * scale;
	//
	//
	//m_rGen3Health.left = 0;
	//m_rGen3Health.top = 0;
	//m_rGen3Health.right = 128;
	//m_rGen3Health.bottom = 8;
	//
	//
	//D3DXMatrixIdentity(&m_Gen3Mat);
	//m_Gen3Mat._41 = 708;
	//m_Gen3Mat._42 = GENERATOR_BAR_Y;
	//
	//D3DXMatrixIdentity(&scale);
	//D3DXMatrixScaling(&scale, 0.9f,3.0f,2);
	//m_Gen3Mat = m_Gen3Mat * scale;
	//

	m_d3dmDestination = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fZScale = 1.0f;
	m_fZOriginal = 1.0f; 

	m_fEntranceDelay = 0.0f;
	m_fDeathTimer = 8.25f;

	m_bFireLaser = false;
	m_bStopFiringLaser = false;

	for( int i(0); i < BOSS_DEBRIS_COUNT+10; ++i )
	{
		D3DXVECTOR3 vec;
		if( i < BOSS_DEBRIS_COUNT )
		{
			vec = D3DXVECTOR3((float)(rand()%200-100)*4.0f, (float)(rand()%200-100)*4.0f, (float)(rand()%200-100)*4.0f);
		}
		else
		{
			vec = D3DXVECTOR3((float)(rand()%200-100), (float)(rand()%200-100), (float)(rand()%200-100));
		}

		D3DXVECTOR3 d3dBossDebrisVector = D3DXVECTOR3(0,0,800000);	
		CEnvironmentObject* _pSpaceDibris = new CEnvironmentObject(eSPACE_DEBRIS+(i%2), d3dBossDebrisVector, vec);
		_pSpaceDibris->SetIsVisable(false);
		_pSpaceDibris->SetIsScenery(true);
		m_pSpaceDibris.push_back(_pSpaceDibris);
		m_pObjectManager->AddObject(_pSpaceDibris);
	}

	//generators
	m_pGenerators[0] = new CGenerator(this, 1);	
	m_pGenerators[0]->SetPosition(GetPosition().x + 452,  GetPosition().y,GetPosition().z);	
	m_pObjectManager->AddObject(m_pGenerators[0]);

	m_pGenerators[1] = new CGenerator(this, 2);	
	m_pGenerators[1]->SetPosition(GetPosition().x ,GetPosition().y + 398,GetPosition().z);	
	m_pObjectManager->AddObject(m_pGenerators[1]);

	m_pGenerators[2] = new CGenerator(this,0);	
	m_pGenerators[2]->SetPosition(GetPosition().x -452,GetPosition().y,GetPosition().z);	
	m_pObjectManager->AddObject(m_pGenerators[2]);

	CBossTurret* m_pBossTurrets = new CBossTurret(this, m_pPlayer, TOP, eTURRET_LASER,-225);
	m_pBossTurrets->SetPosition(GetPosition().x + 0,GetPosition().y + 292,GetPosition().z);	
	m_pObjectManager->AddObject(m_pBossTurrets);
	m_vpBossTurrets.push_back(m_pBossTurrets);

 	m_pBossTurrets = new CBossTurret(this, m_pPlayer, LEFT, eTURRET_LASER,200);
 	m_pBossTurrets->SetPosition(GetPosition().x - 113,GetPosition().y - 110,GetPosition().z);	
 	m_pObjectManager->AddObject(m_pBossTurrets);
 	m_vpBossTurrets.push_back(m_pBossTurrets);

 	m_pBossTurrets = new CBossTurret(this, m_pPlayer, RIGHT, eTURRET_LASER,200);
 	m_pBossTurrets->SetPosition(GetPosition().x + 113,GetPosition().y - 110,GetPosition().z);	
 	m_pObjectManager->AddObject(m_pBossTurrets);
 	m_vpBossTurrets.push_back(m_pBossTurrets);
 
 
  
   	m_pBossTurrets = new CBossTurret(this, m_pPlayer, LEFT, eTURRET_LASER,-85);
   	m_pBossTurrets->SetPosition(GetPosition().x - 102,GetPosition().y - 70,GetPosition().z);	
   	m_pObjectManager->AddObject(m_pBossTurrets);
   	m_vpBossTurrets.push_back(m_pBossTurrets);
  
  	m_pBossTurrets = new CBossTurret(this, m_pPlayer, LEFT, eTURRET_LASER,-230);
  	m_pBossTurrets->SetPosition(GetPosition().x - 182,GetPosition().y + 55,GetPosition().z);	
  	m_pObjectManager->AddObject(m_pBossTurrets);
  	m_vpBossTurrets.push_back(m_pBossTurrets);
 
 	m_pBossTurrets = new CBossTurret(this, m_pPlayer, LEFT, eTURRET_LASER,-500);
 	m_pBossTurrets->SetPosition(GetPosition().x - 170,GetPosition().y ,GetPosition().z);	
 	m_pObjectManager->AddObject(m_pBossTurrets);
 	m_vpBossTurrets.push_back(m_pBossTurrets);
 
  	m_pBossTurrets = new CBossTurret(this, m_pPlayer, RIGHT, eTURRET_LASER,-85);
  	m_pBossTurrets->SetPosition(GetPosition().x + 102,GetPosition().y - 70,GetPosition().z);	
  	m_pObjectManager->AddObject(m_pBossTurrets);
  	m_vpBossTurrets.push_back(m_pBossTurrets);
  
 	m_pBossTurrets = new CBossTurret(this, m_pPlayer, RIGHT, eTURRET_LASER,-230);
 	m_pBossTurrets->SetPosition(GetPosition().x + 183,GetPosition().y + 55,GetPosition().z);	
 	m_pObjectManager->AddObject(m_pBossTurrets);
 	m_vpBossTurrets.push_back(m_pBossTurrets);
 	// 
 	m_pBossTurrets = new CBossTurret(this, m_pPlayer, RIGHT, eTURRET_LASER,-500);
 	m_pBossTurrets->SetPosition(GetPosition().x + 172,GetPosition().y + 5,GetPosition().z);	
 	m_pObjectManager->AddObject(m_pBossTurrets);
 	m_vpBossTurrets.push_back(m_pBossTurrets);


// 	 	m_pBossTurrets = new CBossTurret(this, m_pPlayer, RIGHT, eTURRET_LASER,-585);
// 	 	m_pBossTurrets->SetPosition(GetPosition().x + 60,GetPosition().y - 250,GetPosition().z);	
// 	 	m_pObjectManager->AddObject(m_pBossTurrets);
// 	 	m_vpBossTurrets.push_back(m_pBossTurrets);
// 
// 	 	m_pBossTurrets = new CBossTurret(this, m_pPlayer, LEFT, eTURRET_LASER,-585);
// 	 	m_pBossTurrets->SetPosition(GetPosition().x - 60,GetPosition().y - 250,GetPosition().z);	
// 	 	m_pObjectManager->AddObject(m_pBossTurrets);
// 	 	m_vpBossTurrets.push_back(m_pBossTurrets);
// 


	CBossLargerTurret * m_pBossLargeTurrets = new CBossLargerTurret(this, m_pPlayer, TOP, eTURRET_LASER,-80);
	m_pBossLargeTurrets->SetPosition(GetPosition().x + 0,GetPosition().y + 208,GetPosition().z);	
	m_pObjectManager->AddObject(m_pBossLargeTurrets);
	m_vpBossLargeTurrets.push_back(m_pBossLargeTurrets);

 	m_pBossLargeTurrets = new CBossLargerTurret(this, m_pPlayer, TOP, eTURRET_MISSILE,-250);
 	m_pBossLargeTurrets->SetPosition(GetPosition().x + 130,GetPosition().y +185,GetPosition().z);	
 	m_pObjectManager->AddObject(m_pBossLargeTurrets );
 	m_vpBossLargeTurrets.push_back(m_pBossLargeTurrets);
 
 	m_pBossLargeTurrets = new CBossLargerTurret(this, m_pPlayer, TOP, eTURRET_MISSILE,-250);
 	m_pBossLargeTurrets->SetPosition(GetPosition().x - 130,GetPosition().y + 185,GetPosition().z);	
 	m_pObjectManager->AddObject(m_pBossLargeTurrets );
 	m_vpBossLargeTurrets.push_back(m_pBossLargeTurrets);
  
  	m_pBossLargeTurrets = new CBossLargerTurret(this, m_pPlayer, TOP, eTURRET_LASER,+260);
  	m_pBossLargeTurrets->SetPosition(GetPosition().x + 0,GetPosition().y + 218,GetPosition().z);	
  	m_pObjectManager->AddObject(m_pBossLargeTurrets );
  	m_vpBossLargeTurrets.push_back(m_pBossLargeTurrets);
  
  	m_pBossLargeTurrets= new CBossLargerTurret(this, m_pPlayer, LEFT, eTURRET_LASER,-260); //maybe right
  	m_pBossLargeTurrets->SetPosition(GetPosition().x - 181,GetPosition().y + 100,GetPosition().z);	
  	m_pObjectManager->AddObject(m_pBossLargeTurrets );
  	m_vpBossLargeTurrets.push_back(m_pBossLargeTurrets);
  
  	m_pBossLargeTurrets = new CBossLargerTurret(this, m_pPlayer, RIGHT, eTURRET_LASER,+-260);
  	m_pBossLargeTurrets->SetPosition(GetPosition().x + 181, GetPosition().y + 100,GetPosition().z);	
  	m_pObjectManager->AddObject(m_pBossLargeTurrets );
  	m_vpBossLargeTurrets.push_back(m_pBossLargeTurrets);
 
 	m_pBossLargeTurrets = new CBossLargerTurret(this, m_pPlayer, LEFT, eTURRET_LASER,5);
 	m_pBossLargeTurrets->SetPosition(GetPosition().x - 191, GetPosition().y + 35, GetPosition().z);
 	m_pObjectManager->AddObject(m_pBossLargeTurrets );
 	m_vpBossLargeTurrets.push_back(m_pBossLargeTurrets);
  
  	m_pBossLargeTurrets = new CBossLargerTurret(this, m_pPlayer, RIGHT, eTURRET_LASER,5);
  	m_pBossLargeTurrets->SetPosition(GetPosition().x + 191, GetPosition().y + 35, GetPosition().z);
  	m_pObjectManager->AddObject(m_pBossLargeTurrets );
  	m_vpBossLargeTurrets.push_back(m_pBossLargeTurrets);
 

	showHanger = true;
	showTurret = true;
	//real one
	m_pBossWinTurret = new CBossWinTurret(this, m_pPlayer, BOTTOM, eTURRET_LASER,240, m_pObjectManager);
	m_pBossWinTurret->SetPosition(GetPosition().x,GetPosition().y - 220,GetPosition().z + 240);	
	m_pObjectManager->AddObject(m_pBossWinTurret);


	//top 2 thrusters
	m_pBossThruster = new CBossThruster(this, D3DXVECTOR3(75.0f, 155.0f, -800.0f), 80.0f);
	m_pBossThruster->SetPosition(GetPosition().x, GetPosition().y, GetPosition().z -460);
	m_pObjectManager->AddObject(m_pBossThruster);
	m_pBossThruster = new CBossThruster(this, D3DXVECTOR3(-75.0f, 155.0f, -800.0f), 80.0f);
	m_pBossThruster->SetPosition(GetPosition().x, GetPosition().y, GetPosition().z -460);
	m_pObjectManager->AddObject(m_pBossThruster);
	/////
	//bot 2 thrusters
	m_pBossThruster = new CBossThruster(this, D3DXVECTOR3(75.0f, 0.0f, -815.0f), 80.0f);
	m_pBossThruster->SetPosition(GetPosition().x, GetPosition().y, GetPosition().z -440);
	m_pObjectManager->AddObject(m_pBossThruster);
	m_pBossThruster = new CBossThruster(this, D3DXVECTOR3(-75.0f, 0.0f, -815.0f), 80.0f);
	m_pBossThruster->SetPosition(GetPosition().x, GetPosition().y, GetPosition().z -440);
	m_pObjectManager->AddObject(m_pBossThruster);


	
	//test one
	// 	m_pBossWinTurret = new CBossWinTurret(this, m_pPlayer, BOTTOM, eTURRET_LASER,240, m_pObjectManager);
	// 	m_pBossWinTurret->SetPosition(GetPosition().x,GetPosition().y - 220,GetPosition().z + 240);	
	// 	m_pObjectManager->AddObject(m_pBossWinTurret);

	m_pPlanet = _pPlanet;
	m_pLight1 = _pLight1;
	m_pLight2 = _pLight2;
	m_pLight3 = _pLight3;
	m_pLight4 = _pLight4;

	//m_pPlanet = new CEnvironmentObject(ePLANET1,  D3DXVECTOR3(-5000,0,60000));
	SetHealth(m_pBossWinTurret->GetHealth());
	m_fHealthCounter = 1;
	m_bHasPlayedAIHydranceEntrance = false;
	m_bHasPlayedAIHydraShieldEntrance = false;
	m_bHasPlayedAIHydraTurretEntrance = false;
	m_bHasPlayedAIHydraMegaLaserEntrance = false;
	
	m_nLastBigExplosionSound = -1;
}

void CBoss::SetDestinationPosition(float _fX, float _fY, float _fZ)
{
	m_d3dmDestination.x = _fX;
	m_d3dmDestination.y = _fY;
	m_d3dmDestination.z = _fZ;

	if( m_nPhase == eCINEMATIC_PHASE )
	{
		D3DXVECTOR3 toplayer = m_pPlayer->GetPosition() - m_d3dmDestination;
		D3DXVec3Normalize(&toplayer, &toplayer);

		D3DXMATRIX mat;
		D3DXMatrixTranslation(&mat, 0.0f, 0.0f, -toplayer.z * 27500.0f);

		m_d3dMatrix._41 = mat._41;
		m_d3dMatrix._42 = mat._42;
		m_d3dMatrix._43 = mat._43;
	}
	else
	{
		m_d3dMatrix._41 = _fX;
		m_d3dMatrix._42 = _fY;
		m_d3dMatrix._43 = _fZ;
	}

	m_fZOriginal = m_d3dMatrix._33;
}

/*****************************************************************
* ~CBoss(): Destructor, probably doing nothing
* Ins:			    None    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CBoss::~CBoss(void)
{
	for(int i = 0; i < 3; i++)
	{
		m_pGenerators[i] = nullptr;
	}
	m_pObjectManager = nullptr;
	delete m_pShieldBV;

	m_bFireLaser = false;
	m_bStopFiringLaser = false;
}

void CBoss::CheckPlayerSide()
{
	CPlayer * m_pPlayer = (CPlayer*)m_pTarget;
	D3DXVECTOR3 d3dEnemyPosition = m_pPlayer->GetPosition();
	D3DXVECTOR3 d3dVectorBetween = d3dEnemyPosition - GetPosition();
	D3DXVec3Normalize(  &d3dVectorBetween, &d3dVectorBetween );
	//top
	D3DXVECTOR3 d3dYAxis =  CPhysics::GetMat4Vec(eYAXIS, GetMatrix());
	D3DXVec3Normalize(  &d3dYAxis, &d3dYAxis );
	//Left/Right
	D3DXVECTOR3 d3dXAxis =  CPhysics::GetMat4Vec(eXAXIS, GetMatrix());
	D3DXVec3Normalize(  &d3dXAxis, &d3dXAxis );
	m_bIsOnRight = false;
	m_bIsOnLeft = false;
	m_bIsOnTop = false;

	if (D3DXVec3Dot( &d3dVectorBetween, &d3dXAxis)  > .1f)
	{
		m_bIsOnRight = true;
	}
	if (D3DXVec3Dot( &d3dVectorBetween, &d3dXAxis)  < -.1f)
	{
		m_bIsOnLeft = true;
	}
	if (D3DXVec3Dot( &d3dVectorBetween, &d3dYAxis)  > .1f)
	{
		m_bIsOnTop = true;
	}
}

/*****************************************************************
* Update():		Will update the boss internal timers and update the behaviors 
*					based on the elapsed time.
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29`/2012
* Mod. Initials:	AR
*****************************************************************/
void CBoss::DrawFeedback(float _time, bool _bCinematic)
{
	char szBuff3[32];
	char szBuff5[32];
	char szBuff6[32];
	float bossHealthPercentage = (m_fHealthCounter / float(m_nMaxHealth));

	float bossShieldPercentage = (m_pBossWinTurret->GetShields() / 200.0f);


	if (m_bDrawWarning  && m_fBattleTimer > 0)
	{
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

		if (!m_bIsDisabled)
		{
			char szBuf[32];
			sprintf_s(szBuf, "WARNING" );
			m_pRenderer->RenderText(szBuf, 405, 122, 1.95f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_fSkipTextAlpha,0,0,0));
			m_pRenderer->RenderText(szBuf, 403, 120, 1.95f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_fSkipTextAlpha,235,55,45));
		}

	}
	if ( m_nPhase == eSEEK_AND_DESTROY )
	{
		// 		if (bossHealthPercentage >= .70f)
		// 		{
		// 			m_BossColor = D3DCOLOR_ARGB( 150, 0, int( 255 * bossHealthPercentage ), 0 );
		// 		}
		// 		else if (bossHealthPercentage >= .56f && bossHealthPercentage <= .70f)
		// 		{
		// 			m_BossColor = D3DCOLOR_ARGB( 150, int(  400 * bossHealthPercentage ), int(  400 * bossHealthPercentage ), 0 );
		// 		}
		// 		else
		// 		{
		// 			m_BossColor = D3DCOLOR_ARGB( 150, int(  500 * bossHealthPercentage ), 20, 20 );
		// 		}

		m_rBossHealth.right = -LONG( bossHealthPercentage * 128);
		if(m_fHealthCounter > 0)
		{
			m_pRenderer->RenderArrowAndrewR(BossHealthTexture, &m_rBossHealth, m_HealthMat , D3DCOLOR_ARGB(200, 25, 220, 25), false ,false );

			//if (m_pBossWinTurret->GetShields() <= 0)
			//{
			sprintf_s(szBuff3, "I.D.F. HYDRA: "  );
			m_pRenderer->RenderText(szBuff3, 340, GENERATOR_TEXT_Y, 0.8f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255,255,255));
			//}
		}


		// 		if (bossShieldPercentage >= .70f)
		// 		{
		// 			m_BossColor = D3DCOLOR_ARGB( 150, 0, int( 255 * bossShieldPercentage ), 0 );
		// 		}
		// 		else if (bossShieldPercentage >= .56f && bossShieldPercentage <= .70f)
		// 		{
		// 			m_BossColor = D3DCOLOR_ARGB( 150, int(  400 * bossShieldPercentage ), int(  400 * bossShieldPercentage ), 0 );
		// 		}
		// 		else
		// 		{
		// 			m_BossColor = D3DCOLOR_ARGB( 150, int(  500 * bossShieldPercentage ), 20, 20 );
		// 		}

		m_rBossHealth.right = -LONG( bossShieldPercentage * 128);
		if(m_fHealthCounter > 0)
		{
			RECT r = m_rBossHealth;
			r.bottom = 3;
			D3DXMATRIX m = m_HealthMat;
			m._42 -= 27;
			m_pRenderer->RenderArrowAndrewR(BossHealthTexture, &r, m  , D3DCOLOR_ARGB(200, 215,60,60), false,false );
			//if (m_pBossWinTurret->GetShields() > 0)
			//{
			//sprintf_s(szBuff3, "SHIELD: "  );
			//m_pRenderer->RenderText(szBuff3, 340, GENERATOR_TEXT_Y, 0.8f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255,255,255));
			//}
		}


	}

	if (!_bCinematic)
	{
		teachPhase1 = false;
		teachPhase2 = false;
		teachPhase3 = false;
		xPoScrolls = -200;

		//if phase == x do the other text
// 		if (m_nPhase == eLASER_PHASE)
// 		{
// 			sprintf_s(szBuff4, "Shields Active" );
// 			m_pRenderer->RenderText(szBuff4, 832, PHASE_TEXT_Y + 2, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0,0,0));
// 			m_pRenderer->RenderText(szBuff4, 830, PHASE_TEXT_Y, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(180,255,0));
// 
// 		}
// 		else if (m_nPhase == eMISSILE_PHASE)
// 		{
// 			sprintf_s(szBuff4, "Turrets Active" );
// 			m_pRenderer->RenderText(szBuff4, 832, PHASE_TEXT_Y + 2, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0,0,0));
// 			m_pRenderer->RenderText(szBuff4, 830, PHASE_TEXT_Y, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(210,60,60));
// 
// 		}
// 		if(m_nPhase == eSEEK_AND_DESTROY)
// 		{
// 			int HealthLeft = m_pBossWinTurret->GetHealth();
// 			int shieldLeft = m_pBossWinTurret->GetShields();
// 			sprintf_s(szBuff4, "Planet Destroyer Active");
// 			m_pRenderer->RenderText(szBuff4, 752, PHASE_TEXT_Y + 2, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0,0,0));
// 			if (shieldLeft > 0)
// 			{
// 				m_pRenderer->RenderText(szBuff4, 750, PHASE_TEXT_Y, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(240,80,80));
// 			}
// 			else
// 			{
// 				m_pRenderer->RenderText(szBuff4, 750, PHASE_TEXT_Y, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(90,90,240));
// 			}
// 		}


		if (m_fBattleTimer >= 180)
		{
			m_TimerColor = D3DCOLOR_ARGB(255, 0, 255, 0);
		}
		else if(m_fBattleTimer >= 90 && m_fBattleTimer <= 180)
		{
			m_TimerColor = D3DCOLOR_ARGB(255, 255, 255, 0);
		}
		else
		{
			m_TimerColor = D3DCOLOR_ARGB(255, 255, 0, 0);
		}
		/*
		if( m_tTimerPosition.x < 820 )
		{
		m_tTimerPosition.x += (long)(10.0f * _time);
		if( m_tTimerPosition.x > 820 )
		{
		m_tTimerPosition.x = 820; 
		}
		}
		if( m_tTimerPosition.y < 700 )
		{
		m_tTimerPosition.y += (long)(10.0f * _time);
		if( m_tTimerPosition.y > 700 )
		{
		m_tTimerPosition.y = 700; 
		}
		}
		if( m_fTimerLowScale > 0.0f )
		{
		m_fTimerLowScale -= 10.0f * _time;
		if( m_fTimerLowScale < 0.0f )
		{
		m_fTimerLowScale = 0.0f;
		}
		}

		if( ((int)m_fBattleTimer%60) != ((int)m_fseconds%60)+2 && m_fBattleTimer < 30.0f )
		{
		m_tTimerPosition.x -= (LONG)(10.0l);
		m_tTimerPosition.y -= (LONG)(10.0l);
		m_fTimerLowScale += 0.2f;
		m_fseconds = m_fBattleTimer;
		}*/

		if(m_nPhase != eSEEK_AND_DESTROY)
		{
			sprintf_s(szBuff5, "Time: %d :%02d" ,(int)m_fBattleTimer / 60 , int(m_fBattleTimer) % 60 );
			m_pRenderer->RenderText(szBuff5, 822, 702, 1.4f + m_fTimerLowScale, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0,0,0));
			m_pRenderer->RenderText(szBuff5, m_tTimerPosition.x, m_tTimerPosition.y, 1.4f + m_fTimerLowScale, 0.0f, 0.0f, 0.0f, m_TimerColor);
		}

		if (m_nNumGenerators > 0)
		{
			//////////////////////////////////////////////////////////////////////////
			//Draw the generators health bars here
			// 		m_rGen1Health.right = -LONG( (m_pGenerators[0]->GetHealth() / 500.0f) * 128);
			sprintf_s(szBuff6, "Kill Marked Targets");
			m_pRenderer->RenderText(szBuff6, 802, 602, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0,0,0));
			m_pRenderer->RenderText(szBuff6, 800, 600, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(203, 81, 63));


			sprintf_s(szBuff3, "Generators: %d", m_nNumGenerators  );
			// 		if (m_pGenerators[0]->GetHealth() > 0)
			{
				m_AttachmentMat._41 = 830 * m_pRenderer->GetXRatio();
				m_AttachmentMat._42 = 646 * m_pRenderer->GetYRatio();
				m_pRenderer->RenderArrowAndrewR(GeneratorTexture, &m_rAttachmentRect, m_AttachmentMat , D3DCOLOR_ARGB(200,255,255,255), true, false);

				m_pRenderer->RenderText(szBuff3,  int(865), 631, 0.85f, 0.0f, 0.0f, 0.0f,D3DCOLOR_XRGB(0,0,0) );
				m_pRenderer->RenderText(szBuff3,  int(865), 630, 0.85f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(203, 81, 63));
			}
			// 		else
			// 		{
			// 			m_pRenderer->RenderText(szBuff3, 250, GENERATOR_TEXT_Y, 0.65f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255,0,0));
			// 		}
			// 		m_pRenderer->RenderArrowAndrewR(BossHealthTexture, &m_rGen1Health, m_Gen1Mat , D3DCOLOR_ARGB(140, 0, 0, 200 ), false );
			// 
			// 		m_rGen2Health.right = -LONG( (m_pGenerators[1]->GetHealth() / 500.0f) * 128);
			// 		sprintf_s(szBuff, "Generator 2: "  );
			// 		if (m_pGenerators[1]->GetHealth() > 0)
			// 		{
			// 			m_pRenderer->RenderText(szBuff, 450, GENERATOR_TEXT_Y, 0.65f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255,255,255));
			// 		}
			// 		else
			// 		{
			// 			m_pRenderer->RenderText(szBuff, 450, GENERATOR_TEXT_Y, 0.65f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255,0,0));
			// 		}
			// 		m_pRenderer->RenderArrowAndrewR(BossHealthTexture, &m_rGen2Health, m_Gen2Mat , D3DCOLOR_ARGB(140, 0, 0, 200 ), false );
			// 
			// 		m_rGen3Health.right = -LONG( (m_pGenerators[2]->GetHealth() / 500.0f) * 128);
			// 		sprintf_s(szBuff2, "Generator 3: "  );
			// 		if (m_pGenerators[2]->GetHealth() > 0)
			// 		{
			// 			m_pRenderer->RenderText(szBuff2, 650, GENERATOR_TEXT_Y, 0.65f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255,255,255));
			// 		}
			// 		else
			// 		{
			// 			m_pRenderer->RenderText(szBuff2, 650, GENERATOR_TEXT_Y, 0.65f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255,0,0));
			// 		}
			// 		m_pRenderer->RenderArrowAndrewR(BossHealthTexture, &m_rGen3Health, m_Gen3Mat , D3DCOLOR_ARGB(140, 0, 0, 200 ), false );
		}
		else if(numAttachments > 0)
		{
			sprintf_s(szBuff6, "Kill Marked Targets");
			m_pRenderer->RenderText(szBuff6, 802, 602, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0,0,0));
			m_pRenderer->RenderText(szBuff6, 800, 600, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(253, 81, 63));



			sprintf_s(szBuff3, "Turrets: %d", numAttachments  );
			// 		if (m_pGenerators[0]->GetHealth() > 0)
			{
				m_AttachmentMat._41 = 835 * m_pRenderer->GetXRatio();
				m_AttachmentMat._42 = 632 * m_pRenderer->GetYRatio();
				m_pRenderer->RenderArrowAndrewR(TurretTexture, &m_rAttachmentRect, m_AttachmentMat , D3DCOLOR_ARGB(200,255,255,255), true, false);
				
				m_pRenderer->RenderText(szBuff3,  int(885), 631, 0.85f, 0.0f, 0.0f, 0.0f,D3DCOLOR_XRGB(0,0,0) );
				m_pRenderer->RenderText(szBuff3,  int(885), 630, 0.85f, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(253, 81, 63));
			}
		}

		static float teachTimer = 0.0f;
		//Teach the player how to beat phase 1
	}
	else
	{

		if(teachPhase1)
		{
			xPoScrolls += 10;
			if(xPoScrolls >= 98)
			{
				xPoScrolls = 98;
			}
			// 		if (0 xpos <= -500)
			// 		{
			// 			DisplayteachPhase1 = false;
			// 		}
			// 		char zBuff2[62];
			// 		teachTimer += _time;
			// 		sprintf_s(zBuff2, "Destroy Generators To\n     Break the Shield "  );
			// 		m_pRenderer->RenderText(zBuff2, 302, 42, 1.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(alpha, 0,0,0));
			// 		m_pRenderer->RenderText(zBuff2, 300, 40, 1.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(alpha,0 ,255,0));
			// 		if (teachTimer >= 0.0674f)
			// 		{
			// 			alpha -= 1;
			// 			if (alpha <= 0)
			// 			{
			// 				teachPhase1 = false;
			// 				alpha = 255;
			// 			}
			// 			teachTimer = 0.0f;
			// 		}
			if(m_bHasPlayedAIHydraShieldEntrance == false)
			{
				WwiseNS::StopAllTutorialVoices(eAIHydraShieldEntrance);
				WwiseNS::SoundEvent(eAIHydraShieldEntrance, ePlay);
				m_bHasPlayedAIHydraShieldEntrance = true;
			}
			char szBuff[255];
			char szBuff2[255];
			char szBuff3[255];
			char szBuff5[255];
//			char szBuff6[255];
			sprintf_s(szBuff2, "Generator");
			sprintf_s(szBuff3, "\n\n\n\n\n\nTIPS");
			sprintf_s(szBuff, "\nHealth   : Medium\nVelocity : Low \nFire Rate: None\nDamage : None\n\n\n  -Generators Power the shield.\n  -Use            .\n");
			sprintf_s(szBuff5, "\n\n\n\n\n\n\n\n           Lasers");
			//sprintf_s(szBuff5, "\n\n\n\n\n\n\n\n             Lasers");

			D3DXMATRIX tmpBackMat = m_BlackMat;
			tmpBackMat._41 += xPoScrolls * m_pRenderer->GetXRatio();
			m_pRenderer->RenderArrowAndrewR(BlackTexture, &m_rBlack, tmpBackMat , D3DCOLOR_ARGB(140,255,255,255), true, false);

			m_pRenderer->RenderText(szBuff2,xPoScrolls, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,0, 220, 220));
			m_pRenderer->RenderText(szBuff3, xPoScrolls, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,0, 220, 220));
			m_pRenderer->RenderText(szBuff, xPoScrolls, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,245,245,245));
			//m_pRenderer->RenderText(szBuff4, xPoScrolls, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 128, 128, 255));
			m_pRenderer->RenderText(szBuff5, xPoScrolls, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 255, 64, 64));

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

			char szBuff4[32];
			sprintf_s(szBuff4, "Press SPACE to Skip");
			m_pRenderer->RenderText(szBuff4, 739, SKIP_TEXT_Y_POSITION-1, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_fSkipTextAlpha,0,0,0));
			m_pRenderer->RenderText(szBuff4, 740, SKIP_TEXT_Y_POSITION, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_fSkipTextAlpha,245,245,245));

		}
		else if(teachPhase2)
		{
			if(m_bHasPlayedAIHydraTurretEntrance == false)
			{
				WwiseNS::StopAllTutorialVoices(eAIHydraTurretEntrance);
				WwiseNS::SoundEvent(eAIHydraTurretEntrance, ePlay);
				m_bHasPlayedAIHydraTurretEntrance = true;
			}
			xPoScrolls += 10;
			if(xPoScrolls >= 98)
			{
				xPoScrolls = 98;
			}
			// 		char zBuff2[62];
			// 		teachTimer += _time;
			// 		sprintf_s(zBuff2, "  Kill Turrets\n to Stop Him"  );
			// 		m_pRenderer->RenderText(zBuff2, 402, 42, 1.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(alpha, 0,0,0));
			// 		m_pRenderer->RenderText(zBuff2, 400, 40, 1.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(alpha,0 ,255,0));
			// 		if (teachTimer >= 0.0674f)
			// 		{
			// 			alpha -= 1;
			// 			if (alpha <= 0)
			// 			{
			// 				teachPhase2 = false;
			// 				alpha = 255;
			// 			}
			// 			teachTimer = 0.0f;
			// 		}	

			char szBuff[255];
			char szBuff2[255];
			char szBuff3[255];
			char szBuff5[255];
			char szBuff6[255];
			char szBuff7[255];
			sprintf_s(szBuff2, "Turrets");
			sprintf_s(szBuff3, "\n\n\n\n\n\nTIPS");
			sprintf_s(szBuff, "\nHealth   : Low\nVelocity : None\nFire Rate: High\nDamage : Medium\n\n\n  -Use           .\n  -Turrets fire               and            . \n");
			sprintf_s(szBuff5, "\n\n\n\n\n\n\n          Lasers");
			sprintf_s(szBuff6, "\n\n\n\n\n\n\n\n                      missiles");
			sprintf_s(szBuff7, "\n\n\n\n\n\n\n\n                                           lasers");

			D3DXMATRIX tmpBackMat = m_BlackMat;
			tmpBackMat._41 += xPoScrolls * m_pRenderer->GetXRatio();
			m_pRenderer->RenderArrowAndrewR(BlackTexture, &m_rBlack, tmpBackMat , D3DCOLOR_ARGB(140,255,255,255), true, false);

			m_pRenderer->RenderText(szBuff2,xPoScrolls, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,0, 220, 220));
			m_pRenderer->RenderText(szBuff3, xPoScrolls, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,0, 220, 220));
			m_pRenderer->RenderText(szBuff, xPoScrolls, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,245,245,245));
			m_pRenderer->RenderText(szBuff5, xPoScrolls, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 255, 64, 64));
			m_pRenderer->RenderText(szBuff6, xPoScrolls, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 128, 128, 255));
			m_pRenderer->RenderText(szBuff7, xPoScrolls, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 255, 64, 64));

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
			char szBuff4[32];
			sprintf_s(szBuff4, "Press SPACE to Skip");
			m_pRenderer->RenderText(szBuff4, 739, SKIP_TEXT_Y_POSITION-1, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_fSkipTextAlpha,0,0,0));
			m_pRenderer->RenderText(szBuff4, 740, SKIP_TEXT_Y_POSITION, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_fSkipTextAlpha,245,245,245));

		}
		else  if(teachPhase3)
		{
			if(m_bHasPlayedAIHydraMegaLaserEntrance == false)
			{
				WwiseNS::StopAllTutorialVoices(eAIHydraMegaLaserEntrance);
				WwiseNS::SoundEvent(eAIHydraMegaLaserEntrance, ePlay);
				m_bHasPlayedAIHydraMegaLaserEntrance = true;
			}
			xPoScrolls += 10;
			if(xPoScrolls >= 98)
			{
				xPoScrolls = 98;
			}
			char szBuff[255];
			char szBuff2[255];
			char szBuff3[255];
			char szBuff5[255];
			char szBuff6[255];
			sprintf_s(szBuff2, "Planet Destroyer");
			sprintf_s(szBuff3, "\n\n\n\n\n\nTIPS");
			sprintf_s(szBuff, "\nHealth   : Very High\nShields : Medium\nFire Rate: Low\nDamage : Very Very High\n\n\n  -Shield Reflects             .\n  -             destroy UnShielded gun.");
			sprintf_s(szBuff5, "\n\n\n\n\n\n\n                            missiles");
			sprintf_s(szBuff6, "\n\n\n\n\n\n\n\n   Missiles");

			D3DXMATRIX tmpBackMat = m_BlackMat;
			tmpBackMat._41 += xPoScrolls * m_pRenderer->GetXRatio();
			m_pRenderer->RenderArrowAndrewR(BlackTexture, &m_rBlack, tmpBackMat , D3DCOLOR_ARGB(140,255,255,255), true, false);

			m_pRenderer->RenderText(szBuff2,xPoScrolls, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,0, 220, 220));
			m_pRenderer->RenderText(szBuff3, xPoScrolls, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,0, 220, 220));
			m_pRenderer->RenderText(szBuff, xPoScrolls, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,245,245,245));
			m_pRenderer->RenderText(szBuff5, xPoScrolls, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 128, 128, 255));
			m_pRenderer->RenderText(szBuff6, xPoScrolls, TEXT_Y_POSITION, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 128, 128, 255));

			// 		char zBuff2[62];
			// 		teachTimer += _time;
			// 		sprintf_s(zBuff2, "  Destroy The Big Turret"  );
			// 		m_pRenderer->RenderText(zBuff2, 302, 42, 1.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(alpha, 0,0,0));
			// 		m_pRenderer->RenderText(zBuff2, 300, 40, 1.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(alpha,0 ,255,0));
			// 		if (teachTimer >= 0.0674f)
			// 		{
			// 			alpha -= 1;
			// 			if (alpha <= 0)
			// 			{
			// 				teachPhase3 = false;
			// 				alpha = 255;
			// 			}
			// 			teachTimer = 0.0f;
			// 		}	
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

			char szBuff4[32];
			sprintf_s(szBuff4, "Press SPACE to Skip");
			m_pRenderer->RenderText(szBuff4, 739, SKIP_TEXT_Y_POSITION-1, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_fSkipTextAlpha,0,0,0));
			m_pRenderer->RenderText(szBuff4, 740, SKIP_TEXT_Y_POSITION, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_fSkipTextAlpha,245,245,245));

		}

	}
}

void CBoss::FireAtAsteroid(float _eTime)
{
	static float sRestrictFireTimer = 0.0f;
	sRestrictFireTimer += _eTime;

	if (m_pAsteroidTarget != 0 && m_pAsteroidTarget->GetActive())
	{
		if (sRestrictFireTimer >= 1.25f)
		{
			CMissile* pMissile = (CMissile*)m_pObjectManager->ReturnArmamentToMasterList(eMISSILE);
			if(pMissile != nullptr)
			{
				//pMissile->SetMatrix(&GetMatrixCopy());
				CEntity * tempAsteroid = m_pAsteroidTarget;
				D3DXVECTOR3 pos = GetPosition();
				pos.y = pos.y - 50;
				pMissile->SetPosition(pos);

				pMissile->SetTarget( tempAsteroid );
				pMissile->SetLifetime(15.0f);
				pMissile->SetHealth(100);
				//_AIHelper->GetmissileVelocityModifier();
				pMissile->SetDoOnce(true);
				pMissile->SetVelocityModifier(100);
				pMissile->SetOwner(this);

				for (unsigned int i = 0; i < pMissile->GetMovementBehaviorStack().size(); i++)
				{
					pMissile->PopBehavior();
				}

				pMissile->CreateEffect(eBLUE_ENEMY_FIRE_MISSILE);
			}
			sRestrictFireTimer = 0.0f;

		}

	}

}

void CBoss::Update(float _fElapedTime)
{
	unsigned helperSize =  m_vpCollisionHelper.size();
	for (unsigned i = 0; i < helperSize; i++)
	{
		if (m_nPhase == eLASER_PHASE || m_nPhase == eCINEMATIC_PHASE)
		{
			if (m_vpCollisionHelper[i]->GetDuringShield())
			{
				m_vpCollisionHelper[i]->SetActive(true);
			}
			else
			{
				m_vpCollisionHelper[i]->SetActive(false);
			}
		}
		else
		{
			if (!m_vpCollisionHelper[i]->GetDuringShield())
			{
				m_vpCollisionHelper[i]->SetActive(true);
			}
			else
			{
				m_vpCollisionHelper[i]->SetActive(false);
			}
		}
	}

	////
	SetHealth(INT_MAX);
	D3DXMATRIX d3dMat;
	D3DXMatrixIdentity( &d3dMat );
 	if (m_nPhase == eLASER_PHASE && m_fBattleTimer > 0.0f)
 	{
 		d3dMat._43 = m_nVelocityModifier * _fElapedTime * 0.0555f; 
 		SetMatrix( &( d3dMat * ( *GetMatrix() ) ) );
 		D3DXVECTOR3 planerPos = m_pPlanet->GetPosition();
 		planerPos.z -= .96f;
 		m_pPlanet->SetPosition(planerPos);

		D3DXMATRIX d3dTempLight1Mat = *m_pLight1->GetMatrix();
		D3DXMATRIX d3dTempLight1Mat2 = *m_pLight1->GetMatrix();
		d3dTempLight1Mat._43 = m_nVelocityModifier * _fElapedTime * 0.0555f;
		m_pLight1->SetMatrix(&(d3dTempLight1Mat * d3dTempLight1Mat2));
		D3DXVECTOR3 d3dLightPos = m_pLight1->GetPosition();
		d3dLightPos.z -= 0.96f;
		m_pLight1->SetPosition(d3dLightPos);

		D3DXMATRIX d3dTempLight2Mat = *m_pLight2->GetMatrix();
		D3DXMATRIX d3dTempLight2Mat2 = *m_pLight2->GetMatrix();
		d3dTempLight2Mat._43 = m_nVelocityModifier * _fElapedTime * 0.0555f;
		m_pLight2->SetMatrix(&(d3dTempLight2Mat * d3dTempLight2Mat2));
		D3DXVECTOR3 d3dLight2Pos = m_pLight2->GetPosition();
		d3dLight2Pos.z -= 0.96f;
		m_pLight2->SetPosition(d3dLight2Pos);

		D3DXMATRIX d3dTempLight3Mat = *m_pLight3->GetMatrix();
		D3DXMATRIX d3dTempLight3Mat2 = *m_pLight3->GetMatrix();
		d3dTempLight3Mat._43 = m_nVelocityModifier * _fElapedTime * 0.0555f;
		m_pLight3->SetMatrix(&(d3dTempLight3Mat * d3dTempLight3Mat2));
		D3DXVECTOR3 d3dLight3Pos = m_pLight3->GetPosition();
		d3dLight3Pos.z -= 0.96f;
		m_pLight3->SetPosition(d3dLight3Pos);

		D3DXMATRIX d3dTempLight4Mat = *m_pLight4->GetMatrix();
		D3DXMATRIX d3dTempLight4Mat2 = *m_pLight4->GetMatrix();
		d3dTempLight4Mat._43 = m_nVelocityModifier * _fElapedTime * 0.0555f;
		m_pLight4->SetMatrix(&(d3dTempLight4Mat * d3dTempLight4Mat2));
		D3DXVECTOR3 d3dLight4Pos = m_pLight4->GetPosition();
		d3dLight4Pos.z -= 0.96f;
		m_pLight4->SetPosition(d3dLight4Pos);
 	}
 	else if (m_nPhase == eMISSILE_PHASE && m_fBattleTimer > 0.0f)
 	{
 		d3dMat._43 = m_nVelocityModifier * _fElapedTime * 0.09f; 
 		SetMatrix( &( d3dMat * ( *GetMatrix() ) ) );
 		D3DXVECTOR3 planerPos = m_pPlanet->GetPosition();
 		planerPos.z -= 1.29f;
 		m_pPlanet->SetPosition(planerPos);

		D3DXMATRIX d3dTempLight1Mat = *m_pLight1->GetMatrix();
		D3DXMATRIX d3dTempLight1Mat2 = *m_pLight1->GetMatrix();
		d3dTempLight1Mat._43 = m_nVelocityModifier * _fElapedTime * 0.09f;
		m_pLight1->SetMatrix(&(d3dTempLight1Mat * d3dTempLight1Mat2));
		D3DXVECTOR3 d3dLightPos = m_pLight1->GetPosition();
		d3dLightPos.z -= 1.29f;
		m_pLight1->SetPosition(d3dLightPos);

		D3DXMATRIX d3dTempLight2Mat = *m_pLight2->GetMatrix();
		D3DXMATRIX d3dTempLight2Mat2 = *m_pLight2->GetMatrix();
		d3dTempLight2Mat._43 = m_nVelocityModifier * _fElapedTime * 0.09f;
		m_pLight2->SetMatrix(&(d3dTempLight2Mat * d3dTempLight2Mat2));
		D3DXVECTOR3 d3dLight2Pos = m_pLight2->GetPosition();
		d3dLight2Pos.z -= 1.29f;
		m_pLight2->SetPosition(d3dLight2Pos);

		D3DXMATRIX d3dTempLight3Mat = *m_pLight3->GetMatrix();
		D3DXMATRIX d3dTempLight3Mat2 = *m_pLight3->GetMatrix();
		d3dTempLight3Mat._43 = m_nVelocityModifier * _fElapedTime * 0.09f;
		m_pLight3->SetMatrix(&(d3dTempLight3Mat * d3dTempLight3Mat2));
		D3DXVECTOR3 d3dLight3Pos = m_pLight3->GetPosition();
		d3dLight3Pos.z -= 1.29f;
		m_pLight3->SetPosition(d3dLight3Pos);

		D3DXMATRIX d3dTempLight4Mat = *m_pLight4->GetMatrix();
		D3DXMATRIX d3dTempLight4Mat2 = *m_pLight4->GetMatrix();
		d3dTempLight4Mat._43 = m_nVelocityModifier * _fElapedTime * 0.0555f;
		m_pLight4->SetMatrix(&(d3dTempLight4Mat * d3dTempLight4Mat2));
		D3DXVECTOR3 d3dLight4Pos = m_pLight4->GetPosition();
		d3dLight4Pos.z -= 1.29f;
		m_pLight4->SetPosition(d3dLight4Pos);
 	}
	if( m_bIsDisabled )
	{
		static float max = 8.25f;
		static float min = 8.05f;
		static int selectetdebris = 0;
		m_fDeathTimer -= _fElapedTime;
		if( m_fDeathTimer > 4.5f && (m_fDeathTimer < max && m_fDeathTimer > min) )
		{
			max -= 0.5f;
			min = max - 0.2f;
			m_bFirst = false;
			for( int i(0); i < 4; ++i )
			{
				CMissile* tempMissile = (CMissile*)(m_pObjectManager->ReturnArmamentToMasterList(eMISSILE));

				if(tempMissile != NULL)
				{
					D3DXMATRIX	d3dMissileMat;

					if(i%2 == 1)
						WwiseNS::SoundEvent(eEnemyExplosion, ePlay, true, this);

					tempMissile->SetObjectManager(m_pObjectManager);
					tempMissile->SetLifetime((float)i * 0.1f);
					tempMissile->SetTarget(0);
					m_pSpaceDibris[selectetdebris]->GetMatrix()->_11 = 1.0f;
					m_pSpaceDibris[selectetdebris]->GetMatrix()->_22 = 1.0f;
					m_pSpaceDibris[selectetdebris]->GetMatrix()->_33 = 1.0f;
					if( i % 2 == 0 )
					{
						tempMissile->SetOwner(this, m_pSpaceDibris[selectetdebris++]);
						if( selectetdebris >= BOSS_DEBRIS_COUNT )
						{
							selectetdebris = 0;
						}
					}
					tempMissile->SetActive(true);
					D3DXMatrixTranslation(&d3dMissileMat, (float)(rand()%500-250), (float)(rand()%300-150), (float)(rand()%1250-625));
					tempMissile->SetMatrix(&( (*GetMatrix()) * d3dMissileMat ));
					tempMissile->SetDoOnce(true);
					tempMissile->SetHealth(100000);
					tempMissile->SetVelocityModifier(0.0f);
					tempMissile->SetDamage(0);
					tempMissile->SetIsScenery(true);
					tempMissile->SetIsVisable(false);
					for (unsigned int i = 0; i < tempMissile->GetMovementBehaviorStack().size(); i++)
					{
						tempMissile->PopBehavior();
					}
				}
			}
		}
		else if( !m_bBigExplosion && m_fDeathTimer < 0.2f )
		{
			m_bBigExplosion = true;
			selectetdebris = BOSS_DEBRIS_COUNT;
			for( int i(0); i < 10; ++i )
			{
				CMissile* tempMissile = (CMissile*)(m_pObjectManager->ReturnArmamentToMasterList(eMISSILE));

				if(tempMissile != NULL)
				{
					D3DXMATRIX	d3dMissileMat;

					tempMissile->SetObjectManager(m_pObjectManager);
					tempMissile->SetLifetime((float)i * 0.01f);
					tempMissile->SetTarget(0);
					m_pSpaceDibris[selectetdebris]->GetMatrix()->_11 = 1.0f;
					m_pSpaceDibris[selectetdebris]->GetMatrix()->_22 = 1.0f;
					m_pSpaceDibris[selectetdebris]->GetMatrix()->_33 = 1.0f;
					tempMissile->SetOwner(this, m_pSpaceDibris[selectetdebris++]);
				
					m_pSpaceDibris[selectetdebris-1]->Scale(rand()%3+2.0f, rand()%3+2.0f, rand()%3+2.0f);
					m_pSpaceDibris[selectetdebris-1]->SetRotation(D3DXVECTOR3(0,0,0), 0.0f);
					if( selectetdebris == BOSS_DEBRIS_COUNT + 10 )
					{
						selectetdebris = BOSS_DEBRIS_COUNT;
					}
					tempMissile->SetActive(true);
					D3DXMatrixTranslation(&d3dMissileMat, (float)(rand()%500-250), (float)(rand()%300-150), (float)(rand()%1250-625));
					tempMissile->SetMatrix(&( (*GetMatrix()) * d3dMissileMat ));
					tempMissile->SetDoOnce(true);
					tempMissile->SetHealth(100000);
					tempMissile->SetVelocityModifier(0.0f);
					tempMissile->SetDamage(0);
					tempMissile->SetIsScenery(true);
					tempMissile->SetIsVisable(false);
					for (unsigned int i = 0; i < tempMissile->GetMovementBehaviorStack().size(); i++)
					{
						tempMissile->PopBehavior();
					}
				}
			}
		}

		if( m_fDeathTimer < 4.0f && !m_bPlayedBigExplosion )
		{
			EffectInformation::CreateEffect(eBOSS_BIG_EXPLOSION, &m_esBigExplosion, this, false, true);
			WwiseNS::SoundEvent(eEnemyExplosion, ePlay, true, this);
			m_bPlayedBigExplosion = true;
		}
		if(m_bPlayedBigExplosion == true && m_fDeathTimer > 0.5f && m_fDeathTimer < 2.0f)
		{
			int tempDelay = ((int)(m_fDeathTimer*10.0f)%3);
			if(tempDelay == 0)
			{
				if(m_nLastBigExplosionSound != ((int)(m_fDeathTimer*10.0f)))
				{
					WwiseNS::SoundEvent(eEnemyExplosion, ePlay, true, this);
					m_nLastBigExplosionSound = ((int)(m_fDeathTimer*10.0f));
				}
			}
		}

		if( m_fDeathTimer < 0.0f )
		{
		/*	for( int i(BOSS_DEBRIS_COUNT); i < (BOSS_DEBRIS_COUNT+10); ++i )
			{
				D3DXMATRIX mat;
				D3DXMatrixTranslation(&mat, (float)(rand()%500-250), (float)(rand()%300-150), (float)(rand()%1250-625));
				m_pSpaceDibris[i]->SetIsVisable(true);
				m_pSpaceDibris[i]->SetMatrix( &((*GetMatrix()) * mat));
				m_pSpaceDibris[i]->SetRotation(D3DXVECTOR3(rand()%10*0.1f,rand()%10*0.1f,rand()%10*0.1f), rand()%10*0.1f);
				m_pSpaceDibris[i]->Scale(rand()%20*0.1f+3.0f, rand()%20*0.1f+3.0f, rand()%20*0.1f+3.0f);
			}*/
			m_bIsVisable = false;
			m_pObjectManager->RemoveObject(this);
			///play AI victory speech based on time
			if(m_fBattleTimer > 150)
			{
				if(!m_bHasPlayedAIVictorySpeech)
				{
					//battle ended quickly
					WwiseNS::SoundEvent(eAIQuickVictory, ePlay);
					m_bHasPlayedAIVictorySpeech = true;
				}
			}
			else
			{
				if(!m_bHasPlayedAIVictorySpeech)
				{
					//battle ended.. but took too long
					WwiseNS::SoundEvent(eAILongVictory, ePlay);
					m_bHasPlayedAIVictorySpeech = true;
				}
			}
		}
	}
		D3DXVECTOR3 bossPosition = GetPosition();

	if( m_nPhase == eCINEMATIC_PHASE )
	{
		_fElapedTime *= 1000.0f;
		CinematicFunction(_fElapedTime);
	}
	else if( m_nPhase != eCINEMATIC_PHASE && startBoss == false)
	{
		m_bWarped = false;
		startBoss = true;
		//teachPhase1 = true;
		///////////// Generator
		m_pGenerators[0]->SetPosition(bossPosition.x + 455,  bossPosition.y,bossPosition.z);	
		//		m_pGenerators[0]->StartParticles();

		m_pGenerators[1]->SetPosition(bossPosition.x ,bossPosition.y + 398,bossPosition.z);	
		//m_pGenerators[1]->StartParticles();

		m_pGenerators[2]->SetPosition(bossPosition.x -455,bossPosition.y,bossPosition.z);	
		//m_pGenerators[2]->StartParticles();

		m_pBossWinTurret->SetPosition(bossPosition.x,bossPosition.y - 220,bossPosition.z+240);	
	}

		//shield
		m_vpCollisionHelper[0]->SetPosition(bossPosition.x, bossPosition.y, bossPosition.z - 400);
 		m_vpCollisionHelper[1]->SetPosition(bossPosition.x, bossPosition.y, bossPosition.z - 200);
 		m_vpCollisionHelper[2]->SetPosition(bossPosition.x, bossPosition.y, bossPosition.z);
 		m_vpCollisionHelper[3]->SetPosition(bossPosition.x, bossPosition.y, bossPosition.z + 200);
 		m_vpCollisionHelper[4]->SetPosition(bossPosition.x, bossPosition.y, bossPosition.z + 400);
 		m_vpCollisionHelper[5]->SetPosition(bossPosition.x, bossPosition.y, bossPosition.z + 600);
 		m_vpCollisionHelper[6]->SetPosition(bossPosition.x, bossPosition.y, bossPosition.z + 780);
 		m_vpCollisionHelper[7]->SetPosition(bossPosition.x, bossPosition.y, bossPosition.z - 780);
		//body
		m_vpCollisionHelper[8]->SetPosition(bossPosition.x - 45, bossPosition.y, bossPosition.z + 510);
		m_vpCollisionHelper[9]->SetPosition(bossPosition.x + 45, bossPosition.y, bossPosition.z + 510);
		m_vpCollisionHelper[10]->SetPosition(bossPosition.x, bossPosition.y - 80, bossPosition.z + 250);
		m_vpCollisionHelper[11]->SetPosition(bossPosition.x, bossPosition.y + 110, bossPosition.z + 250);
		m_vpCollisionHelper[12]->SetPosition(bossPosition.x, bossPosition.y - 80, bossPosition.z + 50);
		m_vpCollisionHelper[13]->SetPosition(bossPosition.x, bossPosition.y + 10, bossPosition.z + 50);
		m_vpCollisionHelper[14]->SetPosition(bossPosition.x, bossPosition.y, bossPosition.z - 115);
		m_vpCollisionHelper[15]->SetPosition(bossPosition.x, bossPosition.y + 120, bossPosition.z - 185);
		m_vpCollisionHelper[16]->SetPosition(bossPosition.x, bossPosition.y + 240, bossPosition.z - 230);
		m_vpCollisionHelper[17]->SetPosition(bossPosition.x, bossPosition.y, bossPosition.z - 630);
		m_vpCollisionHelper[18]->SetPosition(bossPosition.x, bossPosition.y, bossPosition.z - 350);


	if(startBoss)
	{
		CEnemy::Update(_fElapedTime);
		FixMode(-_fElapedTime);
		CheckWin(_fElapedTime);
		CheckPlayerSide();
//  		if (GetAsyncKeyState('N') & 1 )
//  		{
//  			m_fBattleTimer -= 10;
//  			for(int i = 0; i < 3; i++)
//  			{
//  				m_pGenerators[i]->SetActive(false);
//  			}
//  
//  		}
//  		if (CInput::DIKBufferedButtonCheck(DIK_UPARROW))
//  		{
//  			for (unsigned int i = 0; i < m_vpBossLargeTurrets.size(); i++)
//  			{
//  				m_vpBossLargeTurrets[i]->SetHealth(0);
//  			}
//  		}
//  		if (CInput::DIKBufferedButtonCheck(DIK_DOWNARROW))
//  		{
//  			m_pBossWinTurret->SetHealth(0);
//  		}
//		if (GetAsyncKeyState('N') & 1 )
//		{
//			cout <<"x:" << m_vpCollisionHelper[0]->GetPosition().x  <<"  y:" << m_vpCollisionHelper[0]->GetPosition().y  <<"  z:" << m_vpCollisionHelper[0]->GetPosition().z << endl;
//		}
//		if (GetAsyncKeyState(VK_UP))
//		{
//			D3DXVECTOR3 newPosition = m_vpCollisionHelper[0]->GetPosition();
//			newPosition.y += 5;
//			m_vpCollisionHelper[0]->SetPosition(newPosition);
//		}
//		if (GetAsyncKeyState(VK_DOWN))
//		{
//			D3DXVECTOR3 newPosition = m_vpCollisionHelper[0]->GetPosition();
//			newPosition.y -= 5;
//			m_vpCollisionHelper[0]->SetPosition(newPosition);
//		}
//		if (GetAsyncKeyState(VK_LEFT))
//		{
//			D3DXVECTOR3 newPosition = m_vpCollisionHelper[0]->GetPosition();
//			newPosition.x -= 5;
//			m_vpCollisionHelper[0]->SetPosition(newPosition);
//		}
//		if (GetAsyncKeyState(VK_RIGHT))
//		{
//			D3DXVECTOR3 newPosition = m_vpCollisionHelper[0]->GetPosition();
//			newPosition.x += 5;
//			m_vpCollisionHelper[0]->SetPosition(newPosition);
//		}
// 		if (GetAsyncKeyState('J'))
// 		{
// // 			float zMod = m_pBossWinTurret->GetZModifier();
// // 			zMod -= 5;
// // 			m_pBossWinTurret->SetZModifier(zMod);
// 			D3DXVECTOR3 newPosition = m_vpCollisionHelper[0]->GetPosition();
// 			newPosition.z -= 5;
// 			m_vpCollisionHelper[0]->SetPosition(newPosition);
// 		}
// 		if (GetAsyncKeyState('K'))
// 		{
// // 			float zMod = m_pBossWinTurret->GetZModifier();
// // 			zMod += 5;
// // 			m_pBossWinTurret->SetZModifier(zMod);
// 			D3DXVECTOR3 newPosition = m_vpCollisionHelper[0]->GetPosition();
// 			newPosition.z += 5;
// 			m_vpCollisionHelper[0]->SetPosition(newPosition);
// 		}
		static bool m_bTimerHasRunOut = false;
		if(m_nPhase != eSEEK_AND_DESTROY && m_nPhase != eCINEMATIC_PHASE)
		{
			m_fBattleTimer -= _fElapedTime;
			if( m_fBattleTimer < 0 )
			{
				m_pObjectManager->SetPlayerLose(true);
				if(!m_bTimerHasRunOut)
				{ 
					WwiseNS::SoundEvent(eHydraTimerOut, ePlay);
					m_bTimerHasRunOut = true;
				}
				m_fBattleTimer = 0.0f;
				m_fDeathWaitTime += _fElapedTime;
			}
		}
	}
	WwiseNS::SetBossPos(m_d3dMatrix);


}
void  CBoss::CheckWin(float _time)
{
	numAttachments = 0;
	m_nNumHangers = 0;
	m_nNumTurrets = 0;
	m_fHealthCounter = 0;

	if (m_fBattleTimer <= 0.0f)
	{
		if( m_fDeathWaitTime > 20.0f )
		{
			m_bPlayerLose = true;
		}
		else if(m_fDeathWaitTime > 20.0f)
		{			
			m_bFireLaser = false;
			m_bStopFiringLaser = true;
		}
		else if(m_fDeathWaitTime > 8.5f)
		{			
			m_pPlanet->Destroy();
		}
		//else if(m_fDeathWaitTime > 14.5f)
		//{
		//	m_pPlanet->Destroy();
		//}

		//m_bPlayerLose = true;
		// Fire laser at planet and destroy it!
		//*************************************
		m_pBossWinTurret->SetTarget(m_pPlanet);
		m_pBossWinTurret->SetShouldFireAtPlanet(true);
		//*************************************
	}
	unsigned int largeSize = m_vpBossLargeTurrets.size();
	for (unsigned int i = 0; i < largeSize; i++)
	{
		if (m_vpBossLargeTurrets[i]->GetActive() == true  )
		{
			numAttachments++;
			m_nNumHangers++;
		}
	}
	bool nowTransition = false;

	if ( numAttachments <= 0)
	{
		transitionTimer2 += 0.075f;
		if (transitionTimer2 >= 2.0f)
		{
			nowTransition = true;
		}
	}
	if(nowTransition)
	{
		if (showHanger)
		{
			m_pObjectManager->GetIndicatorList()->push_back(m_pBossWinTurret);
			showHanger = false;
			m_bTransitionPhaseText2To3 = true;
			m_pBossWinTurret->SetShields(1);
			m_pBossWinTurret->SetRechargeTimer(30.0f);
			m_pBossWinTurret->SetHealth(1);
			m_fHealthCounter = 1;
			m_bNewPhase = true;
			WwiseNS::SoundEvent(eHydraThrusters, eStop);
		}
		//TODO draw what phase in large bold
		m_nPhase = eSEEK_AND_DESTROY;
	}
	unsigned int turretSize =  m_vpBossTurrets.size();
	for (unsigned int i = 0; i < turretSize; i++)
	{
		if (m_vpBossTurrets[i]->GetActive() == true  )
		{
			m_nNumTurrets++;

		}
	}
	m_fHealthCounter+=m_pBossWinTurret->GetHealth();

	if (m_bMaxHealth)
	{
		m_nMaxHealth = 1400;
		m_bMaxHealth = false;
	}

	if (numAttachments <= 0)
	{
		//m_bIsDisabled = true;
	}

	if ( m_pBossWinTurret->GetActive() == false || m_pTarget->GetActive() == false)
	{
		for (unsigned int i = 0; i < largeSize; i++)
		{
			if (m_vpBossLargeTurrets[i]->GetActive() == true  )
			{
				m_vpBossLargeTurrets[i]->SetHealth(0);
				m_vpBossLargeTurrets[i]->SetActive(0);
				m_vpBossLargeTurrets[i]->SetIsVisable(false);
			}
		}

		for (unsigned int i = 0; i < turretSize; i++)
		{
			if (m_vpBossTurrets[i]->GetActive() == true  )
			{
				m_vpBossTurrets[i]->SetHealth(0);
				m_vpBossTurrets[i]->SetActive(0);
				m_vpBossTurrets[i]->SetIsVisable(false);
			}
		}

		if( !m_bIsDisabled )
		{
			/*
			int nextDibris = 0;
			for( int i(0); i < 20; ++i )
			{
				CMissile* tempMissile = (CMissile*)(m_pObjectManager->ReturnArmamentToMasterList(eMISSILE));

				if(tempMissile != NULL)
				{
					D3DXMATRIX	d3dMissileMat;
					int diceRoll = rand()%20;
					if(!diceRoll)
					{
						WwiseNS::SoundEvent(eEnemyExplosion, ePlay, true, this);
					} 
					tempMissile->SetObjectManager(m_pObjectManager);
					tempMissile->SetTarget(0);
					tempMissile->SetActive(true);
					if( i > 10 )
					{
						tempMissile->SetOwner(this, m_pSpaceDibris[nextDibris++]);
					}	
					else
					{
						tempMissile->SetOwner(this, nullptr);
					}

					float randomized = rand()%10*0.1f;
					float squared_halfed = i*0.1f;
					float life = randomized + squared_halfed + (i*0.2f) + 0.5f;
					tempMissile->SetLifetime(life);
					D3DXMatrixTranslation(&d3dMissileMat, (float)(rand()%200-100), (float)(rand()%100-50), (float)(rand()%400-200));
					tempMissile->SetMatrix(&( (*GetMatrix()) * d3dMissileMat ));
					tempMissile->SetDoOnce(true);
					tempMissile->SetHealth(100000);
					tempMissile->SetVelocityModifier(0.0f);
					tempMissile->SetDamage(0);
					tempMissile->SetIsVisable(false);
					for (unsigned int i = 0; i < tempMissile->GetMovementBehaviorStack().size(); i++)
					{
						tempMissile->PopBehavior();
					}
				}
			}
			*/
		}

		m_bIsDisabled = true;
	}
}



void  CBoss::FixMode(float _time)
{
	int genCount = 0;
	generatorHealth = 0;
	for (int numGenerators = 0; numGenerators < 3; ++numGenerators)
	{
		if (m_pGenerators[numGenerators]->GetActive())
		{
			genCount++;
			generatorHealth += m_pGenerators[numGenerators]->GetHealth();
		}
	}
	m_nNumGenerators = genCount;

	bool nowTransition = false;

	if (m_nNumGenerators == 0)
	{
		transitionTimer += 0.075f;
		if (transitionTimer >= 2.0f)
		{
			nowTransition = true;
		}
	}
	if (nowTransition)
	{
		//TODO draw what phase in large bold
		m_nPhase = eMISSILE_PHASE;
		m_nShieldStatus = eSHIELDS_DOWN;

		if (showTurret)
		{
			m_pBossThruster = new CBossThruster(this, D3DXVECTOR3(75.0f, 155.0f, -880.0f), 80.0f);
			m_pBossThruster->SetPosition(GetPosition().x, GetPosition().y, GetPosition().z -460);
			m_pObjectManager->AddObject(m_pBossThruster);
			m_pBossThruster = new CBossThruster(this, D3DXVECTOR3(-75.0f, 155.0f, -880.0f), 80.0f);
			m_pBossThruster->SetPosition(GetPosition().x, GetPosition().y, GetPosition().z -460);
			m_pObjectManager->AddObject(m_pBossThruster);
			/////
			//bot 2 thrusters
			m_pBossThruster = new CBossThruster(this, D3DXVECTOR3(75.0f, 0.0f, -895.0f), 80.0f);
			m_pBossThruster->SetPosition(GetPosition().x, GetPosition().y, GetPosition().z -440);
			m_pBossThruster->SetActive(false);
			m_pObjectManager->AddObject(m_pBossThruster);
			m_pBossThruster = new CBossThruster(this, D3DXVECTOR3(-75.0f, 0.0f, -895.0f), 80.0f);
			m_pBossThruster->SetPosition(GetPosition().x, GetPosition().y, GetPosition().z -440);
			m_pObjectManager->AddObject(m_pBossThruster);

			m_bNewPhase = true;
			WwiseNS::SoundEvent(eHydraShieldGenDown, ePlay);
			m_bTransitionPhaseText1To2 = true;
			unsigned int size = m_vpBossLargeTurrets.size();
			for (unsigned int i = 0; i < size; i++)
			{
				m_pObjectManager->GetIndicatorList()->push_back(m_vpBossLargeTurrets[i]);
			}

			showTurret = false;
		}
	}


}

void CBoss::CreateCollisionSpheres()
{
		///shield collision spheres
	D3DXVECTOR3 bossPosition = GetPosition();
	CEnvironmentObject * collisionHelper = new CEnvironmentObject(eSPACE_DEBRIS,bossPosition, D3DXVECTOR3(0,0,0), true, 400.0f);
	collisionHelper->SetPosition( bossPosition.x, bossPosition.y, bossPosition.z - 400 );
	collisionHelper->SetIsVisable(false);
	collisionHelper->SetDuringShield(true);
	m_vpCollisionHelper.push_back(collisionHelper);
	m_pObjectManager->AddObject(collisionHelper);

 	collisionHelper = new CEnvironmentObject(eSPACE_DEBRIS,bossPosition, D3DXVECTOR3(0,0,0), true, 450.0f);
 	collisionHelper->SetPosition( bossPosition.x, bossPosition.y, bossPosition.z - 200 );
 	collisionHelper->SetIsVisable(false);
 	collisionHelper->SetDuringShield(true);
 	m_vpCollisionHelper.push_back(collisionHelper);
 	m_pObjectManager->AddObject(collisionHelper);
 
 	collisionHelper = new CEnvironmentObject(eSPACE_DEBRIS,bossPosition, D3DXVECTOR3(0,0,0), true, 437.0f);
 	collisionHelper->SetPosition( bossPosition.x, bossPosition.y, bossPosition.z );
 	collisionHelper->SetIsVisable(false);
 	collisionHelper->SetDuringShield(true);
 	m_vpCollisionHelper.push_back(collisionHelper);
 	m_pObjectManager->AddObject(collisionHelper);
 
 	collisionHelper = new CEnvironmentObject(eSPACE_DEBRIS,bossPosition, D3DXVECTOR3(0,0,0), true, 450.0f);
 	collisionHelper->SetPosition( bossPosition.x, bossPosition.y, bossPosition.z + 200);
 	collisionHelper->SetIsVisable(false);
 	collisionHelper->SetDuringShield(true);
 	m_vpCollisionHelper.push_back(collisionHelper);
 	m_pObjectManager->AddObject(collisionHelper);
 
 	collisionHelper = new CEnvironmentObject(eSPACE_DEBRIS,bossPosition, D3DXVECTOR3(0,0,0), true, 400.0f);
 	collisionHelper->SetPosition( bossPosition.x, bossPosition.y, bossPosition.z + 400);
 	collisionHelper->SetIsVisable(false);
 	collisionHelper->SetDuringShield(true);
 	m_vpCollisionHelper.push_back(collisionHelper);
 	m_pObjectManager->AddObject(collisionHelper);
 
 	collisionHelper = new CEnvironmentObject(eSPACE_DEBRIS,bossPosition, D3DXVECTOR3(0,0,0), true, 240.0f);
 	collisionHelper->SetPosition( bossPosition.x, bossPosition.y, bossPosition.z + 600);
 	collisionHelper->SetIsVisable(false);
 	collisionHelper->SetDuringShield(true);
 	m_vpCollisionHelper.push_back(collisionHelper);
 	m_pObjectManager->AddObject(collisionHelper);
 
 	collisionHelper = new CEnvironmentObject(eSPACE_DEBRIS,bossPosition, D3DXVECTOR3(0,0,0), true, 240.0f);
 	collisionHelper->SetPosition( bossPosition.x, bossPosition.y, bossPosition.z + 800);
 	collisionHelper->SetIsVisable(false);
 	collisionHelper->SetDuringShield(true);
 	m_vpCollisionHelper.push_back(collisionHelper);
 	m_pObjectManager->AddObject(collisionHelper);
 
 	collisionHelper = new CEnvironmentObject(eSPACE_DEBRIS,bossPosition, D3DXVECTOR3(0,0,0), true, 240.0f);
 	collisionHelper->SetPosition( bossPosition.x, bossPosition.y, bossPosition.z - 800);
 	collisionHelper->SetIsVisable(false);
 	collisionHelper->SetDuringShield(true);
 	m_vpCollisionHelper.push_back(collisionHelper);
 	m_pObjectManager->AddObject(collisionHelper);
	//////////////////////////////////////////////////////////////////////////
	//non shielded collision spheres

	collisionHelper = new CEnvironmentObject(eSPACE_DEBRIS,bossPosition, D3DXVECTOR3(0,0,0), true, 200.0f);
	collisionHelper->SetPosition( bossPosition.x, bossPosition.y, bossPosition.z + 480);
	collisionHelper->SetIsVisable(false);
	collisionHelper->SetDuringShield(false);
	m_vpCollisionHelper.push_back(collisionHelper);
	m_pObjectManager->AddObject(collisionHelper);


	collisionHelper = new CEnvironmentObject(eSPACE_DEBRIS,bossPosition, D3DXVECTOR3(0,0,0), true, 200.0f);
	collisionHelper->SetPosition( bossPosition.x, bossPosition.y, bossPosition.z + 480);
	collisionHelper->SetIsVisable(false);
	collisionHelper->SetDuringShield(false);
	m_vpCollisionHelper.push_back(collisionHelper);
	m_pObjectManager->AddObject(collisionHelper);


	collisionHelper = new CEnvironmentObject(eSPACE_DEBRIS,bossPosition, D3DXVECTOR3(0,0,0), true, 140.0f);
	collisionHelper->SetPosition( bossPosition.x, bossPosition.y, bossPosition.z + 250);
	collisionHelper->SetIsVisable(false);
	collisionHelper->SetDuringShield(false);
	m_vpCollisionHelper.push_back(collisionHelper);
	m_pObjectManager->AddObject(collisionHelper);

	collisionHelper = new CEnvironmentObject(eSPACE_DEBRIS,bossPosition, D3DXVECTOR3(0,0,0), true, 120.0f);
	collisionHelper->SetPosition( bossPosition.x, bossPosition.y+50, bossPosition.z + 250);
	collisionHelper->SetIsVisable(false);
	collisionHelper->SetDuringShield(false);
	m_vpCollisionHelper.push_back(collisionHelper);
	m_pObjectManager->AddObject(collisionHelper);

	collisionHelper = new CEnvironmentObject(eSPACE_DEBRIS,bossPosition, D3DXVECTOR3(0,0,0), true, 130.0f);
	collisionHelper->SetPosition( bossPosition.x, bossPosition.y, bossPosition.z + 250);
	collisionHelper->SetIsVisable(false);
	collisionHelper->SetDuringShield(false);
	m_vpCollisionHelper.push_back(collisionHelper);
	m_pObjectManager->AddObject(collisionHelper);

	collisionHelper = new CEnvironmentObject(eSPACE_DEBRIS,bossPosition, D3DXVECTOR3(0,0,0), true, 100.0f);
	collisionHelper->SetPosition( bossPosition.x, bossPosition.y, bossPosition.z + 250);
	collisionHelper->SetIsVisable(false);
	collisionHelper->SetDuringShield(false);
	m_vpCollisionHelper.push_back(collisionHelper);
	m_pObjectManager->AddObject(collisionHelper);

	collisionHelper = new CEnvironmentObject(eSPACE_DEBRIS,bossPosition, D3DXVECTOR3(0,0,0), true, 120.0f);
	collisionHelper->SetPosition( bossPosition.x, bossPosition.y, bossPosition.z - 250);
	collisionHelper->SetIsVisable(false);
	collisionHelper->SetDuringShield(false);
	m_vpCollisionHelper.push_back(collisionHelper);
	m_pObjectManager->AddObject(collisionHelper);

	collisionHelper = new CEnvironmentObject(eSPACE_DEBRIS,bossPosition, D3DXVECTOR3(0,0,0), true, 95.0f);
	collisionHelper->SetPosition( bossPosition.x, bossPosition.y, bossPosition.z - 250);
	collisionHelper->SetIsVisable(false);
	collisionHelper->SetDuringShield(false);
	m_vpCollisionHelper.push_back(collisionHelper);
	m_pObjectManager->AddObject(collisionHelper);

	
	collisionHelper = new CEnvironmentObject(eSPACE_DEBRIS,bossPosition, D3DXVECTOR3(0,0,0), true, 60.0f);
	collisionHelper->SetPosition( bossPosition.x, bossPosition.y, bossPosition.z - 250);
	collisionHelper->SetIsVisable(false);
	collisionHelper->SetDuringShield(false);
	m_vpCollisionHelper.push_back(collisionHelper);
	m_pObjectManager->AddObject(collisionHelper);

	collisionHelper = new CEnvironmentObject(eSPACE_DEBRIS,bossPosition, D3DXVECTOR3(0,0,0), true, 270.0f);
	collisionHelper->SetPosition( bossPosition.x, bossPosition.y, bossPosition.z - 250);
	collisionHelper->SetIsVisable(false);
	collisionHelper->SetDuringShield(false);
	m_vpCollisionHelper.push_back(collisionHelper);
	m_pObjectManager->AddObject(collisionHelper);

	collisionHelper = new CEnvironmentObject(eSPACE_DEBRIS,bossPosition, D3DXVECTOR3(0,0,0), true, 230.0f);
	collisionHelper->SetPosition( bossPosition.x, bossPosition.y, bossPosition.z - 250);
	collisionHelper->SetIsVisable(false);
	collisionHelper->SetDuringShield(false);
	m_vpCollisionHelper.push_back(collisionHelper);
	m_pObjectManager->AddObject(collisionHelper);
}
void CBoss::MoveAttachments()
{
	D3DXVECTOR3 bossPos = GetPosition();
	for (int i = 0; i < 3; ++i)
	{
		m_pGenerators[i]->SetPosition(bossPos.x, bossPos.y, bossPos.z);
	}
}
/*****************************************************************
* Render():		Draws the boss effects.				
* Ins:			    None    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CBoss::Render() 
{

}

void CBoss::CinematicFunction(float _fDt)
{	
	if( _fDt < 0.24f )
	{
		m_fEntranceDelay += _fDt;
	}
	if( m_fEntranceDelay < 2.05f )
	{
		m_bIsVisable = false;
		return;
	}
	else
	{
		m_bTeachBoss = true;
		m_bIsVisable = true;
	}
	D3DXMATRIX d3dMat;
	D3DXVECTOR3 d3dPos;
	d3dPos.x = m_d3dmDestination.x - m_d3dMatrix._41;
	d3dPos.y = m_d3dmDestination.y - m_d3dMatrix._42;
	d3dPos.z = m_d3dmDestination.z - m_d3dMatrix._43;

	float dist = D3DXVec3Length(&d3dPos);

	D3DXVec3Normalize(&d3dPos, &d3dPos);

	D3DXMatrixTranslation(&d3dMat,	d3dPos.x *dist*_fDt, 
		d3dPos.y *dist*_fDt, 
		d3dPos.z *dist*_fDt);

	m_d3dMatrix =  m_d3dMatrix * d3dMat;	
	if( dist > 500.0f )
	{
		CPhysics::LookAt(this, m_d3dmDestination);	
	}

	if( !m_bWarped && dist < 6000)
	{
		EffectInformation::CreateEffect(eBOSS_WARPIN, &m_esEntranceWarp, this, false, true);
		m_bWarped = true;
	}

	m_d3dMatrix._11 = 40.0f;
	m_d3dMatrix._22 = 40.0f;
	m_d3dMatrix._33 = 40.0f;

	if( m_d3dMatrix._41 != 0 )
		int x = 0;
}

void CBoss::DrawIntro(int _nTexture)
{
	if (m_bTeachBoss && m_bShowTeachBoss)
	{
		xPos+=5;
		alphaTeach++;
		if (alphaTeach>= 145)
		{
			alphaTeach = 145;
		}
		if (xPos >= 100)
		{
			xPos = 100;

		}
	}
	else
	{
		xPos-=3;
		alphaTeach-=3;
		if (alphaTeach <= 0)
		{
			alphaTeach = 0;
		}
		if (xPos <= -600)
		{
			m_bShowTeachBoss = false;
		}
	}


	char szBuff[255];
	char szBuff2[255];
	char szBuff3[255];
	sprintf_s(szBuff2, "IDF HYDRA");
	sprintf_s(szBuff3, "\n\n\n\n\n\nTIPS");
	sprintf_s(szBuff, "\nHealth   : VERY HIGH\nVelocity : HIGH\nFire Rate: High\nDamage : VERY HIGH\n\n\n  - Heading to Destroy Your Home\n  - 5 Minutes to stop him. \n");
	
	D3DXMATRIX tmpBackMat = m_BlackMat;
	tmpBackMat._41 += xPos * m_pRenderer->GetXRatio();
	m_pRenderer->RenderArrowAndrewR(_nTexture, &m_rBlack, tmpBackMat, D3DCOLOR_ARGB(alphaTeach,255,255,255), true, false);

	m_pRenderer->RenderText(szBuff2,xPos, TEXT_Y_POSITION-10, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,0, 220, 220));
	m_pRenderer->RenderText(szBuff3, xPos, TEXT_Y_POSITION-10, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,0, 220, 220));
	m_pRenderer->RenderText(szBuff, xPos, TEXT_Y_POSITION-10, 0.7f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,245,245,245));

	if (whichFade)
	{
		m_fSkipTextAlpha -= 3;
		if (m_fSkipTextAlpha <= 0)
		{
			
			m_fSkipTextAlpha = 0;
			whichFade = !whichFade;
		}
	}
	else
	{
		m_fSkipTextAlpha += 3;
		if (m_fSkipTextAlpha >= 255) 
		{
			if(m_bHasPlayedAIHydranceEntrance == false)
			{
				WwiseNS::StopAllTutorialVoices(eAIHydraEntrance);
				WwiseNS::SoundEvent(eAIHydraEntrance, ePlay);
				m_bHasPlayedAIHydranceEntrance = true;
			}
			m_fSkipTextAlpha = 255;
			whichFade = !whichFade;

		}
	}

	char szBuff4[32];
	sprintf_s(szBuff4, "Press SPACE to Skip");
	m_pRenderer->RenderText(szBuff4, 739, SKIP_TEXT_Y_POSITION-1, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_fSkipTextAlpha,0,0,0));
	m_pRenderer->RenderText(szBuff4, 740, SKIP_TEXT_Y_POSITION, 0.9f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_fSkipTextAlpha,245,245,245));

}

void CBoss::SetInCinematic(bool _inCinematic)
{
	if( _inCinematic )
	{
		m_nPhase = eCINEMATIC_PHASE;
		m_bShowTeachBoss = true;
	}
	else
	{ 
		if( m_nPhase == eCINEMATIC_PHASE )
		{
			m_pObjectManager->RegenerateMisc(GetPosition());
		}
		m_nPhase = eLASER_PHASE;
		// 		for( int i(0); i < 3; ++i )
		// 		{
		// 			m_pGenerators[i]->Activate();
		// 		}
	}
}

CCollOBJ* CBoss::GetBV()
{
	if(generatorHealth)
	{
		return m_pShieldBV;
	}

	return m_pBV;
}

void CBoss::AddEffectFromCollision( CEntity* _pOwner )
{
	CEffectSystem* tmpEffect = new CEffectSystem();
	EffectInformation::CreateEffect(eASTEROID_TAIL, tmpEffect, _pOwner, false, true);
	//m_vCinematicParticles.push_back(*tmpEffect);
}
