#ifndef CAMBIENTFLOCKINGMOVEMENT_H
#define CAMBIENTFLOCKINGMOVEMENT_H
#include "MovementBehavior.h"
class CAmbientFighter; 
class CAmbientFlockingMovement : public CMovementBehavior
{
public:  
	~CAmbientFlockingMovement();
	CAmbientFlockingMovement(CAmbientFighter* _pEnemy, float _fSeperation = 0.5f, float _fCohesion = 1.0f, float _fAlignment = 0.5f);
	void Update(float _fTime); 
protected: 
	std::vector<CAmbientFighter*> m_vFlock;
	CAmbientFighter* m_pOwner;
	float m_fCohesionWeight; 
	float m_fSeperationWeight;
	float m_fAlignmentWeight;
	float m_fFlockRadius;
	D3DXVECTOR3 m_dAverageHeading;
	D3DXVECTOR3 m_dAveragePosition;
	void CalculateAverages();
	D3DXVECTOR3 Cohesion();
	D3DXVECTOR3 Seperation();
	D3DXVECTOR3 Alignment();
	void MoveBoid(D3DXVECTOR3 _d3dTurnDir, float fTime);
	void AcquireFlock();
	bool OwnerTooFar();
	void TurnTo(D3DXMATRIX& objMatrix, D3DXVECTOR3 targetPos, float deltaTime);
};

#endif