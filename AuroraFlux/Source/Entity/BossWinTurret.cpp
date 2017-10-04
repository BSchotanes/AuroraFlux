/***********************************************
* Filename:  		Asteroid.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will hold all of the data for the asteroids.				
************************************************/
#include "../StdAfx.h"
#include "../Entity/BossWinTurret.h"
#include "../Entity/Player.h"
#include "../Entity/Boss.h"
#include "../Collision/Physics.h"
#include "BossBigLazer.h"
/*****************************************************************
* CAsteroid():  Default Constructor will set its type. Initilizes asteroid unique data.
* Ins:			    None    		       
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CBossWinTurret::CBossWinTurret(CEntity * _pOwner, CEntity * _pTarget, int _type, int _weaponType, float _zModifier, CObjectManager* _objManager) : m_pOwner(_pOwner), m_nTurretType(_type), m_fRechargeTimer(30.0f)
{	
	shouldRecharce = true;
	fLargeLazerChargeTimer = 0.0f;
	corretSide = false;
	m_bStartRecharging = false;
	m_fCDTime = 10.0f;
	m_nWeaponType	 = TOP;
	m_nObjectType   = eHanger;
	m_nHealth       = 1;
	m_nShields      = 1;
	m_nVelocityModifier = 20;
	m_nDamage		= 0;
	m_bIsHit        = false;
	m_bActive       = true;
	m_pTarget = _pTarget;
	m_bFireAtPlanet = false;
	m_bFireMegaLaser = false;
	SetBV(CCollOBJ::Create(eSPH, D3DXVECTOR3(0,0,0), 30.0));
	SetTransitionValue(1.0f);
	m_fZModifier = _zModifier;
	m_pTarget = m_pOwner->GetTarget();
	m_pBoss = ((CBoss*)m_pOwner);

	//m_pObjectManager = _objManager;
	//if(m_pObjectManager != NULL)
	//{
	//	m_pBigLazer = new CBossBigLazer();
	//	m_pObjectManager->AddToInactiveArmament(m_pBigLazer);
	//	m_pBigLazer = NULL;
	//}
	m_bHasPlayedLazerChargingSound = false;

	m_bFireLaser = false;

	EffectInformation::CreateEffect(eWIN_LASER_CHARGE, &m_esChargeParticles, this, true, false);
	//EffectInformation::CreateEffect(eBOSS_WIN_LASER_PARTICLES, &m_esLaserParticles, this, true, false);
}

/*Destructor*/
CBossWinTurret::~CBossWinTurret(void)
{
	m_bFireLaser = false;
}
/*****************************************************************
* Update():		Will update its internal timers and behaviors.
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CBossWinTurret::Update(float _fElapedTime)
{
	if( m_pBoss->GetIsDisabled() )
	{
		WwiseNS::SoundEvent(eHydraLaser, eStop);
		//if( m_pBigLazer )
		//{
		//	//kill the lazer, put it back into the innactive list, set this turrets lazer pointer to null
		//	m_pBigLazer->KillLazer();
		//	m_pBigLazer->SetActive(false);
		//	m_pBigLazer = NULL;
		//	m_bHasPlayedLazerChargingSound = false;
		//	m_esLaserParticles.ResetPosition();
		//}
		//m_esLaserParticles.SetPlay(false);
		return;
	}
	if (shouldRecharce && m_pOwner && m_pBoss->GetTransition())
	{
		_fElapedTime = 0.00756f;
		m_nHealth   += int( 1050 * _fElapedTime);
		if (m_nHealth >= 1400)
		{
			m_nHealth = 1400; 
			shouldRecharce = false;
		}
	}
	corretSide = false;
	CEnemy::Update(_fElapedTime);

	if (m_nShields <= 0)
	{
		m_fRechargeTimer += _fElapedTime;
	}
	if (m_fRechargeTimer >= 21.50f)
	{
		m_bStartRecharging = true;
		m_fRechargeTimer = 0.0f;
	}
	if (m_bStartRecharging)
	{
		m_nShields += int(150 * _fElapedTime);
		if (m_nShields >= 200)
		{
			m_nShields = 200;
			m_bStartRecharging = false;

		}
	}


	if(m_pBoss->GetFireLaser() == true)
	{
		m_bFireLaser = true;
	}
	else if(m_pBoss->GetFireLaser() == false && m_pBoss->GetStopFireLaser() == true)
	{
		m_bFireLaser = false;
	}
	// 	switch(m_nTurretType)
	// 	{
	// 	case TOP:
	// 		{
	// 			if(TopTurret(_fElapedTime))
	// 			{
	// 				corretSide = true;
	// 			}
	// 		}
	// 		break;
	// 	case BOTTOM:
	// 		{
	// 			if(BottomTurret(_fElapedTime))
	// 			{
	// 				corretSide = true;
	// 			}
	// 		}
	// 		break;
	// 	case LEFT:
	// 		{
	// 			if(	LeftTurret(_fElapedTime))
	// 			{
	// 				corretSide = true;
	// 			}
	// 		}
	// 		break;
	// 	case RIGHT:
	// 		{
	// 			if(RightTurret(_fElapedTime))
	// 			{
	// 				corretSide = true;
	// 			}
	// 		}
	// 		break;
	// 	case FRONT:
	// 		{
	// 			if(FrontTurret(_fElapedTime))
	// 			{
	// 				corretSide = true;
	// 			}
	// 		}
	// 		break;
	// 	case BACK:
	// 		{
	// 			if(BackTurret(_fElapedTime))
	// 			{
	// 				corretSide = true;
	// 			}
	// 		}
	// 		break;
	// 	};
	// 
	// 	

	SetPosition(GetPosition().x, GetPosition().y, m_pOwner->GetPosition().z + m_fZModifier);


	///turn to would probably look better
	// 	if (corretSide)
	// 	{
	// 
	// 		CPlayer * m_pPlayer = (CPlayer*)m_pTarget;
	// 		D3DXVECTOR3 d3dEnemyPosition = m_pPlayer->GetPosition();
	// 		D3DXVECTOR3 d3dVectorBetween = d3dEnemyPosition - m_pOwner->GetPosition();
	// 		float sqDist = D3DXVec3Length(&d3dVectorBetween);
	// 		if(sqDist >= 450)
	// 		{
	// 			CPhysics::TurnTo(this, m_pTarget, _fElapedTime, TURRET_TURN_RATE / 6 );
	// 		}
	// 		else
	// // 		{

	if( m_bFireAtPlanet )
	{
		CPhysics::LookAt(this, m_pTarget);
		if( m_fCDTime < 1000.0f )
		{
			m_fCDTime = 1.0f;
		}
		TopTurret(_fElapedTime);
	}

	if (m_pBoss->GetPhase() == eSEEK_AND_DESTROY)
	{
		if (TopTurret(_fElapedTime))
		{
			CPhysics::TurnTo(this, m_pTarget, _fElapedTime, (float)(TURRET_WIN_TURN_RATE));
			corretSide = true;
		}
	}
	//		}
	// 	}

	if (m_pCurrMoveBehavior)
	{
		m_pCurrMoveBehavior->Update(_fElapedTime);
	}

	if(m_bFireMegaLaser == true )
	{
		static float fLzrTimer = 0.0f;
		if(fLzrTimer > 4.0f)
		{
			fLzrTimer = 0.0f;
			m_bHasPlayedLazerChargingSound = false;
			WwiseNS::SoundEvent(eHydraLaser, eStop);
			//m_esLaserParticles.SetPlay(false);
			//m_esLaserParticles.ResetPosition();
			m_bFireMegaLaser = false;
			m_pBoss->SetStopFireLaser(true);
		}
		else
		{
			vector<D3DXVECTOR3> vec;
			D3DXVECTOR3 tvec = m_pTarget->GetPosition() - GetPosition();
			D3DXVec3Normalize(&tvec, &tvec);
			for( float i(0.0f); i < 3.0f; ++i )
			{
				vec.push_back(tvec*1000.0f);
			}

			//for( unsigned int i(0); i < m_esLaserParticles.GetGeometrySystem()->size(); ++i )
			//{
			//	for( unsigned int j(0); j < m_esLaserParticles.GetGeometrySystem()[i].size(); ++j )
			//	{
			//		m_esLaserParticles.GetGeometrySystem()[i][j].SetAllVelocityValues(&vec);
			//	}
			//}
			fLzrTimer += _fElapedTime;
		}
	}
}

bool CBossWinTurret::TopTurret(float _fElapsedTime)
{
	static float s_fCounter = 0.0f;
	s_fCounter += _fElapsedTime;
	bool bCanFireLaser = false;
	bool bCanFireMissile = false; 

	// how frequently the red enemy will shoot
	if (m_pOwner && m_pBoss->GetIsDisabled())
	{
		m_esChargeParticles.SetPlay(false);
		//m_esLaserParticles.SetPlay(false);

		return false;
	}
	if (s_fCounter >=  TURRET_WIN_FIRE_RATE_LASER )
	{
		bCanFireLaser = true;
		s_fCounter = 0.0f;

	}

	if (s_fCounter >=  TURRET_FIRE_RATE_MISSILE )
	{
		bCanFireMissile = true;
		s_fCounter = 0.0f;

	}

	// if the player is on the correct side of the turret move on 
	if (m_pTarget->GetPosition().y > GetPosition().y + 200 && !m_bFireAtPlanet)
	{
		return false;
	}

	CPlayer * m_pPlayer = (CPlayer*)m_pTarget;
	D3DXVECTOR3 d3dEnemyPosition = m_pPlayer->GetPosition();
	D3DXVECTOR3 d3dVectorBetween = d3dEnemyPosition - m_pOwner->GetPosition();
	float sqDist = D3DXVec3Length(&d3dVectorBetween);
	D3DXVec3Normalize(  &d3dVectorBetween, &d3dVectorBetween );

	CBoss * _pTheBoss = ((CBoss*)m_pOwner);


	D3DXVECTOR3 d3dYAxis =  CPhysics::GetMat4Vec(eYAXIS, m_pOwner->GetMatrix());
	D3DXVec3Normalize(  &d3dYAxis, &d3dYAxis );
	// on the right side?
	if((D3DXVec3Dot( &d3dVectorBetween, &d3dYAxis)  < -TURRET_WIN_SIGHT) || _pTheBoss->IsOnLeft() || _pTheBoss->IsOnRight() || _pTheBoss->IsOnTop() || m_bFireAtPlanet)  
	{
		//bucket increment for the "cool-down" for after the lazer fires
		static float fLargeLazerCooldownTimer = 0.0f;
		if(fLargeLazerCooldownTimer > m_fCDTime)
		{ 
			//if (m_pBigLazer == NULL)
			//{ 
			//begin charging lazer
			if(m_bHasPlayedLazerChargingSound == false)
			{
				m_esChargeParticles.SetPlay(true);
				m_bHasPlayedLazerChargingSound = true;
				WwiseNS::SoundEvent(eHydraLaserCharge, ePlay);
			}
			if(fLargeLazerChargeTimer > 3.0f)
			{
				if(/*m_pObjectManager != NULL && */!m_pBoss->GetIsDisabled() )	
				{
					fLargeLazerChargeTimer = 0.0f;
					//m_pBigLazer = (CBossBigLazer*)m_pObjectManager->ReturnArmamentToMasterList(eBARRIER);
					//if(m_pBigLazer != NULL)
					//{
					if( m_pTarget->GetType() == ePLANET1 ||
						m_pTarget->GetType() == ePLANET2 ||
						m_pTarget->GetType() == ePLANET3 ||
						m_pTarget->GetType() == ePLANET4)
					{
						m_fCDTime = 1000.0f;
					}
					//m_esLaserParticles.SetPlay(true);
					//m_pBigLazer->SetTurretOwner(this);
					fLargeLazerCooldownTimer = 0.0f;
					m_pBoss->DrawWarning(false);
					WwiseNS::SoundEvent(eHydraLaser, ePlay);
					m_esChargeParticles.SetPlay(false);
					m_bFireMegaLaser = true;
					m_bFireLaser = true;


					// TODO Alex: Raycast here for boss laser collision,
					// Use the target(which will always be the player) and check ray to him, 
					// if theres collision just lower his health and shields


					//}
				}
			}
			else
			{
				if(GetTransitionValue() >= -1.0f) 
					SetTransitionValue((GetTransitionValue()) - (_fElapsedTime * 2.0f)); 
				else 
					SetTransitionValue(-1.0f); 

				fLargeLazerChargeTimer += _fElapsedTime;
				m_pBoss->DrawWarning(true);

				m_esChargeParticles.SetPlay(true);


				static float m_fLockOnTime = 0.0f;

				if(m_fLockOnTime > 0.3f)
				{
					WwiseNS::SoundEvent(eLockOnWarning, ePlay);

					m_fLockOnTime = 0.0f;
				}

				m_fLockOnTime += _fElapsedTime;
			}
			//}
			if (bCanFireMissile)
			{
				//m_cFlags |= eFIRE_MISSILE;
			}

		}
		else
		{
			if(GetTransitionValue() <= 1.0f) 
				SetTransitionValue((GetTransitionValue()) + (_fElapsedTime * 2.0f)); 
			else 
				SetTransitionValue(1.0f); 

			fLargeLazerCooldownTimer += _fElapsedTime;
		}


		if(m_bFireMegaLaser == true)
		{
			CSphere tmpSph = *(CSphere*)(m_pPlayer->GetBV());
			tmpSph.SetPosition(tmpSph.GetPosition() + m_pPlayer->GetPosition());
			tmpSph.SetRadius(tmpSph.GetRadius() * 1.4f);
			D3DXVECTOR3 d3dVec;
			float fScale = FLT_MAX;
			D3DXVECTOR3 tempVector = GetPosition();
			tempVector.y -= 16;

			if(CCollisionLib::RayToSphere(tempVector, CPhysics::GetMat4Vec(eZAXIS, this->GetMatrix()), tmpSph, fScale, d3dVec) == true)
			{
				if(m_pPlayer->GetShields() > 0)
				{
					m_pPlayer->SetShields(m_pPlayer->GetShields() - 4);
				}
				else if(m_pPlayer->GetHealth() > 0)
				{
					m_pPlayer->SetHealth(m_pPlayer->GetHealth() - 4);
				}

				//cout << "OH DEAR GOD!!!!! I'M HIT! I'M HIT!!" << '\n';
			}
		}


		return true;
	}
	return false;
}
bool CBossWinTurret::BottomTurret(float _fElapsedTime)
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
				if(m_pBoss->GetPhase() == eLASER_PHASE)
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
bool CBossWinTurret::LeftTurret(float _fElapsedTime)
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
	if (m_pTarget->GetPosition().x > GetPosition().x)
	{
		return false;
	}
	CPlayer * m_pPlayer = (CPlayer*)m_pTarget;
	D3DXVECTOR3 d3dEnemyPosition = m_pPlayer->GetPosition();
	D3DXVECTOR3 d3dVectorBetween = d3dEnemyPosition - m_pOwner->GetPosition();
	float sqDist = D3DXVec3Length(&d3dVectorBetween);

	D3DXVec3Normalize(  &d3dVectorBetween, &d3dVectorBetween );

	D3DXVECTOR3 d3dXAxis =  CPhysics::GetMat4Vec(eXAXIS, m_pOwner->GetMatrix());
	D3DXVec3Normalize(  &d3dXAxis, &d3dXAxis );
	// on the right side?
	if(D3DXVec3Dot( &d3dVectorBetween, &d3dXAxis)  < -TURRET_SIGHT && sqDist <= BOSS_TURRET_RANGE )  
	{
		if (bCanFire)
		{
			if(m_nWeaponType == eTURRET_LASER)
			{
				m_cFlags |= eFIRE_LASER;

			}
			else
			{
				if(m_pBoss->GetPhase() == eLASER_PHASE)
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
bool CBossWinTurret::RightTurret(float _fElapsedTime)
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
	if (m_pTarget->GetPosition().x < GetPosition().x)
	{
		return false;
	}
	CPlayer * m_pPlayer = (CPlayer*)m_pTarget;
	D3DXVECTOR3 d3dEnemyPosition = m_pPlayer->GetPosition();
	D3DXVECTOR3 d3dVectorBetween = d3dEnemyPosition - m_pOwner->GetPosition();
	float sqDist = D3DXVec3Length(&d3dVectorBetween);

	D3DXVec3Normalize(  &d3dVectorBetween, &d3dVectorBetween );

	D3DXVECTOR3 d3dXAxis =  CPhysics::GetMat4Vec(eXAXIS, m_pOwner->GetMatrix());
	D3DXVec3Normalize(  &d3dXAxis, &d3dXAxis );
	// on the right side?
	if(D3DXVec3Dot( &d3dVectorBetween, &d3dXAxis)  > TURRET_SIGHT && sqDist <= BOSS_TURRET_RANGE)  
	{
		if (bCanFire)
		{
			if(m_nWeaponType == eTURRET_LASER)
			{
				m_cFlags |= eFIRE_LASER;

			}
			else
			{
				if(m_pBoss->GetPhase() == eLASER_PHASE)
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
bool CBossWinTurret::FrontTurret(float _fElapsedTime)
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
				if(m_pBoss->GetPhase() == eLASER_PHASE)
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
bool CBossWinTurret::BackTurret(float _fElapsedTime)
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
				if(m_pBoss->GetPhase() == eLASER_PHASE)
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
void CBossWinTurret::Render() 
{

}
