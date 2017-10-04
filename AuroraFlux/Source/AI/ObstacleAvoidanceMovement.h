#ifndef COBSTACLEAVOIDANCE_H
#define COBSTACLEAVOIDANCE_H

#include "MovementBehavior.h"
class CObstacleAvoidanceMovement : public CMovementBehavior
{
public:
	CObstacleAvoidanceMovement(CEnemy* _pEnemy );
	/*Destructor*/
	~CObstacleAvoidanceMovement(void);
	void Update(float _fElapedTime);
private:
protected:
	std::vector<int> avoidFlags;
};
#endif