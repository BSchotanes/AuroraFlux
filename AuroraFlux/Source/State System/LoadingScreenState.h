/***********************************************
 * Filename:  		LoadingScreenState.h
 * Date:      		1/14/2013
 * Mod. Date: 		1/14/2013
 * Mod. Initials:	AQ
 * Author:    		Alexander Garcia de Quevedo
 * Purpose:   		This will display the loading screen
 *					when the player attempts to play the game
 *					
 ************************************************/
#ifndef	CLOADINGSCREENSTATE_H
#define CLOADINGSCREENSTATE_H

#include "IGameState.h"

/*enum ConfirmationStates {eC_NONE, eC_YES, eC_NO};*/

class CGame;
class CRenderer;
class GamePlayState;

class CLoadingScreenState : public IGameState
{
	CGame*	m_pGame; 
	bool	m_bDoneLoading;
	unsigned m_nTime;
	int  m_nAlphaValue;
	bool m_bChangeState;
	bool m_bAlphaIn;
	CRenderer * m_pRenderer;
	D3DXMATRIX	m_ReticleMat;
	D3DXMATRIX	m_TargetMat;
	D3DCOLOR m_d3dLoadingBarColor;
	RECT m_rLoadingBarRect; 
	D3DXMATRIX	m_d3dLoadingBarMatrix;
	float m_fLoadingPercentage;
	GamePlayState* m_pGamePlayState;
	 int m_nLoadingStage;
	 float m_fInitialLoadingBucket;
	 bool m_bShouldRender[7];
	 int m_nWhichScreen;
	 int loadingBlink;
	 bool loadingBlinkBool;
public:

	/*****************************************************************
	* CLoadingScreenState():  Default Constructor. Will set its type and behaviors.
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	CLoadingScreenState(void);
	
	/*****************************************************************
	* CLoadingScreenState():  Overloaded Constructor. Will be used to create new states.
	* Ins:			    _pGame    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	CLoadingScreenState(CGame * _pGame) { m_pGame = _pGame; }

	/*****************************************************************
	* ~CLoadingScreenState():  Virtual destructor used when shutting down the confirmation state
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	virtual ~CLoadingScreenState(void);

	/*****************************************************************
	* Enter(): Enters the Loading Screen state. Will set any variables needed for the Loading Screen state
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Enter();
	
	/*****************************************************************
	* Exit(): Exits the Loading Screen state. Will set any variables used to null values
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
	* Update(): Updates the Loading Screen state based on the change in time
	* Ins:			    _fElapsedTime    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Update(float _fElapsedTime);
	
	/*****************************************************************
	* Render(): Renders the Loading Screen state by calling the renderer classes render function
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Render();
};
#endif
