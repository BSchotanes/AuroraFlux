/***********************************************
* Filename:  		CollisionLib.h
* Date:      		11/21/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AQ
* Author:    		Alexander G. Garcia de Quevedo
* Purpose:   		This is the class that handles all
*					collision detection and responses.
*					Object Manager will call the public
*					function and private helper functions
*					will be delegated to more precise work.
************************************************/

#ifndef _COLLSIONLIB_H_
#define _COLLSIONLIB_H_

#include "../Definitions.h"
//#pragma comment(lib, "d3dx9math.lib")
#include "CollOBJ.h"
#include "Sphere.h"
#include "Capsule.h"
#include "AABB.h"
#include "../Entity/Entity.h"
#include "../../source/Renderer/CAssetManager.h"

struct tPlane
{
	D3DXVECTOR3 m_d3dNormal;
	float m_fOffset;
};

struct tFrustum
{
	tPlane m_arrPlanes[6];
	D3DXVECTOR3 m_d3darrCorners[8];
};

//struct tCollisionData
//{
//	D3DXVECTOR3 m_d3dContactNormal;
//	D3DXVECTOR3 m_d3dContactPoint;
//	float m_fPenDepth;
//};
class CGame;

class CCollisionLib
{
	// Collision Detection

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
	static bool AABBtoAABB( CAABB& _lhs, CAABB& _rhs);

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
	static int ClassifyCapsuleToPlane(tPlane& _lhs, CCapsule& _rhs);

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
	static int ClassifySphereToPlane(tPlane& _lhs, CSphere& _rhs);

	/*****************************************************************
	* ClassifyPointToPlane():		This function classifies whether a Point 
	*								is on either side of the plane or straddles it
	*
	* Ins:							tPlane& plane
	*								D3DXVECTOR3& point
	*
	* Outs:			
	*
	* Returns:						int
	*								1 - in front of plane
	*								2 - behind plane
	*								3 - intersects plane
	*
	* Mod. Date:					2/18/2013
	* Mod. Initials:				BS
	*****************************************************************/
	static int ClassifyPointToPlane(const tPlane& plane, const D3DXVECTOR3& point);

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
	static int ClassifyAabbToPlane(tPlane& _lhs, CAABB& _rhs);

	

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
	static bool SphereToCapsule(CSphere& _lhs, CCapsule& _rhs, D3DXVECTOR3 _d3dLhfv, D3DXVECTOR3 _d3dRhfv, float& _fDelta);

	/*****************************************************************
	* SphereToAABB():		This function handles sphere / AABB collision
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
	static bool SphereToAABB( CSphere& _lhs,  CAABB& _rhs);

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
	static bool CapsuleToAABB( CCapsule& _lhs,  CAABB& _rhs);

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
	static bool CapsuleToCapsule( CCapsule& _lhs, CCapsule& _rhs, D3DXVECTOR3 _d3dLhfv, D3DXVECTOR3 _d3dRhfv, float& _fDelta);

	// Collision Reaction

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
	static void Reactions(CEntity* _pLhs, CEntity* _pRhs, float _fDelta, CAssetManager* _pAssets, CGame* _pGame = nullptr);

	/*****************************************************************
	* IntersectRayCylinder():		Casts a ray and determines if the ray ever comes 
	*								comes into contact with the Cylinder, and saves the 
	*								time at which that would happen
	*
	* Ins:				_sa
	*					_n
	*					_p
	*					_q
	*					_r
	*
	* Outs:			_t
	*
	* Returns:			bool
	*
	* Mod. Date:		      11/21/2012
	* Mod. Initials:	      AQ
	*****************************************************************/
	static bool IntersectRayCylinder(const D3DXVECTOR3 &_d3dSa, const D3DXVECTOR3 &_d3dN, const D3DXVECTOR3 &_d3dP, const D3DXVECTOR3 &_d3dQ, float _fR, float &_fT);


	/*****************************************************************
	* ComputePlane():		Helper function that builds a plane with the
	* 						three passed in points
	*
	* Ins:					_pointA
	*      				_pointB
	*						_pointC
	*
	* Outs:				plane
	*
	* Returns:	
	*
	* Mod. Date:			11/30/2012
	* Mod. Initials:		AQ
	*****************************************************************/
	static void ComputePlane(tPlane &_plane, const D3DXVECTOR3& _d3dPointA, const D3DXVECTOR3& _d3dPointB, const D3DXVECTOR3 &_d3dPointC);

	static void SphereToMesh(CEntity* _pLhs, CEntity* _pRhs, float _fDelta, CAssetManager* _pAssets);

public:

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
	CCollisionLib(void);

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
	~CCollisionLib(void);

	/*****************************************************************
	* RayToSphere():		Casts a ray and determines if the ray ever comes 
	*						comes into contact with the sphere, and saves the 
	*						time at which that would happen
	*
	* Ins:				_p
	*					_d
	*					_rhs
	*
	* Outs:			_t
	*					_q
	*
	* Returns:			bool
	*
	* Mod. Date:		      11/21/2012
	* Mod. Initials:	      AQ
	*****************************************************************/
	static bool RayToSphere(const D3DXVECTOR3 &_d3dP, const D3DXVECTOR3 &_d3dD, CSphere& _rhs, float &_fT, D3DXVECTOR3 &_d3dQ);

	/*****************************************************************
	* IntersectRayCapsule():		Casts a ray and determines if the ray ever comes 
	*								comes into contact with the Capsule, and saves the 
	*								time at which that would happen
	*
	* Ins:				_sa
	*					_n
	*					_rhs
	*
	* Outs:			_t
	*
	* Returns:			bool
	*
	* Mod. Date:		      11/21/2012
	* Mod. Initials:	      AQ
	*****************************************************************/
	static bool IntersectRayCapsule(const D3DXVECTOR3 &_d3dSa, const D3DXVECTOR3 &_d3dN, CCapsule* _rhs, float &_fT, D3DXVECTOR3 &_d3dQ);

	/*****************************************************************
	* CheckCollision():	This is the function the object manager calls
	*						for every object in the active list. It will 
	*						determine the type of the collision object and
	*						call the appropriate function.  If collision IS 
	*						detected, the function will hand the objects to
	*						the reactions function.
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
	static void CheckCollision( CEntity* _pLhs, CEntity* _pRhs, CAssetManager* _pAssets, CGame* _pGame = nullptr);

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
	static void BuildFrustum(tFrustum& _frustum,  float _fFOV, float _fNearDist, float _fFarDist, float _fRatio, D3DXMATRIX& _d3dMat );

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
	static bool FrustumToCapsule(tFrustum& _lhs, CCapsule& _rhs);

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
	static bool FrustumToAABB(tFrustum& _lhs, CAABB& _rhs);

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
	static bool MovingSphereToSphere(CSphere& _lhs, CSphere& _rhs, D3DXVECTOR3 _d3dLhfv, D3DXVECTOR3 _d3dRhfv, float& _fDelta/*, tCollisionData& _collisionData*/);
	static bool FrustumToPoint(tFrustum& frustum,D3DXVECTOR3& point);
	static bool FrustumToSphere(tFrustum& _lhs,  CSphere& _rhs);
};


#endif
