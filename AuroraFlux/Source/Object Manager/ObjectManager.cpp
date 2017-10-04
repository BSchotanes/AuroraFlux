/***********************************************
* Filename:  		ObjectManager.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	
* Author:    		Stephen warren
* Purpose:   		This class will handle all objects, calling their update,
*					render, collision, and the passing of objects back and forth
*					to the master list.
************************************************/

#include "../StdAfx.h"
#include "ObjectManager.h"
#include "../Renderer/CRenderer.h"
#include "../Definitions.h"
#include "../Entity/Missile.h"
#include "../Entity/Lasers.h"
#include "../Entity/BlueEnemy.h"
#include "../Collision/CollisionLib.h"
#include "../Entity/RedEnemy.h"
#include "../Entity/BlueEnemy.h"
#include "../Entity/YellowEnemy.h"
#include "../Entity/BossHanger.h"
#include "../AI/AIHelper.h"
#include "../../Source/Entity/Asteroid.h"
#include "../AI/Ambient Fighters/AmbientBoundingBox.h"

/*****************************************************************
* CObjectManager(): Overload Constructor will take in a pointer to the reference. 
* Ins:			    _pRenderer    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		
* Mod. Initials:	
*****************************************************************/
CObjectManager::CObjectManager(CRenderer* _pRenderer, CCamera* _pCamera, CAIHelper *_AIHelper) : m_bIterating(false), m_pRenderer(_pRenderer), m_pCamera(_pCamera), s_AIHelper(_AIHelper), m_bBossFight(false),
	m_nNumYellowEnemies(0), m_pBoss(nullptr), m_pIterPlayer(nullptr)
{
	m_pPreviousTarget = nullptr;
	m_fTargetFleeCounter = 0.0f;
	m_bShowDogFights = false;
	m_vUnusedNodes.reserve(NUM_NODES);
	m_bRemoveMiscItems = false;
	for(int i=0; i<NUM_NODES; i++)
	{
		m_vUnusedNodes.push_back(new tNode);
	}

	for (int i=0; i<NUM_BUCKETS; i++)
	{
		m_vHashTable[i] = 0;
	}

	m_bInCinematic = false;

	m_bPlayerLose = false;
}

/*****************************************************************
* ~CObjectManager():     Destructor to take care of all the . 
* Ins:					 None    		      
* Outs:				 	 None
* Returns:				 None
* Mod. Date:		
* Mod. Initials:	
*****************************************************************/
CObjectManager::~CObjectManager(void)
{

}
/*****************************************************************
* UpdateObjects():     Updates all the objects in the master list 
* Ins:			       None    		      
* Outs:			 	   None
* Returns:		       None
* Mod. Date:		
* Mod. Initials:	
*****************************************************************/
void CObjectManager::UpdateObjects( float _fDeltaTime, float _fOtherDelta /*= -123 */ )
{
	//If we are already going through the list in 
	//another function we dont want to overwrite data
	if( m_bIterating )
	{
		return;
	}

	if(m_bPlayerLose == false)
	{
		DismantleHashTable();
	}

	m_vActiveMissileList.clear();
	//If not we are now
	m_bIterating = true;

	//Create this to go through the list
	std::vector<CEntity *>::iterator iter;
	CAssetManager* ass = m_pRenderer->GetAssetManager();

	for(iter = m_vPendingToAddtoMasterList.begin(); iter != m_vPendingToAddtoMasterList.end(); ++iter)
	{
		m_vMasterList.push_back(*iter);
	}

	if( m_bRemoveMiscItems )
	{
		m_bRemoveMiscItems = false;
		for( unsigned int i(0); i < m_vMasterList.size(); ++i)
		{
			CEntity* obj = m_vMasterList[i];
			switch(obj->GetType())
			{
			case eSPACE_DEBRIS:
			case eSPACE_DEBRIS2:
			case eSPACE_DEBRIS3:
			case eSPACESTATION1:
			case eSPACE_STATION:
			case eSATELITE:
			case eDIBRIS_OBJ:
				{
					m_vMiscList.push_back(obj);
					m_vMasterList.erase(m_vMasterList.begin()+i);
					i--;
				}
				break;
			case eASTEROID:
			case eLARGE_ASTEROID:
				{
					m_vMiscList.push_back(obj);
					m_vMasterList.erase(m_vMasterList.begin()+i);
					i--;
				}
				break;
			}
		}
	}

	m_vPendingToAddtoMasterList.clear();
	CEntity *pIterData = nullptr;
	int nIterDataType = 0;

	// FRUSTUM CULLING
	m_vRenderList.clear();
	m_vTransparentEnemyList.clear();
	float m_pCameraToPlayer = 0;
	if(m_pIterPlayer)
		m_pCameraToPlayer = D3DXVec3Length(&(m_pIterPlayer->GetPosition() - m_pCamera->GetCurrentPosition()));

	//Update every entity!
	for(iter = m_vMasterList.begin(); iter != m_vMasterList.end(); ++iter)
	{
		pIterData = (*iter);
		nIterDataType = pIterData->GetType();
		if(nIterDataType == eBOSS)
		{
			m_pBoss = pIterData;
		}

		if(nIterDataType == eRED_PLAYER || nIterDataType == eBLUE_PLAYER)
		{
			m_pIterPlayer = pIterData;
		}

		if(m_pIterPlayer != nullptr && (nIterDataType != eRED_PLAYER || nIterDataType != eBLUE_PLAYER))
		{
			float fDistanceFromPlayer = D3DXVec3Length(&(m_pIterPlayer->GetPosition() - pIterData->GetPosition()));

			pIterData->SetDistanceFromPlayer(fDistanceFromPlayer);
		}

		if(m_pIterPlayer != NULL && nIterDataType == eLARGE_ASTEROID)
		{
			CCollisionLib::CheckCollision(m_pIterPlayer, pIterData, ass);
			CCollisionLib::CheckCollision(pIterData, m_pIterPlayer, ass);
		}

		if (pIterData->GetDistanceFromPlayer() > m_pCameraToPlayer)
		{
			pIterData->SeIsTransparent(false);
		}
		else
		{
			pIterData->SeIsTransparent(true);
		}

		//pIterData = (*iter);
		if( m_pCamera->GetInWinScreen() )
		{
			m_pIterPlayer->Update(_fDeltaTime);
			if( pIterData->GetType() == eCAM_OBJ )
			{
				pIterData->Update(_fDeltaTime);
			}
		}
		else
		{
			pIterData->Update( _fDeltaTime );
		}
		if(pIterData->GetFlags() == eLOCKED_ON)
		{
			m_vActiveMissileList.push_back(pIterData);
		}
		else
		{
			SpawnEntity(pIterData, _fDeltaTime);
		}

		// FRUSTUM CULLING
		tFrustum* tempFrustum =  m_pCamera->GetFrustum();

		CSphere tempSphere;
		CCapsule tempCapsule;

		if( pIterData->GetBV() != nullptr)
		{
			if(m_bInCinematic == true)
			{
				m_vRenderList.push_back(pIterData);
				continue;
			}

			int nType = pIterData->GetType();

			if(nType ==  eBOSS || nType == eSPACE_STATION || nType == eSPACESTATION1 || nType ==  eSATELITE || nType == eRED_PLAYER || nType == eBLUE_PLAYER)
			{
				m_vRenderList.push_back(pIterData);
				continue;
			}

			if(pIterData->GetBV()->GetObjType() == eSPH)
			{
				if(nType == eLARGE_ASTEROID)
				{
					tempSphere.SetRadius(((CSphere*)(pIterData->GetBV()))->GetFrustumRadius());
				}
				else
				{
					tempSphere.SetRadius(((CSphere*)(pIterData->GetBV()))->GetRadius());
				}

				tempSphere.SetPosition(CPhysics::GetMat4Vec(ePOS,pIterData->GetMatrix()));

				if(CCollisionLib::FrustumToSphere(*tempFrustum, tempSphere))
				{

					if(nType ==  eWAYPOINT)
					{
						int x = 0;
					}
					if(nType != eCAM_OBJ )
					{
						if(pIterData->GetIsTransparent() == false)
						{
							m_vRenderList.push_back(pIterData);
						}
						else
						{
							m_vTransparentEnemyList.push_back(pIterData);
						}
					}

					if(nType == eBLUE_ENEMY)
					{
						m_vRenderList.push_back(pIterData->GetShield());
					}

				}
				continue;
			}
			else if(pIterData->GetBV()->GetObjType() == eCAP)
			{
				//TODO BERN : FIX CAPSULE CHECK
				CCapsule* currentCapsule = (CCapsule*)pIterData->GetBV();
				tempCapsule.SetRadius(currentCapsule->GetRadius());
				tempCapsule.SetStart(currentCapsule->GetStart() + pIterData->GetPosition());
				tempCapsule.SetEnd(currentCapsule->GetEnd() + pIterData->GetPosition());

				if(CCollisionLib::FrustumToCapsule(*tempFrustum, tempCapsule))
				{
					m_vRenderList.push_back(pIterData);
				}
				continue;
			}
		}
	}

	CEntity* pIndicatorListIter = nullptr;
	for(iter = m_vIndicatorList.begin(); iter != m_vIndicatorList.end();)
	{
		pIndicatorListIter = (*iter);
		if(!pIndicatorListIter->GetActive())
		{
			if(pIndicatorListIter->GetType() == eYELLOW_ENEMY)
			{
				m_nNumYellowEnemies -= 1;
			}

			iter = m_vIndicatorList.erase(iter);
		}
		else
		{
			++iter;
		}
	}


	//MakeRenderableList();


	QuickSort(m_vRenderList, 0, m_vRenderList.size()-1);

	//CEntity* pRenderListIter = nullptr;
	//std::vector<CEntity *>::iterator iterRenderable;
	//for(iterRenderable = m_vRenderList.begin(); iterRenderable != m_vRenderList.end();)
	//{
	//	pRenderListIter = (*iterRenderable);
	//	if( pRenderListIter == m_pPlayerShield)
	//	{
	//		iterRenderable = m_vRenderList.erase( iterRenderable );
	//		break;
	//	}
	//	else
	//	{
	//		++iterRenderable;
	//	}
	//}

	//m_vRenderList.push_back(m_pPlayerShield);

	//We are done give control back to the player
	m_bIterating = false;

	std::vector<CEffectSystem*>::iterator iterEffects;
	CEffectSystem* pAllEffects = nullptr;
	for(iterEffects = m_vAllEffects.begin(); iterEffects != m_vAllEffects.end(); )
	{
		pAllEffects = (*iterEffects);
		if( !pAllEffects->GetAlive() )
		{
			if(pAllEffects->GetType() == eASTEROID_TAIL)
			{
				CEffectSystem* tmp = pAllEffects;
				delete tmp;
			}
			iterEffects = m_vAllEffects.erase( iterEffects );
		}
		else
		{
			if(_fOtherDelta == -123)
			{
				pAllEffects->Update(_fDeltaTime);
			}
			else
			{
				pAllEffects->Update(_fOtherDelta);
			}
			++iterEffects;
		}
	}

	if(m_bShowDogFights&& !m_bBossFight)
	{
		for(unsigned int i(0); i < m_vAmbientDogFights.size(); ++i)
		{
			m_vAmbientDogFights[i]->Update(_fDeltaTime,m_bBossFight );
		}
	}

	if(m_bPlayerLose == false)
	{
		BuildHashTable();
	}
}

void CObjectManager::RemoveLasers()
{
	for( unsigned int i(0); i < m_vMasterList.size(); ++i )
	{
		if( m_vMasterList[i]->GetType() == eLASER )
		{
			m_vInactiveArmamentList.push_back(m_vMasterList[i]);
			m_vMasterList.erase(m_vMasterList.begin()+i);
			i--;
		}
	}
}

void CObjectManager::RegenerateMisc(D3DXVECTOR3 pos)
{
	for( unsigned int i(0); i < m_vMiscList.size(); ++i )
	{
		float dist = D3DXVec3Length(&(m_vMiscList[i]->GetPosition()-pos));
		if( dist < 800 )
		{
			continue;
		}
		AddObject(m_vMiscList[i]);
		m_vMiscList.erase(m_vMiscList.begin()+i);
		i--;
	}
}

void CObjectManager::UpdateMainMenuObjects( float _fDeltaTime )
{
	//If we are already going through the list in 
	//another function we dont want to overwrite data
	if( m_bIterating )
	{
		return;
	}


	m_vActiveMissileList.clear();
	//If not we are now
	m_bIterating = true;

	//Create this to go through the list
	std::vector<CEntity *>::iterator iter;
	CAssetManager* ass = m_pRenderer->GetAssetManager();

	for(iter = m_vPendingToAddtoMasterList.begin(); iter != m_vPendingToAddtoMasterList.end(); ++iter)
	{
		m_vMasterList.push_back(*iter);
	}

	m_vPendingToAddtoMasterList.clear();


	//Update every entity!
	for(iter = m_vMasterList.begin(); iter != m_vMasterList.end(); ++iter)
	{
		(*iter)->Update( _fDeltaTime );

	}


	//We are done give control back to the player
	m_bIterating = false;

	std::vector<CEffectSystem*>::iterator iterEffects;
	CEffectSystem* pAllEffects = nullptr;
	for(iterEffects = m_vAllEffects.begin(); iterEffects != m_vAllEffects.end(); )
	{
		pAllEffects = (*iterEffects);
		if( !pAllEffects->GetAlive() )
		{
			if(pAllEffects->GetType() == eASTEROID_TAIL)
			{
				CEffectSystem* tmp = pAllEffects;
				delete tmp;
			}
			iterEffects = m_vAllEffects.erase( iterEffects );
		}
		else
		{
			pAllEffects->Update(_fDeltaTime);
			++iterEffects;
		}
	}

}

/*****************************************************************
* AddObject():     Adds an object to the master list. Add only if needed. 
*				   Otherwise use the ReturnTo function. 
* Ins:			   CEntity*    		      
* Outs:			   None
* Returns:		   None
* Mod. Date:		
* Mod. Initials:	
*****************************************************************/
void CObjectManager::AddObject( CEntity* _pEntity )
{
	//Are we iterating through the list?
	//Do we even have an entity?
	if( _pEntity == nullptr)
	{
		return;
	}
	else if( m_bIterating )
	{
		m_vPendingToAddtoMasterList.push_back(_pEntity);
		return;
	}

#ifdef _DEBUG

	string meshName;
	meshName = _pEntity->GetType();

	if(_pEntity->GetBV() != NULL && _pEntity->GetBV()->GetObjType() == eSPH)
	{
		meshName += (char)((CSphere*)_pEntity->GetBV())->GetRadius();
		_pEntity->SetMesh(m_pRenderer->GetAssetManager()->LoadMeshSphere(meshName.c_str(),((CSphere*)_pEntity->GetBV())->GetRadius()));
	}
	else if(_pEntity->GetBV() != NULL && _pEntity->GetBV()->GetObjType() == eCAP)
	{
		meshName += (char)((CCapsule*)_pEntity->GetBV())->GetRadius();
		meshName += (char)((CCapsule*)_pEntity->GetBV())->GetLength();
		_pEntity->SetMesh(m_pRenderer->GetAssetManager()->LoadMeshCapsule(meshName.c_str(),((CCapsule*)_pEntity->GetBV())->GetRadius(),((CCapsule*)_pEntity->GetBV())->GetLength()));
	}
#endif

	//Put it in the list
	m_vMasterList.push_back(_pEntity);

	switch(_pEntity->GetType())
	{
	case eRED_ENEMY:
	case eBLUE_ENEMY:
	case eYELLOW_ENEMY:
	case eWAYPOINT:
		{

			m_vIndicatorList.push_back(_pEntity);				

		}
		break;
	case eGENERATOR:
		{
			m_vIndicatorList.push_back(_pEntity);				
		}
		break;
	default:
		{
		}
		break;
	}

}

/*****************************************************************
* RemoveObject():  Removes an object from the master list.
*				   Should only be called if the entity is no longer needed 
* Ins:			   CEntity*    		      
* Outs:			   None
* Returns:		   None
* Mod. Date:		
* Mod. Initials:	
*****************************************************************/
void CObjectManager::RemoveObject( CEntity* _pEntity )
{
	//Are we iterating through the list?
	//Do we even have an entity?
	//Getting redundant, isnt it?
	if( m_bIterating || _pEntity == nullptr )
	{
		return;
	}

	//We are about to iterate
	m_bIterating = true;

	//Create an iterator through the list
	std::vector< CEntity* >::iterator iterMaster;
	CEntity* pMasterListIter = nullptr;

	//now go through until we find what we are looking for
	for(iterMaster = m_vMasterList.begin(); iterMaster != m_vMasterList.end(); ++iterMaster )
	{
		pMasterListIter = *iterMaster;
		//We found it!
		if( pMasterListIter == _pEntity )
		{
			//TODO SEt up Waypoint info
			//Put it into the correct inactive list
			switch(_pEntity->GetType())
			{
			case eASTEROID:
				{
					AddToInactiveAsteroid(_pEntity);
				}
				break;
			case eRED_ENEMY:
			case eBLUE_ENEMY:
			case eYELLOW_ENEMY:
			case eBOSS:
				{
					if((*_pEntity).GetType() == eBLUE_ENEMY)
					{
						((CBlueEnemy*)(_pEntity))->GetDamagedEffect()->SetPlay(false);
					}

					AddToInactiveEnemy(_pEntity);
				}
				break;

			case eMISSILE:
			case eLASER:
				{
					AddToInactiveArmament(_pEntity);
				}
				break;
			default:
				{
					m_vCatchAllList.push_back(_pEntity);
				}
				break;
			}

			//Get rid of it and set the iterator to the previous entity
			m_vMasterList.erase( iterMaster );

			//That was the only one we cared about 
			break;
		}

	}

	//We are done
	m_bIterating = false;
}

/*****************************************************************
* RemoveAllObjects():     Removes all objects from the master list.
*						  Should only be called if the entities are no longer needed 
* Ins:			   None    		      
* Outs:			   None
* Returns:		   None
* Mod. Date:		
* Mod. Initials:	
*****************************************************************/
void CObjectManager::RemoveAllObjects()
{
	//Are we iterating?
	if( m_bIterating )
	{
		return;
	}

	//We are now
	m_bIterating = true;

	//Create the iterator
	std::vector< CEntity* >::iterator iterMaster;
	CEntity* pMasterListIter = nullptr;
	//Loop through deleting them all!
	for(iterMaster = m_vMasterList.begin(); iterMaster != m_vMasterList.end(); ++iterMaster )
	{
		//TODO
		//Put them in the correct inactive vector
		pMasterListIter = (*iterMaster);
		switch(pMasterListIter->GetType())
		{
		case eASTEROID:
			{
				AddToInactiveAsteroid(pMasterListIter);
			}
			break;
		case eRED_ENEMY:
		case eBLUE_ENEMY:
		case eYELLOW_ENEMY:
		case eBOSS:
			{
				AddToInactiveEnemy(pMasterListIter);
			}
			break;
		case eMISSILE:
		case eLASER:
			{
				AddToInactiveArmament(pMasterListIter);
			}
			break;
		default:
			{
				m_vCatchAllList.push_back(pMasterListIter);
			}
			break;
		}

	}

	//We are done wrecking havoc
	m_bIterating = false;

	//Clear the list of all the pointers
	m_vMasterList.clear();


	m_vIndicatorList.clear();
}

/*****************************************************************
* AddToInactiveAsteroid():     Adds an inactive asteroid to the asteroid list.
* Ins:			   CEntity*    		      
* Outs:			   None
* Returns:		   None
* Mod. Date:		
* Mod. Initials:	
*****************************************************************/
void CObjectManager::AddToInactiveAsteroid( CEntity* _pEntity )
{
	if(_pEntity == nullptr )
	{
		return;
	}

	_pEntity->SetActive(false);
#ifdef _DEBUG
	string meshName;
	meshName = _pEntity->GetType();

	if(_pEntity->GetBV() != NULL && _pEntity->GetBV()->GetObjType() == eSPH)
	{
		meshName += (char)((CSphere*)_pEntity->GetBV())->GetRadius();
		_pEntity->SetMesh(m_pRenderer->GetAssetManager()->LoadMeshSphere(meshName.c_str(),((CSphere*)_pEntity->GetBV())->GetRadius()));
	}
	else if(_pEntity->GetBV() != NULL && _pEntity->GetBV()->GetObjType() == eCAP)
	{
		meshName += (char)((CCapsule*)_pEntity->GetBV())->GetRadius();
		meshName += (char)((CCapsule*)_pEntity->GetBV())->GetLength();
		_pEntity->SetMesh(m_pRenderer->GetAssetManager()->LoadMeshCapsule(meshName.c_str(),((CCapsule*)_pEntity->GetBV())->GetRadius(),((CCapsule*)_pEntity->GetBV())->GetLength()));
	}
#endif
	m_vInactiveAsteroidList.push_back(_pEntity);
}

/*****************************************************************
* AddToInactiveEnemy():     Adds an inactive enemy to the enemy list.
* Ins:			   CEntity*    		      
* Outs:			   None
* Returns:		   None
* Mod. Date:		
* Mod. Initials:	
*****************************************************************/
void CObjectManager::AddToInactiveEnemy( CEntity* _pEntity )
{
	if(_pEntity == nullptr )
	{
		return;
	}

	_pEntity->SetActive(false);
#ifdef _DEBUG
	string meshName;
	meshName = _pEntity->GetType();

	if(_pEntity->GetBV() != NULL && _pEntity->GetBV()->GetObjType() == eSPH)
	{
		meshName += (char)((CSphere*)_pEntity->GetBV())->GetRadius();
		_pEntity->SetMesh(m_pRenderer->GetAssetManager()->LoadMeshSphere(meshName.c_str(),((CSphere*)_pEntity->GetBV())->GetRadius()));
	}
	else if(_pEntity->GetBV() != NULL && _pEntity->GetBV()->GetObjType() == eCAP)
	{
		meshName += (char)((CCapsule*)_pEntity->GetBV())->GetRadius();
		meshName += (char)((CCapsule*)_pEntity->GetBV())->GetLength();
		_pEntity->SetMesh(m_pRenderer->GetAssetManager()->LoadMeshCapsule(meshName.c_str(),((CCapsule*)_pEntity->GetBV())->GetRadius(),((CCapsule*)_pEntity->GetBV())->GetLength()));
	}
#endif
	m_vInactiveEnemyList.push_back(_pEntity);
}

/*****************************************************************
* AddToInactiveArmament():     Adds an inactive missile or laser 
*							   to the armament list.
* Ins:			   CEntity*    		      
* Outs:			   None
* Returns:		   None
* Mod. Date:		
* Mod. Initials:	
*****************************************************************/
void CObjectManager::AddToInactiveArmament( CEntity* _pEntity )
{
	if(_pEntity == nullptr )
	{
		return;
	}

	_pEntity->SetActive(false);

#ifdef _DEBUG
	string meshName;
	meshName = _pEntity->GetType();

	if(_pEntity->GetBV() != NULL && _pEntity->GetBV()->GetObjType() == eSPH)
	{
		meshName += (char)((CSphere*)_pEntity->GetBV())->GetRadius();
		_pEntity->SetMesh(m_pRenderer->GetAssetManager()->LoadMeshSphere(meshName.c_str(),((CSphere*)_pEntity->GetBV())->GetRadius()));
	}
	else if(_pEntity->GetBV() != NULL && _pEntity->GetBV()->GetObjType() == eCAP)
	{
		meshName += (char)((CCapsule*)_pEntity->GetBV())->GetRadius();
		meshName += (char)((CCapsule*)_pEntity->GetBV())->GetLength();
		_pEntity->SetMesh(m_pRenderer->GetAssetManager()->LoadMeshCapsule(meshName.c_str(),((CCapsule*)_pEntity->GetBV())->GetRadius(),((CCapsule*)_pEntity->GetBV())->GetLength()));
	}
#endif

	m_vInactiveArmamentList.push_back(_pEntity);
}

/*****************************************************************
* ReturnAsteroidToMasterList():     Returns an inactive asteroid to 
*									the master list.
* Ins:			   None    		      
* Outs:			   None
* Returns:		   None
* Mod. Date:		
* Mod. Initials:	
*****************************************************************/
CEntity* CObjectManager::ReturnAsteroidToMasterList( float radius)
{

	if(m_vInactiveAsteroidList.size() <= 0)
	{
		return 0;
	}

	std::vector< CEntity* >::iterator iterAsteroid = m_vInactiveAsteroidList.begin();
	CEntity* pEntity = (*iterAsteroid);
	pEntity->SetActive(true);
	m_vInactiveAsteroidList.erase(iterAsteroid);
	((CAsteroid*)pEntity)->SetRadius(radius);

#ifdef _DEBUG
	string meshName;
	meshName = pEntity->GetType();

	meshName += char(((CSphere*)pEntity->GetBV())->GetRadius());

	if(pEntity->GetBV() != NULL && pEntity->GetBV()->GetObjType() == eSPH)
	{
		pEntity->SetMesh(m_pRenderer->GetAssetManager()->LoadMeshSphere(meshName.c_str(),((CSphere*)pEntity->GetBV())->GetRadius()));
	}
	else if(pEntity->GetBV() != NULL && pEntity->GetBV()->GetObjType() == eCAP)
	{
		meshName += (char)((CCapsule*)pEntity->GetBV())->GetRadius();
		meshName += (char)((CCapsule*)pEntity->GetBV())->GetLength();
		pEntity->SetMesh(m_pRenderer->GetAssetManager()->LoadMeshCapsule(meshName.c_str(),((CCapsule*)pEntity->GetBV())->GetRadius(),((CCapsule*)pEntity->GetBV())->GetLength()));
	}
#endif

	m_vPendingToAddtoMasterList.push_back(pEntity);

	return pEntity;

}

/*****************************************************************
* ReturnEnemyToMasterList():     Returns an inactive Enemy to 
*								 the master list.
* Ins:			   None    		      
* Outs:			   None
* Returns:		   None
* Mod. Date:		
* Mod. Initials:	
*****************************************************************/
CEntity* CObjectManager::ReturnEnemyToMasterList( GameObjectTypes _eType)
{

	std::vector< CEntity* >::iterator iterEnemy; 
	CEntity* pEntity = nullptr;
	CEntity* pInactiveListIter = nullptr;

	for(iterEnemy = m_vInactiveEnemyList.begin(); iterEnemy != m_vInactiveEnemyList.end(); ++iterEnemy)
	{
		pInactiveListIter = (*iterEnemy);
		if(pInactiveListIter->GetType() == _eType)
		{
			pEntity = (*iterEnemy);
			pEntity->SetActive(true);
			m_vInactiveEnemyList.erase(iterEnemy);

			m_vPendingToAddtoMasterList.push_back(pEntity);

			m_vIndicatorList.push_back(pEntity);



			if(_eType == eBLUE_ENEMY)
			{
				std::vector< CEntity* >::iterator iterShield;
				CEntity * shield = pEntity->GetShield();
				CEntity* pShieldIter = nullptr;

				for(iterShield = m_vCatchAllList.begin(); iterShield != m_vCatchAllList.end(); ++iterShield)
				{
					pShieldIter = (*iterShield);
					if(shield == pShieldIter)
					{
						m_vPendingToAddtoMasterList.push_back(pShieldIter);
						m_vCatchAllList.erase(iterShield);
						break;
					}
				}
			}

			break;
		}
	}

	return pEntity;
}

/*****************************************************************
* ReturnArmamentToMasterList():     Returns an inactive Laser or Missile
*								 to the master list.
* Ins:			   None    		      
* Outs:			   None
* Returns:		   None
* Mod. Date:		
* Mod. Initials:	
*****************************************************************/
CEntity* CObjectManager::ReturnArmamentToMasterList( GameObjectTypes _eType )
{

	std::vector< CEntity* >::iterator iterArmament; 
	CEntity* pEntity = nullptr;
	CEntity* pInactiveArmamentIter = nullptr;

	for(iterArmament = m_vInactiveArmamentList.begin(); iterArmament != m_vInactiveArmamentList.end(); ++iterArmament)
	{
		pInactiveArmamentIter = (*iterArmament);
		if(pInactiveArmamentIter->GetType() == _eType)
		{
			pEntity = (*iterArmament);
			pEntity->SetActive(true);
			m_vInactiveArmamentList.erase(iterArmament);
			m_vPendingToAddtoMasterList.push_back(pEntity);
			break;
		}
	}

	return pEntity;
}

/*****************************************************************
* CheckCollision():     Passes all objects that need to be checked to the CollisionLib.
* Ins:			   None    		      
* Outs:			   None
* Returns:		   None
* Mod. Date:		
* Mod. Initials:	
*****************************************************************/
void CObjectManager::CheckCollision( bool _bBossCin /*= false*/, CGame* _pGame )
{

	if( m_bIterating )
	{
		return;
	}

	//TODO: SW Better Object Checking System
	int nFirstObjectType = 0;
	int nSecondObjectType = 0;
	CEntity* pFirstListIterData = nullptr;
	CEntity* pSecondListIterData = nullptr;

	//Create the iterator
	std::vector< CEntity* >::iterator iterMaster;//, iterActiveList1, iterActiveList2,iterEnd;
	//iterEnd = m_vMasterList.end();

	CAssetManager* ass = m_pRenderer->GetAssetManager();

	// ALL OBJECT WITHOUT BOSS
	for (int i=0; i<NUM_BUCKETS; i++)
	{

		if(m_vHashTable[i] == 0)
			continue;
		if(m_vHashTable[i]->pNext == 0)
			continue;
		//for(iterActiveList1 = m_vMasterList.begin(); iterActiveList1 != iterEnd; ++iterActiveList1)
		for(tNode* iterActiveList1 = m_vHashTable[i]; iterActiveList1; iterActiveList1 = iterActiveList1->pNext)
		{
			pFirstListIterData = iterActiveList1->pObj;
			nFirstObjectType = pFirstListIterData->GetType();
			if( nFirstObjectType == eSHIELD)
			{
				continue;
			}

			//	for(++(iterActiveList2 = iterActiveList1); iterActiveList2 != iterEnd; ++iterActiveList2)
			for(tNode* iterActiveList2 = m_vHashTable[i]; iterActiveList2; iterActiveList2 = iterActiveList2->pNext)
			{
				pSecondListIterData = iterActiveList2->pObj;
				nSecondObjectType = pSecondListIterData->GetType();

				if(nSecondObjectType == eSHIELD)
				{
					continue;
				}

				if(pFirstListIterData != pSecondListIterData && nFirstObjectType != nSecondObjectType)
				{
					if(nFirstObjectType == eLASER && ((CLaser*)pFirstListIterData)->GetOwner() == pSecondListIterData) 	
					{
						continue;
					}
					else if (nSecondObjectType == eLASER  && ((CLaser*)pSecondListIterData)->GetOwner() == pSecondListIterData)
					{
						continue;
					}
					else if(!_bBossCin)
					{
						if (pFirstListIterData->GetIsScenery() || pSecondListIterData->GetIsScenery() )
						{
							continue;
						}
					}


					CCollisionLib::CheckCollision(pFirstListIterData, pSecondListIterData, ass, _pGame);
					CCollisionLib::CheckCollision(pSecondListIterData, pFirstListIterData, ass, _pGame);
				}

				if(m_pBoss != nullptr && m_pIterPlayer != nullptr)
				{
					unsigned int helperSize = ((CBoss*)m_pBoss)->GetCollisionHelp().size();
					if (nFirstObjectType == eASTEROID || nFirstObjectType == eLARGE_ASTEROID)
					{
						if (((CBoss*)m_pBoss)->GetCollisionHelp()[6]->GetActive())
						{
							CCollisionLib::CheckCollision(((CBoss*)m_pBoss)->GetCollisionHelp()[6], pFirstListIterData, ass, _pGame);
						}
					}
					else if (nFirstObjectType == eSPACE_DEBRIS || nFirstObjectType == eSPACE_DEBRIS2 || nFirstObjectType == eSPACE_DEBRIS3 || nFirstObjectType == eMISSILE)
					{
						if (((CBoss*)m_pBoss)->GetCollisionHelp()[6]->GetActive() && !((CEnvironmentObject*) pFirstListIterData)->GetBossHelper())
						{
							CCollisionLib::CheckCollision(((CBoss*)m_pBoss)->GetCollisionHelp()[6], pFirstListIterData, ass, _pGame);
						}
					}

				}

			}
		}
	}
	//TODO: GOLD
	//This needs to be optimized out, find the boss inside of another iteration of the master list and store it in the header
	// check the new pointer that gets caught in the other itheration for null to see if it has been created yet

	// BOSS AGAINST ALL OBJECTS
	//CEntity* pMasterListIter = nullptr;
	//
	//for(iterMaster = m_vMasterList.begin(); iterMaster != m_vMasterList.end(); ++iterMaster )
	//{
	//	pMasterListIter = (*iterMaster);
	//	if(pMasterListIter->GetType() == eBOSS)
	//	{
	//		pBoss = pMasterListIter;
	//		break;
	//	}
	//}
	////////////////////////////////////////////////////////////////////////////////////////GOLD
	//pMasterListIter = nullptr;
	//
	if(m_pBoss != nullptr && m_pIterPlayer != nullptr)
	{
		CCollisionLib::CheckCollision(m_pIterPlayer, m_pBoss, ass);
		CCollisionLib::CheckCollision(m_pBoss, m_pIterPlayer, ass);

		unsigned int helperSize = ((CBoss*)m_pBoss)->GetCollisionHelp().size();

		for (unsigned i = 0; i < helperSize; ++i)
		{
			if (((CBoss*)m_pBoss)->GetCollisionHelp()[i]->GetActive())
			{
				CCollisionLib::CheckCollision(m_pIterPlayer, ((CBoss*)m_pBoss)->GetCollisionHelp()[i], ass, _pGame);
				CCollisionLib::CheckCollision(((CBoss*)m_pBoss)->GetCollisionHelp()[i], m_pIterPlayer, ass, _pGame);
			}

		}
		//	for(iterMaster = m_vMasterList.begin(); iterMaster != m_vMasterList.end(); ++iterMaster )
		//	{
		//		pMasterListIter = (*iterMaster);
		//		if(pMasterListIter->GetType() == eBOSS)
		//		{
		//			continue;
		//		}
		//
		//		if(pMasterListIter->GetType() == eRED_PLAYER || pMasterListIter->GetType() == eBLUE_PLAYER)
		//		{
		//			m_pIterPlayer = pMasterListIter;
		//			CCollisionLib::CheckCollision(pMasterListIter, pBoss, &ass);
		//		}
		//		else
		//		{
		//			CCollisionLib::CheckCollision(pBoss, pMasterListIter, &ass);
		//			CCollisionLib::CheckCollision(pMasterListIter, pBoss, &ass);
		//		}
		//	}
	}
	//
	//pMasterListIter = nullptr;
	//// BOSS BIG LASER
	//
	////TODO: GOLD WAIT WHAT IS THIS FOR AND WHATS IT DOING? WAS THIS NOT ALREADY PART OF THE LOOP BEFORE THIS?
	//CEntity* pBigBossLaser = 0, *pPlayer = 0;
	//
	//for(iterMaster = m_vMasterList.begin(); iterMaster != m_vMasterList.end(); ++iterMaster )
	//{
	//	pMasterListIter = (*iterMaster);
	//	if(pMasterListIter->GetType() == eBARRIER)
	//	{
	//		pBigBossLaser = pMasterListIter;
	//		//break;
	//	}
	//	else if(pMasterListIter->GetType() == eRED_PLAYER || pMasterListIter->GetType() == eBLUE_PLAYER)
	//	{
	//		pPlayer = pMasterListIter;
	//	}
	//
	//	if( pBigBossLaser && pPlayer)
	//	{
	//		break;
	//	}
	//}
	//
	//if(pBigBossLaser && pPlayer)
	//{
	//	
	//	CCollisionLib::CheckCollision(pPlayer, pBigBossLaser, &ass);
	//	
	//}

	//pMasterListIter = nullptr;

	//Loop through deleting them all!
	for(iterMaster = m_vMasterList.begin(); iterMaster != m_vMasterList.end();  )
	{
		pFirstListIterData = (*iterMaster);
		nFirstObjectType = pFirstListIterData->GetType();
		if(nFirstObjectType == eBOSSTHRUSTER)
		{
			++iterMaster;
			continue;
		}
		if(!pFirstListIterData->GetActive() && nFirstObjectType != eRED_PLAYER 
			&& nFirstObjectType != eBLUE_PLAYER )
		{
			//Put it into the correct inactive list
			switch(nFirstObjectType)
			{
			case eASTEROID:
				{
					if(!_bBossCin && m_bBossFight)
						((CAsteroid*)pFirstListIterData)->DeathVFX();
					AddToInactiveAsteroid(pFirstListIterData);
				}
				break;
			case eRED_ENEMY:
			case eBLUE_ENEMY:
			case eYELLOW_ENEMY:
			case eBOSS:
				{
					if(nFirstObjectType == eRED_ENEMY)
					{
						((CRedEnemy*)pFirstListIterData)->GetTrailEffect()->SetPlay(false);
						((CRedEnemy*)pFirstListIterData)->GetDamagedEffect()->SetPlay(false);

					}
					else if(nFirstObjectType == eBLUE_ENEMY)
					{
						((CBlueEnemy*)pFirstListIterData)->GetTrailEffect()->SetPlay(false);
						((CBlueEnemy*)pFirstListIterData)->GetDamagedEffect()->SetPlay(false);
					}
					else if(nFirstObjectType == eYELLOW_ENEMY)
					{
						((CYellowEnemy*)pFirstListIterData)->GetTrailEffect()->SetPlay(false);
						((CYellowEnemy*)pFirstListIterData)->GetDamagedEffect()->SetPlay(false);
					}
					AddToInactiveEnemy(pFirstListIterData);
				}
				break;
			case eMISSILE:
				{
					((CMissile*)pFirstListIterData)->DisableEffects();
				}
			case eBARRIER: 
			case eLASER:
			case eLARGE_ASTEROID:
				{
					AddToInactiveArmament(pFirstListIterData);
				}
				break;
			default:
				{
					m_vCatchAllList.push_back(pFirstListIterData);
				}
				break;
			}
			//Get rid of it and set the iterator to the previous entity
			iterMaster = m_vMasterList.erase( iterMaster );

		}
		else
			++iterMaster;
	}
}

/*****************************************************************
* RenderObjects():     Renders the objects by passing them to the Renderer
* Ins:			       None    		      
* Outs:			 	   None
* Returns:		       None
* Mod. Date:		
* Mod. Initials:	
*****************************************************************/
void CObjectManager::RenderObjects()
{
	//TODO SW switch to only things in the frustum
	//TODO DEVAN : MAKE THIS WORK 
	//if/else check is so that ambient dog fights can be toggled on and off with a debug key so that they can be tested
	//and shown to lab aids that it works



	if(m_bShowDogFights && !m_bBossFight)
	{
		m_pRenderer->RenderObjects(&m_vRenderList,&m_vAmbientDogFights, m_vAllEffects,&m_vTransparentEnemyList);
	}
	else
	{
		m_pRenderer->RenderObjects(&m_vRenderList, &vector<CAmbientBoundingBox*>(), m_vAllEffects, &m_vTransparentEnemyList);
	}


}

void CObjectManager::RenderMainMenuObjects()
{
	//TODO SW switch to only things in the frustum
	//TODO DEVAN : MAKE THIS WORK 
	//if/else check is so that ambient dog fights can be toggled on and off with a debug key so that they can be tested
	//and shown to lab aids that it works

	m_pRenderer->RenderMainMenuObjects(&m_vMasterList, m_vAllEffects);

}


/*****************************************************************
* Shutdown():     Removes and deletes all objects
* Ins:			   None    		      
* Outs:			   None
* Returns:		   None
* Mod. Date:		
* Mod. Initials:	
*****************************************************************/
void CObjectManager::Shutdown()
{
	std::vector< CEntity* >::iterator iterAll;

	for(iterAll = m_vMasterList.begin(); iterAll != m_vMasterList.end(); ++iterAll)
	{
		if(*iterAll != nullptr)
		{
			delete (*iterAll);
			(*iterAll) = nullptr;
		}
	}

	m_vMasterList.clear();

	for(iterAll = m_vInactiveAsteroidList.begin(); iterAll != m_vInactiveAsteroidList.end(); ++iterAll)
	{
		if(*iterAll != nullptr)
		{
			delete (*iterAll);
			(*iterAll) = nullptr;
		}
	}

	m_vInactiveAsteroidList.clear();

	for(iterAll = m_vInactiveEnemyList.begin(); iterAll != m_vInactiveEnemyList.end(); ++iterAll)
	{
		if(*iterAll != nullptr)
		{
			delete (*iterAll);
			(*iterAll) = nullptr;
		}
	}

	m_vInactiveEnemyList.clear();

	for(iterAll = m_vInactiveArmamentList.begin(); iterAll != m_vInactiveArmamentList.end(); ++iterAll)
	{
		if(*iterAll != nullptr)
		{
			delete (*iterAll);
			(*iterAll) = nullptr;
		}
	}

	m_vInactiveArmamentList.clear();



	for(iterAll = m_vPendingToAddtoMasterList.begin(); iterAll != m_vPendingToAddtoMasterList.end(); ++iterAll)
	{
		if(*iterAll != nullptr)
		{
			delete (*iterAll);
			(*iterAll) = nullptr;
		}
	}

	m_vPendingToAddtoMasterList.clear();


	for(iterAll = m_vCatchAllList.begin(); iterAll != m_vCatchAllList.end(); ++iterAll)
	{
		if(*iterAll != nullptr)
		{
			delete (*iterAll);
			(*iterAll) = nullptr;
		}
	}	
	m_vCatchAllList.clear();

	std::vector<CAmbientBoundingBox*>::iterator iterAmbientBoundingBox;
	for(iterAmbientBoundingBox = m_vAmbientDogFights.begin(); iterAmbientBoundingBox != m_vAmbientDogFights.end(); ++iterAmbientBoundingBox)
	{
		if(*iterAmbientBoundingBox != nullptr) 
		{
			delete (*iterAmbientBoundingBox);
			(*iterAmbientBoundingBox) = nullptr;
		}
	}	
	m_vCatchAllList.clear();

	m_vIndicatorList.clear();
	m_vRenderList.clear();

	for(unsigned int i(0); i < m_vAmbientDogFights.size(); ++i)
	{
		delete m_vAmbientDogFights[i];
		m_vAmbientDogFights[i] = NULL;
	}
	m_vAmbientDogFights.clear();

	
	DismantleHashTable();
	

	auto iter = m_vUnusedNodes.begin();
	//auto iterEnd = m_vUnusedNodes.end();
	while (iter != m_vUnusedNodes.end())
	{
		tNode* tmp = *iter;
		iter = m_vUnusedNodes.erase(iter);
		delete tmp;
	}
}

/*****************************************************************
* SpawnEntity():   Spawns an entity if it needs to
* Ins:			   CEntity*    		      
* Outs:			   None
* Returns:		   None
* Mod. Date:		
* Mod. Initials:	
*****************************************************************/
void CObjectManager::SpawnEntity(CEntity* _pEntity, float _fDeltaTime)
{
	char cFlags = _pEntity->GetFlags();

	if(cFlags & eFIRE_MISSILE)
	{
		CMissile* pMissile = (CMissile*)ReturnArmamentToMasterList(eMISSILE);
		if(pMissile != nullptr)
		{

			D3DXVECTOR3	d3dMissileMat =  _pEntity->GetPosition();
			int _type = _pEntity->GetType();
			if (_type == eTURRET_MISSILE || _type == eHanger)
			{
				d3dMissileMat.y += 20;
				d3dMissileMat.z += 5;
			}
			else
			{
				d3dMissileMat.z += 7;
			}
			pMissile->SetVelocity((D3DXVECTOR3(0, -40, -7)));

			pMissile->SetActive(true);
			pMissile->SetPosition(d3dMissileMat);
			CPhysics::LookAt( pMissile, _pEntity->GetTarget()  );

			if( _type != eHanger )
			{
				pMissile->SetTarget((_pEntity)->GetTarget());
			}

			WwiseNS::SoundEvent(eEnemyMissileFire, ePlay, false, _pEntity);
			pMissile->SetIsThrusted(false);
			pMissile->SetIsVisable(true);
			pMissile->SetIsScenery(false);
			pMissile->SetLifetime(6.5);
			pMissile->SetHealth(100);
			pMissile->SetOwner(_pEntity);
			pMissile->SetDoOnce(true);
			pMissile->SetVelocityModifier(s_AIHelper->GetmissileVelocityModifier());
			for (unsigned int i = 0; i < pMissile->GetMovementBehaviorStack().size(); i++)
			{
				pMissile->PopBehavior();
			}

			pMissile->CreateEffect(eBLUE_ENEMY_FIRE_MISSILE);
		}
	}
	if(cFlags & eFIRE_BOSSLAZER)
	{

	}
	if (cFlags & eFIRE_LASER)
	{
		bool fireLaser = true;
		for (unsigned int i = 0; i < m_vIndicatorList.size(); i++)
		{
			if (m_vIndicatorList[i] == _pEntity)
			{
				continue;
			}
			if(	m_vIndicatorList[i]->GetType() == eRED_ENEMY)
			{
				D3DXVECTOR3 d3dEnemyPosition = _pEntity->GetPosition();
				D3DXVECTOR3 d3dPartnerPosition = m_vIndicatorList[i]->GetPosition();
				D3DXVECTOR3 vectorBetween = d3dEnemyPosition - d3dPartnerPosition ;
				float fSquaredDistance = D3DXVec3Length( &(vectorBetween ));

				D3DXVECTOR3 zAxis = CPhysics::GetMat4Vec(eZAXIS,m_vIndicatorList[i]->GetTarget()->GetMatrix() );
				float halfSpace = D3DXVec3Dot(D3DXVec3Normalize(&vectorBetween,&vectorBetween ),D3DXVec3Normalize(&zAxis, &zAxis ));

				if( halfSpace > .90f && fSquaredDistance < 20)
				{
					fireLaser = false;
				}
			}
		} 

		if (fireLaser)
		{
			CLaser* pLaser = (CLaser*)ReturnArmamentToMasterList(eLASER);

			if(pLaser != nullptr)
			{
				if(_pEntity->GetType() == eAMBIENT_ENEMY)
				{
					WwiseNS::SoundEvent(eEnemyLaserFire, ePlay, false, _pEntity);

					pLaser->SetIsScenery(true);
					pLaser->SetLifetime(1.5f);
					D3DXMATRIX _scale;
					D3DXMatrixIdentity(&_scale);
					D3DXMatrixScaling(&_scale, 8.0f, 8.0f, 8.0f);
					pLaser->SetMatrix(&(_scale*_pEntity->GetMatrixCopy()));
				}
				else
				{
					// Play laser fire sound with enemy data for 3D sounds
					if(_pEntity->GetType() == eTURRET_LASER || _pEntity->GetType() == eTURRET_SMALL)
					{
						//J.wu Super safety Check.
						CEntity* tmpEntity = _pEntity->GetTarget();
						if(tmpEntity != nullptr && (tmpEntity->GetType() == eRED_PLAYER || tmpEntity->GetType() == eBLUE_PLAYER))
						{
							if(_pEntity->GetActive() && ((CPlayer*)tmpEntity)->GetHealth() > -1 && WwiseNS::GetPreventPlaying() == false)
								WwiseNS::SoundEvent(eHydraPeaShooter, ePlay, false, _pEntity);
						}
					}
					else// if(_pEntity->GetType() == eRED_ENEMY || _pEntity->GetType() == eYELLOW_ENEMY)
					{
						if(_pEntity->GetActive())
							WwiseNS::SoundEvent(eEnemyLaserFire, ePlay, false, _pEntity);
					}
					pLaser->SetMatrix(&_pEntity->GetMatrixCopy());
					pLaser->SetLifetime(LASER_LIFETIME);
				}
				//Laser->SetPosition(LaserCast(_pEntity->GetPosition(), CPhysics::GetMat4Vec(eZAXIS, _pEntity->GetMatrix()), false));
				pLaser->SetTarget(_pEntity->GetTarget());
				pLaser->SetOwner(_pEntity);
				pLaser->SetShouldBounce(false);
				pLaser->CreateEffect(eRED_ENEMY_FIRE_LASER);
				pLaser->Scale(1.0f, 1.0f, 6.0f);
			}


		}
	} 

	// 	if (cFlags & eADD_RED_ENEMY)
	// 	{
	// 		for (int i = 1; i < 3; i++)
	// 		{
	// 
	// 			CBossHanger * bossHanger =	( (CBossHanger *)_pEntity);
	// 			CRedEnemy * pNewEnemy = (CRedEnemy*)ReturnEnemyToMasterList( eRED_ENEMY );
	// 			if (pNewEnemy != nullptr)
	// 			{
	// 				pNewEnemy->SetTarget(bossHanger->GetPlayer());
	// 				D3DXVECTOR3 tempPos = bossHanger->GetPosition();
	// 
	// 				tempPos.x += (40 * i);
	// 				tempPos.y += (10 * i);
	// 				tempPos.z += 20;
	// 				pNewEnemy->SetPosition( tempPos );
	// 				pNewEnemy->SetHealth(s_AIHelper->GetHealth());
	// 				pNewEnemy->SetShields(s_AIHelper->GetredShield());
	// 				pNewEnemy->SetFlockID(0);
	// 				pNewEnemy->RemoveAllPartners();
	// 				if (pNewEnemy->GetMovementBehavior())
	// 				{
	// 					pNewEnemy->GetMovementBehavior()->SetForceChasebool(false);
	// 				}
	// 			}
	// 		}
	// 	}
	// 
	// 	if (cFlags & eADD_BLUE_ENEMY)
	// 	{
	// 		CBossHanger * bossHanger =	( (CBossHanger *)_pEntity);
	// 		CBlueEnemy * pNewEnemy = (CBlueEnemy*)ReturnEnemyToMasterList( eBLUE_ENEMY );
	// 		if (pNewEnemy != nullptr)
	// 		{
	// 			pNewEnemy->SetTarget(bossHanger->GetPlayer());
	// 			D3DXVECTOR3 tempPos = bossHanger->GetPosition();
	// 			tempPos.x += 40 ;
	// 			tempPos.y += 10 ;
	// 			tempPos.z += 20;
	// 			pNewEnemy->SetPosition( tempPos );
	// 			pNewEnemy->SetHealth(s_AIHelper->GetHealth());
	// 			pNewEnemy->SetShields(s_AIHelper->GetblueShield());
	// 			pNewEnemy->SetIsStunned(false, 0.0f);
	// 			pNewEnemy->GetShield()->SetActive(true);
	// 
	// 			if (pNewEnemy->GetMovementBehavior())
	// 			{
	// 				pNewEnemy->GetMovementBehavior()->SetForceChasebool(false);
	// 			}
	// 		}
	// 	}
	// 	if (cFlags & eADD_YELLOW_ENEMY)
	// 	{
	// 		CBossHanger * bossHanger =	( (CBossHanger *)_pEntity);
	// 		CYellowEnemy * pNewEnemy = (CYellowEnemy*)ReturnEnemyToMasterList( eYELLOW_ENEMY );
	// 		if (pNewEnemy != nullptr)
	// 		{
	// 
	// 			D3DXVECTOR3 tempPos = bossHanger->GetPosition();
	// 			tempPos.z -= 50;
	// 			tempPos.x += 20;
	// 			tempPos.y -= 20;
	// 			pNewEnemy->SetPosition( tempPos );
	// 			pNewEnemy->SetHealth(s_AIHelper->GetHealth());
	// 			pNewEnemy->SetShields(s_AIHelper->GetyellowShield());
	// 			pNewEnemy->SetIsStunned(false, 0.0f);
	// 			pNewEnemy->SetPlayer( bossHanger->GetPlayer() );
	// 			vector<CEntity*> vpPath = pNewEnemy->GetPath();
	// 			unsigned int uiPathSize = pNewEnemy->GetPath().size();
	// 			for (unsigned int uiPaths = 0; uiPaths < uiPathSize; uiPaths++)
	// 			{
	// 				if (vpPath[uiPaths])
	// 				{
	// 					delete vpPath[uiPaths];
	// 					vpPath[uiPaths] = 0;
	// 				}
	// 			}
	// 			vpPath.clear();
	// 
	// 			for (unsigned int uiWay = 0; uiWay < 3; uiWay++)
	// 			{
	// 				CEntity * pWpt = new CEntity();
	// 				D3DXVECTOR3 tempPos = bossHanger->GetPosition();
	// 				tempPos.z += (rand() % 500) - 250;
	// 				tempPos.x += (rand() % 500) - 250;
	// 				tempPos.y += (rand() % 500) - 250;
	// 				pWpt->SetPosition( tempPos );
	// 
	// 				pNewEnemy->AddWaypoint(pWpt);
	// 			}
	// 			pNewEnemy->SetTarget( pNewEnemy->GetPath()[0]);
	// 		}
	// 	}
	if (cFlags & eAVOIDANCE)
	{

		for(unsigned int i = 0; i < m_vIndicatorList.size(); ++i)
		{
			if( _pEntity != m_vIndicatorList[i] )
			{
				D3DXVECTOR3 d3dEnemyPosition = _pEntity->GetPosition();
				D3DXVECTOR3 d3dPartnerPosition = m_vIndicatorList[i]->GetPosition();
				D3DXVECTOR3 vectorBetween = d3dEnemyPosition - d3dPartnerPosition ;
				float fSquaredDistance = D3DXVec3Length( &(vectorBetween ));
				if ( fSquaredDistance <= 15 )
				{

					if ( d3dEnemyPosition.x  > d3dPartnerPosition.x )
					{
						d3dEnemyPosition.x += 50 * _fDeltaTime;
					}
					else
					{
						d3dEnemyPosition.x -=  50 * _fDeltaTime;

					}
					if ( d3dEnemyPosition.y > d3dPartnerPosition.y)
					{
						d3dEnemyPosition.y +=  50 * _fDeltaTime;

					}
					else
					{
						d3dEnemyPosition.y +=  50 * _fDeltaTime;

					}


				}
				_pEntity->SetPosition( d3dEnemyPosition );
			}
		}



	}

	_pEntity->SetFlags(0);


}

/*****************************************************************
* EnemyLockOnTarget():     Locks on the Target being pointed at
* Ins:			   D3DXVECTOR3 _pos, D3DXVECTOR3 _dir    		      
* Outs:			   Pointer to the Target
* Returns:		   None
* Mod. Date:		
* Mod. Initials:	
*****************************************************************/
CEntity* CObjectManager::EnemyLockOnTarget( D3DXVECTOR3 _d3dPos, D3DXVECTOR3 _d3dDir, float _fScale, float _time )
{
	if( m_bIterating )
	{
		return nullptr;
	}

	CEntity* pTheOneWeWant = nullptr;

	float fClosestScale = FLT_MAX, fTmpScale = FLT_MAX;
	D3DXVECTOR3 d3dVec;

	//Create the iterator
	std::vector< CEntity* >::iterator iterActiveList1;
	CEntity* pIndicatorListIter = nullptr;

	for(iterActiveList1 = m_vIndicatorList.begin(); iterActiveList1 != m_vIndicatorList.end(); ++iterActiveList1)
	{
		pIndicatorListIter = (*iterActiveList1);
		//There are only enemies and waypoints in this list
		if(pIndicatorListIter->GetType() != eWAYPOINT && !pIndicatorListIter->GetIsScenery() && pIndicatorListIter->GetType() != eSPACE_DEBRIS)
		{

			if(pIndicatorListIter->GetBV()->GetObjType() == eCAP)
			{
				CCapsule tmpCap = *(CCapsule*)(pIndicatorListIter->GetBV());
				tmpCap.SetStart(tmpCap.GetStart() + pIndicatorListIter->GetPosition());
				tmpCap.SetEnd(tmpCap.GetEnd() + pIndicatorListIter->GetPosition());
				tmpCap.SetRadius(tmpCap.GetRadius()*_fScale);
				CCollisionLib::IntersectRayCapsule(_d3dPos, _d3dDir, &tmpCap, fTmpScale, d3dVec);
			}
			else
			{
				CSphere tmpSph = *(CSphere*)(pIndicatorListIter->GetBV());
				tmpSph.SetPosition(tmpSph.GetPosition() + pIndicatorListIter->GetPosition());
				tmpSph.SetRadius(tmpSph.GetRadius()*_fScale);
				CCollisionLib::RayToSphere(_d3dPos, _d3dDir, tmpSph, fTmpScale, d3dVec);
			}

			//Make sure we picked the closest one
			if(fClosestScale > fTmpScale)
			{
				fClosestScale = fTmpScale;
				if (pIndicatorListIter == m_pPreviousTarget)
				{
					m_fTargetFleeCounter += _time;
					if (m_fTargetFleeCounter >= 11.45f)
					{
						((CEnemy*)pIndicatorListIter)->SetShouldChasePlayer(true);
						m_fTargetFleeCounter = 0.0f;
					}
				}
				// 				else
				// 				{
				// 				//	m_fTargetFleeCounter = 0.0f;
				// 				}
				m_pPreviousTarget = pIndicatorListIter;
				pTheOneWeWant = pIndicatorListIter;
				((CEnemy*)pTheOneWeWant)->SetShouldRun(true);
			}
		}
	}

	// #if _DEBUG
	// 	if(pTheOneWeWant)
	// 	{
	// 		cout << "Missile Locked!" << endl;
	// 	}
	// #endif

	return pTheOneWeWant;
}



/*****************************************************************
* QuickSort():     Sorts the renderable list before sending it to the 
*				   Renderer.
* Ins:			   vector<CEntity *> &, int, int   		      
* Outs:			   None
* Returns:		   None
* Mod. Date:		
* Mod. Initials:	
*****************************************************************/
void CObjectManager::QuickSort(vector<CEntity *> & _vpRenderableObjects, int _nLeft, int _nRight)
{
	int nLeftSide = _nLeft, nRightSide = _nRight;
	CEntity* pEntity;
	CEntity* pPivot = (CEntity*)_vpRenderableObjects[(_nLeft + _nRight) / 2];

	//Loop to sort through the array
	while (nLeftSide <= nRightSide)
	{
		while (D3DXVec3Length(&(_vpRenderableObjects[nLeftSide]->GetPosition() - m_pCamera->GetPosition( ) ) ) > D3DXVec3Length(&(pPivot->GetPosition( ) - m_pCamera->GetPosition( ) ) ) )
		{
			nLeftSide++;
		}
		while (D3DXVec3Length(&(_vpRenderableObjects[nRightSide]->GetPosition() - m_pCamera->GetPosition( ) ) ) > D3DXVec3Length(&(pPivot->GetPosition( ) - m_pCamera->GetPosition( ) ) ) )
		{
			nRightSide--;
		}

		//Swap the values inside
		if (nLeftSide <= nRightSide)
		{
			pEntity = _vpRenderableObjects[nLeftSide];
			_vpRenderableObjects[nLeftSide] = _vpRenderableObjects[nRightSide];
			_vpRenderableObjects[nRightSide] = pEntity;
			nLeftSide++;
			nRightSide--;
		}
	}

	if (_nLeft < nRightSide)
	{
		QuickSort(_vpRenderableObjects, _nLeft, nRightSide);
	}

	if (nLeftSide < _nRight)
	{
		QuickSort(_vpRenderableObjects, nLeftSide, _nRight);
	}
}

/*****************************************************************
* MakeRenderableList():  Perform Frustum Culling
* Ins:			   None    		      
* Outs:			   None
* Returns:		   None
* Mod. Date:		
* Mod. Initials:	
*****************************************************************/
void CObjectManager::MakeRenderableList()
{
	m_vRenderList.clear();

	std::vector< CEntity* >::iterator iterMaster;
	CCollisionLib Collision;

	tFrustum* tempFrustum =  m_pCamera->GetFrustum();
	CEntity* pMasterListIter = nullptr;

	CSphere tempSphere;
	CCapsule tempCapsule;
	for(iterMaster = m_vMasterList.begin(); iterMaster != m_vMasterList.end(); ++iterMaster)
	{
		pMasterListIter = (*iterMaster);
		if( pMasterListIter->GetBV() != nullptr  && pMasterListIter->GetBV()->GetObjType() == eSPH)
		{
			if(m_bInCinematic == true)
			{
				m_vRenderList.push_back(pMasterListIter);
				continue;
			}

			int nType = pMasterListIter->GetType();

			if(/*nType == ePLANET1 || nType == ePLANET2 || nType == ePLANET3 || nType == ePLANET4 ||*/ nType == eSPACE_STATION || nType == eSPACESTATION1 || nType ==  eSATELITE)
			{
				m_vRenderList.push_back(pMasterListIter);
				continue;
			}

			if(nType == eRED_PLAYER || nType == eBLUE_PLAYER)
			{
				m_vRenderList.push_back(pMasterListIter);
				continue;
			}

			tempSphere.SetRadius(((CSphere*)(pMasterListIter->GetBV()))->GetRadius());
			tempSphere.SetPosition(CPhysics::GetMat4Vec(ePOS,pMasterListIter->GetMatrix()));

			if(CCollisionLib::FrustumToSphere(*tempFrustum, tempSphere))
			{
				if(nType != eCAM_OBJ )
				{
					m_vRenderList.push_back(pMasterListIter);
				}

				if(nType == eBLUE_ENEMY)
				{
					m_vRenderList.push_back(pMasterListIter->GetShield());
				}

			}
			continue;
		}

		if( pMasterListIter->GetBV() != nullptr  && pMasterListIter->GetBV()->GetObjType() == eCAP)
		{
			//TODO BERN : FIX CAPSULE CHECK
			CCapsule* currentCapsule = (CCapsule*)(*iterMaster)->GetBV();
			tempCapsule.SetRadius(currentCapsule->GetRadius());
			tempCapsule.SetStart(currentCapsule->GetStart() + (*iterMaster)->GetPosition());
			tempCapsule.SetEnd(currentCapsule->GetEnd() + (*iterMaster)->GetPosition());

			if(CCollisionLib::FrustumToCapsule(*tempFrustum, tempCapsule))
			{
				m_vRenderList.push_back(pMasterListIter);
			}
			continue;
		}
	}
}

void CObjectManager::AddEffect(CEffectSystem* _Effect)
{
	std::vector<CEffectSystem*>::iterator iterEffects;
	for(iterEffects = m_vAllEffects.begin(); iterEffects != m_vAllEffects.end(); )
	{
		if((*iterEffects) == _Effect)
		{
			m_vAllEffects.erase(iterEffects);
			break;
		}
		else
		{
			++iterEffects;
		}
	}

	m_vAllEffects.push_back(_Effect);

}

D3DXVECTOR3 CObjectManager::AdjustToAvoidCollision(CEnemy* _pObj, std::vector<int> _vObstacleFlags)
{
	if(_pObj->GetType() != eRED_ENEMY && _pObj->GetType() != eBLUE_ENEMY && _pObj->GetType() != eYELLOW_ENEMY && _pObj->GetType() != eCAM_OBJ)
		return D3DXVECTOR3(0,0,0);
	D3DXVECTOR3 _d3dDirection = D3DXVECTOR3(0,0,0);
	unsigned int uMasterListSize = m_vMasterList.size();
	CEnemy* _pClosestIntersectingObject = NULL;
	float _fDistToObject = 99999.9f;
	float _fRadiusOfCIO = -1.0f;
	for(unsigned int i(0); i < uMasterListSize; ++i)
	{
		if(m_vMasterList[i] == _pObj)
			continue;
		unsigned int obstacleListSize = _vObstacleFlags.size();
		for(unsigned int x(0); x < obstacleListSize; ++x)
		{
			if(m_vMasterList[i]->GetType() == _vObstacleFlags[x])
			{ 
				float _fObjRadius = 0;
				if(m_vMasterList[i]->GetBV()->GetObjType() == eSPH)
				{
					_fObjRadius = ((CSphere*)_pObj->GetBV())->GetRadius();
				}
				else if(m_vMasterList[i]->GetBV()->GetObjType() == eCAP)
				{
					_fObjRadius = ((CCapsule*)_pObj->GetBV())->GetRadius();
				}
				if(!_fObjRadius)
				{
					continue;
				}
				D3DXVECTOR3 _d3dVecToObj = m_vMasterList[i]->GetPosition() - _pObj->GetPosition();
				float _fDist = D3DXVec3LengthSq(&_d3dVecToObj);
				if(_fDist <= _fObjRadius*800)  
				{
					D3DXVECTOR3 _d3dHeading = D3DXVECTOR3(_pObj->GetMatrix()->_31, _pObj->GetMatrix()->_32, _pObj->GetMatrix()->_33);
					float _fTimeToCollide = -1.0f;
					D3DXVECTOR3 _d3dLHFV = D3DXVECTOR3(_pObj->GetMatrix()->_31, _pObj->GetMatrix()->_32, _pObj->GetMatrix()->_33);
					D3DXVECTOR3 _d3dRHFV = D3DXVECTOR3(m_vMasterList[i]->GetMatrix()->_31, m_vMasterList[i]->GetMatrix()->_32, m_vMasterList[i]->GetMatrix()->_33);
					CSphere lhs = *(CSphere*)m_vMasterList[i]->GetBV();
					lhs.SetRadius(lhs.GetRadius() + 1.0f);
					lhs.SetPosition(m_vMasterList[i]->GetPosition());
					CSphere rhs = *(CSphere*)m_vMasterList[i]->GetBV();
					rhs.SetRadius(rhs.GetRadius() + 1.0f);
					rhs.SetPosition(m_vMasterList[i]->GetPosition());
					if( CCollisionLib::MovingSphereToSphere(lhs, rhs, _d3dLHFV, _d3dRHFV, _fTimeToCollide))
					{
						if(_fTimeToCollide < _fDistToObject)
						{
							_fDistToObject = _fTimeToCollide;
							_pClosestIntersectingObject = (CEnemy*)m_vMasterList[i];
							_fRadiusOfCIO = _fObjRadius;
						}
					}
				}

			}
		}
	}
	if(_pClosestIntersectingObject)
	{ 
		float _fMultiplier = 3.0f;
		_d3dDirection.y = (_fRadiusOfCIO - _pClosestIntersectingObject->GetPosition().y) * _fMultiplier;
		float _fBreakingWeight = 0.2f;
		_d3dDirection.x = (_fRadiusOfCIO - _pClosestIntersectingObject->GetPosition().x) * _fMultiplier;
		D3DXVec3Normalize(&_d3dDirection, &_d3dDirection);
	}
	return _d3dDirection;
}

void CObjectManager::AddAmbientFight(D3DXMATRIX _worldMat, int nFighterCount, float _fHeight, float _fDepth, float _fWidth)
{
	CAmbientBoundingBox* _ambBox = new CAmbientBoundingBox(_worldMat, nFighterCount, _fHeight, _fDepth, _fWidth, this);
	m_vAmbientDogFights.push_back(_ambBox);
}

void CObjectManager::AddIntroAmbientFighters(CAmbientBoundingBox* _abb)
{
	m_vAmbientDogFights.push_back(_abb);
}
void CObjectManager::RemoveAmbientFighters()
{
	for(unsigned int i(0); i < m_vAmbientDogFights.size(); ++i)
	{
		if(m_vAmbientDogFights[i]->IsIntroFighters())
		{
			m_vAmbientDogFights.erase(m_vAmbientDogFights.begin() + i);
			break;
		}
	}
}

CEntity* CObjectManager::GetReticuleTarget( D3DXVECTOR3 _d3dBeginPos, D3DXVECTOR3 _d3dEndPos, float _fScale, bool _bBossTargetable )
{
	if( m_bIterating )
	{
		return nullptr;
	}

	CEntity* pTheOneWeWant = nullptr;

	float fClosestScale = FLT_MAX, fTmpScale = FLT_MAX;
	D3DXVECTOR3 d3dVec, d3dDir = _d3dEndPos - _d3dBeginPos;
	D3DXVec3Normalize(&d3dDir, &d3dDir);

	//Create the iterator
	std::vector< CEntity* >::iterator iterActiveList1;

	for(iterActiveList1 = m_vMasterList.begin(); iterActiveList1 != m_vMasterList.end(); ++iterActiveList1)
	{
		CEntity * entity = (*iterActiveList1);
		int nType = entity->GetType();
		//There are only enemies and waypoints in this list
		if(nType != eWAYPOINT && nType != eRED_PLAYER && nType != eBLUE_PLAYER 
			&& nType != eMISSILE && nType != eLASER && entity->GetBV() != 0 && !entity->GetIsScenery() && nType != eSPACE_DEBRIS )
		{
			if(!_bBossTargetable)
			{
				if(nType == eBOSS)
				{
					continue;
				}
			}

			if(entity->GetBV()->GetObjType() == eCAP)
			{
				CCapsule tmpCap = *(CCapsule*)(entity->GetBV());
				tmpCap.SetStart(tmpCap.GetStart() + entity->GetPosition());
				tmpCap.SetEnd(tmpCap.GetEnd() +entity->GetPosition());
				tmpCap.SetRadius(tmpCap.GetRadius()*_fScale);
				CCollisionLib::IntersectRayCapsule(_d3dBeginPos, d3dDir, &tmpCap, fTmpScale, d3dVec);
			}
			else
			{
				CSphere tmpSph = *(CSphere*)(entity->GetBV());
				tmpSph.SetPosition(tmpSph.GetPosition() +entity->GetPosition());
				tmpSph.SetRadius(tmpSph.GetRadius()*_fScale);
				CCollisionLib::RayToSphere(_d3dBeginPos, d3dDir, tmpSph, fTmpScale, d3dVec);
			}

			//Make sure we picked the closest one
			if(fClosestScale > fTmpScale)
			{
				fClosestScale = fTmpScale;
				pTheOneWeWant = *iterActiveList1;
				((CEnemy*)pTheOneWeWant)->SetShouldRun(true);
			}
		}
	}

	// #if _DEBUG
	// 	if(pTheOneWeWant)
	// 	{
	// 		cout << "Missile Locked!" << endl;
	// 	}
	// #endif

	return pTheOneWeWant;
}

/*D3DXVECTOR3 CObjectManager::LaserCast( D3DXVECTOR3 _d3dBeginPos, D3DXVECTOR3 _d3dDirection, bool _bIsPlayer )
{

D3DXVECTOR3 pTheOneWeWant = D3DXVECTOR3(FLT_MAX, -FLT_MAX, FLT_MAX);

float fClosestScale = FLT_MAX, fTmpScale = FLT_MAX;
D3DXVECTOR3 d3dVec;

//Create the iterator
std::vector< CEntity* >::iterator iterActiveList1;
CEntity* pMasterListIter = nullptr;

if(_bIsPlayer)
{
for(iterActiveList1 = m_vMasterList.begin(); iterActiveList1 != m_vMasterList.end(); ++iterActiveList1)
{
pMasterListIter = (*iterActiveList1);
//There are only enemies and waypoints in this list
if(pMasterListIter->GetType() != eWAYPOINT && pMasterListIter->GetType() != eRED_PLAYER && pMasterListIter->GetType() != eBLUE_PLAYER 
&& pMasterListIter->GetType() != eMISSILE && pMasterListIter->GetType() != eLASER && pMasterListIter->GetBV() != 0 )
{

if(pMasterListIter->GetBV()->GetObjType() == eCAP)
{
CCapsule tmpCap = *(CCapsule*)(pMasterListIter->GetBV());
tmpCap.SetStart(tmpCap.GetStart() + pMasterListIter->GetPosition());
tmpCap.SetEnd(tmpCap.GetEnd() + pMasterListIter->GetPosition());
tmpCap.SetRadius(tmpCap.GetRadius()*1.35f);
CCollisionLib::IntersectRayCapsule(_d3dBeginPos, _d3dDirection, &tmpCap, fTmpScale, d3dVec);
}
else
{
CSphere tmpSph = *(CSphere*)(pMasterListIter->GetBV());
tmpSph.SetPosition(tmpSph.GetPosition() + pMasterListIter->GetPosition());
tmpSph.SetRadius(tmpSph.GetRadius()*1.35f);
CCollisionLib::RayToSphere(_d3dBeginPos, _d3dDirection, tmpSph, fTmpScale, d3dVec);
}

//Make sure we picked the closest one
if(fClosestScale > fTmpScale)
{
fClosestScale = fTmpScale;
pTheOneWeWant = d3dVec;
}
}
}
}

else
{
pMasterListIter = nullptr;

for(iterActiveList1 = m_vMasterList.begin(); iterActiveList1 != m_vMasterList.end(); ++iterActiveList1)
{
pMasterListIter = (*iterActiveList1);
//There are only enemies and waypoints in this list
if(pMasterListIter->GetType() != eWAYPOINT && pMasterListIter->GetType() != eRED_ENEMY && pMasterListIter->GetType() != eBLUE_ENEMY 
&& pMasterListIter->GetType() != eYELLOW_ENEMY && pMasterListIter->GetType() != eMISSILE && pMasterListIter->GetType() != eLASER && pMasterListIter->GetBV() != 0 )
{

if(pMasterListIter->GetBV()->GetObjType() == eCAP)
{
CCapsule tmpCap = *(CCapsule*)(pMasterListIter->GetBV());
tmpCap.SetStart(tmpCap.GetStart() + pMasterListIter->GetPosition());
tmpCap.SetEnd(tmpCap.GetEnd() + pMasterListIter->GetPosition());
tmpCap.SetRadius(tmpCap.GetRadius()*3.00f);
CCollisionLib::IntersectRayCapsule(_d3dBeginPos, _d3dDirection, &tmpCap, fTmpScale, d3dVec);
}
else
{
CSphere tmpSph = *(CSphere*)(pMasterListIter->GetBV());
tmpSph.SetPosition(tmpSph.GetPosition() + pMasterListIter->GetPosition());
tmpSph.SetRadius(tmpSph.GetRadius()*3.00f);
CCollisionLib::RayToSphere(_d3dBeginPos, _d3dDirection, tmpSph, fTmpScale, d3dVec);
}

//Make sure we picked the closest one
if(fClosestScale > fTmpScale)
{
fClosestScale = fTmpScale;
pTheOneWeWant = d3dVec;
}
}
}
}



// #if _DEBUG
// 	if(pTheOneWeWant)
// 	{
// 		cout << "Missile Locked!" << endl;
// 	}
// #endif

return pTheOneWeWant;
}
*/
int CObjectManager::ComputeHashIndex( CEntity* _pObj )
{
	static __int64 h1 = 0x7D066DF640546095;	// Arbitrary, large primes.
	static __int64 h2 = 0x1f75104d551d68c6;	// Primes are popular for hash functions
	static __int64 h3 = 0x4CCCCCCCCCCCCCCC;	// for reducing the chance of hash collision.
	__int64 n = (h1 * (int)(_pObj->GetPosition().x / CELL_SIZE)) + (h2 * (int)(_pObj->GetPosition().y / CELL_SIZE)) + (h3 * (int)(_pObj->GetPosition().z / CELL_SIZE));
	int n2 = n % NUM_BUCKETS;	// Wrap indices to stay in bucket range
	if( n2 < 0 ) n2 += NUM_BUCKETS;	// Keep indices in positive range
	return n2;

}

void CObjectManager::BuildHashTable()
{
	unsigned nNumObjs = m_vMasterList.size();
	int nType = 0;
	for(unsigned unIndex = 0; unIndex < nNumObjs; unIndex++)
	{
		CEntity * currEntity =  m_vMasterList[unIndex];
		if (currEntity == nullptr)
		{
			continue;
		}
		nType = currEntity->GetType();

		if(nType == eBOSS || nType == eBARRIER)
		{
			continue;
		}

		int nIndex = ComputeHashIndex(currEntity);

		tNode* newNode = m_vUnusedNodes[0];
		m_vUnusedNodes.erase(m_vUnusedNodes.begin());

		// Bucket isn't empty
		if(m_vHashTable[nIndex])
		{
			newNode->pNext = m_vHashTable[nIndex];
		}
		else
		{
			newNode->pNext = 0;
		}

		m_vHashTable[nIndex] = newNode;

		newNode->pObj = currEntity;

	}
}

void CObjectManager::DismantleHashTable()
{
	for(int i=0; i<NUM_BUCKETS; i++)
	{
		tNode* tmpNode = m_vHashTable[i];

		while (tmpNode)
		{
			m_vUnusedNodes.push_back(tmpNode);
			tmpNode->pObj = 0;
			tmpNode = tmpNode->pNext;
		}

		m_vHashTable[i] = 0;
	}
}
