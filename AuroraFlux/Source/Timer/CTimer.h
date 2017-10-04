#ifndef	CTIMER_H
#define CTIMER_H
/***********************************************
 * Filename:  		CTimer.h
 * Date:      		11/29/2012
 * Mod. Date: 		11/29/2012
 * Mod. Initials:	BS
 * Author:    		Bryan W. Schotanes
 * Purpose:   		This class will keep track of time for the project
 *
 ************************************************/
class CTimer
{
private:
    int             m_nFPS;
    float           m_fTimeStart;
    float           m_fTimeDelta;
    LARGE_INTEGER		m_tTickFrequency;
	
public:
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
	
	CTimer( void );
	
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
	
	~CTimer( void );
   
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
   
	void Reset();
	
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
    void Update();

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
	* Returns:					float
	*
	* Mod. Date:				11/29/2012
	* Mod. Initials:			BS
	*****************************************************************/
    
	float GetTime();

	/***************
	*  Accessors
	***************/

   /*****************************************************************
	* GetDeltaTime():  Returns time since last frame
	* 			      
	*			      
	*			      
	*
	* Ins:						void
	*      		      
	*
	* Outs:						None
	*
	* Returns:					float
	*
	* Mod. Date:				11/29/2012
	* Mod. Initials:			BS
	*****************************************************************/
    
	float GetDeltaTime() {return m_fTimeDelta;}

    /*****************************************************************
	* GetFPS():  Returns time since last Reset
	* 			      
	*			      
	*			      
	*
	* Ins:						void
	*      		      
	*
	* Outs:						None
	*
	* Returns:					int
	*
	* Mod. Date:				11/29/2012
	* Mod. Initials:			BS
	*****************************************************************/
    
	int GetFPS() {return m_nFPS;}
};

#endif