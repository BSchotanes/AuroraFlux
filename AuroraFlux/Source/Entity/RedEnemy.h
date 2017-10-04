/***********************************************
 * Filename:  		RedEnemy.h
 * Date:      		11/27/2012
 * Mod. Date: 		11/29/2012
 * Mod. Initials:	AR
 * Author:    		Andrew A Rosser
 * Purpose:   		This is what will contain the 
 *					Red enemys personal data.				
 ************************************************/
#ifndef CREDENEMY_H
#define	CREDENEMY_H

#include "Enemy.h"
#include "../AI/AIHelper.h"
#include "../Particle System/EffectInformation.h"

class CRedEnemy : public CEnemy
{
	// How long until the enemy can dodge again
	float m_fDodgeTimer;
	bool isRolling;
	float m_fRollTimer;

	float m_fRollAnimationTimer;

	// on if the enemy can dodge
	bool m_bCanDodge;

	bool m_bCanRoll;

	bool m_bShouldDodge;

	//Vector of enemies to flock with
	vector<CEntity*> m_pPartners;

	// how many partners he thought he had
	int m_nNumpartners;

	// what flock this guy belongs to
	int m_nFlockID;

	// timer to help flocking
	int m_nFlockCounter;

	CAIHelper *s_AIHelper;

	CEffectSystem m_esEngineTrail;
	CEffectSystem m_esDamage;
	CEffectSystem m_esArrival;
	
public:
/*****************************************************************
 * CRedEnemy():  Default Constructor. Will set its type and behaviors.
 * Ins:			    None    		      
 * Outs:		    None
 * Returns:		    None
 * Mod. Date:		11/29/2012
 * Mod. Initials:	AR
 *****************************************************************/
	CRedEnemy(void);
 /*****************************************************************
 * CRedEnemy():  Overload Constructor. Will set its type, target, and behaviors. 
 *					Sets the target parameter to its target member.
 * Ins:			    _target    		      
 * Outs:		    None
 * Returns:		    None
 * Mod. Date:		11/29/2012
 * Mod. Initials:	AR
 *****************************************************************/
	CRedEnemy(CEntity* _pTarget ,CAIHelper *_AIHelper ,CWaypoint * _pWaypoint, CObjectManager* _pObjManager);
/*****************************************************************
 * ~CRedEnemy(): Destructor, probably doing nothing
 * Ins:			    None    		      
 * Outs:		    None
 * Returns:		    None
 * Mod. Date:		11/29/2012
 * Mod. Initials:	AR
 *****************************************************************/
	~CRedEnemy(void);
/*****************************************************************
 * Update():		Will update the enemys internal timers and update the behaviors 
 *					based on the elapsed time.
 * Ins:			    fElapedTime    		      
 * Outs:		    None
 * Returns:		    void
 * Mod. Date:		11/29/2012
 * Mod. Initials:	AR
 *****************************************************************/
	void Update(float _fElapedTime);
/*****************************************************************
 * Render():		Draws the enemies.				
 * Ins:			    None    		      
 * Outs:		    None
 * Returns:		    void
 * Mod. Date:		11/29/2012
 * Mod. Initials:	AR
 *****************************************************************/
	void Render();
/*****************************************************************
 * AddPartner():		Adds a enemy to influence his current flock			
 * Ins:			    _add    		      
 * Outs:		    None
 * Returns:		    void
 * Mod. Date:		11/29/2012
 * Mod. Initials:	AR
 *****************************************************************/
	void AddPartner(CEntity* _pAdd);
	/*****************************************************************
 * RemoveAllPartners():		removes all partners from his current flock			
 * Ins:			    None    		      
 * Outs:		    None
 * Returns:		    void
 * Mod. Date:		11/29/2012
 * Mod. Initials:	AR
 *****************************************************************/
	void RemoveAllPartners();

/*****************************************************************
 * isPartner():		returns true if the paassed in entity is already in his partner list			
 * Ins:			    _red    		      
 * Outs:		    None
 * Returns:		    bool
 * Mod. Date:		11/29/2012
 * Mod. Initials:	AR
 *****************************************************************/
	bool isPartner(CRedEnemy * _pRed);

	void CreateArrivalEffect();
	void CreateDeathEffect();

	//TODO://REMOVE THIS FUNCTION WHEN AI IS DONE
	void UpdateHelperAI(float _elapsedTime);
/***************
 *  Accessors
 ***************/
 bool GetIsRolling(){return isRolling;}
	bool GetShouldDodge(){return m_bShouldDodge;}
	bool GetCanRoll(){return m_bCanRoll;}
	float GetMissileTimer() {return m_fDodgeTimer; }
	bool GetCanFire() {return m_bCanDodge; }
	int GetFlockID() {return m_nFlockID; }
	vector<CEntity*> GetPartners() {return m_pPartners; }
	CEffectSystem* GetTrailEffect() {return &m_esEngineTrail; }
	CEffectSystem* GetDamagedEffect() { return &m_esDamage; }
/***************
 * Mutators
 ***************/
 void SetIsRolling(bool _bDodge) { isRolling = _bDodge; }
	void SetCanFire(bool _bDodge) { m_bCanDodge = _bDodge; }
	void SetCanRoll(bool _bDodge) { m_bCanRoll = _bDodge; }
	void SetRollTimer(float _time) {m_fRollTimer = _time ;}
	void SetShouldDodge(bool _dodge){m_bShouldDodge = _dodge;}

	void SetFlockID(int _nId) {m_nFlockID = _nId;}
	void SetMissileTimer(float _fTimer) { m_fDodgeTimer = _fTimer; }
	void SetPartners(vector<CEntity*> _vpPartner) { m_pPartners = _vpPartner; }
	CAIHelper * GetHelper(){return s_AIHelper;}
};

#endif