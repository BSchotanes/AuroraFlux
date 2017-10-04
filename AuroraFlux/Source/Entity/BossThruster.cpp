#include "BossThruster.h"

CBossThruster::CBossThruster(CBoss* _pBoss, D3DXVECTOR3 _offset, float fRadius, bool shouldDoEffect )
{
	m_nObjectType = eBOSSTHRUSTER;
	m_pBoss = _pBoss;
	SetBV(CCollOBJ::Create(eSPH, D3DXVECTOR3(0,0,0), fRadius));
	SetActive(true);
	m_bIsVisable = false;
	m_d3dOffset = _offset;
	if(shouldDoEffect)
		EffectInformation::CreateEffect(eBOSS_THRUSTER, &m_ESBossThruster, this, true, false);
	m_bShouldDoEffect = shouldDoEffect;
}
void CBossThruster::Update(float fTime)
{
	CEnemy::Update(fTime);
	if(m_pBoss)
	{
		if(m_pBoss->GetPhase() != eSEEK_AND_DESTROY && m_pBoss->GetIsDisabled() == false)
		{
			
			SetPosition(m_pBoss->GetPosition().x + m_d3dOffset.x, m_pBoss->GetPosition().y + m_d3dOffset.y, m_pBoss->GetPosition().z + m_d3dOffset.z);
			if(m_ESBossThruster.GetPlay() == false && m_bShouldDoEffect)
			{
				m_ESBossThruster.SetPlay(true);
				m_ESBossThruster.ResetPosition();
			}
		}
		else
		{
			if(m_ESBossThruster.GetPlay() == true)
			{
 				m_ESBossThruster.SetPlay(false);
			}
		}
	}
}