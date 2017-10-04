/***********************************************
* Filename:  		Physics.h
* Date:      		11/20/2012
* Mod. Date: 		12/11/2012
* Mod. Initials:	AQ
* Author:    		Andrew A. Rosser
* Purpose:   		This is a basic helper class that
*					does more complicated math functions
*					that don't deal with collision directly
************************************************/

#ifndef	CPHYSICS_H
#define CPHYSICS_H
#include "../Definitions.h"


class CPhysics
{
	friend class IEntity;
	friend class CEntity;

public:

	/*****************************************************************
	* CPhysics():		Default Constructor
	*
	* Ins:				
	*
	* Outs:			
	*
	* Returns:			
	*
	* Mod. Date:		      11/25/12
	* Mod. Initials:	      AR
	*****************************************************************/
	CPhysics(void);

	/*****************************************************************
	* ~CPhysics():		Default Destructor
	*
	* Ins:				
	*
	* Outs:			
	*
	* Returns:			
	*
	* Mod. Date:		      11/25/12
	* Mod. Initials:	      AR
	*****************************************************************/
	~CPhysics(void);

	/*****************************************************************
	* TurnTo():		Turns the source towards the target at a specified
	*					rate, then scaled by time.
	*
	* Ins:				_psource
	*					_d3dptarget(d3dxvector3*)
	*					_fdt
	*					_frate
	*
	* Outs:			
	*
	* Returns:			
	*
	* Mod. Date:		      12/14/12
	* Mod. Initials:	      AQ
	*****************************************************************/
	static void TurnTo( D3DXMATRIX *_pSource, D3DXVECTOR3 *_d3dpTarget, float _fDt, float _fRate );


	/*****************************************************************
	* TurnTo():		Turns the source towards the target at a specified
	*					rate, then scaled by time.
	*
	* Ins:				_psource
	*					_d3dptarget(d3dxvector3*)
	*					_fdt
	*					_frate
	*
	* Outs:			
	*
	* Returns:			
	*
	* Mod. Date:		      12/14/12
	* Mod. Initials:	      AQ
	*****************************************************************/
	static void TurnTo( CEntity *_pSource, D3DXVECTOR3 *_d3dpTarget, float _fDt, float _fRate );

	/*****************************************************************
	* TurnTo():		Turns the source towards the target at a specified
	*					rate, then scaled by time.
	*
	* Ins:				_psource
	*					_ptarget(centity*)
	*					_fdt
	*					_frate
	*
	* Outs:			
	*
	* Returns:			
	*
	* Mod. Date:		      12/14/12
	* Mod. Initials:	      AQ
	*****************************************************************/
	static void TurnTo( CEntity *_pSource, CEntity *_pTarget, float _fDt, float _fRate );

	/*****************************************************************
	* DotProduct():	Turns the source towards the target at a specified
	*					rate, then scaled by time.
	*
	* Ins:				_d3dlhs
	*					_d3drhs
	*
	* Outs:			
	*
	* Returns:			float
	*
	* Mod. Date:		      11/25/12
	* Mod. Initials:	      AR
	*****************************************************************/
	static float DotProduct( D3DXVECTOR3 _d3dLhs, D3DXVECTOR3 _d3dRhs );

	/*****************************************************************
	* LookAt():		points the source at the target
	*
	* Ins:				_psource
	*					_ptarget(centity*)
	*
	* Outs:			
	*
	* Returns:			
	*
	* Mod. Date:		      12/14/12
	* Mod. Initials:	      AQ
	*****************************************************************/
	static void LookAt( CEntity *_pSource, CEntity *_pTarget);

	/*****************************************************************
	* LookAt():		points the source at the target
	*
	* Ins:				_psource
	*					_d3dtarget(d3dxvector3)
	*
	* Outs:			
	*
	* Returns:			
	*
	* Mod. Date:		      12/14/12
	* Mod. Initials:	      AQ
	*****************************************************************/
	static void LookAt( CEntity *_pSource, D3DXVECTOR3 _d3dTarget);

	/*****************************************************************
	* LookAt():		points the source at the target
	*
	* Ins:				_psource
	*					_d3dtarget(d3dxmatrix)
	*
	* Outs:			
	*
	* Returns:			
	*
	* Mod. Date:		      12/14/12
	* Mod. Initials:	      AQ
	*****************************************************************/
	static void LookAt( CEntity *_pSource, D3DXMATRIX _d3dTarget);


	static void CPhysics::LookAt( D3DXMATRIX *_d3dSource, D3DXMATRIX *_d3dTarget);


	/*****************************************************************
	* GetMat4Vec():	Returns a specified axis in a 4x4 matrix
	*
	* Ins:				_whichVec
	*					_mat
	*
	* Outs:			
	*
	* Returns:			D3DXVECTOR3
	*
	* Mod. Date:		      11/25/12
	* Mod. Initials:	      AR
	*****************************************************************/
	static D3DXVECTOR3 GetMat4Vec(MATRIX_AXIS _eWhichVec, D3DXMATRIX* _d3dpMat);


	/*****************************************************************
	* SetAxis():	Sets a specified axis in a 4x4 matrix
	*
	* Ins:				_whichVec
	*					D3DXMATRIX& _mat: the matrix to change
	*
	* Outs:			
	*
	* Returns:			void
	*
	* Mod. Date:		      12/03/12
	* Mod. Initials:	      SW
	*****************************************************************/
	static void SetAxis(MATRIX_AXIS _eWhichVec, D3DXMATRIX& _d3dMat, D3DXVECTOR3 _d3dVec);

	static void ReverseDirection( D3DXMATRIX *_pSource, D3DXVECTOR3 *_d3dpTarget, float _fDt, float _fRate );
	static void CPhysics::DodgeReverseDirection( D3DXMATRIX *_pSource, D3DXVECTOR3 *_d3dpTarget, float _fDt, float _fRate );
};


#endif