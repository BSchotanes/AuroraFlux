/***********************************************
* Filename:  		CDialogSystem.h
* Mod. Date: 		1/30/2013
* Mod. Date: 		1/30/2013
* Mod. Initials:	JW
* Author:    		Jayson S. Wu
* Purpose:   		Dialog event handler class for the game
************************************************/
#ifndef DIALOG_SYSTEM_H_
#define DIALOG_SYSTEM_H_

#include "../Renderer/CRenderer.h"

#define NUM_DIALOG 50

enum eDialogStartPoints { eDialog_None = 0, eDialog_Start, eDialog_Boss, eDialog_BossShieldGen, eDialog_BossTurret, 
							eDialog_Asteroids, eDialog_Speed, eDialog_Dodge, eDialog_Controls, eDialog_EnemyArrow, eDialog_Waypoint, 
							eDialog_RedEnemy, eDialog_BlueEnemy, eDialog_PressQ, eDialog_TurnAround, eDialog_FirstBlood, eTutorial_Speed, 
							eTutorial_Laser, eTutorial_SwitchState, eTutorial_FireMissile, eTutorial_Great_Job, eTutorial_Blue_Fire, 
							eTutorial_Blue_Dodge, eTutorial_Waypoint, eTutorial_Waypoint_Arrow, eTutorial_DifficultInstruction, eDialog_Max};
	
/***********************************************
* Structure:		tDialogNode
* Mod. Date: 		1/30/2013
* Mod. Initials:	JW
* Author:    		Jayson S. Wu
* Purpose:   		Node that holds text, position, 
*					scale and time for the dialog to be rendered
************************************************/
struct tDialogNode
{
	char*	m_pchDialogText;
	float	m_fWaitTime;
	int		m_nXPosition;
	int		m_nYPosition;
	float	m_fTextScale;
	
	void SetNodes(char* pchText, float fTime, int nX, int nY, float fScale);
};

/***********************************************
* Structure:		tSection
* Mod. Date: 		1/30/2013
* Mod. Initials:	JW
* Author:    		Jayson S. Wu
* Purpose:   		Start and end point of a section
************************************************/
struct tSection
{
	int m_nStartPoint, m_nEndPoint;
	void SetPoints(int _Start, int _End);
};

class CDialogSystem
{
	tSection			m_arrDialogSections[eDialog_Max];
	tDialogNode			m_arrDialog[NUM_DIALOG];
	
	eDialogStartPoints	eCurrentPoint;
	float				m_fCurrentDialogTime;
	int					m_nCurrentDialog;
	int					m_nMaxNumDialog;
	bool				m_bSectionDone;
	bool				m_bChanging;
	bool				m_bHasPlayedSound;
	int m_nWTexture;
	int m_nATexture;
	int m_nSTexture;
	int m_nDTexture;
	int m_nQTexture;
	int m_nLeftMouseTexture;
	int m_nRightMouseTexture;

	CRenderer* m_pRenderer;
	//int diffx;
	//int diffy;
public:
	/*****************************************************************
	* CDialogSystem():  Default constructor
	*
	* Ins:						Void
	* Outs:						None
	* Returns:					void
	*
	* Mod. Date:				1/30/2013
	* Mod. Initials:			JW
	*****************************************************************/
	CDialogSystem(void);

	/*****************************************************************
	* ~CDialogSystem():  Default destructor
	*
	* Ins:						Void
	* Outs:						None
	* Returns:					void
	*
	* Mod. Date:				1/30/2013
	* Mod. Initials:			JW
	*****************************************************************/
	~CDialogSystem(void){}

	// Maybe make it load text from files in the future
	/*****************************************************************
	* LoadDialog():  Function for loading files
	*
	* Ins:						char* _szFilename - Name of file you want to load
	* Outs:						None
	* Returns:					void
	*
	* Mod. Date:				1/30/2013
	* Mod. Initials:			JW
	*****************************************************************/
	//LoadDialog(char* _szFilename);

	/*****************************************************************
	* Initialize():  Initializing our Dialog system
	*
	* Ins:						char* _szFilename - Name of file you want to load
								If "" is passed in, it will load the default texts
	* Outs:						None
	* Returns:					int	-	
	*
	* Mod. Date:				1/30/2013
	* Mod. Initials:			JW
	*****************************************************************/
	int Initialize(char* _filename, CRenderer* _pRenderer);

	/*****************************************************************
	* Update():		Update function
	*
	* Ins:						float _fElapsedTime - time for update
	* Outs:						None
	* Returns:					int - 0 if sucessful
	*
	* Mod. Date:				1/30/2013
	* Mod. Initials:			JW
	*****************************************************************/
	void Update(float _fElapsedTime);

	/*****************************************************************
	* Render():  Render function
	*
	* Ins:						CRenderer* _pRenderer - Pass in the renderer for
									rendering our text.
	* Outs:						None
	* Returns:					void
	*
	* Mod. Date:				1/30/2013
	* Mod. Initials:			JW
	*****************************************************************/
	void Render();
	
	/*****************************************************************
	* ChangeSection(): Gets dialog system ready for changing between two dialogNodes
	*
	* Ins:						eDialogStartPoints NewPoint - Which section of dialog to use
	* Outs:						None
	* Returns:					void
	*
	* Mod. Date:				1/30/2013
	* Mod. Initials:			JW
	*****************************************************************/
	void ChangeSection(eDialogStartPoints NewPoint) {m_bChanging = true; eCurrentPoint = NewPoint;}

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Accessors
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	bool GetSectionDone()					{return m_bSectionDone;}
	int GetCurrDialog()						{return m_nCurrentDialog;}
	int GetMaxDialogs()						{return m_nMaxNumDialog;}
	float GetCurrDialogTime()				{return m_fCurrentDialogTime;}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Mutators
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetCurrDialogTime(float TimeToSet)	{m_fCurrentDialogTime = TimeToSet;}
	void SetMaxDialogs(int MaxToSet)		{m_nMaxNumDialog = MaxToSet;}
	void SetCurrDialog(int DialogToUse)		{m_nCurrentDialog = DialogToUse;}
};
#endif