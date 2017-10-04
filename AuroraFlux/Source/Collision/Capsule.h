/***********************************************
 * Filename:  		Capsule.h
 * Date:      		11/21/2012
 * Mod. Date: 		11/29/2012
 * Mod. Initials:	AQ
 * Author:    		Alexander G. Garcia de Quevedo
 * Purpose:   		This is a child class of CCollOBJ. 
 *					It acts as a bounding volume in the 
 *					shape of a capsule.
 ************************************************/

#ifndef _CAPSULE_H_
#define _CAPSULE_H_

#include "../Definitions.h"
#include "collobj.h"


class CCapsule :
	public CCollOBJ
{
	D3DXVECTOR3 m_d3dStart;
	D3DXVECTOR3 m_d3dEnd;
	float m_fRadius;
	float m_fLength;
	void SetLength();
public:

/*****************************************************************
 * CCapsule():	      Default Constructor
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
	CCapsule(void);

/*****************************************************************
 * ~CCapsule():	      Default Destructor
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
	virtual ~CCapsule(void);

	float GetLength() {return m_fLength;}
	
/***************
 *  Accessors
 ***************/

/*****************************************************************
 * GetStart():	      Start Vector Accessor
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
	D3DXVECTOR3 GetStart()							
	{
		return m_d3dStart;
	}

/*****************************************************************
 * GetEnd():	      End Vector Accessor
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
	D3DXVECTOR3 GetEnd()							
	{
		return m_d3dEnd;
	}

/*****************************************************************
 * GetRadius():	      Radius Accessor
 *
 * Ins:			      
 *
 * Outs:		     
 *
 * Returns:		      float
 *
 * Mod. Date:		      11/21/2012
 * Mod. Initials:	      AQ
 *****************************************************************/
	float GetRadius()								
	{
		return m_fRadius;
	}


/***************
 *  Mutators
 ***************/

/*****************************************************************
 * SetStart():	      Start Vector Mutator
 *
 * Ins:			      _newStart
 *
 * Outs:		     
 *
 * Returns:		      
 *
 * Mod. Date:		      11/21/2012
 * Mod. Initials:	      AQ
 *****************************************************************/
	void SetStart(D3DXVECTOR3 _d3dNewStart);

/*****************************************************************
 * SetStart():	      Start Vector Mutator
 *
 * Ins:				_x
 *					_y
 *					_z
 *
 * Outs:		     
 *
 * Returns:		      
 *
 * Mod. Date:		      11/21/2012
 * Mod. Initials:	      AQ
 *****************************************************************/
	void SetStart(float _fX, float _fY, float _fZ);

/*****************************************************************
 * SetEnd():	      End Vector Mutator
 *
 * Ins:			      _newEnd
 *
 * Outs:		     
 *
 * Returns:		      
 *
 * Mod. Date:		      11/21/2012
 * Mod. Initials:	      AQ
 *****************************************************************/
	void SetEnd(D3DXVECTOR3 _d3dNewEnd);

/*****************************************************************
 * SetEnd():	      End Vector Mutator
 *
 * Ins:			      _x
 *					_y
 *					_z
 *
 * Outs:		     
 *
 * Returns:		      
 *
 * Mod. Date:		      11/21/2012
 * Mod. Initials:	      AQ
 *****************************************************************/
	void SetEnd(float _fX, float _fY, float _fZ);

/*****************************************************************
 * GetRadius():	      Radius Mutator
 *
 * Ins:			      _newRad
 *
 * Outs:		     
 *
 * Returns:		      
 *
 * Mod. Date:		      11/21/2012
 * Mod. Initials:	      AQ
 *****************************************************************/
	void SetRadius(float _fNewRad)					
	{
		m_fRadius = _fNewRad;
	}
};

#endif