/***********************************************
* Filename:  		Asteroid.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will hold all of the data for the asteroids.				
************************************************/
#include "../StdAfx.h"
#include "../Entity/BossTurret.h"
#include "../Entity/Player.h"
#include "../Entity/Boss.h"
#include "../Collision/Physics.h"
/*****************************************************************
* CAsteroid():  Default Constructor will set its type. Initilizes asteroid unique data.
* Ins:			    None    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CBossTurret::CBossTurret(CEntity * _pOwner, CEntity * _pTarget, int _type, int _weaponType, float _zModifier) : m_pOwner(_pOwner), m_nTurretType(_type)
{
	//m_nObjectType   = eASTEROID;
	m_nWeaponType	 = _weaponType;
	m_nObjectType   = eTURRET_SMALL;
	m_nHealth       = 35;
	m_nShields      = 0;
	m_nVelocityModifier = 20;
	m_nDamage = 0;
	m_bIsHit        = false;
	m_bActive       = true;
	m_pTarget = _pTarget;
	SetBV(CCollOBJ::Create(eSPH, D3DXVECTOR3(0,0,0), 7.0));
	m_fZModifier = _zModifier;
	m_pTarget = m_pOwner->GetTarget();
	corretSide = false;
	m_fCounter = 0.0f;
}

/*Destructor*/
CBossTurret::~CBossTurret(void)
{
}
/*****************************************************************
* Update():		Will update its internal timers and behaviors.
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CBossTurret::Update(float _fElapedTime)
{
	corretSide = false;
	CEnemy::Update(_fElapedTime);

	if (m_pOwner && ((CBoss*)m_pOwner)->GetIsDisabled())
	{
		return;
	}

	switch(m_nTurretType)
	{
	case TOP:
		{
			if(TopTurret(_fElapedTime))
			{
				corretSide = true;
			}
		}
		break;
	case BOTTOM:
		{
			if(BottomTurret(_fElapedTime))
			{
				corretSide = true;
			}
		}
		break;
	case LEFT:
		{
			if(	LeftTurret(_fElapedTime))
			{
				corretSide = true;
			}
		}
		break;
	case RIGHT:
		{
			if(RightTurret(_fElapedTime))
			{
				corretSide = true;
			}
		}
		break;
	case FRONT:
		{
			if(FrontTurret(_fElapedTime))
			{
				corretSide = true;
			}
		}
		break;
	case BACK:
		{
			if(BackTurret(_fElapedTime))
			{
				corretSide = true;
			}
		}
		break;
	};

	SetPosition(GetPosition().x, GetPosition().y, m_pOwner->GetPosition().z + m_fZModifier);

	///turn to would probably look better
	if (corretSide)
	{
		CPlayer * m_pPlayer = (CPlayer*)m_pTarget;
		D3DXVECTOR3 d3dEnemyPosition = m_pPlayer->GetPosition();
		D3DXVECTOR3 d3dVectorBetween = d3dEnemyPosition - m_pOwner->GetPosition();
		float sqDist = D3DXVec3Length(&d3dVectorBetween);
		if(sqDist >= 1000)
		{
			CPhysics::TurnTo(this, m_pTarget, _fElapedTime, .03f);
		}
		else
		{
			CPhysics::TurnTo(this, m_pTarget, _fElapedTime, TURRET_TURN_RATE);
		}
	}

	if (m_pCurrMoveBehavior)
	{
		m_pCurrMoveBehavior->Update(_fElapedTime);
	}
}

bool CBossTurret::TopTurret(float _fElapsedTime)
{
	m_fCounter += _fElapsedTime;
	bool bCanFire = false;

	// how frequently the red enemy will shoot
	if(m_nWeaponType == eTURRET_LASER)
	{
		if (m_fCounter >=  TURRET_FIRE_RATE_LASER )
		{
			bCanFire = true;
		}
	}
	else
	{
		if (m_fCounter >=  TURRET_FIRE_RATE_MISSILE )
		{
			bCanFire = true;
		}
	}
	// if the player is on the correct side of the turret move on 
	if (m_pTarget->GetPosition().y < GetPosition().y)
	{
		return false;
	}
	// 		CPlayer * m_pPlayer = (CPlayer*)m_pTarget;
	// 		D3DXVECTOR3 d3dEnemyPosition = m_pPlayer->GetPosition();
	// 		D3DXVECTOR3 d3dVectorBetween = d3dEnemyPosition - m_pOwner->GetPosition();
	// 		float sqDist = D3DXVec3Length(&d3dVectorBetween);
	// 		D3DXVec3Normalize(  &d3dVectorBetween, &d3dVectorBetween );
	// 
	// 		D3DXVECTOR3 d3dYAxis =  CPhysics::GetMat4Vec(eYAXIS, m_pOwner->GetMatrix());
	// 		D3DXVec3Normalize(  &d3dYAxis, &d3dYAxis );
	// on the right side?
	//if(D3DXVec3Dot( &d3dVectorBetween, &d3dYAxis)  > TURRET_SIGHT && sqDist <= BOSS_TURRET_RANGE )  
	if(((CBoss*)m_pOwner)->IsOnLeft())		
	{
		if (bCanFire)
		{
			// do a turret type check here for lasers or missiles
			if(m_nWeaponType == eTURRET_LASER)
			{
				m_cFlags |= eFIRE_LASER;
			}
			else
			{
				if(((CBoss*)m_pOwner)->GetPhase() == eLASER_PHASE)
				{
					return false;
				}
				m_cFlags |= eFIRE_MISSILE;
			}
			m_fCounter = 0.0f;

		}
		return true;
	}
	return false;
}
bool CBossTurret::BottomTurret(float _fElapsedTime)
{
	m_fCounter += _fElapsedTime;
	bool bCanFire = false;

	// how frequently the red enemy will shoot
	if(m_nWeaponType == eTURRET_LASER)
	{
		if (m_fCounter >=  TURRET_FIRE_RATE_LASER )
		{
			bCanFire = true;
		}
	}
	else
	{
		if (m_fCounter >=  TURRET_FIRE_RATE_MISSILE )
		{
			bCanFire = true;
		}
	}
	// if the player is on the correct side of the turret move on 
	if (m_pTarget->GetPosition().y > GetPosition().y)
	{
		return false;
	}
	CPlayer * m_pPlayer = (CPlayer*)m_pTarget;
	D3DXVECTOR3 d3dEnemyPosition = m_pPlayer->GetPosition();
	D3DXVECTOR3 d3dVectorBetween = d3dEnemyPosition - m_pOwner->GetPosition();
	float sqDist = D3DXVec3Length(&d3dVectorBetween);

	D3DXVec3Normalize(  &d3dVectorBetween, &d3dVectorBetween );

	D3DXVECTOR3 d3dYAxis =  CPhysics::GetMat4Vec(eYAXIS, m_pOwner->GetMatrix());
	D3DXVec3Normalize(  &d3dYAxis, &d3dYAxis );
	// on the right side?
	if(D3DXVec3Dot( &d3dVectorBetween, &d3dYAxis)  < -TURRET_SIGHT && sqDist <= BOSS_TURRET_RANGE)  
	{
		if (bCanFire)
		{
			if(m_nWeaponType == eTURRET_LASER)
			{
				m_cFlags |= eFIRE_LASER;
			}
			else
			{
				if(((CBoss*)m_pOwner)->GetPhase() == eLASER_PHASE)
				{
					return false;
				}
				m_cFlags |= eFIRE_MISSILE;
			}
			m_fCounter = 0.0f;

		}
		return true;
	}
	return false;

}
bool CBossTurret::LeftTurret(float _fElapsedTime)
{
	m_fCounter += _fElapsedTime;
	bool bCanFire = false;

	// how frequently the red enemy will shoot
	if(m_nWeaponType == eTURRET_LASER)
	{
		if (m_fCounter >=  TURRET_FIRE_RATE_LASER )
		{
			bCanFire = true;
		}

	}
	else
	{
		if (m_fCounter >=  TURRET_FIRE_RATE_MISSILE )
		{
			bCanFire = true;
		}
	}
	// if the player is on the correct side of the turret move on 
	if (m_pTarget->GetPosition().x > GetPosition().x)
	{
		return false;
	}
	// 		CPlayer * m_pPlayer = (CPlayer*)m_pTarget;
	// 		D3DXVECTOR3 d3dEnemyPosition = m_pPlayer->GetPosition();
	// 		D3DXVECTOR3 d3dVectorBetween = d3dEnemyPosition - m_pOwner->GetPosition();
	// 		float sqDist = D3DXVec3Length(&d3dVectorBetween);
	// 
	// 		D3DXVec3Normalize(  &d3dVectorBetween, &d3dVectorBetween );
	// 
	// 		D3DXVECTOR3 d3dXAxis =  CPhysics::GetMat4Vec(eXAXIS, m_pOwner->GetMatrix());
	// 		D3DXVec3Normalize(  &d3dXAxis, &d3dXAxis );
	// on the right side?
	//if(D3DXVec3Dot( &d3dVectorBetween, &d3dXAxis)  < -TURRET_SIGHT && sqDist <= BOSS_TURRET_RANGE )  
	if(((CBoss*)m_pOwner)->IsOnLeft())		
	{
		if (bCanFire)
		{
			if(m_nWeaponType == eTURRET_LASER)
			{
				m_cFlags |= eFIRE_LASER;
			}
			else
			{
				if(((CBoss*)m_pOwner)->GetPhase() == eLASER_PHASE)
				{
					return false;
				}
				m_cFlags |= eFIRE_MISSILE;
			}
			m_fCounter = 0.0f;

		}
		return true;
	}
	return false;
}
bool CBossTurret::RightTurret(float _fElapsedTime)
{
	m_fCounter += _fElapsedTime;
	bool bCanFire = false;

	// how frequently the red enemy will shoot
	if(m_nWeaponType == eTURRET_LASER)
	{
		if (m_fCounter >=  TURRET_FIRE_RATE_LASER )
		{
			bCanFire = true;
		}
	}
	else
	{
		if (m_fCounter >=  TURRET_FIRE_RATE_MISSILE )
		{
			bCanFire = true;

		}
	}
	// if the player is on the correct side of the turret move on 
	if (m_pTarget->GetPosition().x < GetPosition().x)
	{
		return false;
	}
	// 		CPlayer * m_pPlayer = (CPlayer*)m_pTarget;
	// 		D3DXVECTOR3 d3dEnemyPosition = m_pPlayer->GetPosition();
	// 		D3DXVECTOR3 d3dVectorBetween = d3dEnemyPosition - m_pOwner->GetPosition();
	// 		float sqDist = D3DXVec3Length(&d3dVectorBetween);
	// 
	// 		D3DXVec3Normalize(  &d3dVectorBetween, &d3dVectorBetween );
	// 
	// 		D3DXVECTOR3 d3dXAxis =  CPhysics::GetMat4Vec(eXAXIS, m_pOwner->GetMatrix());
	// 		D3DXVec3Normalize(  &d3dXAxis, &d3dXAxis );
	// 		// on the right side?
	// 		if(D3DXVec3Dot( &d3dVectorBetween, &d3dXAxis)  > TURRET_SIGHT && sqDist <= BOSS_TURRET_RANGE)  
	if(((CBoss*)m_pOwner)->IsOnRight())		
	{
		if (bCanFire)
		{
			if(m_nWeaponType == eTURRET_LASER)
			{
				m_cFlags |= eFIRE_LASER;
			}
			else
			{
				if(((CBoss*)m_pOwner)->GetPhase() == eLASER_PHASE)
				{
					return false;
				}
				m_cFlags |= eFIRE_MISSILE;
			}
			m_fCounter = 0.0f;

		}
		return true;
	}
	return false;

}
bool CBossTurret::FrontTurret(float _fElapsedTime)
{
	static float s_fCounter = 0.0f;
	s_fCounter += _fElapsedTime;
	bool bCanFire = false;

	// how frequently the red enemy will shoot
	/*if(m_nObjectType == eTURRET_LASER)
	{*/
	if(m_nWeaponType == eTURRET_LASER)
	{
		if (s_fCounter >=  TURRET_FIRE_RATE_LASER )
		{
			bCanFire = true;
			s_fCounter = 0.0f;
		}
	}
	else
	{
		if (s_fCounter >=  TURRET_FIRE_RATE_MISSILE )
		{
			bCanFire = true;
			s_fCounter = 0.0f;
		}
	}
	//}
	// if the player is on the correct side of the turret move on 
	if (m_pTarget->GetPosition().z < GetPosition().z)
	{
		return false;
	}
	CPlayer * m_pPlayer = (CPlayer*)m_pTarget;
	D3DXVECTOR3 d3dEnemyPosition = m_pPlayer->GetPosition();
	D3DXVECTOR3 d3dVectorBetween = d3dEnemyPosition - m_pOwner->GetPosition();
	float sqDist = D3DXVec3Length(&d3dVectorBetween);

	D3DXVec3Normalize(  &d3dVectorBetween, &d3dVectorBetween );

	D3DXVECTOR3 d3dZAxis =  CPhysics::GetMat4Vec(eZAXIS, m_pOwner->GetMatrix());
	D3DXVec3Normalize(  &d3dZAxis, &d3dZAxis );
	// on the right side?
	if(D3DXVec3Dot( &d3dVectorBetween, &d3dZAxis)  > TURRET_SIGHT && sqDist <= BOSS_TURRET_RANGE)  
	{
		if (bCanFire)
		{
			if(m_nWeaponType == eTURRET_LASER)
			{
				m_cFlags |= eFIRE_LASER;
			}
			else
			{
				if(((CBoss*)m_pOwner)->GetPhase() == eLASER_PHASE)
				{
					return false;
				}
				m_cFlags |= eFIRE_MISSILE;
			}
		}
		return true;
	}
	return false;

}
bool CBossTurret::BackTurret(float _fElapsedTime)
{
	static float s_fCounter = 0.0f;
	s_fCounter += _fElapsedTime;
	bool bCanFire = false;

	// how frequently the red enemy will shoot
	if(m_nWeaponType == eTURRET_LASER)
	{
		if (s_fCounter >=  TURRET_FIRE_RATE_LASER )
		{
			bCanFire = true;
			s_fCounter = 0.0f;
		}
	}
	else
	{
		if (s_fCounter >=  TURRET_FIRE_RATE_MISSILE )
		{
			bCanFire = true;
			s_fCounter = 0.0f;
		}
	}
	// if the player is on the correct side of the turret move on 
	if (m_pTarget->GetPosition().z > GetPosition().z)
	{
		return false;
	}
	CPlayer * m_pPlayer = (CPlayer*)m_pTarget;
	D3DXVECTOR3 d3dEnemyPosition = m_pPlayer->GetPosition();
	D3DXVECTOR3 d3dVectorBetween = d3dEnemyPosition - m_pOwner->GetPosition();
	float sqDist = D3DXVec3Length(&d3dVectorBetween);

	D3DXVec3Normalize(  &d3dVectorBetween, &d3dVectorBetween );

	D3DXVECTOR3 d3dZAxis =  CPhysics::GetMat4Vec(eZAXIS, m_pOwner->GetMatrix());
	D3DXVec3Normalize(  &d3dZAxis, &d3dZAxis );
	// on the right side?
	if(D3DXVec3Dot( &d3dVectorBetween, &d3dZAxis)  < -TURRET_SIGHT && sqDist <= BOSS_TURRET_RANGE)  
	{
		if (bCanFire)
		{
			if(m_nWeaponType == eTURRET_LASER)
			{
				m_cFlags |= eFIRE_LASER;
			}
			else
			{
				if(((CBoss*)m_pOwner)->GetPhase() == eLASER_PHASE)
				{
					return false;
				}
				m_cFlags |= eFIRE_MISSILE;
			}
		}
		return true;
	}
	return false;
}

/*****************************************************************
* Render():		Draws the updated asteroid and details with asteroids.
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CBossTurret::Render() 
{

}