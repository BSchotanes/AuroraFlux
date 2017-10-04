/***********************************************
* Filename:  		FlockingMovement.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class is for the red enemy normally. Will keep enemies flocked together.
************************************************/
#include "../StdAfx.h"
#include "../Entity/Enemy.h"
#include "../Entity/RedEnemy.h"
#include "FlockingMovement.h"
#include "FleeMovement.h"
#include "ChaseMovement.h"
#include "../Collision/Physics.h"
#include "WanderingMovement.h"
#include "..\Entity\Player.h"
#include "..\Entity/Waypoint.h"
#include "../AI/DeathMovement.h"
#include "../AI/ReturnMovement.h"
#include "LockedOnMovement.h"
#include "../Object Manager/ObjectManager.h"
#include "ObstacleAvoidanceMovement.h"
#include "../AI/RollingMovement.h"
#include "../AI/ForceChaseMovement.h"

/*****************************************************************
* CFlockingMovement():  Overload Constructor will set its type. Does mostly nothing. 
* Ins:			    _pEnemy    		      
* Outs:			    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CFlockingMovement::CFlockingMovement(CEnemy* _pEnenmy) : CMovementBehavior(_pEnenmy)
{
	m_bForceChase = false;
	//AverageForward = D3DXVECTOR3(0.0f,0.0f,0.0f);
	//AveragePosition= D3DXVECTOR3(0.0f,0.0f,0.0f);
	//AlignmentStrength = 5.0f;
	//FlockRadius = 40.0f;
	//CohesionStrength = 2.0f;
}
/*Destructor*/
CFlockingMovement::~CFlockingMovement(void)
{

}
/*****************************************************************
* Update():		Average positions of flockers.
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CFlockingMovement::Update(float _fElapedTime)
{

	if(m_pEnemy->GetHealth() <= 0)
	{
		m_pEnemy->PushBehavior(new CDeathMovement(m_pEnemy));
		return;
	}
	//vector<CEntity*> vPartners = ((CRedEnemy*)m_pEnemy)->GetPartners();
	D3DXVECTOR3 d3dVelocity = D3DXVECTOR3(0.0f,0.0f,((CEnemy*)m_pEnemy)->GetVelocityModifier());
	D3DXMATRIX d3dMat;
	D3DXMatrixIdentity( &d3dMat );
	d3dMat._41 = d3dVelocity.x * _fElapedTime; 
	d3dMat._42 = d3dVelocity.y * _fElapedTime; 
	d3dMat._43 = d3dVelocity.z * _fElapedTime; 
	m_pEnemy->SetMatrix( &( d3dMat * ( *m_pEnemy->GetMatrix() ) ) );
	///////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	CPhysics::TurnTo(m_pEnemy, &m_pEnemy->GetTarget()->GetPosition(), _fElapedTime, m_pEnemy->GetTurnRate());
	//////////////////////////////////////////////////////////////////////////

	
	if (((CEnemy*)m_pEnemy)->GetType() == eRED_ENEMY && ((CRedEnemy*)m_pEnemy)->GetShouldDodge())
	{
		
			((CRedEnemy*)m_pEnemy)->SetShouldDodge(false);
			((CRedEnemy*)m_pEnemy)->PushBehavior(new CRollingMovement(m_pEnemy));
			return;

	}
	if (m_pEnemy->GetShouldChasePlayer())
	{
		((CRedEnemy*)m_pEnemy)->PushBehavior(new CForceChaseMovement(m_pEnemy));
		return;
	}
// 	if(m_pEnemy->GetObjectManager())
// 	{
// 		D3DXVECTOR3 _d3dVelocity = D3DXVECTOR3(0,0,0);
// 		std::vector<int> avoidFlags;
// 		avoidFlags.push_back(eRED_ENEMY); avoidFlags.push_back(eBLUE_ENEMY); avoidFlags.push_back(eYELLOW_ENEMY); avoidFlags.push_back(eRED_PLAYER); avoidFlags.push_back(eBLUE_PLAYER);
// 		avoidFlags.push_back(eASTEROID); avoidFlags.push_back(eLARGE_ASTEROID);
// 		_d3dVelocity += m_pEnemy->GetObjectManager()->AdjustToAvoidCollision(m_pEnemy, avoidFlags);
// 		if(D3DXVec3LengthSq(&_d3dVelocity) > 0.7)
// 		{
// 			//m_pEnemy->PushBehavior(new CObstacleAvoidanceMovement(m_pEnemy));
// 		}
// 	}
	if (m_pEnemy->GetIsStunned())
	{
		m_pEnemy->PushBehavior(new CLockedOnMovement(m_pEnemy));
		return;
	}
	if(  D3DXVec3Length( &(m_pEnemy->GetPosition() - m_pEnemy->GetWaypoint()->GetPosition() )) >= RETURN_RANGE)
	{
		m_pEnemy->PushBehavior(new CReturnMovement(m_pEnemy));
		return;
	}


	CMovementBehavior::Update(_fElapedTime);

	// if the target is NOT visible and your not being forced to chase
	if (m_pEnemy->GetTarget() && !((CPlayer*)m_pEnemy->GetTarget())->GetIsVisable() && !m_bForceChase)
	{
		m_pEnemy->PushBehavior(new CWanderingMovement(m_pEnemy));
		return;
	}

	// if you get hit and you dont need to help your partner
	if (m_pEnemy->GetShouldRun() && !m_bForceChase)
	{
		m_pEnemy->PushBehavior(new CWanderingMovement(m_pEnemy));
		return;
	}
	 

	// partners influence
// 	for (unsigned int nIndex = 0; nIndex <vPartners.size(); nIndex++)
// 	{
// 		if (vPartners[nIndex]->GetActive())
// 		{ 
// 			float fSquaredDistance =  D3DXVec3Length( &(m_pEnemy->GetPosition() - vPartners[nIndex]->GetPosition() ));
// 			D3DXVECTOR3 d3dEnemyPosition = m_pEnemy->GetPosition();
// 			D3DXVECTOR3 d3dPartnerPosition = vPartners[nIndex]->GetPosition();
// 
// 			if( fSquaredDistance < 15 )
// 			{
// 
// 				if ( d3dEnemyPosition.x  > d3dPartnerPosition.x )
// 				{
// 					d3dVelocity.x += 20*_fElapedTime;
// 				}
// 				else
// 				{
// 					d3dVelocity.x -= 20*_fElapedTime;
// 				}
// 				if ( d3dEnemyPosition.y > d3dPartnerPosition.y)
// 				{
// 					d3dVelocity.y += 20*_fElapedTime;
// 				}
// 				else
// 				{
// 					d3dVelocity.y -= 20*_fElapedTime;
// 				}
// 			}
// 
// 			if( fSquaredDistance > 20 )
// 			{
// 				if (d3dEnemyPosition.x  > d3dPartnerPosition.x )
// 				{
// 					d3dVelocity.x -= 20*_fElapedTime;
// 				}
// 				else
// 				{
// 					d3dVelocity.x += 20*_fElapedTime;
// 				}
// 				if ( d3dEnemyPosition.y > d3dPartnerPosition.y)
// 				{
// 					d3dVelocity.y -= 20*_fElapedTime;
// 				}
// 				else
// 				{
// 					d3dVelocity.y += 20*_fElapedTime;
// 				}
// 			}
// 		}
// 	}
	/// use partner influence

	if(m_pEnemy->GetTarget() )
	{
		CAIHelper* tempAI = ((CRedEnemy*)m_pEnemy)->GetHelper();
		D3DXVECTOR3 vectorBetween = m_pEnemy->GetPosition() - m_pEnemy->GetTarget()->GetPosition() ;
		float fSquaredDistance = D3DXVec3Length( &(vectorBetween ));
		//dot z w/ vector between

		D3DXVECTOR3 zAxis = CPhysics::GetMat4Vec(eZAXIS,m_pEnemy->GetTarget()->GetMatrix() );
		float halfSpace = D3DXVec3Dot(D3DXVec3Normalize(&vectorBetween,&vectorBetween ),D3DXVec3Normalize(&zAxis, &zAxis ));
		if ( halfSpace > .85f && !m_bForceChase &&  fSquaredDistance  < tempAI->Getforce_red_to_flee_distance() * 2  )
		{	
			m_pEnemy->PushBehavior(new CFleeMovement( m_pEnemy ));
			return;
		}

		if ( !m_bForceChase &&  fSquaredDistance  < tempAI->Getforce_red_to_flee_distance()  )
		{	
			m_pEnemy->PushBehavior(new CFleeMovement( m_pEnemy ));
			return;
		}

		if ( m_bForceChase && fSquaredDistance < tempAI->Getforce_red_to_flee_distance()  )
		{	
			m_pEnemy->PushBehavior(new CFleeMovement( m_pEnemy ));
			return;
		}
	}
}
/*

void CFlockingMovement::CalculateAverages()
{

D3DXVECTOR3 avgVelocity;

D3DXVECTOR3 position;
vector<CEntity*> _partners = ((CRedEnemy*)GetEnemy())->GetPartners();
for(unsigned int i = 0; i < _partners.size(); i++)
{
if (((CRedEnemy*)_partners[i])->GetActive())
{
//maybe do x and y axis here as well
avgVelocity.z += ((CRedEnemy*)_partners[i])->GetVelocityModifier();
}

}

avgVelocity.z /= (float)_partners.size();

AverageForward = avgVelocity;


for(unsigned int i = 0; i < _partners.size(); i++)
{
if (((CRedEnemy*)_partners[i])->GetActive())
{
position += _partners[i]->GetPosition();
}
}
position.x /= (float)_partners.size();
position.y /= (float)_partners.size();
position.z /= (float)_partners.size();

AveragePosition = position;

}

D3DXVECTOR3 CFlockingMovement::CalculateAlignmentAcceleration(CEntity * _enemy)
{
//            The alignment algorithm is rather simplistic as most of the work has already been done when calculating 
//the average velocity of each boid in the flock. This average velocity will serve as the influencing vector. 
//The only problem is that velocities range in magnitude from zero to the boid’s max speed whereas we 
//require an influence vector ranging in magnitude from zero to one. This is solved by simply scaling the 
//average velocity vector by one over the boid’s max speed. You must also insure that your influencing 
//vector does not exceed unit length before applying any strength weights
D3DXVECTOR3 temp = AverageForward;
float velocity =  ((CRedEnemy*)_enemy)->GetVelocityModifier();
temp.x /= velocity;
temp.y /= velocity;
temp.z /= velocity;
////idk

if (D3DXVec3Length(&temp) > 1)
{
D3DXVec3Normalize(&temp, &temp);
}

return temp * AlignmentStrength;
}

D3DXVECTOR3 CFlockingMovement::CalculateCohesionAcceleration(CEntity * _enemy)
{
D3DXVECTOR3 temp = D3DXVECTOR3(0.0f,0.0f,0.0f);
float distance = 0.0f;
temp = AveragePosition - _enemy->GetPosition();

distance = D3DXVec3Length(&temp);
D3DXVec3Normalize(&temp, &temp);

// Set the speed based on the distance from the flock.
if (distance < FlockRadius)
temp *= distance / FlockRadius;

return temp * CohesionStrength;

}

D3DXVECTOR3 CFlockingMovement::CalculateSeparationAcceleration(CEntity * _enemy)
{
vector<CEntity*> _partner = ((CRedEnemy*)GetEnemy())->GetPartners();

D3DXVECTOR3 sum = D3DXVECTOR3(0.0f,0.0f,0.0f);
D3DXVECTOR3 vector = D3DXVECTOR3(0.0f,0.0f,0.0f);
for (unsigned int i = 0; i < _partner.size(); i++)
{
if (_partner[i] == _enemy)
{
continue;
}
vector = _enemy->GetPosition() -_partner[i]->GetPosition();
float distance = D3DXVec3Length(&vector);
float safeDistance = 70;
//	// If a collision is likely...
if (distance < safeDistance)
{
D3DXVec3Normalize(&vector, &vector);

// Scale according to distance between boids.
//  normalize vector (to get directional unit vector);
vector *= (safeDistance - distance) * (1 / safeDistance);
sum += vector;
}
//}
//
if (D3DXVec3Length(&sum) > 1.0f)
D3DXVec3Normalize(&sum, &sum);

}
return sum * 2;

}*/