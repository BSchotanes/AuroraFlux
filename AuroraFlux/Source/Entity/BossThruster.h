#ifndef CBOSSTHRUSTER_H
#define CBOSSTHRUSTER_H
#include "Enemy.h"
#include "Boss.h"

class CBossThruster : public CEnemy
{
public:
	CBossThruster(CBoss* _pBoss, D3DXVECTOR3 _offset, float fRadius, bool shouldDoEffect = true);
	void Update(float fTime);
private:
protected:
	CBoss* m_pBoss;
	D3DXVECTOR3 m_d3dOffset;
	CEffectSystem m_ESBossThruster;
	bool m_bShouldDoEffect;
};
#endif