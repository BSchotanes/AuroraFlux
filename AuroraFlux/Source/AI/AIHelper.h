/***********************************************
* Filename:  		CAIhelper.h
	* Date:      		11/27/2012
	* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This is the base class that the Movement and
*					shooting behaviors will inherit off of.
************************************************/
#ifndef	CAIHELPER_H
#define CAIHELPER_H

class CAIHelper
{
	// Enemy Stats	
int m_nHealth;
int redShield;
int blueShield;
int yellowShield;

// Enemy Weapon Stats
int laserDamage;
int missileDamage;
int yellowDamage;

// Enemy Velocity Modifiers
float  yellowVelocityModifier;
float  yellowChaseVelocityModifier;
float  blueVelocityModifier;
float  blueEvadeVelocityModifier;
float  redVelocityModifier;
float  redEvadeVelocityModifier;
float  laserVelocityModifier;
float  missileVelocityModifier;

// Enemy Turn Rates
float	  redTurnRate;
float	  redBehindTurnRate;
float	  blueTurnRate;
float	  blueEvadeTurnRate;
float	  yellowTurnRate;
float	  yellowChaseTurnRate;
float	  missileTurnRate;
float	  missileLowerTurnRate;
float	  laserTurnRate;

//AI behavior
float	red_to_flee_distance;
float	blue_to_flee_distance;
float	red_to_chase_distance;
float	blue_to_chase_distance;
float	force_red_to_chase_distance;
float	force_red_to_flee_distance;
float	yellow_stealth_chase_radius;
float	yellow_not_stealth_chase_radius;

float yellow_waypoint_range ; //distance till the yellow enemy will switch to its next waypoint
float wandering_waypoint_range ;//distance till anyone wandering will switch to its next waypoint

float blue_enemy_missile_fire_timer; //time the blue enemy will wait to shoot a missile
float red_enemy_fire_rate ; // will fire a laser every x seconds where x is its value

float evade_timer ; // how long an enemy will flee if they have been hit
float blue_evade_timer ; // how long an enemy will flee if they have been hit
float force_chase_timer; // how long a partner will chase if an enemy is hit


public:

	/*****************************************************************
	* CAIHelper():     Overload Constructor will set its type. The enemy passed in 
		*					will be the enemy this behavior is attached to.
	* Ins:			    _pEnemy    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CAIHelper();
	// Destructor
	~CAIHelper(void);
	/*****************************************************************
	* Update():		Will pass down the updates to its children. 
		* Ins:			    fElapedTime    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Update(float _fElapedTime);

	bool ReadFile(LPCTSTR _szFilePath);
	bool SaveFile(LPCTSTR _szFilePath);

	int GetHealth(){return m_nHealth;}
	int GetredShield(){return redShield;}
	int GetblueShield(){return blueShield;}
	int GetyellowShield(){return yellowShield;}
	int GetlaserDamage(){return laserDamage;}
	int GetmissileDamage(){return missileDamage;}
	int GetyellowDamage(){return yellowDamage;}

	float GetyellowVelocityModifier (){return yellowVelocityModifier;}
	float GetyellowChaseVelocityModifier (){return yellowChaseVelocityModifier;}
	float GetblueVelocityModifier (){return blueVelocityModifier;}
	float GetblueEvadeVelocityModifier (){return blueEvadeVelocityModifier ;}
	float GetredVelocityModifier (){return redVelocityModifier;}
	float GetredEvadeVelocityModifier (){return redEvadeVelocityModifier;}
	float GetlaserVelocityModifier (){return laserVelocityModifier;}
	float GetmissileVelocityModifier (){return missileVelocityModifier;}

	float GetredTurnRate (){return redTurnRate;}
	float GetredBehindTurnRate (){return redBehindTurnRate ;}
	float GetblueTurnRate (){return blueTurnRate;}
	float GetblueEvadeTurnRate (){return blueEvadeTurnRate;}
	float GetyellowTurnRate (){return yellowTurnRate;}
	float GetyellowChaseTurnRate (){return yellowChaseTurnRate;}
	float GetmissileTurnRate (){return missileTurnRate;}
	float GetmissileLowerTurnRate (){return missileLowerTurnRate;}
	float GetlaserTurnRate (){return laserTurnRate;}
	
	float Getred_to_flee_distance (){return red_to_flee_distance;}
	float Getblue_to_flee_distance (){return blue_to_flee_distance;}
	float Getred_to_chase_distance (){return red_to_chase_distance;}
	float Getblue_to_chase_distance (){return blue_to_chase_distance;}
	float Getforce_red_to_chase_distance (){return force_red_to_chase_distance;}
	float Getforce_red_to_flee_distance (){return force_red_to_flee_distance;}
	float Getyellow_stealth_chase_radius (){return yellow_stealth_chase_radius;}
	float Getyellow_not_stealth_chase_radius (){return yellow_not_stealth_chase_radius;}

	float Getyellow_waypoint_range (){return yellow_waypoint_range;}
	float Getwandering_waypoint_range (){return wandering_waypoint_range;}
	float Getblue_enemy_missile_fire_timer (){return blue_enemy_missile_fire_timer;}
	float Getred_enemy_fire_rate (){return red_enemy_fire_rate;}
	float Getevade_timer (){return evade_timer;}
	float Getblue_evade_timer (){return blue_evade_timer;}
	float Getforce_chase_timer (){return force_chase_timer;}


};

#endif