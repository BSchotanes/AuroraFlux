/***********************************************
 * Filename:  		CollOBJ.cpp
 * Date:      		11/21/2012
 * Mod. Date: 		11/29/2012
 * Mod. Initials:	AQ
 * Author:    		Alexander G. Garcia de Quevedo
 * Purpose:   		This is the bounding volume parent
 *					class.  All bounding volume classes 
 *					inherit from this class.  This class 
 *					holds the function that all Entities
 *					need to use to create BVs properly.
 ************************************************/

/*#include "../StdAfx.h"*/
#include "CollOBJ.h"
#include "Sphere.h"
#include "AABB.h"
#include "Capsule.h"



/*****************************************************************
 * CCollOBJ():	      Default Constructor
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
CCollOBJ::CCollOBJ(void)
{
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
CCollOBJ::~CCollOBJ(void)
{
}


/*****************************************************************
 * Create():		Function that creates a child version of this
 *					class, then stores it in parent form. 
 *
 * Ins:				_type
 *					... (this will be expanded on below)
 *					Spheres - d3dxvector3 position, float radius
 *					AABBs - d3dcvector3 min, d3dxvector3 max
 *					Capsules - d3dxvector3 start, d3dxvector3 end, float radius
 *					Frustum - float _fov, float _nearDist, float _farDist, float _ratio, D3DXMATRIX& _leMat
 *
 * Outs:		     
 *
 * Returns:		      CCollOBJ*
 *
 * Mod. Date:		      12/4/2012
 * Mod. Initials:	      AQ
 *****************************************************************/
CCollOBJ* CCollOBJ::Create( CollObjType _eType, ... )
{
	va_list VariableArgList;
	va_start(VariableArgList, _eType);

	switch(_eType)
	{
	case eSPH:
		{
			CSphere* pTmpSPH = new CSphere();
			pTmpSPH->SetPosition(va_arg(VariableArgList, D3DXVECTOR3));
			pTmpSPH->SetRadius((float)va_arg(VariableArgList, double));
			pTmpSPH->SetFrustumRadius((float)va_arg(VariableArgList, double));
			va_end(VariableArgList);
			return pTmpSPH;
			break;
		}
		
	case eAABB:
		{
			CAABB* pTmpAABB = new CAABB();
			pTmpAABB->SetMin(va_arg(VariableArgList, D3DXVECTOR3));
			pTmpAABB->SetMax(va_arg(VariableArgList, D3DXVECTOR3));
			va_end(VariableArgList);
			return pTmpAABB;
			break;
		}
	case eCAP:
		{
			CCapsule* pTmpCap = new CCapsule();
			pTmpCap->SetStart(va_arg(VariableArgList, D3DXVECTOR3));
			pTmpCap->SetEnd(va_arg(VariableArgList, D3DXVECTOR3));
			pTmpCap->SetRadius((float)va_arg(VariableArgList, double));
			va_end(VariableArgList);
			return pTmpCap;
			break;
		}
	//case eFSTM:
	//	{
	//		CFrustum* pTmpFstm = new CFrustum(	
	//			(float)va_arg(VariableArgList, double), // _fov,
	//			(float)va_arg(VariableArgList, double), // _nearDist,
	//			(float)va_arg(VariableArgList, double), // _farDist,
	//			(float)va_arg(VariableArgList, double), // _ratio,
	//			va_arg(VariableArgList, D3DXMATRIX));	// _leMat
	//		va_end(VariableArgList);
	//		return pTmpFstm;			
	//	}
	//	break;
	}

	va_end(VariableArgList);
	return 0;
}
