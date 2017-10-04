#include "SceneryManager.h"
#include "EnvironmentObjects.h"

CSceneryManager::CSceneryManager(CObjectManager * _pObjectManager,  bool _isMainMenu) : m_pObjectManager(_pObjectManager)
{
	m_Position = D3DXVECTOR3(0,0,0);
	for (int i = 0; i < 40; i++)
	{
		CComet *poop = new CComet(rand() % 5 + 5.0f);
		poop->SetActive(false);
		poop->SetMainMenuType(2);
		m_pObjectManager->AddToInactiveArmament(poop);
		m_vpListOfComets.push_back(poop);
	}
	m_bIsMainMenu = _isMainMenu;
}
CSceneryManager::~CSceneryManager(void)
{

}
void CSceneryManager::Update(float _time)
{
// 	if (GetAsyncKeyState('N') & 1 )
// 	{
// 		cout <<"x:" << currObject->GetPosition().x  <<"  y:" << currObject->GetPosition().y  <<"  z:" << currObject->GetPosition().z << endl;
// 	}
// 	if (GetAsyncKeyState(VK_UP))
// 	{
// 		m_Position.y += 1000;
// 	}
// 	if (GetAsyncKeyState(VK_DOWN))
// 	{
// 		m_Position.y -= 1000;
// 	}
// 	if (GetAsyncKeyState(VK_LEFT))
// 	{
// 		m_Position.x -= 1000;
// 	}
// 	if (GetAsyncKeyState(VK_RIGHT))
// 	{
// 		m_Position.x += 1000;
// 	}
// 	if (GetAsyncKeyState('J'))
// 	{
// 		m_Position.z -= 1000;
// 	}
// 	if (GetAsyncKeyState('K'))
// 	{
// 		m_Position.z += 1000;
// 
// 	}
// 
// 	currObject->SetPosition(m_Position);
	HandleComets(_time);
}

void CSceneryManager::HandleComets(float _elapsedTime)
{
	static float one = 0.0f;
	one+=_elapsedTime;
	if (one >= 2.0f)
	{
		one = 0.0f;
		bool doIt = false;
		unsigned int sizeOfCometList = m_vpListOfComets.size();
		for (unsigned int i = 0; i < sizeOfCometList; i++)
		{
			if (m_vpListOfComets[i]->GetActive() == false)
			{
				doIt = true;
				break;
			}
		}
		if (doIt)
		{
			CComet *poop = (CComet*) m_pObjectManager->ReturnArmamentToMasterList(eLARGE_ASTEROID);
			if (poop)
			{
				if (m_bIsMainMenu == false)
				{

					int choice = rand() % 4;
					if (choice == 0)
					{
						poop->SetPosition(rand() % 10000 - 5000.0f,  rand() % 5000 +10000.0f, rand() % 3000 + 3000.0f);

					}
					else if (choice == 1)
					{
						poop->SetPosition( rand() % 5000 + 10000.0f,  rand() % 10000 - 5000.0f, rand() % 3000 + 3000.0f);

					}
					else if (choice == 2)
					{
						poop->SetPosition( (rand() % 3000 * -1)- 10000.0f,  rand() % 10000 - 5000.0f,rand() % 3000 + 3000.0f);
					}
					else
					{
						poop->SetPosition(rand() % 10000 - 5000.0f,  (rand() % 3000 * -1) - 10000.0f,rand() % 3000 + 3000.0f);
					}
					poop->Rotate(rand() % 300 +0.0f, 0,1,0);
				}
				else
				{
					int choice = rand() % 4;
					if (choice == 0)
					{
						poop->SetPosition(rand() % 10000 - 5000.0f,  rand() % 50 +100.0f, rand() % 6000 - 3000.0f);

					}
					else if (choice == 1)
					{
						poop->SetPosition( rand() % 5000 + 10000.0f,  rand() % 100 - 50.0f, rand() % 6000 - 3000.0f);

					}
					else if (choice == 2)
					{
						poop->SetPosition( (rand() % 3000 * -1)- 10000.0f,  rand() % 100 - 50.0f,rand() % 6000 - 3000.0f);
					}
					else
					{
						poop->SetPosition(rand() % 10000 - 5000.0f,  (rand() % 30 * -1) - 100.0f,rand() % 6000 - 3000.0f);
					}

					poop->Rotate(rand() % 300 +0.0f, 0,1,0);
				}
			}
		//	poop->SetTarget(currObject);

		}
	}
}


/*Destructor*/
void CSceneryManager::CreateScene()
{
// 	currObject = new CEnvironmentObject( eSPACE_STATION, m_Position);
// 	m_pObjectManager->AddObject(currObject);
	CEnvironmentObject* pOwner = nullptr;
	// Planets
	//AddSceneryObject(ePLANET1, D3DXVECTOR3(-5000,0,60000),0,35.0f );
 	AddSceneryObject(eSPACESTATION1, D3DXVECTOR3(-4150, 900, 2950),0, 3 );
 	AddSceneryObject(ePLANET4, D3DXVECTOR3(12000,2090, -3000),0, 22.0f, true );
	pOwner = AddSceneryObject(ePLANET3, D3DXVECTOR3(-22000,-15000,-20000),0, 19 );
	//AddSceneryObject(eSPACE_STATION, D3DXVECTOR3(-14000,-10000,-10000) , 6, .025f,0,D3DXVECTOR3(0,0,0),0, 0);
 	AddSceneryObject(ePLANET2, D3DXVECTOR3(-30000, 0, 5000),0, 4 );
 	//AddSceneryObject(eSPACE_STATION, D3DXVECTOR3(-6100,9000,1900), 10 );
 	//AddSceneryObject(eSPACE_STATION, D3DXVECTOR3( 6100,0,10900) , 1, 2);
 	//AddSceneryObject(eSPACE_STATION, D3DXVECTOR3(-13000,8000,41550) , 4, .025f,0, D3DXVECTOR3(0,0,0), 0, 0);
 	//AddSceneryObject(eSPACE_STATION, D3DXVECTOR3(-22000,-6000,40550) , 6, .025f,0, D3DXVECTOR3(0,0,0), 0, 0);
 	////AddSceneryObject(eSPACE_STATION, D3DXVECTOR3(-9000,-1000,37550) , 6, .025f,0, D3DXVECTOR3(0,0,0), 0, 0);
 	//AddSceneryObject(eSPACE_STATION, D3DXVECTOR3(-11000,-6000,39550) , 6, .025f,0, D3DXVECTOR3(0,0,0), 0, 0);
 	////AddSceneryObject(eSPACE_STATION, D3DXVECTOR3(-6000,-14000,38550) , 6, .025f,0, D3DXVECTOR3(0,0,0), 0, 0);
 	//AddSceneryObject(eSPACE_STATION, D3DXVECTOR3(-6000,-1000,38550) , 6, .025f,0, D3DXVECTOR3(0,0,0), 0, 0);
 
 	AddSceneryObject(eSPACE_STATION, D3DXVECTOR3(-25000,-9000,-17000) , 6, .025f,0, D3DXVECTOR3(0,0,0), 0, 0);
 	//AddSceneryObject(eSPACE_STATION, D3DXVECTOR3(-22000,-20000,-13000) , 6, .025f,0, D3DXVECTOR3(0,0,0), 0, 0);
 	AddSceneryObject(eSPACE_STATION, D3DXVECTOR3(-23000,-12000,-11000) , 6, .025f,0, D3DXVECTOR3(0,0,0), 0, 0);
 
	
 
 	//AddSceneryObject(eSPACE_STATION, D3DXVECTOR3(0,0,0) , 2, .45f,0,D3DXVECTOR3(0,0,0), 0, false);
 
 	//AddSceneryObject(eSPACESTATION1, D3DXVECTOR3(-20000,-15000,-13000) , 2, .45f);
 	
 	AddSceneryObject(eSPACESTATION1, D3DXVECTOR3(4150, 4900, 2950),0, 4 );
 
 	AddSceneryObject(eSPACESTATION1, D3DXVECTOR3(-2000, -8000, 0),0, 4 );
 	//AddSceneryObject(eSPACE_STATION, D3DXVECTOR3(-15000, 0, 5000),0, 4,0,D3DXVECTOR3(0,0,0),0 , 0 );
 	AddSceneryObject(eSPACE_STATION, D3DXVECTOR3(-25000, 0, 5000),0, 4,0,D3DXVECTOR3(0,0,0),0 , 0 );
 	//AddSceneryObject(eSPACE_STATION, D3DXVECTOR3(-15000, 0, 10000),0, 4,0,D3DXVECTOR3(0,0,0),0 , 0 );
 	AddSceneryObject(eSPACE_STATION, D3DXVECTOR3(-15000, 0, 2500),0, 4,0,D3DXVECTOR3(0,0,0),0 , 0 );
 	//AddSceneryObject(eSPACE_STATION, D3DXVECTOR3(-15000, -3000, 2500),0, 4,0,D3DXVECTOR3(0,0,0),0 , 0 );
 
 	AddSceneryObject(eSPACE_STATION, D3DXVECTOR3(-24000, 5000, 5000 ), 2, 2.45f,0,D3DXVECTOR3(0,0,0), 0, false); 
 	if (!m_bIsMainMenu)
 	{
 	AddSceneryObject(eSPACESTATION1, D3DXVECTOR3(3500 , -2000 , 2000), 2, .45f,0,D3DXVECTOR3(0,0,0), 0, true); 
 	AddSceneryObject(eSATELITE, D3DXVECTOR3(-5250, 4000 , 2000), 2, 20,0,D3DXVECTOR3(0,0,0), 0, true); 
 	AddSceneryObject(eSPACESTATION1, D3DXVECTOR3(600, -3500 , 2000), 2, .45f,0,D3DXVECTOR3(0,0,0), 0, true); 
 	AddSceneryObject(eSPACESTATION1, D3DXVECTOR3(0, 3500 , 2000), 2, .45f,0,D3DXVECTOR3(0,0,0), 0, true); 
 	AddSceneryObject(eSATELITE, D3DXVECTOR3(3000, -7000, 8300), 3, 20,0,D3DXVECTOR3(0,0,0), 0, true); 
 	AddSceneryObject(eSATELITE, D3DXVECTOR3(0, 400, -3250), 2, 20,0,D3DXVECTOR3(0,0,0), 0, true);

 	}
	ScatterDebris(); 
	  
	//need moons around purple planet 
	AddSceneryObject(ePLANET4, pOwner->GetPosition(), 0, 0.2f, false, D3DXVECTOR3(-7550, -5350, -5600) - pOwner->GetPosition(), pOwner); 
	AddSceneryObject(ePLANET4, pOwner->GetPosition(), 0, 0.2f, false, D3DXVECTOR3(-9350, -8950, -9350) - pOwner->GetPosition(), pOwner); 
	 
}
CEnvironmentObject* CSceneryManager::AddSceneryObject( int _type, D3DXVECTOR3 _position, int _rotate /*= 0*/, float _scale /*= 1.0f*/, bool _hasAsteroidField /*= false*/ , D3DXVECTOR3 _d3dOffset /*= D3DXVECTOR3(0,0,0)*/, CEntity* _pOwner/* = 0*/, bool _isVisable)
{
	CEnvironmentObject* tmp = 0;

	switch (_type)
	{
	case ePLANET1:
		{
			tmp = new CEnvironmentObject(ePLANET1,_position);
			tmp->Scale(_scale,_scale,_scale);
			tmp->SetMainMenuType(3);

			m_pObjectManager->AddObject(tmp);
		}
		
		break;
	case ePLANET2:
		{
			tmp = new CEnvironmentObject(ePLANET2,_position);
			tmp->Scale(_scale,_scale,_scale);
			tmp->SetMainMenuType(4);

			m_pObjectManager->AddObject(tmp);
		}

		break;
	case ePLANET3:
		{
			tmp = new CEnvironmentObject(ePLANET3,_position);
			tmp->Scale(_scale,_scale,_scale);
			tmp->SetMainMenuType(5);

			m_pObjectManager->AddObject(tmp);
			
		}
		break;

	case ePLANET4:
		{
			tmp = new CEnvironmentObject(ePLANET4,_position);
			tmp->Scale(_scale,_scale,_scale);
			m_pObjectManager->AddObject(tmp);
			if (_hasAsteroidField)
			{
				CreateAsteroidField(tmp );
			}
			tmp->SetOwner(_pOwner);
			tmp->SetOffset(_d3dOffset);
			tmp->SetMainMenuType(5);

		}
		break;

	case eSPACE_STATION:
		{
			tmp = new CEnvironmentObject(eSPACE_STATION,_position);
			tmp->SetIsVisable(_isVisable);
			tmp->SetMainMenuType(6);

			if (_rotate == 1)
			{
				tmp->Rotate(.85f,1,0,1);
				tmp->Scale(_scale,_scale,_scale);
			}
			else if(_rotate == 2)
			{
				tmp->Rotate(-.65f,1,0,0);
				tmp->Scale(_scale,_scale,_scale);
			}
			else
			{
				tmp->Scale(_scale,_scale,_scale);
			}
			m_pObjectManager->AddObject(tmp);
		}
		break;

	case eSPACE_DEBRIS:
		{
			tmp = new CEnvironmentObject(eSPACE_DEBRIS,_position);
			tmp->Scale(_scale,_scale,_scale);
			tmp->SetIsScenery(false);
			tmp->Rotate(rand() % 300 + 0.0f, rand() & 2,rand() & 2,rand() & 2 );
			m_pObjectManager->AddObject(tmp);
			tmp->SetMainMenuType(7);

		}
		break;
	case eSPACE_DEBRIS2:
		{
			tmp = new CEnvironmentObject(eSPACE_DEBRIS2,_position);
			tmp->Scale(_scale,_scale,_scale);
			tmp->SetIsScenery(false);
			tmp->Rotate(rand() % 300 + 0.0f, rand() & 2,rand() & 2,rand() & 2 );
			m_pObjectManager->AddObject(tmp);
			tmp->SetMainMenuType(8);

		}
		break;
	case eSPACE_DEBRIS3:
		{
			tmp = new CEnvironmentObject(eSPACE_DEBRIS3,_position);
			tmp->Scale(_scale,_scale,_scale);
			tmp->SetIsScenery(false);
			tmp->Rotate(rand() % 300 + 0.0f, rand() & 2,rand() & 2,rand() & 2 );
			m_pObjectManager->AddObject(tmp);
			tmp->SetMainMenuType(9);

		}
		break;
	case eBARRIER:
		{
			tmp = new CEnvironmentObject(eBARRIER,_position);
			m_pObjectManager->AddObject(tmp);
			tmp->SetMainMenuType(7);

		}
		break;
	case eSPACESTATION1:
		{
			tmp = new CEnvironmentObject(eSPACESTATION1,_position);
			tmp->Scale(_scale,_scale,_scale);
			tmp->Rotate(rand() % 300 + 0.0f, rand() & 2,rand() & 2,rand() & 2 );

			m_pObjectManager->AddObject(tmp);
			tmp->SetMainMenuType(6);

		}
		break;
	case eASTEROID:
		{
 			//CEnvironmentObject* tmp = new CEnvironmentObject(eASTEROID,_position);
			//tmp->Scale(30,30,30);
			//tmp->SetIsOrbit(true);
 			
 			//m_pObjectManager->AddObject(tmp);
		}
		break;
	case eLARGE_ASTEROID:
		{
			
		}
		break;
	case eSATELITE:
		{
			tmp = new CEnvironmentObject(eSATELITE,_position);
			tmp->Scale(_scale,_scale,_scale);
			tmp->Rotate(rand() % 300 + 0.0f, rand() & 2,rand() & 2,rand() & 2 );
			tmp->SetMainMenuType(6);

			m_pObjectManager->AddObject(tmp);
		}
		break;
	}
	return tmp;
}
void CSceneryManager::CreateAsteroidField(CEnvironmentObject * _pOwner)
{
	int helper = 0;
	for (int i = -15; i != 15; i++)
	{
		helper = i;
		if (helper <= 3 && helper >= -3)
		{
			if (helper > 0)
			{
				helper = 5;
			}
			else
			{
				helper = -5;
			}
		}
		D3DXVECTOR3 newpos1 = D3DXVECTOR3(_pOwner->GetPosition());
		newpos1.x -=  6200 + (rand() % 120 * i);
		newpos1.z += (300* i);
		newpos1.y -= (((rand() % 200) - 100) * helper);
		//	newpos.z += 3000;
		CEnvironmentObject* asteroidBelt = new CEnvironmentObject(eASTEROID,newpos1);
		asteroidBelt->Scale(10,10,10);
		asteroidBelt->SetIsOrbit(true);
		asteroidBelt->SetMainMenuType(2);
		asteroidBelt->Rotate(rand() % 300 + 0.0f, rand() % 2,  rand() % 2, rand() % 2);
		m_pObjectManager->AddObject(asteroidBelt);

		m_vListOfAsteroids.push_back(asteroidBelt);

	}

	for (int i = -15; i != 15; i++)
	{
		D3DXVECTOR3 newpos1 = D3DXVECTOR3(_pOwner->GetPosition());
		newpos1.x -=  6200 + (rand() % 120 * i);
		newpos1.z += (300* i);
		newpos1.y -= (((rand() % 200) - 100) * helper);
		//	newpos.z += 3000;
		CEnvironmentObject* asteroidBelt = new CEnvironmentObject(eASTEROID,newpos1);
		asteroidBelt->Scale(10,10,10);
		asteroidBelt->SetMainMenuType(2);
		asteroidBelt->SetIsOrbit(true);
		asteroidBelt->Rotate(rand() % 300 + 0.0f, rand() % 2,  rand() % 2, rand() % 2);

		m_pObjectManager->AddObject(asteroidBelt);

		m_vListOfAsteroids.push_back(asteroidBelt);

	}
	D3DXVECTOR3 newpos1 = D3DXVECTOR3(_pOwner->GetPosition());
	newpos1.x -=  6000;
	newpos1.y +=  2000;

	AddSceneryObject(eSPACE_STATION, newpos1 ,1, 0.4f);

}
void CSceneryManager::ScatterDebris()
{
	for (int i = 10; i < 95; i++)
	{
		int r = rand() % 3;
		if (r == 0)
		{
			AddSceneryObject(eSPACE_DEBRIS, D3DXVECTOR3(((rand() % 3000) - 1500.0f ),((rand() % 1200) - 600.0f ),  500.0f + ((rand() % 3000))) ,0, ((rand() % 12) / 5.0f) );
		}
		else if(r == 1)
		{
			AddSceneryObject(eSPACE_DEBRIS2, D3DXVECTOR3(((rand() % 3000) - 1500.0f ),((rand() % 1200) - 600.0f ),  500.0f + ((rand() % 3000))) ,0, ((rand() % 12) / 5.0f) );
		}
		else
		{
			AddSceneryObject(eSPACE_DEBRIS3, D3DXVECTOR3(((rand() % 3000) - 1500.0f ),((rand() % 1200) - 600.0f ),  500.0f + ((rand() % 3000))) ,0, ((rand() % 12) / 5.0f) );
		}
	}
	
	for (int i = 10; i < 25; i++)
	{
			AddSceneryObject(eSPACE_DEBRIS3, D3DXVECTOR3( 1.0f + rand() % 300 - 150 ,1.0f + rand() % 300 - 150,  5000.0f + ((rand() % 3000))) ,0, ((rand() % 12) / 5.0f) );
	}

}
