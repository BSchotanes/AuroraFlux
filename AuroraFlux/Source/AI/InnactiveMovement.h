#ifndef CINNACTIVEMOVEMENT_H
#define	CINNACTIVEMOVEMENT_H

#include "MovementBehavior.h"

class CInnactiveMovement : public CMovementBehavior
{
public:
	CInnactiveMovement(CEnemy* _pEnenmy );
	~CInnactiveMovement(void);
	void Update(float _fElapedTime);
};
#endif