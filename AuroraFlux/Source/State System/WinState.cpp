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
#include "../StdAfx.h"
#include "WinState.h"
#include "CreditsState.h"

/*****************************************************************
* CWinState():		Default Constructor. Will set its type and behaviors.
* Ins:			    None    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
CWinState::CWinState(void)
{
	m_pGame = nullptr;
	
}

/*****************************************************************
* ~CWinState():		Virtual destructor used when shutting down the pause state
* Ins:			    none    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
CWinState::~CWinState(void)
{
}

/*****************************************************************
* Enter(): Enters the main menu state. Will set any variables needed for the main menu state
* Ins:			    none    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CWinState::Enter()
{	
	m_pGame->GetCamera()->ResetCamera();
	EndStats = m_pGame->GetCurrentStats();
	D3DXMATRIX mat;
	D3DXMatrixTranslation(&mat, 0.0f, 0.0f, 8000.0f);
	m_pGame->GetCamera()->SetViewMatrix(&mat);
	
	WwiseNS::SoundEvent(ePause, ePause);
	CInput::SetCursorLock(false);

	m_fTimeToCredits = 0.0f;

	m_nAlphaValue = 0;
	m_bChangeState = false;
	m_bAlphaIn = true;
	m_bPressedEscape = false;
}

/*****************************************************************
* Exit(): Exits the main menu state. Will set any variables used to null values
* Ins:			    none    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CWinState::Exit()
{
}

/*****************************************************************
* Input(): Grabs any input made by the player
* Ins:			    none    		      
* Outs:				None
* Returns:		    bool
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
bool CWinState::Input()
{
	if(CInput::BufferedButtonPress(eStart, true) && m_bPressedEscape == false)
	{	
		WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
		m_bAlphaIn = false;
		m_bPressedEscape = true;
	}
	else if(m_bChangeState == true)
	{
		WwiseNS::RemoveAllSoundObjs();
		m_pGame->SetCheckPoint(0);
	//	m_pGame->GetCamera()->ResetCamera();
	//	m_pGame->PopState();
		m_pGame->PushState(new CCreditsState(m_pGame));
		//m_pGame->switchState(new CCreditsState(m_pGame));
		return true;
	}

	return true;
}

/*****************************************************************
* Update(): Updates the main menu state based on the change in time
* Ins:			    _fElapsedTime    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CWinState::Update(float _fElapsedTime)
{
	m_pGame->GetState(0)->Update(_fElapsedTime);
	if(m_fTimeToCredits > 10.0f && m_bAlphaIn == true)
	{
		WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
		m_bAlphaIn = false;
	}
	else if(m_bChangeState == true)
	{
		WwiseNS::RemoveAllSoundObjs();
		m_pGame->SetCheckPoint(0);
	//	m_pGame->PopState();
		m_pGame->PushState(new CCreditsState(m_pGame));
		//m_pGame->switchState(new CCreditsState(m_pGame));
		return;
	}

	m_fTimeToCredits += _fElapsedTime;

	if(m_bAlphaIn == true)
	{
		if(m_nAlphaValue < 255)
		{
			m_nAlphaValue += 15;
		}
		else if(m_nAlphaValue > 255)
		{
			m_nAlphaValue = 255;
		}
	}
	else
	{
		if(m_nAlphaValue > 0)
		{
			m_nAlphaValue -= 15;
		}
		else if(m_nAlphaValue <= 0)
		{
			m_nAlphaValue = 0;
			m_bChangeState = true;
		}
	}
}

/*****************************************************************
* Render(): Renders the main menu state by calling the renderer classes render function
* Ins:			    none    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CWinState::Render()
{
//	m_pGame->GetRenderer()->Render2d(m_pGame->GetMenuTexture(0), m_nAlphaValue / 255.0f);
	m_pGame->GetState(0)->Render();
	char buff[64];
	if(EndStats->m_nLasersFired < 1)
		sprintf_s(buff, "Laser Accuracy:\t\t\t  None Fired");
	else
		sprintf_s(buff, "Laser Accuracy:\t\t\t  %3.2f", (float)((float)EndStats->m_nLasersHit/(float)EndStats->m_nLasersFired) * 100.0f);
	m_pGame->GetRenderer()->RenderText(buff, 102, 302, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
	m_pGame->GetRenderer()->RenderText(buff, 100, 300, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));

	if(EndStats->m_nMissilesFired < 1)
		sprintf_s(buff, "Missile Accuracy:\t\t\tNone Fired");
	else
		sprintf_s(buff, "Missile Accuracy:\t\t\t%3.2f", (float)((float)EndStats->m_nMissilesHit/(float)EndStats->m_nMissilesFired) * 100.0f);
	m_pGame->GetRenderer()->RenderText(buff, 102, 352, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
	m_pGame->GetRenderer()->RenderText(buff, 100, 350, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));

	sprintf_s(buff, "Missiles Dodged:\t\t\t %d", EndStats->m_nMissilesDodged);
	m_pGame->GetRenderer()->RenderText(buff, 102, 402, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
	m_pGame->GetRenderer()->RenderText(buff, 100, 400, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	sprintf_s(buff, "Damage Taken:\t\t\t\t%d", EndStats->m_nDamageTaken);
	m_pGame->GetRenderer()->RenderText(buff, 102, 452, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
	m_pGame->GetRenderer()->RenderText(buff, 100, 450, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	sprintf_s(buff, "Deaths:\t\t\t\t\t\t\t\t\t %d", EndStats->m_nDeaths);
	m_pGame->GetRenderer()->RenderText(buff, 102, 502, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_nAlphaValue, 0, 0, 0));
	m_pGame->GetRenderer()->RenderText(buff, 100, 500, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));

	m_pGame->GetRenderer()->RenderText("YOU WIN", 400, 200, 2.5f, 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
	m_pGame->GetRenderer()->RenderText("PRESS ENTER TO CONTINUE", 300, 700, 1.25f, 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));
}