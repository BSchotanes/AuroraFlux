/***********************************************
 * Filename:  		CollOBJ.h
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


// TODO: refine create function to deal with new data member lists in child classes


#ifndef _COLLOBJ_H_
#define _COLLOBJ_H_

#include "../Definitions.h"
class CCollOBJ
{

protected:
	CollObjType m_eType;

public:

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
	CCollOBJ(void);

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
	virtual ~CCollOBJ(void);


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
	static CCollOBJ* Create(CollObjType _eType, ...);


/***************
 *  Accessors
 ***************/



/*****************************************************************
 * GetObjType():	      Type Accessor
 *
 * Ins:			      
 *
 * Outs:		     
 *
 * Returns:		      enum
 *
 * Mod. Date:		      11/21/2012
 * Mod. Initials:	      AQ
 *****************************************************************/
	CollObjType GetObjType() 
	{
		return m_eType;
	}

};

#endif
