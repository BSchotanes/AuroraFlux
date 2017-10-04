/***********************************************
* Filename:  		CGamePlayState.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AG
* Author:    		Andrew Goossens
* Purpose:   		This will display the game state for
*						the game. 
*					
************************************************/
#ifndef CGAMEPLAYSTATE_H
#define	CGAMEPLAYSTATE_H

#include "IGameState.h"
#include "../Entity/Enemy.h"
#include "../Entity/BlueEnemy.h"
#include "../Entity/Player.h"
#include "../Entity/SceneryManager.h"
#include "../Player State/RedPlayer.h"
#include "../Player State/BluePlayer.h"
#include "../Player State/YellowPlayer.h"
#include "../Player State/DeadPlayer.h"
#include "../Object Manager/ObjectManager.h"
#include "../Player State/PlayerBehavior.h"
#include "../Script Manager/ScriptManager.h"
#include "../Entity/Boss.h"
#include "../Entity/Asteroid.h"
#include "../Particle System/EffectSystem.h"
#include "../Player State/PlayerEnterWarp.h"
#include "../AI/AIHelper.h"
#include "../Player State/InactivePlayer.h"
#include "../Camera/CameraObject.h"

class CGame;
class GamePlayState : public IGameState
{
	CGame*				m_pGame;
	CPlayer*			m_pPlayer;
	CObjectManager *    m_pObjectManager;
	CRedPlayer*			m_pRedPlayer;
	CBluePlayer*	    m_pBluePlayer;
	CYellowPlayer*	    m_pYellowPlayer;
	CDeadPlayer			m_pDeadPlayer;
	CPlayerEnterWarp*	m_pWarpPlayer;
	CPlayerBehavior*    m_pPlayerBehavior;
	CPlayerBehavior*    m_pPrevPlayerBehavior;
	CCamera*			m_pCamera;
	CInactivePlayer*	m_pInactivePlayer;
	CCameraObject*		m_pCinematicCamera;
	CRenderer		   *m_pRenderer;
	D3DXVECTOR3			newZaxisDirection;
	D3DXVECTOR3			m_d3dEnemyEntranceMat;
	CEntity*			m_pnewTurnToMat;
	CEffectSystem		m_esSpaceDust;
	CEffectSystem		m_esWarpDust;
	CSceneryManager    * m_pSceneryManager;
	vector<CAsteroid*>	m_pAsteroidList;
	vector<CAsteroid*>	m_pBossAsteroidList;

	vector<CEnemy*>	m_createdEnemies;

	int				m_fTeachXLocation;
	CEffectSystem		m_pLargeExplosion;

	//int  m_nGreenArrowText;
	//int  m_nRedArrowText;
	//int  m_nBlueArrowText;
	//int  m_nYellowArrowText;
	RECT m_rArrowRect; 
	//int m_nHUDHealthIcon;
	//int m_nHUDShieldIcon;
	//int m_nBlueEnemyShieldIcon;
	bool m_bFlashHealth;
	int m_nFlashAlpha;
	bool whichWay;
	int m_nYellowLockOnTex;
	int m_nRedLockOnTex;

	//int m_nBlackTexture;
	int m_nRedTexture;

	RECT m_rHorizontalDamage;
	RECT m_rVerticalDamage;
	int m_nDamageDirectionVertical;
	int m_nDamageDirectionHorizontal;
	D3DXMATRIX m_d3dTopSideDamage;
	D3DXMATRIX m_d3dBotSideDamage;
	D3DXMATRIX m_d3dLeftSideDamage;
	D3DXMATRIX m_d3dRightSideDamage;
	D3DXMATRIX	m_ArrowMat;
	D3DXMATRIX	m_ArrowMat2;

	D3DXVECTOR3 m_d3dvPlayerWinPos;

	RECT m_rEnemyIconRect;
	D3DXMATRIX m_d3dEnemyIconPos;
	int m_nRedEnemyIconTex;
	int m_nBlueEnemyIconTex;
	int m_nYellowEnemyIconTex;
	
	int m_nLockOnWarningRedTex;
	int m_nLockOnWarningYellowTex;
	int m_nLockOnNoWarningGreenTex;
	RECT m_rWarningRect;
	D3DXMATRIX m_d3dWarningMat;

	float	m_fLockOnTimer,
			m_fMissileReloadTimer,
			m_fWinTimer,
			m_fCinematicTimer,
			m_fTimeToLockOn;
	
	bool m_bCompleted;// = false;
	float m_fCompletedBucket;// = 0.0f;
	
	bool m_bShowHealth;
	bool m_bShowShields;  
	bool gameJustStarted;
	bool m_bTooFar;
	bool m_bTurnAround;
	bool m_bShowWaypoint;
	bool m_bLaserSide;
	bool m_bEnemyBehind;
	bool m_bInCinematic;
	bool m_bSpawnYellowEnemy;

	float m_fFadeToWhiteAlpha;
	
	float			m_fBehindSound;
	CScriptManager* m_pScriptManager;
	float			startTimer;
	D3DXMATRIX		m_d3dArrow;
	CAIHelper*		s_AIHelper;
	bool			spawnBoss;

	int			m_nReticleTex;
	D3DXMATRIX	m_ReticleMat;
	D3DXMATRIX	m_TargetMat;

	CBlueEnemy*	tempBoss;
	bool bossFight;
	bool MakebossOnce;
	bool m_bInCombat;

	D3DXVECTOR3 m_d3dDodgeVector; 
	float xValue;
	bool m_bIsAlive;
	int m_nMissilesToFire;
	float m_fMissileDelay;
	bool bIsLooping;
	float m_fDistanceFromWaypoint;
	float m_fBossAsteroidSpawnTimer;
	bool m_bPlayCinematic;
	float m_fWaitForCinematicBucket;
	bool m_bShowRedData;
	bool m_bShowBlueData;
	bool m_bShowYellowData;
	bool m_bShowBossData; 
	int m_fSkipTextAlpha;

	bool m_bFirstTime;

	bool m_bHasDodgedMissile;
	bool m_bWithinRange;
	bool m_bLockedOn;
	float m_fMissileDistance;
	bool m_bChangeTurnAroundCam;
	bool m_bReturnToCamera;

	//int BlackTexture;
	RECT m_rBlack; 
	D3DXMATRIX	m_BlackMat;
	D3DXMATRIX	m_HealthBox;
	D3DXMATRIX	m_DistanceBox;
	D3DXMATRIX	m_EnemyBox;
	
	int m_nExplosionFade;
	bool m_bFlashed;
	float m_fExplosionFadeAlpha;

	D3DXVECTOR3 m_d3dPrevWaypointPosition;

	float				m_fHelpScreenAlpha;
	int					m_nHelpScreenTextureID;
	bool				m_bHelpScreenToggleOn;

	bool m_bRenderIndicatorBox;

	bool m_bPlaySoundCruiserEntrance;
	bool m_bPlaySoundMarauderEntrance;
	bool m_bPlaySoundSeekerEntrance;
	bool m_bPlaySoundIncommingWave;
	bool m_bPlaySoundWaveComplete;
	bool m_bPlaySoundHydraTimerRanOut;
	bool m_bPlaySoundCheckpointArrival;
	float m_fPlaySoundBucketBuffer;
	bool m_bSlowDodgeEnemy;

	bool m_bContinued;

	bool m_bNearExplodingMissiles;

	bool m_bNeedToProceed;
	float m_fProceedTimer;
public:
	/*****************************************************************
	* CCreditsState():  Default Constructor. Will set its type and behaviors.
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	GamePlayState(void);

	/*****************************************************************
	* CCreditsState():  Overloaded Constructor. Will be used to create new states.
	* Ins:			    _game    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	GamePlayState(CGame * _pGame){ m_pGame =  _pGame;}
	GamePlayState(CGame* _pGame, bool _Continued = false) {m_pGame = _pGame; m_bContinued = _Continued;} 

	/*****************************************************************
	* ~CCreditsState():  Virtual destructor used when shutting down the game state
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	virtual ~GamePlayState(void);

	/*****************************************************************
	* Enter(): Enters the game state. Will set any variables needed for the state
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Enter();
	/*****************************************************************
	* Exit(): Exits the game state. Will set any variables used to null values
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Exit();

	/*****************************************************************
	* Input(): Grabs any input made by the player
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    bool
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	bool Input();

	/*****************************************************************
	* Update(): Updates the game state based on the change in time
	* Ins:			    fElapsedTime    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Update(float _fElapsedTime);

	/*****************************************************************
	* Render(): Renders the game state by calling the renderer classes and object managers render functions
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Render();

	/*****************************************************************
	* EnemyLocationArrow(): Function that calculates the placement of the waypoint arrows
	* Ins:					none    		      
	* Outs:					None
	* Returns:				void
	* Mod. Date:			12/06/2012
	* Mod. Initials:		AG
	*****************************************************************/
	void EnemyLocationArrow();

	/*****************************************************************
	* EnemyLockOnIcon(): Function that calculates the placement of the lockon icon
	* Ins:					none    		      
	* Outs:					None
	* Returns:				void
	* Mod. Date:			1/12/2013
	* Mod. Initials:		AQ
	*****************************************************************/
	void EnemyLockOnIcon();

	/* ACCESSORS */
	CScriptManager* GetScriptManager() { return m_pScriptManager;}
	CObjectManager* GetObjectManager() { return m_pObjectManager;}

	void SetFirstTime(bool _firstTime) { m_bFirstTime = _firstTime;}

	void CreateAmbientFighters();

	void FirstEnter();
	void SecondEnter();
	void ThirdEnter();
	void FourthEnter();
};
#endif