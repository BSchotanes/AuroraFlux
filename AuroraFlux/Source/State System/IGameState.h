/***********************************************
* Filename:  		IGameState.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AG
* Author:    		Andrew Goossens
* Purpose:   		This is the base class for all of
*					the game states.
*					
************************************************/
#ifndef IGAMESTATE_H
#define	IGAMESTATE_H

class IGameState
{
public:
	/*****************************************************************
	* ~IGameState(): Destructor, probably doing nothing
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	virtual ~IGameState(void) = 0
	{/* DO NOTHING DTOR*/}

	/*****************************************************************
	* Enter(): 
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	virtual void Enter() = 0;		// Called to load resources

	/*****************************************************************
	* Exit(): Exit the current state and sets all values to a random value
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	virtual void Exit() = 0;		// Called to unload resources

	/*****************************************************************
	* Input(): Registers the input made by the player
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    bool
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	virtual bool Input() = 0;		// Handle Input

	/*****************************************************************
	* Update(): Update the game states based on the change in time
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	virtual void Update(float _fElapsedTime) = 0;		// Update game entities

	/*****************************************************************
	* Render(): Renders the game state by calling the object managers render
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AG
	*****************************************************************/
	virtual void Render() = 0;		// Render game entities
};
#endif