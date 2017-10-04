/***********************************************
 * Filename:  		Capsule.cpp
 * Date:      		11/21/2012
 * Mod. Date: 		11/29/2012
 * Mod. Initials:	AQ
 * Author:    		Alexander G. Garcia de Quevedo
 * Purpose:   		This is a child class of CCollOBJ. 
 *					It acts as a bounding volume in the 
 *					shape of a capsule.
 ************************************************/

/*#include "../StdAfx.h"*/
#include "Capsule.h"

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
CCapsule::CCapsule(void)
{
	m_eType = eCAP;
	m_d3dEnd = D3DXVECTOR3(0,0,0);
	m_d3dStart = D3DXVECTOR3(0,0,0);
	m_fRadius = 0;
	SetLength();
}

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
CCapsule::~CCapsule(void)
{

}

void CCapsule::SetLength()
{
	m_fLength = sqrt( ( ( m_d3dStart.x - m_d3dEnd.x ) * ( m_d3dStart.x - m_d3dEnd.x ) ) + ( ( m_d3dStart.y - m_d3dEnd.y ) * ( m_d3dStart.y - m_d3dEnd.y ) ) + ( ( m_d3dStart.z - m_d3dEnd.z ) * ( m_d3dStart.z - m_d3dEnd.z ) ) );
}

void CCapsule::SetStart(D3DXVECTOR3 _d3dNewStart)			
{
	m_d3dStart = _d3dNewStart;
	SetLength();
}
void CCapsule::SetStart(float _fX, float _fY, float _fZ)		
{
	m_d3dStart = D3DXVECTOR3(_fX, _fY, _fZ);
	SetLength();
}


void CCapsule::SetEnd(D3DXVECTOR3 _d3dNewEnd)				
{
	m_d3dEnd = _d3dNewEnd;
	SetLength();
}

void CCapsule::SetEnd(float _fX, float _fY, float _fZ)		
{
	m_d3dEnd = D3DXVECTOR3(_fX, _fY, _fZ);
	SetLength();
}