/***********************************************
* Filename:  		ScriptManager.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	
* Author:    		Stephen warren
* Purpose:   		This class will handle the loading of all the enemies based
*					on the waypoints. The waypoints have multiple events which 
*					each spawn enemies based off the waypoint.
************************************************/

#ifndef CSCRIPTMANAGER_H
#define CSCRIPTMANAGER_H

#include "../Entity/Waypoint.h"
#include "../Object Manager/ObjectManager.h"
#include "../AI/AIHelper.h"
#include "../Entity/YellowEnemy.h"
#include "../Entity/BlueEnemy.h"
#include "../Entity/Boss.h"
#include "../Entity/Asteroid.h"
#include "../Renderer/CRenderer.h"
#include "../Dialog System/Dialog System.h"
#include "../Entity/EnvironmentObjects.h"
#include "../Camera/CameraObject.h"

class CGame;


struct EnemiesToCreate
{
	int m_cType;
	int m_nFlockId;
	D3DXVECTOR3 m_d3dPosition;
	vector<D3DXVECTOR3> m_vPaths;

	EnemiesToCreate() : m_cType(0), m_nFlockId(0), m_d3dPosition(0.0f, 0.0f, 0.0f)
	{}
};


struct Asteroid
{
	D3DXVECTOR3 m_d3dPosition;
	float radius;
};

struct Wave
{
	int m_cNextSpawn; //how many enemies left till we spawn more
	vector<EnemiesToCreate> m_vEnemiesToCreate;

	Wave() : m_cNextSpawn(0) 
	{}
};

struct Events
{
	vector<Wave> m_vWave;
	vector<Asteroid> m_vAsteroids;
	D3DXVECTOR3 m_d3dWaypointPos;

	Events() : m_d3dWaypointPos(0.0f, 0.0f, 0.0f)
	{}
};

class CScriptManager
{
	// Holds all of the events for the level
	vector<Events> m_vScript;

	// the actual waypoint in game to be triggered
	CWaypoint* m_pCurrWaypoint;
	
	// so we can create entities
	CObjectManager* m_pOBJManager;

	CCameraObject* m_pCinematicCamera;
	
	// which event we need to process
	unsigned int m_unCurrentEventIndex;
	
	// holds the current wave data
	vector<Wave>::iterator m_CurrWaveIter;
	
	// so we only read the initial waypoint data the first time
	bool m_bInitialEvent; 
	
	// a copy of the enemies we created
	vector<CEntity*> m_vpCurrentEnemies;

	vector<CEntity*> m_vpCurrentYellowEnemies;

	vector<CAsteroid*> m_pAsteroids;
	//Pointer to the player
	CEntity * m_pThePlayer;

	//Pointer to the game
	CGame * m_pGame;
	unsigned int lastWave;
	float m_fSpawnTimer;
	bool m_bSpawnBool;
	bool m_bSpawnBoss;
	bool doOnce;
	bool loadAsteroids;
	static bool m_bTutorial;
	bool m_bAddedWaypoint;
	float m_fTimer;
	CEnvironmentObject * planet; 
	bool m_bSpawnedEnemyWave;

	CAIHelper *s_AIHelper;
	CDialogSystem* m_pDialog;

	CBoss * pTheBoss ;

	CRenderer * m_pRenderer;

	void Tutorial( float _fDeltaTime);

	char m_cCurrentTutorial;
	bool m_bCompletedObjective;

	bool m_bSpawnYellowEnemies;
	CYellowEnemy* m_pTutorialEnemy;

	float m_GoodJobTimer;

	bool m_bHasPlayedMoveMouse;
	bool m_bHasPlayedSpeedUpSlowDown;
	bool m_bHasPlayedGreatJob;
	bool m_bHasPlayedFireWeapon;
	bool m_bHasPlayedDodgeRoll;
	bool m_bHasPlayedDodgeRollCooldown;
	bool m_bHasPlayedSwitchModes;
	bool m_bHasPlayedModeWeapon;
	bool m_bHasPlayedQuickTurn;
	bool m_bHasPlayedQuickTurnReposition;
	bool m_bHasPlayedLookAnEnemy;
	bool m_bHasCompletedDifficultInstruction;
	bool m_bHasPlayedDifficultInstruction;
	bool m_bHasPlayedChildMode;
	float m_fInitialInstructionBucket;
	float m_fChildModeBucket;

	CEnvironmentObject* m_pLight1;
	CEnvironmentObject* m_pLight2;
	CEnvironmentObject* m_pLight3;
	CEnvironmentObject* m_pLight4;
public:


	/*****************************************************************
	* CreateEnemies():   Creates Enemies determined by the current Script
	* Ins:			   None    		      
	* Outs:			   None
	* Returns:		   bool
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	void CreateEnemies();
	void BossDestroyAsteroids(float _elapsedTime);

	void CreateAsteroids();
	/*****************************************************************
	* CScriptManager(): Overload Constructor will take in the Object Manager.
	*					The player, and the game
	* Ins:			    CObjectManager*, CEntity *, CGame*   		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	CScriptManager(CObjectManager* _pOM, CEntity * _pPlayer, CGame* _pGame, CAIHelper *_AIHelper, CRenderer * _pRenderer, CDialogSystem* _pScript);

	/*****************************************************************
	* ~CScriptManager():     Destructor to take care of all the memory . 
	* Ins:					 None    		      
	* Outs:				 	 None
	* Returns:				 None
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	~CScriptManager(void);

	/*****************************************************************
	* LoadBinaryScript():   Loads a Binary File to Set the ScriptManager. 
	*						Returns true if loaded correctly
	* Ins:			   LPCTSTR    		      
	* Outs:			   None
	* Returns:		   bool
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	bool LoadBinaryScript(LPCTSTR _szFilePath);

	/*****************************************************************
	* LoadTextScript():   Loads a Text File to Set the ScriptManager. 
	*					  Returns true if loaded correctly
	* Ins:			   LPCTSTR    		      
	* Outs:			   None
	* Returns:		   bool
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	bool LoadTextScript(LPCTSTR _szFilePath);

	bool LoadTextScriptOld(LPCTSTR _szFilePath);

	/*****************************************************************
	* SaveInBinary():   Saves the ScriptManager's loaded Waypoint information. 
	*					Returns true if saved correctly in Binary
	* Ins:			   LPCTSTR    		      
	* Outs:			   None
	* Returns:		   bool
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	bool SaveInBinary(LPCTSTR _szFilePath);

	/*****************************************************************
	* SaveInText():   Saves the ScriptManager's loaded Waypoint information. 
	*				  Returns true if saved correctly in Text
	* Ins:			   LPCTSTR    		      
	* Outs:			   None
	* Returns:		   bool
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	bool SaveInText(LPCTSTR _szFilePath);
	
	/*****************************************************************
	* Update():   Updates the Script to the correct script, 
	*					 returns true if the game is over  
	* Ins:			   float _dt    		      
	* Outs:			   None
	* Returns:		   bool
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	bool Update(float _fDeltaTime);

	/*****************************************************************
	* LoadCheckpoint():   Sets the next Checkpoint to the correct one. 
	* Ins:			   None 		      
	* Outs:			   None
	* Returns:		   None
	* Mod. Date:		
	* Mod. Initials:	
	*****************************************************************/
	void LoadCheckpoint(  );

	vector<CEntity*> GetCurrentEnemies(){return m_vpCurrentEnemies;}
	vector<CEntity*> GetCurrentYellowEnemies(){return m_vpCurrentYellowEnemies;}
	CWaypoint* GetCurrentWaypoint() {return m_pCurrWaypoint;}
	bool GetSpawnBoss(){return m_bSpawnBoss;}
	float GetSpawnTimer() const { return m_fSpawnTimer; }
	void SetSpawnedEnemyBool(bool _b) {m_bSpawnedEnemyWave = _b;}
	bool GetSpawnedEnemyBool() {return m_bSpawnedEnemyWave;}
	bool GetSpawnYellowEnemies() {return m_bSpawnYellowEnemies;}
	bool GetIsInTutorial()	{return m_bTutorial;}
	bool GetAddedWaypoint() const { return m_bAddedWaypoint; }
	/*void AddedWaypoint(bool val) { m_bAddedWaypoint = val; }*/
	CBoss * GetTheBoss(){return pTheBoss;}
	void SetCinematicCamera(CCameraObject* _pObj) {m_pCinematicCamera = _pObj;}
};

#endif