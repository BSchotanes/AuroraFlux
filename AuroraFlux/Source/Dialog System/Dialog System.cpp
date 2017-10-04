#include "Dialog System.h"

void tDialogNode::SetNodes(char* pchText, float fTime, int nX, int nY, float fScale)
{
	m_pchDialogText = pchText;
	m_fWaitTime = fTime;
	m_nXPosition = nX;
	m_nYPosition = nY;
	m_fTextScale = fScale;
}

void tSection::SetPoints(int _Start, int _End)
{
	m_nStartPoint = _Start;
	m_nEndPoint = _End;
}

CDialogSystem::CDialogSystem(void)
{
	for( int CurrNode = 0; CurrNode < NUM_DIALOG; CurrNode++)
	{
		m_arrDialog[CurrNode].SetNodes("", 0.0f, 0, 0, 0.0f);
	}

	for(int CurrSel = 0; CurrSel < eDialog_Max; CurrSel++)
	{
		m_arrDialogSections[CurrSel].SetPoints(0,0);
	}
	eCurrentPoint = eDialog_None;
	m_fCurrentDialogTime = 0.0f;
	m_nCurrentDialog = 0;
	m_nMaxNumDialog = 0;
	m_bChanging = false;
	m_bSectionDone = true;
	m_bHasPlayedSound = false;
	//diffx = 0;
	//diffy = 0;
}
int CDialogSystem::Initialize(char* _filename = "", CRenderer* _pRenderer = nullptr)
{
	m_pRenderer = _pRenderer;

	m_nWTexture = m_pRenderer->GetAssetManager()->LoadTexture("WKey.png");
	m_nSTexture = m_pRenderer->GetAssetManager()->LoadTexture("SKey.png");
	m_nATexture = m_pRenderer->GetAssetManager()->LoadTexture("AKey.png");
	m_nDTexture = m_pRenderer->GetAssetManager()->LoadTexture("DKey.png");
	//m_nQTexture = m_pRenderer->GetAssetManager()->LoadTexture("QKey.png");
	m_nLeftMouseTexture = m_pRenderer->GetAssetManager()->LoadTexture("LeftMouse.png");
	m_nRightMouseTexture = m_pRenderer->GetAssetManager()->LoadTexture("RightMouse.png");

	if(strlen(_filename) > 0)
	{
		// For future if we decide to support loading in dialog files.
		return 0;
	}
	else
	{
		// Default Case
		// prefered parameters for instructions ("", 3.0f, 32, 290, 1.0f);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//  Starting Text	//////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		m_arrDialog[0].SetNodes("", 1.0f, 460, 290, 1.0f );

		//Controls
		m_arrDialog[1].SetNodes("Move the mouse to steer your ship.", 0.0f, 295, 290, 1.0f);
		m_arrDialogSections[eDialog_Controls].SetPoints(1, 1);

		//Lasers
		m_arrDialog[2].SetNodes("Click      to fire weapons", 0.0f, 378, 290, 1.0f);
		m_arrDialogSections[eTutorial_Laser].SetPoints(2, 2);

		// Speed modifiers
		m_arrDialog[3].SetNodes("Use      &      to Speed up/Slow down", 1.0f, 307, 290, 1.0f);
		m_arrDialogSections[eDialog_Speed].SetPoints(3, 3);

		// Dodging
		m_arrDialog[4].SetNodes("Use      or      to", 3.0f, 390, 290, 1.0f);
		m_arrDialog[5].SetNodes("Careful,            has a cooldown time", 2.0f, 310, 290, 1.0f);
		m_arrDialogSections[eDialog_Dodge].SetPoints(4, 4);
		m_arrDialogSections[eDialog_Start].SetPoints(5, 5);

		//Changing Mode
		m_arrDialog[6].SetNodes("Click      to switch to        /         MODE.", 3.0f, 285, 290, 1.0f);
		m_arrDialog[7].SetNodes("You can only fire                           and", 4.5f, 205, 290, 1.0f);
		m_arrDialog[8].SetNodes("You can't            in          , but your health and shield                 ", 4.0f, 185, 290, 1.0f);

		m_arrDialogSections[eTutorial_SwitchState].SetPoints(6, 6);
		m_arrDialogSections[eTutorial_Blue_Fire].SetPoints(7, 8);
		m_arrDialogSections[eTutorial_Blue_Dodge].SetPoints(8, 8);

		// Go to Waypoints
		m_arrDialog[9].SetNodes("Go to the", 2.0f, 350, 290, 1.0f);
		m_arrDialog[10].SetNodes("Follow the                     if you get lost", 4.0f, 315, 290, 1.0f);
		m_arrDialog[11].SetNodes("", 0.0f, 32, 290, 1.0f);
		m_arrDialog[12].SetNodes("Great Job!", 1.0f, 460, 290, 1.0f );
		m_arrDialogSections[eTutorial_Great_Job].SetPoints(12,12);

		m_arrDialogSections[eDialog_Waypoint].SetPoints(9, 11);
		m_arrDialogSections[eTutorial_Waypoint].SetPoints(10, 10);
		m_arrDialogSections[eTutorial_Waypoint_Arrow].SetPoints(11,11);

		// Q Turnaround
		//m_arrDialog[13].SetNodes("Press      to do a 180* flip", 0.50f, 380, 290, 1.0f);
		//m_arrDialog[14].SetNodes("You can reposition around enemies this way", 3.0f, 250, 290, 1.0f);
		//m_arrDialog[15].SetNodes("", 0.0f, 32, 290, 1.0f);
		//
		//m_arrDialogSections[eDialog_PressQ].SetPoints(13, 13);
		//m_arrDialogSections[eDialog_TurnAround].SetPoints(13, 15);

		m_arrDialog[16].SetNodes("Look an enemy! Let's test your Marksmanship", 4.0f, 245, 290, 1.0f);
		m_arrDialog[17].SetNodes("", 3.5f, 0, 290, 0.8f);
		m_arrDialogSections[eDialog_FirstBlood].SetPoints(16, 17);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//		Boss Encounter			//////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		m_arrDialog[18].SetNodes("WOW! Look at the size of that Ship!", 5.0f, 340, 290, 1.0f);
		m_arrDialog[19].SetNodes("That must be the mothership", 3.0f, 370, 290, 1.0f);
		m_arrDialog[20].SetNodes("", 2.5f, 0, 0, 1.0f);
		m_arrDialogSections[eDialog_Boss].SetPoints(18, 20);

		//Boss Events - Shield Generators
		m_arrDialog[21].SetNodes("Those shield generators are protecting...", 4.0f, 340, 290, 1.0f);
		m_arrDialog[22].SetNodes("the mothership. Go take them out first.", 5.0f, 340, 290, 1.0f);
		m_arrDialog[23].SetNodes("", 5.0f, 0, 0, 1.0f);
		m_arrDialogSections[eDialog_BossShieldGen].SetPoints(21, 23);

		//Boss Events - Turrets
		m_arrDialog[24].SetNodes("Looks like the mothership has it's own weapons", 4.0f, 300, 290, 1.0f);
		m_arrDialog[25].SetNodes("Watch out for those turret lasers", 5.0f, 360, 370, 1.0f);
		m_arrDialog[26].SetNodes("", 5.0f, 0, 0, 1.0f);
		m_arrDialogSections[eDialog_BossTurret].SetPoints(24, 26);

		m_arrDialog[27].SetNodes("", 5.0f, 200, 290, 1.0f);
		m_arrDialogSections[eDialog_RedEnemy].SetPoints(27, 27);
		m_arrDialogSections[eDialog_BlueEnemy].SetPoints(27, 27);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//		Tutorial				//////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////

		m_arrDialogSections[eDialog_None].SetPoints(41, 41);

		m_nCurrentDialog = 0;
		m_fCurrentDialogTime = 0.0f;
		m_nMaxNumDialog = 0;
	}
	return 0;
}

void CDialogSystem::Update(float _fElapsedTime)
{
	if(m_bChanging)
	{

		m_bChanging = m_bSectionDone = false;

		if(eCurrentPoint > 0)
		{
			m_fCurrentDialogTime = 0.0f;
			m_nCurrentDialog = m_arrDialogSections[eCurrentPoint].m_nStartPoint;
			m_nMaxNumDialog =  m_arrDialogSections[eCurrentPoint].m_nEndPoint;
		}
		else
		{
			m_nCurrentDialog = 0;
			m_fCurrentDialogTime = 0.0f;
			m_nMaxNumDialog = 0;
		}
	}
	else
	{
		if(m_nCurrentDialog >= 0 && m_nCurrentDialog < m_nMaxNumDialog)
		{
			m_fCurrentDialogTime += _fElapsedTime;

			if(m_fCurrentDialogTime > m_arrDialog[m_nCurrentDialog].m_fWaitTime)
			{
				m_nCurrentDialog++;
				m_fCurrentDialogTime = 0.0f;
			}
		}
		else if(m_nCurrentDialog == m_nMaxNumDialog)
		{
			m_bSectionDone = true;
		}
	}
}

void CDialogSystem::Render()
{
	//Debug code for moving text
	//if(CInput::DIKButtonCheck(DIK_LEFTARROW))
	//	diffx--;
	//if(CInput::DIKButtonCheck(DIK_RIGHTARROW))
	//	diffx++;
	//if(CInput::DIKButtonCheck(DIK_UPARROW))
	//	diffy--;
	//if(CInput::DIKButtonCheck(DIK_DOWNARROW))
	//	diffy++;
	//if(CInput::DIKBufferedButtonCheck(DIK_SPACE))
	//{
	//	diffx = diffy = 0;
	//}

	RECT rKeyRect = {0, 0, 32, 32};
	D3DXMATRIX d3dKeyMat;
	D3DXMatrixIdentity(&d3dKeyMat);

	float xRatio = m_pRenderer->GetXRatio();
	float yRatio = m_pRenderer->GetYRatio();

	if(m_nCurrentDialog != 8)
		m_bHasPlayedSound = false;
	
	if(m_nCurrentDialog >= 0 && strlen(m_arrDialog[m_nCurrentDialog].m_pchDialogText) > 0)
	{
		tDialogNode* TempDNode = &m_arrDialog[m_nCurrentDialog];
		switch(m_nCurrentDialog)
		{		
		case 1:
			{
				m_pRenderer->RenderText("Objective:", (int)((460)), (int)((250+2)), 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
				m_pRenderer->RenderText("Objective:", (int)((460)), (int)((250)), 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 64, 255, 64));
			}
			break;
		case 2:
			{
				rKeyRect.bottom = 41;

				d3dKeyMat._41 = float(441 * xRatio);
				d3dKeyMat._42 = float(294 * yRatio);

				m_pRenderer->RenderArrow(m_nLeftMouseTexture, &rKeyRect, d3dKeyMat);
				m_pRenderer->RenderText("Objective:", (int)((460)), (int)((250+2)), 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
				m_pRenderer->RenderText("Objective:", (int)((460)), (int)((250)), 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 64, 255, 64));

			}
			break;
		case 3:
			{
				d3dKeyMat._41 = float(362 * xRatio);
				d3dKeyMat._42 = float(294 * yRatio);

				m_pRenderer->RenderArrow(m_nWTexture, &rKeyRect, d3dKeyMat);

				d3dKeyMat._41 = float(429 * xRatio);
				d3dKeyMat._42 = float(294 * yRatio);

				m_pRenderer->RenderArrow(m_nSTexture, &rKeyRect, d3dKeyMat);
				m_pRenderer->RenderText("Objective:", (int)((460)), (int)((250+2)), 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
				m_pRenderer->RenderText("Objective:", (int)((460)), (int)((250)), 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 64, 255, 64));
			}
			break;
		case 4:
			{
				d3dKeyMat._41 = float(445 * xRatio);
				d3dKeyMat._42 = float(294 * yRatio);

				m_pRenderer->RenderArrow(m_nATexture, &rKeyRect, d3dKeyMat);

				d3dKeyMat._41 = float(519 * xRatio);
				d3dKeyMat._42 = float(294 * yRatio);

				m_pRenderer->RenderArrow(m_nDTexture, &rKeyRect, d3dKeyMat);

				m_pRenderer->RenderText("dodge", (int)((TempDNode->m_nXPosition+202)), (int)((TempDNode->m_nYPosition+3)), TempDNode->m_fTextScale, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
				m_pRenderer->RenderText("dodge", (int)((TempDNode->m_nXPosition+200)), (int)((TempDNode->m_nYPosition+1)), TempDNode->m_fTextScale, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 255, 64, 64));
				
				m_pRenderer->RenderText("Objective:", (int)((460)), (int)((250+2)), 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
				m_pRenderer->RenderText("Objective:", (int)((460)), (int)((250)), 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 64, 255, 64));
			}
			break;
		case 5:
			{
				m_pRenderer->RenderText("Dodge", (int)((TempDNode->m_nXPosition+98)), (int)((TempDNode->m_nYPosition+3)), TempDNode->m_fTextScale, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
				m_pRenderer->RenderText("Dodge", (int)((TempDNode->m_nXPosition+96)), (int)((TempDNode->m_nYPosition+1)), TempDNode->m_fTextScale, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 255, 64, 64));
			}
			break;
		case 6:
			{
				rKeyRect.bottom = 41;

				d3dKeyMat._41 = float(348 * xRatio);
				d3dKeyMat._42 = float(294 * yRatio);

				m_pRenderer->RenderArrow(m_nRightMouseTexture, &rKeyRect, d3dKeyMat);
				
				m_pRenderer->RenderText("RED", (int)((TempDNode->m_nXPosition+254)), (int)((TempDNode->m_nYPosition+3)), TempDNode->m_fTextScale, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
				m_pRenderer->RenderText("RED", (int)((TempDNode->m_nXPosition+252)), (int)((TempDNode->m_nYPosition+1)), TempDNode->m_fTextScale, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 255, 64, 64));

				m_pRenderer->RenderText("BLUE", (int)((TempDNode->m_nXPosition+320)), (int)((TempDNode->m_nYPosition+1)), TempDNode->m_fTextScale, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
				m_pRenderer->RenderText("BLUE", (int)((TempDNode->m_nXPosition+320)), (int)((TempDNode->m_nYPosition-1)), TempDNode->m_fTextScale, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 128, 128, 255));
				
				m_pRenderer->RenderText("Objective:", (int)((460)), (int)((250+2)), 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
				m_pRenderer->RenderText("Objective:", (int)((460)), (int)((250)), 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 64, 255, 64));
			}
			break;
		case 7:
			{
				m_pRenderer->RenderText("Missiles in BLUE", (int)((TempDNode->m_nXPosition+216)), (int)((TempDNode->m_nYPosition+1)), TempDNode->m_fTextScale, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
				m_pRenderer->RenderText("Missiles in BLUE", (int)((TempDNode->m_nXPosition+216)), (int)((TempDNode->m_nYPosition-1)), TempDNode->m_fTextScale, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 128, 128, 255));

				m_pRenderer->RenderText("Lasers in RED", (int)((TempDNode->m_nXPosition+480)), (int)((TempDNode->m_nYPosition+3)), TempDNode->m_fTextScale, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
				m_pRenderer->RenderText("Lasers in RED", (int)((TempDNode->m_nXPosition+480)), (int)((TempDNode->m_nYPosition+1)), TempDNode->m_fTextScale, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 255, 64, 64));
			}
			break;
		case 8:
			{
				//////////////////////////////////////////////////////////////////
				//plays the audio for blue mode regenerating, in here since this dialogue is handled differently than all of the others
				if(!m_bHasPlayedSound)
				{
					WwiseNS::StopAllTutorialVoices(eAIBlueModeRegenerate);
					WwiseNS::SoundEvent(eAIBlueModeRegenerate, ePlay);
					m_bHasPlayedSound = true;
				}
				//////////////////////////////////////////////////////////////////	
				m_pRenderer->RenderText("Dodge", (int)((TempDNode->m_nXPosition+124)), (int)((TempDNode->m_nYPosition+3)), TempDNode->m_fTextScale, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
				m_pRenderer->RenderText("Dodge", (int)((TempDNode->m_nXPosition+124)), (int)((TempDNode->m_nYPosition+1)), TempDNode->m_fTextScale, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 255, 64, 64));

				m_pRenderer->RenderText("BLUE                               \n   regenerates", (int)((TempDNode->m_nXPosition+240)), (int)((TempDNode->m_nYPosition+1)), TempDNode->m_fTextScale, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
				m_pRenderer->RenderText("BLUE                               \n   regenerates", (int)((TempDNode->m_nXPosition+240)), (int)((TempDNode->m_nYPosition-1)), TempDNode->m_fTextScale, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 128, 128, 255));
			}
			break;
		case 9:
			{
				m_pRenderer->RenderText("Green Waypoint", (int)((TempDNode->m_nXPosition+122)), (int)((TempDNode->m_nYPosition+2)), TempDNode->m_fTextScale, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
				m_pRenderer->RenderText("Green Waypoint", (int)((TempDNode->m_nXPosition+122)), (int)((TempDNode->m_nYPosition)), TempDNode->m_fTextScale, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 64, 255, 64));
			}
			break;
		case 10:
			{
				m_pRenderer->RenderText("Green Arrow", (int)((TempDNode->m_nXPosition+134)), (int)((TempDNode->m_nYPosition+2)), TempDNode->m_fTextScale, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
				m_pRenderer->RenderText("Green Arrow", (int)((TempDNode->m_nXPosition+134)), (int)((TempDNode->m_nYPosition)), TempDNode->m_fTextScale, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 64, 255, 64));
			}
			break;
		case 13:
			{
				d3dKeyMat._41 = float(457 * xRatio);
				d3dKeyMat._42 = float(294 * yRatio);

				m_pRenderer->RenderArrow(m_nQTexture, &rKeyRect, d3dKeyMat);
				
				m_pRenderer->RenderText("Objective:", (int)((460)), (int)((250+2)), 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
				m_pRenderer->RenderText("Objective:", (int)((460)), (int)((250)), 1.0f, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 64, 255, 64));
			}
			break;
		default:
			break;
		}

		//m_pRenderer->RenderText(TempDNode->m_pchDialogText, (int)((TempDNode->m_nXPosition+2) * xRatio), (int)((TempDNode->m_nYPosition+2) * yRatio), TempDNode->m_fTextScale, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
		//m_pRenderer->RenderText(TempDNode->m_pchDialogText, (int)((TempDNode->m_nXPosition + diffx) * xRatio), (int)((TempDNode->m_nYPosition + diffy)* yRatio), TempDNode->m_fTextScale, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 255, 255, 255));

		m_pRenderer->RenderText(TempDNode->m_pchDialogText, (int)((TempDNode->m_nXPosition+2)), (int)((TempDNode->m_nYPosition+2)), TempDNode->m_fTextScale, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
		m_pRenderer->RenderText(TempDNode->m_pchDialogText, (int)(TempDNode->m_nXPosition), (int)(TempDNode->m_nYPosition), TempDNode->m_fTextScale, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}
