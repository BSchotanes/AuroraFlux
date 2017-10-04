#ifndef _CAM_OBJ
#define _CAM_OBJ

#include "../Entity/IEntity.h"
#include "../Entity/Enemy.h"
#include "../Entity/Boss.h"
#include "../Entity/Asteroid.h"
#include "../Collision/Sphere.h"
#include "../Object Manager/ObjectManager.h"
#include "Camera.h"

class CAmbientFighter;
class CCameraObject : public CEnemy
{
private:
	CCamera*			m_pCurrentCamera;	
	CObjectManager*		m_pObjectManager;
	CBoss*				m_pBoss;
	D3DXMATRIX			m_d3dDestination;
	D3DXMATRIX			m_d3dLookAtMat;
	D3DXMATRIX			m_d3dDestLookAtMat;
	D3DXMATRIX			m_d3dPlayerPos;
	D3DXMATRIX			m_d3dReturnMatrix;
	float				m_fDistanceFromDestination,
						m_fRotationAmount,
						m_fWaitTimer,					
						m_fSpeed,
						m_fRotationBucket,
						m_fRotation;
	bool				m_bRotate,
						m_bMoveToDestination,
						m_bReachedDestination,
						m_bHasFinishedCinematic,
						m_bIsBossFight,
						m_bCinematicEnter,
						m_bReturn;
	int					m_nTotalDistance;
	float				m_dwTime;
	POINT				m_tPrevMousePos;
	bool				m_bOnlyRotate;
	bool				m_bReturning;
	bool				m_bHasPostPosition;
	float				m_fDestWaitTime;
	float				m_fTransition;
	float				m_fBlackoutAlpha;
	bool				m_bShouldFadeIn;
	char				m_szFadeFlag;
	float				m_fWaitToReturn;
	bool				m_bHasSkipped;
	bool				m_bCanBeSkipped;

	bool				m_bIsIntroCinematic;
	char				m_szIntroStage;
	float				m_fIntroReturnBucket;
	vector<CAsteroid*>	m_vAsteroidsInScene;
	vector<D3DXMATRIX>	m_vd3dAsteroidDestinations;
	vector<int>			m_vAlreadyBeen;
	int					m_nCurrentAsteroidTarget;

	int m_fTeachXLocation;
	int m_ntransitionAlpha;

	D3DXMATRIX		m_d3dPostEntrancePosition; 

	void GoToDestination(float _fDt);
	void Rotate(float _fDt);
	void Return(float _fDt);
	void Entrance(float _fDt);
	void StopCinematic();

	void PlayIntroCinematic(float _fDt);
	void AvoidAsteroids(float _fDt);
	void ThroughMeteors(float _fDt);
	void PastFighters(float _fDt);
	void GoToPlayer(float _fDt);

	void SelectNewAsteroid();
	//CAmbientFighter* m_pRightSideAmbientFighter; 
	//CAmbientFighter* m_pLeftSideAmbientFighter;
	CAmbientBoundingBox* _abb;
	bool m_bFirstPass;
	bool m_bFirstFighterPassed;
	bool m_bSecondFigherPassed;
	bool m_bHasPlayedIntroduction;
	bool m_bHasPlayedHomePlanet;
	bool m_bHasPlayedDontWorry;
	float m_fDontWorryBuffer;
	float m_fIntroSoundBuffer;
	vector<D3DXMATRIX> m_d3dIntroCameraWaypoints;
	unsigned int m_nIntroCameraWaypointIter;
	float m_fTimeGoingToWaypoint;
	bool m_bAlreadySeen;
public:
	void SetMatPos(D3DXVECTOR3 _p, D3DXMATRIX& _m);
	void SetUpIntroWaypoints();
	/*****************************************************************
	* CCameraObject():  Default Constructor will set its type. 
	* Ins:			    CCamera* - used for attatching the entity to a camera    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		2/5/2013
	* Mod. Initials:	AL
	*****************************************************************/
	CCameraObject(CCamera* _pCurrentCamera = NULL, CObjectManager* _pObjMan = NULL);
	~CCameraObject();
	/*****************************************************************
	* Update():		Will update its internal timers and behaviors.
	* Ins:			    fElapsedTime    		      
	* Outs:				None
	* Returns:		    void
	* Mod. Date:		2/5/2013
	* Mod. Initials:	AL
	*****************************************************************/
	void Update(float _fElapsedTime);

	void Render();
	int GetType();
	D3DXMATRIX* GetMatrix();
	
	void SetRotation(float _fRotationAmount, D3DXMATRIX _lookAtMat, float _fRotationDuration, bool _bOnlyRotate = false, bool _bReturning = true);
	void SetDestination(D3DXMATRIX _d3dDestMat, D3DXMATRIX _d3dLookAtMat, float _fDist, float _fWait = 0.0f );
	void SetIsBossFight(bool _bIsBoss, CBoss* _pBoss = NULL) {m_bIsBossFight = _bIsBoss; m_pBoss = _pBoss;}
	void SetCameraPositionAfterEntrance(D3DXMATRIX mat);
	void MoveToDestination( bool _CanBeSkipped = true);
	void SetTeachXLocation(int _x)						{ m_fTeachXLocation = _x ;}
	void SetTransitionAlpha(int _x)						{ m_ntransitionAlpha = _x ;}
	void SetPlayerPos(D3DXMATRIX _d3dVec)				{ m_d3dPlayerPos = _d3dVec;}
	void SetIsIntroCinematic(bool _isIntroCinematic)	{ m_bIsIntroCinematic = _isIntroCinematic;}

	D3DXMATRIX GetDestination()			{ return m_d3dDestination;}
	int GetTeachXLocation()				{ return m_fTeachXLocation;}		
	int GetTransitionAlpha()			{ return m_ntransitionAlpha;}
	bool GetHasFinishedCinematic()		{ return m_bHasFinishedCinematic;}
	bool GetHasReachedDestination()		{ return m_bRotate;}
	bool GetHasFinishedRotation()		{ return m_bReturn;}
	float GetBlackoutAlpha()			{ return m_fBlackoutAlpha;}
	bool GetInEntrance()				{ return m_bCinematicEnter;}
	bool GetShouldFadeIn()				{ return m_bShouldFadeIn;}
	bool GetIsIntroCinematic()			{ return m_bIsIntroCinematic;}
	char GetIntroStageFlag()			{ return m_szIntroStage;}

	void SetHasFinishedRotation(bool _bReturn) {m_bReturn = _bReturn;}
	void AddAsteroid( CAsteroid* _roid );

	bool GetAlreadySeenIntro() {return m_bAlreadySeen;}
	void SetAlreadySeenIntro(bool _bAlreadySeen) {m_bAlreadySeen = _bAlreadySeen;}
};

#endif