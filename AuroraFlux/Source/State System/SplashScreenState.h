/***********************************************
* Filename:  		CSplashScreenState.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AG
* Author:    		Andrew Goossens
* Purpose:   		This will display the splash screen 
*						while the game is loading assets
*						when the game is first loaded
*					
************************************************/
#ifndef CSPLASHSCREENSTATE_H
#define	CSPLASHSCREENSTATE_H

#include "IGameState.h"

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)			if (p) { p->Release(); p = NULL; }
#endif

class CGame;

class CSplashScreenState : public IGameState
{
	CGame *				m_pGame; 
	vector<IDirect3DTexture9*>	m_d3dpSplashScreens;
	unsigned int			m_nCurrentSplashScreen;
	bool				m_bShowMouse;
	float				m_fTime;
	float				m_fAlpha;
	IDirect3DTexture9* m_nPSTCTexture;
	bool m_bRenderPSTC;
	bool m_bRenderLoading;
	int m_fTextAlpha;

	int  m_nAlphaValue;
	bool m_bChangeState;
	bool m_bAlphaIn;
public:

	/*****************************************************************
	* CSplashScreenState():  Default Constructor. Will set its type and behaviors.
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	CSplashScreenState(void);

	/*****************************************************************
	* CSplashScreenState():  Overloaded Constructor. Will be used to create new states.
	* Ins:			    _pGame    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	CSplashScreenState(CGame * _pGame) { m_pGame = _pGame;}

	/*****************************************************************
	* ~CSplashScreenState():  Virtual destructor used when shutting down the splash screen state
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	virtual ~CSplashScreenState(void);

	/*****************************************************************
	* Enter(): Enters the splash screen state. Will set any variables needed for the splash screen state
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Enter();

	/*****************************************************************
	* Exit(): Exits the splash screen state. Will set any variables used to null values
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
	* Update(): Updates the splash screen state based on the change in time
	* Ins:			    fElapsedTime    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Update(float _fElapsedTime);

	/*****************************************************************
	* Render(): Renders the splash screen state by calling the renderer classes render function
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Render();
};
#endif