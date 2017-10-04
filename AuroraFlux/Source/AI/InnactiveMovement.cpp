#include "InnactiveMovement.h"
#include "../Entity/Enemy.h"
#include "DeathMovement.h"

CInnactiveMovement::CInnactiveMovement(CEnemy* _pEnenmy) : CMovementBehavior(_pEnenmy)
{

}
CInnactiveMovement::~CInnactiveMovement(void)
{

}
void CInnactiveMovement::Update(float _fElapedTime)
{
	if(m_pEnemy->GetHealth() <= 0)
	{
		m_pEnemy->PushBehavior(new CDeathMovement(m_pEnemy));
		return;
	}
}