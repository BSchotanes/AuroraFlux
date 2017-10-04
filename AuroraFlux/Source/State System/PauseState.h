/***********************************************
* Filename:  		CPauseState.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AG
* Author:    		Andrew Goossens
* Purpose:   		This will display the pause screen
*						when the player pauses the game
*					
************************************************/
#ifndef CPAUSESTATE_H
#define	CPAUSESTATE_H

#include "IGameState.h"

enum newPauseState{ePS_NONE, ePS_RESUME, ePS_OPTIONS, ePS_RESTART, ePS_MAINMENU};

class CGame;

class CPauseState : public IGameState
{
	CGame*	m_pGame;
	//bool m_bButtonHighlighted[4];
	int m_nButtonHighlighted;
	bool m_bMouseHasPriority;
	int m_nSwitchState;	
	int m_nPrevMouseXPos;
	int m_nPrevMouseYPos;

	int  m_nAlphaValue;
	bool m_bChangeState;
	bool m_bAlphaIn;
	bool m_bPressedEscape;

	D3DXMATRIX	m_ReticleMat;
	D3DXMATRIX	m_TargetMat;
public:

	/*****************************************************************
	* CPauseState():  Default Constructor. Will set its type and behaviors.
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	CPauseState(void);

	/*****************************************************************
	* CPauseState():  Overloaded Constructor. Will be used to create new states.
	* Ins:			    _pGame    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	CPauseState(CGame * _pGame) { m_pGame = _pGame;}

	/*****************************************************************
	* ~CPauseState():  Virtual destructor used when shutting down the pause state
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	virtual ~CPauseState(void);

	/*****************************************************************
	* Enter(): Enters the pause state. Will set any variables needed for the pause state
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Enter();

	/*****************************************************************
	* Exit(): Exits the pause state. Will set any variables used to null values
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
	* Update(): Updates the pause state based on the change in time
	* Ins:			    fElapsedTime    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Update(float _fElapsedTime);

	/*****************************************************************
	* Render(): Renders the pause state by calling the renderer classes render function
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Render();
};

#endif