#include "../StdAfx.h"

#include "../AI/AIHelper.h"

CAIHelper::CAIHelper()
{
	ReadFile("AIHelper.txt");
}
// Destructor
CAIHelper::~CAIHelper(void)
{

}
/*****************************************************************
* Update():		Will pass down the updates to its children. 
* Ins:			    fElapedTime    		      
* Outs:		    None
* Returns:		    void
* Mod. Date:		11/29/2012
* Mod. Initials:	AR
*****************************************************************/
void CAIHelper::Update(float _fElapedTime)
{

}

bool CAIHelper::ReadFile(LPCTSTR szFilePath)
{
	string szTempFilePath = "Resources/Scripts/";
	szTempFilePath += szFilePath;

	fstream fsScriptIn(szTempFilePath, std::ios_base::in);

	//	if(fsScriptIn == NULL)
	//	{
	//#if _DEBUG
	//		cout << "Script did not load properly!" << '\n';
	//#endif
	//
	//		return false;
	//	}
	if(fsScriptIn.is_open())
	{
		fsScriptIn >>m_nHealth ;
		fsScriptIn >>redShield	 ;
		fsScriptIn >>blueShield	 ;
		fsScriptIn >>yellowShield	 ;

		fsScriptIn >>laserDamage		  ;
		fsScriptIn >>missileDamage		  ;
		fsScriptIn >>yellowDamage		  ;

		fsScriptIn >>yellowVelocityModifier		  ;
		fsScriptIn >>yellowChaseVelocityModifier		  ;
		fsScriptIn >>blueVelocityModifier		  ;
		fsScriptIn >>blueEvadeVelocityModifier		  ;
		fsScriptIn >>redVelocityModifier		  ;
		fsScriptIn >>redEvadeVelocityModifier		  ;
		fsScriptIn >>laserVelocityModifier		  ;
		fsScriptIn >>missileVelocityModifier		  ;
		fsScriptIn >>redTurnRate		  ;
		fsScriptIn >>blueTurnRate		  ;
		fsScriptIn >>redBehindTurnRate		  ;
		fsScriptIn >>blueEvadeTurnRate		  ;
		fsScriptIn >>yellowTurnRate		  ;
		fsScriptIn >>yellowChaseTurnRate		  ;
		fsScriptIn >>missileTurnRate		  ;
		fsScriptIn >>missileLowerTurnRate		  ;
		fsScriptIn >>laserTurnRate		  ;

		fsScriptIn >>red_to_flee_distance		  ;
		fsScriptIn >>blue_to_flee_distance		  ;
		fsScriptIn >>red_to_chase_distance		  ;
		fsScriptIn >>blue_to_chase_distance		  ;
		fsScriptIn >>force_red_to_chase_distance		  ;
		fsScriptIn >>force_red_to_flee_distance		  ;
		fsScriptIn >>yellow_stealth_chase_radius		  ;
		fsScriptIn >>yellow_not_stealth_chase_radius		  ;
		fsScriptIn >>yellow_waypoint_range		  ;
		fsScriptIn >>wandering_waypoint_range		  ;
		fsScriptIn >>blue_enemy_missile_fire_timer		  ;
		fsScriptIn >>red_enemy_fire_rate		  ;
		fsScriptIn >>evade_timer		  ;
		fsScriptIn >>blue_evade_timer		  ;
		fsScriptIn >>force_chase_timer		  ;
	}
	else
	{
#if _DEBUG
		cout << "Script did not load properly!" << '\n';
#endif
		return false;
	}
	fsScriptIn.close();
	return true;
}
bool CAIHelper::SaveFile(LPCTSTR szFilePath)
{
	string szTempFilePath = "Resources/Scripts/";
	szTempFilePath += szFilePath;

	fstream fsScriptOut(szTempFilePath, ios_base::trunc | ios_base::out );

	if (!fsScriptOut.is_open())
	{
		return false;
	}

	fsScriptOut << m_nHealth << '\n';
	fsScriptOut << redShield	 << '\n';
	fsScriptOut << blueShield	 << '\n';
	fsScriptOut << yellowShield	 << '\n';

	fsScriptOut <<laserDamage		  << '\n';
	fsScriptOut <<missileDamage		  << '\n';
	fsScriptOut <<yellowDamage		  << '\n';
	
	fsScriptOut <<yellowVelocityModifier		  << '\n';
	fsScriptOut <<yellowChaseVelocityModifier		  << '\n';
	fsScriptOut <<blueVelocityModifier		  << '\n';
	fsScriptOut <<blueEvadeVelocityModifier		  << '\n';
	fsScriptOut <<redVelocityModifier		  << '\n';
	fsScriptOut <<redEvadeVelocityModifier		  << '\n';
	fsScriptOut <<laserVelocityModifier		  << '\n';
	fsScriptOut <<missileVelocityModifier		  << '\n';
	fsScriptOut <<redTurnRate		  << '\n';
	fsScriptOut <<blueTurnRate		  << '\n';
	fsScriptOut <<redBehindTurnRate		  << '\n';
	fsScriptOut <<blueEvadeTurnRate		  << '\n';
	fsScriptOut <<yellowTurnRate		  << '\n';
	fsScriptOut <<yellowChaseTurnRate		  << '\n';
	fsScriptOut <<missileTurnRate		  << '\n';
	fsScriptOut <<missileLowerTurnRate		  << '\n';
	fsScriptOut <<laserTurnRate		  << '\n';

	fsScriptOut <<red_to_flee_distance		  << '\n';
	fsScriptOut <<blue_to_flee_distance		  << '\n';
	fsScriptOut <<red_to_chase_distance		  << '\n';
	fsScriptOut <<blue_to_chase_distance		  << '\n';
	fsScriptOut <<force_red_to_chase_distance		  << '\n';
	fsScriptOut <<force_red_to_flee_distance		  << '\n';
	fsScriptOut <<yellow_stealth_chase_radius		  << '\n';
	fsScriptOut <<yellow_not_stealth_chase_radius		  << '\n';
	fsScriptOut <<yellow_waypoint_range		  << '\n';
	fsScriptOut <<wandering_waypoint_range		  << '\n';
	fsScriptOut <<blue_enemy_missile_fire_timer		  << '\n';
	fsScriptOut <<red_enemy_fire_rate		  << '\n';
	fsScriptOut <<evade_timer		  << '\n';
	fsScriptOut <<blue_evade_timer		  << '\n';
	fsScriptOut <<force_chase_timer		  << '\n';

	return true;
}
