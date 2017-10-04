/*******************************************************
* Filename:	Wwise.cpp
* Created:		04/08/2011
* Purpose:		Contains the functionality of the Wwise
*				API.
*******************************************************/
#include "Wwise.h"
#if AK_ENABLE_PROFILING
#pragma comment(lib,"CommunicationCentral.lib") 
#include <AK/Comm/AkCommunication.h>	// Communication between Wwise designer and the game
#else
#define AK_OPTIMIZED
#endif //AK_USE_PROFILER

// load the libraries required, this alternatively could be done in the project properties but having it defined in code makes it more portable and reusable
// Thread defines
#pragma comment(lib,"dxguid.lib")      //needed for 3d sounds
#pragma comment(lib,"Ws2_32.lib")      //wwise uses this for communication
#include <AK/SoundEngine/Common/AkSoundEngine.h>  // Sound Engine
#pragma comment(lib,"AkSoundEngine.lib")
#include <AK/MusicEngine/Common/AkMusicEngine.h>  // Music Engine
#pragma comment(lib,"AkMusicEngine.lib")
#include <AK/SoundEngine/Common/IAkStreamMgr.h>   // Streaming Manager
#pragma comment(lib,"AkStreamMgr.lib")
#include <AK/SoundEngine/Common/AkMemoryMgr.h>   // Memory Manager
#pragma comment(lib,"AkMemoryMgr.lib")
#include <AK/Plugin/AkVorbisFactory.h>     // Needed for the Ogg Vorbis codec plugin
#pragma comment(lib,"AkVorbisDecoder.lib")

#include <AK/Tools/Common/AkPlatformFuncs.h>   // Thread defines
#include "AKIncludes/AkFilePackageLowLevelIOBlocking.h" // Sample low-level I/O implementation
#include <AK/SoundEngine/Common/AkModule.h>    // Default memory and stream managers
#include <AK/SoundEngine/Common/AkQueryParameters.h>
#include <AK/SoundEngine/Common/AkDynamicSequence.h>
#include <AK/IBytes.h>
#include "Wwise_IDs.h"

#define NUM_WWISE_MISSILES	15
#define NUM_WWISE_ENEMIES	20

struct GameObjectPair
{
	CEntity* Entity;
	AkGameObjectID SoundObject;
};

struct ToggleNode
{
	char SoundPlayToggle;
	GameObjectPair ObjectInfo;
};


// Bank file names
#define BANKNAME_INIT		L"Init.bnk"
#define BANKNAME_SOUNDBANK	L"SoundBank.bnk"

// We're using the default Low-Level I/O implementation that's part
// of the SDK's sample code, with the file package extension
CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

// Defining the hooks below is required for every project that links to the
// Wwise sound engine.
// Custom alloc/free functions. These are declared as "extern" in AkMemoryMgr.h
// and MUST be defined by the game developer.
namespace AK
{
	// This is the default implementation of the Memory Manager.
	void* AllocHook(size_t in_size)
	{
		return malloc(in_size);
	}
	void FreeHook(void* in_ptr)
	{
		free(in_ptr);
	}

	// Note: VirtualAllocHook(() may be used by I/O pools of the default implementation
	// of the Stream Manager, to allow "true" unbuffered I/O (using FILE_FLAG_NO_BUFFERING
	// - refer to the Windows SDK documentation for more details). This is NOT mandatory;
	// you may implement it with a simple malloc().
	void* VirtualAllocHook(void*  in_pMemAddress,
		size_t in_size,
		DWORD in_dwAllocationType,
		DWORD in_dwProtect)
	{
		return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
	}
	void VirtualFreeHook(void* in_pMemAddress,
		size_t in_size,
		DWORD in_dwFreeType)
	{
		VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
	}
};

namespace WwiseNS
{
	// Here are all the sound event toggles
	// for the Wwise Sound System setup.
	//TODO:: Make our own Events
	enum SoundEvents
	{
		//////////////////////////////////////		
		// Kill switch
		//////////////////////////////////////
		EVENT_KILL_ALL = 0,

		//////////////////////////////////////		
		// Music
		//////////////////////////////////////

		EVENT_MUSIC_PLAY_MENU,
		EVENT_MUSIC_STOP_MENU,
		EVENT_MUSIC_PLAY_LVL1,
		EVENT_MUSIC_STOP_LVL1,
		EVENT_MUSIC_PLAY_LVL2,
		EVENT_MUSIC_STOP_LVL2,
		EVENT_MUSIC_PLAY_BATTLETHEME,
		EVENT_MUSIC_STOP_BATTLETHEME,
		EVENT_MUSIC_PLAY_CREDITSTHEME,
		EVENT_MUSIC_STOP_CREDITSTHEME,
		EVENT_MUSIC_PLAY_BOSSTHEME,
		EVENT_MUSIC_STOP_BOSSTHEME,
		
		//////////////////////////////////////		
		// Menu Sounds
		//////////////////////////////////////
		EVENT_SOUND_MENUITEM_SWITCH,
		EVENT_SOUND_MENUITEM_CONFIRM,
		EVENT_SOUND_MENUITEM_CANCEL,
		EVENT_SOUND_MENUITEM_PLAY_CONFIRM,

		//////////////////////////////////////		
		// Looping sounds
		//////////////////////////////////////


		//////////////////////////////////////		
		// Player sounds
		//////////////////////////////////////
		EVENT_SOUND_PLAY_PLAYER_IDLE,
		EVENT_SOUND_STOP_PLAYER_IDLE,
		EVENT_SOUND_PLAY_PLAYER_THRUST,
		EVENT_SOUND_PLAYER_DODGE,
		EVENT_SOUND_PLAYER_HYPERSPACE,
		EVENT_SOUND_PLAYER_MODECHANGE,
		EVENT_SOUND_PLAYER_FIRE_LASER,
		EVENT_SOUND_PLAYER_FIRE_MISSILE,
		EVENT_SOUND_PLAYER_SHIELD_HURT,
		EVENT_SOUND_PLAYER_SHIELD_RECHARGE,
		EVENT_SOUND_PLAYER_SHIELD_FULL,
		EVENT_SOUND_PLAYER_HEALTH_RECHARGE,
		EVENT_SOUND_PLAYER_SHIELD_BROKE,
		EVENT_SOUND_PLAYER_HEALTH_FULL,
		EVENT_SOUND_PLAYER_LOCK_ON_WARNING,
		EVENT_SOUND_PLAYER_LOW_SHIELDS,
		EVENT_SOUND_PLAYER_LOW_HEALTH,
		EVENT_SOUND_PLAYER_DEATH,
		EVENT_SOUND_MISSILE_TRAVEL,
		EVENT_SOUND_STOP_MISSILE_TRAVEL,
		EVENT_SOUND_MISSILE_LOCK_ON,
		EVENT_SOUND_MISSILE_EXPLOSION,
		EVENT_SOUND_MISSILE_RELOAD,
		EVENT_SOUND_PLAYER_HULL_HIT,

		//////////////////////////////////////		
		// Enemy sounds
		//////////////////////////////////////
		EVENT_SOUND_ENEMY_THRUST,
		EVENT_SOUND_ENEMY_EXPLOSION,
		EVENT_SOUND_ENEMY_FIRE_LASER,
		EVENT_SOUND_ENEMY_FIRE_MISSILE,
		EVENT_SOUND_ENEMY_LASER_HIT_METAL,
		EVENT_SOUND_ENEMY_LASER_HIT_ROCK,
		EVENT_SOUND_ENEMY_LASER_HIT_SHEILD,
		EVENT_SOUND_ENEMY_SPOTTED_ALERT,
		EVENT_SOUND_ENEMY_HYDRA_LASER,
		EVENT_SOUND_ENEMY_HYDRA_STOP_LASER,
		EVENT_SOUND_ENEMY_HYDRA_LASER_CHARGE,
		EVENT_SOUND_STOP_ENEMY_HYDRA_LASER_CHARGE,
		EVENT_SOUND_ENEMY_HYDRA_PEASHOOTER,
		EVENT_SOUND_ENEMY_HYDRA_SHEILD_GEN,
		EVENT_SOUND_ENEMY_HYDRA_SHEILD_HIT,
		EVENT_SOUND_ENEMY_HYDRA_ENTRANCE,
		EVENT_SOUND_ENEMY_HYDRA_TIMEROUT,
		EVENT_SOUND_ENEMY_HYDRA_PLANETDESTROYED,
		EVENT_SOUND_PLAY_HYDRA_THRUSTERS,
				
		//////////////////////////////////////		
		// Dialogue sounds
		//////////////////////////////////////
		
		EVENT_SOUND_AI_MOVE_MOUSE_PLAY,
		EVENT_SOUND_AI_MOVE_MOUSE_STOP,
		EVENT_SOUND_AI_FIREWEAPON_PLAY,
		EVENT_SOUND_AI_FIREWEAPON_STOP,
		EVENT_SOUND_AI_SPEEDUPSLOWDOWN_PLAY,
		EVENT_SOUND_AI_SPEEDUPSLOWDOWN_STOP,
		EVENT_SOUND_AI_GREATJOB_PLAY,
		EVENT_SOUND_AI_GREATJOB_STOP,
		EVENT_SOUND_AI_DODGEROLL_PLAY,
		EVENT_SOUND_AI_DODGEROLL_STOP,
		EVENT_SOUND_AI_DODGEROLLCOOLDOWN_PLAY,
		EVENT_SOUND_AI_DODGEROLLCOOLDOWN_STOP,
		EVENT_SOUND_AI_SWITCHMODES_PLAY,
		EVENT_SOUND_AI_SWITCHMODES_STOP,
		EVENT_SOUND_AI_MODEWEAPON_PLAY,
		EVENT_SOUND_AI_MODEWEAPON_STOP,
		EVENT_SOUND_AI_BLUEMODEREGENERATE_PLAY,
		EVENT_SOUND_AI_BLUEMODEREGENERATE_STOP,
		EVENT_SOUND_AI_QUICKTURN_PLAY,
		EVENT_SOUND_AI_QUICKTURN_STOP,
		EVENT_SOUND_AI_QUICKTURNREPOSITION_PLAY,
		EVENT_SOUND_AI_QUICKTURNREPOSITION_STOP,
		EVENT_SOUND_AI_LOOKANENEMY_PLAY,
		EVENT_SOUND_AI_LOOKANENEMY_STOP,
		EVENT_SOUND_AI_SEEKERENTRANCE_PLAY,
		EVENT_SOUND_AI_SEEKERENTRANCE_STOP,
		EVENT_SOUND_AI_CRUISERENTRANCE_PLAY,
		EVENT_SOUND_AI_CRUISERENTRANCE_STOP,
		EVENT_SOUND_AI_MARAUDERENTRANCE_PLAY,
		EVENT_SOUND_AI_MARAUDERENTRANCE_STOP,
		EVENT_SOUND_AI_HYDRAENTRANCE_PLAY,
		EVENT_SOUND_AI_HYDRAENTRANCE_STOP,
		EVENT_SOUND_AI_HYDRASHIELDENTRANCE_PLAY,
		EVENT_SOUND_AI_HYDRASHIELDENTRANCE_STOP,
		EVENT_SOUND_AI_HYDRATURRETENTRANCE_PLAY,
		EVENT_SOUND_AI_HYDRATURRETENTRANCE_STOP,
		EVENT_SOUND_AI_HYDRAMEGALASERENTRANCE_PLAY,
		EVENT_SOUND_AI_HYDRAMEGALASERENTRANCE_STOP,
		EVENT_SOUND_AI_LONGVICTORY_PLAY,
		EVENT_SOUND_AI_LONGVICTORY_STOP,
		EVENT_SOUND_AI_QUICKVICTORY_PLAY,
		EVENT_SOUND_AI_QUICKVICTORY_STOP,
		EVENT_SOUND_AI_TIMERRANOUT_PLAY,
		EVENT_SOUND_AI_TIMERRANOUT_STOP,
		EVENT_SOUND_AI_PLAYERCRITICALHEALTH_PLAY,
		EVENT_SOUND_AI_PLAYERCRITICALHEALTH_STOP,
		EVENT_SOUND_AI_PLAYERSHIELDDOWN_PLAY,
		EVENT_SOUND_AI_PLAYERSHIELDDOWN_STOP,
		EVENT_SOUND_AI_WAVECOMPLETE_PLAY,
		EVENT_SOUND_AI_WAVECOMPLETE_STOP,
		EVENT_SOUND_AI_INCOMMINGWAVE_PLAY,
		EVENT_SOUND_AI_INCOMMINGWAVE_STOP,
		

		//////////////////////////////////////		
		// Other sounds
		//////////////////////////////////////
		EVENT_SOUND_ASTEROID_EXPLOSION,
		EVENT_SOUND_HUD_BOUNDS,
		EVENT_SOUND_HUD_ONYOURSIX,
		EVENT_SOUND_HUD_GOODJOB,
		EVENT_SOUND_HUD_CHECKPOINT,
		EVENT_SOUND_HUD_COUNTDOWN,
		EVENT_SOUND_HUD_PROCEED,

		// TODO: If more sounds are needed, add events here

		EVENT_NUM_EVENTS
	};



	// Register and Unregister of Wwise
	// game object.
	void RegisterGameObjects();
	void UnregisterGameObjects();

	// Loading and Unloading of Wwise
	// sound banks.
	void LoadSoundBanks();
	void UnloadSoundBanks();

	// Toggling, handling, processing and
	// playing of events/music.
	void StopAllMusic();

	// Movement of the listener, which in
	// this application is the player.

	// 'Set' functions for Wwise game object positions.
	void SetGameObjPos(AkGameObjectID gameObjectID, AkSoundPosition soundPosition);

	// These are the private update functions
	// for the sound application.
	void UpdateGame();
	void UpdateListenerPos();
	void UpdatePlayer();
	void UpdateGameObjects();

	// Player Wwise game object...
	const AkGameObjectID GAME_OBJECT_ID_PLAYER			= 1;
	const AkGameObjectID GAME_OBJECT_ID_BGM				= 2;
	const AkGameObjectID GAME_OBJECT_ID_BGM2			= 3;
	const AkGameObjectID GAME_OBJECT_ID_BOSS			= 4;
	const AkGameObjectID GAME_OBJECT_ID_ASTEROID		= 5;
	const AkGameObjectID GAME_OBJECT_ID_DEFAULT_ENEMY	= 6;
	
	const AkGameObjectID GAME_OBJECT_ID_MAX				= 7;
	
	GameObjectPair GAME_OBJECT_ID_MISSILES[NUM_WWISE_MISSILES];
	GameObjectPair GAME_OBJECT_ID_ENEMIES[NUM_WWISE_ENEMIES];

	static AkPlayingID playingID = 0;

	AkSoundPosition		playerPos;
	AkSoundPosition		soundPos;
	AkSoundPosition		BossPos;
	AkSoundPosition		AsteroidPos;
	AkListenerPosition	listener;
	AkTimeMs			uPosition = 0;
	bool				m_bPreventPlaying;

	ToggleNode	triggerSoundEvents[EVENT_NUM_EVENTS];	// sound event bit toggles

	//Important functions
	void ToggleEventsOn(SoundEvents soundEvent, CEntity* _Entity);
	void ToggleEventsOn(SoundEvents soundEvent, AkGameObjectID GameObjID = GAME_OBJECT_ID_PLAYER);
	void HandleEvents();
	int ProcessAudio();

};

int WwiseNS::InitSoundEngine()
{
	m_bPreventPlaying = false;
	for(int nNumMissiles = 0; nNumMissiles < NUM_WWISE_MISSILES; nNumMissiles++)
	{
		GAME_OBJECT_ID_MISSILES[nNumMissiles].Entity = nullptr;
		GAME_OBJECT_ID_MISSILES[nNumMissiles].SoundObject = GAME_OBJECT_ID_MAX + nNumMissiles;
	}
	for(int nNumEnemies = 0; nNumEnemies < NUM_WWISE_ENEMIES; nNumEnemies++)
	{
		GAME_OBJECT_ID_ENEMIES[nNumEnemies].Entity = nullptr;
		GAME_OBJECT_ID_ENEMIES[nNumEnemies].SoundObject = GAME_OBJECT_ID_MAX + nNumEnemies + NUM_WWISE_MISSILES/*Number of Missile Objects*/;
	}

	// Creating and initializing an instance of the default memory manager.
	// It is highly recommended you don't use the default settings for your game.
	AkMemSettings memSettings;
	memSettings.uMaxNumPools = 20;
	if(AK::MemoryMgr::Init(&memSettings) != AK_Success)
	{
		AKASSERT(!"Could not create the memory manager.");
		return 0;
	}

	// Create and initialize an instance of the default streaming manager. Note
	// that you can override the dafault streaming manager with your own. Refer
	// to the SDK documentation for more information.
	// It is highly recommended you don't use the default settings for your game.
	AkStreamMgrSettings stmSettings;
	// 	stmSettings.uMemorySize = 32 * 1024*1024; // 32 MB
	AK::StreamMgr::GetDefaultSettings(stmSettings);
	if(!AK::StreamMgr::Create(stmSettings))
	{
		AKASSERT(!"Could not create the Streaming Manager");
		return 0;
	}

	// Create a streaming device with blocking low-level I/O handshaking.
	// Note that you can override the default low-level I/O module with your own. Refer
	// to the SDK documentation for more information.
	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);
	// CAkFilePackageLowLevelIOBlocking::Init() creates a streaming device
	// in the Stream Manager, and registers itself as the File Location Resolver.
	if(g_lowLevelIO.Init(deviceSettings) != AK_Success)
	{
		AKASSERT(!"Could not create the streaming device and Low-Level I/O system");
		return 0;
	}

	// Create the Sound Engine
	// Using default initialization parameters
	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

	// Setting pool sizes for this game.
	// These sizes are tuned for this game, every game should determine its own optimal values.
	// 	initSettings.uDefaultPoolSize           = 4 * 1024 * 1024;  // 4 MB
	// 	initSettings.uMaxNumPaths               = 16;
	// 	initSettings.uMaxNumTransitions         = 128;

	//	platformInitSettings.uLEngineDefaultPoolSize    = 4 * 1024 * 1024;  // 4 MB

	if(AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
	{
		AKASSERT(!"Could not initialize the Sound Engine.");
		return 0;
	}

	// Initialize the music engine
	// Using default initialization parameters
	// *****This must be done after the sound engine******
	AkMusicSettings musicInit;
	musicInit.fStreamingLookAheadRatio = 100;
	if(AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		AKASSERT(!"Could not initialize the Music Engine.");
		return 0;
	}
#ifndef AK_OPTIMIZED
	// Initialize communications
	// This is necessary to perform in-game mixing, profiling and troubleshooting.
	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);
	if(AK::Comm::Init(commSettings) != AK_Success)
	{
		assert(!"Could not initialize communication.");
		return 0;
	}
#endif // AK_OPTIMIZED
	// After all initialization is completed we will register
	// any plugins that are needed for the game. In this example,
	// we will use the Ogg Vorbis codec plugin.
	AK::SoundEngine::RegisterCodec(AKCOMPANYID_AUDIOKINETIC,
		AKCODECID_VORBIS,
		CreateVorbisFilePlugin,
		CreateVorbisBankPlugin);

	// Clearing out the listener position object
	memset(&listener, 0, sizeof(AkListenerPosition));
	memset(&soundPos, 0, sizeof(AkSoundPosition));
	memset(&BossPos, 0, sizeof(AkSoundPosition));
	memset(&AsteroidPos, 0, sizeof(AkSoundPosition));

	// Sound and listener position/orientation setup
	listener.OrientationTop.Y = listener.OrientationFront.Z = 1.0f;
	listener.OrientationTop.X = listener.OrientationTop.Z = listener.OrientationFront.X = listener.OrientationFront.Y = 0.0;

	AsteroidPos.Orientation.Y = AsteroidPos.Orientation.Z = 1.0f;
	AsteroidPos.Orientation.X = AsteroidPos.Position.X = AsteroidPos.Position.Y = AsteroidPos.Position.Z = 0.0;

	// Set sound object position
	UpdateListenerPos();
	// Unitializing event flags
	for(int i = 0; i < EVENT_NUM_EVENTS; i++)
	{
		triggerSoundEvents[i].SoundPlayToggle			= 0;
		triggerSoundEvents[i].ObjectInfo.Entity			= nullptr;
		triggerSoundEvents[i].ObjectInfo.SoundObject	= -1;
	}

	//Jayson: Load in soundbank
	LoadSoundBanks();
	RegisterGameObjects();

	//printf_s("Successful Initialization\n");
	return 1;
}

void WwiseNS::SoundEvent( int _SoundEvent, int _DoWhat, bool PlayerSound, CEntity* _pEntity)
{
	if(_DoWhat == ePause)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PAUSE_ALL, GAME_OBJECT_ID_BGM);
		return;
	}
	if(_DoWhat == eResume)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::RESUME_ALL, GAME_OBJECT_ID_BGM);
		return;
	}

	switch(_SoundEvent)
	{
		////////////////////////////////////////////////////////////////////////
		// looped Music (Menu and Levels)
		////////////////////////////////////////////////////////////////////////
	case eBGM_MainMenuMusic:
		if(_DoWhat == ePlay)
			ToggleEventsOn(EVENT_MUSIC_PLAY_MENU, GAME_OBJECT_ID_BGM);
		else if(_DoWhat == eStop)
			ToggleEventsOn(EVENT_MUSIC_STOP_MENU, GAME_OBJECT_ID_BGM);
		break;

	case eBGM_BattleMusic:
		if(_DoWhat == ePlay)
			ToggleEventsOn(EVENT_MUSIC_PLAY_BATTLETHEME, GAME_OBJECT_ID_BGM);
		else if(_DoWhat == eStop)
			ToggleEventsOn(EVENT_MUSIC_STOP_BATTLETHEME, GAME_OBJECT_ID_BGM);
		break;

	case eBGM_CreditsMusic:
		if(_DoWhat == ePlay)
			ToggleEventsOn(EVENT_MUSIC_PLAY_CREDITSTHEME, GAME_OBJECT_ID_BGM);
		else if(_DoWhat == eStop)
			ToggleEventsOn(EVENT_MUSIC_STOP_CREDITSTHEME, GAME_OBJECT_ID_BGM);
		break;

	case eBGM_LevelOneMusic:
		if(_DoWhat == ePlay)
			ToggleEventsOn(EVENT_MUSIC_PLAY_LVL1, GAME_OBJECT_ID_BGM);
		else if(_DoWhat == eStop)
			ToggleEventsOn(EVENT_MUSIC_STOP_LVL1, GAME_OBJECT_ID_BGM);
		break;

	case eBGM_LevelTwoMusic:
		if(_DoWhat == ePlay)
			ToggleEventsOn(EVENT_MUSIC_PLAY_LVL2, GAME_OBJECT_ID_BGM2);
		else if(_DoWhat == eStop)
			ToggleEventsOn(EVENT_MUSIC_STOP_LVL2, GAME_OBJECT_ID_BGM2);
		break;

	case eBGM_BossMusic:
		if(_DoWhat == ePlay)
			ToggleEventsOn(EVENT_MUSIC_PLAY_BOSSTHEME, GAME_OBJECT_ID_BGM);
		else if(_DoWhat == eStop)
			ToggleEventsOn(EVENT_MUSIC_STOP_BOSSTHEME, GAME_OBJECT_ID_BGM);
		break;
	
		//////////////////////////////////////		
		// Menu Sounds
		//////////////////////////////////////
	case eMenuItemSwitch:
		if(_DoWhat == ePlay)
			ToggleEventsOn(EVENT_SOUND_MENUITEM_SWITCH, GAME_OBJECT_ID_BGM);
		break;

	case eMenuItemConfirm:
		if(_DoWhat == ePlay)
			ToggleEventsOn(EVENT_SOUND_MENUITEM_CONFIRM, GAME_OBJECT_ID_BGM);
		break;

	case eMenuItemCancel:
		if(_DoWhat == ePlay)
			ToggleEventsOn(EVENT_SOUND_MENUITEM_CANCEL, GAME_OBJECT_ID_BGM);
		break;

	case eMenuItemPlayGame:
		if(_DoWhat == ePlay)
			ToggleEventsOn(EVENT_SOUND_MENUITEM_PLAY_CONFIRM, GAME_OBJECT_ID_BGM);
		break;

		////////////////////////////////////////		
		//// Player sounds
		////////////////////////////////////////
	case ePlayerIdle:
		if(_DoWhat == ePlay)
			ToggleEventsOn(EVENT_SOUND_PLAY_PLAYER_IDLE, GAME_OBJECT_ID_PLAYER);
		else if(_DoWhat == eStop)
			ToggleEventsOn(EVENT_SOUND_STOP_PLAYER_IDLE, GAME_OBJECT_ID_PLAYER);
		break;

	case ePlayerThrust:
		if(_DoWhat == ePlay)
			ToggleEventsOn(EVENT_SOUND_PLAY_PLAYER_THRUST, GAME_OBJECT_ID_PLAYER);
		break;

		case ePlayerShieldBroken:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_PLAYER_SHIELD_BROKE, GAME_OBJECT_ID_PLAYER);
			break;

		case ePlayerHealthFull:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_PLAYER_HEALTH_FULL, GAME_OBJECT_ID_PLAYER);
			break;

		case ePlayerLowHealthWarning:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_PLAYER_LOW_HEALTH, GAME_OBJECT_ID_PLAYER);
			break;
		
		case ePlayerDeath:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_PLAYER_DEATH, GAME_OBJECT_ID_PLAYER);
			break;

		case ePlayerLowShields:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_PLAYER_LOW_SHIELDS, GAME_OBJECT_ID_PLAYER);
			break;

		case eLockOnWarning:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_PLAYER_LOCK_ON_WARNING, GAME_OBJECT_ID_PLAYER);
			break;

		case eMissileLockOn:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_MISSILE_LOCK_ON, GAME_OBJECT_ID_PLAYER);
			break;

		case eMissileExplosion:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_MISSILE_EXPLOSION, _pEntity);
			break;

		case eMissileReload:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_MISSILE_RELOAD, GAME_OBJECT_ID_PLAYER);
			break;

		case eMissileTravel:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_MISSILE_TRAVEL, _pEntity);
			else if(_DoWhat == eStop)
				ToggleEventsOn(EVENT_SOUND_STOP_MISSILE_TRAVEL, _pEntity);
			break;

		case eHullRepair:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_PLAYER_HEALTH_RECHARGE, GAME_OBJECT_ID_PLAYER);
			break;

		case eSheildRecharge:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_PLAYER_SHIELD_RECHARGE, GAME_OBJECT_ID_PLAYER);
			break;

		case eSheildFull:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_PLAYER_SHIELD_FULL, GAME_OBJECT_ID_PLAYER);
			break;

		case eHyperSpace:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_PLAYER_HYPERSPACE, GAME_OBJECT_ID_PLAYER);
			break;

		case eModeChange:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_PLAYER_MODECHANGE, GAME_OBJECT_ID_PLAYER);
			break;

		case ePlayerMissileFire :
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_PLAYER_FIRE_MISSILE, GAME_OBJECT_ID_PLAYER);
			break;

		case ePlayerLaserFire:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_PLAYER_FIRE_LASER, GAME_OBJECT_ID_PLAYER);
			break;

		case ePlayerShieldHit:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_PLAYER_SHIELD_HURT, GAME_OBJECT_ID_PLAYER);
			break;

		case ePlayerHullHit:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_PLAYER_HULL_HIT, GAME_OBJECT_ID_PLAYER);
			break;

		case ePlayerDodge:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_PLAYER_DODGE, GAME_OBJECT_ID_PLAYER);
			break;

		//////////////////////////////////////////////////////////////////////
		// Enemy sounds
		//////////////////////////////////////////////////////////////////////
		case eEnemyThrust:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_ENEMY_THRUST, _pEntity);
			break;
		case eEnemyExplosion:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_ENEMY_EXPLOSION, _pEntity);
				break;

		case eEnemyLaserFire:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_ENEMY_FIRE_LASER, _pEntity);
			break;

		case eEnemyMissileFire:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_ENEMY_FIRE_MISSILE, _pEntity);
			break;

		case eLaserHitRock:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_ENEMY_LASER_HIT_ROCK, _pEntity);
			break;
		case eLaserHitSheild:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_ENEMY_LASER_HIT_SHEILD, _pEntity);
			break;
		case eLaserHitMetal: 
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_ENEMY_LASER_HIT_METAL, _pEntity);
			break;

		case eHydraLaser:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_ENEMY_HYDRA_LASER, GAME_OBJECT_ID_BOSS);
			if(_DoWhat == eStop)
				ToggleEventsOn(EVENT_SOUND_ENEMY_HYDRA_STOP_LASER, GAME_OBJECT_ID_BOSS);
			break;
			
		case eHydraPeaShooter:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_ENEMY_HYDRA_PEASHOOTER, _pEntity);
			break;

		case eHydraLaserCharge:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_ENEMY_HYDRA_LASER_CHARGE, GAME_OBJECT_ID_BOSS);
			if(_DoWhat == eStop)
				ToggleEventsOn(EVENT_SOUND_STOP_ENEMY_HYDRA_LASER_CHARGE, GAME_OBJECT_ID_BOSS);
			break;

		case eHydraShieldGenDown:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_ENEMY_HYDRA_SHEILD_GEN, GAME_OBJECT_ID_BOSS);
			break;

		case eHydraSheildHit:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_ENEMY_HYDRA_SHEILD_HIT, GAME_OBJECT_ID_BOSS);
			break;

		case eHydraEntrance:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_ENEMY_HYDRA_ENTRANCE, GAME_OBJECT_ID_BOSS);
			break;

		case eHydraTimerOut:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_ENEMY_HYDRA_TIMEROUT, GAME_OBJECT_ID_BOSS);
			break;
		case eHydraPlanetDestroyed:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_ENEMY_HYDRA_PLANETDESTROYED, GAME_OBJECT_ID_BOSS);
			break;
		
		case eHydraThrusters:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_PLAY_HYDRA_THRUSTERS, GAME_OBJECT_ID_BOSS);
			break;

	//////////////////////////////////////////////////////////////////////
	// Dialogue sounds
	//////////////////////////////////////////////////////////////////////
		case eAIMoveMouse:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_AI_MOVE_MOUSE_PLAY, GAME_OBJECT_ID_PLAYER);
			else if(_DoWhat == eStop)
				ToggleEventsOn(EVENT_SOUND_AI_MOVE_MOUSE_STOP, GAME_OBJECT_ID_PLAYER);
			break;
		case eAIFireWeapon:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_AI_FIREWEAPON_PLAY, GAME_OBJECT_ID_PLAYER);
			else if(_DoWhat == eStop)
				ToggleEventsOn(EVENT_SOUND_AI_FIREWEAPON_STOP, GAME_OBJECT_ID_PLAYER);
			break;
		case eAISpeedUpSlowDown:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_AI_SPEEDUPSLOWDOWN_PLAY, GAME_OBJECT_ID_PLAYER);
			else if(_DoWhat == eStop)
				ToggleEventsOn(EVENT_SOUND_AI_SPEEDUPSLOWDOWN_STOP, GAME_OBJECT_ID_PLAYER);
			break;
		case eAIGreatJob:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_AI_GREATJOB_PLAY, GAME_OBJECT_ID_PLAYER);
			else if(_DoWhat == eStop)
				ToggleEventsOn(EVENT_SOUND_AI_GREATJOB_STOP, GAME_OBJECT_ID_PLAYER);
			break;
		case eAIDodgeRoll:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_AI_DODGEROLL_PLAY, GAME_OBJECT_ID_PLAYER);
			else if(_DoWhat == eStop)
				ToggleEventsOn(EVENT_SOUND_AI_DODGEROLL_STOP, GAME_OBJECT_ID_PLAYER);
			break;
		case eAIDodgeRollCooldown:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_AI_DODGEROLLCOOLDOWN_PLAY, GAME_OBJECT_ID_PLAYER);
			else if(_DoWhat == eStop)
				ToggleEventsOn(EVENT_SOUND_AI_DODGEROLLCOOLDOWN_STOP, GAME_OBJECT_ID_PLAYER);
			break;
		case eAISwitchModes:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_AI_SWITCHMODES_PLAY, GAME_OBJECT_ID_PLAYER);
			else if(_DoWhat == eStop)
				ToggleEventsOn(EVENT_SOUND_AI_SWITCHMODES_STOP, GAME_OBJECT_ID_PLAYER);
			break;
		case eAIModeWeapon:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_AI_MODEWEAPON_PLAY, GAME_OBJECT_ID_PLAYER);
			else if(_DoWhat == eStop)
				ToggleEventsOn(EVENT_SOUND_AI_MODEWEAPON_STOP, GAME_OBJECT_ID_PLAYER);
			break;
		case eAIBlueModeRegenerate:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_AI_BLUEMODEREGENERATE_PLAY, GAME_OBJECT_ID_PLAYER);
			else if(_DoWhat == eStop)
				ToggleEventsOn(EVENT_SOUND_AI_BLUEMODEREGENERATE_STOP, GAME_OBJECT_ID_PLAYER);
			break;
		case eAIQuickTurn:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_AI_QUICKTURN_PLAY, GAME_OBJECT_ID_PLAYER);
			else if(_DoWhat == eStop)
				ToggleEventsOn(EVENT_SOUND_AI_QUICKTURN_STOP, GAME_OBJECT_ID_PLAYER);
			break;
		case eAIQuickTurnReposition:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_AI_QUICKTURNREPOSITION_PLAY, GAME_OBJECT_ID_PLAYER);
			else if(_DoWhat == eStop)
				ToggleEventsOn(EVENT_SOUND_AI_QUICKTURNREPOSITION_STOP, GAME_OBJECT_ID_PLAYER);
			break;
		case eAILookAnEnemy:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_AI_LOOKANENEMY_PLAY, GAME_OBJECT_ID_PLAYER);
			else if(_DoWhat == eStop)
				ToggleEventsOn(EVENT_SOUND_AI_LOOKANENEMY_STOP, GAME_OBJECT_ID_PLAYER);
			break;
		case eAISeekerEntrance:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_AI_SEEKERENTRANCE_PLAY, GAME_OBJECT_ID_PLAYER);
			else if(_DoWhat == eStop)
				ToggleEventsOn(EVENT_SOUND_AI_SEEKERENTRANCE_STOP, GAME_OBJECT_ID_PLAYER);
			break;
		case eAICruiserEntrance:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_AI_CRUISERENTRANCE_PLAY, GAME_OBJECT_ID_PLAYER);
			else if(_DoWhat == eStop)
				ToggleEventsOn(EVENT_SOUND_AI_CRUISERENTRANCE_STOP, GAME_OBJECT_ID_PLAYER);
			break;
		case eAIMarauderEntrance:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_AI_MARAUDERENTRANCE_PLAY, GAME_OBJECT_ID_PLAYER);
			else if(_DoWhat == eStop)
				ToggleEventsOn(EVENT_SOUND_AI_MARAUDERENTRANCE_STOP, GAME_OBJECT_ID_PLAYER);
			break;
		case eAIHydraEntrance:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_AI_HYDRAENTRANCE_PLAY, GAME_OBJECT_ID_PLAYER);
			else if(_DoWhat == eStop)
				ToggleEventsOn(EVENT_SOUND_AI_HYDRAENTRANCE_STOP, GAME_OBJECT_ID_PLAYER);
			break;
		case eAIHydraShieldEntrance:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_AI_HYDRASHIELDENTRANCE_PLAY, GAME_OBJECT_ID_PLAYER);
			else if(_DoWhat == eStop)
				ToggleEventsOn(EVENT_SOUND_AI_HYDRASHIELDENTRANCE_STOP, GAME_OBJECT_ID_PLAYER);
			break;
		case eAIHydraTurretEntrance:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_AI_HYDRATURRETENTRANCE_PLAY, GAME_OBJECT_ID_PLAYER);
			else if(_DoWhat == eStop)
				ToggleEventsOn(EVENT_SOUND_AI_HYDRATURRETENTRANCE_STOP, GAME_OBJECT_ID_PLAYER);
			break;
		case eAIHydraMegaLaserEntrance:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_AI_HYDRAMEGALASERENTRANCE_PLAY, GAME_OBJECT_ID_PLAYER);
			else if(_DoWhat == eStop)
				ToggleEventsOn(EVENT_SOUND_AI_HYDRAMEGALASERENTRANCE_STOP, GAME_OBJECT_ID_PLAYER);
			break;
		case eAILongVictory:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_AI_LONGVICTORY_PLAY, GAME_OBJECT_ID_PLAYER);
			else if(_DoWhat == eStop)
				ToggleEventsOn(EVENT_SOUND_AI_LONGVICTORY_STOP, GAME_OBJECT_ID_PLAYER);
			break;
		case eAIQuickVictory:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_AI_QUICKVICTORY_PLAY, GAME_OBJECT_ID_PLAYER);
			else if(_DoWhat == eStop)
				ToggleEventsOn(EVENT_SOUND_AI_QUICKVICTORY_STOP, GAME_OBJECT_ID_PLAYER);
			break;
		case eAITimerRanOut:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_AI_TIMERRANOUT_PLAY, GAME_OBJECT_ID_PLAYER);
			else if(_DoWhat == eStop)
				ToggleEventsOn(EVENT_SOUND_AI_TIMERRANOUT_STOP, GAME_OBJECT_ID_PLAYER);
			break;
		case eAIPlayerCriticalHealth:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_AI_PLAYERCRITICALHEALTH_PLAY, GAME_OBJECT_ID_PLAYER);
			else if(_DoWhat == eStop)
				ToggleEventsOn(EVENT_SOUND_AI_PLAYERCRITICALHEALTH_STOP, GAME_OBJECT_ID_PLAYER);
			break;
		case eAIPlayerShieldsDown:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_AI_PLAYERSHIELDDOWN_PLAY, GAME_OBJECT_ID_PLAYER);
			else if(_DoWhat == eStop)
				ToggleEventsOn(EVENT_SOUND_AI_PLAYERSHIELDDOWN_STOP, GAME_OBJECT_ID_PLAYER);
			break;
		case eAIWaveComplete:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_AI_WAVECOMPLETE_PLAY , GAME_OBJECT_ID_PLAYER);
			break;
		case eAIIncommingWave:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_AI_INCOMMINGWAVE_PLAY , GAME_OBJECT_ID_PLAYER);
			break;
		
	//////////////////////////////////////////////////////////////////////
	// Extra sounds
	//////////////////////////////////////////////////////////////////////
		case eOnYourSix:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_HUD_ONYOURSIX, GAME_OBJECT_ID_BGM);
			break;

		case eCheckpoint:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_HUD_CHECKPOINT, GAME_OBJECT_ID_BGM);
			break;

		case eGoodJob:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_HUD_GOODJOB, GAME_OBJECT_ID_BGM);
			break;

		case eCountDown: 
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_HUD_COUNTDOWN, GAME_OBJECT_ID_BGM);
			break;

		case eAsteroidExplosion:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_ASTEROID_EXPLOSION, _pEntity);
			break;

		case eOutOfBounds:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_HUD_BOUNDS, GAME_OBJECT_ID_BGM);
			break;
		
		case eProceedToWaypoint:
			if(_DoWhat == ePlay)
				ToggleEventsOn(EVENT_SOUND_HUD_PROCEED, GAME_OBJECT_ID_BGM);
			break;

	default:
		break;
	}
}

int WwiseNS::ProcessAudio()
{
	// Process bank requests, events, positions, RTPC, etc.
	if(AK::SoundEngine::RenderAudio() != AK_Success)
		return 0;

	// For Level finish sound event
	AK::SoundEngine::GetSourcePlayPosition(playingID, &uPosition);
	return 1;
}

void WwiseNS::TermSoundEngine()
{
	RemoveAllSoundObjs();
	UnloadSoundBanks();

#ifndef AK_OPTIMIZED
	// Terminate Communication Services
	AK::Comm::Term();
#endif // AK_OPTIMIZED
	// Terminate the music engine
	AK::MusicEngine::Term();
	// Terminate the sound engine
	AK::SoundEngine::Term();
	// Terminate the streaming device and streaming manager
	// CAkFilePackageLowLevelIOBlocking::Term() destroys the associated streaming device
	// that lives in the Stream Manager, and unregisters itself as the File Location Resolver.
	g_lowLevelIO.Term();
	if(AK::IAkStreamMgr::Get())
		AK::IAkStreamMgr::Get()->Destroy();
	// Terminate the Memory Manager
	AK::MemoryMgr::Term();
}

void WwiseNS::LoadSoundBanks()
{
	// Setup banks path
	g_lowLevelIO.SetBasePath(DEFAULT_SOUNDBANK_PATH);

	// Load banks synchronously (from file name).

	// IMPORTANT NOTE-----------------------------------------------------------
	// You can load sound banks through the generated header file if and only
	// if the sound banks are generated with ID filenames and not the actual
	// sound bank filenames. The functions are written in the commented section
	// below.
	//--------------------------------------------------------------------------
	//eResult = AK::SoundEngine::LoadBank(AK::BANKS::INIT, AK_DEFAULT_POOL_ID);
	//eResult = AK::SoundEngine::LoadBank(AK::BANKS::SONIC, AK_DEFAULT_POOL_ID);
	//eResult = AK::SoundEngine::LoadBank(AK::BANKS::MARIO, AK_DEFAULT_POOL_ID);
	//--------------------------------------------------------------------------

	/*AkBankID bankID;	// Not used in this tutorial. Banks can be unloaded with their file name, this can be useful to unload menu or intro sounds while in game and vice versa.
	AKRESULT eResult;
	eResult = AK::SoundEngine::LoadBank(BANKNAME_INIT, AK_DEFAULT_POOL_ID, bankID);
	if(eResult != AK_BankAlreadyLoaded)
		assert(eResult == AK_Success);

	eResult = AK::SoundEngine::LoadBank(BANKNAME_SOUNDBANK, AK_DEFAULT_POOL_ID, bankID);
	assert(eResult == AK_Success);
	*/
	//printf_s("Loaded sound banks successfully\n\n");
}

void WwiseNS::UnloadSoundBanks()
{
	if (AK::SoundEngine::UnloadBank(BANKNAME_SOUNDBANK, nullptr) != AK_Success)
		printf_s("Failed to unload sound bank\n");

	//printf_s("Unloaded sound banks successfully.\n");
}

void WwiseNS::RegisterGameObjects()
{
	// Register game objects. Some global game objects might be registered
	// at initialization time, but most of your game objects will probably
	// be dynamically registered/unregistered as they spawn or get killed

	AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_PLAYER, "Player");
	AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_BGM, "BGM");
	AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_BGM2, "BGM2");
	AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_BOSS, "Boss");
	AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_ASTEROID, "Asteroid");
	AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_DEFAULT_ENEMY, "DefaultEnemy");

	for(int nNumMissiles = 0; nNumMissiles < NUM_WWISE_MISSILES; nNumMissiles++)
	{
		char buff[16];
		sprintf_s(buff, 16, "Missile%d", nNumMissiles);
		AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_MISSILES[nNumMissiles].SoundObject, buff);
	}

	for(int nNumEnemies = 0; nNumEnemies < NUM_WWISE_ENEMIES; nNumEnemies++)
	{
		char buff[16];
		sprintf_s(buff, 16, "Enemy%d", nNumEnemies);
		AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_ENEMIES[nNumEnemies].SoundObject, buff);
	}
	//AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_MENU, "MenuBGM");

	SetGameObjPos(GAME_OBJECT_ID_PLAYER, soundPos);

	// Setting the registered game object's position in the world.
	//TODO: Set Object Positions
	//SetGameObjPos();

	//TODO: Play menu music here
	//LevelMusicPlay();
}

void WwiseNS::UnregisterGameObjects()
{
	AK::SoundEngine::UnregisterGameObj(GAME_OBJECT_ID_PLAYER);
	AK::SoundEngine::UnregisterGameObj(GAME_OBJECT_ID_BGM);
	AK::SoundEngine::UnregisterGameObj(GAME_OBJECT_ID_BGM2);
	AK::SoundEngine::UnregisterGameObj(GAME_OBJECT_ID_BOSS);
	AK::SoundEngine::UnregisterGameObj(GAME_OBJECT_ID_ASTEROID);
	AK::SoundEngine::UnregisterGameObj(GAME_OBJECT_ID_DEFAULT_ENEMY);
	for(int nNumEnemies = 0; nNumEnemies < NUM_WWISE_ENEMIES; nNumEnemies++)
	{
		AK::SoundEngine::UnregisterGameObj(GAME_OBJECT_ID_ENEMIES[nNumEnemies].SoundObject);
	}
}

void WwiseNS::ToggleEventsOn(SoundEvents soundEvent, CEntity* _Enemy)
{
	if(triggerSoundEvents[soundEvent].SoundPlayToggle == 0)
	{
		triggerSoundEvents[soundEvent].SoundPlayToggle = 1;
		if(_Enemy != nullptr)
		{
			//if(_Enemy->GetType() == eMISSILE || _Enemy->GetType() == eLARGE_ASTEROID || _Enemy->GetType() == eGENERATOR)
			if(soundEvent == EVENT_SOUND_MISSILE_EXPLOSION || soundEvent == EVENT_SOUND_MISSILE_TRAVEL)
			{
				triggerSoundEvents[soundEvent].ObjectInfo.Entity = _Enemy;
				triggerSoundEvents[soundEvent].ObjectInfo.SoundObject = FindMissile(_Enemy);
			}
			else if(_Enemy->GetType() == eASTEROID)
			{
				D3DXVECTOR3 tmpPos = _Enemy->GetPosition();
				AsteroidPos.Position.X = tmpPos.x;
				AsteroidPos.Position.Y = tmpPos.y;
				AsteroidPos.Position.Z = tmpPos.z;
				triggerSoundEvents[soundEvent].ObjectInfo.Entity = _Enemy;
				triggerSoundEvents[soundEvent].ObjectInfo.SoundObject = GAME_OBJECT_ID_ASTEROID;
			}
			else
			{
				triggerSoundEvents[soundEvent].ObjectInfo.Entity = _Enemy;
				triggerSoundEvents[soundEvent].ObjectInfo.SoundObject = FindObject(_Enemy);
			}
		}
		else
		{
			triggerSoundEvents[soundEvent].ObjectInfo.Entity = nullptr;
			triggerSoundEvents[soundEvent].ObjectInfo.SoundObject = GAME_OBJECT_ID_DEFAULT_ENEMY;
		}
	}
}
void WwiseNS::ToggleEventsOn(SoundEvents soundEvent, AkGameObjectID GameObjID)
{

	if(triggerSoundEvents[soundEvent].SoundPlayToggle == 0)
	{
		triggerSoundEvents[soundEvent].SoundPlayToggle = 1;
		triggerSoundEvents[soundEvent].ObjectInfo.Entity = nullptr; 
		triggerSoundEvents[soundEvent].ObjectInfo.SoundObject = GameObjID;
	}
}

void WwiseNS::StopAllMusic()
{
	AK::SoundEngine::PostEvent(AK::EVENTS::STOP_MX_LEVELMUSIC, GAME_OBJECT_ID_BGM);
	AK::SoundEngine::PostEvent(AK::EVENTS::STOP_MX_MENUMUSIC, GAME_OBJECT_ID_BGM);
	AK::SoundEngine::PostEvent(AK::EVENTS::STOP_MX_CREDITS, GAME_OBJECT_ID_BGM);
	AK::SoundEngine::PostEvent(AK::EVENTS::STOP_MX_PAUSE_MENU, GAME_OBJECT_ID_BGM2);
}

void WwiseNS::HandleEvents()
{
	
	////////////////////////////////////////////////////////////////////////
	// Stop sounds first so we don't have bugs with music stopping.
	////////////////////////////////////////////////////////////////////////
	// Stop Menu Music
	if(triggerSoundEvents[EVENT_MUSIC_STOP_MENU].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_MX_MENUMUSIC, GAME_OBJECT_ID_BGM);
		triggerSoundEvents[EVENT_MUSIC_STOP_MENU].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_MUSIC_STOP_CREDITSTHEME].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_MX_CREDITS, GAME_OBJECT_ID_BGM);
		triggerSoundEvents[EVENT_MUSIC_STOP_CREDITSTHEME].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_MUSIC_STOP_LVL1].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_MX_LEVELMUSIC, GAME_OBJECT_ID_BGM);
		triggerSoundEvents[EVENT_MUSIC_STOP_LVL1].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_MUSIC_STOP_LVL2].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_MX_PAUSE_MENU, GAME_OBJECT_ID_BGM2);
		triggerSoundEvents[EVENT_MUSIC_STOP_LVL2].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_MUSIC_STOP_BOSSTHEME].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_MX_MENUMUSIC, GAME_OBJECT_ID_BGM);
		triggerSoundEvents[EVENT_MUSIC_STOP_BOSSTHEME].SoundPlayToggle = 0;
	}
	
	// Stop for Looping sounds
	if(triggerSoundEvents[EVENT_SOUND_STOP_PLAYER_IDLE].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_PLAYERIDLE, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_STOP_PLAYER_IDLE].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_STOP_MISSILE_TRAVEL].SoundPlayToggle == 1)
	{
		if(!m_bPreventPlaying)
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_MISSLETRAVEL, FindMissile(triggerSoundEvents[EVENT_SOUND_MISSILE_TRAVEL].ObjectInfo.Entity));
			RemoveMissile(triggerSoundEvents[EVENT_SOUND_MISSILE_TRAVEL].ObjectInfo.Entity);
		}
		triggerSoundEvents[EVENT_SOUND_STOP_MISSILE_TRAVEL].SoundPlayToggle = 0;
	}

	////////////////////////////////////////////////////////////////////////
	// Menu Loop sounds
	////////////////////////////////////////////////////////////////////////	
	if(triggerSoundEvents[EVENT_MUSIC_PLAY_MENU].SoundPlayToggle == 1)
	{
		StopAllMusic();
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_MX_MENUMUSIC, GAME_OBJECT_ID_BGM);
		triggerSoundEvents[EVENT_MUSIC_PLAY_MENU].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_MUSIC_PLAY_LVL1].SoundPlayToggle == 1)
	{
		StopAllMusic();
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_MX_LEVELMUSIC, GAME_OBJECT_ID_BGM);
		triggerSoundEvents[EVENT_MUSIC_PLAY_LVL1].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_MUSIC_PLAY_LVL2].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_MX_PAUSE_MENU, GAME_OBJECT_ID_BGM2);
		triggerSoundEvents[EVENT_MUSIC_PLAY_LVL2].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_MUSIC_PLAY_CREDITSTHEME].SoundPlayToggle == 1)
	{
		StopAllMusic();
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_MX_CREDITS, GAME_OBJECT_ID_BGM);
		triggerSoundEvents[EVENT_MUSIC_PLAY_CREDITSTHEME].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_MUSIC_PLAY_BOSSTHEME].SoundPlayToggle == 1)
	{
		StopAllMusic();
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_MX_MENUMUSIC, GAME_OBJECT_ID_BGM);
		triggerSoundEvents[EVENT_MUSIC_PLAY_BOSSTHEME].SoundPlayToggle = 0;
	}

	////////////////////////////////////////////////////////////////////////
	// Menu Sounds
	////////////////////////////////////////////////////////////////////////
	if(triggerSoundEvents[EVENT_SOUND_MENUITEM_SWITCH].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_MENUITEMSWITCH, GAME_OBJECT_ID_BGM);
		triggerSoundEvents[EVENT_SOUND_MENUITEM_SWITCH].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_MENUITEM_CONFIRM].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_MENUITEMCONFIRM, GAME_OBJECT_ID_BGM);
		triggerSoundEvents[EVENT_SOUND_MENUITEM_CONFIRM].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_MENUITEM_CANCEL].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_MENUITEMCANCEL, GAME_OBJECT_ID_BGM);
		triggerSoundEvents[EVENT_SOUND_MENUITEM_CANCEL].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_MENUITEM_PLAY_CONFIRM].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_MENUITEMPLAYGAME, GAME_OBJECT_ID_BGM);
		triggerSoundEvents[EVENT_SOUND_MENUITEM_PLAY_CONFIRM].SoundPlayToggle = 0;
	}
		
	////////////////////////////////////////////////////////////////////////
	// Player sounds
	////////////////////////////////////////////////////////////////////////
	if(triggerSoundEvents[EVENT_SOUND_PLAY_PLAYER_IDLE].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_PLAYERIDLE, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_PLAY_PLAYER_IDLE].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_PLAY_PLAYER_THRUST].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_PLAYERTHRUST, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_PLAY_PLAYER_THRUST].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_PLAYER_DODGE].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_PLAYERDODGE, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_PLAYER_DODGE].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_PLAYER_HYPERSPACE].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_HYPERSPACE, GAME_OBJECT_ID_PLAYER);	
		triggerSoundEvents[EVENT_SOUND_PLAYER_HYPERSPACE].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_PLAYER_MODECHANGE].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_MODECHANGE, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_PLAYER_MODECHANGE].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_PLAYER_FIRE_LASER].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_PLAYERLASERFIRE, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_PLAYER_FIRE_LASER].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_PLAYER_FIRE_MISSILE].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_ENEMYMISSLEFIRE , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_PLAYER_FIRE_MISSILE].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_PLAYER_SHIELD_HURT].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_HYDRASHIELDHIT, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_PLAYER_SHIELD_HURT].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_PLAYER_HULL_HIT].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_PLAYERHULLHIT, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_PLAYER_HULL_HIT].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_PLAYER_SHIELD_RECHARGE].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_SHIELDRECHARGE, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_PLAYER_SHIELD_RECHARGE].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_PLAYER_SHIELD_FULL].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_SHEILDFULL, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_PLAYER_SHIELD_FULL].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_PLAYER_HEALTH_RECHARGE].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_PLAYERFULLHEALTH, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_PLAYER_HEALTH_RECHARGE].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_PLAYER_SHIELD_BROKE].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_PLAYERSHEILDBROKEN, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_PLAYER_SHIELD_BROKE].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_PLAYER_HEALTH_FULL].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_PLAYERFULLHEALTH, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_PLAYER_HEALTH_FULL].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_PLAYER_LOCK_ON_WARNING].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_LOCKONWARNING, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_PLAYER_LOCK_ON_WARNING].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_PLAYER_LOW_SHIELDS].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_PLAYERLOWSHIELDS, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_PLAYER_LOW_SHIELDS].SoundPlayToggle = 0;
	}
	
	if(triggerSoundEvents[EVENT_SOUND_PLAYER_LOW_HEALTH].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_PLAYERLOWHEALTHWARNING, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_PLAYER_LOW_HEALTH].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_PLAYER_DEATH].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_PLAYERDEATH, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_PLAYER_DEATH].SoundPlayToggle = 0;
	}
	
	if(triggerSoundEvents[EVENT_SOUND_MISSILE_TRAVEL].SoundPlayToggle == 1)
	{
		if(!m_bPreventPlaying)
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_MISSLETRAVEL, FindMissile(triggerSoundEvents[EVENT_SOUND_MISSILE_TRAVEL].ObjectInfo.Entity));
		triggerSoundEvents[EVENT_SOUND_MISSILE_TRAVEL].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_MISSILE_LOCK_ON].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_MISSLELOCKON, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_MISSILE_LOCK_ON].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_MISSILE_EXPLOSION].SoundPlayToggle == 1)
	{
		if(!m_bPreventPlaying)
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_MISSLEEXPLOSION, FindMissile(triggerSoundEvents[EVENT_SOUND_MISSILE_EXPLOSION].ObjectInfo.Entity));
		triggerSoundEvents[EVENT_SOUND_MISSILE_EXPLOSION].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_MISSILE_RELOAD].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_MISSLERELOAD, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_MISSILE_RELOAD].SoundPlayToggle = 0;
	}

	////////////////////////////////////////////////////////////////////////
	// Enemy sounds
	////////////////////////////////////////////////////////////////////////

	if(triggerSoundEvents[EVENT_SOUND_ENEMY_HYDRA_LASER_CHARGE].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_HYDRALASERCHARGE, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_ENEMY_HYDRA_LASER_CHARGE].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_STOP_ENEMY_HYDRA_LASER_CHARGE].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_HYDRALASERCHARGE, GAME_OBJECT_ID_PLAYER); 
		triggerSoundEvents[EVENT_SOUND_STOP_ENEMY_HYDRA_LASER_CHARGE].SoundPlayToggle = 0;
	}
	
	if(triggerSoundEvents[EVENT_SOUND_ENEMY_HYDRA_LASER].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_HYDRALASERFIRE, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_ENEMY_HYDRA_LASER].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_ENEMY_HYDRA_STOP_LASER].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_HYDRALASERFIRE, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_ENEMY_HYDRA_STOP_LASER].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_ENEMY_HYDRA_PEASHOOTER].SoundPlayToggle == 1)
	{
		if(!m_bPreventPlaying)
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_HYDRALASERPEASHOOTER, FindObject(triggerSoundEvents[EVENT_SOUND_ENEMY_HYDRA_PEASHOOTER].ObjectInfo.Entity));
		//AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_HYDRALASERPEASHOOTER, GAME_OBJECT_ID_BOSS);		
		triggerSoundEvents[EVENT_SOUND_ENEMY_HYDRA_PEASHOOTER].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_ENEMY_HYDRA_SHEILD_GEN].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_HYDRASHEILDGENDOWN, GAME_OBJECT_ID_BOSS);
		triggerSoundEvents[EVENT_SOUND_ENEMY_HYDRA_SHEILD_GEN].SoundPlayToggle = 0;
	}
	
	if(triggerSoundEvents[EVENT_SOUND_ENEMY_HYDRA_SHEILD_HIT].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_HYDRASHIELDHIT, GAME_OBJECT_ID_BOSS);
		triggerSoundEvents[EVENT_SOUND_ENEMY_HYDRA_SHEILD_HIT].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_ENEMY_HYDRA_ENTRANCE].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_BOSSENTRANCE, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_ENEMY_HYDRA_ENTRANCE].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_ENEMY_HYDRA_TIMEROUT].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_HYDRATIMEROUT, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_ENEMY_HYDRA_TIMEROUT].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_ENEMY_HYDRA_PLANETDESTROYED].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_HYDRAPLANETDESTROYED, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_ENEMY_HYDRA_PLANETDESTROYED].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_PLAY_HYDRA_THRUSTERS].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_HYPERSPACE, GAME_OBJECT_ID_BOSS);
		triggerSoundEvents[EVENT_SOUND_PLAY_HYDRA_THRUSTERS].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_ENEMY_THRUST].SoundPlayToggle == 1)
	{
		if(!m_bPreventPlaying)
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_ENEMYTHRUST, FindObject(triggerSoundEvents[EVENT_SOUND_ENEMY_THRUST].ObjectInfo.Entity));
		triggerSoundEvents[EVENT_SOUND_ENEMY_THRUST].SoundPlayToggle = 0;
	}
	
	if(triggerSoundEvents[EVENT_SOUND_ENEMY_EXPLOSION].SoundPlayToggle == 1)
	{
		if(!m_bPreventPlaying)
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_ENEMYEXPLOSION, FindObject(triggerSoundEvents[EVENT_SOUND_ENEMY_EXPLOSION].ObjectInfo.Entity));
		triggerSoundEvents[EVENT_SOUND_ENEMY_EXPLOSION].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_ENEMY_FIRE_LASER].SoundPlayToggle == 1)
	{
		if(!m_bPreventPlaying)
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_ENEMYLASERFIRE, FindObject(triggerSoundEvents[EVENT_SOUND_ENEMY_FIRE_LASER].ObjectInfo.Entity));
		triggerSoundEvents[EVENT_SOUND_ENEMY_FIRE_LASER].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_ENEMY_FIRE_MISSILE].SoundPlayToggle == 1)
	{
		if(!m_bPreventPlaying)
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_ENEMYMISSLEFIRE, FindObject(triggerSoundEvents[EVENT_SOUND_ENEMY_FIRE_MISSILE].ObjectInfo.Entity));
		triggerSoundEvents[EVENT_SOUND_ENEMY_FIRE_MISSILE].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_ENEMY_LASER_HIT_METAL].SoundPlayToggle == 1)
	{
		if(!m_bPreventPlaying)
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_LASERHITMETAL, FindObject(triggerSoundEvents[EVENT_SOUND_ENEMY_LASER_HIT_METAL].ObjectInfo.Entity));
		triggerSoundEvents[EVENT_SOUND_ENEMY_LASER_HIT_METAL].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_ENEMY_LASER_HIT_ROCK].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_LASERHITROCK, GAME_OBJECT_ID_ASTEROID);
		triggerSoundEvents[EVENT_SOUND_ENEMY_LASER_HIT_ROCK].SoundPlayToggle = 0;
	}

	//if(triggerSoundEvents[EVENT_SOUND_ENEMY_LASER_HIT_SHEILD].SoundPlayToggle == 1)
	//{
	//	AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_2D_UNDERWATER_LOOP, FindObject(triggerSoundEvents[EVENT_SOUND_ENEMY_LASER_HIT_METAL].ObjectInfo.Entity));
	//	triggerSoundEvents[EVENT_SOUND_ENEMY_LASER_HIT_SHEILD].SoundPlayToggle = 0;
	//}
	
	//////////////////////////////////////		
	// Dialogue sounds // if we need them
	//////////////////////////////////////
#pragma region DialogEvents
	if(triggerSoundEvents[EVENT_SOUND_AI_MOVE_MOUSE_PLAY].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_TUTORIAL_MOVEMOUSE , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_MOVE_MOUSE_PLAY].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_MOVE_MOUSE_STOP].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_TUTORIAL_MOVEMOUSE , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_MOVE_MOUSE_STOP].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_FIREWEAPON_PLAY].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_TUTORIAL_FIREWEAPON , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_FIREWEAPON_PLAY].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_FIREWEAPON_STOP].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_TUTORIAL_FIREWEAPON , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_FIREWEAPON_STOP].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_SPEEDUPSLOWDOWN_PLAY].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_TUROTIAL_SPEEDUPSLOWDOWN , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_SPEEDUPSLOWDOWN_PLAY].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_SPEEDUPSLOWDOWN_STOP].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_TUROTIAL_SPEEDUPSLOWDOWN, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_SPEEDUPSLOWDOWN_STOP].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_GREATJOB_PLAY].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_TUTORIAL_GREATJOB , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_GREATJOB_PLAY].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_GREATJOB_STOP].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_TUTORIAL_GREATJOB , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_GREATJOB_STOP].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_DODGEROLL_PLAY].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_TUTORIAL_DODGEROLL, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_DODGEROLL_PLAY].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_DODGEROLL_STOP].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_TUTORIAL_DODGEROLL , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_DODGEROLL_STOP].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_DODGEROLLCOOLDOWN_PLAY].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_TUTORIAL_DODGEROLLCOOLDOWN , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_DODGEROLLCOOLDOWN_PLAY].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_DODGEROLLCOOLDOWN_STOP].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_TUTORIAL_DODGEROLLCOOLDOWN , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_DODGEROLLCOOLDOWN_STOP].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_SWITCHMODES_PLAY].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_TUTORIAL_SWITCHMODES , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_SWITCHMODES_PLAY].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_SWITCHMODES_STOP].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_TUTORIAL_SWITCHMODES , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_SWITCHMODES_STOP].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_MODEWEAPON_PLAY].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_TUTORIAL_MODEWEAPON , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_MODEWEAPON_PLAY].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_MODEWEAPON_STOP].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_TUTORIAL_MODEWEAPON , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_MODEWEAPON_STOP].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_BLUEMODEREGENERATE_PLAY].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_TUTORIAL_BLUEMODEREGENERATE, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_BLUEMODEREGENERATE_PLAY].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_BLUEMODEREGENERATE_STOP].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_TUTORIAL_BLUEMODEREGENERATE , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_BLUEMODEREGENERATE_STOP].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_QUICKTURN_PLAY].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_TUTORIAL_QUICKTURN_01 , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_QUICKTURN_PLAY].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_QUICKTURN_STOP].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_TUTORIAL_QUICKTURN , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_QUICKTURN_STOP].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_QUICKTURNREPOSITION_PLAY].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_TUTORIAL_QUICKTURNREPOSITION , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_QUICKTURNREPOSITION_PLAY].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_QUICKTURNREPOSITION_STOP].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_TUTORIAL_QUICKTURNREPOSITION , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_QUICKTURNREPOSITION_STOP].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_LOOKANENEMY_PLAY].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_TUTORIAL_LOOKANENEMY , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_LOOKANENEMY_PLAY].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_LOOKANENEMY_STOP].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_TUTORIAL_LOOKANENEMY , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_LOOKANENEMY_STOP].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_SEEKERENTRANCE_PLAY].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_TUTORIAL_SEEKERENTRANCE , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_SEEKERENTRANCE_PLAY].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_SEEKERENTRANCE_STOP].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_TUTORIAL_SEEKERENTRANCE, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_SEEKERENTRANCE_STOP].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_CRUISERENTRANCE_PLAY].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_TUTORIAL_CRUISERENTRANCE, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_CRUISERENTRANCE_PLAY].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_CRUISERENTRANCE_STOP].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_TUTORIAL_CRUISERENTRANCE , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_CRUISERENTRANCE_STOP].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_MARAUDERENTRANCE_PLAY].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_TUTORIAL_MARAUDERENTRANCE , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_MARAUDERENTRANCE_PLAY].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_MARAUDERENTRANCE_STOP].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_TUTORIAL_MARAUDERENTRANCE, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_MARAUDERENTRANCE_STOP].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_HYDRAENTRANCE_PLAY].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_TUTORIAL_HYDRAENTRANCE , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_HYDRAENTRANCE_PLAY].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_HYDRAENTRANCE_STOP].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_TUTORIAL_HYDRAENTRANCE , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_HYDRAENTRANCE_STOP].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_HYDRASHIELDENTRANCE_PLAY].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_TUTORIAL_HYDRASHIELDENTRANCE , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_HYDRASHIELDENTRANCE_PLAY].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_HYDRASHIELDENTRANCE_STOP].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_TUTORIAL_HYDRASHIELDENTRANCE , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_HYDRASHIELDENTRANCE_STOP].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_HYDRATURRETENTRANCE_PLAY].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_TUTORIAL_HYDRATURRETENTRANCE , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_HYDRATURRETENTRANCE_PLAY].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_HYDRATURRETENTRANCE_STOP].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_TUTORIAL_HYDRATURRETENTRANCE , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_HYDRATURRETENTRANCE_STOP].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_HYDRAMEGALASERENTRANCE_PLAY].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_TUTORIAL_HYDRAMEGALASERENTRANCE , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_HYDRAMEGALASERENTRANCE_PLAY].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_HYDRAMEGALASERENTRANCE_STOP].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_TUTORIAL_HYDRAMEGALASERENTRANCE , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_HYDRAMEGALASERENTRANCE_STOP].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_LONGVICTORY_PLAY].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_TUTORIAL_LONGVICTORY , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_LONGVICTORY_PLAY].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_LONGVICTORY_STOP].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_TUTORIAL_LONGVICTORY, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_LONGVICTORY_STOP].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_QUICKVICTORY_PLAY].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_TUTORIAL_QUICKVICTORY, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_QUICKVICTORY_PLAY].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_QUICKVICTORY_STOP].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_TUTORIAL_QUICKVICTORY , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_QUICKVICTORY_STOP].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_TIMERRANOUT_PLAY].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_TUTORIAL_TIMERRANOUT , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_TIMERRANOUT_PLAY].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_TIMERRANOUT_STOP].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_TUTORIAL_TIMERRANOUT , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_TIMERRANOUT_STOP].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_PLAYERCRITICALHEALTH_PLAY].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_TUTORIAL_PLAYERCRITICALHEALTH , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_PLAYERCRITICALHEALTH_PLAY].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_PLAYERCRITICALHEALTH_STOP].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_TUTORIAL_PLAYERCRITICALHEALTH , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_PLAYERCRITICALHEALTH_STOP].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_PLAYERSHIELDDOWN_PLAY].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_TUTORIAL_PLAYERSHIELDSDOWN , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_PLAYERSHIELDDOWN_PLAY].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_PLAYERSHIELDDOWN_STOP].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_TUTORIAL_PLAYERSHIELDSDOWN , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_PLAYERSHIELDDOWN_STOP].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_WAVECOMPLETE_PLAY].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_TUTORIAL_WAVECOMPLETE , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_WAVECOMPLETE_PLAY].SoundPlayToggle = 0;
	}
	if(triggerSoundEvents[EVENT_SOUND_AI_INCOMMINGWAVE_PLAY].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_TUTORIAL_INCOMINGWAVE , GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_AI_INCOMMINGWAVE_PLAY].SoundPlayToggle = 0;
	}
		
	
#pragma endregion

	//////////////////////////////////////		
	// Other sounds
	//////////////////////////////////////
	if(triggerSoundEvents[EVENT_SOUND_ASTEROID_EXPLOSION].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_ASTEROIDEXPLOSION, GAME_OBJECT_ID_ASTEROID);
		triggerSoundEvents[EVENT_SOUND_ASTEROID_EXPLOSION].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_HUD_BOUNDS].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DX_OUTOFBOUNDS, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_HUD_BOUNDS].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_HUD_PROCEED].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_TUTORIAL_PROCEEDTOWAYPOINT, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_HUD_PROCEED].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_HUD_ONYOURSIX].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DX_ONYOURSIX, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_HUD_ONYOURSIX].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_HUD_CHECKPOINT].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_CHECKPOINT, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_HUD_CHECKPOINT].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_HUD_GOODJOB].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DX_GOODJOB, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_HUD_GOODJOB].SoundPlayToggle = 0;
	}

	if(triggerSoundEvents[EVENT_SOUND_HUD_COUNTDOWN].SoundPlayToggle == 1)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_COUNTDOWN, GAME_OBJECT_ID_PLAYER);
		triggerSoundEvents[EVENT_SOUND_HUD_COUNTDOWN].SoundPlayToggle = 0;
	}

	// you can specify a game object id in order to stop all sounds associated with that individual object.
	if(triggerSoundEvents[EVENT_KILL_ALL].SoundPlayToggle == 1)
	{
		AK::SoundEngine::StopAll();
	}
}

// Should pass in player position to update the listener position
bool WwiseNS::Update()
{
	UpdateGame();
	ProcessAudio();	
	HandleEvents();
	return true;
}

void WwiseNS::SetVolumeRTPC(AkRtpcID setID, float fVol)
{
	AK::SoundEngine::SetRTPCValue(setID, fVol);
}

void WwiseNS::UpdateGame()
{
	UpdateListenerPos();	// Listener's position
	UpdatePlayer();
	//TODO: may need this for individual objects
	UpdateGameObjects();	// All the objects
}

void WwiseNS::SetPreventPlaying(bool _Prevent) 
{
	m_bPreventPlaying = _Prevent;
}
bool WwiseNS::GetPreventPlaying()
{
	return m_bPreventPlaying;
}
//void WwiseNS::SetPlayerPos(float fX, float fY, float fZ)
//{
//	listener.Position.X = soundPos.Position.X = playerPos.Position.X = fX/10;
//	listener.Position.Y = soundPos.Position.Y = playerPos.Position.Y = fY/10;
//	listener.Position.Z = soundPos.Position.Z = playerPos.Position.Z = fZ/10;
//	listener.Position.Z = playerPos.Position.Z + 2;
//}
//void WwiseNS::SetPlayerPos(D3DXVECTOR3 _SetPos)
//{
//	listener.Position.X = soundPos.Position.X = playerPos.Position.X = _SetPos.x/10;
//	listener.Position.Y = soundPos.Position.Y = playerPos.Position.Y = _SetPos.y/10;
//	listener.Position.Z = soundPos.Position.Z = playerPos.Position.Z = _SetPos.z/10;
//	listener.Position.Z = playerPos.Position.Z + 2;
//}
void WwiseNS::SetPlayerPos(D3DXMATRIX _SetMat)
{
	D3DXVECTOR3 temp, temp2;
	temp.x = _SetMat._31;
	temp.y = _SetMat._32;
	temp.z = _SetMat._33;

	D3DXVec3Normalize(&temp2,&temp);

	playerPos.Orientation.X = temp2.x;
	playerPos.Orientation.Y = temp2.y;
	playerPos.Orientation.Z = temp2.z;

	playerPos.Position.X = _SetMat._41/10;
	playerPos.Position.Y = _SetMat._42/10;
	playerPos.Position.Z = _SetMat._43/10;
}
void WwiseNS::SetListenerPos(D3DXMATRIX _SetMat)
{
	listener.OrientationFront.X = _SetMat._31;
	listener.OrientationFront.Y = _SetMat._32;
	listener.OrientationFront.Z = _SetMat._33;

	listener.OrientationTop.X = _SetMat._21;
	listener.OrientationTop.Y = _SetMat._22;
	listener.OrientationTop.Z = _SetMat._23;

	listener.Position.X = _SetMat._41/10;
	listener.Position.Y = _SetMat._42/10;
	listener.Position.Z = _SetMat._43/10;
}

void WwiseNS::SetEnemyPos(CEntity* _Enemy)
{
}
void WwiseNS::SetBossPos(D3DXMATRIX _SetMat)
{
	D3DXVECTOR3 temp, temp2;
	temp.x = _SetMat._31;
	temp.y = _SetMat._32;
	temp.z = _SetMat._33;

	D3DXVec3Normalize(&temp2, &temp);

	BossPos.Orientation.X = temp2.x;
	BossPos.Orientation.Y = temp2.y;
	BossPos.Orientation.Z = temp2.z;

	BossPos.Position.X = _SetMat._41;
	BossPos.Position.Y = _SetMat._42;
	BossPos.Position.Z = _SetMat._43;
}
void WwiseNS::SetBossPos(D3DXVECTOR3 _SetPos)
{
	BossPos.Orientation.X = 1;
	BossPos.Orientation.Y = 0;
	BossPos.Orientation.Z = 0;

	BossPos.Position.X = _SetPos.x;
	BossPos.Position.Y = _SetPos.y;
	BossPos.Position.Z = _SetPos.z;
}

void WwiseNS::UpdateListenerPos()
{
	AK::SoundEngine::SetListenerPosition(listener);
}
void WwiseNS::UpdateGameObjects()
{
	soundPos.Position.X = (BossPos.Position.X/10);
	soundPos.Position.Y = (BossPos.Position.Y/10);
	soundPos.Position.Z = (BossPos.Position.Z/10);

	soundPos.Orientation.X = (playerPos.Position.X - BossPos.Position.X);
	soundPos.Orientation.Y = (playerPos.Position.Y - BossPos.Position.Y);
	soundPos.Orientation.Z = (playerPos.Position.Z - BossPos.Position.Z);

	SetGameObjPos(GAME_OBJECT_ID_BOSS, soundPos);
	//SetGameObjPos(GAME_OBJECT_ID_BOSS, playerPos);
	
	soundPos.Position.X = (AsteroidPos.Position.X/10);
	soundPos.Position.Y = (AsteroidPos.Position.Y/10);
	soundPos.Position.Z = (AsteroidPos.Position.Z/10);

	soundPos.Orientation.X = (playerPos.Position.X - AsteroidPos.Position.X);
	soundPos.Orientation.Y = (playerPos.Position.Y - AsteroidPos.Position.Y);
	soundPos.Orientation.Z = (playerPos.Position.Z - AsteroidPos.Position.Z);

	if(soundPos.Position.X >= 0 || soundPos.Position.X < 0)
		SetGameObjPos(GAME_OBJECT_ID_ASTEROID, soundPos);
	else
	{
		// Debug. If it gets in here ever again. . . . debug it.
		SetGameObjPos(GAME_OBJECT_ID_ASTEROID, soundPos);
	}

	for(int SoundObjs = 0; SoundObjs < NUM_WWISE_ENEMIES; SoundObjs++)
	{
		CEntity* tempEnt = GAME_OBJECT_ID_ENEMIES[SoundObjs].Entity;
		if(tempEnt != nullptr)
		{
			D3DXVECTOR3 EntityPos = tempEnt->GetPosition();
			soundPos.Position.X = (EntityPos.x/10);
			soundPos.Position.Y = (EntityPos.y/10);
			soundPos.Position.Z = (EntityPos.z/10);

			soundPos.Orientation.X = (playerPos.Position.X - EntityPos.x);
			soundPos.Orientation.Y = (playerPos.Position.Y - EntityPos.y);
			soundPos.Orientation.Z = (playerPos.Position.Z - EntityPos.z);
			SetGameObjPos(GAME_OBJECT_ID_ENEMIES[SoundObjs].SoundObject, soundPos);
						
			if(tempEnt->GetActive() == false)
			{
				RemoveObject(tempEnt);
			}
		}
	}

	for(int SoundObjs = 0; SoundObjs < NUM_WWISE_MISSILES; SoundObjs++)
	{
		CEntity* tmpMissile = GAME_OBJECT_ID_MISSILES[SoundObjs].Entity;
		if(tmpMissile != nullptr)
		{
			if(tmpMissile->GetActive() == false)
			{
				AK::SoundEngine::PostEvent(AK::EVENTS::STOP_FX_MISSLETRAVEL, GAME_OBJECT_ID_MISSILES[SoundObjs].SoundObject);
				RemoveMissile(tmpMissile);
			}
			else
			{
				D3DXVECTOR3 MissilePos = tmpMissile->GetPosition();
				soundPos.Position.X = (MissilePos.x/10);
				soundPos.Position.Y = (MissilePos.y/10);
				soundPos.Position.Z = (MissilePos.z/10);

				soundPos.Orientation.X = (playerPos.Position.X - MissilePos.x);
				soundPos.Orientation.Y = (playerPos.Position.Y - MissilePos.y);
				soundPos.Orientation.Z = (playerPos.Position.Z - MissilePos.z);

				SetGameObjPos(GAME_OBJECT_ID_MISSILES[SoundObjs].SoundObject, soundPos);
			}
		}
	}
}


void WwiseNS::SetGameObjPos(AkGameObjectID gameObjectID, AkSoundPosition soundPosition)
{
	AK::SoundEngine::SetPosition(gameObjectID, soundPosition); ///< If the listener index is valid, the listener position is used instead of the game object position.
}

void WwiseNS::RemoveAllSoundObjs()
{
	m_bPreventPlaying = false;
	for(int SoundObjs = 0; SoundObjs < NUM_WWISE_ENEMIES; SoundObjs++)
	{
		if(GAME_OBJECT_ID_ENEMIES[SoundObjs].Entity != nullptr)
		{
			//GAME_OBJECT_ID_ENEMIES[SoundObjs].SoundObject = -1;
			GAME_OBJECT_ID_ENEMIES[SoundObjs].Entity = nullptr;
		}
	}
	for(int SoundObjs = 0; SoundObjs < NUM_WWISE_MISSILES; SoundObjs++)
	{
		if(GAME_OBJECT_ID_MISSILES[SoundObjs].Entity != nullptr)
		{
			//GAME_OBJECT_ID_MISSILES[SoundObjs].SoundObject = -1;
			GAME_OBJECT_ID_MISSILES[SoundObjs].Entity = nullptr;
		}
	}
}

void WwiseNS::UpdatePlayer()
{
	//playerPos.Orientation = listener.OrientationFront;
	//playerPos.Position = listener.Position;
	//SetGameObjPos(GAME_OBJECT_ID_BOSS, playerPos);
	SetGameObjPos(GAME_OBJECT_ID_PLAYER, playerPos);
	SetGameObjPos(GAME_OBJECT_ID_DEFAULT_ENEMY, playerPos);
}
int WwiseNS::FindMissile(CEntity* _Missile)
{
	if(_Missile == nullptr)
		return GAME_OBJECT_ID_DEFAULT_ENEMY;

	for(int numObjects = 0; numObjects < NUM_WWISE_MISSILES; numObjects++)
	{
		if(GAME_OBJECT_ID_MISSILES[numObjects].Entity != nullptr)
		{
			if(GAME_OBJECT_ID_MISSILES[numObjects].Entity == _Missile)
			{
				//cout << "Missile " << numObjects << " Found";
				return GAME_OBJECT_ID_MISSILES[numObjects].SoundObject;
			}
			//else keep looping for one
		}
		else
		{
			GAME_OBJECT_ID_MISSILES[numObjects].Entity = _Missile;
			//cout << "Missile " << numObjects << " Added";
			return GAME_OBJECT_ID_MISSILES[numObjects].SoundObject;
		}
	}
	return GAME_OBJECT_ID_DEFAULT_ENEMY;
}

int WwiseNS::FindObject(CEntity* _Entity)
{
	if(_Entity == nullptr)
		return GAME_OBJECT_ID_DEFAULT_ENEMY;

	int EntityType = _Entity->GetType();
	// if it's not the player, find out if there is already an object for it.
	if(EntityType != eRED_PLAYER && EntityType != eBLUE_PLAYER && EntityType != eYELLOW_PLAYER)
	{
		for(int numObjects = 0; numObjects < NUM_WWISE_ENEMIES; numObjects++)
		{
			if(GAME_OBJECT_ID_ENEMIES[numObjects].Entity != nullptr)
			{
				if(GAME_OBJECT_ID_ENEMIES[numObjects].Entity == _Entity)
					return GAME_OBJECT_ID_ENEMIES[numObjects].SoundObject;
			}
			else
			{
				GAME_OBJECT_ID_ENEMIES[numObjects].Entity = _Entity;
				return GAME_OBJECT_ID_ENEMIES[numObjects].SoundObject;
			}
		}
		return GAME_OBJECT_ID_DEFAULT_ENEMY;
	}
	else
		return GAME_OBJECT_ID_DEFAULT_ENEMY;
}
void WwiseNS::RemoveObject(CEntity* _Entity)
{
	if(_Entity == nullptr)
		return;

	for(int numObjects = 0; numObjects < NUM_WWISE_ENEMIES; numObjects++)
	{
		if(GAME_OBJECT_ID_ENEMIES[numObjects].Entity == _Entity)
		{
			//GAME_OBJECT_ID_ENEMIES[numObjects].SoundObject = -1;
			GAME_OBJECT_ID_ENEMIES[numObjects].Entity = nullptr;
			return;
		}
	}
}
void WwiseNS::RemoveMissile(CEntity* _Missile)
{
	if(_Missile == nullptr)
		return;

	for(int numObjects = 0; numObjects < NUM_WWISE_MISSILES; numObjects++)
	{
		if(GAME_OBJECT_ID_MISSILES[numObjects].Entity == _Missile)
		{
			//cout << "Missile " << numObjects << "Removed"; 
			//GAME_OBJECT_ID_MISSILES[numObjects].SoundObject = -1;
			GAME_OBJECT_ID_MISSILES[numObjects].Entity = nullptr;
			return;
		}
	}
}
void WwiseNS::StopAllTutorialVoices(int _enumOfSoundToSkip)
{
	if(eAIMoveMouse != _enumOfSoundToSkip)
		SoundEvent(eAIMoveMouse, eStop);
	if(eAIFireWeapon != _enumOfSoundToSkip)
		SoundEvent(eAIFireWeapon, eStop);
	if(eAISpeedUpSlowDown != _enumOfSoundToSkip)
		SoundEvent(eAISpeedUpSlowDown, eStop);
	if(eAIGreatJob != _enumOfSoundToSkip)
		SoundEvent(eAIGreatJob, eStop);
	if(eAIDodgeRoll != _enumOfSoundToSkip)
		SoundEvent(eAIDodgeRoll, eStop);
	if(eAIDodgeRollCooldown != _enumOfSoundToSkip)
		SoundEvent(eAIDodgeRollCooldown, eStop);
	if(eAISwitchModes != _enumOfSoundToSkip)
		SoundEvent(eAISwitchModes, eStop);
	if(eAIModeWeapon != _enumOfSoundToSkip)
		SoundEvent(eAIModeWeapon, eStop);
	if(eAIBlueModeRegenerate != _enumOfSoundToSkip)
		SoundEvent(eAIBlueModeRegenerate, eStop);
	if(eAIQuickTurn != _enumOfSoundToSkip)
		SoundEvent(eAIQuickTurn, eStop);
	if(eAIQuickTurnReposition != _enumOfSoundToSkip)
		SoundEvent(eAIQuickTurnReposition, eStop);
	if(eAILookAnEnemy != _enumOfSoundToSkip)
		SoundEvent(eAILookAnEnemy, eStop);
	if(eAISeekerEntrance != _enumOfSoundToSkip)
		SoundEvent(eAISeekerEntrance, eStop);
	if(eAICruiserEntrance != _enumOfSoundToSkip)
		SoundEvent(eAICruiserEntrance, eStop);
	if(eAIMarauderEntrance != _enumOfSoundToSkip)
		SoundEvent(eAIMarauderEntrance, eStop);
	if(eAIHydraEntrance != _enumOfSoundToSkip)
		SoundEvent(eAIHydraEntrance, eStop);
	if(eAIHydraShieldEntrance != _enumOfSoundToSkip)
		SoundEvent(eAIHydraShieldEntrance, eStop);
	if(eAIHydraTurretEntrance != _enumOfSoundToSkip)
		SoundEvent(eAIHydraTurretEntrance, eStop);
	if(eAILongVictory != _enumOfSoundToSkip)
		SoundEvent(eAILongVictory, eStop);
	if(eAIQuickVictory != _enumOfSoundToSkip)
		SoundEvent(eAIQuickVictory, eStop);
	if(eAITimerRanOut != _enumOfSoundToSkip)
		SoundEvent(eAITimerRanOut, eStop);
	if(eAIPlayerCriticalHealth != _enumOfSoundToSkip)
		SoundEvent(eAIPlayerCriticalHealth, eStop);
	if(eAIPlayerShieldsDown != _enumOfSoundToSkip)
		SoundEvent(eAIPlayerShieldsDown, eStop);
	if(eAIWaveComplete != _enumOfSoundToSkip)
		SoundEvent(eAIWaveComplete, eStop);
	if(eAIIncommingWave != _enumOfSoundToSkip)
		SoundEvent(eAIIncommingWave, eStop);

}