/***********************************************
* Filename:  		CPlayerBehaviour.h
* Date:      		12/08/2012
* Mod. Date: 		12/08/2012
* Mod. Initials:	AG
* Author:    		Andrew Goossens
* Purpose:   		This class will be the base class all entities will inherit from.
************************************************/
#include "../StdAfx.h"
#include "PlayerBehavior.h"

/*****************************************************************
* CPlayerBehavior():	Default Constructor will pass down to its respective chlid.
* Ins:					None    		      
* Outs:				None
* Returns:				None
* Mod. Date:			12/08/2012
* Mod. Initials:		AG
*****************************************************************/
CPlayerBehavior::CPlayerBehavior(void) : m_nType(-1), m_nHealth(0), m_nShields(0), m_nDamageModifier(0),
	m_fVelocityModifier(0.0f), m_bIsActive(false), m_bIsVisible(false)
{
}

/*****************************************************************
* CPlayerBehavior():	Overload Constructor Default Constructor will pass down to its 
*			respective chlid and set the entites target to the passed in one.
* Ins:					_player    		      
* Outs:				None
* Returns:				None
* Mod. Date:			12/08/2012
* Mod. Initials:		AG
*****************************************************************/
CPlayerBehavior::CPlayerBehavior(CPlayer* _pPlayer)
{
}

/*****************************************************************
* ~CBluePlayer():	Default Destructor
* Ins:			    None    		      
* Outs:				None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CPlayerBehavior::~CPlayerBehavior(void)
{
}

/*****************************************************************
* Update():		Pass update to its children still.
* Ins:			    _elapsedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		12/08/2012
* Mod. Initials:	AG
*****************************************************************/
void CPlayerBehavior::Update(float _fElapsedTime)
{
}

/*****************************************************************
* Render():		Pass render to its children still.
* Ins:			    None    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		12/08/2012
* Mod. Initials:	AG
*****************************************************************/
void CPlayerBehavior::Render()
{
}