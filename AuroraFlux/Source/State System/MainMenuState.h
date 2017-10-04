/***********************************************
* Filename:  		CMainMenuState.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AG
* Author:    		Andrew Goossens
* Purpose:   		This will display the main menu screen
*						when the player first enters the game
*					
************************************************/
#ifndef CMAINMENUSTATE_H
#define CMAINMENUSTATE_H 

#include "IGameState.h"
#include "Game.h"
#include "../Input/Input.h"
#include "../Object Manager/ObjectManager.h"

enum newMainMenuState{eMM_NONE, eMM_PLAY, eMM_OPTIONS, eMM_CREDITS, eMM_EXIT};

class CGame;
class CSceneryManager;
class CMainMenuState : public IGameState
{
	CSceneryManager * m_pScene;
	CGame *	m_pGame; 
	int	m_nButtonHighlighted;
	int m_nAlphaValue;
	bool m_bChangeState;
	bool m_bAlphaIn;
	bool m_bMouseHasPriority;
	int m_nPrevMouseXPos;
	int m_nPrevMouseYPos;
	int m_nSwitchState;
	CPlayer * m_pPlayer;
	D3DXMATRIX	m_ReticleMat;
	D3DXMATRIX	m_TargetMat;
	CObjectManager * m_pObjectManager;
	CRenderer * m_pRenderer;
	CCamera   * m_pCamera;
	bool m_bPSTCOn;
	CAIHelper * s_AIHelper;
	int m_nBlinkAlpha;
	bool m_bBlinkUp;

public:

	/*****************************************************************
	* CMainMenuState():  Default Constructor. Will set its type and behaviors.
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	CMainMenuState(void);

	/*****************************************************************
	* CMainMenuState():  Overloaded Constructor. Will be used to create new states.
	* Ins:			    _pGame    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	CMainMenuState(CGame * _pGame); 

	/*****************************************************************
	* ~CMainMenuState():  Virtual destructor used when shutting down the main menu state
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	virtual ~CMainMenuState(void);

	/*****************************************************************
	* Enter(): Enters the main menu state. Will set any variables needed for the main menu state
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Enter();

	/*****************************************************************
	* Exit(): Exits the main menu state. Will set any variables used to null values
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
	* Update(): Updates the main menu state based on the change in time
	* Ins:			    fElapsedTime    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Update(float _fElapsedTime);

	/*****************************************************************
	* Render(): Renders the main menu state by calling the renderer classes render function
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Render();
};
#endif