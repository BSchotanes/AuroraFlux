/***********************************************
 * Filename:  		CConfirmationState.h
 * Date:      		11/27/2012
 * Mod. Date: 		11/29/2012
 * Mod. Initials:	AG
 * Author:    		Andrew Goossens
 * Purpose:   		This will display the confirmation screen
 *						when the player attempts to leave the gameplay state
 *					
 ************************************************/
#ifndef	CCONFIRMATIONSTATE_H
#define CCONFIRMATIONSTATE_H

#include "IGameState.h"

enum newConfirmationState{eCS_NONE, eCS_YES, eCS_NO};

class CGame;

class CConfirmationState : public IGameState
{
	CGame*	m_pGame; 
	int		m_nButtonHighlighted;
	int		m_nGameState;
	int  m_nAlphaValue;
	bool m_bChangeState;
	bool m_bAlphaIn;
	bool m_bPressedEscape;
	bool m_bResolutionChanged;
	//bool m_bButtonHighlighted[2];

	bool m_bMouseHasPriority;
	int m_nPrevMouseXPos;
	int m_nPrevMouseYPos;
	int m_nSwitchState;

	D3DXMATRIX	m_ReticleMat;
	D3DXMATRIX	m_TargetMat;
	
public:

	/*****************************************************************
	* CConfirmationState():  Default Constructor. Will set its type and behaviors.
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	CConfirmationState(void);
	
	/*****************************************************************
	* CConfirmationState():  Overloaded Constructor. Will be used to create new states.
	* Ins:			    _pGame
						_eState
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	CConfirmationState(CGame * _pGame, GameStates _eState);

	/*****************************************************************
	* ~CConfirmationState():  Virtual destructor used when shutting down the confirmation state
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	virtual ~CConfirmationState(void);

	/*****************************************************************
	* Enter(): Enters the confirmation state. Will set any variables needed for the confirmation state
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Enter();
	
	/*****************************************************************
	* Exit(): Exits the confirmation state. Will set any variables used to null values
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
	* Update(): Updates the confirmation state based on the change in time
	* Ins:			    fElapsedTime    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Update(float _fElapsedTime);
	
	/*****************************************************************
	* Render(): Renders the confirmation state by calling the renderer classes render function
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Render();
};
#endif