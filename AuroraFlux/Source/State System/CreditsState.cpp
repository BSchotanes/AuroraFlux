		/***********************************************
* Filename:  		CreditsState.cpp
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
#include "../StdAfx.h"
#include "MainMenuState.h"
#include "GamePlayState.h"
#include "CreditsState.h"
#include "OptionsState.h"

/*****************************************************************
* CCreditsState():  Default Constructor. Will set its type and behaviors.
* Ins:			    None    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*******************************************************************/
CCreditsState::CCreditsState(void)
{
}

/*****************************************************************
* ~CCreditsState():  Virtual destructor used when shutting down the game state
* Ins:			    none    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
CCreditsState::~CCreditsState(void)
{
}

/*****************************************************************
* Enter(): Enters the credits state. Will set any variables needed for the state
* Ins:			    none    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CCreditsState::Enter()
{
	//cout << "Entering credits State" << endl;
	CreateSpecialThanksNames();
	m_fPosition = 700;
	m_nAlphaValue = 0;
	m_bChangeState = false;
	m_bAlphaIn = true;
	m_bPressedEscape = false;

	CInput::SetCursorLock(true);

	D3DXMatrixIdentity(&m_ReticleMat);
	WwiseNS::SoundEvent(eBGM_CreditsMusic, ePlay);
		
	m_nBGAlpha = 0;	
	m_nBGBlackTexture =  m_pGame->GetRenderer()->GetAssetManager()->LoadTexture("black.png");
}
void CCreditsState::CreateSpecialThanksNames()
{
	m_d3dSpecialThanksNames.push_back("Jonathan Loftin");
	m_d3dSpecialThanksNames.push_back("Daniel Lima");
	m_d3dSpecialThanksNames.push_back("Carolina DaSilva");
	m_d3dSpecialThanksNames.push_back("Mike Godfrey");
	m_d3dSpecialThanksNames.push_back("Robert Macauley");
	m_d3dSpecialThanksNames.push_back("Tom Gregg");
	m_d3dSpecialThanksNames.push_back("Joe Winter");
	m_d3dSpecialThanksNames.push_back("Clark Abel");
	m_d3dSpecialThanksNames.push_back("Jonathan Burnside");
	//m_d3dSpecialThanksNames.push_back("");
	//m_d3dSpecialThanksNames.push_back("Doug Monroe");
	//m_d3dSpecialThanksNames.push_back("Justin Tacket");
	//m_d3dSpecialThanksNames.push_back("Rodney Stoeffler");
	//m_d3dSpecialThanksNames.push_back("Arthur Johnson");
	//m_d3dSpecialThanksNames.push_back("Bahin from Cronos");
	//m_d3dSpecialThanksNames.push_back("Robert Muscarella");
	//m_d3dSpecialThanksNames.push_back("Charles Rainey");
	//m_d3dSpecialThanksNames.push_back("Joshua Fox");
	//m_d3dSpecialThanksNames.push_back("Nick Penney");
	//m_d3dSpecialThanksNames.push_back("Lary Norris");
	//m_d3dSpecialThanksNames.push_back("Dave Brown");
	//m_d3dSpecialThanksNames.push_back("Steve VanZandt");
	//m_d3dSpecialThanksNames.push_back("John Czaban");
	//m_d3dSpecialThanksNames.push_back("John O'Leske");
	//m_d3dSpecialThanksNames.push_back("Richard Wright");
	//m_d3dSpecialThanksNames.push_back("Gary Miller");
	//m_d3dSpecialThanksNames.push_back("Richard Arndt");
	//m_d3dSpecialThanksNames.push_back("Ryan Van Dyke");
	//m_d3dSpecialThanksNames.push_back("Brandon Patterson");
	//m_d3dSpecialThanksNames.push_back("Jonathan Burnside");
	//m_d3dSpecialThanksNames.push_back("John Magno");
	//m_d3dSpecialThanksNames.push_back("Jeremiah Blanchard");
	//m_d3dSpecialThanksNames.push_back("Shawn Kendall");
	//m_d3dSpecialThanksNames.push_back("Matt Norris");
	//m_d3dSpecialThanksNames.push_back("Darryl Malcomb");
	//m_d3dSpecialThanksNames.push_back("Susan Capozza");
	//m_d3dSpecialThanksNames.push_back("Shane Saedi");
}
/*****************************************************************
* Exit(): Exits the credits state. Will set any variables used to null values
* Ins:			    none    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CCreditsState::Exit()
{
	//cout << "Leaving credits State\n" << endl;
	m_d3dSpecialThanksNames.clear();
}

/*****************************************************************
* Input(): Grabs any input made by the player
* Ins:			    none    		      
* Outs:				None
* Returns:		    bool
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
bool CCreditsState::Input()
{
	if((CInput::BufferedButtonPress(eEsc, true) || CInput::BufferedButtonPress(eStart, true)) && m_bPressedEscape == false)
	{
		WwiseNS::SoundEvent(eMenuItemConfirm, ePlay);
		m_bAlphaIn = false;
		m_bPressedEscape = true;
	}
	else if(m_bChangeState == true)
	{
		//m_pGame->switchState(new CMainMenuState(m_pGame));
		if( m_pGame->GetStateSize() == 2 )
		{
			m_pGame->GetState(0)->Exit();
			m_pGame->GetState(0)->Enter();
			m_pGame->PopState();
			return true;
		}
		else
		{
			for( int i(0); i < m_pGame->GetStateSize(); ++i )
			{
				m_pGame->PopBottom();
			}
			m_pGame->switchState(new CMainMenuState(m_pGame));
		}
	}

	return true;
}

/*****************************************************************
* Update(): Updates the credits state based on the change in time
* Ins:			    fElapsedTime    		      
* Outs:				None 
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CCreditsState::Update(float _fElapsedTime)
{
	if( m_pGame->GetStateSize() > 1 )
	{
		m_pGame->GetState(0)->Update(_fElapsedTime);
	}

	if( m_nBGAlpha < 200 )
	{
		m_nBGAlpha += (int)(_fElapsedTime * 200.0f);
	}

	m_fPosition += -75.0f * _fElapsedTime;

	if(m_fPosition < 0 - (m_d3dSpecialThanksNames.size() * 175.0f))
	{
		m_fPosition = 700;
	}

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
* Render(): Renders the credits state by calling the renderer classes render function
* Ins:			    none    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
void CCreditsState::Render()
{
	if (m_pGame->GetStateSize() > 1)
	{
		m_pGame->GetState(0)->Render();
	}

	m_pGame->GetRenderer()->Render2d(m_nBGBlackTexture, (float)(m_nBGAlpha)/255.0f);
	
	m_pGame->GetRenderer()->RenderText("CREDITS", 425, 100, 3.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255));

	int nPosition = (int)m_fPosition;
	int XPos = 412;//int((m_pGame->GetWidth() / 2) - 100);
	float fExitAlpha = (float)m_nAlphaValue/255.0f;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Studio Head", XPos - 64, nPosition, 1.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 215, 0));
	nPosition += 30;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Liam Hislop", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));
	nPosition += 40;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Tech Director", XPos - 64, nPosition, 1.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 215, 0));
	nPosition += 30;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Mike Lebo", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));
	nPosition += 40;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("External Producer", XPos - 64, nPosition, 1.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 215, 0));
	nPosition += 30;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Carlos Lugo", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));
	nPosition += 40;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Art Director", XPos - 64, nPosition, 1.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 215, 0));
	nPosition += 30;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Phil Marunowski", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));
	nPosition += 40;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Audio Director", XPos - 64, nPosition, 1.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 215, 0));
	nPosition += 30;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Chris Marks", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5f)*3)), 255, 255, 255));
	nPosition += 40;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Gameplay Producer", XPos - 64, nPosition, 1.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 215, 0));
	nPosition += 30;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Patrick Kelly", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5f)*3)), 255, 255, 255));

	nPosition += 80;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Team Rogue Theory", XPos - 96, nPosition, 1.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 215, 255));
	nPosition += 40;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Team Lead / Programming Lead", XPos - 64, nPosition, 1.25f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 215, 0));
	nPosition += 30;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Andrew Goossens", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));
	nPosition += 40;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("AI Lead / Gameplay Lead", XPos - 64, nPosition, 1.25f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 215, 0));
	nPosition += 30;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Andrew Rosser", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));
	nPosition += 40;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Rendering Lead", XPos - 64, nPosition, 1.25f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 215, 0));
	nPosition += 30;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Bryan Schotanes", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));
	nPosition += 40;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Audio Lead / QA Lead", XPos - 64, nPosition, 1.25f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 215, 0));
	nPosition += 30;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Jayson Wu", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));
	nPosition += 40;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Programmers", XPos - 64, nPosition, 1.25f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 215, 0));
	nPosition += 30;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Alex Garcia", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));
	nPosition += 20;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Antone Lopez", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));
	nPosition += 20;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Devan Oatney", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));
	nPosition += 20;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Stephen Warren", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));

	nPosition += 60;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Artists", XPos - 64, nPosition, 1.25f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 215, 0));
	nPosition += 30;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Jordan Powers", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));
	nPosition += 20;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Jimmy Verschaeve", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));
	nPosition += 20;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Robert J Barngrover", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));
	nPosition += 20;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Jason C Cross", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));
	nPosition += 20;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Bryan P Davis", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));
	nPosition += 20;
	
	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Michael R Diaz", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));
	nPosition += 20;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Kevin Garcia", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));
	nPosition += 20;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Danella Georgiev", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));
	nPosition += 20;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Arielle R Goldfarb", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));
	nPosition += 20;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Ashley Nicky Larraga", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));
	nPosition += 20;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Tobias J Melville", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));
	nPosition += 20;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Colin M Mulvey", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));
	nPosition += 20;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Christian J Taormina", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));
	nPosition += 20;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Benjamin A Trout", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));
	nPosition += 20;	

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Sara E Withrow", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));
	nPosition += 20;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Christopher Zhen", XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));
	nPosition += 60;

	if(nPosition < 640 && nPosition > 300)
		m_pGame->GetRenderer()->RenderText("Special Thanks", XPos - 64, nPosition, 1.5f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 215, 0));
	nPosition += 30;

	for(unsigned int i(0); i < m_d3dSpecialThanksNames.size(); ++i)
	{
		if(nPosition < 640 && nPosition > 300)
			m_pGame->GetRenderer()->RenderText(m_d3dSpecialThanksNames[i].c_str(), XPos, nPosition, 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB((int)(fExitAlpha * (float) min(255, int((170 - abs(470 - nPosition))*1.5)*3)) ,255, 255, 255));
		nPosition += 25;
	}

	RECT m_rArrowRect = {0, 0, 64, 64};

	m_pGame->GetRenderer()->RenderText("PRESS ESCAPE OR SELECT TO CONTINUE", 300, 700, 1.0f, 0, 0, 0, D3DCOLOR_ARGB(m_nAlphaValue, 255, 255, 255)); 
}