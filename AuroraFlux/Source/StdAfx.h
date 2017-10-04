/***********************************************
 * Filename:  		StdAfx.h
 * Date:      		11/20/2012
 * Mod. Date: 		11/29/2012
 * Mod. Initials:	AQ
 * Author:    		Andrew T. Goosens
 * Purpose:   		This is the precompiled Header used
 *					to simplify all major includes
 *					for .h and .cpp files
 ************************************************/


#ifndef STDAFX_H
#define STDAFX_H

#include <Windows.h>
#include <process.h>

//#if _DEBUG
//#include <vld.h>
//#endif

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


#pragma comment(lib,"dxguid.lib")						//needed for 3d sounds
#pragma comment(lib,"Ws2_32.lib")						//wwise uses this for communication

#include <cstdio>
#include <cstdlib>
#include <conio.h>

#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <D3Dcommon.h>

#pragma comment(lib,"d3d9.lib")	
#pragma comment(lib,"d3dx9.lib")

//#pragma comment(linker, "/include:vld_x86.dll")

#include <time.h>
#include <queue>
#include "Sound\Wwise.h"

#endif