/***********************************************
 * Filename:  		Player.h
 * Date:      		11/27/2012
 * Mod. Date: 		11/29/2012
 * Mod. Initials:	AR
 * Author:    		Andrew A Rosser
 * Purpose:   		This class will hold all the player entities data.
 ************************************************/
#ifndef	CPLAYER_H
#define CPLAYER_H

#include "Entity.h"
#include "../Camera/Camera.h"
#include "../Particle System/EffectInformation.h"

class CPlayer : public CEntity
{
	// -If health <= 0 the player is destroyed. 
	// -Health will only be lowered if shields are <=0
	int m_nHealth;

	//If shields <= 0 the ships health is vulnerable
	int m_nShields;

	// Damage done by enemy or modifier 
	int m_nDamage;

	// multiply this by the players velocity
	float m_fVelocityModifier;

	// on if the player is alive
	bool m_bAlive, 
		m_bHealthStateChanged, 
		m_bShieldStateChanged;

	// on if the enemy is in stealth Mode
	//bool m_bIsVisable;

	// how much the shield will recharge over time
	float m_fShieldRechargeRate;

	// how much the health will recharge over time
	float m_fHealthRechargeRate;

	float m_fReturnFromInactiveBucket;

	CCollOBJ* m_pExtraBV;

	CCamera* m_pCamera;

	// Show Shield or not
	bool m_bShieldVisible;

	int m_nArrowTexture;
	// Shield Mesh IDs
	int m_nVisibleShieldMesh;
	int m_nInvisibleShieldMesh;

	// Default position Z position of the player
	float m_fForwardPosition;

	// Max forward position player can go
	float m_fMaxForwardPosition;

	// Max backward position player can go
	float m_fMaxBackPosition;

	// Float to store the yaw degree of the ship
	float m_fYawDegree;
	
	// Float to store the pitch degree of the ship
	float m_fPitchDegree;

	// Float to store the translation along the X-Axis
	float m_fTranslateX;
	
	// Float to store the translation along the X-Axis
	float m_fTranslateY;

	// Bools to determine if the player should return to its default position
	bool m_bYawLeft;
	bool m_bYawRight;
	bool m_bPitchUp;
	bool m_bPitchDown;
	bool m_bPosDegree;
	bool m_bNegDegree;

	// Buffer zone for mouse change
	float m_fBufferZone;
	float m_fYawReturnTime;
	float m_fDegreeReturnTime;
	float m_fPitchReturnTime;
	float m_fCameraLevel;
	// 1 for each side, itterating from 0 - TOP, 1 - BOT, 2 - LEFT, 3 - RIGHT
	float m_arrfHurtRecentTimer[4];
	int	  m_nDamageDirction;

	bool startMovingL;
	bool startMovingR;
	bool doOnceR;
	bool doOnceL;

	float m_fMaxXTranslation;
	float m_fMaxYTranslation;
	float m_fMaxDodgeTranslation;
	float m_fMaxYawDegree;
	// Char to check if player has dodged left/right
	char m_bHasDodged;
	bool m_bMoveFaster;

	bool m_bPlayShieldHit;
	bool m_bPlayHullHit;
	bool m_bDisableInput;
	bool m_bLoadingComplete;
	int m_nModelTriCollPoint;
	int m_nPlayerMass;
	
	float m_fModeChangeTimer;
	char m_cCruiseControl;

	D3DXVECTOR3 m_d3dNewTurnAroundVec;
	D3DXMATRIX m_d3dNewTurnAroundMat;

	CEffectSystem m_ESPlayerIdle;
	CEffectSystem m_ESPlayerModeChange;
	CEffectSystem m_ESPlayerThruster;
	CEffectSystem m_ESPlayerDamaged;
	CEffectSystem m_ESPlayerMildlyDamaged;
	CEffectSystem m_ESPlayerHeavilyDamaged;
	//CEffectSystem m_ESPlayerJetStreamers;
	CEffectSystem m_esPlayerDepart;

	bool m_bIsInactive;

	D3DXMATRIX	m_d3dDodgeDestination;
	D3DXMATRIX m_d3dLookAtMat;

	D3DXVECTOR3 m_vDestination;

	CCamera* m_pDodgeCamera;
	float m_fSpeed;
	int m_nTotalDistance;
	bool m_bMoveDodgeCam;
	bool m_bForceReturn;
	float m_fSubtractFromRotation;

	bool m_bShouldWarpOut;

	//////////////////////////////////////////////////////////////
	//variables for ai prompts based on health/shields
	bool m_bHasPlayedCriticalHealthSound;
	bool m_bHasPlayedLossOfShieldSound;
	bool m_bHasPlayedPlayerDeath;
	bool m_bHasPlayedSafetyEjection;

	float m_fHealthAlarmDelay;
	float m_fShieldAlarmDelay;

	bool m_bFinalCinematic;

public:
/*****************************************************************
 * CPlayer():		Default Constructor will set up his data members
 * Ins:			    None    		      
 * Outs:		    None
 * Returns:		    None
 * Mod. Date:		11/29/2012
 * Mod. Initials:	AR
 *****************************************************************/
	CPlayer();

/*****************************************************************
 * CPlayer():		overload Constructor will set up his data members
 * Ins:			    _health, _shields, _damage, _velMod  		      
 * Outs:		    None
 * Returns:		    None
 * Mod. Date:		11/29/2012
 * Mod. Initials:	AR
 *****************************************************************/
	CPlayer::CPlayer(int _nHealth, int _nShields, int _nDamage, float _fVelMod);
/*Destructor*/
	~CPlayer(void);
/*****************************************************************
 * Update():		Will update its respective data, shields, and health.
 * Ins:			    fElapedTime    		      
 * Outs:		    None
 * Returns:		    void
 * Mod. Date:		11/29/2012
 * Mod. Initials:	AR
 *****************************************************************/
	void Update(float _fElapsedTime);
/*****************************************************************
 * Render():		Draw effects or whatnot.
 * Ins:			    None    		      
 * Outs:		    None
 * Returns:		    void
 * Mod. Date:		11/29/2012
 * Mod. Initials:	AR
 *****************************************************************/
	void Render() ;

/*****************************************************************
 * YawReturn():		Function that detrmines when the yaw value of the 
 *					ship returns to the default position
 * Ins:			    _elapsedTime	      
 * Outs:		    None
 * Returns:		    void
 * Mod. Date:		12/11/2012
 * Mod. Initials:	AG
 *****************************************************************/
	void YawReturn(float _fElapsedTime);

/*****************************************************************
 * PitchReturn():	Function that detrmines when the pitch value of the 
 *					ship returns to the default position
 * Ins:			    _elapsedTime	      
 * Outs:		    None
 * Returns:		    void
 * Mod. Date:		12/11/2012
 * Mod. Initials:	AG
 *****************************************************************/
	void PitchReturn(float _fElapsedTime);

/*****************************************************************
 * AngleReturn():	Function that detrmines when the angle value of the 
 *					ship returns to the default position
 * Ins:			    _elapsedTime	      
 * Outs:		    None
 * Returns:		    void
 * Mod. Date:		12/11/2012
 * Mod. Initials:	AG
 *****************************************************************/
	void AngleReturn(float _fElapsedTime);

	int CalculateDamageDirection(D3DXVECTOR3 _HarmfulSubstance);

	void StopAllEffects();
	void GetDepartEffect();
	void ChangeModeEffect(bool ToBlue = true);
	
	void DodgeCamera(float _fDt);

	
	void SetDodgeDestination(D3DXMATRIX _d3dDestinationMat);

	//Accessors
	int GetHealth() {return m_nHealth;}
	// Pass in 1-4 for each side (top, bottom, left right) This function should take care of subscript problems
	float GetHurtRecently(int _side) {return (_side >= 0 && _side < 4)? m_arrfHurtRecentTimer[_side] : 1.0f;}
	int	GetDamageDirection() {return m_nDamageDirction;}
	int GetShields() {return m_nShields;}
	int GetDamage() {return m_nDamage; }
	float GetVelocityModifier() {return m_fVelocityModifier; }
	bool GetAlive() {return m_bAlive; }
	//%bool GetIsVisable() {return m_bIsVisable; }
	float GetHealthRechargeRate() {return m_fHealthRechargeRate; }	
	float GetShieldRechargeRate() {return m_fShieldRechargeRate; }
	virtual CCollOBJ* GetBV();
	int GetVisibleShieldMesh() const { return m_nVisibleShieldMesh; }
	int GetInvisibleShieldMesh() const { return m_nInvisibleShieldMesh; }
	CCamera* GetCamera() {return m_pCamera;}
	char GetHasDodged() {return m_bHasDodged;}
	bool GetMoveFaster() const { return m_bMoveFaster; }
	bool GetPlayShieldHit() const { return m_bPlayShieldHit; }
	bool GetPlayHullHit() const { return m_bPlayHullHit; }
	char GetPlayerSpeedMode() {return m_cCruiseControl;}
//	bool DisableInput() const { return m_bDisableInput; }
	int GetModelTriCollPoint() const { return m_nModelTriCollPoint; }
	int GetLoadingComplete() {return m_bLoadingComplete;}
	int GetPlayerMass() {return m_nPlayerMass;}
	bool GetForceReturn() {return m_bForceReturn;}
	bool GetIsFinalCinematic() { return m_bFinalCinematic;}

	//Mutators
	void SetHealth(int _nHealth, int DamageDirection = 0, CGame* _pGame = nullptr); // Pass in direction for 1 - Top, 2 - Bottom, 3 - Left, 4 - Right. Function should take care of subscript problems.
	void SetShields(int _nShields,int DamageDirection = 0, CGame* _pGame = nullptr);//modified so that if the sheilds were to be depleted from an outside source, it would still play the shield break sound. Also damage direction indicator
	void SetDamage(int _nDamage) { m_nDamage = _nDamage; }
	void SetVelocityModifier(float _fVelMod) { m_fVelocityModifier = _fVelMod; }
	void SetAlive(bool _bIsAlive) { m_bAlive = _bIsAlive; }
	//void SetIsVisable(bool _bIsVisable) { m_bIsVisable = _bIsVisable; }
	void SetHealthRechargeRate(float _fHRate) { m_fHealthRechargeRate = _fHRate; }
	void SetShieldRechargeRate(float _fSRate) { m_fShieldRechargeRate = _fSRate; }
	void SetShieldBV(CCollOBJ* _pBV) { m_pExtraBV = _pBV; }
	void SetVisibleShieldMesh(int _nVisibileMesh) { m_nVisibleShieldMesh = _nVisibileMesh; }
	void SetInvisibleShieldMesh(int _nInvisibleMesh) { m_nInvisibleShieldMesh = _nInvisibleMesh; }
	void SetCamera(CCamera* _pCamera) {m_pCamera = _pCamera;}
	void SetHasDodged(char _bHasDodged) {m_bHasDodged = _bHasDodged;}
	void SetMoveFaster(bool _bVal) { m_bMoveFaster = _bVal; }
	void SetPlayShieldHit(bool _bVal) { m_bPlayShieldHit = _bVal; }
	void SetPlayHullHit(bool _bVal) { m_bPlayHullHit = _bVal; }
	void SetPlayerSpeedMode(char _cVal) {m_cCruiseControl = _cVal;}
	void DisableInput(bool _bVal) { m_bDisableInput = _bVal; }
	void SetModelTriCollPoint(int _nWhichOne) { m_nModelTriCollPoint = _nWhichOne; }
	void SetLoadingComplete(bool _isComplete) {m_bLoadingComplete = _isComplete;}
	void SetPlayerMass(int _nMass) {m_nPlayerMass = _nMass;}
	void SetInactive(bool _bIsInactive);
	void SetForceReturn(bool _bForceReturn) {m_bForceReturn = _bForceReturn; m_fReturnFromInactiveBucket = 1.0f;}
	void SetIsFinalCinematic(bool _bFinalCinematic) {m_bFinalCinematic = _bFinalCinematic;}
	void SetDestination(D3DXVECTOR3 vec);
};

#endif