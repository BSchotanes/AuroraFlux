/***********************************************
* Filename:  		FlankingMovement.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		Enemies will flank their target.
************************************************/
#include "../StdAfx.h"
#include "FlankingMovement.h"

#include "../Entity/Enemy.h"
#include "ChaseMovement.h"
#include "FleeMovement.h"

#include "DeathMovement.h"
#include "..\Entity\Player.h"
#include "..\Entity\YellowEnemy.h"
#include "../Collision/Physics.h"
#include "../Entity/Boss.h"
#include "../Entity/BossTurret.h"
#include "../Entity/BossHanger.h"
#include "../Entity/BossLargerTurret.h"
#include "../Entity/BossWinTurret.h"
/************************************ *****************************
* CFlankingMovement():  Overload Constructor will set its type. Does mostly nothing. 
* Ins:			    _pEnemy    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CDeathMovement::CDeathMovement(CEnemy* _pEnemy) : CMovementBehavior(_pEnemy)
{
	switch( m_pEnemy->GetType() )
	{
	case eGENERATOR:
		{
			EffectInformation::CreateEffect(eGENERIC_EXPLOSION, &m_DeathAnimation, _pEnemy, false, true);
			WwiseNS::SoundEvent(eHydraShieldGenDown, ePlay);
		}
		break;
	case eSPACE_DEBRIS:
	case eSPACE_DEBRIS2:
	case eSPACE_DEBRIS3:
	case eASTEROID:
		{
			EffectInformation::CreateEffect(eASTEROID_DEATH, &m_DeathAnimation, _pEnemy, false, true);
		}
		break;
	case eTURRET_SMALL:
		{
			//if( ((CBoss*)((CBossTurret*)m_pEnemy)->GetOwner())->GetIsDisabled() )
			//{
				EffectInformation::CreateEffect(eGENERIC_EXPLOSION, &m_DeathAnimation, _pEnemy, false, true);
			//}
			//else
			//{
			//	EffectInformation::CreateEffect(eRED_ENEMY_DEATH, &m_DeathAnimation, _pEnemy, false, true);
			//}
		}
		return;
	case eHanger:
		{
			if( ((CBoss*)((CBossHanger*)m_pEnemy)->GetOwner())->GetIsDisabled() )
			{
				EffectInformation::CreateEffect(eGENERIC_EXPLOSION, &m_DeathAnimation, _pEnemy, false, true);
			}
		}
		return;
	case eTURRET_LASER:
		{
			//if( ((CBoss*)((CBossWinTurret*)m_pEnemy)->GetOwner())->GetIsDisabled() )
			//{
				EffectInformation::CreateEffect(eGENERIC_EXPLOSION, &m_DeathAnimation, _pEnemy, false, true);
			//}
			//else
			//{
			//	EffectInformation::CreateEffect(eRED_ENEMY_DEATH, &m_DeathAnimation, _pEnemy, false, true);
			//}
		}
		return;
	case eTURRET_MISSILE:
		{
			//if( ((CBoss*)((CBossLargerTurret*)m_pEnemy)->GetOwner())->GetIsDisabled() )
			//{
				EffectInformation::CreateEffect(eGENERIC_EXPLOSION, &m_DeathAnimation, _pEnemy, false, true);
			//}
			//else
			//{
			//	EffectInformation::CreateEffect(eRED_ENEMY_DEATH, &m_DeathAnimation, _pEnemy, false, true);
			//}
		}
		return;
	case eMISSILE:
		{
			CMissile* tmpMissile = ((CMissile*)m_pEnemy);
			if(tmpMissile->GetOwner()->GetType() == eBOSS )
			{
				if(((CBoss*)tmpMissile->GetOwner())->GetIsDisabled())
				{
					EffectInformation::CreateEffect(eBOSS_EXPLOSION, &m_DeathAnimation, _pEnemy, false, true);
				}
				else
				{
					EffectInformation::CreateEffect(eGENERIC_EXPLOSION, &m_DeathAnimation, _pEnemy, false, true);
				}
			}
			else
			{
				EffectInformation::CreateEffect(eGENERIC_EXPLOSION, &m_DeathAnimation, _pEnemy, false, true);
			}
			tmpMissile->SetIsThrusted(false);
			if(tmpMissile->GetOwner() && tmpMissile->GetOwner()->GetType() != eAMBIENT_ENEMY)
				WwiseNS::SoundEvent(eMissileExplosion, ePlay, false, m_pEnemy);
		}
		return;
	case eRED_ENEMY:
		{
			EffectInformation::CreateEffect(eRED_ENEMY_DEATH, &m_DeathAnimation, _pEnemy, false, true);
		}
		return;
	case eBLUE_ENEMY:
		{
			EffectInformation::CreateEffect(eBLUE_ENEMY_DEATH, &m_DeathAnimation, _pEnemy, false, true);
		}
		return;
	//case eYELLOW_ENEMY:
	//	{
	//		EffectInformation::CreateEffect(, &m_DeathAnimation, _pEnemy, false, true);
	//	}
	//	return;
	default:
		{
			EffectInformation::CreateEffect(eGENERIC_EXPLOSION, &m_DeathAnimation, _pEnemy, false, true);			
		}
		break;
	}
	
}
/*Destructor*/
CDeathMovement::~CDeathMovement(void)
{

}
/*****************************************************************
* Update():		Flanking
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CDeathMovement::Update(float _fElapedTime)
{
// 	deathTimer += _fElapedTime;
// 	scaleTimer-= _fElapedTime;
// 	if (deathTimer >= .35f)
// 	{
	//NOTE:: Moved up to constructor, just play sound based on type when entering.
		//if(m_pEnemy->GetType() == eMISSILE && ((CMissile*)m_pEnemy)->GetOwner() && ((CMissile*)m_pEnemy)->GetOwner()->GetType() != eAMBIENT_ENEMY)
		//	WwiseNS::SoundEvent(eMissileExplosion, ePlay, false, m_pEnemy);
		//
		m_pEnemy->SetActive(false);

		// 	}
// 
// 	m_pEnemy->SetVelocityModifier( 6 );
// 
// 	m_pEnemy->Rotate(_fElapedTime *10,1,0,1);
// 
// 	m_pEnemy->Scale(scaleTimer,scaleTimer,scaleTimer);
// 
// 	//Forward Movement
// 	D3DXMATRIX d3dMat;
// 	D3DXMatrixIdentity( &d3dMat );
// 	d3dMat._43 = _fElapedTime * m_pEnemy->GetVelocityModifier();
// 	m_pEnemy->SetMatrix( &( d3dMat * ( *m_pEnemy->GetMatrix() ) ) );

}