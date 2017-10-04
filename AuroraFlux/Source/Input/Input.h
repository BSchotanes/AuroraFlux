/***********************************************
* Filename:  		CInput.h
* Mod. Date: 		08/29/2012
* Mod. Date: 		11/30/2012
* Mod. Initials:	JW
* Author:    		Jayson S. Wu
* Purpose:   		Input class for the game
* 
************************************************/

#ifndef CINPUT_H_
#define CINPUT_H_

#include "../StdAfx.h"

#define DEADZONEBUFFER	10000
#define LSTICKCENTERX	32000
#define LSTICKCENTERY	32000

// enums used with the character map to assign keys
enum eCommands {eAttack = 0, eTurnAround, eExtraButton, eUpArrow, eLeft, eRight, eEsc, eStart, eUp, eDown,eDownArrow, eLeftArrow, eRightArrow, eNum1, eNum2, eNum3,eNumPad1,eNumPad2,eNumPad3,eNumPad4,eNumPad5,eNumPad6,eNumPad7,eNumPad8,eNumPad9, eNUMCOMMANDS};
enum eMouseClick {eLeftMouseClick = eNUMCOMMANDS, eRightMouseClick, eMiddleMouseClick, eMKJCOMMANDS};

/*****************************************************************
* szButtonNames[]: This is a chart that matches up with Direct Input keys
*				It is used to get button names for dynamic text input.
*
* Mod. Date: 			08/29/2012
* Mod. Initials:	      JW
*****************************************************************/
const string szButtonNames[] = 
{
	/*0*/		" ",				"ESCAPE",		"1",			"2",				"3",
	/*5*/		"4",				"5",			"6",			"7",				"8",
	/*10*/		"9",				"0",			"-",			"=",				"BACKSPACE",
	/*15*/		"Tab",				"Q",			"W",			"E",				"R",
	/*20*/		"T",				"Y"	,			"U",			"I",				"O",
	/*25*/		"P",				"[",			"]",			"ENTER",			"LCTRL",
	/*30*/		"A",				"S",			"D",			"F",				"G",
	/*35*/		"H",				"J",			"K",			"L",				";",
	/*40*/		"'",				"GRAVE",		"LSHIFT",		"\\",				"Z",
	/*45*/		"X",				"C",			"V",			"B",				"N", 
	/*50*/		"M",				",",			".",			"/",				"RSHIFT", 
	/*55*/		"*",				"LALT",			" SPACE",		"CAPITAL",			"F1",
	/*60*/		"F2",				"F3",			"F4",			"F5",				"F6", 
	/*65*/		"F7",				"F8",			"F9",			"F10",				"NUMLOCK", 
	/*70*/		"SCROLL",			"7NUMPAD",		"8NUMPAD",		"9NUMPAD",			"-", 
	/*75*/		"4NUMPAD",			"5NUMPAD",		"6NUMPAD",		"+",				"1NUMPAD", 
	/*80*/		"2NUMPAD",			"3NUMPAD",		"0NUMPAD",		".",				"", 
	/*85*/		"",					"",				"F11",			"F12",				"", 
	/*90*/		"",					"",				"",				"",					"",
	/*95*/		"",					"",				"",				"",					"",
	/*100*/		"F13",				"F14",			"F15",			"",					"",
	/*105*/		"",					"",				"",				"",					"",
	/*110*/		"",					"",				"KANA",			"",					"",
	/*115*/		"",					"",				"",				"",					"", 
	/*120*/		"",					"CONVERT",		"",				"NOCONVERT",		"",
	/*125*/		"YEN",				"",				"",				"",					"",
	/*130*/		"",					"",				"",				"",					"", 
	/*135*/		"",					"",				"",				"",					"",
	/*140*/		"",					"NUMPADEQUALS",	"",				"",					"CIRCUMFLEX",
	/*145*/		"@",				":",			"_",			"KANJI",			"STOP",
	/*150*/		"AX",				"UNLABLED",		"",				"",					"",
	/*155*/		"",					"NUMPADENTER",	"RCTRL",		"",					"",
	/*160*/		"",					"",				"",				"",					"",
	/*165*/		"",					"",				"",				"",					"",
	/*170*/		"",					"",				"",				"",					"",
	/*175*/		"",					"",				"",				"",					"NUMPADCOMMA",
	/*180*/		"",					"DIVIDE",		"",				"SYSRQ",			"RALT",
	/*185*/		"",					"",				"",				"",					"",
	/*190*/		"",					"",				"",				"",					"",
	/*195*/		"",					"",				"",				"",					"HOME",
	/*200*/		"UPARROW",			"PGUP",			"",				"LEFTARROW",		"",
	/*205*/		"RIGHTARROW",		"",				"END",			"DOWNARROW",		"PGDN",
	/*210*/		"INSERT",			"DELETE",		"",				"",					"",
	/*215*/		"",					"",				"",				"",					"LWINDOWS",
	/*220*/		"RWINDOWS",			"APP",			"",				"",					"",
	/*225*/		"",					""
};	

class CInput
{
	//Variables needed for Direct Input
	static IDirectInput8 *m_pDIn;
	static IDirectInputDevice8 *m_pDIMouse;
	static IDirectInputDevice8 *m_pDIKeyBoard;
	static IDirectInputDevice8 *m_pDIGamePad;

	// Variable stores key info used to see if key is down
	static short m_arrsKeyMap[eNUMCOMMANDS];

	// Used for key buffering if the key was down last frame
	static char m_arrchPrevKeyStat[256];
	static char m_arrchPrevPadStat[eNUMCOMMANDS];
	static char m_arrchPrevMouseStat[4];

	// Timer variable for each function/key. Used for TimeBufferedButtonHeld
	static float m_fElapsedTime;
	static float m_arrfTimeBuffer[eMKJCOMMANDS];

	// Mouse state. Checks the condition of the Mouse and movement
	static DIJOYSTATE2	m_DIjs;
	static DIMOUSESTATE m_DIms;
	static HWND	m_hHandle;
	static bool m_bCursorLocked;
	static POINT m_tCenterPoint;
	static POINT m_tMouseDiff;
	static POINT m_nPrevCursorPos;
	static bool m_bInDeadZone;
	static bool m_bHasDodged;

	static bool m_bInvertY;
	static bool m_bInvertX;
	//static bool m_bLockCursorToScreen;
	static float m_fWidth;
	static float m_fHeight;
public:
	// keyboard buffer for the keys pressed down
	static char m_arrchKeys[256];

	/*****************************************************************
	* CInput():  Default constructor
	*
	* Ins:						Void
	*      		      
	* Outs:						None
	*
	* Returns:					void
	*
	* Mod. Date:				11/30/2012
	* Mod. Initials:			JW
	*****************************************************************/
	CInput();

	/*****************************************************************
	* ~CInput():  Default Destructor
	* 			      
	* Ins:						Void      
	*
	* Outs:						None
	*
	* Returns:					void
	*
	* Mod. Date:				11/30/2012
	* Mod. Initials:			JW
	*****************************************************************/
	~CInput();

	/***************
	* Mutators
	***************/
	/*****************************************************************
	* InitDInput():  Initializes Direct Input. Parameters passed in 
	*				used for creating Direct Input devices
	*
	* Ins:			HWND hwnd - Window Handle from Program
	*
	*				HINSTANCE hinst - Instance of the game
	*
	* Outs:		      N/A
	*
	* Returns:		      void
	*
	* Mod. Date: 			08/29/2012
	* Mod. Initials:	      JW
	*****************************************************************/
	static void InitDInput(HWND hwnd, HINSTANCE hinst,float _Width,float _Height);

	/*****************************************************************
	* void Update(float fDt): Updates the Keyboard State buffer for key 
	*				presses and mouse state. MUST BE CALLED BEFORE KEY PRESS CHECKS
	*
	* Ins:			N/A
	*
	* Outs:		      N/A
	*
	* Returns:		      void
	*
	* Mod. Date: 			11/30/2012
	* Mod. Initials:	      JW
	*****************************************************************/
	static void Update(float fDt);

	/***************
	*  Accessors
	***************/	
	/*****************************************************************
	* DIKButtonCheck(int DIK_Key): Custom Key check with DIK keys
	* 				
	*
	* Ins:		int DIK_Key - the custom DIK key you want to check for.
	*
	* Outs:		     N/A
	*
	* Returns:		      bool - If key was down or not
	*
	* Mod. Date: 			08/29/2012
	* Mod. Initials:	      JW
	*****************************************************************/
	static bool DIKButtonCheck(int nDIK_Key){ return m_arrchKeys[nDIK_Key] & 0x80? true : false;}

	/*****************************************************************
	* DIKBufferedButtonCheck(int DIK_Key): Custom Key check with DIK keys
	* 
	* Ins:		int DIK_Key - the custom DIK key you want to check for.
	*
	*			bool bPushDown - This boolean tells the function to
	*					return true only when the key is pressed down (bPushDown == false)
	*						or only when the key is let go (bPushDown == true).
	*
	* Outs:		     N/A
	*
	* Returns:		      bool - If key was down or not
	*
	* Mod. Date: 			08/29/2012
	* Mod. Initials:	      JW
	*****************************************************************/
	static bool DIKBufferedButtonCheck(int nDIK_Key, bool bPushDown = false);

	/*****************************************************************
	* ButtonHeld(Int nButton): Takes in a button/Function and checks
	* 			 if that button is pressed down.
	*
	* Ins:			 int nButton - Represents an enum that is used in an 
	*					array to find what button is being checked for 
	*						being held down
	*
	* Outs:		     N/A
	*
	* Returns:		      bool - True if button is pressed down, false if not
	*
	* Mod. Date: 			08/29/2012
	* Mod. Initials:	      JW
	*****************************************************************/
	static	bool ButtonHeld(int nButton);

	/*****************************************************************
	* BufferedButtonPress(Int nButton, bool bPushDown): Takes in a 
	*			button/Function and checks if that button is pressed  
	*				down or let go of, depending on the bool passed in
	*
	* Ins:			int nButton - Represents an enum that is used in an 
	*					array to find what button is being checked for 
	*						being held down
	*
	*				bool bPushDown - This boolean tells the function to
	*					return true only when the key is pressed down (bPushDown == false)
	*						or only when the key is let go (bPushDown == true).
	*
	* Outs:		     N/A
	*
	* Returns:		      bool - True if button is pressed down/let go of depending on bool, false if otherwise
	*
	* Mod. Date: 			08/29/2012
	* Mod. Initials:	      JW
	*****************************************************************/
	static bool BufferedButtonPress(int nButton, bool bPushDown = false);

	/*****************************************************************
	* TimeBufferedButtonHeld(Int nButton, float fDt, float fTimeBuff): 
	*			Takes in a button/Function and checks if that button is
	*			pressed down. If the button is being held down for more
	*			than one call, it will only return true after a certain
	*			time has been reached.
	*
	* Ins:			int nButton - Represents an enum that is used in an 
	*					array to find what button is being checked for 
	*						being held down
	*				
	*				float fDt - Time being passed in for correct timing
	*				
	*				float fTimeBuff - If key is held down for multiple calls, 
	*					this is the time to reach before returning true again.
	*
	* Outs:		     N/A
	*
	* Returns:		      bool - True if button is pressed down, false if not
	*
	* Mod. Date: 			08/29/2012
	* Mod. Initials:	      JW
	* Note: Needs real timer to work correctly, currently using frames
	*****************************************************************/
	static bool TimeBufferedButtonHeld(int nButton, float fTimeBuff);

	/*****************************************************************
	* GetButtonName(Int nButton): Takes in a button/Function and returns
	* 			 the name of the button is pressed down from a look up chart
	*
	* Ins:			 int nButton - Represents an enum that is used in an 
	*					array to find what button is being checked for 
	*
	* Outs:		     N/A
	*
	* Returns:		const char*	- A string that is the name of the button
	*					 being asked for, via in Parameter. ("Tab", "J", "Up", "Esc")
	*
	* Mod. Date:		      11/30/2012
	* Mod. Initials:	      JW
	*****************************************************************/
	static const char* GetButtonName(int nButton) {return szButtonNames[m_arrsKeyMap[nButton]].c_str();}

	/*****************************************************************
	* WhatButtonPressedChar(int Key): Loops and checks what button is being pressed on the keyboard and returns
	*					the first letter of that name.
	*
	* Ins:			 N/A
	*
	* Outs:		     N/A
	*
	* Returns:		const char	- A char that is the first ltter in the name of the button
	*					 being pressed ("T" for "Tab", "J" for "J", "Up", "E" = "Esc")
	*
	* Mod. Date:		      11/30/2012
	* Mod. Initials:	      JW
	*****************************************************************/
	static const char WhatButtonPressedChar();

	/*****************************************************************
	* GetMouseChangeX(): Gets the amount of change the mouse has gone through on the X
	*
	* Ins:			 N/A
	*
	* Outs:		     N/A
	*
	* Returns:		
	*
	* Mod. Date:		      12/12/2012
	* Mod. Initials:	      JW
	*****************************************************************/
	static float GetMouseChangeX();

	/*****************************************************************
	* GetMouseChangeY(): Gets the amount of change the mouse has gone through on the Y
	*
	* Ins:			 N/A
	*
	* Outs:		     N/A
	*
	* Returns:		
	*
	* Mod. Date:		      12/12/2012
	* Mod. Initials:	      JW
	*****************************************************************/
	static float GetMouseChangeY();

	/*****************************************************************
	* SetCursorLock(bool SetTo): Sets the m_bCursorLocked variable to 
	*					whatever is passed in and toggles ShowCursor()
	*
	* Ins:			 bool SetTo - Variable that determines if the cursor is being 
	*							locked and hidden or not.
	*
	* Outs:		     N/A
	*
	* Returns:		
	*
	* Mod. Date:		      12/12/2012
	* Mod. Initials:	      JW
	*****************************************************************/
	static void	SetCursorLock(bool bSetTo);

	static POINT GetMouseScreenPos();
	static void SetMouseScreenPos(int x, int y);
	static bool GetInDeadZone();
	static bool GetHasDodge();
	static void SetHasDodged(bool _hasDodged);
	static bool GetInvertY();
	static bool GetInvertX();
	static void SetInvertY(bool _bIsInverted);
	static void SetInvertX(bool _bIsInverted);
	static void SetWidthHeight(float _Width, float _Height);
};
#endif