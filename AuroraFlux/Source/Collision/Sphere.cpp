/***********************************************
 * Filename:  		Sphere.cpp
 * Date:      		11/21/2012
 * Mod. Date: 		11/29/2012
 * Mod. Initials:	AQ
 * Author:    		Alexander G. Garcia de Quevedo
 * Purpose:   		This is a child class of CCollOBJ. 
 *					It acts as a bounding volume in the 
 *					shape of a sphere.
 ************************************************/

/*#include "../StdAfx.h"*/
#include "Sphere.h"


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
CSphere::CSphere(void)
{
	m_eType = eSPH;
	m_fRadius = 0;
	m_fFrustumRadius = 0;
	m_d3dPosition = D3DXVECTOR3(0,0,0);

}


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
CSphere::~CSphere(void)
{
}
