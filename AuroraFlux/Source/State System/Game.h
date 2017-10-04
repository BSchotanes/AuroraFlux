/***********************************************
* Filename:  		Game.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This is the where the game starts and plays.
************************************************/
#ifndef	GAME_H
#define GAME_H

#include "../StdAfx.h"
#include "../Renderer/CRenderer.h"
#include "../Camera/Camera.h"

#include "../Timer/CTimer.h"
#include "IGameState.h"
#include "MainMenuState.h"
#include "../Dialog System/Dialog System.h"
#include "GuiHelper.h"

#define DEBUG_OUTPUT(str) cout << str << endl

struct PlayerCheckPoint
{
	unsigned int m_unCurrEvent;
	PlayerCheckPoint() : m_unCurrEvent(0)  {}
};

struct StatTrack
{
	int m_nLasersFired;
	int m_nLasersHit;
	int m_nMissilesFired;
	int m_nMissilesHit;
	int m_nMissilesDodged;
	int m_nDamageTaken;
	int m_nDeaths;

	//StatTrack& operator=(const StatTrack& OtherStats);
	int LaserFired() {return m_nLasersFired++;}
	int LaserHit() {return m_nLasersHit++;}
	int MissileFired(){return m_nMissilesFired++;}
	int MissileHit() {return m_nMissilesHit++;}
	int SlowDodged(){return m_nMissilesDodged++;}
	int IncreaseDamageTaken(int _nDamage) {return (m_nDamageTaken += _nDamage);}
	int DiedStats(){return m_nDeaths++;}

	void InitStats();
};

class CGame
{
private: 
	HWND				m_hWnd;

	// Screen Info:
	float				m_fScreenWidth;
	float				m_fScreenHeight;
	bool				m_bIsWindowed;
	POINT				m_tPoint;
	POINT				m_tCursorPosition;
	PlayerCheckPoint	m_tPlayerCheckPoint;
	bool				m_bLoadingDone;
	bool				m_bStartMainMenu;
	bool				m_bLockCursorToScreen;
	float				m_fMX_Volume;
	float				m_fFX_Volume;
	float				m_fDX_Volume;

	CTimer				m_cTimer;
	//The currently focused state. 
	IGameState*			m_pCurrState;
	// All active states.
	vector<IGameState*> m_vStates;

	CGuiHelper* m_pGuiHelper;

	int m_nMenuTextureID;
	int m_nPauseTextureScreens;
	int m_nGameOverTextures;
	int m_nVictoryTextures;
	int m_nConfirmationTextures;
	int m_nLoadingTextures[7];
	int m_nOptionsTextures;
	int m_nButtonTexture;
	int m_nCreditTexture;
	int m_nMenuCursorTexture;
	int m_nAruroraFluxLogo;
	int m_nAruroraFluxLogoSmall;

	int m_nGreenArrowText;
	int m_nGreenBoxText;
	int m_nRedArrowText;
	int m_nRedBoxText;
	int m_nBlueArrowText;
	int m_nBlueBoxText;
	int m_nYellowArrowText;
	int m_nYellowBoxText;
	int m_nHUDHealthIcon;
	int m_nHUDShieldIcon;
	int m_nBlueEnemyShieldIcon;
	int m_nCurrentResolution;
	float m_fPrevGamma; 
	int m_nBlackTexture;
	
	int m_nArrowText;
	int m_nEnterKeyText;
	int m_nArrowKeyLRText;
	int m_nArrow306;
	int m_nHelpTexture;
	//int 

	// The Renderer 
	CDialogSystem		m_pDialogs;
	CRenderer			m_cRenderer;
	CAssetManager*		m_pAssetManager;
	CCamera*			m_pCamera;

	// Time
	//DWORD				m_dwTime;
	unsigned int		m_unThreadID;

	HANDLE				m_hThreadHandle;
	CRITICAL_SECTION	m_CS;

	float				m_fDT;
	bool				m_bSeenYellowEnemy,
		m_bSeenRedEnemy,
		m_bSeenBlueEnemy,	
		m_bBeenInGeneratorPhase,
		m_bBeenInLaserPhase,
		m_bBeenInBigLaserPhase;

	/*float m_fApplyVolume;
	float m_fApplySFX;
	float m_fApplyGamma;
	float m_fApplyYaw;
	float m_fApplyPitch;
	bool m_bApplyInvertY;
	bool m_bApplyInvertX;

	bool m_bCancelChanges;
	float m_fPrevVolume;
	float m_fPrevSFX;
	float m_fPrevGamma;
	float m_fPrevYaw;
	float m_fPrevPitch;
	bool m_bPrevInvertY;
	bool m_bPrevInvertX;

	bool m_bRevertToDefault;
	float m_fDefaultVolume;
	float m_fDefaultSFX;
	float m_fDefaultGamma;
	float m_fDefaultYaw;
	float m_fDefaultPitch;
	bool m_bDefaultInvertY;
	bool m_bDefaultInvertX;*/

	StatTrack m_tSavedStats;
	StatTrack m_tCurrentStats;

	bool m_bOptionsApply;
	bool m_bOptionsDefault;
	bool m_bOptionsCancel;
	bool m_bFromMainMenu;
	bool m_bResolutionChanged;

	int m_nPSTCTexture;
public:
	/*****************************************************************
	* CGame(): Default Constructor will set up the window and the state.
	* Ins: None    		      
	* Outs:	None
	* Returns: None
	* Mod. Date: 11/29/2012
	* Mod. Initials: AR
	*****************************************************************/
	CGame(void);
	//Destructor
	~CGame(void);

	bool Input(void);
	void Update(void);
	void Render(void);

	bool Init(HWND _hWnd, HINSTANCE _hInstance, float _fScreenWidth, float _fScreenHeight, bool _bIsWindowed);
	bool Main(void);
	void Shutdown(void);

	void SetGamma(float _Gamma);

	//Manage states
	void CGame::PushState(IGameState *_pState);
	void CGame::PopState();
	void CGame::PopBottom();
	void CGame::switchState(IGameState *_pState);

	void ChangeResolution(float _Width,float _Height);
	
	/* ACCESSORS */
	
	StatTrack* GetCurrentStats(){return &m_tCurrentStats;}
	void SaveCurrentStats(){ m_tSavedStats = m_tCurrentStats;}
	void RevertToSavedStats(){m_tCurrentStats = m_tSavedStats;}
	void ResetCurrentStats();

	float GetWidth() const {return m_fScreenWidth;}
	float GetHeight() const {return m_fScreenHeight;}
	bool IsWindowed() const {return m_bIsWindowed;}
	//int GetPauseTexture(int _nTextureNumber) {return m_nPauseTextureScreens;}
	int GetMenuTexture(int _nTextureNumber) {return m_nMenuTextureID;}
	//int GetGameOverTexture(int _nTextureNumber) {return m_nGameOverTextures;}
	//int GetVictoryTexture(int _nTextureNumber) {return m_nVictoryTextures;}
	//int GetConfirmationTexture(int _nTextureNumber) {return m_nConfirmationTextures;}
	int GetLoadingScreenTexture(int _nTextureNumber) {return m_nLoadingTextures[_nTextureNumber];}
	bool GetLoadingDone(void){return m_bLoadingDone;}
	CDialogSystem* GetDialogSys(){return &m_pDialogs;}
	CRenderer* GetRenderer() {return &m_cRenderer;}
	CCamera* GetCamera() {return m_pCamera;}
	CTimer* GetTimer() {return &m_cTimer;}
	PlayerCheckPoint* GetCheckPoint() {return &m_tPlayerCheckPoint; }
	CRITICAL_SECTION GetCriticalSection() {return m_CS;}
	int GetStateSize() {return m_vStates.size();}
	IGameState* GetState(int _nIndicie) {return m_vStates[_nIndicie];}
	bool GetStartMainMenu() const { return m_bStartMainMenu; }
	POINT GetCursorPosition() {return m_tCursorPosition;}
	//int GetOptionsTextures(int _nTextureNumber) {return m_nOptionsTextures;}
	//int GetButtonTexture(int _nTextureNumber) {return m_nButtonTexture;}
	//int GetCreditTexture(int _nTextureNumber) {return m_nCreditTexture;}
	int GetAuroraFluxLogo() {return m_nAruroraFluxLogo;}
	int GetSmallAuroraFluxLogo() {return m_nAruroraFluxLogoSmall;}
	CGuiHelper* GetGuiHelper() {return m_pGuiHelper;}
	float GetMusicVolume() {return m_fMX_Volume;}
	float GetSFXVolume() {return m_fFX_Volume;}
	HWND GetHWND() {return m_hWnd;}
	float GetDT() {return m_fDT;}
	int GetMenuCursorTexture() {return m_nMenuCursorTexture;}
	bool GetHasSeenYellow() {return m_bSeenYellowEnemy;}
	bool GetHasSeenBlue() {return m_bSeenBlueEnemy;}
	bool GetHasSeenRed() {return m_bSeenRedEnemy;}
	bool GetHasBeenInLaserPhase() {return m_bBeenInLaserPhase;}
	bool GetHasBeenInGeneratorPhase() {return m_bBeenInGeneratorPhase;}
	bool GetHasBeenInBigLaserPhase() {return m_bBeenInBigLaserPhase;}
	int GetGreenArrowTexture() {return m_nGreenArrowText;}
	int GetGreenBoxTexture() {return m_nGreenBoxText;}
	int GetRedArrowTexture() {return m_nRedArrowText;}
	int GetRedBoxTexture() {return m_nRedBoxText;}
	int GetBlueArrowTexture() {return m_nBlueArrowText;}
	int GetBlueBoxTexture() {return m_nBlueBoxText;}
	int GetYellowArrowTexture() {return m_nYellowArrowText;}
	int GetYellowBoxTexture() {return m_nYellowBoxText;}
	int GetHUDHealthIcon() {return m_nHUDHealthIcon;}
	int GetHUDShieldIcon() {return m_nHUDShieldIcon;}
	int GetBlueEnemyShieldIcon() {return m_nBlueEnemyShieldIcon;}
	int GetCurrentResolution(void) {return m_nCurrentResolution;}
	bool GetOptionsApply() {return m_bOptionsApply;}
	bool GetOptionsCancel() {return m_bOptionsCancel;}
	bool GetOptionsDefault() {return m_bOptionsDefault;}
	int GetBlackTexture() {return m_nBlackTexture;}
	int GetArrowKeyTexture() {return m_nArrowText;}
	int GetEnterKeyTexture() {return m_nEnterKeyText;}
	int GetArrowKeyLRTexture() {return m_nArrowKeyLRText;}
	int GetArrow306Texture() {return m_nArrow306;}
	bool GetFromMainMenu() {return m_bFromMainMenu;}
	bool GetResolutionChanged() {return m_bResolutionChanged;}
	int GetPSTCTexture() {return m_nPSTCTexture;}
	int GetHelpTexture() {return m_nHelpTexture;}

	/* MUTATORS */
	void SetCursorPosition(POINT _tPos) {m_tCursorPosition = _tPos;}
	void SetCursorXPosition(LONG _lX)	{m_tCursorPosition.x = _lX;} 
	void SetCursorYPosition(LONG _lY)	{m_tCursorPosition.y = _lY;} 
	void SetCheckPoint(unsigned int _unCurrEvent );
	//void SetPauseTexture(int _nValue) {m_nPauseTextureScreens = _nValue;}
	void SetMenuTexture(int _nValue) {m_nMenuTextureID = _nValue;}
	//void SetGameOverTexture(int _nValue) {m_nGameOverTextures = _nValue;}
	//void SetVictoryTexture(int _nValue) {m_nVictoryTextures = _nValue;}
	//void SetConfirmationTextures(int _nValue) {m_nConfirmationTextures = _nValue;}
	void SetLoadingScreenTextures(int _nValue, int _nIndex) {m_nLoadingTextures[_nIndex] = _nValue;}
	void SetLoadingDone(bool _bValue) {m_bLoadingDone = _bValue;}
	void SetStartMainMenu(bool bVal) { m_bStartMainMenu = bVal; }
	//void SetOptionsTextures( int _nValue) {m_nOptionsTextures = _nValue;}
	void SetButtonTextures(int _nValue) {m_nButtonTexture = _nValue;}
	//void SetCreditTextures(int _nValue) {m_nCreditTexture = _nValue;}
	void SetMusicVolume(float _fVolume) {m_fMX_Volume = _fVolume;}
	void SetSFXVolume(float _fVolume) {m_fFX_Volume = _fVolume;}
	void SetMenuCursor(int _nValue) {m_nMenuCursorTexture = _nValue;}
	void SeenYellow(bool _seen) {m_bSeenYellowEnemy = _seen;}
	void SeenRed(bool _seen) {m_bSeenRedEnemy = _seen;}
	void SeenBlue(bool _seen) {m_bSeenBlueEnemy = _seen;}
	void SetAuroraFluxLogo(int _nValue) {m_nAruroraFluxLogo = _nValue;}
	void SetSmallAuroraFluxLogo(int _nValue) {m_nAruroraFluxLogoSmall = _nValue;}
	void SetHasBeenInLaserPhase(bool _been) {m_bBeenInLaserPhase = _been;}
	void SetHasBeenInGeneratorPhase(bool _been) {m_bBeenInGeneratorPhase = _been;}
	void SetHasBeenInBigLaserPhase(bool _been) {m_bBeenInBigLaserPhase = _been;}
	void SetGreenArrowTexture(int _nTexture) {m_nGreenArrowText = _nTexture;}
	void SetGreenBoxTexture(int _nTexture) {m_nGreenBoxText = _nTexture;}
	void SetRedArrowTexture(int _nTexture) {m_nRedArrowText = _nTexture;}
	void SetRedBoxTexture(int _nTexture) {m_nRedBoxText = _nTexture;}
	void SetBlueArrowTexture(int _nTexture) {m_nBlueArrowText = _nTexture;}
	void SetBlueBoxTexture(int _nTexture) {m_nBlueBoxText = _nTexture;}
	void SetYellowArrowTexture(int _nTexture) {m_nYellowArrowText = _nTexture;}
	void SetYellowBoxTexture(int _nTexture) {m_nYellowBoxText = _nTexture;}
	void SetHUDHealthIcon(int _nTexture) {m_nHUDHealthIcon = _nTexture;}
	void SetHUDShieldIcon(int _nTexture) {m_nHUDShieldIcon = _nTexture;}
	void SetBlueEnemyShieldIcon(int _nTexture) {m_nBlueEnemyShieldIcon = _nTexture;}
	void SetCurrentResolution(int _Resolution)  {m_nCurrentResolution = _Resolution;}
	void SetOptionsApply(bool _bShouldApply) {m_bOptionsApply = _bShouldApply;}
	void SetOptionsDefault(bool _bShouldDefault) {m_bOptionsDefault = _bShouldDefault;}
	void SetOptionsCancel(bool _bShouldCancel) {m_bOptionsCancel = _bShouldCancel;}
	void SetBlackTexture(int _nBlackTexture) {m_nBlackTexture = _nBlackTexture;}
	void SetArrowKeyTexture(int _texture) {m_nArrowText = _texture;}
	void SetEnterKeyTexture(int _texture) {m_nEnterKeyText = _texture;}
	void SetArrowKeyLRTexture(int _texture) {m_nArrowKeyLRText = _texture;}
	void SetArrow306Texture(int _texture) {m_nArrow306 = _texture;}
	void SetFromMainMenu(bool _bFromMainMenu) {m_bFromMainMenu = _bFromMainMenu;}
	void SetResolutionChanged(bool _bResolutionChanged) {m_bResolutionChanged = _bResolutionChanged;}
	void SetPSTCTexture(int _texture) {m_nPSTCTexture = _texture;}
	void SetHelpTexture(int _texture) {m_nHelpTexture = _texture;}
};
#endif
