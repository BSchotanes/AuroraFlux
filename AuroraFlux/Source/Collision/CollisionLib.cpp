/***********************************************
* Filename:  		CollisionLib.cpp
* Date:      		11/21/2012
* Mod. Date: 		12/12/2012
* Mod. Initials:	AQ
* Author:    		Alexander G. Garcia de Quevedo
* Purpose:   		This is the class that handles all
*					collision detection and responses.
*					Object Manager will call the public
*					function and private helper functions
*					will be delegated to more precise work.
************************************************/

#include "../StdAfx.h"
#include "CollisionLib.h"
#include "Physics.h"
#include "../Entity/Lasers.h"
#include "../Entity/Player.h"
#include "../Entity/YellowEnemy.h"
#include "../Entity/Missile.h"
#include "../Entity/Enemy.h"
#include "../Entity/Asteroid.h"
#include "../Entity/Boss.h"
#include "../Entity/BossHanger.h"
#include "../Entity/BossTurret.h"
#include "../Entity/Generator.h"
#include "../Entity/Waypoint.h"
#include "../Entity/BossBigLazer.h"
#include "../ai/ChaseMovement.h"
#include "../ai/yellowshooting.h"
#include "../Entity/RedEnemy.h"
#include "../Particle System/EffectInformation.h"
#include "../Entity/EnvironmentObjects.h"
#include "../State System/Game.h"

/*****************************************************************
* CollisionLib():	      Default Constructor
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
CCollisionLib::CCollisionLib(void)
{
}

/*****************************************************************
* ~CollisionLib():	      Default Destructor
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
CCollisionLib::~CCollisionLib(void)
{
}

/*****************************************************************
* RayToSphere():		Casts a ray and determines if the ray ever comes 
*						comes into contact with the sphere, and saves the 
*						time at which that would happen
*
* Ins:				_p
*					_d
*					_rhs
*
* Outs:				_t
*					_q
*
* Returns:			bool
*
* Mod. Date:		      11/21/2012
* Mod. Initials:	      AQ
*****************************************************************/
bool CCollisionLib::RayToSphere( const D3DXVECTOR3 &_d3dP, const D3DXVECTOR3 &_d3dD, CSphere& _rhs, float &_fT, D3DXVECTOR3 &_d3dQ )
{

	D3DXVECTOR3 d3dVecBetween = _d3dP - _rhs.GetPosition();
	// how much the vec points in direction of the ray
	float fDirCorrelation = D3DXVec3Dot( &d3dVecBetween, &_d3dD );
	// Squared distance from start of ray to sphere surface
	float fSqrDist = D3DXVec3Dot( &d3dVecBetween, &d3dVecBetween ) - _rhs.GetRadius() * _rhs.GetRadius(); 

	// Ray starts outside and points away
	if( fSqrDist > 0.0f && fDirCorrelation > 0.0f ) 
	{
		return false; 
	}

	float fDiscr = fDirCorrelation*fDirCorrelation - fSqrDist;

	// Negative discriminant means ray missed
	if( fDiscr < 0.0f )
	{
		return false; 
	}

	_fT = -fDirCorrelation - sqrt( fDiscr );

	// Ray starts inside sphere, clamp to 0
	if( _fT < 0.0f ) 
	{
		_fT=0;	
		return false; 
	}

	_d3dQ = _d3dP + (_d3dD * _fT);

	return true;
}

/*****************************************************************
* SphereToSphere():	This function handles moving sphere collision
*
* Ins:				_lhs
*					_rhs
*					_lhfv
*					_rhfv
*
* Outs:			_delta
*
* Returns:			bool
*
* Mod. Date:		      11/21/2012
* Mod. Initials:	      AQ
*****************************************************************/
bool CCollisionLib::MovingSphereToSphere( CSphere& _lhs, CSphere& _rhs, D3DXVECTOR3 _d3dLhfv, D3DXVECTOR3 _d3dRhfv, float& _fDelta )
{
	//if(_lhs.GetRadius()*_lhs.GetRadius() + _rhs.GetRadius()*_rhs.GetRadius() >= D3DXVec3Dot(&(_rhs.GetPosition()-_lhs.GetPosition()), &(_rhs.GetPosition()-_lhs.GetPosition())))
	//return true;

	D3DXVECTOR3 d3dVecBetween = _rhs.GetPosition() - _lhs.GetPosition();
	D3DXVECTOR3 d3dFwdVecBetween = _d3dRhfv - _d3dLhfv;
	float fRadius = _lhs.GetRadius() + _rhs.GetRadius();
	float fDist = D3DXVec3Dot( &d3dVecBetween, &d3dVecBetween ) - fRadius * fRadius;

	// Sphere’s overlap at start
	if( fDist < 0.0f ) 
	{ 
		_fDelta = 0.0f; 
		return true; 
	} 

	float fFwdVecLen = D3DXVec3Dot( &d3dFwdVecBetween, &d3dFwdVecBetween );

	// Sphere’s not moving relative to each other
	if( fFwdVecLen < D3DX_16F_EPSILON ) 
	{
		return false; 
	}

	float fRelationalLen = D3DXVec3Dot( &d3dFwdVecBetween, &d3dVecBetween );

	// Sphere’s not moving toward eachother
	if( fRelationalLen >= 0.0f )
	{
		return false; 
	}

	// Discriminant
	float fDiscr = fRelationalLen*fRelationalLen - fFwdVecLen*fDist; 

	// Sphere’s do not intersect
	if( fDiscr < 0.0f ) 
	{
		return false; 
	}

	_fDelta = (-fRelationalLen - sqrt(fDiscr)) / fFwdVecLen;

	return true;

}

/*****************************************************************
* SphereToCapsule():	This function handles moving sphere / capsule
*						collision by finding the closest point in the
*						capsule and then calling spheretosphere()
*						
*
* Ins:				_lhs
*					_rhs
*					_lhfv
*					_rhfv
*
* Outs:			_delta
*
* Returns:			bool
*
* Mod. Date:		      11/21/2012
* Mod. Initials:	      AQ
*****************************************************************/
bool CCollisionLib::SphereToCapsule( CSphere& _lhs, CCapsule& _rhs, D3DXVECTOR3 _d3dLhfv, D3DXVECTOR3 _d3dRhfv, float& _fDelta )
{
	// center of capsule
	D3DXVECTOR3 d3dCapCenter = _rhs.GetEnd() - _rhs.GetStart();
	// vector from capsule to other sphere
	D3DXVECTOR3 d3dVecBetween = _lhs.GetPosition() - _rhs.GetStart();

	// D = V * C / C * C
	float fDist = (D3DXVec3Dot(&d3dVecBetween, &d3dCapCenter)) / D3DXVec3Dot(&d3dCapCenter, &d3dCapCenter);

	// 0 <= D <= 1
	fDist = min(1, max(0 , fDist));


	CSphere tmpSph;
	tmpSph.SetPosition( _rhs.GetStart() + (d3dCapCenter * fDist));
	tmpSph.SetRadius(_rhs.GetRadius());

	return MovingSphereToSphere(_lhs, tmpSph, _d3dLhfv, _d3dRhfv, _fDelta);
}

/*****************************************************************
* SphereToAABB():		This function handles sphere / AABB
*
* Ins:				_lhs
*					_rhs
*
* Outs:			
*
* Returns:			bool
*
* Mod. Date:		      11/21/2012
* Mod. Initials:	      AQ
*****************************************************************/
bool CCollisionLib::SphereToAABB( CSphere& _lhs, CAABB& _rhs)
{
	D3DXVECTOR3 d3dClosestPoint;

	D3DXVECTOR3 d3dLHS = _lhs.GetPosition();
	D3DXVECTOR3 d3dRhsMax = _rhs.GetMax();
	D3DXVECTOR3 d3dRhsMin = _rhs.GetMin();


	// cap outside values that pass min/max val
	if(d3dLHS.x < d3dRhsMin.x)
		d3dClosestPoint.x = d3dRhsMin.x;
	else if(d3dLHS.x > d3dRhsMax.x)
		d3dClosestPoint.x = d3dRhsMax.x;
	else
		d3dClosestPoint.x = d3dLHS.x;

	if(d3dLHS.y < d3dRhsMin.y)
		d3dClosestPoint.y = d3dRhsMin.y;
	else if(d3dLHS.y > d3dRhsMax.y)
		d3dClosestPoint.y = d3dRhsMax.y;
	else
		d3dClosestPoint.y = d3dLHS.y;

	if(d3dLHS.z < d3dRhsMin.z)
		d3dClosestPoint.z = d3dRhsMin.z;
	else if(d3dLHS.z > d3dRhsMax.z)
		d3dClosestPoint.z = d3dRhsMax.z;
	else
		d3dClosestPoint.z = d3dLHS.z;


	// squared length of vector between sphere and aabb
	float fLenSquared = D3DXVec3Dot(&(d3dLHS - d3dClosestPoint), &(d3dLHS - d3dClosestPoint));

	//if sq len is less than the radius squared, we have collision
	if(fLenSquared <= _lhs.GetRadius() * _lhs.GetRadius() )
	{
		return true;
	}


	return false;
}

/*****************************************************************
* CapsuleToAABB():		This function handles capsule / AABB collsiion
*						by finding the closest point on the capsule
*						and then calls SphereToAABB
*
* Ins:				_lhs
*					_rhs
*
* Outs:			
*
* Returns:			bool
*
* Mod. Date:		      11/21/2012
* Mod. Initials:	      AQ
*****************************************************************/
bool CCollisionLib::CapsuleToAABB(  CCapsule& _lhs,  CAABB& _rhs )
{
	// center of capsule
	D3DXVECTOR3 d3dCenter = _lhs.GetEnd() - _lhs.GetStart();

	// vector from capsue to center of aabb
	D3DXVECTOR3 d3dVecBetween = (_rhs.GetMax() + _rhs.GetMin())*0.5f - _lhs.GetStart();
	float fDist = (D3DXVec3Dot(&d3dVecBetween, &d3dCenter)) / D3DXVec3Dot(&d3dCenter, &d3dCenter);

	fDist = min(1, max(0 , fDist));
	CSphere tmpSph;
	tmpSph.SetPosition( _lhs.GetStart() + (d3dCenter * fDist));
	tmpSph.SetRadius(_lhs.GetRadius());

	return SphereToAABB(tmpSph, _rhs);
}

/*****************************************************************
* CapsuleToCapsule():	This function handles moving capsule
*						collision by finding the closest point in both
*						capsules and then calling spheretosphere()
*						
*
* Ins:				_lhs
*					_rhs
*					_lhfv
*					_rhfv
*
* Outs:			_delta
*
* Returns:			bool
*
* Mod. Date:		      11/21/2012
* Mod. Initials:	      AQ
*****************************************************************/
bool CCollisionLib::CapsuleToCapsule( CCapsule& _lhs, CCapsule& _rhs, D3DXVECTOR3 _d3dLhfv, D3DXVECTOR3 _d3dRhfv, float& _fDelta )
{
	//center of capsule 2
	D3DXVECTOR3 d3dCenter = _rhs.GetEnd() - _rhs.GetStart();
	// vector from start of capsule 2 to center of capsule 1
	D3DXVECTOR3 d3dVecBetween = (_lhs.GetEnd() + _lhs.GetStart())*0.5f - _rhs.GetStart();
	float fDist = (D3DXVec3Dot(&d3dVecBetween, &d3dCenter)) / D3DXVec3Dot(&d3dCenter, &d3dCenter);

	fDist = min(1, max(0 , fDist));
	CSphere tmpSph;
	tmpSph.SetPosition( _rhs.GetStart() + (d3dCenter * fDist));
	tmpSph.SetRadius(_rhs.GetRadius());

	//center of capsule 2
	d3dCenter = _lhs.GetEnd() - _lhs.GetStart();
	// vector from start of capsule 2 to center of capsule 1
	d3dVecBetween = tmpSph.GetPosition() - _lhs.GetStart();
	fDist = (D3DXVec3Dot(&d3dVecBetween, &d3dCenter)) / D3DXVec3Dot(&d3dCenter, &d3dCenter);

	fDist = min(1, max(0 , fDist));
	CSphere tmpSph2;
	tmpSph2.SetPosition( _lhs.GetStart() + (d3dCenter * fDist));
	tmpSph2.SetRadius(_lhs.GetRadius());

	return MovingSphereToSphere(tmpSph2, tmpSph, _d3dLhfv, _d3dRhfv, _fDelta);
}

/*****************************************************************
* CheckCollision():	This is the function the object manager calls
*						for every object in the active list.  
*
* Ins:				_lhs
*					_rhs
*
* Outs:		     
*
* Returns:		      
*
* Mod. Date:		      11/21/2012
* Mod. Initials:	      AQ
*****************************************************************/
void CCollisionLib::CheckCollision( CEntity* _pLhs, CEntity* _pRhs, CAssetManager* _pAssets , CGame* _pGame)
{
	if( _pLhs->GetBV() == NULL )
		return;
	if( _pRhs->GetBV() == NULL )
		return;

	CollObjType eLHS = _pLhs->GetBV()->GetObjType();
	CollObjType eRHS = _pRhs->GetBV()->GetObjType();

// 	if(_pLhs->GetType() == eBARRIER)
// 	{
// 		float fDeltaT=0;
// 		D3DXVECTOR3 tmpVec;
// 
// 		if(eRHS == eSPH)
// 		{
// 			CSphere rhsSph = *((CSphere*)_pRhs->GetBV());
// 			rhsSph.SetPosition(rhsSph.GetPosition() + _pRhs->GetPosition());
// 			if(RayToSphere(_pLhs->GetPosition(), CPhysics::GetMat4Vec(eZAXIS, ((CBossBigLazer*)_pLhs)->GetOwner()->GetMatrix()), rhsSph, fDeltaT, tmpVec))
// 			{
// 				Reactions(_pLhs, _pRhs, fDeltaT, _pAssets);
// 			}
// 		}
// 		else if(eRHS == eCAP)
// 		{
// 			CCapsule rhsSph = *((CCapsule*)_pRhs->GetBV());
// 			rhsSph.SetStart(rhsSph.GetStart() + _pRhs->GetPosition());
// 			rhsSph.SetEnd(rhsSph.GetEnd() + _pRhs->GetPosition());
// 			if(IntersectRayCapsule(_pLhs->GetPosition(), CPhysics::GetMat4Vec(eZAXIS, ((CBossBigLazer*)_pLhs)->GetOwner()->GetMatrix()), &rhsSph, fDeltaT, tmpVec))
// 			{
// 				Reactions(_pLhs, _pRhs, fDeltaT, _pAssets);
// 			}
// 		}
// 		
// 		
// 
// 		return;
// 	}
// 	if(_pRhs->GetType() == eBARRIER)
// 	{
// 		float fDeltaT=0;
// 		D3DXVECTOR3 tmpVec;
// 
// 		if(eLHS == eSPH)
// 		{
// 			CSphere rhsSph = *((CSphere*)_pLhs->GetBV());
// 			rhsSph.SetPosition(rhsSph.GetPosition() + _pLhs->GetPosition());
// 
// 			if(RayToSphere(_pRhs->GetPosition(), CPhysics::GetMat4Vec(eZAXIS, ((CBossBigLazer*)_pRhs)->GetOwner()->GetMatrix()), rhsSph, fDeltaT, tmpVec))
// 			{
// 				Reactions(_pLhs, _pRhs, fDeltaT, _pAssets);
// 			}
// 		}
// 		else if(eLHS == eCAP)
// 		{
// 			CCapsule rhsSph = *((CCapsule*)_pRhs->GetBV());
// 			rhsSph.SetStart(rhsSph.GetStart() + _pLhs->GetPosition());
// 			rhsSph.SetEnd(rhsSph.GetEnd() + _pLhs->GetPosition());
// 
// 			if(IntersectRayCapsule(_pRhs->GetPosition(), CPhysics::GetMat4Vec(eZAXIS, ((CBossBigLazer*)_pRhs)->GetOwner()->GetMatrix()), &rhsSph, fDeltaT, tmpVec))
// 			{
// 				Reactions(_pLhs, _pRhs, fDeltaT, _pAssets);
// 			}
// 		}
// 
// 
// 		return;
// 	}

	if( eLHS == eSPH)
	{
		CSphere tmpSphere = *((CSphere*)_pLhs->GetBV());
		tmpSphere.SetPosition(tmpSphere.GetPosition() + _pLhs->GetPosition());

		if(eRHS == eSPH)
		{
			float fDeltaT=0;
			CSphere rhsSph = *((CSphere*)_pRhs->GetBV());
			rhsSph.SetPosition(rhsSph.GetPosition() + _pRhs->GetPosition());

			if(MovingSphereToSphere(tmpSphere, rhsSph, CPhysics::GetMat4Vec(eZAXIS, _pLhs->GetMatrix()), CPhysics::GetMat4Vec(eZAXIS, _pRhs->GetMatrix()), fDeltaT))
			{
				Reactions(_pLhs, _pRhs, fDeltaT, _pAssets, _pGame);
			}
		}
		else if(eRHS == eAABB)
		{
			CAABB tmpAABB;
			tmpAABB.SetMax(((CAABB*)_pRhs->GetBV())->GetMax() + _pRhs->GetPosition());
			tmpAABB.SetMin(((CAABB*)_pRhs->GetBV())->GetMin() + _pRhs->GetPosition());
			if(SphereToAABB(tmpSphere, tmpAABB))
			{
				Reactions(_pLhs, _pRhs, 0, _pAssets, _pGame);
			}
		}
		else if(eRHS == eCAP)
		{
			float fDeltaT=0;
			CCapsule rhsSph = *((CCapsule*)_pRhs->GetBV());
			D3DXVECTOR3 d3dZaxis = CPhysics::GetMat4Vec(eZAXIS, _pRhs->GetMatrix()),
			 d3dYaxis = CPhysics::GetMat4Vec(eYAXIS, _pRhs->GetMatrix()),
			 d3dXaxis = CPhysics::GetMat4Vec(eXAXIS, _pRhs->GetMatrix());
			D3DXVec3Normalize(&d3dZaxis, &d3dZaxis);
			D3DXVec3Normalize(&d3dYaxis, &d3dYaxis);
			D3DXVec3Normalize(&d3dXaxis, &d3dXaxis);



			rhsSph.SetStart((rhsSph.GetStart().x * d3dXaxis) + (rhsSph.GetStart().y * d3dYaxis) + (rhsSph.GetStart().z * d3dZaxis) + _pRhs->GetPosition());
			rhsSph.SetEnd((rhsSph.GetEnd().x * d3dXaxis) + (rhsSph.GetEnd().y * d3dYaxis) + (rhsSph.GetEnd().z * d3dZaxis) + _pRhs->GetPosition());

			if( SphereToCapsule(tmpSphere, rhsSph, CPhysics::GetMat4Vec(eZAXIS, _pLhs->GetMatrix()), CPhysics::GetMat4Vec(eZAXIS, _pRhs->GetMatrix()), fDeltaT))
			{
				Reactions(_pLhs, _pRhs, fDeltaT, _pAssets, _pGame);
			}
		}
	}
	else if(eLHS == eAABB)
	{
		CAABB tmpAABB = *((CAABB*)_pLhs->GetBV());
		tmpAABB.SetMin(tmpAABB.GetMin() + _pLhs->GetPosition());
		tmpAABB.SetMax(tmpAABB.GetMax() + _pLhs->GetPosition());

		if(eRHS == eSPH)
		{
			CSphere rhsSph = *((CSphere*)_pRhs->GetBV());
			rhsSph.SetPosition(rhsSph.GetPosition() + _pRhs->GetPosition());

			if( SphereToAABB(rhsSph, tmpAABB))
			{
				Reactions(_pLhs, _pRhs, 0, _pAssets, _pGame);
			}
		}
		else if(eRHS == eCAP)
		{
			CCapsule rhsSph = *((CCapsule*)_pRhs->GetBV());
			D3DXVECTOR3 d3dZaxis = CPhysics::GetMat4Vec(eZAXIS, _pRhs->GetMatrix()),
				d3dYaxis = CPhysics::GetMat4Vec(eYAXIS, _pRhs->GetMatrix()),
				d3dXaxis = CPhysics::GetMat4Vec(eXAXIS, _pRhs->GetMatrix());
			D3DXVec3Normalize(&d3dZaxis, &d3dZaxis);
			D3DXVec3Normalize(&d3dYaxis, &d3dYaxis);
			D3DXVec3Normalize(&d3dXaxis, &d3dXaxis);
			rhsSph.SetStart((rhsSph.GetStart().x * d3dXaxis) + (rhsSph.GetStart().y * d3dYaxis) + (rhsSph.GetStart().z * d3dZaxis) + _pRhs->GetPosition());
			rhsSph.SetEnd((rhsSph.GetEnd().x * d3dXaxis) + (rhsSph.GetEnd().y * d3dYaxis) + (rhsSph.GetEnd().z * d3dZaxis) + _pRhs->GetPosition());

			if( !CapsuleToAABB(rhsSph, tmpAABB))
			{
				Reactions(_pLhs, _pRhs, 0, _pAssets, _pGame);
			}
		}
	}
	else if(eLHS == eCAP)
	{
		CCapsule tmpCapsule = *((CCapsule*)_pLhs->GetBV());
		D3DXVECTOR3 d3dZaxis = CPhysics::GetMat4Vec(eZAXIS, _pLhs->GetMatrix()),
			d3dYaxis = CPhysics::GetMat4Vec(eYAXIS, _pLhs->GetMatrix()),
			d3dXaxis = CPhysics::GetMat4Vec(eXAXIS, _pLhs->GetMatrix());
		D3DXVec3Normalize(&d3dZaxis, &d3dZaxis);
		D3DXVec3Normalize(&d3dYaxis, &d3dYaxis);
		D3DXVec3Normalize(&d3dXaxis, &d3dXaxis);
		tmpCapsule.SetStart((tmpCapsule.GetStart().x * d3dXaxis) + (tmpCapsule.GetStart().y * d3dYaxis) + (tmpCapsule.GetStart().z * d3dZaxis) + _pLhs->GetPosition());
		tmpCapsule.SetEnd((tmpCapsule.GetEnd().x * d3dXaxis) + (tmpCapsule.GetEnd().y * d3dYaxis) + (tmpCapsule.GetEnd().z * d3dZaxis) + _pLhs->GetPosition());

		if(eRHS == eSPH)
		{
			float fDeltaT=0;
			CSphere rhsSph = *((CSphere*)_pRhs->GetBV());
			rhsSph.SetPosition(rhsSph.GetPosition() + _pRhs->GetPosition());

			if( SphereToCapsule(rhsSph, tmpCapsule, CPhysics::GetMat4Vec(eZAXIS, _pRhs->GetMatrix()), CPhysics::GetMat4Vec(eZAXIS, _pLhs->GetMatrix()), fDeltaT))
			{
				Reactions(_pLhs, _pRhs, fDeltaT, _pAssets, _pGame);
			}
		}
		else if(eRHS == eAABB)
		{
			CAABB tmpAABB;
			tmpAABB.SetMax(((CAABB*)_pRhs->GetBV())->GetMax() + _pRhs->GetPosition());
			tmpAABB.SetMin(((CAABB*)_pRhs->GetBV())->GetMin() + _pRhs->GetPosition());

			if( !CapsuleToAABB(*((CCapsule*)_pLhs->GetBV()), *((CAABB*)_pRhs->GetBV())))
			{
				Reactions(_pLhs, _pRhs, 0, _pAssets, _pGame);
			}
		}
		else if(eRHS == eCAP)
		{
			float fDeltaT=0;
			CCapsule rhsCap = *((CCapsule*)_pRhs->GetBV());
			D3DXVECTOR3 d3dZaxis2 = CPhysics::GetMat4Vec(eZAXIS, _pRhs->GetMatrix()),
				d3dYaxis2 = CPhysics::GetMat4Vec(eYAXIS, _pRhs->GetMatrix()),
				d3dXaxis2 = CPhysics::GetMat4Vec(eXAXIS, _pRhs->GetMatrix());
			D3DXVec3Normalize(&d3dZaxis2, &d3dZaxis2);
			D3DXVec3Normalize(&d3dYaxis2, &d3dYaxis2);
			D3DXVec3Normalize(&d3dXaxis2, &d3dXaxis2);
			rhsCap.SetStart((rhsCap.GetStart().x * d3dXaxis2) + (rhsCap.GetStart().y * d3dYaxis2) + (rhsCap.GetStart().z * d3dZaxis2) + _pRhs->GetPosition());
			rhsCap.SetEnd((rhsCap.GetEnd().x * d3dXaxis2) + (rhsCap.GetEnd().y * d3dYaxis2) + (rhsCap.GetEnd().z * d3dZaxis2) + _pRhs->GetPosition());

			if( CapsuleToCapsule(rhsCap, tmpCapsule, CPhysics::GetMat4Vec(eZAXIS, _pRhs->GetMatrix()), CPhysics::GetMat4Vec(eZAXIS, _pLhs->GetMatrix()), fDeltaT))
			{
				Reactions(_pLhs, _pRhs, fDeltaT, _pAssets, _pGame);
			}

		}
	}

}

/*****************************************************************
* Reactions():		This function handles all reactions between two
*					objects.  
*
* Ins:				_lhs
*					_rhs
*					_delta
*
* Outs:			
*
* Returns:			bool
*
* Mod. Date:		      11/21/2012
* Mod. Initials:	      AQ
*****************************************************************/
void CCollisionLib::Reactions( CEntity* _pLhs, CEntity* _pRhs, float _fDelta, CAssetManager* _pAssets, CGame* _pGame)
{ 

	int nLHS = _pLhs->GetType();
	int nRHS = _pRhs->GetType();

	if(nLHS == eBOSS)
	{
		if(((CBoss*)_pLhs)->GetPhase() == eCINEMATIC_PHASE)
		{
			if(nRHS != eRED_PLAYER && nRHS != eBLUE_PLAYER && _fDelta < EPSILON*22)
			{
				if(nRHS > 12 && nRHS < 18)
					return;

				((CBoss*)_pLhs)->AddEffectFromCollision(_pRhs);
				_pRhs->SetActive(false);
				return;
			}
		}
	}

	if(nRHS == eBOSS)
	{
		if(((CBoss*)_pRhs)->GetPhase() == eCINEMATIC_PHASE)
		{
			if(nLHS != eRED_PLAYER && nLHS != eBLUE_PLAYER && _fDelta < EPSILON*23)
			{
				if(nLHS > 12 && nLHS < 18)
					return;

				((CBoss*)_pRhs)->AddEffectFromCollision(_pLhs);
				_pLhs->SetActive(false);
				return;
			}
		}
	}


	// TODO: Alex - implement steering for enemies
// 	if( nLHS == eRED_ENEMY && nRHS == eRED_ENEMY)
// 	{
// 		// TODO: ROSSER - AI FLOCKING THING
// 
// 		// Time data
// 		if(_fDelta >= EPSILON*2)
// 		{
// 			return;
// 		}
// 
// 		CRedEnemy * redEnenmy =((CRedEnemy *)_pLhs);
// 		CRedEnemy * otherRedEnenmy =((CRedEnemy *)_pRhs);
// 
// 		if (!redEnenmy->isPartner(otherRedEnenmy))
// 		{
// 			redEnenmy->AddPartner(otherRedEnenmy);
// 		}
// 
// 	}
	// 	if (_pLhs->GetIsScenery() || _pRhs->GetIsScenery())
	// 	{
	// 		return;
	// 	}


	// LHS == PLAYER
	if(nLHS == eYELLOW_PLAYER || nLHS == eBLUE_PLAYER || nLHS == eRED_PLAYER)
	{
		switch(nRHS) 
		{
		case eWAYPOINT:
			{
				if(_fDelta >= EPSILON*2)
				{
					return;
				}

				CWaypoint* pTmpWaypoint = (CWaypoint*)_pRhs;
				if(pTmpWaypoint->IsTriggered())
				{
					break;
				}
				pTmpWaypoint->GetWaypointID();

				pTmpWaypoint->SetTrigger(true);
			}
			break;
		case eBOSSTHRUSTER:
			{
				static float _fThrusterDotTimer = 0.0f;
				if(_fThrusterDotTimer > 0.1f)
				{

					CPlayer* tmpPlayer = (CPlayer*)_pLhs;
					int nDifference = 2 - tmpPlayer->GetShields();
					if(tmpPlayer->GetShields())
					{
						if(nDifference >= 0)
						{
							tmpPlayer->SetShields(0);
						}
						else
						{
							tmpPlayer->SetShields(max(tmpPlayer->GetShields() - 1, 0), 5, _pGame);
						}
					}
					if(tmpPlayer->GetHealth() && tmpPlayer->GetShields() <= 0)
					{
						if(nDifference >= 0) 
						{
							tmpPlayer->SetHealth(tmpPlayer->GetHealth() - (int)(nDifference * 0.5f), 5, _pGame);
						}
						else
						{
							tmpPlayer->SetHealth(max(tmpPlayer->GetHealth() - (int)(nDifference*0.5f), 0), 5, _pGame);				
						}
						tmpPlayer->SetPlayHullHit(true);
					}

					_fThrusterDotTimer = 0.0f;
				}
				else
				{
					_fThrusterDotTimer += 0.05f;
				}
			}
			break;
		case eBARRIER:
			{
				static float _fThrusterDotTimer = 0.0f;
				if(_fThrusterDotTimer > 0.05f)
				{
					CPlayer* tmpPlayer = (CPlayer*)_pLhs;
					CBossBigLazer* tmpLazer = (CBossBigLazer*)_pRhs;
					int nDifference = 2 - tmpPlayer->GetShields();
					if(tmpPlayer->GetShields())
					{
						if(nDifference >= 0)
						{
							tmpPlayer->SetShields(0);
						}
						else
						{
							tmpPlayer->SetShields(max(tmpPlayer->GetShields() - 1, 0), tmpPlayer->CalculateDamageDirection(tmpLazer->GetOwner()->GetPosition()) );
						}
					}
					if(tmpPlayer->GetHealth() && tmpPlayer->GetShields() <= 0)
					{
						if(nDifference >= 0) 
						{
							tmpPlayer->SetHealth(tmpPlayer->GetHealth() - (int)(nDifference * 0.5f), tmpPlayer->CalculateDamageDirection(tmpLazer->GetOwner()->GetPosition()), _pGame);
						}
						else
						{
							tmpPlayer->SetHealth(max(tmpPlayer->GetHealth() - (int)(nDifference*0.5f), 0), tmpPlayer->CalculateDamageDirection(tmpLazer->GetOwner()->GetPosition()), _pGame);				
						}
						tmpPlayer->SetPlayHullHit(true);
					}
				}
			}
			break;

		case eMISSILE:
			{
				if(_fDelta >= EPSILON*3)
				{
					return;
				}

				int nDifference = -1;

				CPlayer* tmpPlayer = (CPlayer*)_pLhs;
				CMissile* tmpMissile = (CMissile*)_pRhs;

				if(tmpMissile->GetOwner() == tmpPlayer || !tmpMissile->GetActive())
				{
					break;
				}

				D3DXVECTOR3 d3dTmpVec = tmpMissile->GetPosition() - tmpPlayer->GetPosition();
				D3DXVec3Normalize(&d3dTmpVec, &d3dTmpVec);

				//tmpPlayer->SetVelocity(tmpPlayer->GetVelocity() - d3dTmpVec*tmpPlayer->GetVelocityModifier()*0.2f);


				if(!tmpPlayer->GetShieldHit())
				{
					tmpPlayer->SetShieldHit(true);
					tmpPlayer->SetOtherObjectPos(tmpMissile->GetPosition() - tmpPlayer->GetPosition());

					//TODO SW pass in correct effect type
					tmpMissile->CreateEffect(eMISSILE_SHIELD);

				}

				if(tmpPlayer->GetShields())
				{
					nDifference = tmpMissile->GetHelper()->GetmissileDamage() - tmpPlayer->GetShields();
					if(nDifference >= 0)
					{
						tmpPlayer->SetShields(0);
					}
					else
					{
						tmpPlayer->SetShields(max(tmpPlayer->GetShields() - tmpMissile->GetHelper()->GetmissileDamage(), 0), tmpPlayer->CalculateDamageDirection(tmpMissile->GetOwner()->GetPosition()) );
					}

					tmpPlayer->SetPlayShieldHit(true);
					if(tmpPlayer->GetModelTriCollPoint() == -1)
					{
						SphereToMesh(_pLhs, _pRhs, _fDelta, _pAssets);
					}
				}

				if(tmpPlayer->GetHealth() && tmpPlayer->GetShields() <= 0)
				{
					if(nDifference >= 0)
					{
						tmpPlayer->SetHealth(tmpPlayer->GetHealth() - nDifference/2, tmpPlayer->CalculateDamageDirection(tmpMissile->GetOwner()->GetPosition()), _pGame);
					}
					else
					{
						tmpPlayer->SetHealth(max(tmpPlayer->GetHealth() - (tmpMissile->GetHelper()->GetmissileDamage() / 2), 0), tmpPlayer->CalculateDamageDirection(tmpMissile->GetOwner()->GetPosition()), _pGame);				
					}

					//TODO: Pass in correct effect
					tmpMissile->CreateEffect(eMISSILE_HULL);

					tmpPlayer->SetPlayHullHit(true);
				}

				tmpMissile->SetHealth(0);
				tmpMissile->SetIsScenery(true);
				//WwiseNS::SoundEvent(eMissileTravel, eStop);
				tmpPlayer->SetIsHit(true);
			}
			break;

		case eLASER:
			{
				if(_fDelta >= EPSILON*2000)
				{
					return;
				}

				int nDifference = -1;

				CPlayer* tmpPlayer = (CPlayer*)_pLhs;
				CLaser* tmpLaser = (CLaser*)_pRhs;

				if(tmpLaser->GetOwner() == tmpPlayer || !tmpLaser->GetActive())
				{
					break;
				}

				if(!tmpPlayer->GetShieldHit())
				{
					tmpPlayer->SetShieldHit(true);
					tmpPlayer->SetOtherObjectPos(tmpLaser->GetPosition() - tmpPlayer->GetPosition());

					//TODO SW pass in correct effect type
					tmpLaser->CreateEffect(eLASER_SHIELD);

				}

				int nLaserDamage = 0;
				int nType = tmpLaser->GetOwner()->GetType();
				if ( nType == eRED_ENEMY || nType == eBLUE_ENEMY ||  nType == eTURRET_LASER)
				{
					nLaserDamage = tmpLaser->GetHelper()->GetlaserDamage();
				}
				else
				{
					nLaserDamage = tmpLaser->GetHelper()->GetlaserDamage() / 2;
				}
				if(tmpPlayer->GetShields() > 0)
				{
					nDifference = nLaserDamage - tmpPlayer->GetShields(); 
					if(nDifference >= 0)
					{
						tmpPlayer->SetShields(0);
					}
					else
					{
						tmpPlayer->SetShields(max(tmpPlayer->GetShields() - nLaserDamage, 0), tmpPlayer->CalculateDamageDirection(tmpLaser->GetOwner()->GetPosition()) );
					}

					//TODO SW pass in correct effect type
					tmpLaser->CreateEffect(eLASER_SHIELD);

					tmpPlayer->SetPlayShieldHit(true);
					if(tmpPlayer->GetModelTriCollPoint() == -1)
					{
						SphereToMesh(_pLhs, _pRhs, _fDelta, _pAssets);
					}
				}

				if(tmpPlayer->GetHealth() && tmpPlayer->GetShields() <= 0)
				{
					if(nDifference >= 0)
					{
						tmpPlayer->SetHealth(tmpPlayer->GetHealth() - nDifference, tmpPlayer->CalculateDamageDirection(tmpLaser->GetOwner()->GetPosition()), _pGame);
					}
					else
					{
						tmpPlayer->SetHealth(max(tmpPlayer->GetHealth() - nLaserDamage, 0), tmpPlayer->CalculateDamageDirection(tmpLaser->GetOwner()->GetPosition()), _pGame);
					}
					tmpPlayer->SetPlayHullHit(true);
					tmpLaser->CreateEffect(eLASER_HULL);
				}

				tmpLaser->SetActive(false);
				tmpPlayer->SetIsHit(true);

			}
			break;

		case eASTEROID:
			{
				// vector between player and asteroid position
				// dot product between new vec and player forward
				// scale by abs(dotproduct)

				if(_fDelta >= EPSILON*3)
				{
					return;
				}
				int nDifference = -1;


				CPlayer* tmpPlayer = (CPlayer*)_pLhs;
				CAsteroid* tmpAsteroid = (CAsteroid*)_pRhs;

				D3DXVECTOR3 d3dTmpVec = tmpAsteroid->GetPosition() - tmpPlayer->GetPosition();
				D3DXVec3Normalize(&d3dTmpVec, &d3dTmpVec);
				//	D3DXMatrixTranslation(tmpPlayer->GetMatrix(), -d3dTmpVec.x, -d3dTmpVec.y, -d3dTmpVec.y);
				float fScale = abs(D3DXVec3Dot(&d3dTmpVec, &CPhysics::GetMat4Vec(eZAXIS, tmpPlayer->GetMatrix())));

				D3DXMATRIX d3dtmpMat;
				float fRadius = ((CSphere*)tmpPlayer->GetBV())->GetRadius();
				D3DXMatrixTranslation(&d3dtmpMat, -d3dTmpVec.x * fRadius, -d3dTmpVec.y * fRadius, -d3dTmpVec.z * fRadius);

				tmpPlayer->SetVelocity(tmpPlayer->GetVelocity() - d3dTmpVec*tmpPlayer->GetVelocityModifier());
				tmpAsteroid->SetVelocity(tmpAsteroid->GetVelocity() + d3dTmpVec*tmpPlayer->GetVelocityModifier()*0.5f);

				if(fScale > 0.8)
				{
					//tmpPlayer->SetAlive(false);
					tmpAsteroid->SetHealth(0);
				}
				//else
				{


					// 					d3dtmpMat = *tmpPlayer->GetMatrix() * d3dtmpMat;
					// 
					// 					tmpPlayer->SetMatrix(&d3dtmpMat);
					// 
					// 					tmpPlayer->GetCamera()->SetViewMatrixPos(&(CPhysics::GetMat4Vec(ePOS, tmpPlayer->GetCamera()->GetViewMatrix()) + (-d3dTmpVec * fRadius)));

					if(!tmpPlayer->GetShieldHit())
					{
						tmpPlayer->SetShieldHit(true);
						tmpPlayer->SetOtherObjectPos(tmpAsteroid->GetPosition() - tmpPlayer->GetPosition());
					}

					//		if(abs(scale) > 0.7)
					//		{
					//			tmpPlayer->SetActive(false);
					//			tmpAsteroid->SetActive(false);
					//		}
					//	else

					tmpAsteroid->SetHealth(int(max(tmpAsteroid->GetHealth() - 10*fScale,0)));
					
					if(tmpPlayer->GetShields() > 0)
					{
						nDifference = int((20 * fScale) - tmpPlayer->GetShields());
						if(nDifference >= 0)
						{
							tmpPlayer->SetShields(0);
						}
						else
						{
							tmpPlayer->SetShields(max(tmpPlayer->GetShields() - (int)(20 * fScale), 0), tmpPlayer->CalculateDamageDirection(tmpAsteroid->GetPosition()) );
						}

						tmpPlayer->SetPlayShieldHit(true);
					}

					if(tmpPlayer->GetShields() <= 0 && tmpPlayer->GetHealth())
					{
						if(nDifference >= 0)
						{
							tmpPlayer->SetHealth(tmpPlayer->GetHealth() - nDifference, tmpPlayer->CalculateDamageDirection(tmpAsteroid->GetPosition()), _pGame);
						}
						else
						{
							tmpPlayer->SetHealth(int(max(tmpPlayer->GetHealth() - 20*fScale, 0)), tmpPlayer->CalculateDamageDirection(tmpAsteroid->GetPosition()), _pGame);
						}

						tmpPlayer->SetPlayHullHit(true);
					}
					tmpPlayer->SetIsHit(true);
				}

				//D3DXMatrixTranslation(tmpPlayer->GetMatrix(), -tmpVec.x, -tmpVec.y, -tmpVec.y);


			}
			break;

		case eLARGE_ASTEROID:
			{
				// vector between player and asteroid position
				// dot product between new vec and player forward
				// scale by abs(dotproduct)

				if(_fDelta >= EPSILON*3)
				{
					return;
				}
				int nDifference = -1;

				CPlayer* tmpPlayer = (CPlayer*)_pLhs;
				CAsteroid* tmpAsteroid = (CAsteroid*)_pRhs;

				D3DXVECTOR3 d3dTmpVec = tmpAsteroid->GetPosition() - tmpPlayer->GetPosition();
				D3DXVec3Normalize(&d3dTmpVec, &d3dTmpVec);
				//	D3DXMatrixTranslation(tmpPlayer->GetMatrix(), -d3dTmpVec.x, -d3dTmpVec.y, -d3dTmpVec.y);
				float fScale = abs(D3DXVec3Dot(&d3dTmpVec, &CPhysics::GetMat4Vec(eZAXIS, tmpPlayer->GetMatrix())));


				//D3DXMatrixTranslation(tmpPlayer->GetMatrix(), -tmpVec.x, -tmpVec.y, -tmpVec.y);


				if(fScale > 0.65)
				{
					//tmpPlayer->SetAlive(false);
					//tmpPlayer->SetHealth(0);
					//tmpPlayer->SetShields(0);

				}
				else
				{

					tmpPlayer->SetVelocity(tmpPlayer->GetVelocity() - d3dTmpVec*tmpPlayer->GetVelocityModifier());
					tmpAsteroid->SetVelocity(tmpAsteroid->GetVelocity() - d3dTmpVec*tmpPlayer->GetVelocityModifier()*0.5f);
					// 					d3dtmpMat = *tmpPlayer->GetMatrix() * d3dtmpMat;
					// 
					// 					tmpPlayer->SetMatrix(&d3dtmpMat);
					// 
					// 					tmpPlayer->GetCamera()->SetViewMatrixPos(&(CPhysics::GetMat4Vec(ePOS, tmpPlayer->GetCamera()->GetViewMatrix()) + (-d3dTmpVec * fRadius)));

					if(!tmpPlayer->GetShieldHit())
					{
						tmpPlayer->SetShieldHit(true);
						tmpPlayer->SetOtherObjectPos(tmpAsteroid->GetPosition() - tmpPlayer->GetPosition());
					}

					//		if(abs(scale) > 0.7)
					//		{
					//			tmpPlayer->SetActive(false);
					//			tmpAsteroid->SetActive(false);
					//		}
					//	else


					if(tmpPlayer->GetShields() > 0)
					{
						nDifference = int((20 * fScale) - tmpPlayer->GetShields());
						if(nDifference >= 0)
						{
							tmpPlayer->SetShields(0);
						}
						else
						{
							tmpPlayer->SetShields(max(tmpPlayer->GetShields() - (int)(20 * fScale), 0), tmpPlayer->CalculateDamageDirection(tmpAsteroid->GetPosition()) );
						}

						tmpPlayer->SetPlayShieldHit(true);
					}

					if(tmpPlayer->GetShields() <= 0 && tmpPlayer->GetHealth())
					{
						if(nDifference >= 0)
						{
							tmpPlayer->SetHealth(tmpPlayer->GetHealth() - nDifference, tmpPlayer->CalculateDamageDirection(tmpAsteroid->GetPosition()), _pGame);
						}
						else
						{
							tmpPlayer->SetHealth(int(max(tmpPlayer->GetHealth() - 20*fScale, 0)), tmpPlayer->CalculateDamageDirection(tmpAsteroid->GetPosition()), _pGame);
						}

						tmpPlayer->SetPlayHullHit(true);
					}
					tmpPlayer->SetIsHit(true);
				}

				//D3DXMatrixTranslation(tmpPlayer->GetMatrix(), -tmpVec.x, -tmpVec.y, -tmpVec.y);


			}
			break;
		case eSPACE_DEBRIS:
		case eSPACE_DEBRIS2:
		case eSPACE_DEBRIS3:
			{
				if(_fDelta >= EPSILON*3)
				{
					return;
				}
				int nDifference = -1;

				CPlayer* tmpPlayer = (CPlayer*)_pLhs;
				CEnvironmentObject* tmpAsteroid = (CEnvironmentObject*)_pRhs;

				D3DXVECTOR3 d3dTmpVec = tmpAsteroid->GetPosition() - tmpPlayer->GetPosition();
				D3DXVec3Normalize(&d3dTmpVec, &d3dTmpVec);
				//	D3DXMatrixTranslation(tmpPlayer->GetMatrix(), -d3dTmpVec.x, -d3dTmpVec.y, -d3dTmpVec.y);
				float fScale = abs(D3DXVec3Dot(&d3dTmpVec, &CPhysics::GetMat4Vec(eZAXIS, tmpPlayer->GetMatrix())));

				D3DXMATRIX d3dtmpMat;
				float fRadius = ((CSphere*)tmpPlayer->GetBV())->GetRadius();
				D3DXMatrixTranslation(&d3dtmpMat, -d3dTmpVec.x * fRadius, -d3dTmpVec.y * fRadius, -d3dTmpVec.z * fRadius);

				tmpPlayer->SetVelocity(tmpPlayer->GetVelocity() - d3dTmpVec*tmpPlayer->GetVelocityModifier());
				tmpAsteroid->SetVelocity(tmpAsteroid->GetVelocity() + d3dTmpVec*tmpPlayer->GetVelocityModifier()*0.5f);

				if(fScale > 0.8)
				{
					//tmpPlayer->SetAlive(false);
					tmpAsteroid->SetHealth(0);
				}
				//else
				{


					// 					d3dtmpMat = *tmpPlayer->GetMatrix() * d3dtmpMat;
					// 
					// 					tmpPlayer->SetMatrix(&d3dtmpMat);
					// 
					// 					tmpPlayer->GetCamera()->SetViewMatrixPos(&(CPhysics::GetMat4Vec(ePOS, tmpPlayer->GetCamera()->GetViewMatrix()) + (-d3dTmpVec * fRadius)));

					if(!tmpPlayer->GetShieldHit())
					{
						tmpPlayer->SetShieldHit(true);
						tmpPlayer->SetOtherObjectPos(tmpAsteroid->GetPosition() - tmpPlayer->GetPosition());
					}

					//		if(abs(scale) > 0.7)
					//		{
					//			tmpPlayer->SetActive(false);
					//			tmpAsteroid->SetActive(false);
					//		}
					//	else

					tmpAsteroid->SetHealth(int(max(tmpAsteroid->GetHealth() - 10*fScale,0)));
					


					if(tmpPlayer->GetShields() > 0)
					{
						nDifference = int((20 * fScale) - tmpPlayer->GetShields());
						if(nDifference >= 0)
						{
							tmpPlayer->SetShields(0);
						}
						else
						{
							tmpPlayer->SetShields(max(tmpPlayer->GetShields() - (int)(20 * fScale), 0), tmpPlayer->CalculateDamageDirection(tmpAsteroid->GetPosition()) );
							WwiseNS::SoundEvent(eHydraSheildHit, ePlay);
						}

						tmpPlayer->SetPlayShieldHit(true);
					}

					if(tmpPlayer->GetShields() <= 0 && tmpPlayer->GetHealth())
					{
						if(nDifference >= 0)
						{
							tmpPlayer->SetHealth(tmpPlayer->GetHealth() - nDifference, tmpPlayer->CalculateDamageDirection(tmpAsteroid->GetPosition()), _pGame);
						}
						else
						{
							tmpPlayer->SetHealth(int(max(tmpPlayer->GetHealth() - 20*fScale, 0)), tmpPlayer->CalculateDamageDirection(tmpAsteroid->GetPosition()), _pGame);
							WwiseNS::SoundEvent(eHydraSheildHit, ePlay);
						}

						tmpPlayer->SetPlayHullHit(true);
					}
					tmpPlayer->SetIsHit(true);
				}

				//D3DXMatrixTranslation(tmpPlayer->GetMatrix(), -tmpVec.x, -tmpVec.y, -tmpVec.y);

			}

			break;
		case eBOSS:
			{

				if(_fDelta >= EPSILON)
				{
					return;
				}

				int nDifference = -1;

				CPlayer* tmpPlayer = (CPlayer*)_pLhs;
				CBoss* tmpEnemy = (CBoss*)_pRhs;

				if(!tmpEnemy->GetIsVisable())
				{
					break;
				}

				D3DXVECTOR3 d3dTmpVec = tmpEnemy->GetPosition() - tmpPlayer->GetPosition();
				D3DXVec3Normalize(&d3dTmpVec, &d3dTmpVec);
				//	D3DXMatrixTranslation(tmpPlayer->GetMatrix(), -tmpVec.x, -tmpVec.y, -tmpVec.y);
				float fScale = D3DXVec3Dot(&d3dTmpVec, &CPhysics::GetMat4Vec(eZAXIS, tmpPlayer->GetMatrix()));


			
				{
					D3DXMATRIX d3dtmpMat;
					float fRadius = ((CSphere*)tmpPlayer->GetBV())->GetRadius();
					D3DXMatrixTranslation(&d3dtmpMat, -d3dTmpVec.x * fRadius, -d3dTmpVec.y * fRadius, -d3dTmpVec.z * fRadius);

					tmpPlayer->SetVelocity(tmpPlayer->GetVelocity() - d3dTmpVec*tmpPlayer->GetVelocityModifier());


					if(!tmpPlayer->GetShieldHit())
					{
						tmpPlayer->SetShieldHit(true);
						tmpPlayer->SetOtherObjectPos(tmpEnemy->GetPosition() - tmpPlayer->GetPosition());
					}




					if(tmpPlayer->GetShields())
					{
						nDifference = int(2*abs(fScale)) - tmpPlayer->GetShields();
						if(nDifference >= 0)
						{
							tmpPlayer->SetShields(0);
						}
						else
						{
							tmpPlayer->SetShields(max(tmpPlayer->GetShields() - int(2*abs(fScale)), 0), tmpPlayer->CalculateDamageDirection(tmpEnemy->GetPosition()) );
						}
						tmpPlayer->SetPlayShieldHit(true);
					}
					if(tmpPlayer->GetHealth() && tmpPlayer->GetShields() <= 0)
					{
						if(nDifference >= 0)
						{
							tmpPlayer->SetHealth(int(max(tmpPlayer->GetHealth() - nDifference, 0)), tmpPlayer->CalculateDamageDirection(tmpEnemy->GetPosition()), _pGame);
						}
						else
						{
							tmpPlayer->SetHealth(int(max(tmpPlayer->GetHealth() - 2*abs(fScale), 0)), tmpPlayer->CalculateDamageDirection(tmpEnemy->GetPosition()), _pGame);
						}

						tmpPlayer->SetPlayHullHit(true);

					}
				}
			}
			break;

		default:
			{
				// vector between player and asteroid position
				// dot product between new vec and player forward
				// scale by abs(dotproduct)

				if(_fDelta >= EPSILON*3)
				{
					return;
				}

				int nDifference = -1;

				CPlayer* tmpPlayer = (CPlayer*)_pLhs;
				CEnemy* tmpEnemy = (CEnemy*)_pRhs;

				D3DXVECTOR3 d3dTmpVec = tmpEnemy->GetPosition() - tmpPlayer->GetPosition();
				D3DXVec3Normalize(&d3dTmpVec, &d3dTmpVec);
				//	D3DXMatrixTranslation(tmpPlayer->GetMatrix(), -tmpVec.x, -tmpVec.y, -tmpVec.y);
				float fScale = D3DXVec3Dot(&d3dTmpVec, &CPhysics::GetMat4Vec(eZAXIS, tmpPlayer->GetMatrix()));

				D3DXMATRIX d3dtmpMat;
				float fRadius = ((CSphere*)tmpPlayer->GetBV())->GetRadius();
				D3DXMatrixTranslation(&d3dtmpMat, -d3dTmpVec.x * fRadius, -d3dTmpVec.y * fRadius, -d3dTmpVec.z * fRadius);

				if(nRHS == eYELLOW_ENEMY)
				{
					tmpPlayer->SetVelocity(tmpPlayer->GetVelocity() - 0.5f*d3dTmpVec*tmpEnemy->GetVelocityModifier());
					tmpEnemy->SetVelocity(tmpEnemy->GetVelocity() + d3dTmpVec*tmpPlayer->GetVelocityModifier());
				}
				else if(nRHS == eGENERATOR)
				{
					tmpPlayer->SetVelocity(tmpPlayer->GetVelocity() - 0.3f*d3dTmpVec*tmpEnemy->GetVelocityModifier());
					//tmpEnemy->SetVelocity(tmpEnemy->GetVelocity() + d3dTmpVec*tmpPlayer->GetVelocityModifier());
				}
				else if(nRHS != eBOSS)
				{
					tmpPlayer->SetVelocity(tmpPlayer->GetVelocity() - d3dTmpVec*tmpEnemy->GetVelocityModifier());
					tmpEnemy->SetVelocity(tmpEnemy->GetVelocity() + d3dTmpVec*tmpPlayer->GetVelocityModifier());
				}
				else
				{
					tmpPlayer->SetVelocity(tmpPlayer->GetVelocity() - d3dTmpVec*tmpPlayer->GetVelocityModifier());
				}


				if(fScale > 0.8 && tmpEnemy->GetType() != eYELLOW_ENEMY)
				{
					//tmpPlayer->SetHealth(0);
					//tmpPlayer->SetShields(0);
					//tmpEnemy->SetHealth(0);
				}
				else
				{


					// 					d3dtmpMat = *tmpPlayer->GetMatrix() * d3dtmpMat;
					// 
					// 					tmpPlayer->SetMatrix(&d3dtmpMat);
					// 
					// 
					// 					tmpPlayer->GetCamera()->SetViewMatrixPos(&(CPhysics::GetMat4Vec(ePOS, tmpPlayer->GetCamera()->GetViewMatrix()) + (-d3dTmpVec * fRadius)));



					if(!tmpPlayer->GetShieldHit())
					{
						tmpPlayer->SetShieldHit(true);
						tmpPlayer->SetOtherObjectPos(tmpEnemy->GetPosition() - tmpPlayer->GetPosition());
					}

					if(tmpEnemy->GetShields())
					{
						//tmpEnemy->SetShields(int(max(tmpEnemy->GetShields() - , 0)));

						nDifference = (int)(10*abs(fScale)) - tmpEnemy->GetShields();
						if(nDifference >= 0)
						{
							tmpEnemy->SetShields(0);
						}
						else
						{
							tmpEnemy->SetShields(max(tmpEnemy->GetShields() - (int)(10*abs(fScale)), 0) );
						}

						//	tmpPlayer->SetPlayShieldHit(true);
					}

					if(tmpEnemy->GetHealth() && tmpEnemy->GetShields() <= 0)
					{
						if(nDifference)
						{
							tmpEnemy->SetHealth(int(max(tmpEnemy->GetHealth() - nDifference, 0)));
						}
						else
						{
							tmpEnemy->SetHealth(int(max(tmpEnemy->GetHealth() - 10*abs(fScale), 0)));
						}

						/*	if(tmpEnemy->GetHealth() == 0)
						{
						tmpEnemy->SetActive(false);
						}*/
					}

					if(tmpEnemy->GetType() == eBLUE_ENEMY || tmpEnemy->GetType() == eYELLOW_ENEMY)
					{
						if(!tmpEnemy->GetShieldHit())
						{
							tmpEnemy->SetShieldHit(true);
							tmpEnemy->SetOtherObjectPos(tmpPlayer->GetPosition() - tmpEnemy->GetPosition());
						}
					}

					nDifference = -1;

					//if (tmpEnemy->GetType() == eYELLOW_ENEMY)
					//{

					//	if(tmpPlayer->GetShields())
					//	{
					//		//tmpPlayer->SetShields(int(max(tmpPlayer->GetShields() - YELLOW_ENEMY_DAMAGE*abs(scale), 0)));
					//		nDifference = int(YELLOW_ENEMY_DAMAGE*abs(fScale)) - tmpPlayer->GetShields();
					//		if(nDifference >= 0)
					//		{
					//			tmpPlayer->SetShields(0);
					//		}
					//		else
					//		{
					//			tmpPlayer->SetShields(max(tmpPlayer->GetShields() - int(YELLOW_ENEMY_DAMAGE*abs(fScale)), 0) );
					//		}

					//		tmpPlayer->SetPlayShieldHit(true);
					//	}

					//	if(tmpPlayer->GetHealth() && tmpPlayer->GetShields() <= 0)
					//	{
					//		if(nDifference >= 0)
					//		{
					//			tmpPlayer->SetHealth(int(max(tmpPlayer->GetHealth() - nDifference, 0)));
					//		}
					//		else
					//		{
					//			tmpPlayer->SetHealth(int(max(tmpPlayer->GetHealth() - YELLOW_ENEMY_DAMAGE*abs(fScale), 0)));
					//		}
					//		tmpPlayer->SetPlayHullHit(true);

					//	}


					//}
					//else
					{
						if(tmpPlayer->GetShields())
						{
							nDifference = int(10*abs(fScale)) - tmpPlayer->GetShields();
							if(nDifference >= 0)
							{
								tmpPlayer->SetShields(0);
							}
							else
							{
								tmpPlayer->SetShields(max(tmpPlayer->GetShields() - int(10*abs(fScale)), 0), tmpPlayer->CalculateDamageDirection(tmpEnemy->GetPosition()));
							}
							tmpPlayer->SetPlayShieldHit(true);
						}
						if(tmpPlayer->GetHealth() && tmpPlayer->GetShields() <= 0)
						{
							if(nDifference >= 0)
							{
								tmpPlayer->SetHealth(int(max(tmpPlayer->GetHealth() - nDifference, 0)), tmpPlayer->CalculateDamageDirection(tmpEnemy->GetPosition()), _pGame);
							}
							else
							{
								tmpPlayer->SetHealth(int(max(tmpPlayer->GetHealth() - 10*abs(fScale), 0)), tmpPlayer->CalculateDamageDirection(tmpEnemy->GetPosition()), _pGame);
							}

							tmpPlayer->SetPlayHullHit(true);

						}
					}
					//cout << "Player shield: " << tmpPlayer->GetShields() << endl;
					//cout << "Player Health: " << tmpPlayer->GetHealth() << endl;
					//D3DXMatrixTranslation(tmpPlayer->GetMatrix(), -tmpVec.x, -tmpVec.y, -tmpVec.y);
				}

			}
			break;
		}
	}
	//TODO YELLOW
	else if(nLHS == eRED_ENEMY || nLHS == eBLUE_ENEMY || _pLhs->GetType() == eYELLOW_ENEMY)
	{
		switch(nRHS)
		{
		case eMISSILE:
			{

				if(_fDelta >= EPSILON*3)
				{
					return;
				}

				int nDifference = -1;

				CEnemy* tmpEnemy = (CEnemy*)_pLhs;
				CMissile* tmpMissile = (CMissile*)_pRhs;

				if(tmpMissile->GetOwner() == tmpEnemy || !tmpMissile->GetActive())
				{
					break;
				}

				D3DXVECTOR3 d3dTmpVec = tmpMissile->GetPosition() - tmpEnemy->GetPosition();
				D3DXVec3Normalize(&d3dTmpVec, &d3dTmpVec);

				if(nLHS == eYELLOW_ENEMY)
				{
					tmpEnemy->SetVelocity(tmpEnemy->GetVelocity() + d3dTmpVec*tmpMissile->GetVelocityModifier()*0.2f);
					if(_pGame != NULL && tmpMissile->GetOwner() && tmpMissile->GetOwner()->GetType() == eBLUE_PLAYER)
					{
						// To make it fair since you kill the yellow enemies in 1 hit anyways.
						_pGame->GetCurrentStats()->MissileHit();
						_pGame->GetCurrentStats()->MissileHit();
						_pGame->GetCurrentStats()->MissileHit();
					}
				}
				else
				{
					tmpEnemy->SetVelocity(tmpEnemy->GetVelocity() + d3dTmpVec*tmpMissile->GetVelocityModifier()*0.1f);
					if(_pGame != NULL && tmpMissile->GetOwner() && tmpMissile->GetOwner()->GetType() == eBLUE_PLAYER)
						_pGame->GetCurrentStats()->MissileHit();
				}

				WwiseNS::SoundEvent(eMissileExplosion, ePlay, false, tmpEnemy);

				int nType = tmpEnemy->GetType();

				if (nType == eBLUE_ENEMY)
				{
					if(tmpEnemy->GetShields())
					{
						nDifference = (PLAYER_MISSILE_DAMAGE/6) - tmpEnemy->GetShields();
						if(nDifference >= 0)
						{
							tmpEnemy->SetShields(0);
						}
						else
						{
							tmpEnemy->SetShields(max(tmpEnemy->GetShields() - (PLAYER_MISSILE_DAMAGE/ 80 ), 0) );
						}

						if(!tmpEnemy->GetShieldHit())
						{
							//TODO SW pass in correct effect type
							tmpMissile->CreateEffect(eGENERIC_EXPLOSION);

							tmpEnemy->SetShieldHit(true);
							tmpEnemy->SetOtherObjectPos(tmpMissile->GetPosition() - tmpEnemy->GetPosition());
						}


						tmpEnemy->SetIsHit(true);
						tmpEnemy->SetFasterIsHit(true);
					}
					if(tmpEnemy->GetHealth() && tmpEnemy->GetShields() <=0)
					{
						if(nDifference >= 0)
						{
							tmpEnemy->SetHealth(int(max(tmpEnemy->GetHealth() - nDifference*3, 0)));
						}
						else
						{
							tmpEnemy->SetHealth(int(max(tmpEnemy->GetHealth() - PLAYER_MISSILE_DAMAGE, 0)));

						}

						//TODO SW pass in correct effect type
						tmpMissile->CreateEffect(eMISSILE_HULL);

						tmpEnemy->SetIsHit(true);	


						//tmpEnemy->SetHealth(max(tmpEnemy->GetHealth() - PLAYER_MISSILE_DAMAGE, 0) );
					}

				}
				else if(nType == eRED_ENEMY)
				{
					/*if(tmpEnemy->GetShields())
					{
					nDifference = (PLAYER_MISSILE_DAMAGE) - tmpEnemy->GetShields();
					if(nDifference >= 0)
					{
					tmpEnemy->SetShields(0);
					}
					else
					{
					tmpEnemy->SetShields(max(tmpEnemy->GetShields() - (PLAYER_MISSILE_DAMAGE ), 0) );
					}


					}*/
					//if(tmpEnemy->GetHealth() )//&& tmpEnemy->GetShields() <=0)
					//{
					//	if(nDifference >= 0)
					//	{
					//		tmpEnemy->SetHealth(int(max(tmpEnemy->GetHealth() - nDifference, 0)));
					//	}
					//	else
					//	{
					tmpEnemy->SetHealth(int(max(tmpEnemy->GetHealth() - PLAYER_MISSILE_DAMAGE, 0)));
					//	}


					//}

					tmpEnemy->SetIsHit(true);
				}

				//TODO: YELLOW ENEMY
				else
				{
					int damage = PLAYER_MISSILE_DAMAGE;
					if (tmpEnemy->GetIsStunned())
					{
						damage *= 6;
					}

					if(tmpEnemy->GetShields())
					{
						nDifference = (damage) - tmpEnemy->GetShields();
						if(nDifference >= 0)
						{
							tmpEnemy->SetShields(0);
						}
						else
						{
							tmpEnemy->SetShields(max(tmpEnemy->GetShields() - (damage ), 0) );

						}
					}
					if(tmpEnemy->GetHealth() && tmpEnemy->GetShields() <=0)
					{
						if(nDifference >= 0)
						{
							tmpEnemy->SetHealth(int(max(tmpEnemy->GetHealth() - nDifference, 0)));
						}
						else
						{
							tmpEnemy->SetHealth(int(max(tmpEnemy->GetHealth() - damage, 0)));
						}

					}

					tmpEnemy->SetIsHit(true);
				}

				//TODO: YELLOW ENEMY
				//if(nType == eBLUE_ENEMY || nType == eYELLOW_ENEMY)
				//{
				//	if(!tmpEnemy->GetShieldHit())
				//	{
				//		//TODO SW pass in correct effect type
				//		tmpMissile->CreateEffect(eRED_ENEMY_DEATH);

				//		tmpEnemy->SetShieldHit(true);
				//		tmpEnemy->SetOtherObjectPos(tmpMissile->GetPosition() - tmpEnemy->GetPosition());
				//	}
				//}


				//TODO: YELLOW ENEMY
				/*	if ( nType == eYELLOW_ENEMY )
				{
				if(((CYellowShooting*)tmpEnemy->GetShootingBehavior())->GetDoOnce() )
				{
				tmpEnemy->PushBehavior(new CChaseMovement(tmpEnemy));
				((CYellowShooting*)tmpEnemy->GetShootingBehavior())->SetDoOnce(false);

				}
				((CYellowEnemy*)tmpEnemy)->SetFound(true);
				((CYellowEnemy*)tmpEnemy)->SetVelocityModifier(YELLOW_CHASE_VELOCITY_MODIFIER);
				((CYellowEnemy*)tmpEnemy)->SetTurnRate(YELLOW_CHASE_TURN_RATE);
				}*/

				tmpMissile->SetHealth(0);
				tmpMissile->SetIsScenery(true);
				
				/*	if(tmpEnemy->GetHealth()==0)
				{
				tmpEnemy->SetActive(false);
				}*/
			}
			break;

		case eLASER:
			{

				if(_fDelta >= EPSILON*1000)
				{
					return;
				}

				CEnemy* tmpEnemy = (CEnemy*)_pLhs;
				CLaser* tmpLaser = (CLaser*)_pRhs;

				int nDifference = -1;

				if(tmpLaser->GetOwner() == tmpEnemy || !tmpLaser->GetActive())
				{
					break;
				}

				WwiseNS::SoundEvent(eLaserHitMetal, ePlay, false, tmpEnemy);

				int nType = tmpEnemy->GetType();

				if (nType == eBLUE_ENEMY)
				{

					if(tmpEnemy->GetShields())
					{
						nDifference = (PLAYER_LASER_DAMAGE) - tmpEnemy->GetShields();
						if(nDifference >= 0)
						{
							tmpEnemy->SetShields(0);
						}
						else
						{
							tmpEnemy->SetShields(max(tmpEnemy->GetShields() - PLAYER_LASER_DAMAGE, 0) );
						}
						tmpEnemy->SetIsHit(true);
						tmpLaser->SetActive(false);

					}

					if(tmpEnemy->GetHealth() && tmpEnemy->GetShields() <=0)
					{
						tmpLaser->SetShouldBounce(true);
						//TODO SW add correct effect
						//tmpEnemy->SetHealth(max(tmpEnemy->GetHealth() - PLAYER_MISSILE_DAMAGE, 0) );
					}

				}
				else if (nType == eRED_ENEMY)
				{
					/*if(tmpEnemy->GetShields())
					{
					nDifference = (PLAYER_LASER_DAMAGE) - tmpEnemy->GetShields();
					if(nDifference >= 0)
					{
					tmpEnemy->SetShields(0);
					}
					else
					{
					tmpEnemy->SetShields(max(tmpEnemy->GetShields() - (PLAYER_LASER_DAMAGE ), 0) );
					}
					}*/
					/*	if(tmpEnemy->GetHealth() && tmpEnemy->GetShields() <=0)
					{
					if(nDifference >= 0)
					{
					tmpEnemy->SetHealth(int(max(tmpEnemy->GetHealth() - (nDifference), 0)));
					}
					else
					{*/
					tmpEnemy->SetHealth(int(max(tmpEnemy->GetHealth() - (PLAYER_LASER_DAMAGE), 0)));
					//}
					//}
					tmpLaser->SetActive(false);

					tmpEnemy->SetIsHit(true);					
				}
				//TODO Yellow Enemy

				else
				{
					int damage = PLAYER_LASER_DAMAGE;

					if(tmpEnemy->GetShields())
					{
						nDifference = (damage) - tmpEnemy->GetShields();
						if(nDifference >= 0)
						{
							tmpEnemy->SetShields(0);
						}
						else
						{
							tmpEnemy->SetShields(max(tmpEnemy->GetShields() - (damage ), 0) );
						}
					}
					if(tmpEnemy->GetHealth() && tmpEnemy->GetShields() <=0)
					{
						if(nDifference >= 0)
						{
							tmpEnemy->SetHealth(int(max(tmpEnemy->GetHealth() - (nDifference), 0)));
						}
						else
						{
							tmpEnemy->SetHealth(int(max(tmpEnemy->GetHealth() - (damage), 0)));
						}
					}

					tmpLaser->SetActive(false);
					tmpEnemy->SetIsHit(true);					
				}
				tmpLaser->CreateEffect(eLASER_HULL);



				// TODO: remove and do this in enemy dead state
				/*if(tmpEnemy->GetHealth()==0)
				{
				tmpEnemy->SetActive(false);
				}*/

				if(tmpEnemy->GetType() == eBLUE_ENEMY)
				{
					if(!tmpEnemy->GetShieldHit())
					{
						tmpLaser->CreateEffect(eLASER_SHIELD);
						tmpEnemy->SetShieldHit(true);
						tmpEnemy->SetOtherObjectPos(tmpLaser->GetPosition() - tmpEnemy->GetPosition());
					}
				}

				//TODO Yellow Enemy
				/*if ( nType == eYELLOW_ENEMY )
				{
				if(((CYellowShooting*)tmpEnemy->GetShootingBehavior())->GetDoOnce() )
				{
				tmpEnemy->PushBehavior(new CChaseMovement(tmpEnemy));
				((CYellowShooting*)tmpEnemy->GetShootingBehavior())->SetDoOnce(false);

				}
				((CYellowEnemy*)tmpEnemy)->SetFound(true);
				((CYellowEnemy*)tmpEnemy)->SetVelocityModifier(YELLOW_CHASE_VELOCITY_MODIFIER);
				((CYellowEnemy*)tmpEnemy)->SetTurnRate(YELLOW_CHASE_TURN_RATE);
				}*/
			}

			break;

		case eASTEROID:
			{
				// vector between player and asteroid position
				// dot product between new vec and player forward
				// scale by abs(dotproduct)


				// TODO ALEX REMOVE THIS WHEN OBJECT AVOIDANCE FOR ENEMIES IS IMPLEMENTED
				//return;

				if(_fDelta >= EPSILON*3)
				{
					return;
				}

				CEnemy* tmpEnemy = (CEnemy*)_pLhs;
				CAsteroid* tmpAsteroid = (CAsteroid*)_pRhs;

				int nDifference = -1;

				D3DXVECTOR3 d3dTmpVec = tmpAsteroid->GetPosition() - tmpEnemy->GetPosition();
				D3DXVec3Normalize(&d3dTmpVec, &d3dTmpVec);
				//	D3DXMatrixTranslation(tmpEnemy->GetMatrix(), -tmpVec.x, -tmpVec.y, -tmpVec.y);
				float fScale = abs(D3DXVec3Dot(&d3dTmpVec, &CPhysics::GetMat4Vec(eZAXIS, tmpEnemy->GetMatrix())));

				tmpEnemy->SetVelocity(tmpEnemy->GetVelocity() - d3dTmpVec*tmpEnemy->GetVelocityModifier());

				if(nLHS == eYELLOW_ENEMY)
				{
					tmpAsteroid->SetVelocity(tmpAsteroid->GetVelocity() + d3dTmpVec*tmpEnemy->GetVelocityModifier()*0.25f);
				}
				else
				{
					tmpAsteroid->SetVelocity(tmpAsteroid->GetVelocity() + d3dTmpVec*tmpEnemy->GetVelocityModifier()*0.25f);
				}

				if(fScale > 0.7)
				{
					tmpAsteroid->SetHealth(0);
				}
				else
				{
					tmpAsteroid->SetHealth(int(max(tmpAsteroid->GetHealth() - 80*fScale,0)));
				}

				tmpAsteroid->SetIsHit(true);
				// 				if(tmpAsteroid->GetHealth() == 0)
				// 				{
				// 					tmpAsteroid->SetActive(false);
				// 				}

				if(tmpEnemy->GetShields())
				{
					nDifference = int(83*fScale) - tmpEnemy->GetShields();
					if(nDifference >= 0)
					{
						tmpEnemy->SetShields(0);
					}
					else
					{
						tmpEnemy->SetShields(max(tmpEnemy->GetShields() - (int)(83*fScale), 0) );
					}

				}
				//	tmpEnemy->SetShields(int(max(tmpEnemy->GetShields() - 100*scale, 0)));
				if(tmpEnemy->GetHealth() && tmpEnemy->GetShields() <=0)
				{
					if(nDifference >= 0)
					{
						tmpEnemy->SetHealth(int(max(tmpEnemy->GetHealth() - 83*fScale, 0)));
					}
					else
					{
						tmpEnemy->SetHealth(int(max(tmpEnemy->GetHealth() - (int)(83*fScale), 0)));
					}

					/*if(tmpEnemy->GetHealth() == 0)
					{
					tmpEnemy->SetActive(false);
					}*/
				}

				tmpEnemy->SetIsHit(true);

				if(tmpEnemy->GetType() == eBLUE_ENEMY || tmpEnemy->GetType() == eYELLOW_ENEMY)
				{
					if(!tmpEnemy->GetShieldHit())
					{
						tmpEnemy->SetShieldHit(true);
						tmpEnemy->SetOtherObjectPos(tmpAsteroid->GetPosition() - tmpEnemy->GetPosition());
					}
				}

				//D3DXMatrixTranslation(tmpEnemy->GetMatrix(), -tmpVec.x, -tmpVec.y, -tmpVec.y);


			}
			break;

		case eLARGE_ASTEROID:
			{
				// vector between player and asteroid position
				// dot product between new vec and player forward
				// scale by abs(dotproduct)

				if(_fDelta >= EPSILON*3)
				{
					return;
				}



				CEnemy* tmpEnemy = (CEnemy*)_pLhs;
				CAsteroid* tmpAsteroid = (CAsteroid*)_pRhs;

				int nDifference = -1;

				D3DXVECTOR3 d3dTmpVec = tmpAsteroid->GetPosition() - tmpEnemy->GetPosition();
				D3DXVec3Normalize(&d3dTmpVec, &d3dTmpVec);
				//	D3DXMatrixTranslation(tmpEnemy->GetMatrix(), -tmpVec.x, -tmpVec.y, -tmpVec.y);
				float fScale = abs(D3DXVec3Dot(&d3dTmpVec, &CPhysics::GetMat4Vec(eZAXIS, tmpEnemy->GetMatrix())));

				tmpEnemy->SetVelocity(tmpEnemy->GetVelocity() - d3dTmpVec*tmpEnemy->GetVelocityModifier());
				if(nLHS == eYELLOW_ENEMY)
				{
					tmpAsteroid->SetVelocity(tmpAsteroid->GetVelocity() + d3dTmpVec*tmpEnemy->GetVelocityModifier()*0.25f);
				}
				else
				{
					tmpAsteroid->SetVelocity(tmpAsteroid->GetVelocity() + d3dTmpVec*tmpEnemy->GetVelocityModifier()*0.25f);
				}

				if(fScale > 0.65)
				{
					tmpEnemy->SetHealth(0);
					tmpEnemy->SetShields(0);
				}
				else
				{
					//	tmpAsteroid->SetHealth(int(max(tmpAsteroid->GetHealth() - 80*fScale,0)));


					tmpAsteroid->SetIsHit(true);
					// 				if(tmpAsteroid->GetHealth() == 0)
					// 				{
					// 					tmpAsteroid->SetActive(false);
					// 				}

					if(tmpEnemy->GetShields())
					{
						nDifference = int(83*fScale) - tmpEnemy->GetShields();
						if(nDifference >= 0)
						{
							tmpEnemy->SetShields(0);
						}
						else
						{
							tmpEnemy->SetShields(max(tmpEnemy->GetShields() - (int)(83*fScale), 0) );
						}

					}
					//	tmpEnemy->SetShields(int(max(tmpEnemy->GetShields() - 100*scale, 0)));
					if(tmpEnemy->GetHealth() && tmpEnemy->GetShields() <=0)
					{
						if(nDifference >= 0)
						{
							tmpEnemy->SetHealth(int(max(tmpEnemy->GetHealth() - 83*fScale, 0)));
						}
						else
						{
							tmpEnemy->SetHealth(int(max(tmpEnemy->GetHealth() - (int)(83*fScale), 0)));
						}

						/*if(tmpEnemy->GetHealth() == 0)
						{
						tmpEnemy->SetActive(false);
						}*/
					}

					tmpEnemy->SetIsHit(true);

					if(tmpEnemy->GetType() == eBLUE_ENEMY || tmpEnemy->GetType() == eYELLOW_ENEMY)
					{
						if(!tmpEnemy->GetShieldHit())
						{
							tmpEnemy->SetShieldHit(true);
							tmpEnemy->SetOtherObjectPos(tmpAsteroid->GetPosition() - tmpEnemy->GetPosition());
						}
					}

				}
				//D3DXMatrixTranslation(tmpPlayer->GetMatrix(), -tmpVec.x, -tmpVec.y, -tmpVec.y);


			}
			break;

		case eRED_ENEMY:
		case eBLUE_ENEMY:
		case eYELLOW_ENEMY:
			{

				// TODO REMOVE LATER ALEX
				//	break;

				if(_fDelta >= EPSILON*3)
				{
					return;
				}
				int nDifference = -1;
				// vector between player and asteroid position
				// dot product between new vec and player forward
				// scale by abs(dotproduct)

				CEnemy* tmpEnemy = (CEnemy*)_pLhs;
				CEnemy* tmpEnemy2 = (CEnemy*)_pRhs;

				D3DXVECTOR3 d3dTmpVec = tmpEnemy->GetPosition() - tmpEnemy2->GetPosition();
				D3DXVECTOR3 d3dTmpVec2 = tmpEnemy2->GetPosition() - tmpEnemy->GetPosition();
				D3DXVec3Normalize(&d3dTmpVec, &d3dTmpVec);
				D3DXVec3Normalize(&d3dTmpVec2, &d3dTmpVec2);
				//	D3DXMatrixTranslation(tmpPlayer->GetMatrix(), -tmpVec.x, -tmpVec.y, -tmpVec.y);
				float fScale = D3DXVec3Dot(&d3dTmpVec, &CPhysics::GetMat4Vec(eZAXIS, tmpEnemy2->GetMatrix()));
				float fScale2 = D3DXVec3Dot(&d3dTmpVec2, &CPhysics::GetMat4Vec(eZAXIS, tmpEnemy->GetMatrix()));
			

				if(nRHS != nLHS && nRHS == eYELLOW_ENEMY)
				{
					tmpEnemy->SetVelocity(tmpEnemy->GetVelocity() - 0.5f*d3dTmpVec*tmpEnemy2->GetVelocityModifier());
				}
				else
				{
					tmpEnemy->SetVelocity(tmpEnemy->GetVelocity() - d3dTmpVec*tmpEnemy2->GetVelocityModifier());
				}

				if(nLHS != nRHS && nLHS == eYELLOW_ENEMY)
				{
					tmpEnemy2->SetVelocity(tmpEnemy2->GetVelocity() + 0.5f*d3dTmpVec*tmpEnemy->GetVelocityModifier());
				}
				else
				{
					tmpEnemy2->SetVelocity(tmpEnemy2->GetVelocity() + d3dTmpVec*tmpEnemy->GetVelocityModifier());
				}

				// It's a ship taking damage. Use metal hit sound
				WwiseNS::SoundEvent(eLaserHitMetal, ePlay, false, tmpEnemy);

				tmpEnemy->SetIsHit(true);

				if(tmpEnemy->GetType() == eBLUE_ENEMY || tmpEnemy->GetType() == eYELLOW_ENEMY)
				{
					if(!tmpEnemy->GetShieldHit())
					{
						tmpEnemy->SetShieldHit(true);
						tmpEnemy->SetOtherObjectPos(tmpEnemy2->GetPosition() - tmpEnemy->GetPosition());
					}
				}

				if(tmpEnemy2->GetType() == eBLUE_ENEMY || tmpEnemy2->GetType() == eYELLOW_ENEMY)
				{
					if(!tmpEnemy2->GetShieldHit())
					{
						tmpEnemy2->SetShieldHit(true);
						tmpEnemy2->SetOtherObjectPos(tmpEnemy->GetPosition() - tmpEnemy2->GetPosition());
					}
				}


				if(tmpEnemy->GetShields())
				{
					//tmpEnemy->SetShields(int(max(tmpEnemy->GetShields() - , 0)));

					nDifference = (int)(83*abs(fScale)) - tmpEnemy->GetShields();
					if(nDifference >= 0)
					{
						tmpEnemy->SetShields(0);
					}
					else
					{
						tmpEnemy->SetShields(max(tmpEnemy->GetShields() - (int)(83*abs(fScale)), 0) );
					}


				}

				if(tmpEnemy->GetHealth() && tmpEnemy->GetShields() <= 0)
				{
					if(nDifference)
					{
						tmpEnemy->SetHealth(int(max(tmpEnemy->GetHealth() - nDifference, 0)));
					}
					else
					{
						tmpEnemy->SetHealth(int(max(tmpEnemy->GetHealth() - 83*abs(fScale), 0)));
					}

					/*if(tmpEnemy->GetHealth() == 0)
					{
					tmpEnemy->SetActive(false);
					}*/
				}
				nDifference = -1;

				if(tmpEnemy2->GetShields())
				{
					//tmpEnemy->SetShields(int(max(tmpEnemy->GetShields() - , 0)));

					nDifference = (int)(83*abs(fScale)) - tmpEnemy2->GetShields();
					if(nDifference >= 0)
					{
						tmpEnemy2->SetShields(0);
					}
					else
					{
						tmpEnemy2->SetShields(max(tmpEnemy2->GetShields() - (int)(83*abs(fScale)), 0) );
					}

				}

				if(tmpEnemy2->GetHealth() && tmpEnemy2->GetShields() <= 0)
				{
					if(nDifference)
					{
						tmpEnemy2->SetHealth(int(max(tmpEnemy2->GetHealth() - nDifference, 0)));
					}
					else
					{
						tmpEnemy2->SetHealth(int(max(tmpEnemy2->GetHealth() - 83*abs(fScale), 0)));
					}

					/*if(tmpEnemy->GetHealth() == 0)
					{
					tmpEnemy->SetActive(false);
					}*/
				}

			}
			break;

		case eRED_PLAYER:
		case eBLUE_PLAYER:
			//case eYELLOW_PLAYER:
			{
				if(_fDelta >= EPSILON*3)
				{
					return;
				}

				int nDifference = -1;

				CPlayer* tmpPlayer = (CPlayer*)_pRhs;
				CEnemy* tmpEnemy = (CEnemy*)_pLhs;

				D3DXVECTOR3 d3dTmpVec = tmpEnemy->GetPosition() - tmpPlayer->GetPosition();
				D3DXVECTOR3 d3dTmpVec2 = tmpPlayer->GetPosition() - tmpEnemy->GetPosition();
				D3DXVec3Normalize(&d3dTmpVec, &d3dTmpVec);
				D3DXVec3Normalize(&d3dTmpVec2, &d3dTmpVec2);
				//	D3DXMatrixTranslation(tmpPlayer->GetMatrix(), -tmpVec.x, -tmpVec.y, -tmpVec.y);
				float fScale = D3DXVec3Dot(&d3dTmpVec, &CPhysics::GetMat4Vec(eZAXIS, tmpPlayer->GetMatrix()));
				float fScale2 = D3DXVec3Dot(&d3dTmpVec2, &CPhysics::GetMat4Vec(eZAXIS, tmpEnemy->GetMatrix()));


				D3DXMATRIX d3dtmpMat;
				float fRadius = ((CSphere*)tmpPlayer->GetBV())->GetRadius();
				D3DXMatrixTranslation(&d3dtmpMat, -d3dTmpVec.x * fRadius, -d3dTmpVec.y * fRadius, -d3dTmpVec.z * fRadius);

				if(nLHS == eYELLOW_ENEMY)
				{
					tmpPlayer->SetVelocity(tmpPlayer->GetVelocity() - 0.5f*d3dTmpVec*tmpEnemy->GetVelocityModifier());
				}
				else
				{
					tmpPlayer->SetVelocity(tmpPlayer->GetVelocity() - d3dTmpVec*tmpEnemy->GetVelocityModifier());
				}

				tmpEnemy->SetVelocity(tmpEnemy->GetVelocity() + d3dTmpVec*tmpPlayer->GetVelocityModifier());

				
				{
					D3DXMATRIX d3dtmpMat;
					float fRadius = ((CSphere*)tmpPlayer->GetBV())->GetRadius();
					D3DXMatrixTranslation(&d3dtmpMat, -d3dTmpVec.x * fRadius, -d3dTmpVec.y * fRadius, -d3dTmpVec.z * fRadius);

					// 					d3dtmpMat = *tmpPlayer->GetMatrix() * d3dtmpMat;
					// 
					// 					tmpPlayer->SetMatrix(&d3dtmpMat);
					// 
					// 
					// 					tmpPlayer->GetCamera()->SetViewMatrixPos(&(CPhysics::GetMat4Vec(ePOS, tmpPlayer->GetCamera()->GetViewMatrix()) + (-d3dTmpVec * fRadius)));


					if(!tmpPlayer->GetShieldHit())
					{
						tmpPlayer->SetShieldHit(true);
						tmpPlayer->SetOtherObjectPos(tmpEnemy->GetPosition() - tmpPlayer->GetPosition());
					}

					if(tmpEnemy->GetShields())
					{
						//tmpEnemy->SetShields(int(max(tmpEnemy->GetShields() - , 0)));

						nDifference = (int)(83*abs(fScale)) - tmpEnemy->GetShields();
						if(nDifference >= 0)
						{
							tmpEnemy->SetShields(0);
						}
						else
						{
							tmpEnemy->SetShields(max(tmpEnemy->GetShields() - (int)(83*abs(fScale)), 0) );
						}

						tmpPlayer->SetPlayShieldHit(true);
					}

					if(tmpEnemy->GetHealth() && tmpEnemy->GetShields() <= 0)
					{
						if(nDifference)
						{
							tmpEnemy->SetHealth(int(max(tmpEnemy->GetHealth() - nDifference, 0)));
						}
						else
						{
							tmpEnemy->SetHealth(int(max(tmpEnemy->GetHealth() - 83*abs(fScale), 0)));
						}

						/*if(tmpEnemy->GetHealth() == 0)
						{
						tmpEnemy->SetActive(false);
						}*/
					}

					if(tmpEnemy->GetType() == eBLUE_ENEMY )
					{
						if(!tmpEnemy->GetShieldHit())
						{
							tmpEnemy->SetShieldHit(true);
							tmpEnemy->SetOtherObjectPos(tmpPlayer->GetPosition() - tmpEnemy->GetPosition());
						}
					}

					nDifference = -1;

					//TODO YELLO
					//if (tmpEnemy->GetType() == eYELLOW_ENEMY)
					//{

					//	if(tmpPlayer->GetShields())
					//	{
					//		//tmpPlayer->SetShields(int(max(tmpPlayer->GetShields() - YELLOW_ENEMY_DAMAGE*abs(scale), 0)));
					//		nDifference = int(YELLOW_ENEMY_DAMAGE*abs(fScale)) - tmpPlayer->GetShields();
					//		if(nDifference >= 0)
					//		{
					//			tmpPlayer->SetShields(0);
					//		}
					//		else
					//		{
					//			tmpPlayer->SetShields(max(tmpPlayer->GetShields() - int(YELLOW_ENEMY_DAMAGE*abs(fScale)), 0) );
					//		}

					//		tmpPlayer->SetPlayShieldHit(true);
					//	}

					//	if(tmpPlayer->GetHealth() && tmpPlayer->GetShields() <= 0)
					//	{
					//		if(nDifference >= 0)
					//		{
					//			tmpPlayer->SetHealth(int(max(tmpPlayer->GetHealth() - nDifference, 0)));
					//		}
					//		else
					//		{
					//			tmpPlayer->SetHealth(int(max(tmpPlayer->GetHealth() - YELLOW_ENEMY_DAMAGE*abs(fScale), 0)));
					//		}
					//		tmpPlayer->SetPlayHullHit(true);

					//	}


					//}
					//else
					{
						if(tmpPlayer->GetShields())
						{
							nDifference = int(83*abs(fScale)) - tmpPlayer->GetShields();
							if(nDifference >= 0)
							{
								tmpPlayer->SetShields(0);
							}
							else
							{
								tmpPlayer->SetShields(max(tmpPlayer->GetShields() - int(83*abs(fScale)), 0), tmpPlayer->CalculateDamageDirection(tmpEnemy->GetPosition()));
							}
							tmpPlayer->SetPlayShieldHit(true);
						}
						if(tmpPlayer->GetHealth() && tmpPlayer->GetShields() <= 0)
						{
							if(nDifference >= 0)
							{
								tmpPlayer->SetHealth(int(max(tmpPlayer->GetHealth() - nDifference, 0)), tmpPlayer->CalculateDamageDirection(tmpEnemy->GetPosition()), _pGame);
							}
							else
							{
								tmpPlayer->SetHealth(int(max(tmpPlayer->GetHealth() - 83*abs(fScale), 0)), tmpPlayer->CalculateDamageDirection(tmpEnemy->GetPosition()), _pGame);
							}

							tmpPlayer->SetPlayHullHit(true);

						}
					}
					//cout << "Player shield: " << tmpPlayer->GetShields() << endl;
					//cout << "Player Health: " << tmpPlayer->GetHealth() << endl;
					//D3DXMatrixTranslation(tmpPlayer->GetMatrix(), -tmpVec.x, -tmpVec.y, -tmpVec.y);
				}

			}
			break;
		}
	}

	else if(nLHS == eLASER)
	{
		switch(nRHS)
		{
		case eRED_ENEMY:
		case eBLUE_ENEMY:
		case eYELLOW_ENEMY:
			{
				if(_fDelta >= EPSILON*2000)
				{
					return;
				}

				CEnemy* tmpEnemy = (CEnemy*)_pRhs;
				CLaser* tmpLaser = (CLaser*)_pLhs;

				int nDifference = -1;

				if(tmpLaser->GetOwner() == tmpEnemy || !tmpLaser->GetActive())
				{
					break;
				}

				WwiseNS::SoundEvent(eLaserHitMetal, ePlay, false, tmpEnemy);

				int nType = tmpEnemy->GetType();

				if (nType == eBLUE_ENEMY)
				{

					if(tmpEnemy->GetShields())
					{
						nDifference = (PLAYER_LASER_DAMAGE) - tmpEnemy->GetShields();
						if(nDifference >= 0)
						{
							tmpEnemy->SetShields(0);
						}
						else
						{
							tmpEnemy->SetShields(max(tmpEnemy->GetShields() - PLAYER_LASER_DAMAGE, 0) );
						}
						tmpEnemy->SetIsHit(true);
						if(_pGame != NULL && tmpLaser->GetOwner() && tmpLaser->GetOwner()->GetType() == eRED_PLAYER)
							_pGame->GetCurrentStats()->LaserHit();
						tmpLaser->SetActive(false);
					}

					if(tmpEnemy->GetHealth() && tmpEnemy->GetShields() <=0)
					{
						
						tmpLaser->SetShouldBounce(true);
						//TODO SW add correct effect
						//tmpLaser->CreateEffect(eLASER_HULL);
						//tmpEnemy->SetHealth(max(tmpEnemy->GetHealth() - PLAYER_MISSILE_DAMAGE, 0) );
					}

				}
				else if (nType == eRED_ENEMY)
				{
					/*if(tmpEnemy->GetShields())
					{
					nDifference = (PLAYER_LASER_DAMAGE) - tmpEnemy->GetShields();
					if(nDifference >= 0)
					{
					tmpEnemy->SetShields(0);
					}
					else
					{
					tmpEnemy->SetShields(max(tmpEnemy->GetShields() - (PLAYER_LASER_DAMAGE ), 0) );
					}
					}*/
					/*	if(tmpEnemy->GetHealth() && tmpEnemy->GetShields() <=0)
					{
					if(nDifference >= 0)
					{
					tmpEnemy->SetHealth(int(max(tmpEnemy->GetHealth() - (nDifference), 0)));
					}
					else
					{*/
					tmpEnemy->SetHealth(int(max(tmpEnemy->GetHealth() - (PLAYER_LASER_DAMAGE), 0)));
					//}
					//}

					tmpEnemy->SetIsHit(true);
					if(_pGame != NULL && tmpLaser->GetOwner() && tmpLaser->GetOwner()->GetType() == eRED_PLAYER)
						_pGame->GetCurrentStats()->LaserHit();
					tmpLaser->SetActive(false);
				}
				//TODO Yellow Enemy

				else
				{
					int damage = PLAYER_LASER_DAMAGE;

					if(tmpEnemy->GetShields())
					{
						nDifference = (damage) - tmpEnemy->GetShields();
						if(nDifference >= 0)
						{
							tmpEnemy->SetShields(0);
						}
						else
						{
							tmpEnemy->SetShields(max(tmpEnemy->GetShields() - (damage ), 0) );
						}
					}
					if(tmpEnemy->GetHealth() && tmpEnemy->GetShields() <=0)
					{
						if(nDifference >= 0)
						{
							tmpEnemy->SetHealth(int(max(tmpEnemy->GetHealth() - (nDifference), 0)));
						}
						else
						{
							tmpEnemy->SetHealth(int(max(tmpEnemy->GetHealth() - (damage), 0)));
						}
					}

					tmpEnemy->SetIsHit(true);		

					if(_pGame != NULL && tmpLaser->GetOwner() && tmpLaser->GetOwner()->GetType() == eRED_PLAYER)
						_pGame->GetCurrentStats()->LaserHit();

					tmpLaser->SetActive(false);


				}
				tmpLaser->CreateEffect(eLASER_HULL);

				// TODO: remove and do this in enemy dead state
				/*if(tmpEnemy->GetHealth()==0)
				{
				tmpEnemy->SetActive(false);
				}*/

				if(tmpEnemy->GetType() == eBLUE_ENEMY)
				{
					if(!tmpEnemy->GetShieldHit())
					{
						tmpLaser->CreateEffect(eLASER_SHIELD);
						tmpEnemy->SetShieldHit(true);
						tmpEnemy->SetOtherObjectPos(tmpLaser->GetPosition() - tmpEnemy->GetPosition());
					}
				}

				//TODO Yellow Enemy
				/*if ( nType == eYELLOW_ENEMY )
				{
				if(((CYellowShooting*)tmpEnemy->GetShootingBehavior())->GetDoOnce() )
				{
				tmpEnemy->PushBehavior(new CChaseMovement(tmpEnemy));
				((CYellowShooting*)tmpEnemy->GetShootingBehavior())->SetDoOnce(false);

				}
				((CYellowEnemy*)tmpEnemy)->SetFound(true);
				((CYellowEnemy*)tmpEnemy)->SetVelocityModifier(YELLOW_CHASE_VELOCITY_MODIFIER);
				((CYellowEnemy*)tmpEnemy)->SetTurnRate(YELLOW_CHASE_TURN_RATE);
				}*/
			}
			break;

		case eHanger:
			{
				if(_fDelta >= EPSILON*1000)
				{
					return;
				}

				CBossHanger* tmpHanger = (CBossHanger*)_pRhs;
				CLaser* tmpLaser = (CLaser*)_pLhs;

				if (((CBoss*)tmpHanger->GetOwner())->GetPhase() != eSEEK_AND_DESTROY  || (tmpLaser->GetOwner()->GetType() != eRED_PLAYER && tmpLaser->GetOwner()->GetType() != eBLUE_PLAYER))
				{
					tmpLaser->AsteroidDust();
					tmpLaser->SetActive(false);
					break;
				}
				if(!tmpLaser->GetActive())
				{
					break;
				}
				tmpHanger->SetIsHit(true);
				tmpLaser->AsteroidDust();

				tmpHanger->SetShields(max(tmpHanger->GetShields() - PLAYER_LASER_DAMAGE, 0));
				if(_pGame != NULL && tmpLaser->GetOwner() && tmpLaser->GetOwner()->GetType() == eRED_PLAYER)
					_pGame->GetCurrentStats()->LaserHit();

				if(tmpHanger->GetHealth() && tmpHanger->GetShields() <=0)
				{
					tmpLaser->SetShouldBounce(true);
				}
				else
				{
					tmpLaser->SetActive(false);
				}

			}
			break;

		case eBOSS:
			{
				if(_fDelta >= EPSILON*1000)
				{
					return;
				}

				CBoss* tmpBoss = (CBoss*)_pRhs;
				CLaser* tmpLaser = (CLaser*)_pLhs;

				if(tmpLaser->GetOwner() || tmpLaser->GetOwner()->GetType() == eTURRET_SMALL)
				{
					break;
				}

				if(!tmpLaser->GetActive() || !tmpBoss->GetIsVisable())
				{
					break;
				}
				tmpBoss->SetIsHit(true);
				tmpLaser->AsteroidDust();
				if(_pGame != NULL && tmpLaser->GetOwner() && tmpLaser->GetOwner()->GetType() == eRED_PLAYER)
					_pGame->GetCurrentStats()->LaserHit();

				tmpBoss->SetHealth(max(tmpBoss->GetHealth() - PLAYER_LASER_DAMAGE, 0));

				tmpLaser->SetActive(false);

			}
			break;

		case eGENERATOR:
			{
				if(_fDelta >= EPSILON*1000)
				{
					return;
				}

				CGenerator* tmpGen = (CGenerator*)_pRhs;
				CLaser* tmpLaser = (CLaser*)_pLhs;

				if(_pGame != NULL && tmpLaser->GetOwner() && tmpLaser->GetOwner()->GetType() == eRED_PLAYER)
					_pGame->GetCurrentStats()->LaserHit();

				tmpGen->SetIsHit(true);
				tmpLaser->AsteroidDust();
				WwiseNS::SoundEvent(eLaserHitMetal, ePlay);
				tmpGen->SetHealth(max(tmpGen->GetHealth() - PLAYER_LASER_DAMAGE, 0));

				tmpLaser->SetActive(false);

			}
			break;

		case eTURRET_LASER:
		case eTURRET_MISSILE:
		case eTURRET_SMALL:
			{
				if(_fDelta >= EPSILON*1000)
				{
					return;
				}

				CBossTurret* tmpTurret = (CBossTurret*)_pRhs;
				CLaser* tmpLaser = (CLaser*)_pLhs;

				if(!tmpLaser->GetActive())
				{
					break;
				}
				if(_pGame != NULL && tmpLaser->GetOwner() && tmpLaser->GetOwner()->GetType() == eRED_PLAYER)
					_pGame->GetCurrentStats()->LaserHit();

				tmpTurret->SetIsHit(true);
				tmpLaser->AsteroidDust();

				tmpTurret->SetHealth(max(tmpTurret->GetHealth() - PLAYER_LASER_DAMAGE, 0));

				tmpLaser->SetActive(false);

			}
			break;

		case eASTEROID:
			{
				if(_fDelta >= EPSILON*1000)
				{
					return;
				}

				CAsteroid* tmpRock = (CAsteroid*)_pRhs;
				CLaser* tmpLaser = (CLaser*)_pLhs;

				if(!tmpLaser->GetActive())
				{
					break;
				}

				//Play rock hit sound for asteroid
				WwiseNS::SoundEvent(eLaserHitRock, ePlay, false, tmpRock);
				if(_pGame != NULL && tmpLaser->GetOwner() && tmpLaser->GetOwner()->GetType() == eRED_PLAYER)
					_pGame->GetCurrentStats()->LaserHit();

				tmpRock->SetIsHit(true);
				tmpRock->SetHealth(max(tmpRock->GetHealth() - PLAYER_LASER_DAMAGE, 0));
				tmpLaser->AsteroidDust();
				


				tmpLaser->SetActive(false);
			}
			break;

		case eLARGE_ASTEROID:
			{
				if(_fDelta >= EPSILON*1000)
				{
					return;
				}

				CAsteroid* tmpRock = (CAsteroid*)_pRhs;
				CLaser* tmpLaser = (CLaser*)_pLhs;

				if(!tmpLaser->GetActive())
				{
					break;
				}

				//Play rock hit sound for asteroid
				WwiseNS::SoundEvent(eLaserHitRock, ePlay, false, tmpRock);

				if(_pGame != NULL && tmpLaser->GetOwner() && tmpLaser->GetOwner()->GetType() == eRED_PLAYER)
					_pGame->GetCurrentStats()->LaserHit();

				tmpRock->SetIsHit(true);
				//	tmpRock->SetHealth(max(tmpRock->GetHealth() - 20, 0));
				tmpLaser->AsteroidDust();

				tmpLaser->SetActive(false);
			}
			break;
		case eSPACE_DEBRIS:
		case eSPACE_DEBRIS2:
		case eSPACE_DEBRIS3:
			{
				if(_fDelta >= EPSILON*1000)
				{
					return;
				}

				CEnvironmentObject* debris = (CEnvironmentObject*)_pRhs;
				CLaser* tmpLaser = (CLaser*)_pLhs;

				if(!tmpLaser->GetActive())
				{
					break;
				}

				//Play rock hit sound for asteroid
				WwiseNS::SoundEvent(eLaserHitRock, ePlay, false, debris);
				if(_pGame != NULL && tmpLaser->GetOwner() && tmpLaser->GetOwner()->GetType() == eRED_PLAYER)
					_pGame->GetCurrentStats()->LaserHit();

				debris->SetIsHit(true);
				debris->SetHealth(max(debris->GetHealth() - PLAYER_LASER_DAMAGE, 0));
				tmpLaser->AsteroidDust();
				

				tmpLaser->SetActive(false);
			}
			break;
		}
	}

	else if(nLHS == eASTEROID)
	{
		switch(nRHS)
		{
		case eLASER:
			{
				if(_fDelta >= EPSILON*1000)
				{
					return;
				}

				CAsteroid* tmpRock = (CAsteroid*)_pLhs;
				CLaser* tmpLaser = (CLaser*)_pRhs;

				if(!tmpLaser->GetActive())
				{
					break;
				}

				//Play rock hit sound for asteroid
				WwiseNS::SoundEvent(eLaserHitRock, ePlay, false, tmpRock);

				tmpRock->SetIsHit(true);
				tmpLaser->AsteroidDust();
				tmpRock->SetHealth(max(tmpRock->GetHealth() - PLAYER_LASER_DAMAGE, 0));

			

				tmpLaser->SetActive(false);

			}
			break;

		case eMISSILE:
			{
				if(_fDelta >= EPSILON*3)
				{
					return;
				}

				CAsteroid* tmpRock = (CAsteroid*)_pLhs;
				CMissile* tmpMissile = (CMissile*)_pRhs;

				if(!tmpMissile->GetActive())
				{
					break;
				}
				tmpRock->SetIsHit(true);
				tmpMissile->AsteroidDust();
				tmpRock->SetHealth(max(tmpRock->GetHealth() - PLAYER_MISSILE_DAMAGE, 0));

			

				tmpMissile->SetHealth(0);

			}
		}
	}

	else if(nLHS == eLARGE_ASTEROID)
	{
		switch(nRHS)
		{
		case eLASER:
			{
				if(_fDelta >= EPSILON*1000)
				{
					return;
				}

				CAsteroid* tmpRock = (CAsteroid*)_pLhs;
				CLaser* tmpLaser = (CLaser*)_pRhs;

				if(!tmpLaser->GetActive())
				{
					break;
				}
				tmpRock->SetIsHit(true);
				tmpLaser->AsteroidDust();
				//tmpRock->SetHealth(max(tmpRock->GetHealth() - 20, 0));

				tmpLaser->SetActive(false);

			}
			break;

		case eMISSILE:
			{

				if(_fDelta >= EPSILON*3)
				{
					return;
				}

				CAsteroid* tmpRock = (CAsteroid*)_pLhs;
				CMissile* tmpMissile = (CMissile*)_pRhs;

				if(!tmpMissile->GetActive())
				{
					break;
				}

				tmpRock->SetIsHit(true);
				tmpMissile->AsteroidDust();

				tmpMissile->SetHealth(0);

			}
		}
	}

	else if(nLHS == eMISSILE)
	{
		switch(nRHS)
		{
		case eHanger:
			{
				if(_fDelta >= EPSILON*100)
				{
					return;
				}

				CBossWinTurret* tmpHanger = (CBossWinTurret*)_pRhs;
				CMissile* tmpMissile = (CMissile*)_pLhs;
				
				if (((CBoss*)tmpHanger->GetOwner())->GetPhase() != eSEEK_AND_DESTROY  || (tmpMissile->GetOwner()->GetType() != eRED_PLAYER && tmpMissile->GetOwner()->GetType() != eBLUE_PLAYER))
				{
					tmpMissile->SetActive(false);
					break;
				}
				if(!tmpMissile->GetActive())
				{
					break;
				}

				if(_pGame != NULL && tmpMissile->GetOwner() && tmpMissile->GetOwner()->GetType() == eBLUE_PLAYER)
					_pGame->GetCurrentStats()->MissileHit();
				tmpHanger->SetIsHit(true);
				if (tmpHanger->GetShields() <= 0)
				{
					tmpHanger->SetHealth(max(tmpHanger->GetHealth() - PLAYER_MISSILE_DAMAGE, 0));
				}

				tmpMissile->SetHealth(0);
				tmpMissile->SetIsScenery(true);

			}
			break;

		case eBOSS:
			{
				if(_fDelta >= EPSILON*100)
				{
					return;
				}

				CBoss* tmpBoss = (CBoss*)_pRhs;
				CMissile* tmpMissile = (CMissile*)_pLhs;

				if(tmpMissile->GetOwner() || tmpMissile->GetOwner()->GetType() == eTURRET_MISSILE)
				{
					break;
				}

				if(tmpMissile->GetOwner() == tmpBoss || !tmpMissile->GetActive() || !tmpBoss->GetIsVisable())
				{
					break;
				}
				tmpBoss->SetIsHit(true);
				if(_pGame != NULL && tmpMissile->GetOwner() && tmpMissile->GetOwner()->GetType() == eBLUE_PLAYER)
					_pGame->GetCurrentStats()->MissileHit();
				tmpBoss->SetHealth(max(tmpBoss->GetHealth() - PLAYER_MISSILE_DAMAGE, 0));

				if(tmpBoss->GetHealth() == 0)
				{	
					tmpBoss->SetActive(false);
				}

				tmpMissile->SetHealth(0);

			}
			break;

		case eGENERATOR:
			{
				if(_fDelta >= EPSILON*100)
				{
					return;
				}

				CGenerator* tmpGen = (CGenerator*)_pRhs;
				CMissile* tmpMissile = (CMissile*)_pLhs;

				if(!tmpMissile->GetActive())
				{
					break;
				}
				tmpGen->SetIsHit(true);

				//tmpGen->SetHealth(max(tmpGen->GetHealth() - PLAYER_MISSILE_DAMAGE, 0));
// 
// 				if(tmpGen->GetHealth() == 0)
// 				{	
// 					//tmpGen->SetActive(false);
// 				}

				tmpMissile->SetHealth(0);
				tmpMissile->SetIsScenery(true);

			}
			break;

		case eTURRET_LASER:
		case eTURRET_MISSILE:
		case eTURRET_SMALL:
			{
				if(_fDelta >= EPSILON*100)
				{
					return;
				}

				CBossTurret* tmpTurret = (CBossTurret*)_pRhs;
				CMissile* tmpMissile = (CMissile*)_pLhs;

				if(tmpMissile->GetOwner() && tmpMissile->GetOwner()->GetType() == eTURRET_LASER)
				{
					break;
				}

				if (tmpTurret->GetOwner() && tmpTurret->GetOwner()->GetType() == eBOSS)
				{
					if (((CBoss*)tmpTurret->GetOwner())->GetShieldStatus() == eSHIELDS_UP && tmpTurret->GetOwner()->GetType() != eTURRET_SMALL)
					{
						tmpMissile->SetHealth(0);
						break;
					}
				}

				if(!tmpMissile->GetActive())
				{
					break;
				}
				tmpTurret->SetIsHit(true);
				if(_pGame != NULL && tmpMissile->GetOwner() && tmpMissile->GetOwner()->GetType() == eBLUE_PLAYER)
					_pGame->GetCurrentStats()->MissileHit();

				tmpTurret->SetHealth(max(tmpTurret->GetHealth() - PLAYER_MISSILE_DAMAGE / 10, 0));			

				tmpMissile->SetHealth(0);
				tmpMissile->SetIsScenery(1);

			}
			break;

		case eASTEROID:
			{
				if(_fDelta >= EPSILON*100)
				{
					return;
				}

				CAsteroid* tmpRock = (CAsteroid*)_pRhs;
				CMissile* tmpMissile = (CMissile*)_pLhs;

				if(!tmpMissile->GetActive())
				{
					break;
				}

				//Play rock hit sound for asteroid
				WwiseNS::SoundEvent(eLaserHitRock, ePlay, false, tmpRock);

				tmpRock->SetIsHit(true);
				if(_pGame != NULL && tmpMissile->GetOwner() && tmpMissile->GetOwner()->GetType() == eBLUE_PLAYER)
					_pGame->GetCurrentStats()->MissileHit();

				tmpRock->SetHealth(max(tmpRock->GetHealth() - PLAYER_MISSILE_DAMAGE, 0));
				tmpMissile->AsteroidDust();

				if(tmpRock->GetHealth() == 0)
				{	
					tmpRock->SetActive(false);
				}

				tmpMissile->SetHealth(0);
			}
			break;

		case eLARGE_ASTEROID:
			{
				if(_fDelta >= EPSILON*100)
				{
					return;
				}

				CAsteroid* tmpRock = (CAsteroid*)_pRhs;
				CMissile* tmpMissile = (CMissile*)_pLhs;
				static int numTimesHit = 0;

				if(!tmpMissile->GetActive())
				{
					break;
				}

				//Play rock hit sound for asteroid
				WwiseNS::SoundEvent(eLaserHitRock, ePlay, false, tmpRock);

				if (tmpMissile->GetOwner()->GetType() == eBOSS)
				{
					numTimesHit++;
					if (numTimesHit >= 9)
					{
						tmpRock->SetHealth(0);
						tmpRock->DeathVFX();
						numTimesHit = 0;
					}
				}
				tmpRock->SetIsHit(true);
				if(_pGame != NULL && tmpMissile->GetOwner() && tmpMissile->GetOwner()->GetType() == eBLUE_PLAYER)
					_pGame->GetCurrentStats()->MissileHit();
				//tmpRock->SetHealth(max(tmpRock->GetHealth() - 20, 0));
				tmpMissile->AsteroidDust();

				tmpMissile->SetHealth(0);
			}
			break;
		case eSPACE_DEBRIS:
		case eSPACE_DEBRIS2:
		case eSPACE_DEBRIS3:
			{
				if(_fDelta >= EPSILON*1000)
				{
					return;
				}

				CEnvironmentObject* debris = (CEnvironmentObject*)_pRhs;
				CMissile* tmpMissile = (CMissile*)_pLhs;


				if(!tmpMissile->GetActive())
				{
					break;
				}

				//Play rock hit sound for asteroid
				WwiseNS::SoundEvent(eLaserHitRock, ePlay, false, debris);

				debris->SetIsHit(true);
				if(_pGame != NULL && tmpMissile->GetOwner() && tmpMissile->GetOwner()->GetType() == eBLUE_PLAYER)
					_pGame->GetCurrentStats()->MissileHit();

				debris->SetHealth(max(debris->GetHealth() - PLAYER_MISSILE_DAMAGE, 0));
				tmpMissile->AsteroidDust();
				if(debris->GetHealth() == 0)
				{	
					debris->SetActive(false);
				}

				tmpMissile->SetHealth(0);
				tmpMissile->SetIsScenery(true);
			}
			break;

		}
	}

	else if(nLHS == eBOSS)
	{
		switch(nRHS)
		{
		case eLASER:
			{
				if(_fDelta >= EPSILON*100)
				{
					return;
				}

				CBoss* tmpBoss = (CBoss*)_pLhs;
				CLaser* tmpLaser = (CLaser*)_pRhs;

				if(tmpLaser->GetOwner() || tmpLaser->GetOwner()->GetType() != eTURRET_SMALL)
				{
					break;
				}

				if(!tmpLaser->GetActive() || !tmpBoss->GetIsVisable())
				{
					break;
				}
				tmpBoss->SetIsHit(true);

				tmpBoss->SetHealth(max(tmpBoss->GetHealth() - PLAYER_LASER_DAMAGE, 0));

				if(tmpBoss->GetHealth() == 0)
				{	
					tmpBoss->SetActive(false);
				}

				tmpLaser->SetActive(false);

			}
			break;

		case eMISSILE:
			{
				if(_fDelta >= EPSILON*3)
				{
					return;
				}

				CBoss* tmpBoss = (CBoss*)_pLhs;
				CMissile* tmpMissile = (CMissile*)_pRhs;

				if(tmpMissile->GetOwner() || tmpMissile->GetOwner()->GetType() == eTURRET_LASER)
				{
					break;
				}

				if(tmpMissile->GetOwner() == tmpBoss || !tmpMissile->GetActive() || !tmpBoss->GetIsVisable())
				{
					break;
				}

				tmpBoss->SetIsHit(true);

				tmpBoss->SetHealth(max(tmpBoss->GetHealth() - PLAYER_MISSILE_DAMAGE, 0));

				if(tmpBoss->GetHealth() == 0)
				{	
					tmpBoss->SetActive(false);
				}

				tmpMissile->SetHealth(0);

			}
			break;

		case eBLUE_PLAYER:
		case eRED_PLAYER:
			{
				if(_fDelta >= EPSILON*3)
				{
					return;
				}

				int nDifference = -1;

				CPlayer* tmpPlayer = (CPlayer*)_pRhs;
				CBoss* tmpEnemy = (CBoss*)_pLhs;

				if(!tmpEnemy->GetIsVisable())
				{
					break;
				}

				D3DXVECTOR3 d3dTmpVec = tmpEnemy->GetPosition() - tmpPlayer->GetPosition();
				D3DXVec3Normalize(&d3dTmpVec, &d3dTmpVec);
				//	D3DXMatrixTranslation(tmpPlayer->GetMatrix(), -tmpVec.x, -tmpVec.y, -tmpVec.y);
				float fScale = D3DXVec3Dot(&d3dTmpVec, &CPhysics::GetMat4Vec(eZAXIS, tmpPlayer->GetMatrix()));


				
				{
					D3DXMATRIX d3dtmpMat;
					float fRadius = ((CSphere*)tmpPlayer->GetBV())->GetRadius();
					D3DXMatrixTranslation(&d3dtmpMat, -d3dTmpVec.x * fRadius, -d3dTmpVec.y * fRadius, -d3dTmpVec.z * fRadius);

					tmpPlayer->SetVelocity(tmpPlayer->GetVelocity() + d3dTmpVec*tmpPlayer->GetVelocityModifier());


					if(!tmpPlayer->GetShieldHit())
					{
						tmpPlayer->SetShieldHit(true);
						tmpPlayer->SetOtherObjectPos(tmpEnemy->GetPosition() - tmpPlayer->GetPosition());
					}




					if(tmpPlayer->GetShields())
					{
						nDifference = int(2*abs(fScale)) - tmpPlayer->GetShields();
						if(nDifference >= 0)
						{
							tmpPlayer->SetShields(0);
						}
						else
						{
							tmpPlayer->SetShields(max(tmpPlayer->GetShields() - int(2*abs(fScale)), 0), tmpPlayer->CalculateDamageDirection(tmpEnemy->GetPosition()) );
						}
						tmpPlayer->SetPlayShieldHit(true);
					}
					if(tmpPlayer->GetHealth() && tmpPlayer->GetShields() <= 0)
					{
						if(nDifference >= 0)
						{
							tmpPlayer->SetHealth(int(max(tmpPlayer->GetHealth() - nDifference, 0)), tmpPlayer->CalculateDamageDirection(tmpEnemy->GetPosition()), _pGame);
						}
						else
						{
							tmpPlayer->SetHealth(int(max(tmpPlayer->GetHealth() - 2*abs(fScale), 0)), tmpPlayer->CalculateDamageDirection(tmpEnemy->GetPosition()), _pGame);
						}

						tmpPlayer->SetPlayHullHit(true);

					}
				}
				//cout << "Player shield: " << tmpPlayer->GetShields() << endl;
				//cout << "Player Health: " << tmpPlayer->GetHealth() << endl;
				//D3DXMatrixTranslation(tmpPlayer->GetMatrix(), -tmpVec.x, -tmpVec.y, -tmpVec.y);
			}
			break;
		}

	}


	else if(nLHS == eGENERATOR)
	{
		switch(nRHS)
		{
		case eLASER:
			{
				if(_fDelta >= EPSILON*1000)
				{
					return;
				}

				CGenerator* tmpGen = (CGenerator*)_pLhs;
				CLaser* tmpLaser = (CLaser*)_pRhs;

				if(!tmpLaser->GetActive())
				{
					break;
				}
				tmpGen->SetIsHit(true);

				tmpGen->SetHealth(max(tmpGen->GetHealth() - PLAYER_LASER_DAMAGE, 0));

				tmpLaser->SetActive(false);

			}
			break;

		case eMISSILE:
			{
				if(_fDelta >= EPSILON*3)
				{
					return;
				}

				CGenerator* tmpGen = (CGenerator*)_pLhs;
				CMissile* tmpMissile = (CMissile*)_pRhs;

				if(!tmpMissile->GetActive())
				{
					break;
				}
				tmpGen->SetIsHit(true);

				//tmpGen->SetHealth(max(tmpGen->GetHealth() - PLAYER_MISSILE_DAMAGE, 0));

				tmpMissile->SetHealth(0);

			}
			break;


		case eBLUE_PLAYER:
		case eRED_PLAYER:
			{
				if(_fDelta >= EPSILON*3)
				{
					return;
				}

				int nDifference = -1;

				CPlayer* tmpPlayer = (CPlayer*)_pRhs;
				CGenerator* tmpEnemy = (CGenerator*)_pLhs;

				D3DXVECTOR3 d3dTmpVec = tmpEnemy->GetPosition() - tmpPlayer->GetPosition();
				D3DXVec3Normalize(&d3dTmpVec, &d3dTmpVec);
				//	D3DXMatrixTranslation(tmpPlayer->GetMatrix(), -tmpVec.x, -tmpVec.y, -tmpVec.y);
				float fScale = D3DXVec3Dot(&d3dTmpVec, &CPhysics::GetMat4Vec(eZAXIS, tmpPlayer->GetMatrix()));


			
				{
					D3DXMATRIX d3dtmpMat;
					float fRadius = ((CSphere*)tmpPlayer->GetBV())->GetRadius();
					D3DXMatrixTranslation(&d3dtmpMat, -d3dTmpVec.x * fRadius, -d3dTmpVec.y * fRadius, -d3dTmpVec.z * fRadius);

					tmpPlayer->SetVelocity(tmpPlayer->GetVelocity() + d3dTmpVec*tmpPlayer->GetVelocityModifier()*0.3f);
					//tmpEnemy->SetVelocity(tmpEnemy->GetVelocity() + d3dTmpVec*tmpPlayer->GetVelocityModifier()*0.4f);


					if(!tmpPlayer->GetShieldHit())
					{
						tmpPlayer->SetShieldHit(true);
						tmpPlayer->SetOtherObjectPos(tmpEnemy->GetPosition() - tmpPlayer->GetPosition());
					}

					tmpEnemy->SetHealth(tmpEnemy->GetHealth() - 20);


					if(tmpPlayer->GetShields())
					{
						nDifference = int(2*abs(fScale)) - tmpPlayer->GetShields();
						if(nDifference >= 0)
						{
							tmpPlayer->SetShields(0);
						}
						else
						{
							tmpPlayer->SetShields(max(tmpPlayer->GetShields() - int(2*abs(fScale)), 0), tmpPlayer->CalculateDamageDirection(tmpEnemy->GetPosition()) );
						}
						tmpPlayer->SetPlayShieldHit(true);
					}
					if(tmpPlayer->GetHealth() && tmpPlayer->GetShields() <= 0)
					{
						if(nDifference >= 0)
						{
							tmpPlayer->SetHealth(int(max(tmpPlayer->GetHealth() - nDifference, 0)), tmpPlayer->CalculateDamageDirection(tmpEnemy->GetPosition()), _pGame);
						}
						else
						{
							tmpPlayer->SetHealth(int(max(tmpPlayer->GetHealth() - 2*abs(fScale), 0)), tmpPlayer->CalculateDamageDirection(tmpEnemy->GetPosition()), _pGame);
						}

						tmpPlayer->SetPlayHullHit(true);

					}
				}
				//cout << "Player shield: " << tmpPlayer->GetShields() << endl;
				//cout << "Player Health: " << tmpPlayer->GetHealth() << endl;
				//D3DXMatrixTranslation(tmpPlayer->GetMatrix(), -tmpVec.x, -tmpVec.y, -tmpVec.y);
			}
			break;
		}
	}
	else if(nLHS == eSPACE_DEBRIS)
	{
		CEnvironmentObject* tmpLaser = (CEnvironmentObject*)_pLhs;
		if(!tmpLaser->GetBossHelper())
		{
			return;
		}
		switch(nRHS)
		{
		case eLASER:
			{
				if(_fDelta >= EPSILON*1000)
				{
					return;
				}

				CLaser* tmpLaser = (CLaser*)_pRhs;

				if(!tmpLaser->GetActive()  )
				{
					break;
				}

				if ((tmpLaser->GetOwner()->GetType() == eRED_PLAYER || tmpLaser->GetOwner()->GetType() == eBLUE_PLAYER) )
				{
					tmpLaser->SetActive(false);
					tmpLaser->CreateEffect(eLASER_SHIELD);
					return;
				}
				tmpLaser->SetActive(false);

			}
			break;

		case eMISSILE:
			{
				if(_fDelta >= EPSILON*3)
				{
					return;
				}

				CMissile* tmpMissile = (CMissile*)_pRhs;

				if(!tmpMissile->GetActive())
				{
					break;
				}
				if ((tmpMissile->GetOwner()->GetType() == eRED_PLAYER || tmpMissile->GetOwner()->GetType() == eBLUE_PLAYER) )
				{
					tmpMissile->SetActive(false);
					return;
				}
				tmpMissile->SetHealth(0);
			}
			break;

		case eASTEROID:
			{
				if(_fDelta >= EPSILON*1000)
				{
					return;
				}

				CAsteroid* tmpLaser = (CAsteroid*)_pRhs;

				if(!tmpLaser->GetActive()  )
				{
					break;
				}
// 				CEffectSystem m_DeathAnimation;
// 
// 				EffectInformation::CreateEffect(eBOSS_EXPLOSION, &m_DeathAnimation, tmpLaser, false, true);
				tmpLaser->SetHealth(0);
			}
			break;
		
		case eSPACE_DEBRIS:
		case eSPACE_DEBRIS2:
		case eSPACE_DEBRIS3:
		case eSPACE_STATION:
		case eSPACESTATION1:
			{
				if(_fDelta >= EPSILON*1000)
				{
					return;
				}

			

				if(!tmpLaser->GetActive() || !tmpLaser->GetBossHelper() )
				{
					return;
				}
				//CEffectSystem m_DeathAnimation;
					CEnvironmentObject* tmpLaser = (CEnvironmentObject*)_pRhs;
					tmpLaser->CreateEffect(eBOSS_EXPLOSION);
				//EffectInformation::CreateEffect(eBOSS_EXPLOSION, &m_DeathAnimation, tmpLaser, false, true);
				tmpLaser->SetActive(0);

			}
			break;

		}
	}
	else if(nLHS == eTURRET_LASER || nLHS == eTURRET_MISSILE || nLHS == eTURRET_SMALL)
	{
		switch(nRHS)
		{
		case eLASER:
			{
				if(_fDelta >= EPSILON*1000)
				{
					return;
				}

				CBossTurret* tmpTurret = (CBossTurret*)_pLhs;
				CLaser* tmpLaser = (CLaser*)_pRhs;
				if (/*((CBoss*)tmpTurret->GetOwner())->GetShieldStatus() == eSHIELDS_UP ||*/ (tmpLaser->GetOwner()->GetType() != eRED_PLAYER && tmpLaser->GetOwner()->GetType() != eBLUE_PLAYER))
				{
					tmpLaser->SetActive(false);
					break;
				}
				if(!tmpLaser->GetActive())
				{
					break;
				}
				tmpTurret->SetIsHit(true);

				tmpTurret->SetHealth(max(tmpTurret->GetHealth() - PLAYER_LASER_DAMAGE, 0));

				tmpLaser->SetActive(false);

			}
			break;

		case eMISSILE:
			{
				if(_fDelta >= EPSILON*3)
				{
					return;
				}

				CBossTurret* tmpTurret = (CBossTurret*)_pLhs;
				CMissile* tmpMissile = (CMissile*)_pRhs;

				if(tmpMissile->GetOwner() && tmpMissile->GetOwner()->GetType() == eTURRET_LASER)
				{
					break;
				}

				if (((CBoss*)tmpTurret->GetOwner())->GetShieldStatus() == eSHIELDS_UP || (tmpMissile->GetOwner()->GetType() != eRED_PLAYER && tmpMissile->GetOwner()->GetType() != eBLUE_PLAYER))
				{
					tmpMissile->SetHealth(0);

					break;
				}
				if(!tmpMissile->GetActive())
				{
					break;
				}
				tmpTurret->SetIsHit(true);

				tmpTurret->SetHealth(max(tmpTurret->GetHealth() - PLAYER_MISSILE_DAMAGE, 0));

				tmpMissile->SetIsScenery(1);

				tmpMissile->SetHealth(0);

			}
			break;

		case eBLUE_PLAYER:
		case eRED_PLAYER:
			{
				if(_fDelta >= EPSILON*3)
				{
					return;
				}

				int nDifference = -1;

				CPlayer* tmpPlayer = (CPlayer*)_pRhs;
				CBossTurret* tmpEnemy = (CBossTurret*)_pLhs;

				D3DXVECTOR3 d3dTmpVec = tmpEnemy->GetPosition() - tmpPlayer->GetPosition();
				D3DXVec3Normalize(&d3dTmpVec, &d3dTmpVec);
				//	D3DXMatrixTranslation(tmpPlayer->GetMatrix(), -tmpVec.x, -tmpVec.y, -tmpVec.y);
				float fScale = D3DXVec3Dot(&d3dTmpVec, &CPhysics::GetMat4Vec(eZAXIS, tmpPlayer->GetMatrix()));


				{
					D3DXMATRIX d3dtmpMat;
					float fRadius = ((CSphere*)tmpPlayer->GetBV())->GetRadius();
					D3DXMatrixTranslation(&d3dtmpMat, -d3dTmpVec.x * fRadius, -d3dTmpVec.y * fRadius, -d3dTmpVec.z * fRadius);

					d3dtmpMat = *tmpPlayer->GetMatrix() * d3dtmpMat;

					tmpPlayer->SetMatrix(&d3dtmpMat);


					tmpPlayer->GetCamera()->SetViewMatrixPos(&(CPhysics::GetMat4Vec(ePOS, tmpPlayer->GetCamera()->GetViewMatrix()) + (-d3dTmpVec * fRadius)));


					if(!tmpPlayer->GetShieldHit())
					{
						tmpPlayer->SetShieldHit(true);
						tmpPlayer->SetOtherObjectPos(tmpEnemy->GetPosition() - tmpPlayer->GetPosition());
					}

					//tmpEnemy->SetHealth(tmpEnemy->GetHealth() - 20);


					if(tmpPlayer->GetShields())
					{
						nDifference = int(2*abs(fScale)) - tmpPlayer->GetShields();
						if(nDifference >= 0)
						{
							tmpPlayer->SetShields(0);
						}
						else
						{
							tmpPlayer->SetShields(max(tmpPlayer->GetShields() - int(2*abs(fScale)), 0), tmpPlayer->CalculateDamageDirection(tmpEnemy->GetPosition()) );
						}
						tmpPlayer->SetPlayShieldHit(true);
					}
					if(tmpPlayer->GetHealth() && tmpPlayer->GetShields() <= 0)
					{
						if(nDifference >= 0)
						{
							tmpPlayer->SetHealth(int(max(tmpPlayer->GetHealth() - nDifference, 0)), tmpPlayer->CalculateDamageDirection(tmpEnemy->GetPosition()), _pGame);
						}
						else
						{
							tmpPlayer->SetHealth(int(max(tmpPlayer->GetHealth() - 2*abs(fScale), 0)), tmpPlayer->CalculateDamageDirection(tmpEnemy->GetPosition()), _pGame);
						}

						tmpPlayer->SetPlayHullHit(true);

					}
				}
				//cout << "Player shield: " << tmpPlayer->GetShields() << endl;
				//cout << "Player Health: " << tmpPlayer->GetHealth() << endl;
				//D3DXMatrixTranslation(tmpPlayer->GetMatrix(), -tmpVec.x, -tmpVec.y, -tmpVec.y);
			}
			break;
		}
	}

// 	else if(nLHS == eHanger)
// 	{
// 		switch(nRHS)
// 		{
// 		case eLASER:
// 			{
// 				if(_fDelta >= EPSILON*1000)
// 				{
// 					return;
// 				}
// 
// 				CBossHanger* tmpHanger = (CBossHanger*)_pLhs;
// 				CLaser* tmpLaser = (CLaser*)_pRhs;
// 				if (((CBoss*)tmpHanger->GetOwner())->GetShieldStatus() == eSHIELDS_UP || (tmpLaser->GetOwner()->GetType() != eRED_PLAYER && tmpLaser->GetOwner()->GetType() != eBLUE_PLAYER))
// 				{
// 					tmpLaser->SetActive(false);
// 					break;
// 				}
// 				if(!tmpLaser->GetActive())
// 				{
// 					break;
// 				}
// 				tmpHanger->SetIsHit(true);
// 
// 				tmpHanger->SetHealth(max(tmpHanger->GetHealth() - PLAYER_LASER_DAMAGE, 0));
// 
// 				tmpLaser->SetActive(false);
// 
// 			}
// 			break;
// 
// 		case eMISSILE:
// 			{
// 				if(_fDelta >= EPSILON*3)
// 				{
// 					return;
// 				}
// 
// 				CBossHanger* tmpHanger = (CBossHanger*)_pLhs;
// 				CMissile* tmpMissile = (CMissile*)_pRhs;
// 				if (((CBoss*)tmpHanger->GetOwner())->GetShieldStatus() == eSHIELDS_UP || (tmpMissile->GetOwner()->GetType() != eRED_PLAYER && tmpMissile->GetOwner()->GetType() != eBLUE_PLAYER))
// 				{
// 					tmpMissile->SetActive(false);
// 					break;
// 				}
// 				if(!tmpMissile->GetActive())
// 				{
// 					break;
// 				}
// 				tmpHanger->SetIsHit(true);
// 
// 				tmpHanger->SetHealth(max(tmpHanger->GetHealth() - PLAYER_MISSILE_DAMAGE / 8, 0));
// 
// 
// 				tmpMissile->SetHealth(0);
// 
// 			}
// 			break;
// 
// 		case eBLUE_PLAYER:
// 		case eRED_PLAYER:
// 			{
// 				if(_fDelta >= EPSILON*3)
// 				{
// 					return;
// 				}
// 
// 				int nDifference = -1;
// 
// 				CPlayer* tmpPlayer = (CPlayer*)_pRhs;
// 				CBossHanger* tmpEnemy = (CBossHanger*)_pLhs;
// 
// 				D3DXVECTOR3 d3dTmpVec = tmpEnemy->GetPosition() - tmpPlayer->GetPosition();
// 				D3DXVec3Normalize(&d3dTmpVec, &d3dTmpVec);
// 				//	D3DXMatrixTranslation(tmpPlayer->GetMatrix(), -tmpVec.x, -tmpVec.y, -tmpVec.y);
// 				float fScale = D3DXVec3Dot(&d3dTmpVec, &CPhysics::GetMat4Vec(eZAXIS, tmpPlayer->GetMatrix()));
// 
// 
// 				if(fScale > 0.8)
// 				{
// 					tmpPlayer->SetHealth(0);
// 					tmpPlayer->SetShields(0);
// 					tmpEnemy->SetHealth(0);
// 				}
// 				else
// 				{
// 					D3DXMATRIX d3dtmpMat;
// 					float fRadius = ((CSphere*)tmpPlayer->GetBV())->GetRadius();
// 					D3DXMatrixTranslation(&d3dtmpMat, -d3dTmpVec.x * fRadius, -d3dTmpVec.y * fRadius, -d3dTmpVec.z * fRadius);
// 
// 					d3dtmpMat = *tmpPlayer->GetMatrix() * d3dtmpMat;
// 
// 					tmpPlayer->SetMatrix(&d3dtmpMat);
// 
// 
// 					tmpPlayer->GetCamera()->SetViewMatrixPos(&(CPhysics::GetMat4Vec(ePOS, tmpPlayer->GetCamera()->GetViewMatrix()) + (-d3dTmpVec * fRadius)));
// 
// 
// 					if(!tmpPlayer->GetShieldHit())
// 					{
// 						tmpPlayer->SetShieldHit(true);
// 						tmpPlayer->SetOtherObjectPos(tmpEnemy->GetPosition() - tmpPlayer->GetPosition());
// 					}
// 
// 					tmpEnemy->SetHealth(tmpEnemy->GetHealth() - 20);
// 
// 
// 					if(tmpPlayer->GetShields())
// 					{
// 						nDifference = int(2*abs(fScale)) - tmpPlayer->GetShields();
// 						if(nDifference >= 0)
// 						{
// 							tmpPlayer->SetShields(0);
// 						}
// 						else
// 						{
// 							tmpPlayer->SetShields(max(tmpPlayer->GetShields() - int(2*abs(fScale)), 0), tmpPlayer->CalculateDamageDirection(tmpEnemy->GetPosition()) );
// 						}
// 						tmpPlayer->SetPlayShieldHit(true);
// 					}
// 					if(tmpPlayer->GetHealth() && tmpPlayer->GetShields() <= 0)
// 					{
// 						if(nDifference >= 0)
// 						{
// 							tmpPlayer->SetHealth(int(max(tmpPlayer->GetHealth() - nDifference, 0)), tmpPlayer->CalculateDamageDirection(tmpEnemy->GetPosition()));
// 						}
// 						else
// 						{
// 							tmpPlayer->SetHealth(int(max(tmpPlayer->GetHealth() - 2*abs(fScale), 0)), tmpPlayer->CalculateDamageDirection(tmpEnemy->GetPosition()));
// 						}
// 
// 						tmpPlayer->SetPlayHullHit(true);
// 
// 					}
// 				}
// 				//cout << "Player shield: " << tmpPlayer->GetShields() << endl;
// 				//cout << "Player Health: " << tmpPlayer->GetHealth() << endl;
// 				//D3DXMatrixTranslation(tmpPlayer->GetMatrix(), -tmpVec.x, -tmpVec.y, -tmpVec.y);
// 			}
// 			break;
// 		}
// 	}


}


/*****************************************************************
* ClassifySphereToPlane():		This function classifies whether a Sphere 
*								is on either side of the plane or stradles it
*
* Ins:							_lhs
*								_rhs
*
* Outs:			
*
* Returns:						int
*								1 - in front of plane
*								2 - behind plane
*								3 - intersects plane
*
* Mod. Date:					12/1/2012
* Mod. Initials:				AQ
*****************************************************************/
int CCollisionLib::ClassifySphereToPlane(tPlane& _lhs, CSphere& _rhs)
{
	// distance the sphere is from the plane
	float fDist = D3DXVec3Dot(&_rhs.GetPosition(), &_lhs.m_d3dNormal) - _lhs.m_fOffset;

	if(fDist > _rhs.GetRadius())
	{
		return 1;
	}
	else if(fDist < -_rhs.GetRadius())
	{
		return 2;
	}

	return 3;
}

/*****************************************************************
* ClassifyAABBToPlane():	This function classifies whether a AABB 
*							is on either side of the plane or stradles it
*
* Ins:						_lhs
*							_rhs
*
* Outs:			
*
* Returns:					int
*							1 - in front of plane
*							2 - behind plane
*							3 - intersects plane
*
* Mod. Date:				12/1/2012
* Mod. Initials:			AQ
*****************************************************************/
int CCollisionLib::ClassifyAabbToPlane(tPlane& _lhs, CAABB& _rhs)
{
	CSphere tmpSph;
	tmpSph.SetPosition( (_rhs.GetMax() + _rhs.GetMin())*0.5f);
	D3DXVECTOR3 extend = _rhs.GetMax() - tmpSph.GetPosition();

	tmpSph.SetRadius(D3DXVec3Dot(&extend, &D3DXVECTOR3(abs(_lhs.m_d3dNormal.x), abs(_lhs.m_d3dNormal.y), abs(_lhs.m_d3dNormal.z))));

	return ClassifySphereToPlane(_lhs, tmpSph);
}

/*****************************************************************
* ClassifyCapsuleToPlane():	This function classifies whether a capsule 
*								is on either side of the plane or stradles it
*
* Ins:							_lhs
*								_rhs
*
* Outs:			
*
* Returns:						int 
*								1 - in front of plane
*								2 - behind plane
*								3 - intersects plane
*
* Mod. Date:					12/1/2012
* Mod. Initials:				AQ
*****************************************************************/
int CCollisionLib::ClassifyCapsuleToPlane(tPlane& _lhs, CCapsule& _rhs)
{
	float fEndResult   = D3DXVec3Dot(&_lhs.m_d3dNormal, &_rhs.GetEnd()) - _lhs.m_fOffset;
	float fStartResult = D3DXVec3Dot(&_lhs.m_d3dNormal, &_rhs.GetStart()) - _lhs.m_fOffset;

	float fRadius = _rhs.GetRadius(); 
	if(fEndResult > fRadius && fStartResult > fRadius)
	{
		return 1;
	}
	else if(fEndResult < -fRadius && fStartResult < -fRadius)
	{
		return 2;
	}
	else
	{
		return 3;
	}
}

/*****************************************************************
* FrustumToSphere():	This function handles detection between 
*						frustums and spheres
*
* Ins:				_lhs
*					_rhs
*
* Outs:			
*
* Returns:			bool
*
* Mod. Date:		      12/1/2012
* Mod. Initials:	      AQ
*****************************************************************/
bool CCollisionLib::FrustumToSphere(tFrustum& _lhs,  CSphere& _rhs)
{
	for(int nIndex = 0; nIndex < 6; nIndex++)
	{
		if(ClassifySphereToPlane(_lhs.m_arrPlanes[nIndex], _rhs) == 2)
		{
			return false;
		}
	}
	//if(ClassifySphereToPlane(frustum.planes[BOTTOM_PLANE], sphere) == 2)
	//	return false;
	return true;
}

/*****************************************************************
* FrustumToAABB():		This function handles detection between 
*						frustums and AABBs
*
* Ins:				_lhs
*					_rhs
*
* Outs:			
*
* Returns:			bool
*
* Mod. Date:		      12/1/2012
* Mod. Initials:	      AQ
*****************************************************************/
bool CCollisionLib::FrustumToAABB(tFrustum& _lhs, CAABB& _rhs)
{
	for (int nIndex = 0; nIndex < 6; nIndex++)
	{
		if(ClassifyAabbToPlane(_lhs.m_arrPlanes[nIndex], _rhs) == 2)
		{
			return false;
		}
	}

	return true;
}

/*****************************************************************
* FrustumToCapsule():	This function handles detection between 
*						frustums and capsules
*
* Ins:				_lhs
*					_rhs
*
* Outs:			
*
* Returns:			bool
*
* Mod. Date:		      12/1/2012
* Mod. Initials:	      AQ
*****************************************************************/
bool CCollisionLib::FrustumToCapsule(tFrustum& _lhs, CCapsule& _rhs)
{
	for(int nIndex=0; nIndex<6; nIndex++)
	{
		if(ClassifyCapsuleToPlane(_lhs.m_arrPlanes[nIndex], _rhs) == 2)
		{
			return false;
		}
	}

	return true;
}

/*****************************************************************
* AABBtoAABB():	This function handles basic box collision
*
* Ins:				_lhs
*					_rhs
*
* Outs:			
*
* Returns:			bool
*
* Mod. Date:		      12/1/2012
* Mod. Initials:	      AQ
*****************************************************************/
bool CCollisionLib::AABBtoAABB(CAABB& _lhs, CAABB& _rhs)
{

	if( _lhs.GetMax().x < _rhs.GetMin().x || _lhs.GetMin().x > _rhs.GetMax().x ) 
	{
		return false;
	}

	if( _lhs.GetMax().y < _rhs.GetMin().y || _lhs.GetMin().y > _rhs.GetMax().y )
	{	
		return false;
	}

	if( _lhs.GetMax().z < _rhs.GetMin().z || _lhs.GetMin().z > _rhs.GetMax().z )
	{
		return false;
	}

	return true;
}

/*****************************************************************
* IntersectRayCylinder():		Casts a ray and determines if the ray ever comes 
*								comes into contact with the sphere, and saves the 
*								time at which that would happen
*
* Ins:				_sa
*					_n
*					_p
*					_q
*					_r
*
* Outs:				_t
*
* Returns:			bool
*
* Mod. Date:		      11/21/2012
* Mod. Initials:	      AQ
*****************************************************************/
bool CCollisionLib::IntersectRayCylinder( const D3DXVECTOR3 &_d3dSa, const D3DXVECTOR3 &_d3dN, const D3DXVECTOR3 &_d3dP, const D3DXVECTOR3 &_d3dQ, float _fR, float &_fT )
{
	D3DXVECTOR3 d3dDist = _d3dQ - _d3dP; // vector from first point on cylinder segment to the end point on cylinder segment
	D3DXVECTOR3 d3dVecCylRay = _d3dSa - _d3dP; // vector from first point on cylinder segment to start point of ray

	// Values used to calculate coefficients of quadratic formula.
	// You do not necessarily have to use any of these directly for the rest of the algorithm.
	float fDSqrLen = D3DXVec3Dot( &d3dDist, &d3dDist ); // dot product of d with d (squared magnitude of d)
	float fRelationalLen = D3DXVec3Dot( &_d3dN, &d3dDist ); // dot product of ray normal (n) with d
	float fRelationalCylRayLen = D3DXVec3Dot( &d3dVecCylRay, &_d3dN ); 
	float fRelationalDistCylRay = D3DXVec3Dot( &d3dVecCylRay, &d3dDist ); 
	float fCylRaySqrLen = D3DXVec3Dot( &d3dVecCylRay, &d3dVecCylRay ); 


	// Coefficients for the quadratic formula
	float fA = fDSqrLen - fRelationalLen * fRelationalLen;
	float fB = fDSqrLen*fRelationalCylRayLen - fRelationalLen*fRelationalDistCylRay;
	float fC = fDSqrLen*(fCylRaySqrLen - _fR*_fR) - fRelationalDistCylRay*fRelationalDistCylRay;

	// If a is approximately 0.0 then the ray is parallel to the cylinder and can't intersect
	if( abs(fA) < FLT_EPSILON )
	{
		return false;
	}

	// TODO: Find time of intersection, if any
	//		 Use the quadratic formula to solve for t. Reference "Advanced Ray to Sphere.ppt" for an example.
	//		 As with "Advanced Ray to Sphere", the 2s and 4 in the formula ( x = (-b - sqrt(b*b - 4ac)) / 2a )
	//		 are cancelled out, resulting in a simplified form.
	float fSimpleQuadratic = fB*fB - fA*fC;
	if(fSimpleQuadratic<0)
	{
		return false;
	}

	_fT = (-fB - sqrt(fSimpleQuadratic))/fA;

	if(_fT < FLT_EPSILON)
	{
		return false;
	}

	//vec3f norm = d;
	//	norm.normalize();

	//	float tmp1 = dot_product(d,norm);
	if(fRelationalDistCylRay + fRelationalLen*_fT >fDSqrLen || fRelationalDistCylRay+fRelationalLen*_fT <0)
	{
		return false;
	}

	//if((sa + (n*t)))

	return true;
}

/*****************************************************************
* IntersectRayCapsule():		Casts a ray and determines if the ray ever comes 
*								comes into contact with the sphere, and saves the 
*								time at which that would happen
*
* Ins:				_sa
*					_n
*					_rhs
*
* Outs:				_t
*
* Returns:			bool
*
* Mod. Date:		      11/21/2012
* Mod. Initials:	      AQ
*****************************************************************/
bool CCollisionLib::IntersectRayCapsule( const D3DXVECTOR3 &_d3dSa, const D3DXVECTOR3 &_d3dN, CCapsule* _rhs, float &_fT, D3DXVECTOR3 &_d3dQ )
{

	float fTime = FLT_MAX;
	_fT = FLT_MAX;
	bool bReturn = false;

	if(IntersectRayCylinder(_d3dSa, _d3dN, _rhs->GetStart(), _rhs->GetEnd(), _rhs->GetRadius(), _fT))
	{
		_d3dQ = (_rhs->GetStart() + _rhs->GetEnd())*0.5f;
		return true;
	}

	float fTime2=FLT_MAX;
	// placeholder vars for out params
	D3DXVECTOR3 tmp1, tmp2;
	CSphere tmpSph1, tmpSph2;
	tmpSph1.SetPosition(_rhs->GetStart());	tmpSph1.SetRadius(_rhs->GetRadius());
	tmpSph2.SetPosition(_rhs->GetEnd());	tmpSph2.SetRadius(_rhs->GetRadius());
	bReturn = RayToSphere(_d3dSa, _d3dN, tmpSph1, fTime, tmp1);
	bReturn = max(bReturn, RayToSphere(_d3dSa, _d3dN, tmpSph2, fTime2, tmp2));
	if(bReturn)
	{
		_fT = min(fTime, fTime2);
		if(_fT == fTime)
		{
			_d3dQ = tmp1;
		}
		else
		{
			_d3dQ = tmp2;
		}
	}


	return bReturn;
}

/*****************************************************************
* BuildFrustum():	Builds the Frustum to Check Against 
*						
*
* Ins:				frustum, fov, nearDist, FarDist, ratio, camXform
*
* Outs:			
*
* Returns:			None
*
* Mod. Date:		      12/1/2012
* Mod. Initials:	      AQ
*****************************************************************/
void CCollisionLib::BuildFrustum(tFrustum& frustum, float _fFOV, float _fNearDist, float _fFarDist, float _fRatio, D3DXMATRIX& _d3dMat )
{

	D3DXVECTOR3 d3dNearClip = CPhysics::GetMat4Vec(ePOS, &_d3dMat) + CPhysics::GetMat4Vec(eZAXIS, &_d3dMat) * _fNearDist;
	d3dNearClip.z -= 20;
	D3DXVECTOR3 d3dFarClip  = CPhysics::GetMat4Vec(ePOS, &_d3dMat) + CPhysics::GetMat4Vec(eZAXIS, &_d3dMat) * _fFarDist;

	float fHnear = 2.0f * tan(_fFOV * 0.5f) * _fNearDist;
	float fHfar = 2.0f * tan(_fFOV * 0.5f) * _fFarDist;
	float fWnear = fHnear * _fRatio;
	float fWfar = fHfar * _fRatio;


	// The next 8 lines will calculate the eight points that make up the frustum
	frustum.m_d3darrCorners[eFTL] = d3dFarClip + CPhysics::GetMat4Vec(eYAXIS, &_d3dMat) * (fHfar * 0.75f) - CPhysics::GetMat4Vec(eXAXIS, &_d3dMat) * (fWfar * 0.75f);
	frustum.m_d3darrCorners[eFBL] = d3dFarClip - CPhysics::GetMat4Vec(eYAXIS, &_d3dMat) * (fHfar * 0.75f) - CPhysics::GetMat4Vec(eXAXIS, &_d3dMat) * (fWfar * 0.75f);
	frustum.m_d3darrCorners[eFBR] = d3dFarClip - CPhysics::GetMat4Vec(eYAXIS, &_d3dMat) * (fHfar * 0.75f) + CPhysics::GetMat4Vec(eXAXIS, &_d3dMat) * (fWfar * 0.75f);
	frustum.m_d3darrCorners[eFTR] = d3dFarClip + CPhysics::GetMat4Vec(eYAXIS, &_d3dMat) * (fHfar * 0.75f) + CPhysics::GetMat4Vec(eXAXIS, &_d3dMat) * (fWfar * 0.75f);


	frustum.m_d3darrCorners[eNTL] = d3dNearClip + CPhysics::GetMat4Vec(eYAXIS, &_d3dMat) * (fHnear * 0.75f) - CPhysics::GetMat4Vec(eXAXIS, &_d3dMat) * (fWnear * 0.75f);
	frustum.m_d3darrCorners[eNTR] = d3dNearClip + CPhysics::GetMat4Vec(eYAXIS, &_d3dMat) * (fHnear * 0.75f) + CPhysics::GetMat4Vec(eXAXIS, &_d3dMat) * (fWnear * 0.75f);
	frustum.m_d3darrCorners[eNBR] = d3dNearClip - CPhysics::GetMat4Vec(eYAXIS, &_d3dMat) * (fHnear * 0.75f) + CPhysics::GetMat4Vec(eXAXIS, &_d3dMat) * (fWnear * 0.75f);
	frustum.m_d3darrCorners[eNBL] = d3dNearClip - CPhysics::GetMat4Vec(eYAXIS, &_d3dMat) * (fHnear * 0.75f) - CPhysics::GetMat4Vec(eXAXIS, &_d3dMat) * (fWnear * 0.75f);


	// the next six lines will take the eight points from above and
	// calculate the planes that will make up the six sides of the frustum
	/*ComputePlane(frustum.m_arrPlanes[eLEFT_PLANE]  , frustum.m_d3darrCorners[eNBL], frustum.m_d3darrCorners[eFBL], frustum.m_d3darrCorners[eFTL]);
	ComputePlane(frustum.m_arrPlanes[eRIGHT_PLANE] , frustum.m_d3darrCorners[eNTR], frustum.m_d3darrCorners[eFTR], frustum.m_d3darrCorners[eFBR]);	
	ComputePlane(frustum.m_arrPlanes[eTOP_PLANE]   , frustum.m_d3darrCorners[eNTL], frustum.m_d3darrCorners[eFTL], frustum.m_d3darrCorners[eFTR]);		
	ComputePlane(frustum.m_arrPlanes[eBOTTOM_PLANE], frustum.m_d3darrCorners[eNBR], frustum.m_d3darrCorners[eFBR], frustum.m_d3darrCorners[eFBL]);	
	ComputePlane(frustum.m_arrPlanes[eNEAR_PLANE]  , frustum.m_d3darrCorners[eNTR], frustum.m_d3darrCorners[eNBR], frustum.m_d3darrCorners[eNBL]);	
	ComputePlane(frustum.m_arrPlanes[eFAR_PLANE]   , frustum.m_d3darrCorners[eFBL], frustum.m_d3darrCorners[eFBR], frustum.m_d3darrCorners[eFTR]);		*/

	ComputePlane(frustum.m_arrPlanes[eNEAR_PLANE],frustum.m_d3darrCorners[eNTR],frustum.m_d3darrCorners[eNBL],frustum.m_d3darrCorners[eNBR]);
	ComputePlane(frustum.m_arrPlanes[eFAR_PLANE],frustum.m_d3darrCorners[eFTL],frustum.m_d3darrCorners[eFTR],frustum.m_d3darrCorners[eFBR]);
	ComputePlane(frustum.m_arrPlanes[eLEFT_PLANE],frustum.m_d3darrCorners[eNTL],frustum.m_d3darrCorners[eFTL],frustum.m_d3darrCorners[eFBL]);
	ComputePlane(frustum.m_arrPlanes[eRIGHT_PLANE],frustum.m_d3darrCorners[eFTR],frustum.m_d3darrCorners[eNTR],frustum.m_d3darrCorners[eNBR]);
	ComputePlane(frustum.m_arrPlanes[eTOP_PLANE],frustum.m_d3darrCorners[eNTL],frustum.m_d3darrCorners[eNTR],frustum.m_d3darrCorners[eFTR]);
	ComputePlane(frustum.m_arrPlanes[eBOTTOM_PLANE],frustum.m_d3darrCorners[eFBL],frustum.m_d3darrCorners[eFBR],frustum.m_d3darrCorners[eNBL]);
}
int CCollisionLib::ClassifyPointToPlane(const tPlane& plane, const D3DXVECTOR3& point)
{
	float D = (D3DXVec3Dot(&plane.m_d3dNormal,&point) - plane.m_fOffset);

	if(D >= 0)
		return 1;
	else 
		return 2;
}

/*****************************************************************
* ComputePlane():		Helper function that builds a plane with the
* 						three passed in points
*
* Ins:					pointA
*      				pointB
*						pointC
*
* Outs:				plane
*
* Returns:	
*
* Mod. Date:			11/30/2012
* Mod. Initials:		AQ
*****************************************************************/
void CCollisionLib::ComputePlane( tPlane &_plane, const D3DXVECTOR3& _d3dPointA, const D3DXVECTOR3& _d3dPointB, const D3DXVECTOR3 &_d3dPointC )
{
	D3DXVec3Cross(&_plane.m_d3dNormal, &(_d3dPointB - _d3dPointA), &(_d3dPointC - _d3dPointB));

	D3DXVec3Normalize(&_plane.m_d3dNormal, &_plane.m_d3dNormal);

	_plane.m_fOffset = D3DXVec3Dot(&_plane.m_d3dNormal, &_d3dPointA);
}

void CCollisionLib::SphereToMesh( CEntity* _pLhs, CEntity* _pRhs, float _fDelta, CAssetManager* _pAssets )
{
	// for each triangle
	//		check 

	D3DXVECTOR3  d3dP0, d3dP1, d3dZVec;
	int nWhichTri = -1;

	d3dP0 = CPhysics::GetMat4Vec(eZAXIS, _pRhs->GetMatrix());
	float fMagn = D3DXVec3Length(&d3dP0);
	D3DXVec3Normalize(&d3dP1, &d3dP0);
	d3dP1 = d3dP1 * (fMagn*2);

	for(INT32 nTriIndex = 0; nTriIndex < _pAssets->GetModel(eRED_PLAYER)->m_nNumberOfTriangles; nTriIndex++)
	{

		// tri vars
		D3DXVECTOR3 d3dTriNormal, d3dV0, d3dV1, d3dV2, d3dL, d3dCP;

		float fD0, fD1, fD2, fD3;

		d3dV0 = D3DXVECTOR3(_pAssets->GetModel(eRED_PLAYER)->m_vUniqueVerts[_pAssets->GetModel(eRED_PLAYER)->m_vTriangles[nTriIndex].m_uIndex0].fPos);
		d3dV1 = D3DXVECTOR3(_pAssets->GetModel(eRED_PLAYER)->m_vUniqueVerts[_pAssets->GetModel(eRED_PLAYER)->m_vTriangles[nTriIndex].m_uIndex1].fPos);
		d3dV2 = D3DXVECTOR3(_pAssets->GetModel(eRED_PLAYER)->m_vUniqueVerts[_pAssets->GetModel(eRED_PLAYER)->m_vTriangles[nTriIndex].m_uIndex2].fPos);
		D3DXVec3Cross(&d3dTriNormal, &(d3dV0 - d3dV1), &(d3dV0 - d3dV2));

		fD0 = D3DXVec3Dot(&d3dP0, &d3dTriNormal);
		fD1 = D3DXVec3Dot(&d3dV0, &d3dTriNormal);

		// early outs
		if( fD0 - fD1 < 0)
		{
			continue;
		}
		if(D3DXVec3Dot(&d3dP1, &d3dTriNormal) - fD1 > 0)
		{
			continue;
		}

		fD2 = fD0 - fD1;
		d3dL = d3dP1 - d3dP0;

		fD3 = D3DXVec3Dot(&d3dTriNormal, &d3dL);
		if(fD3 == 0)
		{
			continue;
		}

		d3dCP = d3dP0 + (-(fD2 / fD3) * d3dL);

		D3DXVECTOR3 d3dEdge0, d3dEdge1, d3dEdge2, d3dNorm0, d3dNorm1, d3dNorm2;

		d3dEdge0 = d3dV1 - d3dV0;
		D3DXVec3Cross(&d3dNorm0, &d3dEdge0, &d3dTriNormal);

		D3DXVECTOR3 d3dtmpVec = d3dCP - d3dV0;
		if(D3DXVec3Dot(&d3dtmpVec, &d3dNorm0) > 0)
		{
			continue;
		}

		d3dEdge1 = d3dV2 - d3dV1;
		D3DXVec3Cross(&d3dNorm1, &d3dEdge1, &d3dTriNormal);

		d3dtmpVec = d3dCP - d3dV1;
		if(D3DXVec3Dot(&d3dtmpVec, &d3dNorm1) > 0)
		{
			continue;
		}

		d3dEdge2 = d3dV0 - d3dV2;
		D3DXVec3Cross(&d3dNorm2, &d3dEdge2, &d3dTriNormal);

		d3dtmpVec = d3dCP - d3dV2;
		if(D3DXVec3Dot(&d3dtmpVec, &d3dNorm2) > 0)
		{
			continue;
		}


		d3dP1 = d3dCP;
		nWhichTri = nTriIndex;

	}

	((CPlayer*)_pLhs)->SetModelTriCollPoint(nWhichTri);

}

bool CCollisionLib::FrustumToPoint(tFrustum& frustum,D3DXVECTOR3& point)
{
	CSphere tempSphere;
	tempSphere.SetPosition(point);
	tempSphere.SetRadius(17.34742f);

	for(int nIndex = 0; nIndex < 6; nIndex++)
	{
		// Implement point Sprites
		//if(ClassifyPointToPlane(frustum.m_arrPlanes[nIndex], point) == 2)
		if(ClassifySphereToPlane(frustum.m_arrPlanes[nIndex], tempSphere) == 2)
		{
			return false;
		}
	}

	return true;
}
