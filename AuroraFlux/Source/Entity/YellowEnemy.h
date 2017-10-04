/***********************************************
 * Filename:  		YellowEnemy.h
 * Date:      		11/27/2012
 * Mod. Date: 		11/29/2012
 * Mod. Initials:	AR
 * Author:    		Andrew A Rosser
 * Purpose:   		This is what will contain the 
 *					yellow enemys personal data.				
 ************************************************/
#ifndef	CYELLOWENEMY_H
#define CYELLOWENEMY_H

#include "Enemy.h"
#include "../AI/AIHelper.h"
#include "../Particle System/EffectInformation.h"
class CObjectManager;

class CYellowEnemy : public CEnemy
{
	// Pathing waypoints. Yellow enemy will follow them in order 0-x then x-0
	// this may go to behaviors class for how he moves
	vector<CEntity*> m_vpWaypoints;
	//pointer to the player
	CEntity		*m_pThePlayer;

	unsigned int m_unWaypointIndex;
	// radius of sphere till player gets caught
	float m_fSightRadius;
	// on if the player has been spotted
	bool m_bFoundPlayer;

		CAIHelper *s_AIHelper;

		bool needAnArrow;
		CEffectSystem m_esEngineTrail;
		CEffectSystem m_esDamage;
		CEffectSystem m_esArrival;
public:
/*****************************************************************
 * CYellowEnemy():  Default Constructor. Will set its type and behaviors.
 * Ins:			    None    		      
 * Outs:		    None
 * Returns:		    None
 * Mod. Date:		11/29/2012
 * Mod. Initials:	AR
 *****************************************************************/
	CYellowEnemy(void);
 /*****************************************************************
 * CYellowEnemy():  Overload Constructor. Will set its type, target, and behaviors. 
 *					Sets the target parameter to its target member.
 * Ins:			    _target    		      
 * Outs:		    None
 * Returns:		    None
 * Mod. Date:		11/29/2012
 * Mod. Initials:	AR
 *****************************************************************/
	CYellowEnemy(CEntity* _pTarget,CAIHelper *_AIHelper ,CWaypoint * _pWaypoint, CObjectManager* _pObjectManager);
/*****************************************************************
 * ~CYellowEnemy(): Destructor, probably doing nothing
 * Ins:			    None    		      
 * Outs:		    None
 * Returns:		    None
 * Mod. Date:		11/29/2012
 * Mod. Initials:	AR
 *****************************************************************/
	~CYellowEnemy(void);
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

	void CreateArrivalEffect();
/***************
 *  Accessors
 ***************/
	float GetRadius() {return m_fSightRadius; }
	bool GetFound() {return m_bFoundPlayer; }
	bool GetNeedAnArrow() {return needAnArrow; }

	CEntity	* GetPlayer() {return m_pThePlayer; }
	vector<CEntity*> GetPath() {return m_vpWaypoints; }
/***************
 * Mutators
 ***************/


	CEffectSystem* GetTrailEffect() {return &m_esEngineTrail; }
	CEffectSystem* GetDamagedEffect() { return &m_esDamage; }
	void SetRadius(float _fRadius) { m_fSightRadius = _fRadius; }
	void SetNeedAnArrow(bool _fRadius) { needAnArrow = _fRadius; }
	void SetFound(bool _bFound) { m_bFoundPlayer = _bFound; }
	void SetPath(vector<CEntity*> _pWpt) {m_vpWaypoints = _pWpt; }
	void AddWaypoint(CEntity* _pWpt);
	CAIHelper * GetHelper(){return s_AIHelper;}

};

#endif
