#ifndef CGAMEOVERSTATE_H
#define CGAMEOVERSTATE_H

#include "IGameState.h"

enum newGameOverState{eGO_NONE, eGO_CONTINUE, eGO_RESTART, eGO_MAINMENU};

class CGameOverState : public IGameState
{
	CGame* m_pGame;
	int m_nButtonHighlighted;
	//bool m_bButtonHighlighted[3];
	int  m_nAlphaValue;
	bool m_bChangeState;
	bool m_bAlphaIn;
	
	bool m_bMouseHasPriority;
	int m_nPrevMouseXPos;
	int m_nPrevMouseYPos;
	int m_nSwitchState;

	D3DXMATRIX	m_ReticleMat;
	D3DXMATRIX	m_TargetMat;

public:
	/*****************************************************************
	* CGameOverState():  Default Constructor. Will set its type and behaviors.
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*******************************************************************/
	CGameOverState(void);

	/*****************************************************************
	* CGameOverState():  Overloaded Constructor. Will be used to create new states.
	* Ins:			    _pGame    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	CGameOverState(CGame* _pGame);

	/*****************************************************************
	* ~CGameOverState():  Virtual destructor used when shutting down the game state
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	~CGameOverState(void);
	
	/*****************************************************************
	* Enter(): Enters the game over state. Will set any variables needed for the main menu state
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Enter();

	/*****************************************************************
	* Exit(): Exits the game over state. Will set any variables used to null values
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Exit();

	/*****************************************************************
	* Input(): Grabs any input made by the player
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    bool
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	bool Input();

	/*****************************************************************
	* Update(): Updates the game over state based on the change in time
	* Ins:			    _fElapsedTime    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Update(float _fElapsedTime);

	/*****************************************************************
	* Render(): Renders the game over state by calling the renderer classes render function
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Render();
};
#endif