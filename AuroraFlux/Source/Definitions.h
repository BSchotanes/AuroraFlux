/***********************************************
* Filename:  		Definitions.h
* Date:      		11/20/2012
* Mod. Date: 		11/30/2012
* Mod. Initials:	AQ
* Author:    		Andrew T. Goosens
* Purpose:   		This is a basic .h file that 
*					holds all basic and global definitions
*					such as ENUMs, typedefs and #defines
************************************************/
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "StdAfx.h"
#include "Particle System\EffectSystemStructures.h"
#include "Renderer\ExporterIncludes.h"

#define EPSILON 3.4526698307252027193509668742125e-04F

#define WINDOW_WIDTH_720p		1280.0f
#define WINDOW_HEIGHT_720p		720.0f

#define WINDOW_WIDTH_1080p		1920.0f
#define WINDOW_HEIGHT_1080p		1080.0f

//Camera info
#define FIELD_OF_VIEW   D3DXToRadian(70)
#define ASPECT_RATIO   g_fWINDOW_WIDTH * 1.0f / g_fWINDOW_HEIGHT
#define CAM_NEAR 1.0f
#define CAM_FAR 160000.0f

enum GameObjectTypes{eRED_ENEMY = 0, eBLUE_ENEMY, eYELLOW_ENEMY, eRED_PLAYER, eBLUE_PLAYER, eYELLOW_PLAYER, eMISSILE, eLASER, eASTEROID, eWAYPOINT, eSHIELD, eSHIELD_QUAD, eBOSS, eGENERATOR, eHanger ,eTURRET_SMALL,eTURRET_LASER,
	eTURRET_MISSILE, eLARGE_ASTEROID,eBOSS_SHIELD, ePLANET1, ePLANET3, ePLANET4, eSPACE_STATION, eSPACE_DEBRIS, eSPACE_DEBRIS2, eSPACE_DEBRIS3, eBARRIER, eSPACESTATION1,ePLANET2, eSATELITE ,eDEAD_PLAYER , eAMBIENT_ENEMY_LARGE,eBOSS_LASER,/*add new iterms here*/ eMAX_SIZE, 
	eINACTIVE_PLAYER, eCAM_OBJ, eAMBIENT_ENEMY, eDIBRIS_OBJ, eBOSSBIGLAZER, eBOSSTHRUSTER};

enum eAsteroidTypes{eASTEROID_FIRE = eASTEROID,eASTEROID_GRASS = eMAX_SIZE, eASTEROID_DIRT, eASTEROID_MAX_SIZE };

static const char* ObjectNames[] = {"RED_ENEMY", "BLUE_ENEMY", "YELLOW_ENEMY", "RED_PLAYER", "BLUE_PLAYER",
	"YELLOW_PLAYER", "MISSILE", "LASER", "ASTEROID", "BOSS", "WAYPOINT"};
enum CollObjType {eSPH, eAABB, eCAP, eFSTM, eNUM_OBJ_TYPES};

enum MATRIX_AXIS{eXAXIS, eYAXIS, eZAXIS, ePOS};

enum Flags{ eFIRE_LASER = 1, eFIRE_MISSILE = 2 , eLOCKED_ON = 4, eADD_RED_ENEMY = 8, 
	eADD_BLUE_ENEMY = 16, eADD_YELLOW_ENEMY = 32, eAVOIDANCE = 64, eFIRE_BOSSLAZER = 128};

enum FrustumCorners{ eFTL = 0, eFBL, eFBR, eFTR, eNTL, eNTR, eNBR, eNBL };
enum FrustumPlanes{ eNEAR_PLANE = 0, eFAR_PLANE, eLEFT_PLANE, eRIGHT_PLANE, eTOP_PLANE, eBOTTOM_PLANE };

enum GameStates{eMAIN_MENU, eGAME_PLAY_RESTART, eNEW_GAME_PLAY, eGAME_PLAY_OPTION, eOPTIONS_APPLY, eOPTIONS_DEFAULT, eOPTIONS_CANCEL, eCREDITS, ePAUSE, eGAME_OVER, eCONFIRMATION, eWIN, eEXIT_GAME, eOPTIONS_RESOLUTION_CHANGE};

enum PHASES{eLASER_PHASE = 1, eMISSILE_PHASE, eSEEK_AND_DESTROY, eCINEMATIC_PHASE};
enum SHIELDS{eSHIELDS_UP= 130, eSHIELDS_DOWN};

enum Hangers{ eRED_HANGER = 200, eBLUE_HANGER, eYELLOW_HANGER};

enum Effects{ eASTEROID_COLLISION_ASTEROID, eASTEROID_COLLISION_BLUE_ENEMY, eASTEROID_COLLISION_LASER, eASTEROID_COLLISION_MISSILE, eASTEROID_COLLISION_PLAYER, 
	eASTEROID_COLLISION_RED_ENEMY, eASTEROID_DEATH, eASTEROID_TAIL, eBLUE_ENEMY_DAMAGED, eBLUE_ENEMY_DEATH, eBLUE_ENEMY_FIRE_MISSILE, eBLUE_ENEMY_THRUSTER,
	eLASER_HULL, eLASER_SHIELD, eLASER_SPARK, eMISSILE_TRAIL, eMISSILE_HULL, eMISSILE_SHIELD, ePLAYER_CHANGE_BLUE, ePLAYER_CHANGE_RED, ePLAYER_COLLISION_BLUE_ENEMY, ePLAYER_COLLISION_RED_ENEMY,
	ePLAYER_DAMAGED, ePLAYER_DEATH, ePLAYER_THRUSTERS, ePLAYER_IDLE, ePLAYER_JET_STREAMERS, eRED_ENEMY_DAMAGED, eRED_ENEMY_DEATH, eRED_ENEMY_FIRE_LASER, eRED_ENEMY_THRUSTER, 
	eRED_ENEMY_ARRIVAL, eSPACE_DUST, ePLAYER_MILDLY_DAMAGED, ePLAYER_HEAVILY_DAMAGED, eBOSS_EXPLOSION, eBOSS_WARPIN, ePLAYER_THRUSTER_WARP, eWIN_LASER_CHARGE, eGENERATORS_CONNECTION, eGENERIC_EXPLOSION,
	eBOSS_WIN_LASER_PARTICLES, eBOSS_THRUSTER, ePLANET_EXPLOSION, ePLANET_SEMI_EXPLOSION, eBOSS_BIG_EXPLOSION, eWARP_DUST, eMAX_EFFECTS };

enum Menus{eNONE = -1, eBUTTON_MM_PLAY, eBUTTON_MM_OPTIONS, eBUTTON_MM_CREDITS, eBUTTON_MM_EXIT, eBUTTON_PS_RESUME, eBUTTON_PS_OPTIONS, eBUTTON_PS_RESTART, eBUTTON_PS_MAINMENU,
	eBUTTON_GO_RETRY, eBUTTON_GO_RESTART, eBUTTON_GO_MAINMENU, eBUTTON_CF_YES, eBUTTON_CF_NO, eBUTTON_OP_MUSIC, eBUTTON_OP_SFX, eBUTTON_OP_GAMMA, eBUTTON_OP_MOUSEYSENSITIVITY, 
	eBUTTON_OP_MOUSEPSENSITIVITY, eBUTTON_OP_INVERT, eBUTTON_OP_BACK, eBUTTON_OP_SLIDER, eBUTTON_OP_BALL, eBUTTON_OP_BLOCK, eBUTTON_OP_CHECKED, eNUM_BUTTONS_MAX };

static string EffectNames[] = { "Asteroid_Collision_Asteroid.txt", "Asteroid_Collision_Blue_Enemy.txt", "Asteroid_Collision_Laser.txt", "Asteroid_Collision_Missile.txt", 
	"Asteroid_Collision_Player.txt", "Asteroid_Collision_Red_Enemy.txt", "Asteroid_Death.txt", "Asteroid_Tail.txt", "Blue_Enemy_Damaged.txt", "Blue_Enemy_Death.txt", "Blue_Enemy_Fire_Missile.txt", 
	"Blue_Enemy_Thruster.txt", "Laser_Hull.txt", "Laser_Shield.txt", "Laser_Spark.txt", "Missile_Trail.txt", "Missile_Hull.txt", "Missile_Shield.txt", "Player_Change_Blue.txt", "Player_Change_Red.txt", 
	"Player_Collision_Blue_Enemy.txt", "Player_Collision_Red_Enemy.txt","Player_Damaged.txt" , "Player_Death.txt", "Player_Thrusters.txt", "Player_Idle_Thrusters.txt", "Player_Jet_Streamers.txt", 
	"Red_Enemy_Damaged.txt" , "Red_Enemy_Death.txt", "Red_Enemy_Fire_Laser.txt", "Red_Enemy_Thruster.txt", "Red_Enemy_Arrival.txt", "Space_Dust.txt", "Player_Mildly_Damaged.txt", "Player_Heavily_Damaged.txt", 
	"Boss_Explosion.txt", "Boss_WarpIn.txt", "Player_Thruster_Warp.txt", "Win_Laser_Charge.txt", "Generators_Connection.txt", "Generic_Explosion.txt", "Boss_Win_Laser.txt", "Boss_Thruster.txt", "Planet_Explosion.txt",
	"Planet_Semi_Explosion", "Boss_Big_Explosion.txt", "Warp_Dust.txt"}; 

enum ItroCinematicStages {eINTRO_AVOID_ASTEROIDS, eINTRO_THROUGH_METEORS, eINTRO_PAST_FIGHTERS, eINTRO_MOVE_TO_PLAYER};

// Player Stats
#define PLAYER_HEALTH 100
#define RED_STATE_SHIELD 100
#define BLUE_STATE_SHIELD (RED_STATE_SHIELD * 2)
#define YELLOW_STATE_SHIELD 0
#define YELLOW_STATE_HEALTH (PLAYER_HEALTH / 2)

#define RED_STATE_MAX_VELOCITY 120
#define RED_STATE_MIN_VELOCITY 15

#define BLUE_STATE_MAX_VELOCITY 50
#define BLUE_STATE_MIN_VELOCITY 5

#define YELLOW_STATE_MAX_VELOCITY 40
#define YELLOW_STATE_MIN_VELOCITY 0

#define PLAYER_SLOWED_VELOCITY 40

// Player Weapon Stats
#define PLAYER_MISSILE_DAMAGE 80 // shoot 3 so 90 damage total
#define PLAYER_LASER_DAMAGE 16

#define MISSILE_LIFETIME 2.0f
#define LASER_LIFETIME 1.85f

#define BLUE_CHASE_RANGE 720

#define BLUE_ACCELERATION 14.0f
#define RED_ACCELERATION 30.0f


#define FADE_IN 2
#define FADE_OUT 1
#define DONT_FADE 0

#define BOSS_DEBRIS_COUNT 20

///backburner 
#define YELLOW_ENEMY_DAMAGE 0 // IT'S OVER 9000!!!!!!!........no it's not....it's 0. Go home. You're drunk.
#define YELLOW_VELOCITY_MODIFIER 10.20f
#define YELLOW_CHASE_VELOCITY_MODIFIER 90.0f
#define YELLOW_TURN_RATE .016f
#define YELLOW_CHASE_TURN_RATE 0.038f 
#define YELLOW_STEALTH_CHASE_RADIUS 40.0f//distance yellow enemy will chase when target is stealthed
#define YELLOW_NOT_STEALTH_CHASE_RADIUS 120.0f//distance yellow enemy will chase when target is NOT stealthed
#define YELLOW_WAYPOINT_RANGE 20.0f //distance till the yellow enemy will switch to its next waypoint
#define WANDERING_WAYPOINT_RANGE 30.0f//distance till anyone wandering will switch to its next waypoint

#define BOSS_TURRET_RANGE 1800
#define BLUE_ENEMY_LASER_RATE 3.25f
#define RETURN_RANGE 2100.0f

#define TEXT_Y_POSITION 480
#define SKIP_TEXT_Y_POSITION 640
//Boss Values 
#define TURRET_TURN_RATE 0.18f
#define TURRET_FIRE_RATE_LASER 0.250f // lower is faster
#define TURRET_FIRE_RATE_MISSILE 6.0f
#define TURRET_SIGHT 0.305f

#define TURRE_LARGE_TURN_RATE 0.148f
#define TURRE_LARGE_FIRE_RATE_LASER .75f // lower is faster
#define TURRE_LARGE_SIGHT 0.305f

#define TURRET_WIN_TURN_RATE 0.013f
#define TURRET_WIN_FIRE_RATE_LASER 3.00f // lower is faster
#define TURRET_WIN_SIGHT 0.001f

// Dodge
#define DODGE_RATE .1f
#define DODGE_ROTATION_RATE 2.6f
#define DODGE_LEFT 1 
#define	DODGE_RIGHT 2
// Waypoint Radius
#define WAYPOINT_RADIUS 750.0f

//Camera Values
#define CAMERA_PITCH_SPEED 0.15f
#define CAMERA_YAW_SPEED 0.15f
#define CAMERA_ROLL_SPEED 50.0f

// Player Movement Scale Vars
#define CRUISE_UP 1
#define CRUISE_DOWN -1
#define CRUISE_NONE 0

#define PLAYER_MASS 2800
#define RED_ENEMY_MASS 2500
#define BLUE_ENEMY_MASS 3200
#define SMALL_ASTEROID_MASS 4000
#define LARGE_ASTEROID_MASS 8000
#define BOSS_MASS 125000

#define GENERATOR_TEXT_Y 690
#define GENERATOR_BAR_Y 235
#define PHASE_TEXT_Y 660

// Options menu
#define DEFAULT_VOLUME 25.0f
#define DEFAULT_SFX_VOLUME 25.0f
#define DEFAULT_GAMMA 1.0f

//HUD Rects 
static RECT s_rHUD = {0, 0, 1680, 1050};

//Pause state
static RECT s_rResumeHighlightedRect   = {342, 265, 706, 361};
static RECT s_rOptionsHighlighedRect   = {342, 361, 706, 462};
static RECT s_rMainMenuHighlightedRect = {342, 462, 706, 558};
static RECT s_rExitHighlightedRect     = {364, 558, 682, 646};

//Main Menu state
static RECT s_rPlayButtonRect    = {372, 410, 675, 493};
static RECT s_rOptionsButtonRect = {372, 493, 678, 574};
static RECT s_rCreditsButtonRect = {372, 574, 675, 656};
static RECT s_rExitButtonRect    = {393, 656, 660, 731};

// Confirmation State
static RECT s_rYesButtonRect = {361, 313, 687, 405};
static RECT s_rNoButtonRect = { 361, 405, 687, 502};

// Win State
static RECT s_rWinStateMMHighlightedRect = {353, 378, 695, 474};

// Game Over State
static RECT s_rGOContinueButtonRect = {349, 298, 698, 381};
static RECT s_rGOMainMenuRect =	{349, 381, 698, 469};
static RECT s_rGOExitButtonRect = {349, 469, 698, 566};

// Options
static RECT s_rOPMusicRect = {406, 322, 544, 498};
static RECT s_rOPSFXRect = {544, 692, 706, 472};
static RECT s_rOPGammaRect = {670, 330, 825, 499};
static RECT s_rOPYawRect = {670, 330, 825, 499};
static RECT s_rOPPitchRect = {670, 330, 825, 499};
static RECT s_rOPInvertRect = {670, 330, 825, 499};
static RECT s_rOPBackRect = {670, 330, 825, 499};


// HASH TABLE #DEFINES
#define NUM_BUCKETS 511
#define NUM_NODES 499
#define CELL_SIZE 200.0f



#endif
