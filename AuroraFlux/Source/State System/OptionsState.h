/***********************************************
 * Filename:  		COptionsState.h
 * Date:      		11/27/2012
 * Mod. Date: 		11/29/2012
 * Mod. Initials:	AG
 * Author:    		Andrew Goossens
 * Purpose:   		This will display the options screen
 *						and allow the player to change game settings
 *					
 ************************************************/
#ifndef	COPTIONSSTATE_H
#define COPTIONSSTATE_H

#include "IGameState.h"
#include "Game.h"
#include "../Input/Input.h"

enum newOptionsState{eOP_NONE, eOP_MUSIC, eOP_SFX, eOP_GAMMA, eOP_YAW, eOP_PITCH, eOP_RESOLUTION_LEFT, eOP_RESOLUTION_RIGHT, eOP_INVERTX, eOP_INVERTY, eOP_APPLY, eOP_DEFAULT, eOP_CANCEL, eOP_1024x768, eOP_1680x1050, eOP_1920x1080};

class COptionsState : public IGameState
{
	CGame* m_pGame;
	int m_nPrevState;
	//bool m_bButtonHighlighted[10];
	int  m_nAlphaValue;
	bool m_bChangeState;
	bool m_bAlphaIn;
	bool m_bPressedEscape;
	bool m_bApplyChanges;
	int m_nToWhichState;
	int m_nButtonHighlighted;
	int m_nClickedState;
	bool m_bOnInvert;

	// DEFAULT
	bool m_bRevertToDefault;
	float m_fDefaultVolume;
	float m_fDefaultSFX;
	float m_fDefaultGamma;
	float m_fDefaultYaw;
	float m_fDefaultPitch;
	bool m_bDefaultInvertY;
	bool m_bDefaultInvertX;
	int m_nDefaultResolution;

	// CANCEL
	bool m_bCancelChanges;
	float m_fPrevVolume;
	float m_fPrevSFX;
	float m_fPrevGamma;
	float m_fPrevYaw;
	float m_fPrevPitch;
	bool m_bPrevInvertY;
	bool m_bPrevInvertX;
	int m_nPrevResolution;

	// APPLY
	float m_fApplyVolume;
	float m_fApplySFX;
	float m_fApplyGamma;
	float m_fApplyYaw;
	float m_fApplyPitch;
	bool m_bApplyInvertY;
	bool m_bApplyInvertX;
	int m_nApplyResolution;

	// BALL POSITIONS
	int m_nMusicBallXPos;
	int m_nSFXBallXPos;
	int m_nGammaBallXPos;
	int m_nYawBallXPos;
	int m_nPitchBallXPos;
	D3DXMATRIX	m_ReticleMat;
	D3DXMATRIX	m_TargetMat;

	
	bool m_bMouseHasPriority;
	int m_nPrevMouseXPos;
	int m_nPrevMouseYPos;
	int m_nSwitchState;
	int m_nMouseButtonDownOnState;
	bool m_bMouseButtonDown;
	bool m_bButtonPressed;
	bool m_bButtonPressedTimer;

	int m_nResolutionSetting;
	bool m_bChangeResolution;
	bool m_bConfirmedResChange;

	IGameState* m_pPause;
	IGameState* m_pOptions;
public:
	/*****************************************************************
	* COptionsState():  Default Constructor. Will set its type and behaviors.
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	COptionsState(void);

	/*****************************************************************
	* COptionsState():  Overloaded Constructor. Will be used to create new states.
	* Ins:			    _game  
						_nPrevState
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	COptionsState(CGame * _pGame, int _nPrevState); 

	/*****************************************************************
	* ~COptionsState():  Virtual destructor used when shutting down the options state
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	virtual ~COptionsState(void);

	/*****************************************************************
	* Enter(): Enters the options state. Will set any variables needed for the options state
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Enter();

	/*****************************************************************
	* Exit(): Exits the options state. Will set any variables used to null values
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Exit();

	/*****************************************************************
	* Input(): Grabs any input made by the player
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    bool
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	bool Input();

	/*****************************************************************
	* Update(): Updates the options state based on the change in time
	* Ins:			    fElapsedTime    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Update(float _fElapsedTime);

	/*****************************************************************
	* Render(): Renders the options state by calling the renderer classes render function
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Render();
	
};
#endif