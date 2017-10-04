/***********************************************
* Filename:  		Enemy.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will hold all the enemy entities data.
************************************************/
#ifndef CENEMY_H
#define	CENEMY_H

#include "Entity.h"
#include "../AI/Behaviors.h"
#include "../AI/MovementBehavior.h"
#include "../AI/ShootingBehavior.h"

class CWaypoint;
class CObjectManager;
class CEnemy : public CEntity
{
protected:
	// -If health <= 0 the enemy is destroyed. 
	// -Health will only be lowered if shields are <=0
	int m_nHealth;

	//If shields <= 0 the ships health is vulnerable
	int m_nShields;

	// Damage done by enemy or modifier 
	int m_nDamage;

	// multiply this by the objects velocity
	float m_nVelocityModifier;

	// multiply this by the objects velocity
	float m_nTurnRate;

	bool m_bIsStunned;
	float m_fStunPower;

	bool justSpawned;
	float justSpawnedTimer;

	bool m_bShouldChasePlayer;

	// pointer to the playable player
	CEntity * m_pPlayer;

	//current behavior
	CMovementBehavior* m_pCurrMoveBehavior;
	// will hold the behavior of how this enemy will move
	vector<CMovementBehavior*> m_vpMoveBehaviors;

	// will hold the behavior of how this enemy will shoot
	CShootingBehavior* m_pShootBehavior;

	CWaypoint * m_pWaypoint;
	//pointer to the object manager for collision avoidance
	CObjectManager* m_pObjectManager;
public:
	/*****************************************************************
	* CEnemy():		Default Constructor will pass down to its respective chlid.
	* Ins:			    None    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CEnemy(void);
	/*Destructor*/
	virtual	~CEnemy(void);
	/*****************************************************************
	* PopBehavior():	Will remove and clean up the last behavior off of the stack.
	* Ins:			    None    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void PopBehavior();
	/*****************************************************************
	* PushBehavior():	Will add and set up a new behavior for the bottom of the stack.
	* Ins:			    _behavior    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void PushBehavior( CMovementBehavior *_pBehavior );
	/*****************************************************************
	* switchState():	Will create a new state of the passed in type and switch to it.
	* Ins:			    _behavior    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void switchState(CMovementBehavior *_pBehavior);
	/*****************************************************************
	* switchShootState():	Only used for the shooting behavior to switch and set up 
	*						how the enemy will shoot.
	* Ins:			    _behavior    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void switchShootState(CShootingBehavior *_pBehavior);
	/*****************************************************************
	* Update():		Will update behaviors. if any behavior stack is empty this 
	*					guy will add thinking to the stack.
	* Ins:			    fElapedTime    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Update(float _fElapsedTime);
	/*****************************************************************
	* Render():		draw effects. most likely will do nothing here.
	* Ins:			    None    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Render() ;

	/*Accessors*///////////////////////////////////////////////////////////////////
	int GetHealth() {return m_nHealth; }
	int GetShields() {return m_nShields; }
	int GetDamage() {return m_nDamage; }
	float GetTurnRate() {return m_nTurnRate; }
	CEntity* GetPlayer() {return m_pPlayer; }
	float GetVelocityModifier() {return m_nVelocityModifier; }
	CMovementBehavior* GetMovementBehavior() {return m_pCurrMoveBehavior; }
	CShootingBehavior* GetShootingBehavior() {return m_pShootBehavior; }
	vector<CMovementBehavior*> GetMovementBehaviorStack() {return m_vpMoveBehaviors; }
	bool GetIsStunned() {return m_bIsStunned; }
	float GetIsStunnedPower() {return m_fStunPower; }
	CWaypoint *GetWaypoint() {return m_pWaypoint;}
	CObjectManager* GetObjectManager() {return m_pObjectManager;}
	bool GetJustSpawned() {return justSpawned; }
	bool GetShouldChasePlayer() {return m_bShouldChasePlayer;}
	////////////////////////////////////////////////////////////////////////////////

	/*Mutators*////////////////////////////////////////////////////////////////////
	void SetShootingBehavior(CShootingBehavior* _pBehavior) { m_pShootBehavior = _pBehavior; }
	void SetMovementBehavior(CMovementBehavior* _pBehavior) { m_pCurrMoveBehavior = _pBehavior; }
	void SetVelocityModifier(float _fVelMod) { m_nVelocityModifier = _fVelMod; }
	void SetShields(int _nShields) { m_nShields = _nShields; }
	void SetDamage(int _nDamage) { m_nDamage = _nDamage; }
	void SetHealth(int _nHealth) { m_nHealth = _nHealth; }
	void SetTurnRate(float _fRate) { m_nTurnRate = _fRate; }
	void SetPlayer(CEntity* _pPlayer) { m_pPlayer = _pPlayer; }
	void SetIsStunned(bool _bFound, float _power) { m_bIsStunned = _bFound;
												m_fStunPower = _power;}	
	void SetObjectManager(CObjectManager* _pOM) {m_pObjectManager = _pOM;}
	void SetJustSpawned(bool _nShields) { justSpawned = _nShields; }
	void SetShouldChasePlayer(bool _shouldChase){m_bShouldChasePlayer = _shouldChase;}
	////////////////////////////////////////////////////////////////////////////////
};

#endif
