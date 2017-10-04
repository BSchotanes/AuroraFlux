/***********************************************
* Filename:  		CWinState.h
* Date:      		11/29/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AG
* Author:    		Andrew Goossens
* Purpose:   		This will display the win screen
*						when the player beats the game
*					
************************************************/
#ifndef CWINSTATE_H
#define CWINSTATE_H

#include "Game.h"

class CWinState : public IGameState
{
	CGame*	m_pGame;
	StatTrack* EndStats;
	int	m_nButtonHighlighted;
	float m_fTimeToCredits;
	int  m_nAlphaValue;
	bool m_bChangeState;
	bool m_bAlphaIn;
	bool m_bPressedEscape;

public:
	/*****************************************************************
	* CWinState():		Default Constructor. Will set its type and behaviors.
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	CWinState(void);

	/*****************************************************************
	* CWinState():		Overloaded Constructor. Will be used to create new states.
	* Ins:			    _pGame    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	CWinState(CGame* _pGame) {m_pGame = _pGame;}

	/*****************************************************************
	* ~CWinState():		Virtual destructor used when shutting down the pause state
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	~CWinState(void);

	/*****************************************************************
	* Enter(): Enters the win state. Will set any variables needed for the main menu state
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Enter();

	/*****************************************************************
	* Exit(): Exits the win state. Will set any variables used to null values
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
	* Update(): Updates the win state based on the change in time
	* Ins:			    _fElapsedTime    		      
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