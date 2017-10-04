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

/*#include "../StdAfx.h"*/
#include "AABB.h"

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
CAABB::CAABB(void)
{
	m_eType = eAABB;
	m_d3dMax = D3DXVECTOR3(0,0,0);
	m_d3dMin = D3DXVECTOR3(0,0,0);
}

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
CAABB::~CAABB(void)
{
}
