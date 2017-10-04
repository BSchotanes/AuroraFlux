/***********************************************
 * Filename:  		Sphere.h
 * Date:      		11/21/2012
 * Mod. Date: 		11/29/2012
 * Mod. Initials:	AQ
 * Author:    		Alexander G. Garcia de Quevedo
 * Purpose:   		This is a child class of CCollOBJ. 
 *					It acts as a bounding volume in the 
 *					shape of a sphere.
 ************************************************/

#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "../Definitions.h"
#include "collobj.h"


class CSphere :
	public CCollOBJ
{
	D3DXVECTOR3 m_d3dPosition;
	float m_fRadius;
	float m_fFrustumRadius;
public:


/*****************************************************************
 * CSphere():	      Default Constructor
 *
 * Ins:			      
 *
 * Outs:		     
 *
 * Returns:		      
 *
 * Mod. Date:		      11/21/2012
 * Mod. Initials:	      AQ
 *****************************************************************/
	CSphere(void);
/*****************************************************************
 * ~CSphere():	      Default destructor
 *
 * Ins:			      
 *
 * Outs:		     
 *
 * Returns:		      
 *
 * Mod. Date:		      11/21/2012
 * Mod. Initials:	      AQ
 *****************************************************************/
	virtual ~CSphere(void);


/***************
 *  Accessors
 ***************/


/*****************************************************************
 * GetPosition():	      Position Accessor
 *
 * Ins:			      
 *
 * Outs:		     
 *
 * Returns:		      D3DXVECTOR3
 *
 * Mod. Date:		      11/21/2012
 * Mod. Initials:	      AQ
 *****************************************************************/
	D3DXVECTOR3 GetPosition() 
	{
		return m_d3dPosition;
	}

 /*****************************************************************
 * GetRadius():			Radius Accessor
 *
 * Ins:					
 *
 * Outs:				
 *
 * Returns:				float
 *
 * Mod. Date:			11/21/2012
 * Mod. Initials:		AQ
 *****************************************************************/
	float GetRadius() 
	{
		return m_fRadius;
	}

/***************
 *  Mutators
 ***************/

/*****************************************************************
 * SetPosition():	      Position Mutator
 *
 * Ins:			      _newPos
 *
 * Outs:		     
 *
 * Returns:		      
 *
 * Mod. Date:		      11/21/2012
 * Mod. Initials:	      AQ
 *****************************************************************/
	void SetPosition(D3DXVECTOR3 _d3dNewPos) 
	{
		m_d3dPosition = _d3dNewPos;
	}

 /*****************************************************************
 * SetPosition():		Position Mutator
 *
 * Ins:					_x
 *						_y
 *						_z
 *
 * Outs:		     
 *
 * Returns:		      
 *
 * Mod. Date:			11/21/2012
 * Mod. Initials:		AQ
 *****************************************************************/
	void SetPosition(float _fX, float _fY, float _fZ) 
	{
		m_d3dPosition = D3DXVECTOR3(_fX, _fY, _fZ);
	}
	
 /*****************************************************************
 * SetPosition():		Radius Mutator
 *
 * Ins:					_newRad
 *
 * Outs:		     
 *
 * Returns:		      
 *
 * Mod. Date:			11/21/2012
 * Mod. Initials:		AQ
 *****************************************************************/
	void SetRadius(float _fNewRad) 
	{
		m_fRadius = _fNewRad;
	}

	float GetFrustumRadius() {return m_fFrustumRadius;}
	void SetFrustumRadius(float _fFrustumRadius) {m_fFrustumRadius = _fFrustumRadius;}
};

#endif
