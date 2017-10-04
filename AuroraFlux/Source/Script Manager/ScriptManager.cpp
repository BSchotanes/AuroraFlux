/***********************************************
* Filename:  		ScriptManager.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	
* Author:    		Stephen warren
* Purpose:   		This class will handle the loading of all the enemies based
*					on the waypoints. The waypoints have multiple events which 
*					each spawn enemies based off the waypoint.
************************************************/

#include "../StdAfx.h"
#include "ScriptManager.h"
#include "../Entity/RedEnemy.h"
#include "../Entity/Player.h"
#include "../Entity/Shield.h"
#include "../State System/Game.h"
#include "../AI/AIHelper.h"


 bool CScriptManager::m_bTutorial = true;

/*****************************************************************
* CScriptManager(): Overload Constructor will take in the Object Manager.
*					The player, and the game
* Ins:			    CObjectManager*, CEntity *, CGame*   		      
* Outs:				None
* Returns:		    None
* Mod. Date:		
* Mod. Initials:	
*****************************************************************/
CScriptManager::CScriptManager(CObjectManager* _pOM, CEntity * _pPlayer, CGame* _pGame, CAIHelper *_AIHelper, CRenderer * _pRenderer, CDialogSystem * _pDialog)
	: m_pOBJManager(_pOM), m_pCurrWaypoint(), m_unCurrentEventIndex(0), m_bInitialEvent(true), 
	m_pThePlayer(_pPlayer), m_pGame(_pGame), m_bSpawnBoss(false), pTheBoss(0), loadAsteroids(true), lastWave(0), m_pDialog(_pDialog), m_bCompletedObjective(true),
	m_cCurrentTutorial(-1), m_fTimer(0.0f), m_GoodJobTimer(0.0f), m_bAddedWaypoint(false), m_bSpawnYellowEnemies(false)
{
	m_pTutorialEnemy = nullptr;
	doOnce = true;
	m_pRenderer = _pRenderer;
	s_AIHelper = _AIHelper;
	m_fSpawnTimer = 0;
	m_bSpawnBool = false;
	m_pCurrWaypoint = new CWaypoint;
	m_pCurrWaypoint->SetActive(true);
	m_pCurrWaypoint->Scale(20.0f, 20.0f, 20.0f);	

    planet = new CEnvironmentObject(ePLANET1, D3DXVECTOR3(-5000,-3000,59000) );
	
	planet->Scale(35,35,35);
	m_pOBJManager->AddObject(planet);

	m_pLight1 = new CEnvironmentObject(eSPACE_STATION,D3DXVECTOR3(-13000,8000,41550));
	m_pLight1->SetIsVisable(false);
	m_pLight1->SetMainMenuType(6);
	m_pLight1->Scale(.025f,.025f,.025f);
	m_pOBJManager->AddObject(m_pLight1);

	m_pLight2 = new CEnvironmentObject(eSPACE_STATION,D3DXVECTOR3(-22000,-6000,40550));
	m_pLight2->SetIsVisable(false);
	m_pLight2->SetMainMenuType(6);
	m_pLight2->Scale(.025f,.025f,.025f);
	m_pOBJManager->AddObject(m_pLight2);

	m_pLight3 = new CEnvironmentObject(eSPACE_STATION,D3DXVECTOR3(-11000,-6000,39550));
	m_pLight3->SetIsVisable(false);
	m_pLight3->SetMainMenuType(6);
	m_pLight3->Scale(.025f,.025f,.025f);
	m_pOBJManager->AddObject(m_pLight3);

	m_pLight4 = new CEnvironmentObject(eSPACE_STATION,D3DXVECTOR3(-11000,-6000,39550));
	m_pLight4->SetIsVisable(false);
	m_pLight4->SetMainMenuType(6);
	m_pLight4->Scale(.025f,.025f,.025f);
	m_pOBJManager->AddObject(m_pLight4);


	//////////////////////////
	//reset the bools for the audio queues
	m_bHasPlayedMoveMouse = false;
	m_bHasPlayedSpeedUpSlowDown = false;
	m_bHasPlayedGreatJob = false;
	m_bHasPlayedFireWeapon = false;
	m_bHasPlayedDodgeRoll = false;
	m_bHasPlayedDodgeRollCooldown = false;
	m_bHasPlayedSwitchModes = false;
	m_bHasPlayedModeWeapon = false;
	m_bHasPlayedQuickTurn = false;
	m_bHasPlayedQuickTurnReposition = false;
	m_bHasPlayedLookAnEnemy = false;
}

/*****************************************************************
* ~CScriptManager():     Destructor to take care of all the memory . 
* Ins:					 None    		      
* Outs:				 	 None
* Returns:				 None
* Mod. Date:		
* Mod. Initials:	
*****************************************************************/
CScriptManager::~CScriptManager(void)
{

}

/*****************************************************************
* Update():   Updates the Script to the correct script, 
*					 returns true if the game is over  
* Ins:			   float _dt    		      
* Outs:			   None
* Returns:		   bool
* Mod. Date:		
* Mod. Initials:	
*****************************************************************/
bool CScriptManager::Update( float _fDeltaTime )
{
	//m_pDialog->Update(_fDeltaTime);

	if (loadAsteroids)
	{
		loadAsteroids = false;
		CreateAsteroids();
	}

	if(CInput::DIKBufferedButtonCheck(DIK_M, true) && m_bTutorial)
	{
		if(m_pCinematicCamera->GetIsIntroCinematic() == false)
		{
			WwiseNS::StopAllTutorialVoices(-1);
			m_bTutorial = false;
			m_pCurrWaypoint->SetShouldScale(true);
			m_pCurrWaypoint->SetScaleDown(false);
			WwiseNS::SoundEvent(eProceedToWaypoint, ePlay);
			m_pDialog->ChangeSection(eTutorial_Waypoint_Arrow);
		}
	}

	if(m_bTutorial)
	{
		Tutorial( _fDeltaTime );
		return false;
	}
	else if(!m_bAddedWaypoint)
	{
		if(m_pCinematicCamera->GetIsIntroCinematic() == false)
		{
			m_pOBJManager->AddObject(m_pCurrWaypoint);
			m_bAddedWaypoint = true;
		}
	}

	if(m_pCurrWaypoint->GetActive() == false)
	{
		return false;
	}

	if(m_fSpawnTimer > 0)
	{
		m_fSpawnTimer -= _fDeltaTime;
		if(m_fSpawnTimer <= 0)
		{
			m_fSpawnTimer = 0;
			m_bSpawnBool = true;
		}
	}
// 	if (m_bSpawnBoss)
// 	{
// 		BossDestroyAsteroids(_fDeltaTime);
// 	}

// 	if(GetAsyncKeyState('Z')&1)
// 	{
// 		for (unsigned int uiKill = 0; uiKill < m_vpCurrentEnemies.size(); uiKill++)
// 		{
// 			m_vpCurrentEnemies[uiKill]->SetActive(false);
// 		}
// 	}

	
	if(m_bInitialEvent == true && m_pCurrWaypoint->IsTriggered() && m_pCinematicCamera->GetIsIntroCinematic() == false)
	{
		if(m_fSpawnTimer == 0 && m_bSpawnBool == false)
		{
			m_fSpawnTimer = 1.5f;
		}
		else if(m_bSpawnBool)
		{
			m_bInitialEvent = false;
			m_CurrWaveIter = m_vScript[m_unCurrentEventIndex].m_vWave.begin();

			if ((m_CurrWaveIter->m_cNextSpawn != 5000))
			{
				CreateEnemies();
				if(m_unCurrentEventIndex == 0)
				{
					m_pGame->GetDialogSys()->ChangeSection(eDialog_RedEnemy);
				}
				if(m_unCurrentEventIndex == 1)
				{
					m_pGame->GetDialogSys()->ChangeSection(eDialog_BlueEnemy);
				}
			}

			m_bSpawnBool = false;
		}
	}

	if(m_bInitialEvent)
	{
		return false;
	}

	if (m_CurrWaveIter->m_cNextSpawn == -1 && m_vpCurrentEnemies.size() <= 0 ) //TODO this is breaking last waypoint
	{
		D3DXMATRIX d3dTempPlayer; //= m_pThePlayer->GetMatrixCopy();
		D3DXMatrixIdentity(&d3dTempPlayer);
		CPhysics::SetAxis(ePOS, d3dTempPlayer, m_vScript[m_unCurrentEventIndex].m_d3dWaypointPos);

		if(m_unCurrentEventIndex + 1 < m_vScript.size())
		{
			D3DXVECTOR3 zVec = m_vScript[m_unCurrentEventIndex + 1].m_d3dWaypointPos - CPhysics::GetMat4Vec(ePOS, &d3dTempPlayer);
			D3DXVec3Normalize(&zVec, &zVec);
			CPhysics::SetAxis(eZAXIS, d3dTempPlayer, zVec);
			CPhysics::SetAxis(eXAXIS, d3dTempPlayer, D3DXVECTOR3(1.0f,0.0f,0.0f));
			CPhysics::SetAxis(eYAXIS, d3dTempPlayer, D3DXVECTOR3(0.0f,1.0f,0.0f));
		}

		m_pGame->GetCamera()->SetCheckpointCamera(d3dTempPlayer);
		m_pGame->SetCheckPoint(m_unCurrentEventIndex);

		m_unCurrentEventIndex++;

		if(m_unCurrentEventIndex < m_vScript.size() ) // shouldnt be skipped
		{
			//set waypoint id here
	
			m_pCurrWaypoint->SetShouldScale(true);
			m_pCurrWaypoint->SetScaleDown(false);

			m_bInitialEvent = true;
			m_pCurrWaypoint->SetPosition( m_vScript[m_unCurrentEventIndex].m_d3dWaypointPos );
			m_pGame->SetCheckPoint( m_unCurrentEventIndex);
			m_pCurrWaypoint->ResetTriggerParticle();
		}
		else
		{
			//m_pCurrWaypoint->SetActive(false);
		}
	}

	vector<CEntity*>::iterator iEnemyIter; 
	for(iEnemyIter = m_vpCurrentEnemies.begin(); iEnemyIter != m_vpCurrentEnemies.end(); )
	{
		if((*iEnemyIter)->GetActive() == false)
		{
			iEnemyIter = m_vpCurrentEnemies.erase(iEnemyIter);
		}
		else
		{
			++iEnemyIter;
		}
	}
	for(iEnemyIter = m_vpCurrentYellowEnemies.begin(); iEnemyIter != m_vpCurrentYellowEnemies.end(); )
	{
		if((*iEnemyIter)->GetActive() == false)
		{
			iEnemyIter = m_vpCurrentYellowEnemies.erase(iEnemyIter);
		}
		else
		{
			++iEnemyIter;
		}
	}

	if( (int)m_vpCurrentEnemies.size() <= m_CurrWaveIter->m_cNextSpawn  )
	{
		if (m_CurrWaveIter->m_cNextSpawn != 5000)
		{
			if(m_fSpawnTimer == 0 && m_bSpawnBool == false)
			{
				m_fSpawnTimer = 1.5f;
			}
			else if(m_bSpawnBool)
			{
				m_CurrWaveIter++;
				m_bSpawnYellowEnemies = true;
				CreateEnemies();

				m_bSpawnBool = false;
			}
		}
	}


	if(m_CurrWaveIter->m_cNextSpawn == 5000 && m_pCurrWaypoint->IsTriggered() && doOnce )
	{
		if ( D3DXVec3Length( &( m_pThePlayer->GetPosition() - m_pCurrWaypoint->GetPosition())   ) <= 3000 )
		{
			m_bSpawnBoss = true;
			doOnce = false;

			WwiseNS::SoundEvent(eBGM_BossMusic, ePlay);
			pTheBoss = new CBoss((CPlayer*)m_pThePlayer, m_pOBJManager, m_pRenderer , D3DXVECTOR3(0,0,0), planet, m_pLight1, m_pLight2, m_pLight3, m_pLight4);	
			pTheBoss->SetDestinationPosition(0,0,m_pCurrWaypoint->GetPosition().z - 1500);
			m_pOBJManager->AddObject(pTheBoss);



			//TODO::Jayson - If there is an explaination for boss, put it here.
			//m_pGame->GetDialogSys()->ChangeSection(eDialog_Boss);


		/*	pTheBoss = (CBlueEnemy*)m_pOBJManager->ReturnEnemyToMasterList( eBLUE_ENEMY );
			pTheBoss->SetActive(true);
			D3DXVECTOR3 tempVec = m_pThePlayer->GetPosition();
			tempVec.z += 100;
			pTheBoss->SetPosition( tempVec );


			pTheBoss->SetHealth(s_AIHelper->GetHealth());
			pTheBoss->SetShields(s_AIHelper->GetblueShield());
			pTheBoss->SetVelocityModifier(s_AIHelper->GetblueVelocityModifier());

			pTheBoss->GetShield()->SetActive(true);
			pTheBoss->SetIsStunned(false, 0.0f);

			if (pTheBoss->GetMovementBehavior())
			{
				pTheBoss->GetMovementBehavior()->SetForceChasebool(false);
			}
			for (unsigned int i = 0; i < pTheBoss->GetMovementBehaviorStack().size(); i++)
			{
				pTheBoss->PopBehavior();
			}*/
			m_vpCurrentEnemies.push_back(pTheBoss);


			return true;
		}
	}

	m_pCurrWaypoint->SetTrigger(false);

	return false;
}

void CScriptManager::BossDestroyAsteroids(float _elapsedTime)
{
	static float KillOne = 0.0f;
	KillOne += _elapsedTime;
	if (KillOne >= 1.05f)
	{

		for (unsigned int  eachAsteroid = 0; eachAsteroid < m_pAsteroids.size(); ++eachAsteroid)
		{
			if( m_pAsteroids[eachAsteroid]->GetActive() &&  m_pAsteroids[eachAsteroid]->GetWaveIndex() == lastWave)
			{

				pTheBoss->SetAsteroidTarget( m_pAsteroids[eachAsteroid]);
				break;
			}
		}
		KillOne = 0.0f;
	}
}

void CScriptManager::CreateAsteroids()
{
	for(unsigned int uiWave = 0 ; uiWave < m_vScript.size(); uiWave++)
	{
		for (unsigned int  eachAsteroid = 0; eachAsteroid < m_vScript[uiWave].m_vAsteroids.size(); ++eachAsteroid)
		{
			CAsteroid * newAsteroid =  (CAsteroid *)m_pOBJManager->ReturnAsteroidToMasterList((m_vScript[uiWave].m_vAsteroids[eachAsteroid].radius));

			//newAsteroid->SetRadius(m_vScript[uiWave].m_vAsteroids[eachAsteroid].radius);
			newAsteroid->SetPosition((m_vScript[uiWave].m_d3dWaypointPos + m_vScript[uiWave].m_vAsteroids[eachAsteroid].m_d3dPosition) );
			newAsteroid->SetWaveIndex(uiWave);
			m_pAsteroids.push_back(newAsteroid); 			
			
			if( m_pCinematicCamera )
			{
				m_pCinematicCamera->AddAsteroid(newAsteroid);
			}
		}
		lastWave = uiWave;
		
	}
}

/*****************************************************************
* CreateEnemies():   Creates Enemies determined by the current Script
* Ins:			   None    		      
* Outs:			   None
* Returns:		   bool
* Mod. Date:		
* Mod. Initials:	
*****************************************************************/
void CScriptManager::CreateEnemies()
{
	m_bSpawnedEnemyWave = true;
//	m_vpCurrentYellowEnemies.clear();
	for(unsigned int uiWave = 0 ; uiWave < m_CurrWaveIter->m_vEnemiesToCreate.size(); uiWave++)
	{
		EnemiesToCreate tEnemy = m_CurrWaveIter->m_vEnemiesToCreate[uiWave];
		
		if (tEnemy.m_cType == eRED_ENEMY)
		{
			CRedEnemy * pNewEnemy = (CRedEnemy*)m_pOBJManager->ReturnEnemyToMasterList( eRED_ENEMY );
			if (pNewEnemy == nullptr)
			{
				pNewEnemy = new CRedEnemy(m_pThePlayer, s_AIHelper, m_pCurrWaypoint, m_pOBJManager);
				pNewEnemy->SetPosition( tEnemy.m_d3dPosition );
				pNewEnemy->SetHealth(s_AIHelper->GetHealth());
				pNewEnemy->SetShields(s_AIHelper->GetredShield());
				pNewEnemy->SetFlockID(tEnemy.m_nFlockId);
				pNewEnemy->RemoveAllPartners();
				pNewEnemy->SetVelocityModifier(s_AIHelper->GetredVelocityModifier());
				for (unsigned int i = 0; i < pNewEnemy->GetMovementBehaviorStack().size(); i++)
				{
					 pNewEnemy->PopBehavior();
				}
				
				m_vpCurrentEnemies.push_back(pNewEnemy);
				CPhysics::LookAt(pNewEnemy, m_pThePlayer);

				pNewEnemy->CreateArrivalEffect();

				m_pOBJManager->AddObject( pNewEnemy);

			}
			else
			{
				pNewEnemy->SetPosition( tEnemy.m_d3dPosition );
				pNewEnemy->SetHealth(s_AIHelper->GetHealth());
				pNewEnemy->SetShields(s_AIHelper->GetredShield());
				pNewEnemy->SetFlockID(tEnemy.m_nFlockId);
				pNewEnemy->RemoveAllPartners();
				pNewEnemy->SetIsStunned(false, 0.0f);
				pNewEnemy->SetVelocityModifier(s_AIHelper->GetredVelocityModifier());
				pNewEnemy->SetJustSpawned(true);
				if (pNewEnemy->GetMovementBehavior())
				{
					pNewEnemy->GetMovementBehavior()->SetForceChasebool(false);
				}
				for (unsigned int i = 0; i < pNewEnemy->GetMovementBehaviorStack().size(); i++)
				{
					pNewEnemy->PopBehavior();
				}
				CPhysics::LookAt(pNewEnemy, m_pThePlayer);

				pNewEnemy->CreateArrivalEffect();

				m_vpCurrentEnemies.push_back(pNewEnemy);
			}
		}
		else if (tEnemy.m_cType == eBLUE_ENEMY)
		{
			CBlueEnemy * pNewEnemy = (CBlueEnemy*)m_pOBJManager->ReturnEnemyToMasterList( eBLUE_ENEMY );
			if (pNewEnemy == nullptr)
			{
				pNewEnemy = new CBlueEnemy(m_pThePlayer, s_AIHelper, m_pCurrWaypoint, m_pOBJManager);
				pNewEnemy->SetPosition( tEnemy.m_d3dPosition );
				m_vpCurrentEnemies.push_back(pNewEnemy);
				for (unsigned int i = 0; i < pNewEnemy->GetMovementBehaviorStack().size(); i++)
				{
					pNewEnemy->PopBehavior();
				}
				m_pOBJManager->AddObject(pNewEnemy);
				m_pOBJManager->AddObject( (CEntity*)pNewEnemy->GetShield()); //enemies shield
				CPhysics::LookAt(pNewEnemy, m_pThePlayer);

				pNewEnemy->CreateArrivalEffect();
			}
			else
			{
				pNewEnemy->SetPosition( tEnemy.m_d3dPosition );
				pNewEnemy->SetHealth(s_AIHelper->GetHealth());
				pNewEnemy->SetShields(s_AIHelper->GetblueShield());
				pNewEnemy->SetVelocityModifier(s_AIHelper->GetblueVelocityModifier());

				pNewEnemy->GetShield()->SetActive(true);
				pNewEnemy->SetIsStunned(false, 0.0f);
				pNewEnemy->SetJustSpawned(true);

				if (pNewEnemy->GetMovementBehavior())
				{
					pNewEnemy->GetMovementBehavior()->SetForceChasebool(false);
				}
				for (unsigned int i = 0; i < pNewEnemy->GetMovementBehaviorStack().size(); i++)
				{
					pNewEnemy->PopBehavior();
				}
				m_vpCurrentEnemies.push_back(pNewEnemy);
				CPhysics::LookAt(pNewEnemy, m_pThePlayer);

				pNewEnemy->CreateArrivalEffect();
			}
		}
		else if (tEnemy.m_cType == eYELLOW_ENEMY)
		{
			vector<CEntity*> pWaypts;
			CYellowEnemy * pNewEnemy = (CYellowEnemy *)m_pOBJManager->ReturnEnemyToMasterList( eYELLOW_ENEMY );
			
			if (pNewEnemy == nullptr)
			{
				int x = 0;
// 				pNewEnemy = new CYellowEnemy(m_pThePlayer, pWaypts);
// 				pNewEnemy->SetHealth(s_AIHelper->GetHealth());
// 				pNewEnemy->SetShields(s_AIHelper->GetyellowShield());
// #if _DEBUG
// 				pNewEnemy->SetMesh(m_pGame->GetRenderer()->GetAssetManager()->LoadMesh("YellowEnemy",38));
// #endif		
// 				pNewEnemy->SetPosition( tEnemy.m_d3dPosition );
// 				pNewEnemy->SetPlayer( m_pThePlayer );
// 				m_vpCurrentYellowEnemies.push_back(pNewEnemy);
// 
// 				m_pOBJManager->AddObject(pNewEnemy);

			}
			else
			{
				pNewEnemy->SetPosition( tEnemy.m_d3dPosition );
				pNewEnemy->SetHealth(s_AIHelper->GetyellowShield());
				pNewEnemy->SetVelocityModifier(s_AIHelper->GetyellowVelocityModifier());
				pNewEnemy->SetNeedAnArrow(true);
				pNewEnemy->SetIsStunned(false, 0.0f);
				pNewEnemy->SetJustSpawned(true);

				if (pNewEnemy->GetMovementBehavior())
				{
					pNewEnemy->GetMovementBehavior()->SetForceChasebool(false);
				}
				for (unsigned int i = 0; i < pNewEnemy->GetMovementBehaviorStack().size(); i++)
				{
					pNewEnemy->PopBehavior();
				}
				m_vpCurrentEnemies.push_back(pNewEnemy);
				CPhysics::LookAt(pNewEnemy, m_pThePlayer);

				pNewEnemy->CreateArrivalEffect();

			//	vector<CEntity*> vpPath = pNewEnemy->GetPath();
// 				unsigned int uiPathSize = pNewEnemy->GetPath().size();
// 
// 				for (unsigned int uiPaths = 0; uiPaths < uiPathSize; uiPaths++)
// 				{
// 					if (vpPath[uiPaths])
// 					{
// 						delete vpPath[uiPaths];
// 						vpPath[uiPaths] = 0;
// 					}
// 
// 				}
// 				for (unsigned int uiWay = 0; uiWay < pWaypts.size(); uiWay++)
// 				{
// 
// 					pNewEnemy->AddWaypoint(pWaypts[uiWay]);
// 				}
// 				pNewEnemy->SetTarget( pNewEnemy->GetPath()[0]);
// 				pNewEnemy->SetPosition( tEnemy.m_d3dPosition );
// 				pNewEnemy->SetIsStunned(false, 0.0f);
// 				pNewEnemy->SetPlayer( m_pThePlayer );
// 				m_vpCurrentYellowEnemies.push_back(pNewEnemy);


			}
		}
	}


	//Get the Red Enemies partners and set them to himself
	for(unsigned int uiPartners = 0; uiPartners < m_vpCurrentEnemies.size(); uiPartners++)
	{
		if (m_vpCurrentEnemies[uiPartners]->GetType() == eRED_ENEMY)
		{
			for(unsigned int uiEnemys = 0; uiEnemys < m_vpCurrentEnemies.size(); uiEnemys++)
			{
				if( m_vpCurrentEnemies[uiEnemys]->GetType() == eRED_ENEMY &&
					!((CRedEnemy*)m_vpCurrentEnemies[uiEnemys])->isPartner((CRedEnemy*)m_vpCurrentEnemies[uiPartners])
					&& m_vpCurrentEnemies[uiPartners] != m_vpCurrentEnemies[uiEnemys] && ((CRedEnemy*)m_vpCurrentEnemies[uiEnemys])->GetFlockID() == ((CRedEnemy*)m_vpCurrentEnemies[uiPartners])->GetFlockID())
				{
					((CRedEnemy*)m_vpCurrentEnemies[uiEnemys])->AddPartner(m_vpCurrentEnemies[uiPartners] );
				}
			}
		}
	}

}

/*****************************************************************
* LoadBinaryScript():   Loads a Binary File to Set the ScriptManager. 
*						Returns true if loaded correctly
* Ins:			   LPCTSTR    		      
* Outs:			   None
* Returns:		   bool
* Mod. Date:		
* Mod. Initials:	
*****************************************************************/
bool CScriptManager::LoadBinaryScript(LPCTSTR szFilePath)
{

	string szTempFilePath = "Resources/Scripts/";
	szTempFilePath += szFilePath;

	fstream fsScriptIn(szTempFilePath, std::ios_base::in | std::ios_base::binary);

	if(!fsScriptIn)
	{
#if _DEBUG
		cout << "Script did not load properly!" << '\n';
#endif

		return false;
	}

	int nNumWaypoints = 0;

	fsScriptIn.read((char*)&nNumWaypoints, sizeof(nNumWaypoints));

	for(int nWaypoints = 0; nWaypoints < nNumWaypoints; ++nWaypoints)	
	{
		Events tEvents;
		int nNumWaves = 0;

		fsScriptIn.read((char*)&tEvents.m_d3dWaypointPos, sizeof(tEvents.m_d3dWaypointPos));
		fsScriptIn.read((char*)&nNumWaves, sizeof(nNumWaves));

		for(int nWave = 0; nWave < nNumWaves; ++nWave)
		{
			Wave tTempWave;
			int nNumEnemies = 0;

			fsScriptIn.read((char*)&tTempWave.m_cNextSpawn, sizeof(tTempWave.m_cNextSpawn));
			fsScriptIn.read((char*)&nNumEnemies, sizeof(nNumEnemies));

			for(int nEnemies = 0; nEnemies < nNumEnemies; ++nEnemies)
			{
				EnemiesToCreate tEnemyInfo;
				int nNumPathPoints = 0;

				fsScriptIn.read((char*)&tEnemyInfo.m_cType, sizeof(tEnemyInfo.m_cType));
				fsScriptIn.read((char*)&tEnemyInfo.m_d3dPosition, sizeof(tEnemyInfo.m_d3dPosition) );
				fsScriptIn.read((char*)nNumPathPoints, sizeof(nNumPathPoints));

				for(int nPathpoint = 0; nPathpoint < nNumPathPoints; ++nPathpoint)
				{
					D3DXVECTOR3 d3dPaths;
					fsScriptIn.read((char*)&d3dPaths, sizeof(d3dPaths));

					tEnemyInfo.m_vPaths.push_back(d3dPaths);
				}

				tTempWave.m_vEnemiesToCreate.push_back(tEnemyInfo);
			}

			tEvents.m_vWave.push_back(tTempWave);
		}

		m_vScript.push_back(tEvents);
	}

	m_pCurrWaypoint->SetPosition(m_vScript[m_unCurrentEventIndex].m_d3dWaypointPos);


	return true;
}

/*****************************************************************
* LoadTextScript():   Loads a Text File to Set the ScriptManager. 
*					  Returns true if loaded correctly
* Ins:			   LPCTSTR    		      
* Outs:			   None
* Returns:		   bool
* Mod. Date:		
* Mod. Initials:	
*****************************************************************/
bool CScriptManager::LoadTextScript(LPCTSTR szFilePath)
{

	string szTempFilePath = "Resources/Scripts/";
	szTempFilePath += szFilePath;

	fstream fsScriptIn(szTempFilePath, std::ios_base::in);

	if(!fsScriptIn)
	{
#if _DEBUG
		cout << "Script did not load properly!" << '\n';
#endif

		return false;
	}

	int nNumWayPoints = 0;

	fsScriptIn >> nNumWayPoints;

	for(int nWayPoints = 0; nWayPoints < nNumWayPoints; ++nWayPoints)
	{
		Events tEvents;
		int nNumWaves = 0;

		fsScriptIn >> tEvents.m_d3dWaypointPos.x;
		fsScriptIn >> tEvents.m_d3dWaypointPos.y;
		fsScriptIn >> tEvents.m_d3dWaypointPos.z;
		int nNumAsteroids = 0;
		fsScriptIn >> nNumAsteroids;
		
		for(int nAsteroid = 0; nAsteroid < nNumAsteroids; ++nAsteroid)
		{
			Asteroid tmpAsteroid;
			fsScriptIn >> tmpAsteroid.m_d3dPosition.x;
			fsScriptIn >> tmpAsteroid.m_d3dPosition.y;
			fsScriptIn >> tmpAsteroid.m_d3dPosition.z;
			fsScriptIn >> tmpAsteroid.radius;
			tEvents.m_vAsteroids.push_back(tmpAsteroid);
		}

		fsScriptIn >> nNumWaves;

		for( int nEventIndex = 0; nEventIndex < nNumWaves; ++nEventIndex)
		{
			Wave tTempWaves;
			int nNumEnemies = 0;

			fsScriptIn >> tTempWaves.m_cNextSpawn;
			fsScriptIn >> nNumEnemies;

			for(int nEnemyIndex= 0; nEnemyIndex < nNumEnemies; ++nEnemyIndex)
			{
				EnemiesToCreate tEnemyInfo;
				int nNumPaths = 0;

				fsScriptIn >> tEnemyInfo.m_cType;
				if (tEnemyInfo.m_cType == 0)
				{
					fsScriptIn >> tEnemyInfo.m_nFlockId;
				}
				fsScriptIn >> tEnemyInfo.m_d3dPosition.x;
				fsScriptIn >> tEnemyInfo.m_d3dPosition.y;
				fsScriptIn >> tEnemyInfo.m_d3dPosition.z;


				tEnemyInfo.m_d3dPosition.x += tEvents.m_d3dWaypointPos.x;
				tEnemyInfo.m_d3dPosition.y += tEvents.m_d3dWaypointPos.y;
				tEnemyInfo.m_d3dPosition.z += tEvents.m_d3dWaypointPos.z;


				fsScriptIn >> nNumPaths;

				for( int nPathIndex = 0; nPathIndex < nNumPaths; ++nPathIndex)
				{
					D3DXVECTOR3 d3dPaths;
					fsScriptIn >> d3dPaths.x;
					fsScriptIn >> d3dPaths.y;
					fsScriptIn >> d3dPaths.z;

					d3dPaths.x += tEnemyInfo.m_d3dPosition.x;
					d3dPaths.y += tEnemyInfo.m_d3dPosition.y;
					d3dPaths.z += tEnemyInfo.m_d3dPosition.z;

					tEnemyInfo.m_vPaths.push_back(d3dPaths);
				}

				tTempWaves.m_vEnemiesToCreate.push_back(tEnemyInfo);
			}

			tEvents.m_vWave.push_back(tTempWaves);
		}

		m_vScript.push_back(tEvents);
	}

	m_pCurrWaypoint->SetPosition(m_vScript[m_unCurrentEventIndex].m_d3dWaypointPos);

	return true;
}


bool CScriptManager::LoadTextScriptOld(LPCTSTR szFilePath)
{

	string szTempFilePath = "Resources/Scripts/";
	szTempFilePath += szFilePath;

	fstream fsScriptIn(szTempFilePath, std::ios_base::in);

	if(!fsScriptIn)
	{
#if _DEBUG
		cout << "Script did not load properly!" << '\n';
#endif

		return false;
	}

	int nNumWayPoints = 0;

	fsScriptIn >> nNumWayPoints;

	for(int nWayPoints = 0; nWayPoints < nNumWayPoints; ++nWayPoints)
	{
		Events tEvents;
		int nNumWaves = 0;

		fsScriptIn >> tEvents.m_d3dWaypointPos.x;
		fsScriptIn >> tEvents.m_d3dWaypointPos.y;
		fsScriptIn >> tEvents.m_d3dWaypointPos.z;

		fsScriptIn >> nNumWaves;

		for( int nEventIndex = 0; nEventIndex < nNumWaves; ++nEventIndex)
		{
			Wave tTempWaves;
			int nNumEnemies = 0;

			fsScriptIn >> tTempWaves.m_cNextSpawn;
			fsScriptIn >> nNumEnemies;

			for(int nEnemyIndex= 0; nEnemyIndex < nNumEnemies; ++nEnemyIndex)
			{
				EnemiesToCreate tEnemyInfo;
				int nNumPaths = 0;

				fsScriptIn >> tEnemyInfo.m_cType;
				if (tEnemyInfo.m_cType == 0)
				{
					fsScriptIn >> tEnemyInfo.m_nFlockId;
				}
				fsScriptIn >> tEnemyInfo.m_d3dPosition.x;
				fsScriptIn >> tEnemyInfo.m_d3dPosition.y;
				fsScriptIn >> tEnemyInfo.m_d3dPosition.z;


				tEnemyInfo.m_d3dPosition.x += tEvents.m_d3dWaypointPos.x;
				tEnemyInfo.m_d3dPosition.y += tEvents.m_d3dWaypointPos.y;
				tEnemyInfo.m_d3dPosition.z += tEvents.m_d3dWaypointPos.z;


				fsScriptIn >> nNumPaths;

				for( int nPathIndex = 0; nPathIndex < nNumPaths; ++nPathIndex)
				{
					D3DXVECTOR3 d3dPaths;
					fsScriptIn >> d3dPaths.x;
					fsScriptIn >> d3dPaths.y;
					fsScriptIn >> d3dPaths.z;

					d3dPaths.x += tEnemyInfo.m_d3dPosition.x;
					d3dPaths.y += tEnemyInfo.m_d3dPosition.y;
					d3dPaths.z += tEnemyInfo.m_d3dPosition.z;

					tEnemyInfo.m_vPaths.push_back(d3dPaths);
				}

				tTempWaves.m_vEnemiesToCreate.push_back(tEnemyInfo);
			}

			tEvents.m_vWave.push_back(tTempWaves);
		}

		m_vScript.push_back(tEvents);
	}

	m_pCurrWaypoint->SetPosition(m_vScript[m_unCurrentEventIndex].m_d3dWaypointPos);

	return true;
}

/*****************************************************************
* SaveInBinary():   Saves the ScriptManager's loaded Waypoint information. 
*					Returns true if saved correctly in Binary
* Ins:			   LPCTSTR    		      
* Outs:			   None
* Returns:		   bool
* Mod. Date:		
* Mod. Initials:	
*****************************************************************/
bool CScriptManager::SaveInBinary(LPCTSTR szFilePath)
{
	string szTempFilePath = "Resources/Scripts/";
	szTempFilePath += szFilePath;

	fstream fsScriptOut(szTempFilePath, std::ios_base::out | ios_base::trunc | ios_base::binary);

	if(!fsScriptOut)
	{
#if _DEBUG
		cout << "Script did not save properly!" << '\n';
#endif

		return false;
	}
	return true;
}

/*****************************************************************
* SaveInText():   Saves the ScriptManager's loaded Waypoint information. 
*				  Returns true if saved correctly in Text
* Ins:			   LPCTSTR    		      
* Outs:			   None
* Returns:		   bool
* Mod. Date:		
* Mod. Initials:	
*****************************************************************/
bool CScriptManager::SaveInText(LPCTSTR szFilePath)
{
	int nNumWaypoints = 3;

	string szTempFilePath = "Resources/Scripts/";
	szTempFilePath += szFilePath;

	fstream fsScriptOut(szTempFilePath, ios_base::trunc | ios_base::out );

	if (!fsScriptOut.is_open())
	{
		return false;
	}

	fsScriptOut << nNumWaypoints << '\n';

	for(int nWaypoints = 0; nWaypoints < nNumWaypoints; ++nWaypoints)	
	{
		fsScriptOut <<  0  << " ";
		fsScriptOut <<  0  << " ";
		fsScriptOut <<  150 * (nWaypoints + 1) << '\n';
		
		int nNumWaves = 1;
		fsScriptOut << nNumWaves << '\n';
		for(int nWave = 0; nWave < nNumWaves; ++nWave)
		{
			
			int nNumEnemies = 3;

			//Next Wave Spawner Id
			fsScriptOut << -1 << '\n';

			fsScriptOut << nNumEnemies<< '\n';

			for(int nEnemies = 0; nEnemies < nNumEnemies; ++nEnemies)
			{

				int nNumPathPoints = 0;
				int nEnemytype = nWaypoints;
				if (nEnemytype == 2)
				{
					nNumPathPoints = rand() % 4 + 1;
				}

				fsScriptOut << nEnemytype << '\n';
				fsScriptOut <<  -12 * nEnemies << " ";
				fsScriptOut <<  -12 * nEnemies << " ";
				fsScriptOut <<  -12 * nEnemies << '\n';
				fsScriptOut << nNumPathPoints<< '\n';

				for(int nPathpoint = 0; nPathpoint < nNumPathPoints; ++nPathpoint)
				{
					fsScriptOut <<  -2 * nPathpoint << " ";
					fsScriptOut <<  -2 * nPathpoint << " ";
					fsScriptOut <<  -2 * nPathpoint << '\n';
				}
			}
		}
		fsScriptOut << endl;
	}
	fsScriptOut.clear();
	fsScriptOut.close();

	return true;
}

/*****************************************************************
* LoadCheckpoint():   Sets the next Checkpoint to the correct one. 
* Ins:			   None 		      
* Outs:			   None
* Returns:		   None
* Mod. Date:		
* Mod. Initials:	
*****************************************************************/
void CScriptManager::LoadCheckpoint( )
{
	unsigned int nCurrCheckpoint = m_pGame->GetCheckPoint()->m_unCurrEvent ;

	if(nCurrCheckpoint >=  m_vScript.size() || nCurrCheckpoint < 0)
	{
		return;
	}
	doOnce = true;
	m_unCurrentEventIndex = nCurrCheckpoint;

	m_bInitialEvent = true;
	m_pCurrWaypoint->SetPosition( m_vScript[m_unCurrentEventIndex].m_d3dWaypointPos );

	m_pGame->GetCamera()->ResetCameraToCheckPoint();
}

void CScriptManager::Tutorial( float _fDeltaTime)
{
	
	if(m_bCompletedObjective)
	{
		m_cCurrentTutorial++;
		m_bCompletedObjective = false;
		m_GoodJobTimer = 0.0f;
	}
	
	if(m_GoodJobTimer < 1.0f)
	{
		m_GoodJobTimer+= _fDeltaTime;
		return;
	}

	m_fTimer += _fDeltaTime;
	if(((CPlayer*)m_pThePlayer)->GetHealth())
	switch(m_cCurrentTutorial)
	{
	case 0:
		{
			if(m_fTimer < 1.0f)
			{
				return;
			}
			if(!m_bHasPlayedMoveMouse)
			{
				WwiseNS::StopAllTutorialVoices(eAIMoveMouse);
				WwiseNS::SoundEvent(eAIMoveMouse, ePlay);
				m_bHasPlayedMoveMouse = true;
			}
			m_pDialog->ChangeSection(eDialog_Controls);
			if(m_fTimer > 5.0f)
			{
				WwiseNS::StopAllTutorialVoices(-1);
				m_bCompletedObjective = true;
				m_fTimer = 0.0f;
			}
		}
		break;
	case 1:
		{
			if(m_fTimer > 1.0f)
			{
				if((CInput::ButtonHeld(eUp) || CInput::ButtonHeld(eDown)))
				{
					m_pDialog->ChangeSection(eTutorial_Great_Job);
					WwiseNS::SoundEvent(eCheckpoint, ePlay);
					//WwiseNS::SoundEvent(eGoodJob, ePlay);
					if(!m_bHasPlayedGreatJob)
					{
						WwiseNS::StopAllTutorialVoices(eAIGreatJob);
						WwiseNS::SoundEvent(eAIGreatJob, ePlay);
						m_bHasPlayedGreatJob = true;
					}
					m_bCompletedObjective = true;
					m_fTimer = 0.0f;
				}
			}
			else
			{
				if(!m_bHasPlayedSpeedUpSlowDown)
				{
					WwiseNS::StopAllTutorialVoices(eAISpeedUpSlowDown);
					WwiseNS::SoundEvent(eAISpeedUpSlowDown, ePlay);
					m_bHasPlayedSpeedUpSlowDown = true;
				}
				m_pDialog->ChangeSection(eDialog_Speed);
			}
		}
		break;
	case 2:
		{
			if(m_fTimer > 1.0f)
			{
				if(CInput::ButtonHeld(eLeftMouseClick))
				{
					WwiseNS::StopAllTutorialVoices(eCheckpoint);
					WwiseNS::SoundEvent(eCheckpoint, ePlay);
					//WwiseNS::SoundEvent(eGoodJob, ePlay);
					m_bCompletedObjective = true;
					m_fTimer = 0.0f;
				}
			}
			else
			{
				if(!m_bHasPlayedFireWeapon)
				{
					WwiseNS::StopAllTutorialVoices(eAIFireWeapon);
					WwiseNS::SoundEvent(eAIFireWeapon, ePlay);
					m_bHasPlayedFireWeapon = true;
				}
				m_pDialog->ChangeSection(eTutorial_Laser);
			}				
		}
		break;
	case 3:
		{
			if(m_fTimer > 100.0f)
			{
				if(m_fTimer > 102.5f)
				{
					m_bCompletedObjective = true;
					m_fTimer = 0.0f;
				}
			}
			else if(m_fTimer > 1.0f)
			{
				if((CInput::ButtonHeld(eLeft) || CInput::ButtonHeld(eRight)))
				{
					if(!m_bHasPlayedDodgeRollCooldown)
					{
						WwiseNS::StopAllTutorialVoices(eAIDodgeRollCooldown);
						WwiseNS::SoundEvent(eAIDodgeRollCooldown, ePlay);
						m_bHasPlayedDodgeRollCooldown = true;
					}
					m_pDialog->ChangeSection(eDialog_Start);
					WwiseNS::SoundEvent(eCheckpoint, ePlay);
					//WwiseNS::SoundEvent(eGoodJob, ePlay);
					m_fTimer = 100.0f;
				}
			}
			else
			{
				if(!m_bHasPlayedDodgeRoll)
				{
					WwiseNS::StopAllTutorialVoices(eAIDodgeRoll);
					WwiseNS::SoundEvent(eAIDodgeRoll, ePlay);
					m_bHasPlayedDodgeRoll = true;
				}
				m_pDialog->ChangeSection(eDialog_Dodge);
			}
		}
		break;
	case 4:
		{
			if(m_fTimer > 100.0f)
			{ 
				if(m_fTimer > 108.5f)
				{
					m_bCompletedObjective = true;
					m_fTimer = 0.0f;
				}
			}
			else if(m_fTimer > 1.0f)
			{
				if(CInput::ButtonHeld(eRightMouseClick))
				{
					if(!m_bHasPlayedModeWeapon)
					{
						WwiseNS::StopAllTutorialVoices(eAIModeWeapon);
						WwiseNS::SoundEvent(eAIModeWeapon, ePlay);
						m_bHasPlayedModeWeapon = true;
					}
					m_pDialog->ChangeSection(eTutorial_Blue_Fire);
					WwiseNS::SoundEvent(eCheckpoint, ePlay);
					//WwiseNS::SoundEvent(eGoodJob, ePlay);
					m_fTimer = 100.0f;
				}
			}
			else
			{
				if(!m_bHasPlayedSwitchModes)
				{
					WwiseNS::StopAllTutorialVoices(eAISwitchModes);
					WwiseNS::SoundEvent(eAISwitchModes, ePlay);
					m_bHasPlayedSwitchModes = true;
				}
				m_pDialog->ChangeSection(eTutorial_SwitchState);
			}
		}
		break;
	//case 5:
	//	{
	//		if(m_fTimer > 100.0f)
	//		{
	//			if(m_fTimer > 104.0f)
	//			{
	//				m_bCompletedObjective = true;
	//				m_fTimer = 0.0f;
	//			}
	//		}
	//		else if(m_fTimer > 1.00f)
	//		{
	//			if(CInput::DIKButtonCheck(DIK_Q))
	//			{
	//				WwiseNS::StopAllTutorialVoices(eAIQuickTurnReposition);
	//				WwiseNS::SoundEvent(eAIQuickTurnReposition, ePlay);
	//				m_pDialog->ChangeSection(eDialog_TurnAround);
	//				WwiseNS::SoundEvent(eCheckpoint, ePlay);
	//				//WwiseNS::SoundEvent(eGoodJob, ePlay);
	//				m_fTimer = 100.0f;
	//			}
	//		}
	//		else
	//		{
	//			if(!m_bHasPlayedQuickTurn)
	//			{
	//				WwiseNS::StopAllTutorialVoices(eAIQuickTurn);
	//				WwiseNS::SoundEvent(eAIQuickTurn, ePlay);
	//				m_bHasPlayedQuickTurn = true;
	//			}
	//			m_pDialog->ChangeSection(eDialog_PressQ);
	//		}
	//	}
	//	break;
	case 5:
		{
			//Introduce and Kill a yellow enemy here to move on.
			if(m_fTimer <= _fDeltaTime)
			{
				if(!m_bHasPlayedLookAnEnemy)
				{
					WwiseNS::StopAllTutorialVoices(eAILookAnEnemy);
					WwiseNS::SoundEvent(eAILookAnEnemy, ePlay);
					m_bHasPlayedLookAnEnemy = true;
				}
				m_pDialog->ChangeSection(eDialog_FirstBlood);
				m_pTutorialEnemy = (CYellowEnemy*)m_pOBJManager->ReturnEnemyToMasterList( eYELLOW_ENEMY );
				CCamera* m_pCamera = m_pGame->GetCamera();
				CPlayer* m_pPlayer = ((CPlayer*)m_pThePlayer);

				if (m_pTutorialEnemy != nullptr)
				{
					m_pTutorialEnemy->SetTarget(m_pPlayer);
					D3DXVECTOR3 tempPos = m_pPlayer->GetPosition();

					D3DXMATRIX d3dNewPlayerPos(*m_pCamera->GetViewMatrix());

					D3DXVECTOR3 d3dLocalZ(m_pCamera->GetViewMatrix()->_31, m_pCamera->GetViewMatrix()->_32,m_pCamera->GetViewMatrix()->_33);
					D3DXVec3Normalize(&d3dLocalZ, &d3dLocalZ);

					D3DXVECTOR3 _d3dLocalY(m_pCamera->GetViewMatrix()->_21, m_pCamera->GetViewMatrix()->_22, m_pCamera->GetViewMatrix()->_23);
					D3DXVec3Normalize(&_d3dLocalY, &_d3dLocalY);

					D3DXVECTOR3 d3dLocalX(m_pCamera->GetViewMatrix()->_11, m_pCamera->GetViewMatrix()->_12, m_pCamera->GetViewMatrix()->_13);
					D3DXVec3Normalize(&d3dLocalX, &d3dLocalX);

					tempPos.x += d3dLocalZ.x * m_pPlayer->GetVelocityModifier()*10;
					tempPos.y += d3dLocalZ.y * m_pPlayer->GetVelocityModifier()*10;
					tempPos.z += d3dLocalZ.z * m_pPlayer->GetVelocityModifier()*10;

					m_pTutorialEnemy->SetPosition( tempPos );
					m_pTutorialEnemy->SetHealth(s_AIHelper->GetyellowShield());
					m_pTutorialEnemy->SetIsStunned(false, 0.0f);

					if (m_pTutorialEnemy->GetMovementBehavior())
					{
						m_pTutorialEnemy->GetMovementBehavior()->SetForceChasebool(false);
					}

					for (unsigned int i = 0; i < m_pTutorialEnemy->GetMovementBehaviorStack().size(); i++)
					{
						m_pTutorialEnemy->PopBehavior();
					}
					CPhysics::LookAt(m_pTutorialEnemy, m_pPlayer);
					m_pTutorialEnemy->CreateArrivalEffect();
					m_vpCurrentEnemies.push_back(m_pTutorialEnemy);
					m_bSpawnBool = true;
					m_bSpawnedEnemyWave = true;
					
				}
			}
			else if((m_pTutorialEnemy != nullptr && !m_pTutorialEnemy->GetActive()) /*|| m_fTimer > 60.0f*/ || m_pTutorialEnemy == nullptr)
			{
				m_pDialog->ChangeSection(eTutorial_Great_Job);
				WwiseNS::SoundEvent(eGoodJob, ePlay);
				m_bCompletedObjective = true;
				m_vpCurrentEnemies.clear();
				m_fTimer = 0.0f;
			}
		}
		break;
	case 6:
		{
			if(m_fTimer > 5.0f)
			{
				m_bCompletedObjective = true;
				m_fTimer = 0.0f;
				m_bTutorial = false;
			}
			else if(m_fTimer <= _fDeltaTime)
			{
				if(m_pCinematicCamera->GetIsIntroCinematic() == false)
				{
					m_pCurrWaypoint->SetIsVisable(true);
					m_pCurrWaypoint->SetShouldScale(true);
					m_pCurrWaypoint->SetScaleDown(false);
					WwiseNS::SoundEvent(eProceedToWaypoint, ePlay);
					m_pDialog->ChangeSection(eDialog_Waypoint);
					m_pOBJManager->AddObject(m_pCurrWaypoint);
					m_bAddedWaypoint = true;
				}
			}
		}
		break;
	};
}
