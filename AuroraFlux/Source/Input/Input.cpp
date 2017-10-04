/***********************************************
* Filename:  		CInput.cpp
* Date:      		08/29/2012
* Mod. Date: 		12/14/2012
* Mod. Initials:	JW
* Author:    		Jayson S. Wu
* Purpose:   		Defined functoins for the game's Input class
************************************************/
#include "Input.h"
#include "../Definitions.h"

#define RELEASE(ref)	 if(ref != NULL) ref->Release(); ref = NULL;

IDirectInput8* CInput::m_pDIn = nullptr;
IDirectInputDevice8* CInput::m_pDIMouse = nullptr;
IDirectInputDevice8* CInput::m_pDIKeyBoard = nullptr;
IDirectInputDevice8* CInput::m_pDIGamePad = nullptr;
short CInput::m_arrsKeyMap[eNUMCOMMANDS] = {0};
char CInput::m_arrchPrevKeyStat[256] = {0};
char CInput::m_arrchPrevPadStat[eNUMCOMMANDS] = {0};
float CInput::m_fElapsedTime = 0.0f;
float CInput::m_arrfTimeBuffer[eMKJCOMMANDS] = {0};
char CInput::m_arrchKeys[256] = {0};
char CInput::m_arrchPrevMouseStat[4] = {0};
DIMOUSESTATE CInput::m_DIms = DIMOUSESTATE();
DIJOYSTATE2 CInput::m_DIjs = DIJOYSTATE2();

HWND	CInput::m_hHandle = NULL;
bool	CInput::m_bCursorLocked = false;
POINT	CInput::m_tCenterPoint = {((LONG)m_fWidth/2), ((LONG)m_fHeight/2)};
POINT	CInput::m_tMouseDiff = {0,0};
POINT	CInput::m_nPrevCursorPos = {0,0};
bool	CInput::m_bInDeadZone = false;
bool	CInput::m_bHasDodged = false;
bool	CInput::m_bInvertY = false;
bool	CInput::m_bInvertX = false;
float   CInput::m_fHeight = 0.0f;
float	CInput::m_fWidth = 0.0f;
//bool	CInput::m_bLockCursorToScreen = true;
/*****************************************************************
* CInput():  Default constructor
*
* Ins:						Void
*      		      
* Outs:						None
*
* Returns:					void
*
* Mod. Date:				12/14/2012
* Mod. Initials:			JW
*****************************************************************/
CInput::CInput()
{
	m_pDIn = nullptr;
	m_pDIMouse = nullptr;
	m_pDIKeyBoard = nullptr;
	m_pDIGamePad = nullptr;
	m_hHandle = nullptr;
	m_bCursorLocked = false;

	m_arrsKeyMap[eUp] = DIK_W;
	m_arrsKeyMap[eDown] = DIK_S; 
	m_arrsKeyMap[eLeft] = DIK_A;
	m_arrsKeyMap[eRight] = DIK_D; 
	m_arrsKeyMap[eAttack] = DIK_J; 
	m_arrsKeyMap[eTurnAround] = DIK_Q; 
	m_arrsKeyMap[eExtraButton] = DIK_L;
	m_arrsKeyMap[eEsc] = DIK_ESCAPE;
	m_arrsKeyMap[eStart] = DIK_RETURN;
	m_arrsKeyMap[eUpArrow] = DIK_UP;
	m_arrsKeyMap[eDownArrow] = DIK_DOWN;
	m_arrsKeyMap[eLeftArrow] = DIK_LEFT;
	m_arrsKeyMap[eRightArrow] = DIK_RIGHT;
	m_arrsKeyMap[eNum1] = DIK_1;
	m_arrsKeyMap[eNum2] = DIK_2;
	m_arrsKeyMap[eNum3] = DIK_3;
	m_arrsKeyMap[eNumPad1] = DIK_NUMPAD1;
	m_arrsKeyMap[eNumPad2] = DIK_NUMPAD2;
	m_arrsKeyMap[eNumPad3] = DIK_NUMPAD3;
	m_arrsKeyMap[eNumPad4] = DIK_NUMPAD4;
	m_arrsKeyMap[eNumPad5] = DIK_NUMPAD5;
	m_arrsKeyMap[eNumPad6] = DIK_NUMPAD6;
	m_arrsKeyMap[eNumPad7] = DIK_NUMPAD7;
	m_arrsKeyMap[eNumPad8] = DIK_NUMPAD8;
	m_arrsKeyMap[eNumPad9] = DIK_NUMPAD9;

	m_fElapsedTime = 0.0f;
	for(int nEachKey = 0; nEachKey < eNUMCOMMANDS; nEachKey++)
		m_arrfTimeBuffer[nEachKey] = 0.0f;

	memset(m_arrchPrevKeyStat, 0, 256);
	//m_bLockCursorToScreen = true;
}

/*****************************************************************
* ~CInput():  Default Destructor
* 			      
* Ins:						Void      
*
* Outs:						None
*
* Returns:					void
*
* Mod. Date:				12/14/2012
* Mod. Initials:			JW
*****************************************************************/
CInput::~CInput()
{
	RELEASE(m_pDIn);
	RELEASE(m_pDIMouse);
	RELEASE(m_pDIKeyBoard);
	RELEASE(m_pDIGamePad);
}

/***************************************************************************
* Mutators
***************************************************************************/
/*****************************************************************
* void Update(): Updates the Keyboard State buffer for key 
*				presses and mouse state. MUST BE CALLED BEFORE KEY PRESS CHECKS
*
* Ins:			N/A
*
* Outs:		      N/A
*
* Returns:		      void
*
* Mod. Date: 			12/14/2012
* Mod. Initials:	      JW
*****************************************************************/
void CInput::Update(float fDt)
{  
	HRESULT hr;
	if( NULL != m_pDIGamePad )
	{
		hr = m_pDIGamePad->Poll();
		if( FAILED( hr ) )
		{
			// DInput is telling us that the input stream has been
			// interrupted. We aren't tracking any state between polls, so
			// we don't have any special reset that needs to be done. We
			// just re-acquire and try again.
			hr = m_pDIGamePad->Acquire();
			while( hr == DIERR_INPUTLOST )
				hr = m_pDIGamePad->Acquire();
		}
		if(m_pDIGamePad->Poll() >= 0)
		{
			DWORD dwPadValue = m_DIjs.rgdwPOV[0];
			for(int nButton = 0; nButton < eNUMCOMMANDS; nButton++)
			{
				switch(nButton)
				{
				case eUp:
					{
						m_arrchPrevPadStat[nButton] = (m_DIjs.lY < (LSTICKCENTERY - DEADZONEBUFFER) || (dwPadValue < 36000 && (dwPadValue <= 4500 || dwPadValue >= 31500)))? true : false;
						break;
					}
				case eDown:
					{			
						m_arrchPrevPadStat[nButton] = (m_DIjs.lY > (LSTICKCENTERY + DEADZONEBUFFER) || (dwPadValue < 36000 && (dwPadValue <= 22500 && dwPadValue >= 13500)))? true : false;
						break;
					}
				case eLeft:
					{
						m_arrchPrevPadStat[nButton] = (m_DIjs.lX < (LSTICKCENTERX - DEADZONEBUFFER) || (dwPadValue < 36000 && (dwPadValue >= 22500 && dwPadValue <= 31500)))? true : false;
						break;
					}
				case eRight:
					{
						m_arrchPrevPadStat[nButton] = (m_DIjs.lX > (LSTICKCENTERX + DEADZONEBUFFER) || (dwPadValue < 36000 && (dwPadValue >= 4500 && dwPadValue <= 13500)))? true : false;
						break;
					}
				default:
					{
						m_arrchPrevPadStat[nButton] = m_DIjs.rgbButtons[nButton] & 0x80;
					}
				}
			}
			m_pDIGamePad->Acquire();
			m_pDIGamePad->GetDeviceState(sizeof(DIJOYSTATE), &m_DIjs);
		}
	}

	// Update the Input class' fElapsed time, used for the time buffered input
	m_fElapsedTime = fDt;

	for(int nMouseButtons = 0; nMouseButtons < 4; nMouseButtons++)
		m_arrchPrevMouseStat[nMouseButtons] = m_DIms.rgbButtons[nMouseButtons];

	if(m_bCursorLocked)
	{
		POINT tTempMousePos;
		GetCursorPos(&tTempMousePos);

		//if(m_bHasDodged == false)
		//{
		static float fSkipTime = 0.0f;


		m_tMouseDiff.x = (LONG)((tTempMousePos.x - m_tCenterPoint.x));
		m_tMouseDiff.y = (LONG)((tTempMousePos.y - m_tCenterPoint.y));

		if(m_tMouseDiff.x == m_nPrevCursorPos.x && m_tMouseDiff.y == m_nPrevCursorPos.y)
		{
			if(m_tMouseDiff.x < 75.0f && m_tMouseDiff.x > -75.0f && m_tMouseDiff.y < 75.0f && m_tMouseDiff.y > -75.0f)
			{
				if(fSkipTime > 1.0f)
				{				

					m_tMouseDiff.x = 0;
					m_tMouseDiff.y = 0;

					m_tCenterPoint.x = ((LONG)m_fWidth/2);
					m_tCenterPoint.y = ((LONG)m_fHeight/2);
					ClientToScreen(m_hHandle, &m_tCenterPoint);
					m_bInDeadZone = true;
					////////////////////////////////////////////////////////////////////////////////////////////////	
				}

				fSkipTime += m_fElapsedTime;

			}


		}
		else
		{			
			////////////////////////////////////////////////////////////////////////////////////////////////
			//Calculating mouse offset

			if(m_bInvertX == true)
			{
				m_tMouseDiff.x = (LONG)(-(tTempMousePos.x - m_tCenterPoint.x));
			}
			else
			{
				m_tMouseDiff.x = (LONG)((tTempMousePos.x - m_tCenterPoint.x));
			}

			if(m_bInvertY == true) 
			{
				m_tMouseDiff.y = (LONG)(-(tTempMousePos.y - m_tCenterPoint.y));
			}
			else
			{
				m_tMouseDiff.y = (LONG)((tTempMousePos.y - m_tCenterPoint.y));
			}

			m_tCenterPoint.x = ((LONG)m_fWidth/2);
			m_tCenterPoint.y = ((LONG)m_fHeight/2);
			ClientToScreen(m_hHandle, &m_tCenterPoint);
			m_bInDeadZone = false;
			////////////////////////////////////////////////////////////////////////////////////////////////	
			fSkipTime = 0.0f;
			m_nPrevCursorPos = m_tMouseDiff;

		}
		//}
		//else
		//{
		//	POINT tTempMousePos;
		//	GetCursorPos(&tTempMousePos);
		//	
		//	m_tMouseDiff.y = 0;		
		//	m_tMouseDiff.x = 0;
		//
		//	m_tCenterPoint.x = ((LONG)g_fWINDOW_WIDTH/2);
		//	m_tCenterPoint.y = ((LONG)g_fWINDOW_HEIGHT/2);
		//	ClientToScreen(m_hHandle, &m_tCenterPoint);
		//}

		//if(GetAsyncKeyState(VK_OEM_3) & 0x1)
		//{
		//	m_bLockCursorToScreen = !m_bLockCursorToScreen;
		//}
		//
		//if(m_bLockCursorToScreen == true)
		//{
		//	RECT rWindowRect;
		//	GetWindowRect(m_hHandle, &rWindowRect);
		//	rWindowRect.bottom -= 10;
		//	rWindowRect.left += 10;
		//	rWindowRect.top += 30;
		//	rWindowRect.right -= 10;
		//	ClipCursor(&rWindowRect);
		//}
		//else
		//{
		//	ClipCursor(0);
		//}
	}

	m_pDIMouse->Acquire();
	m_pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_DIms);

	for(int nButton = 0; nButton < eNUMCOMMANDS; nButton++)
		m_arrchPrevKeyStat[m_arrsKeyMap[nButton]] = (m_arrchKeys[m_arrsKeyMap[nButton]] & 0x80)? true : false;
	m_pDIKeyBoard->Acquire();
	memcpy(m_arrchPrevKeyStat, m_arrchKeys, sizeof(m_arrchKeys));
	m_pDIKeyBoard->GetDeviceState(sizeof(m_arrchKeys), (void*)m_arrchKeys);
}

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
* Mod. Date: 			12/14/2012
* Mod. Initials:	      JW
*****************************************************************/
void CInput::InitDInput(HWND hwnd, HINSTANCE hinst,float _Width,float _Height)
{
	m_pDIMouse = NULL;
	m_pDIKeyBoard = NULL;
	m_pDIGamePad = NULL;

	CInput();

	m_hHandle = hwnd;
	DirectInput8Create(hinst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDIn, 0);

	m_pDIn->CreateDevice(GUID_SysMouse, &m_pDIMouse, NULL);
	m_pDIMouse->SetDataFormat(&c_dfDIMouse);
	m_pDIMouse->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE);
	m_pDIMouse->Acquire();

	m_pDIn->CreateDevice(GUID_SysKeyboard, &m_pDIKeyBoard, NULL);
	m_pDIKeyBoard->SetDataFormat(&c_dfDIKeyboard);
	m_pDIKeyBoard->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE);
	m_pDIKeyBoard->Acquire();

	m_pDIn->CreateDevice(GUID_Joystick, &m_pDIGamePad, NULL);
	if(m_pDIGamePad)
	{
		m_pDIGamePad->SetDataFormat(&c_dfDIJoystick);
		m_pDIGamePad->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		m_pDIGamePad->Acquire();
	}
	m_fWidth = _Width;
	m_fHeight = _Height;
}

/***************************************************************************
* Accessors
***************************************************************************/

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
* Mod. Date: 			12/14/2012
* Mod. Initials:	      JW
*****************************************************************/
bool CInput::DIKBufferedButtonCheck(int nDIK_Key, bool bPushDown)
{
	bool bChange = false;
	bool bNewKey;
	bool bOldKey;

	// Because compiler wants to complain about changing shorts to bools
	if(m_arrchKeys[nDIK_Key] & 0x80)
	{
		bNewKey = true; // Pressed Down
	}
	else
	{
		bNewKey = false; // Let Go
	}


	if(m_arrchPrevKeyStat[nDIK_Key] & 0x80)
	{
		bOldKey = true;
	}
	else
	{
		bOldKey = false;
	}


	if( bOldKey != bNewKey && bNewKey != bPushDown)
	{
		if(bPushDown == false)
		{
			m_arrchPrevKeyStat[nDIK_Key] = bNewKey;
		}
		bChange = true;
	}


	if(bPushDown == true)
	{
		m_arrchPrevKeyStat[nDIK_Key] = bNewKey;
	}
	return bChange;
}

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
* Mod. Date: 			12/14/2012
* Mod. Initials:	      JW
*****************************************************************/
bool CInput::ButtonHeld(int nButton)
{
	if(m_pDIGamePad)
	{
		DWORD dwPadValue = m_DIjs.rgdwPOV[0];
		//dwPadValue has to be under 36000 to be in range, otherwise
		//it is uninitialized or not being pressed. Joypad values suck
		switch(nButton)
		{
		case eUp:
			{
				if(m_DIjs.lY < (LSTICKCENTERY - DEADZONEBUFFER) || (dwPadValue < 36000 && (dwPadValue <= 4500 || dwPadValue >= 31500)))
				{
					return true;
				}
				break;
			}
		case eDown:
			{			
				if(m_DIjs.lY > (LSTICKCENTERY + DEADZONEBUFFER) || (dwPadValue < 36000 && (dwPadValue <= 22500 && dwPadValue >= 13500)))
				{
					return true;
				}
				break;
			}
		case eLeft:
			{
				if(m_DIjs.lX < (LSTICKCENTERX - DEADZONEBUFFER) || (dwPadValue < 36000 && (dwPadValue >= 22500 && dwPadValue <= 31500)))
				{
					return true;
				}
				break;
			}
		case eRight:
			{
				if(m_DIjs.lX > (LSTICKCENTERX + DEADZONEBUFFER) || (dwPadValue < 36000 && (dwPadValue >= 4500 && dwPadValue <= 13500)))
				{
					return true;
				}
				break;
			}
		default:
			{
				if(m_DIjs.rgbButtons[nButton] & 0x80)
				{
					return true;
				}
			}
		}
	}

	if(nButton >= eNUMCOMMANDS)
	{
		return (m_DIms.rgbButtons[nButton - eNUMCOMMANDS])? true : false;	
	}
	else
	{
		return (m_arrchKeys[m_arrsKeyMap[nButton]] & 0x80)? true : false;
	}
}


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
* Mod. Date: 			12/14/2012
* Mod. Initials:	      JW
*****************************************************************/
bool CInput::BufferedButtonPress(int nButton, bool bPushDown)
{
	bool bChange = false;
	bool bNewKey;
	bool bOldKey;

	if(m_pDIGamePad)
	{
		DWORD dwPadValue = m_DIjs.rgdwPOV[0];
		switch(nButton)
		{
		case eUp:
			{
				if((m_DIjs.lY < (LSTICKCENTERY - DEADZONEBUFFER)) || (dwPadValue < 36000 && (dwPadValue < 4500 || dwPadValue > 31500)))
				{
					bNewKey = true; // Pressed Down
				}
				else
				{
					bNewKey = false;// Let go
				}
				break;
			}
		case eDown:
			{			
				if(m_DIjs.lY > (LSTICKCENTERY + DEADZONEBUFFER) || ((dwPadValue < 36000 && (dwPadValue < 22500 && dwPadValue > 13500))))
				{
					bNewKey = true; // Pressed Down
				}
				else
				{
					bNewKey = false;// Let go
				}
				break;
			}
		case eLeft:
			{
				if(m_DIjs.lX < (LSTICKCENTERX - DEADZONEBUFFER) || (dwPadValue < 36000 && (dwPadValue > 22500 && dwPadValue < 31500)))
				{
					bNewKey = true; // Pressed Down
				}
				else
				{
					bNewKey = false;// Let go
				}
				break;
			}
		case eRight:
			{
				if(m_DIjs.lX > (LSTICKCENTERX + DEADZONEBUFFER) || (dwPadValue < 36000 && (dwPadValue > 4500 && dwPadValue < 13500)))
				{
					bNewKey = true; // Pressed Down
				}
				else
				{
					bNewKey = false;// Let go
				}
				break;
			}
		default:
			{
				// Because compiler wants to complain about changing shorts to bools
				if(m_DIjs.rgbButtons[nButton] & 0x80)
				{
					bNewKey = true; // Pressed Down
				}
				else
				{
					bNewKey = false; // Let Go
				}
			}
		}

		//TODO::Jayson If buffered input breaks, check here first
		if(m_arrchPrevPadStat[nButton])
		{
			bOldKey = true;
		}
		else
		{
			bOldKey = false;
		}												
		if( bOldKey != bNewKey && bNewKey != bPushDown)	
		{
			bChange = true;								
		}
		if(bChange)										
		{
			return bChange;
		}
	}

	if(nButton >= eLeftMouseClick)
	{
		if(m_DIms.rgbButtons[nButton - eNUMCOMMANDS] & 0x80)
		{
			bNewKey = true;
		}
		else
		{
			bNewKey = false;
		}

		if(m_arrchPrevMouseStat[nButton - eNUMCOMMANDS])
		{
			bOldKey = true;
		}
		else
		{
			bOldKey = false;
		}

		if( bOldKey != bNewKey && bNewKey != bPushDown)	
		{
			bChange = true;								
		}					
		return bChange;
	}
	else
	{
		// Because compiler wants to complain about changing shorts to bools
		if(m_arrchKeys[m_arrsKeyMap[nButton]] & 0x80)
		{
			bNewKey = true; // Pressed Down
		}
		else
		{
			bNewKey = false; // Let Go
		}
		if(m_arrchPrevKeyStat[m_arrsKeyMap[nButton]] & 0x80)
		{
			bOldKey = true;
		}
		else
		{
			bOldKey = false;
		}

		if( bOldKey != bNewKey && bNewKey != bPushDown)
		{
			bChange = true;
		}
		return bChange;
	}
}

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
* Mod. Date: 			12/14/2012
* Mod. Initials:	      JW
* Note: Needs real timer to work correctly, currently using frames
*****************************************************************/
bool CInput::TimeBufferedButtonHeld(int nButton, float fTimeBuff)
{
	m_arrfTimeBuffer[nButton] += m_fElapsedTime;

	if(m_pDIGamePad)
	{
		if(m_DIjs.rgbButtons[nButton] & 0x80 || m_arrchKeys[m_arrsKeyMap[nButton]] & 0x80)
		{
			if(m_arrfTimeBuffer[nButton] >= fTimeBuff)
			{
				m_arrfTimeBuffer[nButton] = 0.0f;
				return true;	
			}
		}
	}
	else if(nButton >= eNUMCOMMANDS)
	{
		if(m_DIms.rgbButtons[nButton - eNUMCOMMANDS])
		{
			if(m_arrfTimeBuffer[nButton] >= fTimeBuff)
			{
				// TODO::Jayson - special condition because we want right mouse click and left mouse click to share time buffer
				if(nButton == eLeftMouseClick || nButton == eRightMouseClick)
				{
					m_arrfTimeBuffer[eLeftMouseClick] = 0.0f;
					m_arrfTimeBuffer[eRightMouseClick] = 0.0f;
				}
				m_arrfTimeBuffer[nButton] = 0.0f;
				return true;
			}
		}
	}
	else
	{
		if(m_arrchKeys[m_arrsKeyMap[nButton]] & 0x80)
		{
			if(m_arrfTimeBuffer[nButton] >= fTimeBuff)
			{
				m_arrfTimeBuffer[nButton] = 0.0f;
				return true;	 
			}
		}
	}
	return false;
}

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
* Mod. Date:		      12/14/2012
* Mod. Initials:	      JW
*****************************************************************/
const char CInput::WhatButtonPressedChar()
{
	for(int nButton = 0; nButton < 58; nButton++) 
	{
		if(DIKBufferedButtonCheck(nButton))
		{
			return szButtonNames[nButton].c_str()[0];
		}
	}
	return NULL;
}

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
float CInput::GetMouseChangeX()
{
	return (float)m_tMouseDiff.x;
}

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
float CInput::GetMouseChangeY()
{
	return (float)m_tMouseDiff.y;
}

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
void CInput::SetCursorLock(bool bSetTo) 
{
	ShowCursor(!bSetTo);
	m_bCursorLocked = bSetTo;
}


POINT CInput::GetMouseScreenPos()
{
	POINT tMousePosition;
	GetCursorPos(&tMousePosition);
	ScreenToClient( m_hHandle, &tMousePosition);
	return tMousePosition;
}

void CInput::SetMouseScreenPos(int x, int y)
{
	POINT mousePos;
	mousePos.x = (long)x;
	mousePos.y = (long)y;
	ClientToScreen(m_hHandle, &mousePos);
	SetCursorPos(mousePos.x, mousePos.y);
}

bool CInput::GetInDeadZone()
{
	return m_bInDeadZone;
}

bool CInput::GetHasDodge()
{
	return m_bHasDodged;
}

void CInput::SetHasDodged(bool _hasDodged)
{
	m_bHasDodged = _hasDodged;
}

bool CInput::GetInvertX()
{
	return m_bInvertX;
}

bool CInput::GetInvertY()
{
	return m_bInvertY;
}

void CInput::SetInvertX(bool _bIsInverted)
{
	m_bInvertX = _bIsInverted;
}

void CInput::SetInvertY(bool _bIsInverted)
{
	m_bInvertY = _bIsInverted;
}
void CInput::SetWidthHeight(float _Width, float _Height)
{
	m_fWidth = _Width;
	m_fHeight = _Height;
}
