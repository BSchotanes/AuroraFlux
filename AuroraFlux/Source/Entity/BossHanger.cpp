/***********************************************
* Filename:  		Asteroid.cpp
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will hold all of the data for the asteroids.				
************************************************/
#include "../StdAfx.h"
#include "../Entity/BossHanger.h"
#include "../Entity/Boss.h"
#include "../Object Manager/ObjectManager.h"
/*****************************************************************
* CBossHanger():  Default Constructor will set its type. Initilizes asteroid unique data.
* Ins:			    None    		      
* Outs:		    None
* Returns:		    None
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
CBossHanger::CBossHanger(CEntity * _pOwner, int _type, CObjectManager * _objectManager) : n_fTimeToSpawnNext(0)
{
	m_nObjectType   = eHanger;
	m_nHealth       = 200;
	m_nShields      = 200;
	m_nVelocityModifier = 20;
	m_nDamage = 0;
	m_bIsHit        = false;
	m_bActive       = true;
	m_pOwner = _pOwner;
	Scale(3,3,3);
	m_pObjectManager = _objectManager;
	m_nHangerType = _type;
	m_pPlayer = ((CEnemy*)_pOwner)->GetPlayer();
	SetBV(CCollOBJ::Create(eCAP, D3DXVECTOR3(0,0,0), D3DXVECTOR3(1,1,1), 10.5f));
}

/*Destructor*/
CBossHanger::~CBossHanger(void)
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
void CBossHanger::Update(float _fElapedTime)
{
	CEnemy::Update(_fElapedTime);
	n_fTimeToSpawnNext += _fElapedTime;	


	switch(m_nHangerType)
	{
	case eRED_HANGER:
		{
			RedHanger();

		}
		break;

	case eBLUE_HANGER:
		{
			BlueHanger();
		}
		break;

	case eYELLOW_HANGER:
		{
			YellowHanger();
		}
		break;

	case -1:
		{
#if _DEBUG
			cout << "forgot to set type " << endl;
#endif
		}
		break;
	}

	//Rotate(2 *_fElapedTime, 0,1,0);
	if (m_pCurrMoveBehavior)
	{
		m_pCurrMoveBehavior->Update(_fElapedTime);
	}
}


bool CBossHanger::RedHanger()
{
	CBoss* pTheBoss = ((CBoss*)m_pOwner);
	static int whichGuy = 1;
	int bossPhase = pTheBoss->GetPhase();
			if (bossPhase == eLASER_PHASE)
			{
				if (n_fTimeToSpawnNext >= 23.5f)
				{
					m_cFlags |= eADD_RED_ENEMY;
					n_fTimeToSpawnNext = 0.0f;
				}
			}
			else if(bossPhase == eMISSILE_PHASE)
			{
				if (n_fTimeToSpawnNext >= 20.0f) 
				{
					if(whichGuy >= 1)
					{
						m_cFlags |= eADD_RED_ENEMY;

					}
					else
					{
						m_cFlags |= eADD_BLUE_ENEMY;
					}
					whichGuy *= -1;
					n_fTimeToSpawnNext = 0.0f;

				}
			}
			else
			{
				if (n_fTimeToSpawnNext >= 18.0f) 
				{
					if(whichGuy >= 1)
					{
						m_cFlags |= eADD_RED_ENEMY;

					}
					else
					{
						m_cFlags |= eADD_BLUE_ENEMY;
					}
					whichGuy *= -1;
					n_fTimeToSpawnNext = 0.0f;

				}

			}

	return true;

}

bool CBossHanger::BlueHanger()
{
	CBoss* pTheBoss = ((CBoss*)m_pOwner);
	if(pTheBoss->GetPhase() == eLASER_PHASE)
			{
				return false;
			}
			else if (pTheBoss->GetPhase() == eMISSILE_PHASE)
			{
				if (n_fTimeToSpawnNext >= 60.0f)
				{
					m_cFlags |= eADD_BLUE_ENEMY;

					n_fTimeToSpawnNext = 0.0f;
				}
			}
			else
			{
				if (n_fTimeToSpawnNext >= 40.0f)
				{
					m_cFlags |= eADD_BLUE_ENEMY;

					n_fTimeToSpawnNext = 0.0f;
				}
			}


	return true;

}

bool CBossHanger::YellowHanger()
{
	CBoss* pTheBoss = ((CBoss*)m_pOwner);

	if(pTheBoss->GetPhase() == eLASER_PHASE || pTheBoss->GetPhase() == eMISSILE_PHASE )
			{
				return false;
			}

			if (n_fTimeToSpawnNext >= 10.0f)
			{
				m_cFlags |= eADD_YELLOW_ENEMY;

				n_fTimeToSpawnNext = 0.0f;
			}
	return true;
}
/*****************************************************************
* Render():		Draws the updated asteroid and details with asteroids.
* Ins:			    fElapedTime    		      
* Outs:				None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CBossHanger::Render() 
{

}