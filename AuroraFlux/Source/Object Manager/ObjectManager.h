/***********************************************
* Filename:  		ObjectManager.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012 
* Mod. Initials:	
* Author:    		Stephen warren
* Purpose:   		This class will handle all objects, calling their update,
*					render, collision, and the passing of objects back and forth
*					to the master list.
************************************************/

#ifndef COBJECTMANAGER_H
#define COBJECTMANAGER_H

#include "../Entity/Entity.h"
#include "../Camera/Camera.h"
#include "../AI/AIHelper.h"
#include "../Particle System/EffectSystem.h"

class CRenderer;
class CEnemy;
class CAmbientBoundingBox;
class CAmbientFighter;

class CObjectManager
{

public:
	void RegenerateMisc(D3DXVECTOR3 pos);

	/*****************************************************************
	* CObjectManager(): Overload Constructor will take in a pointer to the reference. 
	* Ins:			    _pRenderer    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	CObjectManager(CRenderer* _pRenderer, CCamera* _pCamera, CAIHelper *_AIHelper);

	/*****************************************************************
	* ~CObjectManager():     Destructor to take care of all the . 
	* Ins:					 None    		      
	* Outs:				 	 None
	* Returns:				 None
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	~CObjectManager(void);
	void CObjectManager::RenderMainMenuObjects();
	void CObjectManager::UpdateMainMenuObjects( float _fDeltaTime );

	/*****************************************************************
	* UpdateObjects():     Updates all the objects in the master list 
	* Ins:			       float     		      
	* Outs:			 	   None
	* Returns:		       None
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	void UpdateObjects( float _fDeltaTime, float _fOtherDelta = -123 );

	/*****************************************************************
	* RenderObjects():     Renders the objects by passing them to the Renderer
	* Ins:			       None    		      
	* Outs:			 	   None
	* Returns:		       None
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	void RenderObjects();

	/*****************************************************************
	* AddObject():     Adds an object to the master list. Add only if needed. 
	*				   Otherwise use the ReturnTo function. 
	* Ins:			   CEntity*    		      
	* Outs:			   None
	* Returns:		   None
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	void AddObject( CEntity* _pEntity );

	/*****************************************************************
	* RemoveObject():  Removes an object from the master list.
	*				   Should only be called if the entity is no longer needed 
	* Ins:			   CEntity*    		      
	* Outs:			   None
	* Returns:		   None
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	void RemoveObject( CEntity* _pEntity );

	/*****************************************************************
	* RemoveAllObjects():     Removes all objects from the master list.
	*						  Should only be called if the entities are no longer needed 
	* Ins:			   None    		      
	* Outs:			   None
	* Returns:		   None
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	void RemoveAllObjects();

	/*****************************************************************
	* CheckCollision():     Passes all objects that need to be checked to the CollisionLib.
	* Ins:			   None    		      
	* Outs:			   None
	* Returns:		   None
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	void CheckCollision(bool _bBossCin = false, CGame* _pGame = nullptr);

	/*****************************************************************
	* AddToInactiveAsteroid():     Adds an inactive asteroid to the asteroid list.
	* Ins:			   CEntity*    		      
	* Outs:			   None
	* Returns:		   None
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	void AddToInactiveAsteroid( CEntity* _pEntity );

	/*****************************************************************
	* AddToInactiveEnemy():     Adds an inactive enemy to the enemy list.
	* Ins:			   CEntity*    		      
	* Outs:			   None
	* Returns:		   None
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	void AddToInactiveEnemy( CEntity* _pEntity );

	/*****************************************************************
	* AddToInactiveArmament():     Adds an inactive missile or laser 
	*							   to the armament list.
	* Ins:			   CEntity*    		      
	* Outs:			   None
	* Returns:		   None
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	void AddToInactiveArmament( CEntity* _pEntity );

	/*****************************************************************
	* ReturnAsteroidToMasterList():     Returns an inactive asteroid to 
	*									the master list.
	* Ins:			   None    		      
	* Outs:			   None
	* Returns:		   None
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	CEntity* ReturnAsteroidToMasterList( float radius );

	/*****************************************************************
	* ReturnEnemyToMasterList():     Returns an inactive Enemy to 
	*								 the master list.
	* Ins:			   GameObjectTypes _eType    		      
	* Outs:			   None
	* Returns:		   None
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	CEntity* ReturnEnemyToMasterList( GameObjectTypes _eType);

	/*****************************************************************
	* ReturnArmamentToMasterList():     Returns an inactive Laser or Missile
	*								 to the master list.
	* Ins:			   GameObjectTypes _eType    		      
	* Outs:			   None
	* Returns:		   None
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	CEntity* ReturnArmamentToMasterList(  GameObjectTypes _eType );

	/*****************************************************************
	* Shutdown():     Removes and deletes all objects
	* Ins:			   None    		      
	* Outs:			   None
	* Returns:		   None
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	void Shutdown();

	/*****************************************************************
	* EnemyLockOnTarget():     Locks on the Target being pointed at
	* Ins:			   D3DXVECTOR3 _pos, D3DXVECTOR3 _dir    		      
	* Outs:			   Pointer to the Target
	* Returns:		   None
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	CEntity* EnemyLockOnTarget(D3DXVECTOR3 _d3dPos, D3DXVECTOR3 _d3dDir, float _fScale, float _time);

	/*****************************************************************
	* QuickSort():     Sorts the renderable list before sending it to the 
	*				   Renderer.
	* Ins:			   vector<CEntity *> &, int, int   		      
	* Outs:			   None
	* Returns:		   None
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	void QuickSort(vector<CEntity *> & _vpRenderableObjects, int _nLeft, int _nRight);

	/*****************************************************************
	* GetIndicatorList():     Returns the list of enemies to be pointed at.				
	* Ins:			   None   		      
	* Outs:			   None
	* Returns:		   std::vector<CEntity*>*
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	std::vector<CEntity*>* GetIndicatorList() {return &m_vIndicatorList; }

	void RemoveLasers();
	/*****************************************************************
	* AdjustToAvoidCollision()
	* Ins:			    Object you want to check other objects against, and a vector of integer for each object type to check collision against    		      
	* Outs:				vec3 for which direction the object should move to avoid collision
	* Returns:		    void
	* Mod. Date:		2/1/2013
	* Mod. Initials:	DO
	*****************************************************************/
	D3DXVECTOR3 AdjustToAvoidCollision(CEnemy* _pObj, std::vector<int> _vObstacleFlags);
	std::vector<CEntity*> *GetMissileList() {return &m_vActiveMissileList;}

	std::vector<CEntity*>* GetActiveEntityList() {return &m_vMasterList;}
	void SetShield(CEntity * _pShield) { m_pPlayerShield = _pShield; }

	void SetBossFight(bool  _boss) { m_bBossFight = _boss; }
	bool GetBossFight() { return m_bBossFight; }

	void SetInCinematic(bool  _nInCinematic) { m_bInCinematic = _nInCinematic;}
	bool GetInCinematic() { return m_bInCinematic;}

	int GetNumYellowEnemies() {return m_nNumYellowEnemies;}
	void SetNumYellowEnemies(int _numEnemies) {m_nNumYellowEnemies = _numEnemies;}

	CEntity* GetReticuleTarget(D3DXVECTOR3 _d3dBeginPos, D3DXVECTOR3 _d3dEndPos, float _fScale, bool _bBossTargetable = false);

	D3DXVECTOR3 LaserCast(D3DXVECTOR3 _d3dBeginPos, D3DXVECTOR3 _d3dDirection, bool _bIsPlayer);

	void AddEffect(CEffectSystem* _Effect);

	void SetRemoveMiscItems(bool _b) {m_bRemoveMiscItems = _b;}
	bool GetREmoveMiscItems() {return m_bRemoveMiscItems;}

	void AddAmbientFight(D3DXMATRIX _worldMat, int nFighterCount, float _fHeight, float _fDepth, float _fWidth);
	void AddIntroAmbientFighters(CAmbientBoundingBox* _abb);
	void RemoveAmbientFighters();

	void ToggleToShowAmbientDogFights() {m_bShowDogFights = !m_bShowDogFights;}

	bool GetPlayerLose() {return m_bPlayerLose;}
	void SetPlayerLose(bool _bPlayerLose) {m_bPlayerLose = _bPlayerLose;}
	/*****************************************************************
	* SpawnEntity():   Spawns an entity if it needs to
	* Ins:			   CEntity*    		      
	* Outs:			   None
	* Returns:		   None
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	void SpawnEntity(CEntity* _entity, float _fDeltaTime);
private:

	//*******************************************
	// ObjectManager Copy Constructor
	// ObjectManager Assignment Operator
	//*******************************************

	CObjectManager(const CObjectManager &);
	CObjectManager& operator=(const CObjectManager& );

	

	/*****************************************************************
	* MakeRenderableList():  Perform Frustum Culling
	* Ins:			   None    		      
	* Outs:			   None
	* Returns:		   None
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	void MakeRenderableList();

	struct tNode
	{
		CEntity* pObj;
		tNode* pNext;
	};

	tNode* m_vHashTable[NUM_BUCKETS];

	int ComputeHashIndex(CEntity* _pObj);
	void BuildHashTable();
	void DismantleHashTable();

	unsigned numObjsEver;

	std::vector<tNode*> m_vUnusedNodes;

	//Vectors to hold all entities
	std::vector<CEntity *> m_vMasterList;
	std::vector<CEntity *> m_vTransparentEnemyList;
	std::vector<CEntity *> m_vInactiveAsteroidList;
	std::vector<CEntity *> m_vInactiveEnemyList;
	std::vector<CEntity *> m_vInactiveArmamentList;
	std::vector<CEntity *> m_vRenderList;
	std::vector<CEntity *> m_vPendingToAddtoMasterList;
	std::vector<CEntity *> m_vIndicatorList;
	std::vector<CEntity *> m_vCatchAllList;
	std::vector<CEntity *> m_vActiveMissileList;

	std::vector<CEntity *> m_vMiscList;

	std::vector<CEntity *> m_vPendingList;

	std::vector<CEffectSystem*> m_vAllEffects;

	std::vector<CAmbientBoundingBox*> m_vAmbientDogFights;
	bool m_bShowDogFights;

	bool m_bRemoveMiscItems;

	CEntity* m_pPreviousTarget;
	float m_fTargetFleeCounter;

	bool m_bIterating;
	CRenderer* m_pRenderer;

	CCamera* m_pCamera;
	CEntity* m_pPlayerShield;
	CAIHelper *s_AIHelper;

	bool m_bBossFight;

	int m_nNumYellowEnemies;

	CEntity* m_pBoss;
	CEntity* m_pIterPlayer;

	bool m_bInCinematic;
	bool m_bPlayerLose;
};

#endif
