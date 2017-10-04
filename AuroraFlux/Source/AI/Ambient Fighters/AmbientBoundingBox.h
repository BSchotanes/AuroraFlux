#ifndef CAMBIENTBOUNDINGBOX_H
#define CAMBIENTBOUNDINGBOX_H

struct D3DXMATRIX;
class CObjectManager;
class CCamera;
#include "Ambient Fighter.h"
 
class CAmbientBoundingBox
{ 
public:
	~CAmbientBoundingBox();
	CAmbientBoundingBox(){} 
	CAmbientBoundingBox(D3DXMATRIX _worldMat, int nFighterCount, float _fHeight, float _fDepth, float _fWidth, CObjectManager* _pObjManager);
	//this one is for the creation of the ambient fighters that will not be actually contained by a bounding box.
	CAmbientBoundingBox( CCamera* _pCamera);
	void Update(float fTime, bool _bossFight);
	std::vector<CAmbientFighter*> GetFighters() {return m_vContainedFighters;}
	D3DXMATRIX GetWorldMatrix() {return m_WorldMatrix;}
	float GetBoundingHeight() {return m_fBoundingHeight;}
	float GetBoundingDepth()  {return m_fBoundingDepth;}
	float GetBoundingWidth()  {return m_fBoundingWidth;}
	CObjectManager* GetObjectManager() {return m_pObjectManager;}
	int GetFighterCount() {return m_vContainedFighters.size();}
	void AddFighter(CAmbientFighter* _fighter) {m_vContainedFighters.push_back(_fighter);}
	bool IsIntroFighters() {return m_bIsIntroFighters;}
	bool IntroIsDone();
private: 
protected:
	//top left corner of the bounding box
	D3DXMATRIX m_WorldMatrix;
	float m_fBoundingHeight;
	float m_fBoundingDepth;
	float m_fBoundingWidth;
	CObjectManager* m_pObjectManager;
	bool m_bIsIntroFighters;
	//counter for how many fighters have passed the camera
	int m_nIntroCounter;
	//counter to keep the bounding box only updating once every 5 frames
	int m_nCounter;
	std::vector<CAmbientFighter*> m_vContainedFighters;
	CCamera* m_pCamera;
};

#endif