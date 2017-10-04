/***********************************************
* Filename:  		Physics.h
* Date:      		11/20/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AQ
* Author:    		Andrew A. Rosser
* Purpose:   		This is a basic helper class that
*					does more complicated math functions
*					that don't deal with collsion directly
************************************************/

#include "Physics.h"
#include "../Entity/Entity.h"
#include "../Timer/CTimer.h"

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
CPhysics::CPhysics(void)
{

}

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
CPhysics::~CPhysics(void)
{

}

/*****************************************************************
* DotProduct():	Turns the source towards the target at a specified
*					rate, then scaled by time.
*
* Ins:				_lhs
*					_rhs
*
* Outs:			
*
* Returns:			float
*
* Mod. Date:		      11/25/12
* Mod. Initials:	      AR
*****************************************************************/
float CPhysics::DotProduct( D3DXVECTOR3 _d3dLhs, D3DXVECTOR3 _d3dRhs )
{
	return (_d3dLhs.x * _d3dRhs.x) + (_d3dLhs.y * _d3dRhs.y) + (_d3dLhs.z * _d3dRhs.z);
}

void CPhysics::ReverseDirection( D3DXMATRIX *_pSource, D3DXVECTOR3 *_d3dpTarget, float _fDt, float _fRate )
{
	if( _d3dpTarget == nullptr )
	{
		return ;
	}

	D3DXMATRIX d3dmRotation;
	D3DXMatrixIdentity(&d3dmRotation);
	D3DXVECTOR3 sourcePos = D3DXVECTOR3(_pSource->_41, _pSource->_42, _pSource->_43 );
	D3DXVECTOR3 d3dOrientation = *_d3dpTarget - sourcePos;

	D3DXVECTOR3 d3dSourceXaxis = D3DXVECTOR3( _pSource->_11, _pSource->_12, _pSource->_13);
	D3DXVECTOR3 d3dSourceYaxis = D3DXVECTOR3( _pSource->_21, _pSource->_22, _pSource->_23);
	D3DXVECTOR3 d3dSourceZaxis = D3DXVECTOR3( _pSource->_31, _pSource->_32, _pSource->_33);
	
	D3DXVECTOR3 d3dXAxis = CPhysics::GetMat4Vec(eXAXIS, _pSource);
	D3DXVECTOR3 d3dYAxis = CPhysics::GetMat4Vec(eYAXIS, _pSource);

	if(DotProduct(d3dOrientation, d3dSourceXaxis) >= EPSILON )
	{
		//cout << "Dodge Up" << '\n';

		D3DXVECTOR3 d3dLocalY(_pSource->_21, _pSource->_22, _pSource->_23);
		
		D3DXMatrixRotationAxis(&d3dmRotation, &d3dLocalY, D3DXToRadian(DotProduct(d3dOrientation, d3dSourceXaxis) * _fDt * _fRate));
		
		_pSource->_41 = _pSource->_42 = _pSource->_43 = 0.0f; 

		D3DXMatrixMultiply(_pSource, _pSource, &d3dmRotation);	
	}

	if(DotProduct(d3dOrientation, d3dSourceXaxis) < -EPSILON)
	{
		//cout << "Dodge Down" << '\n';
		D3DXVECTOR3 d3dLocalY(_pSource->_21, _pSource->_22, _pSource->_23);

		D3DXMatrixRotationAxis(&d3dmRotation, &d3dLocalY, D3DXToRadian(DotProduct(d3dOrientation, d3dSourceXaxis) * _fDt * _fRate));
		
		_pSource->_41 = _pSource->_42 = _pSource->_43 = 0.0f;  

		D3DXMatrixMultiply(_pSource, _pSource, &d3dmRotation);
	}

	if(DotProduct(d3dOrientation, d3dSourceYaxis) >= EPSILON )
	{
		//cout << "Dodge Left" << '\n';	
		D3DXVECTOR3 d3dLocalX(_pSource->_11, _pSource->_12, _pSource->_13);
		D3DXMatrixRotationAxis(&d3dmRotation, &d3dLocalX, D3DXToRadian(-DotProduct(d3dOrientation, d3dSourceYaxis) * _fDt * _fRate));
		
		_pSource->_41 = _pSource->_42 = _pSource->_43 = 0.0f;  
		
		D3DXMatrixMultiply(_pSource, _pSource, &d3dmRotation);
				
	}

	if(DotProduct(d3dOrientation, d3dSourceYaxis) < -EPSILON)
	{
		//cout << "Dodge Right" << '\n';	
		D3DXVECTOR3 d3dLocalX(_pSource->_11, _pSource->_12, _pSource->_13);
		D3DXMatrixRotationAxis(&d3dmRotation, &d3dLocalX, D3DXToRadian(-DotProduct(d3dOrientation, d3dSourceYaxis) * _fDt * _fRate));
		
		_pSource->_41 = _pSource->_42 = _pSource->_43 = 0.0f;
		
		D3DXMatrixMultiply(_pSource, _pSource, &d3dmRotation);
	
	}

	//turnToPtr->gameObject->SetLocalTransform(mm);

	_pSource->_41 = sourcePos.x;
	_pSource->_42 = sourcePos.y;
	_pSource->_43 = sourcePos.z;

	
}

void CPhysics::DodgeReverseDirection( D3DXMATRIX *_pSource, D3DXVECTOR3 *_d3dpTarget, float _fDt, float _fRate )
{
	if( _d3dpTarget == nullptr )
	{
		return ;
	}

	D3DXMATRIX d3dmRotation;
	D3DXMatrixIdentity(&d3dmRotation);
	D3DXVECTOR3 sourcePos = D3DXVECTOR3(_pSource->_41, _pSource->_42, _pSource->_43 );
	D3DXVECTOR3 d3dOrientation = *_d3dpTarget - sourcePos;

	D3DXVECTOR3 d3dSourceXaxis = D3DXVECTOR3( _pSource->_11, _pSource->_12, _pSource->_13);
	D3DXVECTOR3 d3dSourceYaxis = D3DXVECTOR3( _pSource->_21, _pSource->_22, _pSource->_23);
	D3DXVECTOR3 d3dSourceZaxis = D3DXVECTOR3( _pSource->_31, _pSource->_32, _pSource->_33);
	
	D3DXVECTOR3 d3dXAxis = CPhysics::GetMat4Vec(eXAXIS, _pSource);
	D3DXVECTOR3 d3dYAxis = CPhysics::GetMat4Vec(eYAXIS, _pSource);

	//if(DotProduct(d3dOrientation, d3dSourceXaxis) >= EPSILON )
	//{
	//	//cout << "Dodge Up" << '\n';
	//
	//	D3DXVECTOR3 d3dLocalY(_pSource->_21, _pSource->_22, _pSource->_23);
	//	
	//	D3DXMatrixRotationAxis(&d3dmRotation, &d3dLocalY, D3DXToRadian(DotProduct(d3dOrientation, d3dSourceXaxis) * _fDt * _fRate));
	//	
	//	_pSource->_41 = _pSource->_42 = _pSource->_43 = 0.0f; 
	//
	//	D3DXMatrixMultiply(_pSource, _pSource, &d3dmRotation);	
	//}
	//
	//if(DotProduct(d3dOrientation, d3dSourceXaxis) < -EPSILON)
	//{
	//	//cout << "Dodge Down" << '\n';
	//	D3DXVECTOR3 d3dLocalY(_pSource->_21, _pSource->_22, _pSource->_23);
	//
	//	D3DXMatrixRotationAxis(&d3dmRotation, &d3dLocalY, D3DXToRadian(DotProduct(d3dOrientation, d3dSourceXaxis) * _fDt * _fRate));
	//	
	//	_pSource->_41 = _pSource->_42 = _pSource->_43 = 0.0f;  
	//
	//	D3DXMatrixMultiply(_pSource, _pSource, &d3dmRotation);
	//}

	//if(DotProduct(d3dOrientation, d3dSourceYaxis) < -EPSILON )
	//{
		//cout << "Dodge Left" << '\n';	
		D3DXVECTOR3 d3dLocalX(_pSource->_11, _pSource->_12, _pSource->_13);
		D3DXMatrixRotationAxis(&d3dmRotation, &d3dLocalX, D3DXToRadian(-DotProduct(d3dOrientation, d3dSourceYaxis) * _fDt * _fRate));
		
		_pSource->_41 = _pSource->_42 = _pSource->_43 = 0.0f;  
		
		D3DXMatrixMultiply(_pSource, _pSource, &d3dmRotation);
				
	//}

	//if(DotProduct(d3dOrientation, d3dSourceYaxis) >= EPSILON)
	//{
	//	//cout << "Dodge Right" << '\n';	
	//	D3DXVECTOR3 d3dLocalX(_pSource->_11, _pSource->_12, _pSource->_13);
	//	D3DXMatrixRotationAxis(&d3dmRotation, &d3dLocalX, D3DXToRadian(-DotProduct(d3dOrientation, d3dSourceYaxis) * _fDt * _fRate));
	//	
	//	_pSource->_41 = _pSource->_42 = _pSource->_43 = 0.0f;
	//	
	//	D3DXMatrixMultiply(_pSource, _pSource, &d3dmRotation);
	//
	//}

	//turnToPtr->gameObject->SetLocalTransform(mm);

	_pSource->_41 = sourcePos.x;
	_pSource->_42 = sourcePos.y;
	_pSource->_43 = sourcePos.z;

	
}

void CPhysics::TurnTo( D3DXMATRIX *_pSource, D3DXVECTOR3 *_d3dpTarget, float _fDt, float _fRate )
{
	if( _d3dpTarget == nullptr )
	{
		return;
	}

	D3DXVECTOR3 d3dWorldUp;
	d3dWorldUp.x = 0;
	d3dWorldUp.y = 1;
	d3dWorldUp.z = 0;

	D3DXVECTOR3 sourcePos = D3DXVECTOR3(_pSource->_41, _pSource->_42, _pSource->_43 );
	D3DXVECTOR3 d3dOrientation = *_d3dpTarget - sourcePos;

	D3DXVECTOR3 d3dSourceXaxis = D3DXVECTOR3( _pSource->_11, _pSource->_12, _pSource->_13);
	D3DXVECTOR3 d3dSourceYaxis = D3DXVECTOR3( _pSource->_21, _pSource->_22, _pSource->_23);
	D3DXVECTOR3 d3dSourceZaxis = D3DXVECTOR3( _pSource->_31, _pSource->_32, _pSource->_33);

	if(DotProduct(d3dOrientation, d3dSourceXaxis) >= EPSILON )
	{
		float _degrees = (DotProduct(d3dOrientation,d3dSourceXaxis) * _fDt * _fRate);
	
		D3DXMATRIX d3dRotateYMatrix;
		D3DXMatrixIdentity(&d3dRotateYMatrix);
 
		d3dRotateYMatrix._11 = cos(_degrees);
		d3dRotateYMatrix._31 = sin(_degrees);
		d3dRotateYMatrix._13 = -sin(_degrees);
		d3dRotateYMatrix._33 = cos(_degrees);
		(*_pSource) = d3dRotateYMatrix * (*_pSource);

	}

	if(DotProduct(d3dOrientation, d3dSourceXaxis) < -EPSILON)
	{
		float _degrees = (DotProduct(d3dOrientation,d3dSourceXaxis) * _fDt * _fRate);
	
		D3DXMATRIX d3dRotateYMatrix;
		D3DXMatrixIdentity(&d3dRotateYMatrix);
 
		d3dRotateYMatrix._11 = cos(_degrees);
		d3dRotateYMatrix._31 = sin(_degrees);
		d3dRotateYMatrix._13 = -sin(_degrees);
		d3dRotateYMatrix._33 = cos(_degrees);
		(*_pSource) = d3dRotateYMatrix * (*_pSource);
	}

	if(DotProduct(d3dOrientation, d3dSourceYaxis) >= EPSILON )
	{
		float _degrees = (-DotProduct(d3dOrientation, d3dSourceYaxis) * _fDt * _fRate );
	
		D3DXMATRIX d3dRotateXMatrix;
		D3DXMatrixIdentity(&d3dRotateXMatrix);
		d3dRotateXMatrix._22 = cos(_degrees);
		d3dRotateXMatrix._32 = -sin(_degrees);
		d3dRotateXMatrix._23 = sin(_degrees);
		d3dRotateXMatrix._33 = cos(_degrees);
		
		(*_pSource) = d3dRotateXMatrix * (*_pSource);
	}

	if(DotProduct(d3dOrientation, d3dSourceYaxis) < -EPSILON)
	{
		float _degrees = (-DotProduct(d3dOrientation, d3dSourceYaxis) * _fDt * _fRate );
	
		D3DXMATRIX d3dRotateXMatrix;
		D3DXMatrixIdentity(&d3dRotateXMatrix);
		d3dRotateXMatrix._22 = cos(_degrees);
		d3dRotateXMatrix._32 = -sin(_degrees);
		d3dRotateXMatrix._23 = sin(_degrees);
		d3dRotateXMatrix._33 = cos(_degrees);
		
		(*_pSource) = d3dRotateXMatrix * (*_pSource);
	}
	d3dSourceZaxis = D3DXVECTOR3( _pSource->_31, _pSource->_32, _pSource->_33);

	D3DXVECTOR3 d3dZAxis = d3dSourceZaxis;

	D3DXVECTOR3 d3dXAxis;
	D3DXVec3Cross(&d3dXAxis,&d3dWorldUp ,  &d3dZAxis);

	D3DXVec3Normalize( &d3dXAxis, &d3dXAxis );

	D3DXVECTOR3 d3dYAxis;
	D3DXVec3Cross(&d3dYAxis, &d3dZAxis,  &d3dXAxis);
	D3DXVec3Normalize(&d3dYAxis, &d3dYAxis);
	D3DXMATRIX d3dMM;
	D3DXMatrixIdentity(&d3dMM);
	//mm.XAxis = xAxis;
	d3dMM._11 = d3dXAxis.x;
	d3dMM._12 = d3dXAxis.y;
	d3dMM._13 = d3dXAxis.z;

	//mm.YAxis = yAxis;
	d3dMM._21 = d3dYAxis.x;
	d3dMM._22 = d3dYAxis.y;
	d3dMM._23 = d3dYAxis.z;

	//mm.ZAxis = zAxis;
	d3dMM._31 = d3dZAxis.x;
	d3dMM._32 = d3dZAxis.y;
	d3dMM._33 = d3dZAxis.z;

	//mm.WAxis = turnToPtr->gameObject->GetLocalTransform().WAxis;

	d3dMM._41 =  _pSource->_41;
	d3dMM._42 =  _pSource->_42;
	d3dMM._43 =  _pSource->_43;

	//turnToPtr->gameObject->SetLocalTransform(mm);

	//((CEntity*)_pSource)->SetMatrix(&d3dMM);
	(*_pSource) = d3dMM;
}

/*****************************************************************
* LookAt():		points the source at the target
*
* Ins:				_source
*					_target
*
* Outs:			
*
* Returns:			
*
* Mod. Date:		      11/25/12
* Mod. Initials:	      AR
*****************************************************************/
void CPhysics::LookAt( CEntity *_pSource, CEntity *_pTarget)
{
	D3DXVECTOR3 d3dWorldUp;
	d3dWorldUp.x = 0;
	d3dWorldUp.y = 1;
	d3dWorldUp.z = 0;

	D3DXVECTOR3 d3dOrientation = _pTarget->GetPosition() - _pSource->GetPosition();

	D3DXVECTOR3 d3dZAxis = d3dOrientation;

	D3DXVec3Normalize( &d3dZAxis, &d3dZAxis );

	D3DXVECTOR3 d3dXAxis;
	D3DXVec3Cross(&d3dXAxis,&d3dWorldUp ,  &d3dZAxis);

	D3DXVec3Normalize( &d3dXAxis, &d3dXAxis );

	D3DXVECTOR3 d3dYAxis;
	D3DXVec3Cross(&d3dYAxis, &d3dZAxis,  &d3dXAxis);
	D3DXVec3Normalize(&d3dYAxis, &d3dYAxis);
	D3DXMATRIX d3dMM;
	D3DXMatrixIdentity(&d3dMM);
	//mm.XAxis = xAxis;
	d3dMM._11 = d3dXAxis.x;
	d3dMM._12 = d3dXAxis.y;
	d3dMM._13 = d3dXAxis.z;

	//mm.YAxis = yAxis;
	d3dMM._21 = d3dYAxis.x;
	d3dMM._22 = d3dYAxis.y;
	d3dMM._23 = d3dYAxis.z;

	//mm.ZAxis = zAxis;
	d3dMM._31 = d3dZAxis.x;
	d3dMM._32 = d3dZAxis.y;
	d3dMM._33 = d3dZAxis.z;

	//mm.WAxis = turnToPtr->gameObject->GetLocalTransform().WAxis;

	d3dMM._41 =  _pSource->GetMatrix()->_41;
	d3dMM._42 =  _pSource->GetMatrix()->_42;
	d3dMM._43 =  _pSource->GetMatrix()->_43;


	//turnToPtr->gameObject->SetLocalTransform(mm);
	((CEntity*)_pSource)->SetMatrix(&d3dMM);
}
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
void CPhysics::LookAt(CEntity *_pSource, D3DXMATRIX _d3dTarget)
{

	D3DXVECTOR3 d3dWorldUp;
	d3dWorldUp.x = 0;
	d3dWorldUp.y = 1;
	d3dWorldUp.z = 0;

	D3DXVECTOR3 d3dTarget = GetMat4Vec(ePOS, &_d3dTarget);

	D3DXVECTOR3 d3dOrientation = d3dTarget - _pSource->GetPosition();

	D3DXVECTOR3 d3dZAxis = d3dOrientation;

	D3DXVec3Normalize( &d3dZAxis, &d3dZAxis );

	D3DXVECTOR3 d3dXAxis;
	D3DXVec3Cross(&d3dXAxis,&d3dWorldUp ,  &d3dZAxis);

	D3DXVec3Normalize( &d3dXAxis, &d3dXAxis );

	D3DXVECTOR3 d3dYAxis;
	D3DXVec3Cross(&d3dYAxis, &d3dZAxis,  &d3dXAxis);
	D3DXVec3Normalize(&d3dYAxis, &d3dYAxis);
	D3DXMATRIX d3dMM;
	D3DXMatrixIdentity(&d3dMM);
	//mm.XAxis = xAxis;
	d3dMM._11 = d3dXAxis.x;
	d3dMM._12 = d3dXAxis.y;
	d3dMM._13 = d3dXAxis.z;

	//mm.YAxis = yAxis;
	d3dMM._21 = d3dYAxis.x;
	d3dMM._22 = d3dYAxis.y;
	d3dMM._23 = d3dYAxis.z;

	//mm.ZAxis = zAxis;
	d3dMM._31 = d3dZAxis.x;
	d3dMM._32 = d3dZAxis.y;
	d3dMM._33 = d3dZAxis.z;

	//mm.WAxis = turnToPtr->gameObject->GetLocalTransform().WAxis;

	d3dMM._41 =  _pSource->GetMatrix()->_41;
	d3dMM._42 =  _pSource->GetMatrix()->_42;
	d3dMM._43 =  _pSource->GetMatrix()->_43;

	//turnToPtr->gameObject->SetLocalTransform(mm);

	((CEntity*)_pSource)->SetMatrix(&d3dMM);

}


void CPhysics::LookAt( D3DXMATRIX *_d3dSource, D3DXMATRIX *_d3dTarget)
{
	D3DXVECTOR3 d3dWorldUp;
	d3dWorldUp.x = 0;
	d3dWorldUp.y = 1;
	d3dWorldUp.z = 0;

	D3DXVECTOR3 d3dTarget = GetMat4Vec(ePOS, _d3dTarget);
	D3DXVECTOR3 d3dSource = GetMat4Vec(ePOS, _d3dSource);

	D3DXVECTOR3 d3dOrientation = d3dTarget - d3dSource;

	D3DXVECTOR3 d3dZAxis = d3dOrientation;

	D3DXVec3Normalize( &d3dZAxis, &d3dZAxis );

	D3DXVECTOR3 d3dXAxis;
	D3DXVec3Cross(&d3dXAxis,&d3dWorldUp ,  &d3dZAxis);

	D3DXVec3Normalize( &d3dXAxis, &d3dXAxis );

	D3DXVECTOR3 d3dYAxis;
	D3DXVec3Cross(&d3dYAxis, &d3dZAxis,  &d3dXAxis);
	D3DXVec3Normalize(&d3dYAxis, &d3dYAxis);

	D3DXMATRIX d3dMM;
	D3DXMatrixIdentity(&d3dMM);
	//mm.XAxis = xAxis;
	_d3dSource->_11 = d3dXAxis.x;
	_d3dSource->_12 = d3dXAxis.y;
	_d3dSource->_13 = d3dXAxis.z;
	_d3dSource->_14 = 0;

	//mm.YAxis = yAxis;
	_d3dSource->_21 = d3dYAxis.x;
	_d3dSource->_22 = d3dYAxis.y;
	_d3dSource->_23 = d3dYAxis.z;
	_d3dSource->_24 = 0;


	//mm.ZAxis = zAxis;
	_d3dSource->_31 = d3dZAxis.x;
	_d3dSource->_32 = d3dZAxis.y;
	_d3dSource->_33 = d3dZAxis.z;
	_d3dSource->_34 = 0;


	//mm.WAxis = turnToPtr->gameObject->GetLocalTransform().WAxis;

	_d3dSource->_41 = d3dSource.x;
	_d3dSource->_42 = d3dSource.y;
	_d3dSource->_43 = d3dSource.z;
	_d3dSource->_44 = 1.0f;


	
	

	//turnToPtr->gameObject->SetLocalTransform(mm);

}


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

void CPhysics::LookAt( CEntity *_pSource, D3DXVECTOR3 _d3dTarget)
{
	D3DXVECTOR3 d3dWorldUp;
	d3dWorldUp.x = 0;
	d3dWorldUp.y = 1;
	d3dWorldUp.z = 0;

	D3DXVECTOR3 d3dOrientation = _d3dTarget - _pSource->GetPosition();

	D3DXVECTOR3 d3dZAxis = d3dOrientation;

	D3DXVec3Normalize( &d3dZAxis, &d3dZAxis );

	D3DXVECTOR3 d3dXAxis;
	D3DXVec3Cross(&d3dXAxis,&d3dWorldUp ,  &d3dZAxis);

	D3DXVec3Normalize( &d3dXAxis, &d3dXAxis );

	D3DXVECTOR3 d3dYAxis;
	D3DXVec3Cross(&d3dYAxis, &d3dZAxis,  &d3dXAxis);
	D3DXVec3Normalize(&d3dYAxis, &d3dYAxis);
	D3DXMATRIX d3dMM;
	D3DXMatrixIdentity(&d3dMM);
	//mm.XAxis = xAxis;
	d3dMM._11 = d3dXAxis.x;
	d3dMM._12 = d3dXAxis.y;
	d3dMM._13 = d3dXAxis.z;

	//mm.YAxis = yAxis;
	d3dMM._21 = d3dYAxis.x;
	d3dMM._22 = d3dYAxis.y;
	d3dMM._23 = d3dYAxis.z;

	//mm.ZAxis = zAxis;
	d3dMM._31 = d3dZAxis.x;
	d3dMM._32 = d3dZAxis.y;
	d3dMM._33 = d3dZAxis.z;

	//mm.WAxis = turnToPtr->gameObject->GetLocalTransform().WAxis;

	d3dMM._41 =  _pSource->GetMatrix()->_41;
	d3dMM._42 =  _pSource->GetMatrix()->_42;
	d3dMM._43 =  _pSource->GetMatrix()->_43;

	//turnToPtr->gameObject->SetLocalTransform(mm);

	((CEntity*)_pSource)->SetMatrix(&d3dMM);
}

/*****************************************************************
* TurnTo():		Turns the source towards the target at a specified
*					rate, then scaled by time.
*
* Ins:				_source
*					_target
*					_dt
*					_rate
*
* Outs:			
*
* Returns:			
*
* Mod. Date:		      11/25/12
* Mod. Initials:	      AR
*****************************************************************/
void CPhysics::TurnTo(  CEntity *_pSource, CEntity *_pTarget, float _fDt, float _fRate  )
{

	if( _pTarget == nullptr )
	{
		return;
	}

	D3DXVECTOR3 d3dWorldUp;
	d3dWorldUp.x = 0;
	d3dWorldUp.y = 1;
	d3dWorldUp.z = 0;

	D3DXVECTOR3 d3dOrientation = _pTarget->GetPosition() - _pSource->GetPosition();

	D3DXVECTOR3 d3dSourceXaxis = D3DXVECTOR3( _pSource->GetMatrix()->_11, _pSource->GetMatrix()->_12, _pSource->GetMatrix()->_13);
	D3DXVECTOR3 d3dSourceYaxis = D3DXVECTOR3( _pSource->GetMatrix()->_21, _pSource->GetMatrix()->_22, _pSource->GetMatrix()->_23);
	D3DXVECTOR3 d3dSourceZaxis = D3DXVECTOR3( _pSource->GetMatrix()->_31, _pSource->GetMatrix()->_32, _pSource->GetMatrix()->_33);

	if(DotProduct(d3dOrientation, d3dSourceXaxis) >= EPSILON )
	{
		((CEntity*)_pSource)->Rotate( (DotProduct(d3dOrientation,d3dSourceXaxis) * _fDt * _fRate), 0,1,0 );
	}

	if(DotProduct(d3dOrientation, d3dSourceXaxis) < -EPSILON)
	{
		((CEntity*)_pSource)->Rotate( (DotProduct(d3dOrientation,d3dSourceXaxis) * _fDt * _fRate), 0,1,0 );
	}

	if(DotProduct(d3dOrientation, d3dSourceYaxis) >= EPSILON )
	{
		((CEntity*)_pSource)->Rotate( (-DotProduct(d3dOrientation, d3dSourceYaxis) * _fDt * _fRate ), 1,0,0);
	}

	if(DotProduct(d3dOrientation, d3dSourceYaxis) < -EPSILON)
	{
		((CEntity*)_pSource)->Rotate( (-DotProduct(d3dOrientation, d3dSourceYaxis) * _fDt * _fRate ), 1,0,0);
	}
	d3dSourceZaxis = D3DXVECTOR3( _pSource->GetMatrix()->_31, _pSource->GetMatrix()->_32, _pSource->GetMatrix()->_33);

	D3DXVECTOR3 d3dZAxis = d3dSourceZaxis;

	D3DXVECTOR3 d3dXAxis;
	D3DXVec3Cross(&d3dXAxis,&d3dWorldUp ,  &d3dZAxis);

	D3DXVec3Normalize( &d3dXAxis, &d3dXAxis );

	D3DXVECTOR3 d3dYAxis;
	D3DXVec3Cross(&d3dYAxis, &d3dZAxis,  &d3dXAxis);
	D3DXVec3Normalize(&d3dYAxis, &d3dYAxis);
	D3DXMATRIX d3dMM;
	D3DXMatrixIdentity(&d3dMM);
	//mm.XAxis = xAxis;
	d3dMM._11 = d3dXAxis.x;
	d3dMM._12 = d3dXAxis.y;
	d3dMM._13 = d3dXAxis.z;

	//mm.YAxis = yAxis;
	d3dMM._21 = d3dYAxis.x;
	d3dMM._22 = d3dYAxis.y;
	d3dMM._23 = d3dYAxis.z;

	//mm.ZAxis = zAxis;
	d3dMM._31 = d3dZAxis.x;
	d3dMM._32 = d3dZAxis.y;
	d3dMM._33 = d3dZAxis.z;

	//mm.WAxis = turnToPtr->gameObject->GetLocalTransform().WAxis;

	d3dMM._41 =  _pSource->GetMatrix()->_41;
	d3dMM._42 =  _pSource->GetMatrix()->_42;
	d3dMM._43 =  _pSource->GetMatrix()->_43;

	//turnToPtr->gameObject->SetLocalTransform(mm);

	((CEntity*)_pSource)->SetMatrix(&d3dMM);
}

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
void CPhysics::TurnTo( CEntity *_pSource, D3DXVECTOR3 *_d3dpTarget, float _fDt, float _fRate )
{
	if( _d3dpTarget == nullptr )
	{
		return;
	}

	D3DXVECTOR3 d3dWorldUp;
	d3dWorldUp.x = 0;
	d3dWorldUp.y = 1;
	d3dWorldUp.z = 0;

	D3DXVECTOR3 d3dOrientation = *_d3dpTarget - _pSource->GetPosition();

	D3DXVECTOR3 d3dSourceXaxis = D3DXVECTOR3( _pSource->GetMatrix()->_11, _pSource->GetMatrix()->_12, _pSource->GetMatrix()->_13);
	D3DXVECTOR3 d3dSourceYaxis = D3DXVECTOR3( _pSource->GetMatrix()->_21, _pSource->GetMatrix()->_22, _pSource->GetMatrix()->_23);
	D3DXVECTOR3 d3dSourceZaxis = D3DXVECTOR3( _pSource->GetMatrix()->_31, _pSource->GetMatrix()->_32, _pSource->GetMatrix()->_33);

	if(DotProduct(d3dOrientation, d3dSourceXaxis) >= EPSILON )
	{
		((CEntity*)_pSource)->Rotate( (DotProduct(d3dOrientation,d3dSourceXaxis) * _fDt * _fRate), 0,1,0 );
	}

	if(DotProduct(d3dOrientation, d3dSourceXaxis) < -EPSILON)
	{
		((CEntity*)_pSource)->Rotate( (DotProduct(d3dOrientation,d3dSourceXaxis) * _fDt * _fRate), 0,1,0 );
	}

	if(DotProduct(d3dOrientation, d3dSourceYaxis) >= EPSILON )
	{
		((CEntity*)_pSource)->Rotate( (-DotProduct(d3dOrientation, d3dSourceYaxis) * _fDt * _fRate ), 1,0,0);
	}

	if(DotProduct(d3dOrientation, d3dSourceYaxis) < -EPSILON)
	{
		((CEntity*)_pSource)->Rotate( (-DotProduct(d3dOrientation, d3dSourceYaxis) * _fDt * _fRate ), 1,0,0);
	}
	d3dSourceZaxis = D3DXVECTOR3( _pSource->GetMatrix()->_31, _pSource->GetMatrix()->_32, _pSource->GetMatrix()->_33);

	D3DXVECTOR3 d3dZAxis = d3dSourceZaxis;

	D3DXVECTOR3 d3dXAxis;
	D3DXVec3Cross(&d3dXAxis,&d3dWorldUp ,  &d3dZAxis);

	D3DXVec3Normalize( &d3dXAxis, &d3dXAxis );

	D3DXVECTOR3 d3dYAxis;
	D3DXVec3Cross(&d3dYAxis, &d3dZAxis,  &d3dXAxis);
	D3DXVec3Normalize(&d3dYAxis, &d3dYAxis);
	D3DXMATRIX d3dMM;
	D3DXMatrixIdentity(&d3dMM);
	//mm.XAxis = xAxis;
	d3dMM._11 = d3dXAxis.x;
	d3dMM._12 = d3dXAxis.y;
	d3dMM._13 = d3dXAxis.z;

	//mm.YAxis = yAxis;
	d3dMM._21 = d3dYAxis.x;
	d3dMM._22 = d3dYAxis.y;
	d3dMM._23 = d3dYAxis.z;

	//mm.ZAxis = zAxis;
	d3dMM._31 = d3dZAxis.x;
	d3dMM._32 = d3dZAxis.y;
	d3dMM._33 = d3dZAxis.z;

	//mm.WAxis = turnToPtr->gameObject->GetLocalTransform().WAxis;

	d3dMM._41 =  _pSource->GetMatrix()->_41;
	d3dMM._42 =  _pSource->GetMatrix()->_42;
	d3dMM._43 =  _pSource->GetMatrix()->_43;

	//turnToPtr->gameObject->SetLocalTransform(mm);

	((CEntity*)_pSource)->SetMatrix(&d3dMM);
}

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
D3DXVECTOR3 CPhysics::GetMat4Vec( MATRIX_AXIS _eWhichVec, D3DXMATRIX* _d3dpMat )
{
	switch(_eWhichVec)
	{
	case eXAXIS:
		{
			return D3DXVECTOR3(_d3dpMat->_11, _d3dpMat->_12, _d3dpMat->_13);
		}
		break;
	case eYAXIS:
		{
			return D3DXVECTOR3(_d3dpMat->_21, _d3dpMat->_22, _d3dpMat->_23);
		}
		break;
	case eZAXIS:
		{
			return D3DXVECTOR3(_d3dpMat->_31, _d3dpMat->_32, _d3dpMat->_33);
		}
		break;
	case ePOS:
		{
			return D3DXVECTOR3(_d3dpMat->_41, _d3dpMat->_42, _d3dpMat->_43);
		}
		break;
	default:
		{
			return D3DXVECTOR3(0,0,0);
		}
		break;
	}
}

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
void CPhysics::SetAxis(MATRIX_AXIS _eWhichVec, D3DXMATRIX& _d3dMat, D3DXVECTOR3 _d3dVec)
{
	switch(_eWhichVec)
	{
	case eXAXIS:
		{
			_d3dMat._11 = _d3dVec.x;
			_d3dMat._12 = _d3dVec.y;
			_d3dMat._13 = _d3dVec.z;
		}
		break;
	case eYAXIS:
		{
			_d3dMat._21 = _d3dVec.x;
			_d3dMat._22 = _d3dVec.y;
			_d3dMat._23 = _d3dVec.z;
		}
		break;
	case eZAXIS:
		{
			_d3dMat._31 = _d3dVec.x;
			_d3dMat._32 = _d3dVec.y;
			_d3dMat._33 = _d3dVec.z;
		}
		break;
	case ePOS:
		{
			_d3dMat._41 = _d3dVec.x;
			_d3dMat._42 = _d3dVec.y;
			_d3dMat._43 = _d3dVec.z;
		}
		break;
	default:
		{

		}
		break;
	}

}