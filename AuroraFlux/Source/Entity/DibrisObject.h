#ifndef CDEBRIS_H
#define CDEBRIS_H
#include "IEntity.h"
#include "Enemy.h"

class CDibrisObject : public CEnemy
{
private:
public:
	~CDibrisObject();
	CDibrisObject();

	void Update(float fElapedTime);
	void Render();

};

#endif