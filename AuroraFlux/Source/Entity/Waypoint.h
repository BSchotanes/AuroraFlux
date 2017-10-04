/***********************************************
 * Filename:  		CWaypoint.h
 * Date:      		12/01/2012
 * Mod. Date: 		12/02/2012
 * Mod. Initials:	AQ
 * Author:    		Andrew Goossens
 * Purpose:   		This class will hold the waypoint entities for the player to follow
 ************************************************/
#ifndef CWAYPOINT_H
#define CWAYPOINT_H

#include "Entity.h"
#include "../Collision/CollOBJ.h"
#include "../Particle System/EffectSystem.h"
#include "Enemy.h"

class CWaypoint : public CEnemy
{
	//unique id for waypoints to help with winning
	int m_nID;
	//on if the player is within the radius of the waypoint
	bool m_bTrigger;

	float fyDiff;
	float fScaleTimer;
	float fRotateChange;
	float fExplosionTimer;

	bool m_bScaleDown;
	bool m_bShouldScale;
	bool m_bShouldTrigger;
	bool m_bHasAlreadyTriggered;
	bool m_bParticlesExploded;

	CEffectSystem m_esTriggeredParticles;
public:
/*****************************************************************
 * CBluePlayer():		overloaded Constructor will set up his type and timers
 * Ins:			    _Pos    		      
 * Outs:		    None
 * Returns:		    None
 * Mod. Date:		12/2/2012
 * Mod. Initials:	AQ
 *****************************************************************/
	CWaypoint();

/*****************************************************************
 * ~CWaypoint():	Will set all variables to a null value upon leaving the class
 * Ins:			    none    		      
 * Outs:		    None
 * Returns:		    void
 * Mod. Date:		11/29/2012
 * Mod. Initials:	AG
 *****************************************************************/
	~CWaypoint(void);

/*****************************************************************
 * Update():		Will pass down the update calls to its children.
 * Ins:			    fElapedTime    		      
 * Outs:		    None
 * Returns:		    void
 * Mod. Date:		12/01/2012
 * Mod. Initials:	AG
 *****************************************************************/
	void Update(float _fElapedTime);

/*****************************************************************
 * Render():		Will pass down the draw calls to its children.
 * Ins:			    fElapedTime    		      
 * Outs:		    None
 * Returns:		    void
 * Mod. Date:		12/01/2012
 * Mod. Initials:	AG
 *****************************************************************/
	void Render();

	int GetWaypointID() const { return m_nID; }
	void SetWaypointID(int _nVal) { m_nID = _nVal; }

	bool IsTriggered() const { return m_bTrigger; }
	void SetTrigger(bool _nVal);

	bool GetShouldScale() {return m_bShouldScale;}
	void SetShouldScale(bool _shouldScale) {m_bShouldScale = _shouldScale;}

	bool GetScaleDown() {return m_bScaleDown;}
	void SetScaleDown(bool _scaleDown) {m_bScaleDown = _scaleDown;}

	void SetIsVisible(bool _bIsVisible);

	void SetShouldTrigger(bool _bShouldTrigger) {m_bShouldTrigger = _bShouldTrigger;}

	void ResetTriggerParticle();
};

#endif

