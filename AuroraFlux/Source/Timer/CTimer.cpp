#include "../StdAfx.h"
#include "CTimer.h"

/***********************************************
 * Filename:  		CTimer.h
 * Date:      		11/29/2012
 * Mod. Date: 		11/29/2012
 * Mod. Initials:	BS
 * Author:    		Bryan W. Schotanes
 * Purpose:   		This class will keep track of time for the project
 *
 ************************************************/

/*****************************************************************
* CTimer():  Default constructor
* 			      
*			      
*			      
*
* Ins:						Void
*      		      
*
* Outs:						None
*
* Returns:					void
*
* Mod. Date:				11/29/2012
* Mod. Initials:			BS
*****************************************************************/

CTimer::CTimer()
{
    // seed random timer for anything using rand
    srand( ( unsigned int )time( NULL ) );

    m_nFPS = 0;
    m_fTimeStart = 0.0f;
    m_fTimeDelta = 0.0f;
}

/*****************************************************************
	* ~CTimer():  Destructor
	* 			      
	*			      
	*			      
	*
	* Ins:						Void
	*      		      
	*
	* Outs:						None
	*
	* Returns:					void
	*
	* Mod. Date:				11/29/2012
	* Mod. Initials:			BS
	*****************************************************************/

CTimer::~CTimer()
{
    
}

/*****************************************************************
* Reset():  Resets the timer to default values
* 			      
*			      
*			      
*
* Ins:						void
*      		      
*
* Outs:						None
*
* Returns:					void
*
* Mod. Date:				11/29/2012
* Mod. Initials:			BS
*****************************************************************/

void CTimer::Reset()
{
    // locals
    LARGE_INTEGER   nTicks;

    // get processor frequency
    QueryPerformanceFrequency( &m_tTickFrequency );

    // get current tick count
    QueryPerformanceCounter( &nTicks );

    // init time at start
    m_fTimeStart = ( float )nTicks.QuadPart / ( float )m_tTickFrequency.QuadPart;

    // reset frame count
    m_nFPS = 0;
}

/*****************************************************************
* Update():  Updates timer, Only use once per frame
* 			      
*			      
*			      
*
* Ins:						void
*      		      
*
* Outs:						None
*
* Returns:					void
*
* Mod. Date:				11/29/2012
* Mod. Initials:			BS
*****************************************************************/

void CTimer::Update()
{
    // locals
    static float   fTimePrev = GetTime();
    static float   fTimeLastFrame = GetTime();
    static int     nFrames = 0;
    float          fTimeNow = GetTime();

    // increment frame count
    nFrames++;

    // log how many frames since ONE second
    if ( fTimeNow - fTimePrev > 1.0f )
    {
        m_nFPS = nFrames / (int)(fTimeNow - fTimePrev);
        nFrames = 0;
        fTimePrev = fTimeNow;
    }

    // calculate delta time
    m_fTimeDelta     = ( fTimeNow - fTimeLastFrame );
    fTimeLastFrame = fTimeNow;
}

/*****************************************************************
* GetTime():  Returns time since last Reset
* 			      
*			      
*			      
*
* Ins:						void
*      		      
*
* Outs:						None
*
* Returns:					void
*
* Mod. Date:				11/29/2012
* Mod. Initials:			BS
*****************************************************************/

float CTimer::GetTime()
{
    LARGE_INTEGER   tTicks;
    float           fTime;

    // get current tick count
    QueryPerformanceCounter( &tTicks );

    // get current time ( since computer turned on )
    fTime = ( float )tTicks.QuadPart / ( float )m_tTickFrequency.QuadPart;

    // calculate time since reset
    return ( fTime - m_fTimeStart );
}