/***********************************************
 * Filename:  		AABB.h
 * Date:      		11/21/2012
 * Mod. Date: 		11/29/2012
 * Mod. Initials:	AQ
 * Author:    		Alexander G. Garcia de Quevedo
 * Purpose:   		This is a child class of CCollOBJ. 
 *					It acts as a bounding volume in the 
 *					shape of a Axis-aligned bounding box.
 ************************************************/

#ifndef _AABB_H_
#define _AABB_H_

#include "../Definitions.h"
#include "collobj.h"


class CAABB :
	public CCollOBJ
{
	D3DXVECTOR3 m_d3dMin;
	D3DXVECTOR3 m_d3dMax;
public:

/*****************************************************************
 * CAABB():	      Default Constructor
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
	CAABB(void);

/*****************************************************************
 * ~CCollOBJ():	      Default Destructor
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
	virtual ~CAABB(void);

/***************
 *  Accessors
 ***************/

/*****************************************************************
 * GetMin():	      Min Accessor
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
	D3DXVECTOR3 GetMin() 
	{
		return m_d3dMin;
	}

/*****************************************************************
 * SetMin():	      Max Accessor
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
	D3DXVECTOR3 GetMax() 
	{
		return m_d3dMax;
	}

/***************
 *  Mutators
 ***************/

/*****************************************************************
 * SetMin():	      Min Mutator
 *
 * Ins:			      _min
 *
 * Outs:		     
 *
 * Returns:		      
 *
 * Mod. Date:		      11/21/2012
 * Mod. Initials:	      AQ
 *****************************************************************/
	void SetMin(D3DXVECTOR3 _d3dMin) 
	{
		m_d3dMin = _d3dMin;
	}

/*****************************************************************
 * SetMin():	      Max Mutator
 *
 * Ins:			      _max
 *
 * Outs:		     
 *
 * Returns:		      
 *
 * Mod. Date:		      11/21/2012
 * Mod. Initials:	      AQ
 *****************************************************************/
	void SetMax(D3DXVECTOR3 _d3dMax) 
	{
		m_d3dMax = _d3dMax;
	}
};

#endif
