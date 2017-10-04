/***********************************************
 * Filename:  		CreditsState.h
 * Date:      		11/27/2012
 * Mod. Date: 		11/29/2012
 * Mod. Initials:	AG
 * Author:    		Andrew Goossens
 * Purpose:   		This will display the credits for
 *						the game when the game is over
 *						or the option is selected in
 *						the main menu
 *					
 ************************************************/
#ifndef CCREDITSSTATE_H
#define	CCREDITSSTATE_H

#include "IGameState.h"
#include "Game.h"

class CCreditsState : public IGameState
{
	CGame* m_pGame;
	float m_fPosition;
	CDialogSystem m_cCreditDialogSys;

	int  m_nAlphaValue;
	bool m_bChangeState;
	bool m_bAlphaIn;
	bool m_bPressedEscape;

	int	m_nBGAlpha;
	int m_nBGBlackTexture;

	D3DXMATRIX	m_ReticleMat;
	D3DXMATRIX	m_TargetMat;
	std::vector<string> m_d3dSpecialThanksNames;

public:
	/*****************************************************************
	* CCreditsState():  Default Constructor. Will set its type and behaviors.
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	CCreditsState(void);

	/*****************************************************************
	* CCreditsState():  Overloaded Constructor. Will be used to create new states.
	* Ins:			    _game    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	CCreditsState(CGame * _pGame){ m_pGame =  _pGame;}

	/*****************************************************************
	* ~CCreditsState():  Virtual destructor used when shutting down the game state
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	virtual ~CCreditsState(void);

	/*****************************************************************
	* Enter(): Enters the credits state. Will set any variables needed for the state
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Enter();

	/*****************************************************************
	* Exit(): Exits the credits state. Will set any variables used to null values
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
	* Update(): Updates the credits state based on the change in time
	* Ins:			    fElapsedTime    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Update(float _fElapsedTime);

	/*****************************************************************
	* Render(): Renders the credits state by calling the renderer classes render function
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Render();


	void CreateSpecialThanksNames();
};

#endif