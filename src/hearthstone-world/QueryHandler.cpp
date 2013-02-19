/***
 * Demonstrike Core
 */

#include "StdAfx.h"

//////////////////////////////////////////////////////////////
/// This function handles CMSG_NAME_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleNameQueryOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 8);
	uint64 guid;
	recv_data >> guid;

	PlayerInfo *pn = objmgr.GetPlayerInfo( (uint32)guid );
	if(pn == NULL)
		return;

	// We query our own name on player create so check to send MOTD
	if(!_player->sentMOTD)
	{
		_player->sendMOTD();
		_player->sentMOTD = true;
	}

	DEBUG_LOG("WorldSession","Received CMSG_NAME_QUERY for: %s", pn->name );
	WorldPacket data(SMSG_NAME_QUERY_RESPONSE, 100);
	data << pn->guid << uint32(0);	//highguid
	data << pn->name;
	data << uint8(0);
	data << uint32(pn->race);
	data << uint32(pn->gender);
	data << uint32(pn->_class);
	data << uint8(0);
	SendPacket( &data );
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_QUERY_TIME:
//////////////////////////////////////////////////////////////
void WorldSession::HandleQueryTimeOpcode( WorldPacket & recv_data )
{
	WorldPacket data(SMSG_QUERY_TIME_RESPONSE, 4);
	data << uint32(UNIXTIME);
	SendPacket(&data);
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_CREATURE_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleCreatureQueryOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 12);
	uint32 entry;
	uint64 guid;

	recv_data >> entry;
	recv_data >> guid;

	WorldPacket data(SMSG_CREATURE_QUERY_RESPONSE, 150);
	if(entry == 300000)
	{
		data << (uint32)entry;
		data << "WayPoint";
		data << uint8(0) << uint8(0) << uint8(0);
		data << "Level is WayPoint ID" << "";
		for(uint32 i = 0; i < 8; i++)
			data << uint32(0);
		data << uint8(0);
		SendPacket( &data );
		return;
	}

	CreatureInfo* ci = CreatureNameStorage.LookupEntry(entry);
	if(ci == NULL)
	{
		WorldPacket data(SMSG_CREATURE_QUERY_RESPONSE, 4);
		data << uint32(entry | 0x80000000);
		SendPacket(&data);
		return;
	}

	DEBUG_LOG("WORLD","HandleCreatureQueryOpcode CMSG_CREATURE_QUERY '%s'", ci->Name);
	data << entry;
	data << ci->Name;
	data << uint8(0) << uint8(0) << uint8(0);
	data << ci->SubName;
	data << ci->info_str; //!!! this is a string in 2.3.0 Example: stormwind guard has : "Direction"
	data << ci->Flags1;
	data << ci->Type;
	data << ci->Family;
	data << ci->Rank;
	data << ci->Unknown1;
	data << ci->SpellDataID;
	data << ci->Male_DisplayID;
	data << ci->Female_DisplayID;
	data << ci->Male_DisplayID2;
	data << ci->Female_DisplayID2;
	data << ci->unkfloat1;
	data << ci->unkfloat2;
	data << ci->Leader;
	SendPacket( &data );
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_GAMEOBJECT_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleGameObjectQueryOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 12);
	WorldPacket data(SMSG_GAMEOBJECT_QUERY_RESPONSE, 150);

	uint32 entryID;
	uint64 guid;
	recv_data >> entryID;
	recv_data >> guid;

	DEBUG_LOG("WORLD","HandleGameObjectQueryOpcode CMSG_GAMEOBJECT_QUERY '%u'", entryID);

	GameObjectInfo* goinfo = GameObjectNameStorage.LookupEntry(entryID);
	if(goinfo == NULL)
	{
		WorldPacket data(SMSG_GAMEOBJECT_QUERY_RESPONSE, 4);
		data << uint32(entryID | 0x80000000);
		SendPacket(&data);
		return;
	}

	data << entryID;
	data << goinfo->Type;
	data << goinfo->DisplayID;
	data << goinfo->Name;
	data << uint8(0);
	data << uint8(0);
	data << uint8(0);
	data << goinfo->Icon;
	data << goinfo->CastBarText;
	data << uint8(0);
	for(uint32 d = 0; d < 24; d++)
		data << goinfo->RawData.ListedData[d];
	data << float(1);
	SendPacket( &data );
}

void BuildCorpseInfo(WorldPacket* data, Corpse* corpse)
{
	MapInfo *pMapinfo = WorldMapInfoStorage.LookupEntry(corpse->GetMapId());
	if(pMapinfo == NULL || (pMapinfo->type == INSTANCE_NULL || pMapinfo->type == INSTANCE_PVP))
	{
		*data << uint8(0x01); //show ?
		*data << corpse->GetMapId(); // mapid (that tombstones shown on)
		*data << corpse->GetPositionX();
		*data << corpse->GetPositionY();
		*data << corpse->GetPositionZ();
		*data << corpse->GetMapId();
	}
	else
	{
		*data << uint8(0x01); //show ?
		*data << pMapinfo->repopmapid; // mapid (that tombstones shown on)
		*data << pMapinfo->repopx;
		*data << pMapinfo->repopy;
		*data << pMapinfo->repopz;
		*data << corpse->GetMapId(); //instance mapid (needs to be same as mapid to be able to recover corpse)
	}
}

//////////////////////////////////////////////////////////////
/// This function handles MSG_CORPSE_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleCorpseQueryOpcode(WorldPacket &recv_data)
{
	OUT_DEBUG("WORLD: Received MSG_CORPSE_QUERY");

	Corpse *pCorpse= objmgr.GetCorpseByOwner(GetPlayer()->GetLowGUID());
	if(pCorpse == NULL)
		return;

	WorldPacket data(MSG_CORPSE_QUERY, 21);
	BuildCorpseInfo(&data, pCorpse);
	SendPacket(&data);
}

void WorldSession::HandlePageTextQueryOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 4);
	uint32 pageid = 0;
	recv_data >> pageid;

	WorldPacket data(SMSG_PAGE_TEXT_QUERY_RESPONSE, 300);
	while(pageid)
	{
		data.clear();
		ItemPage * page = ItemPageStorage.LookupEntry(pageid);
		if(page == NULL)
			return;

		char* text = page->text;
		if(text == NULL || *text == NULL)
			return;

		data << pageid;
		data << text;
		data << page->next_page;
		pageid = page->next_page;
		SendPacket(&data);
	}
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_ITEM_NAME_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleItemNameQueryOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 4);
	uint32 itemid;
	recv_data >> itemid;
	SKIP_READ_PACKET(recv_data);

	ItemPrototype *proto = ItemPrototypeStorage.LookupEntry(itemid);
	WorldPacket reply(SMSG_ITEM_NAME_QUERY_RESPONSE, 1000);
	reply << itemid;
	reply << (proto ? proto->Name1 : "Unknown Item");
	SendPacket(&reply);
}

void WorldSession::HandleInrangeQuestgiverQuery(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN();

	WorldPacket data(SMSG_QUESTGIVER_STATUS_MULTIPLE, 1000);
	Object::InRangeSet::iterator itr;
	Creature* pCreature;
	uint32 count = 0;
	data << count;
	for( itr = _player->m_objectsInRange.begin(); itr != _player->m_objectsInRange.end(); itr++ )
	{
		pCreature = TO_CREATURE(*itr);
		if( pCreature->GetTypeId() != TYPEID_UNIT )
			continue;

		if( pCreature->isQuestGiver() )
		{
			data << pCreature->GetGUID();
			data << sQuestMgr.CalcStatus( pCreature, _player );
			++count;
		}
	}

	data.put<uint32>(0, count);
	SendPacket(&data);
}