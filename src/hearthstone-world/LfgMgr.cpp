/***
 * Demonstrike Core
 */

#include "StdAfx.h"

initialiseSingleton( LfgMgr );

LfgMgr::LfgMgr()
{
/*	MaxDungeonID = 0;
	uint32 levelgroup[2];
	levelgroup[0] = LFG_LEVELGROUP_NONE;
	levelgroup[1] = LFG_LEVELGROUP_NONE;
	DBCStorage<LookingForGroup>::iterator itr;
	for(itr = dbcLookingForGroup.begin(); itr != dbcLookingForGroup.end(); ++itr)
	{
		if((*itr)->ID > MaxDungeonID)
			MaxDungeonID = (*itr)->ID;

		levelgroup[0] = GetPlayerLevelGroup((*itr)->minlevel);
		if(levelgroup[0] != LFG_LEVELGROUP_NONE)
			DungeonsByLevel[levelgroup[0]].insert((*itr)->ID);

		levelgroup[1] = GetPlayerLevelGroup((*itr)->maxlevel);
		if(levelgroup[1] != LFG_LEVELGROUP_NONE)
			if(levelgroup[0] != levelgroup[1])
				DungeonsByLevel[levelgroup[1]].insert((*itr)->ID);

		levelgroup[0] = LFG_LEVELGROUP_NONE;
		levelgroup[1] = LFG_LEVELGROUP_NONE;
	}*/
}

LfgMgr::~LfgMgr()
{

}

bool LfgMgr::AttemptLfgJoin(Player* pl, uint32 LfgDungeonId)
{
	return false;
}

uint32 LfgMgr::GetPlayerLevelGroup(uint32 level)
{
	if(level > 80)
		return LFG_LEVELGROUP_80_UP;
	else if(level == 80)
		return LFG_LEVELGROUP_80;
	else if(level >= 70)
		return LFG_LEVELGROUP_70_UP;
	else if(level >= 60)
		return LFG_LEVELGROUP_60_UP;
	else if(level >= 50)
		return LFG_LEVELGROUP_50_UP;
	else if(level >= 40)
		return LFG_LEVELGROUP_40_UP;
	else if(level >= 30)
		return LFG_LEVELGROUP_30_UP;
	else if(level >= 20)
		return LFG_LEVELGROUP_20_UP;
	else if(level >= 10)
		return LFG_LEVELGROUP_10_UP;

	return LFG_LEVELGROUP_NONE;
}

void LfgMgr::SetPlayerInLFGqueue(Player* pl,uint32 LfgDungeonId)
{

}

void LfgMgr::RemovePlayerFromLfgQueues(Player* pl)
{

}

void LfgMgr::RemovePlayerFromLfgQueue( Player* plr, uint32 LfgDungeonId )
{

}

void LfgMgr::UpdateLfgQueue(uint32 LfgDungeonId)
{

}

void LfgMgr::SendLfgList( Player* plr, uint32 Dungeon )
{

}

void LfgMgr::SetPlayerInLfmList(Player* pl, uint32 LfgDungeonId)
{

}

void LfgMgr::RemovePlayerFromLfmList(Player* pl, uint32 LfmDungeonId)
{

}
