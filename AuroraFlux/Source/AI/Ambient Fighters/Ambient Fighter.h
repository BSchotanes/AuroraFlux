#ifndef CAMBIENTFIGHTER_H
#define CAMBIENTFIGHTER_H
#include "../../Entity/Enemy.h"
class CAmbientBoundingBox;
class CCamera;
class CAmbientFighter : public CEnemy
{
public:
	CAmbientFighter(){}   
	CAmbientFighter(CAmbientBoundingBox* _pContainingBox, D3DXMATRIX _d3dMatrix, int _flockID);
	CAmbientFighter(D3DXMATRIX _d3dWorldPos, CCamera* _pCamera);
	bool Update(float fTime, bool _bossFight);
	void SetDefaults();
	CAmbientBoundingBox* GetBoundingBox() {return m_pContainingBox;}
	int GetFlockID() {return m_nFlockID;}
	bool IntroUpdate(float fTime);
	bool HalfSpaceTest(const D3DXVECTOR3& planeNorm, const D3DXVECTOR3& pointOnPlane, const D3DXVECTOR3& objPoint);
private:
protected:
	int m_nFlockID;
	CAmbientBoundingBox* m_pContainingBox;
	float bobmTimer;
	int timerOffset;
	int _nRandomFiringOffset;
	bool m_bFirstExplosion; 
	bool m_bSecondExplosion;
	bool m_bThirdExplosion;
	float _fAmbientFighterFiringBucket;
	bool m_bIsInIntroduction;
	CCamera* m_pCamera;
	bool m_bHasAlreadyPassedPlayer;
	bool m_bHasAlreadyShakenCamera;
	CEffectSystem m_esEngineTrail;
	CEffectSystem m_esArrival;
	bool m_bHasPlayedWarpIn;
};
#endif