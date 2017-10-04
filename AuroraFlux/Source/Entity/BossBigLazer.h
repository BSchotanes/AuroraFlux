#ifndef CBOSSBIGLAZER_H
#define CBOSSBIGLAZER_H
#include "Enemy.h"

class CBossWinTurret; 

class CBossBigLazer : public CEnemy
{
public:
	CBossBigLazer();
	void Update(float fTime);
	void SetTurretOwner(CBossWinTurret* _pOwner) {m_pOwner = _pOwner;}
	void KillLazer() {m_pOwner = NULL;}
	CBossWinTurret* GetOwner() {return m_pOwner;}
private:
protected:
	CBossWinTurret* m_pOwner;
};

#endif