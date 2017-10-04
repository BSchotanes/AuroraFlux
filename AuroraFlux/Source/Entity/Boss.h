/***********************************************
* Filename:  		Boss.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This is what will contain the 
*					boss enemys personal data.				
************************************************/
#ifndef	CBOSS_H
#define CBOSS_H

#include "IEntity.h"
#include "Enemy.h"
#include "../Entity/BossHanger.h"
#include "../Entity/BossLargerTurret.h"
#include "../Entity/BossWinTurret.h"
#include "../Entity/BossTurret.h"
#include "../Entity/Asteroid.h"
#include "../Renderer/CRenderer.h"
#include "../Entity/EnvironmentObjects.h"
#include "../Particle System/EffectSystem.h"

class CPlayer;
class CObjectManager;
class CGenerator;
class CBossThruster;
//leaving blank until Alpha, none of these are set in stone
class CBoss : public CEnemy
{
protected:
	bool teachPhase1;
	bool teachPhase2;
	bool teachPhase3;
	bool DisplayteachPhase1;
	bool DisplayteachPhase2;
	bool DisplayteachPhase3;
	bool m_bPlayerLose;
	bool m_bPlayedBigExplosion;
	// which current phase the boss is in
	int m_nPhase;
	int m_nShieldStatus;
	vector<CEnvironmentObject *> m_vpCollisionHelper;
	bool m_bMaxHealth;
	int m_nMaxHealth;
	int numAttachments;
	bool m_bIsDisabled;
	float transitionTimer;
	float transitionTimer2;
	//time until he can fire more missiles
	float m_fmissileTimer;
	// on when the boss can fire missiles
	bool m_bCanFire;
	// how long the yellow driods will be spawned for
	float m_fSeekTimer;
	// number of shield generator to decide which phase to use
	int m_nNumGenerators;
	int m_nNumHangers;
	int m_nNumTurrets;
	CObjectManager * m_pObjectManager;
	bool showHanger;	
	CEffectSystem m_esEntranceWarp;
	bool showTurret;
	CGenerator * m_pGenerators[3];
	int m_fHealthCounter;
	//CBossTurret* m_pBossTurrets[26];
	//CBossLargerTurret* m_pBossLargeTurrets[14];
	CBossThruster* m_pBossThruster;
	int xPoScrolls;
	vector<CBossTurret*> m_vpBossTurrets;
	vector<CBossLargerTurret*> m_vpBossLargeTurrets;
	vector<CEffectSystem> m_vCinematicParticles;
	CCollOBJ* m_pShieldBV;
	vector<CEnvironmentObject*> m_pSpaceDibris;

	bool m_bTransitionPhaseText1To2;
	bool m_bTransitionPhaseText2To3;
	float m_fPhaseTransitionTimer;
	CBossWinTurret* m_pBossWinTurret;
	CRenderer * m_pRenderer;
	D3DCOLOR m_BossColor;
	bool startBoss;
	D3DCOLOR m_TimerColor;
	//Shields health bar variables
	RECT m_rGen1Health; 
	D3DXMATRIX	m_Gen1Mat;
	RECT m_rGen2Health; 
	D3DXMATRIX	m_Gen2Mat;
	RECT m_rGen3Health; 
	D3DXMATRIX	m_Gen3Mat;
	int alpha;
	int BossHealthTexture;
	RECT m_rBossHealth; 
	D3DXMATRIX	m_HealthMat;

	int GeneratorTexture;
	int TurretTexture;
	int BigTurretTexture;
	RECT m_rAttachmentRect; 
	D3DXMATRIX	m_AttachmentMat;

	CEffectSystem m_esBigExplosion;

	float m_fDeathWaitTime;
	float m_fScaleTransition;
	D3DXVECTOR3 m_fTranslationTransition;
	D3DXVECTOR3 m_fShadowTranslationTransition;

	float m_fBattleTimer;

	int generatorHealth;

	CEntity * m_pAsteroidTarget;
	int m_nNumMissilesFired;

	D3DXVECTOR3 m_d3dmDestination;
	float m_fZScale,
		  m_fZOriginal,
		  m_fEntranceDelay;

	bool m_bReachedZScale;

	void CinematicFunction(float _fDt);
	bool m_bDrawWarning;
	float m_fDeathTimer;
	bool m_bShowTeachBoss;
	bool m_bTeachBoss;
	int xPos;
	int BlackTexture;
	RECT m_rBlack; 
	D3DXMATRIX	m_BlackMat;
	bool whichFade;
	int m_fSkipTextAlpha;
	int alphaTeach;
	float m_fTimerLowScale;
	POINT m_tTimerPosition;
	float m_fseconds;
	CEnvironmentObject * m_pPlanet;
	CEnvironmentObject * m_pLight1;
	CEnvironmentObject * m_pLight2;
	CEnvironmentObject * m_pLight3;
	CEnvironmentObject * m_pLight4;
	bool m_bNewPhase;
	bool m_bFirst;
	CEffectSystem m_ESBossThruster;

	bool m_bIsOnTop;
	bool m_bIsOnLeft;
	bool m_bIsOnRight;
	bool m_bHasPlayedAIVictorySpeech;
	bool m_bHasPlayedAIHydranceEntrance;
	bool m_bHasPlayedAIHydraShieldEntrance;
	bool m_bHasPlayedAIHydraTurretEntrance;
	bool m_bHasPlayedAIHydraMegaLaserEntrance;

	bool m_bBigExplosion;

	bool m_bFireLaser;
	bool m_bStopFiringLaser;

	bool m_bWarped;
	int m_nLastBigExplosionSound;
public:
	void	SetAsteroidTarget(CEntity * _asteroid){m_pAsteroidTarget = _asteroid ;}
	void DrawWarning(bool _warn){ m_bDrawWarning = _warn;}
		
	D3DXVECTOR3 GetDestinationPos() {return m_d3dmDestination;}
	vector<CEnvironmentObject *> GetCollisionHelp() {return m_vpCollisionHelper;}
	void FireAtAsteroid(float _eTime);
	void CreateCollisionSpheres();
	/*****************************************************************
	* CBoss():  Default Constructor. Will set its type and behaviors.
	* Ins:			    None    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CBoss(CPlayer * _pPlayer,CObjectManager * _objectManager , CRenderer * _pRenderer, D3DXVECTOR3 _pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnvironmentObject * _pPlanet = 0, CEnvironmentObject* _pLight1 = 0, CEnvironmentObject* _pLight2 = 0, CEnvironmentObject* _pLight3 = 0, CEnvironmentObject* _pLight4 = 0);
	/*****************************************************************
	* ~CBoss(): Destructor, probably doing nothing
	* Ins:			    None    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	~CBoss(void);
	/*****************************************************************
	* Update():		Will update the boss internal timers and update the behaviors 
	*					based on the elapsed time.
	* Ins:			    fElapedTime    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Update(float _fElapedTime);
	/*****************************************************************
	* Render():		Draws the boss effects.				
	* Ins:			    None    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Render();
	void MoveAttachments();
	void FixMode(float _time);
	void CheckWin(float _time);
	void DrawFeedback(float _time, bool _bCinematic = false);
	void SetNumGenerators( int _num ){ m_nNumGenerators = _num; }
	int GetNumGenerators( void ) { return m_nNumGenerators; }

	float GetDeathWaitTimer() { return m_fDeathWaitTime; }

	CEntity* GetPlanet() { return m_pPlanet;}

	int GetPhase( void ) { return m_nPhase; }
	CObjectManager * GetOM(){return m_pObjectManager;}

	bool GetIsDisabled(){return m_bIsDisabled;}
	int GetShieldStatus(){return m_nShieldStatus;}

	bool GetPlayerLose(){ return m_bPlayerLose;}
	void SetPlayerLose(bool _lose) {m_bPlayerLose = _lose;}
	
	bool GetTeachBoss(){ return m_bTeachBoss;}
	void SetTeachBoss(bool _lose) {m_bTeachBoss = _lose;}

	bool GetShowTeachBoss(){ return m_bShowTeachBoss;}
	void SetShowTeachBoss(bool _lose) {m_bShowTeachBoss = _lose;}

	void SetDestinationPosition(float _fX, float _fY, float _fZ);
	void SetInCinematic(bool _inCinematic);
	void DrawIntro(int _nTexture);

	void AddEffectFromCollision(CEntity* _pOwner);
	void CheckPlayerSide();
	float GetBattleTimer() {return m_fBattleTimer;}

	bool GetNewPhase() {return m_bNewPhase;}
	void SetNewPhase(bool _new) {m_bNewPhase = _new;}
	virtual CCollOBJ* GetBV();
	vector<CBossTurret*> GetTurrets() {return m_vpBossTurrets;}
	vector<CBossLargerTurret*> GetLargeTurrets() {return m_vpBossLargeTurrets;}
	CGenerator* GetAGenerator() {return m_pGenerators[0];}
	CBossWinTurret* GetWinTurret() {return m_pBossWinTurret;}

	bool GetTransition() {return m_bTransitionPhaseText2To3;}

	void SetTeachPhase1(bool _new) {teachPhase1 = _new;}
	void SetTeachPhase2(bool _new) {teachPhase2 = _new;}
	void SetTeachPhase3(bool _new) {teachPhase3 = _new;}
	bool GetTeachPhase1( ) { return teachPhase1;}
	bool GetTeachPhase2( ) { return teachPhase2;}
	bool GetTeachPhase3( ) { return teachPhase3;}

	bool IsOnLeft(){ return m_bIsOnLeft;}
	bool IsOnRight(){ return m_bIsOnRight;}
	bool IsOnTop(){ return m_bIsOnTop;}

	bool GetFireLaser() {return m_bFireLaser;}
	void SetFireLaser(bool _nFireLaser) {m_bFireLaser = m_bFireLaser;}

	bool GetStopFireLaser() {return m_bStopFiringLaser;}
	void SetStopFireLaser(bool _nStopFireLaser) {m_bStopFiringLaser = _nStopFireLaser;}
	
};

#endif