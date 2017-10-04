/***********************************************
* Filename:  		GUIHelper.h
* Date:      		01/17/2013
* Mod. Date: 		01/17/2013
* Mod. Initials:	AG
* Author:    		Andrew Goossens
* Purpose:   		This will display the game state for
*						the game. 
*					
************************************************/
#include "../StdAfx.h"
#include "GuiHelper.h"

/*****************************************************************
* CGuiHelper():  Default Constructor. Will set its type and behaviors.
* Ins:			    None    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
CGuiHelper::CGuiHelper(void)
{
	ReadFile("GUILoader.txt");
	ReadTextFile("GUITextLoader.txt");
}

/*****************************************************************
* ~CGuiHelper():  Virtual destructor used when shutting down the game state
* Ins:			    none    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AG
*****************************************************************/
CGuiHelper::~CGuiHelper(void)
{
}

/*****************************************************************
* ReadFile(): Function that reads in a text file that has the GUI information
and fills out the variables
* Ins:					none    		      
* Outs:					None
* Returns:				void
* Mod. Date:			1/12/2013
* Mod. Initials:		AG
*****************************************************************/
bool CGuiHelper::ReadFile(LPCTSTR _szFilePath)
{
	string szTempFilePath = "Resources/Scripts/";
	szTempFilePath += _szFilePath;

	fstream fsScriptIn(szTempFilePath, std::ios_base::in);

	if(!fsScriptIn)
	{
#if _DEBUG
		cout << "Script did not load properly!" << '\n';
#endif
		return false;
	}

	// Main Menu
	fsScriptIn >> m_nMMPlayXPos;
	fsScriptIn >> m_nMMPlayYPos;
	fsScriptIn >> m_fMMPlayXScale;
	fsScriptIn >> m_fMMPlayYScale;
	fsScriptIn >> m_nMMOptionsXPos;
	fsScriptIn >> m_nMMOptionsYPos;
	fsScriptIn >> m_fMMOptionsXScale;
	fsScriptIn >> m_fMMOptionsYScale;
	fsScriptIn >> m_nMMCreditsXPos;
	fsScriptIn >> m_nMMCreditsYPos;
	fsScriptIn >> m_fMMCreditsXScale;
	fsScriptIn >> m_fMMCreditsYScale;
	fsScriptIn >> m_nMMExitXPos;
	fsScriptIn >> m_nMMExitYPos;
	fsScriptIn >> m_fMMExitXScale;
	fsScriptIn >> m_fMMExitYScale;
	fsScriptIn >> m_nMMBorderXPos;
	fsScriptIn >> m_nMMBorderYPos;
	fsScriptIn >> m_fMMBorderXScale;
	fsScriptIn >> m_fMMBorderYScale;

	// Options
	fsScriptIn >> m_nOPMainXPos;
	fsScriptIn >> m_nOPMainYPos;
	fsScriptIn >> m_fOPMainXScale;
	fsScriptIn >> m_fOPMainYScale;
	fsScriptIn >> m_nOPInvertYXPos;
	fsScriptIn >> m_nOPInvertYYPos;
	fsScriptIn >> m_fOPInvertYXScale;
	fsScriptIn >> m_fOPInvertYYScale;
	fsScriptIn >> m_nOPInvertXXPos;
	fsScriptIn >> m_nOPInvertXYPos;
	fsScriptIn >> m_fOPInvertXXScale;
	fsScriptIn >> m_fOPInvertXYScale;
	fsScriptIn >> m_nOPCancelXPos;
	fsScriptIn >> m_nOPCancelYPos;
	fsScriptIn >> m_fOPCancelXScale;
	fsScriptIn >> m_fOPCancelYScale;
	fsScriptIn >> m_nOPDefaultXPos;
	fsScriptIn >> m_nOPDefaultYPos;
	fsScriptIn >> m_fOPDefaultXScale;
	fsScriptIn >> m_fOPDefaultYScale;
	fsScriptIn >> m_nOPApplyXPos;
	fsScriptIn >> m_nOPApplyYPos;
	fsScriptIn >> m_fOPApplyXScale;
	fsScriptIn >> m_fOPApplyYScale;
	fsScriptIn >> m_nOPInvertYBoxXPos;
	fsScriptIn >> m_nOPInvertYBoxYPos;
	fsScriptIn >> m_fOPInvertYBoxXScale;
	fsScriptIn >> m_fOPInvertYBoxYScale;
	fsScriptIn >> m_nOPInvertXBoxXPos;
	fsScriptIn >> m_nOPInvertXBoxYPos;
	fsScriptIn >> m_fOPInvertXBoxXScale;
	fsScriptIn >> m_fOPInvertXBoxYScale;
	fsScriptIn >> m_nOPVolumeBallXPos;
	fsScriptIn >> m_nOPVolumeBallYPos;
	fsScriptIn >> m_fOPVolumeBallXScale;
	fsScriptIn >> m_fOPVolumeBallYScale;
	fsScriptIn >> m_nOPVolumeBallSelectedXPos;
	fsScriptIn >> m_nOPVolumeBallSelectedYPos;
	fsScriptIn >> m_fOPVolumeBallSelectedXScale;
	fsScriptIn >> m_fOPVolumeBallSelectedYScale;
	fsScriptIn >> m_nOPLeftBorderXPos;
	fsScriptIn >> m_nOPLeftBorderYPos;
	fsScriptIn >> m_fOPLeftBorderXScale;
	fsScriptIn >> m_fOPLeftBorderYScale;
	fsScriptIn >> m_nOPRightBorderXPos;
	fsScriptIn >> m_nOPRightBorderYPos;
	fsScriptIn >> m_fOPRightBorderXScale;
	fsScriptIn >> m_fOPRightBorderYScale;

	

	fsScriptIn >> m_nOPSBallXPos;
	fsScriptIn >> m_nOPSBallYPos;
	fsScriptIn >> m_fOPSBallXScale;
	fsScriptIn >> m_fOPSBallYScale;
	fsScriptIn >> m_nOPGBallXPos;
	fsScriptIn >> m_nOPGBallYPos;
	fsScriptIn >> m_fOPGBallXScale;
	fsScriptIn >> m_fOPGBallYScale;
	fsScriptIn >> m_nOPYBallXPos;
	fsScriptIn >> m_nOPYBallYPos;
	fsScriptIn >> m_fOPYBallXScale;
	fsScriptIn >> m_fOPYBallYScale;
	fsScriptIn >> m_nOPPBallXPos;
	fsScriptIn >> m_nOPPBallYPos;
	fsScriptIn >> m_fOPPBallXScale;
	fsScriptIn >> m_fOPPBallYScale;
	
	fsScriptIn >> m_nOPResolutionXPos;
	fsScriptIn >> m_nOPResolutionYPos;
	fsScriptIn >> m_fOPResolutionXScale;
	fsScriptIn >> m_fOPResolutionYScale;
	fsScriptIn >> m_nOPResArrow1XPos;
	fsScriptIn >> m_nOPResArrow1YPos;
	fsScriptIn >> m_fOPResArrow1XScale;
	fsScriptIn >> m_fOPResArrow1YScale;
	fsScriptIn >> m_nOPResArrow2XPos;
	fsScriptIn >> m_nOPResArrow2YPos;
	fsScriptIn >> m_fOPResArrow2XScale;
	fsScriptIn >> m_fOPResArrow2YScale;
	fsScriptIn >> m_nOPResArrow3XPos;
	fsScriptIn >> m_nOPResArrow3YPos;
	fsScriptIn >> m_fOPResArrow3XScale;
	fsScriptIn >> m_fOPResArrow3YScale;
	//fsScriptIn >> m_nOPIBallXPos;
	//fsScriptIn >> m_nOPIBallYPos;
	//fsScriptIn >> m_fOPIBallXScale;
	//fsScriptIn >> m_fOPIBallYScale;

	//fsScriptIn >> m_nOPVBarXPos;
	//fsScriptIn >> m_nOPVBarYPos;
	//fsScriptIn >> m_fOPVBarXScale;
	//fsScriptIn >> m_fOPVBarYScale;
	//fsScriptIn >> m_nOPSBarXPos;
	//fsScriptIn >> m_nOPSBarYPos;
	//fsScriptIn >> m_fOPSBarXScale;
	//fsScriptIn >> m_fOPSBarYScale;
	//fsScriptIn >> m_nOPGBarXPos;
	//fsScriptIn >> m_nOPGBarYPos;
	//fsScriptIn >> m_fOPGBarXScale;
	//fsScriptIn >> m_fOPGBarYScale;
	//fsScriptIn >> m_nOPYBarXPos;
	//fsScriptIn >> m_nOPYBarYPos;
	//fsScriptIn >> m_fOPYBarXScale;
	//fsScriptIn >> m_fOPYBarYScale;
	//fsScriptIn >> m_nOPPBarXPos;
	//fsScriptIn >> m_nOPPBarYPos;
	//fsScriptIn >> m_fOPPBarXScale;
	//fsScriptIn >> m_fOPPBarYScale;
	//
	//fsScriptIn >> m_nOPBorderXPos;
	//fsScriptIn >> m_nOPBorderYPos;
	//fsScriptIn >> m_fOPBorderXScale;
	//fsScriptIn >> m_fOPBorderYScale;

	// Pause
	fsScriptIn >> m_nPSResumeXPos;
	fsScriptIn >> m_nPSResumeYPos;
	fsScriptIn >> m_fPSResumeXScale;
	fsScriptIn >> m_fPSResumeYScale;
	fsScriptIn >> m_nPSOptionsXPos;
	fsScriptIn >> m_nPSOptionsYPos;
	fsScriptIn >> m_fPSOptionsXScale;
	fsScriptIn >> m_fPSOptionsYScale;
	fsScriptIn >> m_nPSRestartXPos;
	fsScriptIn >> m_nPSRestartYPos;
	fsScriptIn >> m_fPSRestartXScale;
	fsScriptIn >> m_fPSRestartYScale;
	fsScriptIn >> m_nPSMainMenuXPos;
	fsScriptIn >> m_nPSMainMenuYPos;
	fsScriptIn >> m_fPSMainMenuXScale;
	fsScriptIn >> m_fPSMainMenuYScale;

	fsScriptIn >> m_nPSBorderXPos;
	fsScriptIn >> m_nPSBorderYPos;
	fsScriptIn >> m_fPSBorderXScale;
	fsScriptIn >> m_fPSBorderYScale;

	// Game Over
	fsScriptIn >> m_nGOContinueXPos;
	fsScriptIn >> m_nGOContinueYPos;
	fsScriptIn >> m_fGOContinueXScale;
	fsScriptIn >> m_fGOContinueYScale;
	fsScriptIn >> m_nGORestartXPos;
	fsScriptIn >> m_nGORestartYPos;
	fsScriptIn >> m_fGORestartXScale;
	fsScriptIn >> m_fGORestartYScale;
	fsScriptIn >> m_nGOMainMenuXPos;
	fsScriptIn >> m_nGOMainMenuYPos;
	fsScriptIn >> m_fGOMainMenuXScale;
	fsScriptIn >> m_fGOMainMenuYScale;

	fsScriptIn >> m_nGOBorderXPos;
	fsScriptIn >> m_nGOBorderYPos;
	fsScriptIn >> m_fGOBorderXScale;
	fsScriptIn >> m_fGOBorderYScale;

	// Confirmation
	fsScriptIn >> m_nCSYesXPos;
	fsScriptIn >> m_nCSYesYPos;
	fsScriptIn >> m_fCSYesXScale;
	fsScriptIn >> m_fCSYesYScale;
	fsScriptIn >> m_nCSNoXPos;
	fsScriptIn >> m_nCSNoYPos;
	fsScriptIn >> m_fCSNoXScale;
	fsScriptIn >> m_fCSNoYScale;

	fsScriptIn >> m_nCSBorderXPos;
	fsScriptIn >> m_nCSBorderYPos;
	fsScriptIn >> m_fCSBorderXScale;
	fsScriptIn >> m_fCSBorderYScale;

	return true;
}

bool CGuiHelper::ReadTextFile(LPCTSTR _szFilePath)
{
	string szTempFilePath = "Resources/Scripts/";
	szTempFilePath += _szFilePath;

	fstream fsScriptIn(szTempFilePath, std::ios_base::in);

	if(!fsScriptIn)
	{
#if _DEBUG
		cout << "Script did not load properly!" << '\n';
#endif
		return false;
	}

	// MAIN MENU
	fsScriptIn >> m_nMMPlayTextXPos;
	fsScriptIn >> m_nMMPlayTextYPos;
	fsScriptIn >> m_fMMPlayTextScale;
	fsScriptIn >> m_nMMOptionsTextXPos;
	fsScriptIn >> m_nMMOptionsTextYPos;
	fsScriptIn >> m_fMMOptionsTextScale;
	fsScriptIn >> m_nMMCreditsTextXPos;
	fsScriptIn >> m_nMMCreditsTextYPos;
	fsScriptIn >> m_fMMCreditsTextScale;
	fsScriptIn >> m_nMMExitTextXPos;
	fsScriptIn >> m_nMMExitTextYPos;
	fsScriptIn >> m_fMMExitTextScale;

	// OPTIONS
	fsScriptIn >> m_nOPCancelTextXPos;
	fsScriptIn >> m_nOPCancelTextYPos;
	fsScriptIn >> m_fOPCancelTextScale;
	fsScriptIn >> m_nOPDefaultTextXPos;
	fsScriptIn >> m_nOPDefaultTextYPos;
	fsScriptIn >> m_fOPDefaultTextScale;
	fsScriptIn >> m_nOPApplyTextXPos;
	fsScriptIn >> m_nOPApplyTextYPos;
	fsScriptIn >> m_fOPApplyTextScale;

	// PAUSE
	fsScriptIn >> m_nPSResumeTextXPos;
	fsScriptIn >> m_nPSResumeTextYPos;
	fsScriptIn >> m_fPSResumeTextScale;
	fsScriptIn >> m_nPSOptionsTextXPos;
	fsScriptIn >> m_nPSOptionsTextYPos;
	fsScriptIn >> m_fPSOptionsTextScale;
	fsScriptIn >> m_nPSRestartTextXPos;
	fsScriptIn >> m_nPSRestartTextYPos;
	fsScriptIn >> m_fPSRestartTextScale;
	fsScriptIn >> m_nPSMainMenuTextXPos;
	fsScriptIn >> m_nPSMainMenuTextYPos;
	fsScriptIn >> m_fPSMainMenuTextScale;

	// GAME OVER
	fsScriptIn >> m_nGOContinueTextXPos;
	fsScriptIn >> m_nGOContinueTextYPos;
	fsScriptIn >> m_fGOContinueTextScale;
	fsScriptIn >> m_nGORestartTextXPos;
	fsScriptIn >> m_nGORestartTextYPos;
	fsScriptIn >> m_fGORestartTextScale;
	fsScriptIn >> m_nGOMainMenuTextXPos;
	fsScriptIn >> m_nGOMainMenuTextYPos;
	fsScriptIn >> m_fGOMainMenuTextScale;

	// CONFIRMATION
	fsScriptIn >> m_nCSYesTextXPos;
	fsScriptIn >> m_nCSYesTextYPos;
	fsScriptIn >> m_fCSYesTextScale;
	fsScriptIn >> m_nCSNoTextXPos;
	fsScriptIn >> m_nCSNoTextYPos;
	fsScriptIn >> m_fCSNoTextScale;

	// RESOLUTION
	fsScriptIn >> m_n1024x768XPos;
	fsScriptIn >> m_n1024x768YPos;
	fsScriptIn >> m_f1024x768Scale;
	fsScriptIn >> m_n1680x1050XPos;
	fsScriptIn >> m_n1680x1050YPos;
	fsScriptIn >> m_f1680x1050Scale;
	fsScriptIn >> m_n1920x1080XPos;
	fsScriptIn >> m_n1920x1080YPos;
	fsScriptIn >> m_f1920x1080Scale;

	return true;
}
