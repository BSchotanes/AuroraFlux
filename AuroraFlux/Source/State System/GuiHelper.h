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
#ifndef CGUIHELPER_H
#define CGUIHELPER_H

class CGuiHelper
{
	/**********************************/
	/**************BUTTONS*************/
	/**********************************/
	// Main Menu Buttons
	int m_nMMPlayXPos;
	int m_nMMPlayYPos;
	float m_fMMPlayXScale;
	float m_fMMPlayYScale;
	int m_nMMOptionsXPos;
	int m_nMMOptionsYPos;
	float m_fMMOptionsXScale;
	float m_fMMOptionsYScale;
	int m_nMMCreditsXPos;
	int m_nMMCreditsYPos;
	float m_fMMCreditsXScale;
	float m_fMMCreditsYScale;
	int m_nMMExitXPos;
	int m_nMMExitYPos;
	float m_fMMExitXScale;
	float m_fMMExitYScale;
	int m_nMMBorderXPos;
	int m_nMMBorderYPos;
	float m_fMMBorderXScale;
	float m_fMMBorderYScale;

	// Pause Menu Buttons
	int   m_nPSResumeXPos;
	int   m_nPSResumeYPos;
	float m_fPSResumeXScale;
	float m_fPSResumeYScale;
	int   m_nPSOptionsXPos;
	int   m_nPSOptionsYPos;
	float m_fPSOptionsXScale;
	float m_fPSOptionsYScale;
	int   m_nPSRestartXPos;
	int   m_nPSRestartYPos;
	float m_fPSRestartXScale;
	float m_fPSRestartYScale;
	int   m_nPSMainMenuXPos;
	int   m_nPSMainMenuYPos;
	float m_fPSMainMenuXScale;
	float m_fPSMainMenuYScale;
	int   m_nPSBorderXPos;
	int   m_nPSBorderYPos;
	float m_fPSBorderXScale;
	float m_fPSBorderYScale;

	// Game Over Buttons
	int m_nGOContinueXPos;
	int m_nGOContinueYPos;
	float m_fGOContinueXScale;
	float m_fGOContinueYScale;
	int m_nGORestartXPos;
	int m_nGORestartYPos;
	float m_fGORestartXScale;
	float m_fGORestartYScale;
	int m_nGOMainMenuXPos;
	int m_nGOMainMenuYPos;
	float m_fGOMainMenuXScale;
	float m_fGOMainMenuYScale;
	int m_nGOBorderXPos;
	int m_nGOBorderYPos;
	float m_fGOBorderXScale;
	float m_fGOBorderYScale;

	// Confirmation Buttons
	int m_nCSYesXPos;
	int m_nCSYesYPos;
	float m_fCSYesXScale;
	float m_fCSYesYScale;
	int m_nCSNoXPos;
	int m_nCSNoYPos;
	float m_fCSNoXScale;
	float m_fCSNoYScale;
	int m_nCSBorderXPos;
	int m_nCSBorderYPos;
	float m_fCSBorderXScale;
	float m_fCSBorderYScale;

	// Options Buttons
	int   m_nOPMainXPos;
	int   m_nOPMainYPos;
	float m_fOPMainXScale;
	float m_fOPMainYScale;
	int   m_nOPInvertYXPos;
	int   m_nOPInvertYYPos;
	float m_fOPInvertYXScale;
	float m_fOPInvertYYScale;
	int   m_nOPInvertXXPos;
	int   m_nOPInvertXYPos;
	float m_fOPInvertXXScale;
	float m_fOPInvertXYScale;
	int   m_nOPCancelXPos;
	int   m_nOPCancelYPos;
	float m_fOPCancelXScale;
	float m_fOPCancelYScale;
	int	  m_nOPDefaultXPos;
	int	  m_nOPDefaultYPos;
	float m_fOPDefaultXScale;
	float m_fOPDefaultYScale;
	int	  m_nOPApplyXPos;
	int	  m_nOPApplyYPos;
	float m_fOPApplyXScale;
	float m_fOPApplyYScale;
	int	  m_nOPInvertYBoxXPos;
	int	  m_nOPInvertYBoxYPos;
	float m_fOPInvertYBoxXScale;
	float m_fOPInvertYBoxYScale;
	int	  m_nOPInvertXBoxXPos;
	int	  m_nOPInvertXBoxYPos;
	float m_fOPInvertXBoxXScale;
	float m_fOPInvertXBoxYScale;
	int	  m_nOPVolumeBallXPos;
	int	  m_nOPVolumeBallYPos;
	float m_fOPVolumeBallXScale;
	float m_fOPVolumeBallYScale;
	int	  m_nOPVolumeBallSelectedXPos;
	int	  m_nOPVolumeBallSelectedYPos;
	float m_fOPVolumeBallSelectedXScale;
	float m_fOPVolumeBallSelectedYScale;

	int	  m_nOPResolutionXPos;
	int	  m_nOPResolutionYPos;
	float m_fOPResolutionXScale;
	float m_fOPResolutionYScale;
	int	  m_nOPResArrow1XPos;
	int	  m_nOPResArrow1YPos;
	float m_fOPResArrow1XScale;
	float m_fOPResArrow1YScale;
	int	  m_nOPResArrow2XPos;
	int	  m_nOPResArrow2YPos;
	float m_fOPResArrow2XScale;
	float m_fOPResArrow2YScale;
	int	  m_nOPResArrow3XPos;
	int	  m_nOPResArrow3YPos;
	float m_fOPResArrow3XScale;
	float m_fOPResArrow3YScale;

	int	  m_nOPLeftBorderXPos;
	int	  m_nOPLeftBorderYPos;
	float m_fOPLeftBorderXScale;
	float m_fOPLeftBorderYScale;
	int	  m_nOPRightBorderXPos;
	int	  m_nOPRightBorderYPos;
	float m_fOPRightBorderXScale;
	float m_fOPRightBorderYScale;

	int	  m_nOPSBallXPos;
	int	  m_nOPSBallYPos;
	float m_fOPSBallXScale;
	float m_fOPSBallYScale;
	int	  m_nOPGBallXPos;
	int	  m_nOPGBallYPos;
	float m_fOPGBallXScale;
	float m_fOPGBallYScale;
	int	  m_nOPYBallXPos;
	int	  m_nOPYBallYPos;
	float m_fOPYBallXScale;
	float m_fOPYBallYScale;
	int	  m_nOPPBallXPos;
	int	  m_nOPPBallYPos;
	float m_fOPPBallXScale;
	float m_fOPPBallYScale;
	//
	//int	m_nOPVBarXPos;
	//int	m_nOPVBarYPos;
	//float m_fOPVBarXScale;
	//float m_fOPVBarYScale;
	//int	m_nOPSBarXPos;
	//int	m_nOPSBarYPos;
	//float m_fOPSBarXScale;
	//float m_fOPSBarYScale;
	//int	m_nOPGBarXPos;
	//int	m_nOPGBarYPos; 
	//float m_fOPGBarXScale;
	//float m_fOPGBarYScale;
	//int	m_nOPYBarXPos;
	//int	m_nOPYBarYPos;
	//float m_fOPYBarXScale;
	//float m_fOPYBarYScale;
	//int	m_nOPPBarXPos;
	//int	m_nOPPBarYPos;
	//float m_fOPPBarXScale;
	//float m_fOPPBarYScale;
	//
	//int m_nOPBorderXPos;
	//int m_nOPBorderYPos;
	//float m_fOPBorderXScale;
	//float m_fOPBorderYScale;

	/**********************************/
	/**************TEXT****************/
	/**********************************/
	// MAIN MENU
	int   m_nMMPlayTextXPos;
	int	  m_nMMPlayTextYPos;
	float m_fMMPlayTextScale;
	int   m_nMMOptionsTextXPos;
	int	  m_nMMOptionsTextYPos;
	float m_fMMOptionsTextScale;
	int   m_nMMCreditsTextXPos;
	int	  m_nMMCreditsTextYPos;
	float m_fMMCreditsTextScale;
	int   m_nMMExitTextXPos;
	int	  m_nMMExitTextYPos;
	float m_fMMExitTextScale;

	// OPTIONS
	int   m_nOPCancelTextXPos;
	int	  m_nOPCancelTextYPos;
	float m_fOPCancelTextScale;
	int   m_nOPDefaultTextXPos;
	int	  m_nOPDefaultTextYPos;
	float m_fOPDefaultTextScale;
	int   m_nOPApplyTextXPos;
	int	  m_nOPApplyTextYPos;
	float m_fOPApplyTextScale;

	// PAUSE
	int   m_nPSResumeTextXPos;
	int	  m_nPSResumeTextYPos;
	float m_fPSResumeTextScale;
	int   m_nPSOptionsTextXPos;
	int	  m_nPSOptionsTextYPos;
	float m_fPSOptionsTextScale;
	int   m_nPSRestartTextXPos;
	int	  m_nPSRestartTextYPos;
	float m_fPSRestartTextScale;
	int   m_nPSMainMenuTextXPos;
	int	  m_nPSMainMenuTextYPos;
	float m_fPSMainMenuTextScale;

	// GAME OVER
	int   m_nGOContinueTextXPos;
	int	  m_nGOContinueTextYPos;
	float m_fGOContinueTextScale;
	int   m_nGORestartTextXPos;
	int	  m_nGORestartTextYPos;
	float m_fGORestartTextScale;
	int   m_nGOMainMenuTextXPos;
	int	  m_nGOMainMenuTextYPos;
	float m_fGOMainMenuTextScale;

	// CONFIRMATION
	int   m_nCSYesTextXPos;
	int	  m_nCSYesTextYPos;
	float m_fCSYesTextScale;
	int   m_nCSNoTextXPos;
	int	  m_nCSNoTextYPos;
	float m_fCSNoTextScale;

	// Resolution
	int   m_n1024x768XPos;
	int	  m_n1024x768YPos;
	float m_f1024x768Scale;
	int   m_n1680x1050XPos;
	int	  m_n1680x1050YPos;
	float m_f1680x1050Scale;
	int   m_n1920x1080XPos;
	int	  m_n1920x1080YPos;
	float m_f1920x1080Scale;

public:
	/*****************************************************************
	* CGuiHelper():  Default Constructor. Will set its type and behaviors.
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	CGuiHelper(void);

	/*****************************************************************
	* ~CGuiHelper():  Virtual destructor used when shutting down the game state
	* Ins:			    none    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	~CGuiHelper(void);

	/*****************************************************************
	* ReadFile(): Function that reads in a text file that has the GUI information
	and fills out the variables
	* Ins:					none    		      
	* Outs:					None
	* Returns:				void
	* Mod. Date:			1/12/2013
	* Mod. Initials:		AG
	*****************************************************************/
	bool ReadFile(LPCTSTR _szFilePath);

	/*****************************************************************
	* ReadTextFile(): Function that reads in a text file that has the GUI text information
	and fills out the variables
	* Ins:					none    		      
	* Outs:					None
	* Returns:				void
	* Mod. Date:			2/13/2013
	* Mod. Initials:		AG
	*****************************************************************/
	bool ReadTextFile(LPCTSTR _szFilePath);

	/*************************************/
	/*           ACCESSORS               */
	/*************************************/
	/**********************************/
	/**************BUTTONS*************/
	/**********************************/
	// Main Menu
	int GetMMPlayXPos() {return m_nMMPlayXPos;}
	int GetMMPlayYPos() {return m_nMMPlayYPos;}
	float GetMMPlayXScale() {return m_fMMPlayXScale;}
	float GetMMPlayYScale() {return m_fMMPlayYScale;}
	int GetMMOptionsXPos() {return m_nMMOptionsXPos;}
	int GetMMOptionsYPos() {return m_nMMOptionsYPos;}
	float GetMMOptionsXScale() {return m_fMMOptionsXScale;}
	float GetMMOptionsYScale() {return m_fMMOptionsYScale;}
	int GetMMCreditsXPos() {return m_nMMCreditsXPos;}
	int GetMMCreditsYPos() {return m_nMMCreditsYPos;}
	float GetMMCreditsXScale() {return m_fMMCreditsXScale;}
	float GetMMCreditsYScale() {return m_fMMCreditsYScale;}
	int GetMMExitXPos() {return m_nMMExitXPos;}
	int GetMMExitYPos() {return m_nMMExitYPos;}
	float GetMMExitXScale() {return m_fMMExitXScale;}
	float GetMMExitYScale() {return m_fMMExitYScale;}
	int GetMMBorderXPos() {return m_nMMBorderXPos;}
	int GetMMBorderYPos() {return m_nMMBorderYPos;}
	float GetMMBorderXScale() {return m_fMMBorderXScale;}
	float GetMMBorderYScale() {return m_fMMBorderYScale;}

	// Options
	int   GetOPMainXPos()   {return m_nOPMainXPos;}
	int   GetOPMainYPos()   {return m_nOPMainYPos;}
	float GetOPMainXScale() {return m_fOPMainXScale;}
	float GetOPMainYScale() {return m_fOPMainYScale;}
	int   GetOPInvertYXPos()   {return m_nOPInvertYXPos;}
	int   GetOPInvertYYPos()   {return m_nOPInvertYYPos;}
	float GetOPInvertYXScale() {return m_fOPInvertYXScale;}
	float GetOPInvertYYScale() {return m_fOPInvertYYScale;}
	int   GetOPInvertXXPos()   {return m_nOPInvertXXPos;}
	int   GetOPInvertXYPos()   {return m_nOPInvertXYPos;}
	float GetOPInvertXXScale() {return m_fOPInvertXXScale;}
	float GetOPInvertXYScale() {return m_fOPInvertXYScale;}
	int   GetOPCancelXPos()   {return m_nOPCancelXPos;}
	int   GetOPCancelYPos()   {return m_nOPCancelYPos;}
	float GetOPCancelXScale() {return m_fOPCancelXScale;}
	float GetOPCancelYScale() {return m_fOPCancelYScale;}
	int   GetOPDefaultXPos()   {return m_nOPDefaultXPos;}
	int   GetOPDefaultYPos()   {return m_nOPDefaultYPos;}
	float GetOPDefaultXScale() {return m_fOPDefaultXScale;}
	float GetOPDefaultYScale() {return m_fOPDefaultYScale;}
	int   GetOPApplyXPos()   {return m_nOPApplyXPos;}
	int   GetOPApplyYPos()   {return m_nOPApplyYPos;}
	float GetOPApplyXScale() {return m_fOPApplyXScale;}
	float GetOPApplyYScale() {return m_fOPApplyYScale;}
	int   GetOPInvertYBoxXPos()   {return m_nOPInvertYBoxXPos;}
	int   GetOPInvertYBoxYPos()   {return m_nOPInvertYBoxYPos;}
	float GetOPInvertYBoxXScale() {return m_fOPInvertYBoxXScale;}
	float GetOPInvertYBoxYScale() {return m_fOPInvertYBoxYScale;}
	int   GetOPInvertXBoxXPos()   {return m_nOPInvertXBoxXPos;}
	int   GetOPInvertXBoxYPos()   {return m_nOPInvertXBoxYPos;}
	float GetOPInvertXBoxXScale() {return m_fOPInvertXBoxXScale;}
	float GetOPInvertXBoxYScale() {return m_fOPInvertXBoxYScale;}
	int   GetOPVolumeBallXPos()   {return m_nOPVolumeBallXPos;}
	int   GetOPVolumeBallYPos()   {return m_nOPVolumeBallYPos;}
	float GetOPVolumeBallXScale() {return m_fOPVolumeBallXScale;}
	float GetOPVolumeBallYScale() {return m_fOPVolumeBallYScale;}
	int   GetOPVolumeBallSelectedXPos()   {return m_nOPVolumeBallSelectedXPos;}
	int   GetOPVolumeBallSelectedYPos()   {return m_nOPVolumeBallSelectedYPos;}
	float GetOPVolumeBallSelectedXScale() {return m_fOPVolumeBallSelectedXScale;}
	float GetOPVolumeBallSelectedYScale() {return m_fOPVolumeBallSelectedYScale;}
	int   GetOPLeftBorderXPos()   {return m_nOPLeftBorderXPos;}
	int   GetOPLeftBorderYPos()   {return m_nOPLeftBorderYPos;}
	float GetOPLeftBorderXScale() {return m_fOPLeftBorderXScale;}
	float GetOPLeftBorderYScale() {return m_fOPLeftBorderYScale;}
	int   GetOPRightBorderXPos()   {return m_nOPRightBorderXPos;}
	int   GetOPRightBorderYPos()   {return m_nOPRightBorderYPos;}
	float GetOPRightBorderXScale() {return m_fOPRightBorderXScale;}
	float GetOPRightBorderYScale() {return m_fOPRightBorderYScale;}

	int   GetOPResolutionXPos()   {return m_nOPResolutionXPos;}
	int   GetOPResolutionYPos()   {return m_nOPResolutionYPos;}
	float GetOPResolutionXScale() {return m_fOPResolutionXScale;}
	float GetOPResolutionYScale() {return m_fOPResolutionYScale;}
	int   GetOPResArrowOneXPos()   {return m_nOPResArrow1XPos;}
	int   GetOPResArrowOneYPos()   {return m_nOPResArrow1YPos;}
	float GetOPResArrowOneXScale() {return m_fOPResArrow1XScale;}
	float GetOPResArrowOneYScale() {return m_fOPResArrow1YScale;}
	int   GetOPResArrowTwoXPos()   {return m_nOPResArrow2XPos;}
	int   GetOPResArrowTwoYPos()   {return m_nOPResArrow2YPos;}
	float GetOPResArrowTwoXScale() {return m_fOPResArrow2XScale;}
	float GetOPResArrowTwoYScale() {return m_fOPResArrow2YScale;}
	int   GetOPResArrowThreeXPos()   {return m_nOPResArrow3XPos;}
	int   GetOPResArrowThreeYPos()   {return m_nOPResArrow3YPos;}
	float GetOPResArrowThreeXScale() {return m_fOPResArrow3XScale;}
	float GetOPResArrowThreeYScale() {return m_fOPResArrow3YScale;}

	int   GetOPSFXXPos()   {return m_nOPSBallXPos;}
	int   GetOPSFXYPos()   {return m_nOPSBallYPos;}
	float GetOPSFXXScale() {return m_fOPSBallXScale;}
	float GetOPSFXYScale() {return m_fOPSBallYScale;}
	int   GetOPGammaXPos()   {return m_nOPGBallXPos;}
	int   GetOPGammaYPos()   {return m_nOPGBallYPos;}
	float GetOPGammaXScale() {return m_fOPGBallXScale;}
	float GetOPGammaYScale() {return m_fOPGBallYScale;}
	int   GetOPYawXPos()   {return m_nOPYBallXPos;}
	int   GetOPYawYPos()   {return m_nOPYBallYPos;}
	float GetOPYawXScale() {return m_fOPYBallXScale;}
	float GetOPYawYScale() {return m_fOPYBallYScale;}
	int   GetOPPitchXPos()   {return m_nOPPBallXPos;}
	int   GetOPPitchYPos()   {return m_nOPPBallYPos;}
	float GetOPPitchXScale() {return m_fOPPBallXScale;}
	float GetOPPitchYScale() {return m_fOPPBallYScale;}
	//int   GetOPIBallXPos()   {return m_nOPIBallXPos;}
	//int   GetOPIBallYPos()   {return m_nOPIBallYPos;}
	//float GetOPIBallXScale() {return m_fOPIBallXScale;}
	//float GetOPIBallYScale() {return m_fOPIBallYScale;}

	//int GetOPVBarXPos()	{return m_nOPVBarXPos;}
	//int GetOPVBarYPos()	{return m_nOPVBarYPos;}
	//float GetOPVBarXScale() {return m_fOPVBarXScale;}
	//float GetOPVBarYScale() {return m_fOPVBarYScale;}
	//int GetOPSBarXPos()	{return m_nOPSBarXPos;}
	//int GetOPSBarYPos()	{return m_nOPSBarYPos;}
	//float GetOPSBarXScale() {return m_fOPSBarXScale;}
	//float GetOPSBarYScale() {return m_fOPSBarYScale;}
	//int GetOPGBarXPos()	{return m_nOPGBarXPos;}
	//int GetOPGBarYPos()	{return m_nOPGBarYPos;}
	//float GetOPGBarXScale() {return m_fOPGBarXScale;}
	//float GetOPGBarYScale() {return m_fOPGBarYScale;}
	//int GetOPYBarXPos()	{return m_nOPYBarXPos;}
	//int GetOPYBarYPos()	{return m_nOPYBarYPos;}
	//float GetOPYBarXScale() {return m_fOPYBarXScale;}
	//float GetOPYBarYScale() {return m_fOPYBarYScale;}
	//int GetOPPBarXPos()	{return m_nOPPBarXPos;}
	//int GetOPPBarYPos()	{return m_nOPPBarYPos;}
	//float GetOPPBarXScale() {return m_fOPPBarXScale;}
	//float GetOPPBarYScale() {return m_fOPPBarYScale;}
	//
	//int GetOPBorderXPos() {return m_nOPBorderXPos;}
	//int GetOPBorderYPos() {return m_nOPBorderYPos;}
	//float GetOPBorderXScale() {return m_fOPBorderXScale;}
	//float GetOPBorderYScale() {return m_fOPBorderYScale;}

	// Pause
	int GetPSResumeXPos() {return m_nPSResumeXPos;}
	int GetPSResumeYPos() {return m_nPSResumeYPos;}
	float GetPSResumeXScale() {return m_fPSResumeXScale;}
	float GetPSResumeYScale() {return m_fPSResumeYScale;}
	int GetPSRestartXPos() {return m_nPSRestartXPos;}
	int GetPSRestartYPos() {return m_nPSRestartYPos;}
	float GetPSRestartXScale() {return m_fPSRestartXScale;}
	float GetPSRestartYScale() {return m_fPSRestartYScale;}
	int GetPSMainMenuXPos() {return m_nPSMainMenuXPos;}
	int GetPSMainMenuYPos() {return m_nPSMainMenuYPos;}
	float GetPSMainMenuXScale() {return m_fPSMainMenuXScale;}
	float GetPSMainMenuYScale() {return m_fPSMainMenuYScale;}
	int GetPSOptionsXPos() {return m_nPSOptionsXPos;}
	int GetPSOptionsYPos() {return m_nPSOptionsYPos;}
	float GetPSOptionsXScale() {return m_fPSOptionsXScale;}
	float GetPSOptionsYScale() {return m_fPSOptionsYScale;}

	int GetPSBorderXPos() {return m_nPSBorderXPos;}
	int GetPSBorderYPos() {return m_nPSBorderYPos;}
	float GetPSBorderXScale() {return m_fPSBorderXScale;}
	float GetPSBorderYScale() {return m_fPSBorderYScale;}

	// Game Over
	int GetGOContinueXPos() {return m_nGOContinueXPos;}
	int GetGOContinueYPos() {return m_nGOContinueYPos;}
	float GetGOContinueXScale() {return m_fGOContinueXScale;}
	float GetGOContinueYScale() {return m_fGOContinueYScale;}
	int GetGORestartXPos() {return m_nGORestartXPos;}
	int GetGORestartYPos() {return m_nGORestartYPos;}
	float GetGORestartXScale() {return m_fGORestartXScale;}
	float GetGORestartYScale() {return m_fGORestartYScale;}
	int GetGOMainMenuXPos() {return m_nGOMainMenuXPos;}
	int GetGOMainMenuYPos() {return m_nGOMainMenuYPos;}
	float GetGOMainMenuXScale() {return m_fGOMainMenuXScale;}
	float GetGOMainMenuYScale() {return m_fGOMainMenuYScale;}

	int GetGOBorderXPos() {return m_nGOBorderXPos;}
	int GetGOBorderYPos() {return m_nGOBorderYPos;}
	float GetGOBorderXScale() {return m_fGOBorderXScale;}
	float GetGOBorderYScale() {return m_fGOBorderYScale;}

	// Confirmation
	int GetCSYesXPos() {return m_nCSYesXPos;}
	int GetCSYesYPos() {return m_nCSYesYPos;}
	float GetCSYesXScale() {return m_fCSYesXScale;}
	float GetCSYesYScale() {return m_fCSYesYScale;}
	int GetCSNoXPos() {return m_nCSNoXPos;}
	int GetCSNoYPos() {return m_nCSNoYPos;}
	float GetCSNoXScale() {return m_fCSNoXScale;}
	float GetCSNoYScale() {return m_fCSNoYScale;}

	int GetCSBorderXPos() {return m_nCSBorderXPos;}
	int GetCSBorderYPos() {return m_nCSBorderYPos;}
	float GetCSBorderXScale() {return m_fCSBorderXScale;}
	float GetCSBorderYScale() {return m_fCSBorderYScale;}

	/**********************************/
	/**************TEXT****************/
	/**********************************/
	// MAIN MENU
	int   GetMMPlayTextXPos() {return m_nMMPlayTextXPos;}
	int   GetMMPlayTextYPos() {return m_nMMPlayTextYPos;}
	float GetMMPlayTextScale() {return m_fMMPlayTextScale;}
	int   GetMMOptionsTextXPos() {return m_nMMOptionsTextXPos;}
	int   GetMMOptionsTextYPos() {return m_nMMOptionsTextYPos;}
	float GetMMOptionsTextScale() {return m_fMMOptionsTextScale;}
	int   GetMMCreditsTextXPos() {return m_nMMCreditsTextXPos;}
	int   GetMMCreditsTextYPos() {return m_nMMCreditsTextYPos;}
	float GetMMCreditsTextScale() {return m_fMMCreditsTextScale;}
	int   GetMMExitTextXPos() {return m_nMMExitTextXPos;}
	int   GetMMExitTextYPos() {return m_nMMExitTextYPos;}
	float GetMMExitTextScale() {return m_fMMExitTextScale;}

	// OPTIONS
	int   GetOPCancelTextXPos() {return  m_nOPCancelTextXPos;}
	int   GetOPCancelTextYPos() {return  m_nOPCancelTextYPos;}
	float GetOPCancelTextScale() {return m_fOPCancelTextScale;}
	int   GetOPDefaultTextXPos() {return  m_nOPDefaultTextXPos;}
	int   GetOPDefaultTextYPos() {return  m_nOPDefaultTextYPos;}
	float GetOPDefaultTextScale() {return m_fOPDefaultTextScale;}
	int   GetOPApplyTextXPos() {return  m_nOPApplyTextXPos;}
	int   GetOPApplyTextYPos() {return  m_nOPApplyTextYPos;}
	float GetOPApplyTextScale() {return m_fOPApplyTextScale;}

	// PAUSE
	int   GetPSResumeTextXPos() {return  m_nPSResumeTextXPos;}
	int   GetPSResumeTextYPos() {return  m_nPSResumeTextYPos;}
	float GetPSResumeTextScale() {return m_fPSResumeTextScale;}
	int   GetPSOptionsTextXPos() {return  m_nPSOptionsTextXPos;}
	int   GetPSOptionsTextYPos() {return  m_nPSOptionsTextYPos;}
	float GetPSOptionsTextScale() {return m_fPSOptionsTextScale;}
	int   GetPSRestartTextXPos() {return  m_nPSRestartTextXPos;}
	int   GetPSRestartTextYPos() {return  m_nPSRestartTextYPos;}
	float GetPSRestartTextScale() {return m_fPSRestartTextScale;}
	int   GetPSMainMenuTextXPos() {return  m_nPSMainMenuTextXPos;}
	int   GetPSMainMenuTextYPos() {return  m_nPSMainMenuTextYPos;}
	float GetPSMainMenuTextScale() {return m_fPSMainMenuTextScale;}

	// GAME OVER
	int   GetGOContinueTextXPos() {return  m_nGOContinueTextXPos;}
	int   GetGOContinueTextYPos() {return  m_nGOContinueTextYPos;}
	float GetGOContinueTextScale() {return m_fGOContinueTextScale;}
	int   GetGORestartTextXPos() {return  m_nGORestartTextXPos;}
	int   GetGORestartTextYPos() {return  m_nGORestartTextYPos;}
	float GetGORestartTextScale() {return m_fGORestartTextScale;}
	int   GetGOMainMenuTextXPos() {return  m_nGOMainMenuTextXPos;}
	int   GetGOMainMenuTextYPos() {return  m_nGOMainMenuTextYPos;}
	float GetGOMainMenuTextScale() {return m_fGOMainMenuTextScale;}

	// CONFIRMATION
	int   GetCSYesTextXPos() {return  m_nCSYesTextXPos;}
	int   GetCSYesTextYPos() {return  m_nCSYesTextYPos;}
	float GetCSYesTextScale() {return m_fCSYesTextScale;}
	int   GetCSNoTextXPos() {return  m_nCSNoTextXPos;}
	int   GetCSNoTextYPos() {return  m_nCSNoTextYPos;}
	float GetCSNoTextScale() {return m_fCSNoTextScale;}

	// GAME OVER
	int   Get1024x768XPos() {return m_n1024x768XPos;}
	int   Get1024x768YPos() {return m_n1024x768YPos;}
	float Get1024x768Scale() {return m_f1024x768Scale;}
	int   Get1680x1050XPos() {return m_n1680x1050XPos;}
	int   Get1680x1050YPos() {return m_n1680x1050YPos;}
	float Get1680x1050Scale() {return m_f1680x1050Scale;}
	int   Get1920x1080XPos() {return m_n1920x1080XPos;}
	int   Get1920x1080YPos() {return m_n1920x1080YPos;}
	float Get1920x1080Scale() {return m_f1920x1080Scale;}	

	/*****************************/
	/*		   MUTATORS          */
	/*****************************/
	void SetVBallXPos(int _nXPos) {m_nOPVolumeBallXPos = _nXPos;}
	void SetSBallXPos(int _nXPos) {m_nOPSBallXPos = _nXPos;}
	void SetGBallXPos(int _nXPos) {m_nOPGBallXPos = _nXPos;}
	void SetYBallXPos(int _nXPos) {m_nOPYBallXPos = _nXPos;}
	void SetPBallXPos(int _nXPos) {m_nOPPBallXPos = _nXPos;}

};

#endif