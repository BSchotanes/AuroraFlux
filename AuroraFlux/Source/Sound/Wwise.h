/*******************************************************
 * Filename:	Wwise.h
 * Created:		04/08/2011
 * Purpose:		Contains variables for the Wwise API
 *				setup.
 *******************************************************/
#ifndef WWISE_H_
#define WWISE_H_

#include "../StdAfx.h"

#if _DEBUG
#define AK_ENABLE_PROFILING 1 // will setup Wwise with the information needed to link with the profiler, if false will use the optimized version of wwise
#else
#define AK_ENABLE_PROFILING 0 // will setup Wwise with the information needed to link with the profiler, if false will use the optimized version of wwise
#endif

#define DEFAULT_SOUNDBANK_PATH L"Resources/Soundbanks/"

#include "Wwise_IDs.h"
#include "../Entity/Enemy.h"
#include "../Entity/Player.h"

// This is the Wwise namespace that is high level code for programmers to use in their code.
enum eFunc{ePlay = 0, eStop, ePause, eResume, eNumFuncs};
enum eSounds{
	/* Menu Sound enums */ 
	eBGM_MainMenuMusic = 0, eBGM_CreditsMusic, eBGM_LevelOneMusic, eBGM_LevelTwoMusic, 
	eBGM_BattleMusic, eBGM_BossMusic, eMenuItemSwitch, eMenuItemConfirm, eMenuItemCancel, eMenuItemPlayGame,
	
	/* Player Sound enums */
	ePlayerIdle, ePlayerThrust, ePlayerDodge, eHyperSpace, eModeChange, ePlayerLaserFire, ePlayerMissileFire,
	ePlayerShieldHit, eSheildRecharge, eSheildFull, eHullRepair, eMissileTravel, eMissileExplosion, eMissileReload,
	eMissileLockOn, ePlayerHullHit, ePlayerShieldBroken, ePlayerLowShields, eLockOnWarning, ePlayerLowHealthWarning, 
	ePlayerHealthFull, ePlayerDeath,

	/* Enemy Sound enums */
	eEnemyThrust, eEnemyExplosion, eEnemyLaserFire, eEnemyMissileFire, eLaserHitMetal, eLaserHitRock, eLaserHitSheild,
	eHydraLaser, eHydraLaserCharge, eHydraPeaShooter, eHydraShieldGenDown, eHydraSheildHit, eHydraEntrance, eHydraTimerOut, 
	eHydraPlanetDestroyed, eHydraThrusters,
	 
	/*Dialog*/
	eAIMoveMouse, eAIFireWeapon, eAISpeedUpSlowDown, eAIGreatJob, eAIDodgeRoll, eAIDodgeRollCooldown, eAISwitchModes, eAIModeWeapon, 
	eAIBlueModeRegenerate, eAIQuickTurn, eAIQuickTurnReposition, eAILookAnEnemy, eAISeekerEntrance, eAICruiserEntrance, eAIMarauderEntrance,
	eAIHydraEntrance, eAIHydraShieldEntrance, eAIHydraTurretEntrance, eAIHydraMegaLaserEntrance, eAILongVictory, eAIQuickVictory, eAITimerRanOut,
	eAIPlayerCriticalHealth, eAIPlayerShieldsDown, eAIWaveComplete, eAIIncommingWave,

	/*Extra sounds*/
	eOnYourSix, eCheckpoint, eGoodJob, eCountDown, eAsteroidExplosion, eOutOfBounds, eProceedToWaypoint,

};

namespace WwiseNS
{
	// Accessors
	int InitSoundEngine();
	void TermSoundEngine();
	void RemoveAllSoundObjs();

	bool Update();

	void SoundEvent( int _SoundEvent, int _DoWhat, bool PlayerSound = true, CEntity* _pEntity = nullptr);
	
	// Modifiers
	void SetPreventPlaying(bool _Prevent);
	bool GetPreventPlaying();
	//void SetPlayerPos(float fX, float fY, float fZ);
	//void SetPlayerPos(D3DXVECTOR3 _SetPos);
	void SetPlayerPos(D3DXMATRIX _SetMat);
	void SetListenerPos(D3DXMATRIX _SetMat);

	void SetEnemyPos(CEntity* _Enemy);
	void SetBossPos(D3DXMATRIX _SetMat);
	void SetBossPos(D3DXVECTOR3 _SetMat);

	void SetVolumeRTPC(AkRtpcID setID, float fVol);
	int FindObject(CEntity* _Entity);
	int FindMissile(CEntity* _Missile);
	void RemoveObject(CEntity* _Entity);
	void RemoveMissile(CEntity* _Missile);
	void StopAllTutorialVoices(int _enumOfSoundToSkip);
};
#endif