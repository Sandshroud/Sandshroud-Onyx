/***
 * Demonstrike Core
 */

#include "StdAfx.h"

bool ChatHandler::HandleRenameAllCharacter(const char * args, WorldSession * m_session)
{
	uint32 uCount = 0;
	uint32 ts = getMSTime();
	QueryResult * result = CharacterDatabase.Query("SELECT guid, name FROM characters");
	if( result )
	{
		do
		{
			uint32 uGuid = result->Fetch()[0].GetUInt32();
			const char * pName = result->Fetch()[1].GetString();
			size_t szLen = strlen(pName);

			if( !sWorld.VerifyName(pName, szLen) )
			{
				printf("renaming character %s, %u\n", pName,uGuid);
				Player* pPlayer = objmgr.GetPlayer(uGuid);
				if( pPlayer != NULL )
				{
					pPlayer->rename_pending = true;
					pPlayer->GetSession()->SystemMessage("Your character has had a force rename set, you will be prompted to rename your character at next login in conformance with server rules.");
				}

				CharacterDatabase.WaitExecute("UPDATE characters SET forced_rename_pending = 1 WHERE guid = %u", uGuid);
				++uCount;
			}

		} while (result->NextRow());
		delete result;
	}

	SystemMessage(m_session, "Procedure completed in %u ms. %u character(s) forced to rename.", getMSTime() - ts, uCount);
	return true;
}

void CapitalizeString(string& arg)
{
	if(arg.length() == 0) return;
	arg[0] = toupper(arg[0]);
	for(uint32 x = 1; x < arg.size(); ++x)
		arg[x] = tolower(arg[x]);
}

void WorldSession::CharacterEnumProc(QueryResult * result)
{
	uint8 num = 0;
	m_asyncQuery = false;

	// should be more than enough.. 200 bytes per char..
	WorldPacket data(SMSG_CHAR_ENUM, (result ? result->GetRowCount() * 200 : 1));

	// parse m_characters and build a mighty packet of
	// characters to send to the client.
	data << num;
	if( result )
	{
		CreatureInfo *info = NULL;
		player_item items[EQUIPMENT_SLOT_END];
		ItemPrototype * proto;
		QueryResult * res;
		Field *fields;
		int8 slot;
		int8 containerslot;
		uint8 Class;
		uint8 race;
		uint32 i;
		uint32 bytes2;
		uint32 flags;
		uint32 banned;
		uint64 guid;
		do
		{
			fields = result->Fetch();
			guid = fields[0].GetUInt32();
			bytes2 = fields[6].GetUInt32();
			Class = fields[3].GetUInt8();
			flags = fields[17].GetUInt32();
			race = fields[3].GetUInt8();

			if( _side < 0 )
			{
				// work out the side
				static uint8 sides[RACE_DRAENEI+1] = { 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0 };
				_side = sides[race];
			}

			/* build character enum, w0000t :p */
			data << fields[0].GetUInt64();		// guid
			data << fields[7].GetString();		// name
			data << fields[2].GetUInt8();		// race
			data << race;						// class
			data << fields[4].GetUInt8();		// gender
			data << fields[5].GetUInt32();		// PLAYER_BYTES
			data << uint8(bytes2 & 0xFF);		// facial hair
			data << fields[1].GetUInt8();		// Level
			data << fields[12].GetUInt32();		// zoneid
			data << fields[11].GetUInt32();		// Mapid
			data << fields[8].GetFloat();		// X
			data << fields[9].GetFloat();		// Y
			data << fields[10].GetFloat();		// Z
			data << fields[18].GetUInt32();		// GuildID

			if( fields[1].GetUInt8() > m_highestLevel )
				m_highestLevel = fields[1].GetUInt8();

			banned = fields[13].GetUInt32();
			if(banned && (banned<10 || banned > (uint32)UNIXTIME))
				data << uint32(0x01A04040);
			else
			{
				if(fields[16].GetUInt32() != 0)
					data << uint32(0x00A04342);
				else if(fields[15].GetUInt32() != 0)
					data << (uint32)8704; // Dead (displaying as Ghost)
				else
					data << uint32(1);		// alive
			}

			data << fields[14].GetUInt8();			// Rest State

			if( Class == WARLOCK || Class == HUNTER )
			{
				res = CharacterDatabase.Query("SELECT entry FROM playerpets WHERE ownerguid="I64FMTD" AND ( active MOD 10 ) =1", guid);

				if(res)
				{
					info = CreatureNameStorage.LookupEntry(res->Fetch()[0].GetUInt32());
					delete res;
				}
				else
					info=NULL;
			}
			else
				info=NULL;

			if(info)  //PET INFO uint32 displayid,	uint32 level,		 uint32 familyid
				data << uint32(info->Male_DisplayID) << uint32(10) << uint32(info->Family);
			else
				data << uint32(0) << uint32(0) << uint32(0);

			res = CharacterDatabase.Query("SELECT containerslot, slot, entry, enchantments FROM playeritems WHERE ownerguid=%u", GUID_LOPART(guid));

			uint32 enchantid;
			EnchantEntry * enc;
			memset(items, 0, sizeof(player_item) * EQUIPMENT_SLOT_END);
			if(res)
			{
				do
				{
					containerslot = res->Fetch()[0].GetInt8();
					slot = res->Fetch()[1].GetInt8();
					if( containerslot == -1 && slot < EQUIPMENT_SLOT_END && slot >= 0 )
					{
						proto = ItemPrototypeStorage.LookupEntry(res->Fetch()[2].GetUInt32());
						if(proto)
						{
							// slot0 = head, slot14 = cloak
							if(!(slot == 0 && (flags & (uint32)PLAYER_FLAG_NOHELM) != 0) && !(slot == 14 && (flags & (uint32)PLAYER_FLAG_NOCLOAK) != 0))
							{
								items[slot].displayid = proto->DisplayInfoID;
								items[slot].invtype = proto->InventoryType;
								if( slot == EQUIPMENT_SLOT_MAINHAND || slot == EQUIPMENT_SLOT_OFFHAND )
								{
									// get enchant visual ID
									const char * enchant_field = res->Fetch()[3].GetString();
									if( sscanf( enchant_field , "%u,0,0;" , (unsigned int *)&enchantid ) == 1 && enchantid > 0 )
									{
										enc = dbcEnchant.LookupEntry( enchantid );
										if( enc != NULL )
											items[slot].enchantment = enc->visual;
										else
											items[slot].enchantment = 0;;
									}
								}
							}
						}
					}
				} while(res->NextRow());
				delete res;
			}

			for( i = 0; i < EQUIPMENT_SLOT_END; i++ )
				data << items[i].displayid << items[i].invtype << uint32(items[i].enchantment);
			data << uint32(0) << uint8(0) << uint32(0);

			num++;
		}
		while( result->NextRow() );
	}

	data.put<uint8>(0, num);

	//OUT_DEBUG("Character Enum", "Built in %u ms.", getMSTime() - start_time);
	SendPacket( &data );
}

void WorldSession::HandleCharEnumOpcode( WorldPacket & recv_data )
{
	if( m_asyncQuery )		// should be enough
		return;

	AsyncQuery * q = new AsyncQuery( new SQLClassCallbackP1<World, uint32>(World::getSingletonPtr(), &World::CharacterEnumProc, GetAccountId()) );
	q->AddQuery("SELECT guid, level, race, class, gender, bytes, bytes2, name, positionX, positionY, positionZ, mapId, zoneId, banned, restState, deathstate, forced_rename_pending, player_flags, guild_data.guildid FROM characters LEFT JOIN guild_data ON characters.guid = guild_data.playerid WHERE acct=%u ORDER BY guid ASC LIMIT 10", GetAccountId());
	m_asyncQuery = true;
	CharacterDatabase.QueueAsyncQuery(q);
}

void WorldSession::LoadAccountDataProc(QueryResult * result)
{
	size_t len;
	const char * data;
	char * d;

	if(!result)
	{
		CharacterDatabase.Execute("INSERT INTO account_data VALUES(%u, '', '', '', '', '', '', '', '', '')", _accountId);
		return;
	}

	for(uint32 i = 0; i < 8; i++)
	{
		data = result->Fetch()[1+i].GetString();
		len = data ? strlen(data) : 0;
		if(len > 1)
		{
			d = new char[len+1];
			memcpy(d, data, len+1);
			SetAccountData(i, d, true, (uint32)len);
		}
	}
}

void WorldSession::HandleCharCreateOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 10);
	std::string name;
	uint8 race, class_;

	recv_data >> name >> race >> class_;
	recv_data.rpos(0);

	WorldPacket data(SMSG_CHAR_CREATE, 1);
	if(!sWorld.VerifyName(name.c_str(), name.length()))
	{
		data << uint8(CHAR_CREATE_IN_USE);
		SendPacket(&data);
		return;
	}

	if(g_characterNameFilter->Parse(name, false))
	{
		data << uint8(CHAR_CREATE_IN_USE);
		SendPacket(&data);
		return;
	}

	//reserved for console whisper
	if(name == "Console" ||  name == "console")
	{
		data << uint8(CHAR_CREATE_IN_USE);
		SendPacket(&data);
		return;
	}

	if(objmgr.GetPlayerInfoByName(name.c_str()) != 0)
	{
		data << uint8(CHAR_CREATE_IN_USE);
		SendPacket(&data);
		return;
	}

	if(!sHookInterface.OnNewCharacter(race, class_, this, name.c_str()))
	{
		data << uint8(CHAR_CREATE_IN_USE);
		SendPacket(&data);
		return;
	}

	QueryResult * result = CharacterDatabase.Query("SELECT COUNT(*) FROM banned_names WHERE name = '%s'", CharacterDatabase.EscapeString(name).c_str());
	if(result)
	{
		if(result->Fetch()[0].GetUInt32() > 0)
		{
			// That name is banned!
			data << uint8(CHAR_NAME_PROFANE);
			SendPacket(&data);
			delete result;
			return;
		}
		delete result;
	}
	// loading characters

	//checking number of chars is useless since client will not allow to create more than 10 chars
	//as the 'create' button will not appear (unless we want to decrease maximum number of characters)
	Player* pNewChar = objmgr.CreatePlayer();
	pNewChar->SetSession(this);
	if(!pNewChar->Create( recv_data ))
	{
		// failed.
		pNewChar->ok_to_remove = true;
		pNewChar->Destruct();
		pNewChar = NULLPLR;
		data << uint8(CHAR_CREATE_ERROR);
		SendPacket(&data);
		return;
	}

	//Same Faction limitation only applies to PVP and RPPVP realms :)
	// Todo.

	pNewChar->UnSetBanned();
	pNewChar->addSpell(22027);	  // Remove Insignia

	if(pNewChar->getClass() == WARLOCK)
	{
		pNewChar->AddSummonSpell(416, 3110);		// imp fireball
		pNewChar->AddSummonSpell(417, 19505);
		pNewChar->AddSummonSpell(1860, 3716);
		pNewChar->AddSummonSpell(1863, 7814);
	}

	pNewChar->SaveToDB(true);

	PlayerInfo *pn = new PlayerInfo;
	memset(pn, 0, sizeof(PlayerInfo));
	pn->guid = pNewChar->GetLowGUID();
	pn->name = strdup(pNewChar->GetName());
	pn->_class = pNewChar->getClass();
	pn->race = pNewChar->getRace();
	pn->gender = pNewChar->getGender();
	pn->lastLevel = pNewChar->getLevel();
	pn->lastZone = pNewChar->GetZoneId();
	pn->lastOnline = UNIXTIME;
	pn->team = pNewChar->GetTeam();
	pn->acct = GetAccountId();
	objmgr.AddPlayerInfo(pn);

	pNewChar->ok_to_remove = true;
	pNewChar->Destruct();
	pNewChar = NULLPLR;

	// CHAR_CREATE_SUCCESS
	data << uint8(CHAR_CREATE_SUCCESS);
	SendPacket(&data);

	sLogonCommHandler.UpdateAccountCount(GetAccountId(), 1);
}

void WorldSession::HandleCharDeleteOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 8);
	uint8 fail = CHAR_DELETE_SUCCESS;

	uint64 guid;
	recv_data >> guid;

	if(objmgr.GetPlayer((uint32)guid) != NULL)
		fail = CHAR_DELETE_FAILED; // "Char deletion failed"
	else
		fail = DeleteCharacter((uint32)guid);

	OutPacket(SMSG_CHAR_DELETE, 1, &fail);
	if(fail == CHAR_DELETE_SUCCESS)
		sLogonCommHandler.UpdateAccountCount(GetAccountId(), -1);
}

uint8 WorldSession::DeleteCharacter(uint32 guid)
{
	PlayerInfo * inf = objmgr.GetPlayerInfo(guid);
	if( inf != NULL && inf->m_loggedInPlayer == NULL )
	{
		QueryResult * result = CharacterDatabase.Query("SELECT name FROM characters WHERE guid = %u AND acct = %u", (uint32)guid, _accountId);
		if(!result)
			return CHAR_DELETE_FAILED;

		if(inf->GuildId)
		{
			GuildMember* gMember = guildmgr.GetGuildMember(inf->guid);
			if(gMember->pRank->Rank.iId == 0)
				return CHAR_DELETE_FAILED;
			else
				guildmgr.RemoveMember(NULL, inf);
		}

		string name = result->Fetch()[0].GetString();
		delete result;

		for(int i = 0; i < NUM_CHARTER_TYPES; i++)
		{
			if( inf->charterId[i] != 0 )
			{
				Charter *pCharter = guildmgr.GetCharter(inf->charterId[i], (CharterTypes)i);
				if( pCharter->LeaderGuid == inf->guid )
					pCharter->Destroy();
				else
					pCharter->RemoveSignature(inf->guid);
			}
		}

		for(int i = 0; i < NUM_ARENA_TEAM_TYPES; i++)
		{
			if( inf->arenaTeam[i] != NULL )
			{
				if( inf->arenaTeam[i]->m_leader == guid )
					return CHAR_DELETE_FAILED;
				else
					inf->arenaTeam[i]->RemoveMember(inf);
			}
		}

		/*if( _socket != NULL )
			sPlrLog.write("Account: %s | IP: %s >> Deleted player %s", GetAccountName().c_str(), GetSocket()->GetRemoteIP().c_str(), name.c_str());*/

		sWorld.LogPlayer(this, "deleted character %s (GUID: %u)", name.c_str(), (uint32)guid);

/*		if ( sWorld.char_restore_enabled )
		{
			//insert data into "deleted" tables
			CharacterDatabase.WaitExecute("INSERT INTO characters_deleted SELECT UNIX_TIMESTAMP(NOW()),characters.* FROM characters WHERE guid = %u", (uint32)guid);
			CharacterDatabase.WaitExecute("INSERT INTO playerglyphs_deleted SELECT playerglyphs.* FROM playerglyphs WHERE guid = %u", (uint32)guid);
			CharacterDatabase.WaitExecute("INSERT INTO playeritems_deleted SELECT playeritems.* FROM playeritems WHERE ownerguid = %u", (uint32)guid);
			CharacterDatabase.WaitExecute("INSERT INTO playerskills_deleted SELECT playerskills.* FROM playerskills WHERE player_guid = %u", (uint32)guid);
			CharacterDatabase.WaitExecute("INSERT INTO playerspells_deleted SELECT playerspells.* FROM playerspells WHERE guid = %u", (uint32)guid);
			CharacterDatabase.WaitExecute("INSERT INTO playertalents_deleted SELECT playertalents.* FROM playertalents WHERE guid = %u", (uint32)guid);
			CharacterDatabase.WaitExecute("INSERT INTO questlog_deleted SELECT questlog.* FROM questlog WHERE player_guid = %u", (uint32)guid);
		}*/

		CharacterDatabase.WaitExecute("DELETE FROM characters WHERE guid = %u", (uint32)guid);

		Corpse* c=objmgr.GetCorpseByOwner((uint32)guid);
		if(c)
			CharacterDatabase.Execute("DELETE FROM corpses WHERE guid = %u", c->GetLowGUID());

		CharacterDatabase.Execute("DELETE FROM auctions WHERE owner = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM charters WHERE leaderGuid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM gm_tickets WHERE guid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM guild_data WHERE playerid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM instances WHERE creator_guid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM mailbox WHERE player_guid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM playercooldowns WHERE player_guid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM playerglyphs WHERE guid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM playeritems WHERE ownerguid=%u",(uint32)guid);
		CharacterDatabase.Execute("DELETE FROM playerpets WHERE ownerguid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM playerpetspells WHERE ownerguid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM playerskills WHERE player_guid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM playerspells WHERE guid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM playersummonspells WHERE ownerguid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM playertalents WHERE guid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM questlog WHERE player_guid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM social_friends WHERE character_guid = %u OR friend_guid = %u", (uint32)guid, (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM social_ignores WHERE character_guid = %u OR ignore_guid = %u", (uint32)guid, (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM tutorials WHERE playerId = %u", (uint32)guid);

		/* remove player info */
		objmgr.DeletePlayerInfo((uint32)guid);
		return CHAR_DELETE_SUCCESS;
	}

	return CHAR_DELETE_FAILED;
}

void WorldSession::HandleCharRenameOpcode(WorldPacket & recv_data)
{
	WorldPacket data(SMSG_CHAR_RENAME, recv_data.size() + 1);

	uint64 guid;
	string name;
	recv_data >> guid >> name;

	PlayerInfo * pi = objmgr.GetPlayerInfo((uint32)guid);
	if(pi == NULL)
		return;

	QueryResult * result = CharacterDatabase.Query("SELECT forced_rename_pending FROM characters WHERE guid = %u AND acct = %u", (uint32)guid, _accountId);
	if(result == NULL)
		return;
	delete result;

	// Check name for rule violation.
	const char * szName=name.c_str();
	for(uint32 x = 0; x < strlen(szName); x++)
	{
		if((int)szName[x] < 65 || ((int)szName[x] > 90 && (int)szName[x] < 97) || (int)szName[x] > 122)
		{
			if((int)szName[x] < 65)
			{
				data << uint8(CHAR_NAME_TOO_SHORT); // Name is too short.
			}
			else if((int)szName[x] > 122) // Name is too long.
			{
				data << uint8(CHAR_NAME_TOO_LONG);
			}
			else
			{
				data << uint8(CHAR_NAME_FAILURE); // No clue.
			}
			data << guid << name;
			SendPacket(&data);
			return;
		}
	}

	QueryResult * result2 = CharacterDatabase.Query("SELECT COUNT(*) FROM banned_names WHERE name = '%s'", CharacterDatabase.EscapeString(name).c_str());
	if(result2)
	{
		if(result2->Fetch()[0].GetUInt32() > 0)
		{
			// That name is banned!
			data << uint8(CHAR_NAME_PROFANE);
			data << guid << name;
			SendPacket(&data);
			delete result2;
			return;
		}
		delete result2;
	}

	// Check if name is in use.
	if(objmgr.GetPlayerInfoByName(name.c_str()) != 0)
	{
		data << uint8(CHAR_NAME_FAILURE);
		data << guid << name;
		SendPacket(&data);
		return;
	}

	// correct capitalization
	CapitalizeString(name);
	objmgr.RenamePlayerInfo(pi, pi->name, name.c_str());

	sWorld.LogPlayer(this, "a rename was pending. Renamed character %s (GUID: %u) to %s.", pi->name, pi->guid, name.c_str());

	// If we're here, the name is okay.
	CharacterDatabase.Query("UPDATE characters SET name = \'%s\',  forced_rename_pending  = 0 WHERE guid = %u AND acct = %u",name.c_str(), (uint32)guid, _accountId);
	free(pi->name);
	pi->name = strdup(name.c_str());

	data << uint8(0) << guid << name;
	SendPacket(&data);
}

void WorldSession::HandlePlayerLoginOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 8);
	uint64 playerGuid = 0;

	DEBUG_LOG( "WorldSession"," Recvd Player Logon Message" );

	recv_data >> playerGuid; // this is the GUID selected by the player

	//Better validate this Guid before we create an invalid _player.
	uint8 response = CHAR_LOGIN_NO_CHARACTER;

	//already active?
	if(objmgr.GetPlayer((uint32)playerGuid) != NULL || m_loggingInPlayer || _player)
		response = CHAR_LOGIN_DUPLICATE_CHARACTER;
	else //Do we exist in DB yet?
	{
		PlayerInfo * plrInfo = objmgr.GetPlayerInfo(playerGuid);
		if( plrInfo )
			response = CHAR_LOGIN_SUCCESS;
	}

	if(response != CHAR_LOGIN_SUCCESS)
	{
		OutPacket(SMSG_CHARACTER_LOGIN_FAILED, 1, &response);
		return;
	}

	//We have a valid Guid so let's create the player and login
	Player* plr = new Player((uint32)playerGuid);
	plr->Init();
	plr->SetSession(this);
	m_bIsWLevelSet = false;

	DEBUG_LOG("WorldSession", "Async loading player %u", (uint32)playerGuid);
	m_loggingInPlayer = plr;
	plr->LoadFromDB((uint32)playerGuid);
}

void WorldSession::FullLogin(Player* plr)
{
	DEBUG_LOG("WorldSession", "Fully loading player %u", plr->GetLowGUID());
	SetPlayer(plr);
	m_MoverWoWGuid.Init(plr->GetGUID());

	// copy to movement array
	plr->movement_packet[0] = m_MoverWoWGuid.GetNewGuidMask();
	memcpy(&plr->movement_packet[1], m_MoverWoWGuid.GetNewGuid(), m_MoverWoWGuid.GetNewGuidLen());

	WorldPacket dataa(MSG_SET_DUNGEON_DIFFICULTY, 20);
	dataa << plr->iInstanceType;
	dataa << uint32(0x01);
	dataa << uint32(0x00);
	SendPacket(&dataa);

	/* world preload */
	packetSMSG_LOGIN_VERIFY_WORLD vwpck;
	vwpck.MapId = plr->GetMapId();
	vwpck.O = plr->GetOrientation();
	vwpck.X = plr->GetPositionX();
	vwpck.Y = plr->GetPositionY();
	vwpck.Z = plr->GetPositionZ();
	OutPacket( SMSG_LOGIN_VERIFY_WORLD, sizeof(packetSMSG_LOGIN_VERIFY_WORLD), &vwpck );

	WorldPacket datab(SMSG_FEATURE_SYSTEM_STATUS, 2);
	datab << uint8(2) << uint8(0);
	SendPacket(&datab);

	plr->UpdateStats();

	// Anti max level hack.
	if(sWorld.LevelCap_Custom_All && (plr->getLevel() > sWorld.LevelCap_Custom_All))
		plr->SetUInt32Value(UNIT_FIELD_LEVEL, sWorld.LevelCap_Custom_All);

	// Enable certain GM abilities on login.
	if(HasGMPermissions())
	{
		plr->bGMTagOn = true;
		plr->m_isGmInvisible = true;
		plr->m_invisible = true;
		plr->bInvincible = true;
		plr->SetFlag(PLAYER_FLAGS, PLAYER_FLAG_GM);
		if(CanUseCommand('z'))
			plr->triggerpass_cheat = true; // Enable for admins automatically.
	}

	// Make sure our name exists (for premade system)
	PlayerInfo * info = objmgr.GetPlayerInfo(plr->GetLowGUID());
	if(info == NULL)
	{
		info = new PlayerInfo;
		memset(info, 0, sizeof(PlayerInfo));
		info->_class = plr->getClass();
		info->gender = plr->getGender();
		info->guid = plr->GetLowGUID();
		info->name = strdup(plr->GetName());
		info->lastLevel = plr->getLevel();
		info->lastOnline = UNIXTIME;
		info->lastZone = plr->GetZoneId();
		info->race = plr->getRace();
		info->team = plr->GetTeam();
		objmgr.AddPlayerInfo(info);
	}

	plr->m_playerInfo = info;
	if(plr->m_playerInfo->GuildId)
	{
		plr->m_uint32Values[PLAYER_GUILDID] = plr->m_playerInfo->GuildId;
		plr->m_uint32Values[PLAYER_GUILDRANK] = plr->m_playerInfo->GuildRank;
	}

	for(uint32 z = 0; z < NUM_ARENA_TEAM_TYPES; ++z)
	{
		if(plr->m_playerInfo->arenaTeam[z] != NULL)
		{
			plr->SetUInt32Value(PLAYER_FIELD_ARENA_TEAM_INFO_1_1 + (z*6), plr->m_playerInfo->arenaTeam[z]->m_id);
			if(plr->m_playerInfo->arenaTeam[z]->m_leader == plr->GetLowGUID())
				plr->SetUInt32Value(PLAYER_FIELD_ARENA_TEAM_INFO_1_1 + (z*6) + 1, 0);
			else
				plr->SetUInt32Value(PLAYER_FIELD_ARENA_TEAM_INFO_1_1 + (z*6) + 1, 1);
		}
	}

	info->m_loggedInPlayer = plr;

	// account data == UI config
	if(sWorld.m_useAccountData)
	{
		WorldPacket data(SMSG_ACCOUNT_DATA_TIMES, 128);
		MD5Hash md5hash;
		for (int i = 0; i < 8; i++)
		{
			AccountDataEntry* acct_data = GetAccountData(i);
			if (!acct_data->data)
			{
				data << uint64(0) << uint64(0);	// Nothing.
				continue;
			}

			md5hash.Initialize();
			md5hash.UpdateData((const uint8*)acct_data->data, acct_data->sz);
			md5hash.Finalize();

			data.append(md5hash.GetDigest(), MD5_DIGEST_LENGTH);
		}
		SendPacket(&data);
	}
	else
	{
		WorldPacket data(SMSG_ACCOUNT_DATA_TIMES, 4+1+4+8*4);
		MD5Hash md5hash;
		for (int i = 0; i < 8; i++)
			data << uint64(0) << uint64(0);	// Nothing.
		SendPacket(&data);
	}

	// Set TIME OF LOGIN
	CharacterDatabase.Execute("UPDATE characters SET online = 1 WHERE guid = %u" , plr->GetLowGUID());

	bool enter_world = true;

	// Find our transporter and add us if we're on one.
	if(plr->m_TransporterGUID != 0)
	{
		Transporter* pTrans = objmgr.GetTransporter(GUID_LOPART(plr->m_TransporterGUID));
		if(pTrans)
		{
			if(plr->isDead())
			{
				plr->RemoteRevive();
			}

			float c_tposx = pTrans->GetPositionX() + plr->m_transportPosition->x;
			float c_tposy = pTrans->GetPositionY() + plr->m_transportPosition->y;
			float c_tposz = pTrans->GetPositionZ() + plr->m_transportPosition->z;
			if(plr->GetMapId() != pTrans->GetMapId())	// loaded wrong map
			{
				plr->SetMapId(pTrans->GetMapId());

				WorldPacket dataw(SMSG_NEW_WORLD, 20);
				dataw << pTrans->GetMapId() << c_tposx << c_tposy << c_tposz << plr->GetOrientation();
				SendPacket(&dataw);

				// shit is sent in worldport ack.
				enter_world = false;
			}

			plr->SetPosition(c_tposx, c_tposy, c_tposz, plr->GetOrientation(), false);
			plr->m_CurrentTransporter = pTrans;
			pTrans->AddPlayer(plr);
		}
	}

	DEBUG_LOG( "WorldSession","Player %s logged in.", plr->GetName());

	if(plr->GetTeam() == 1)
		sWorld.HordePlayers++;
	else
		sWorld.AlliancePlayers++;

	if(sWorld.SendMovieOnJoin && plr->m_FirstLogin && !HasGMPermissions())
		plr->SendCinematic(plr->myRace->cinematic_id);

	DEBUG_LOG( "WorldSession","Created new player for existing players (%s)", plr->GetName() );

	// Login time, will be used for played time calc
	plr->m_playedtime[2] = (uint32)UNIXTIME;

	// Send online status to people having this char in friendlist
	plr->Social_TellOnlineStatus();

	// send friend list (for ignores)
	plr->Social_SendFriendList(7);

	// send to gms
	if(HasGMPermissions())
		sWorld.SendMessageToGMs(this, "%s%s %s (%s) is now online.|r", MSG_COLOR_GOLD, CanUseCommand('z') ? "Admin" : "GameMaster", plr->GetName(), GetAccountNameS(), GetPermissions());

	//Set current RestState
	if( plr->m_isResting) 		// We are in a resting zone, turn on Zzz
		plr->ApplyPlayerRestState(true);

	//Check if there is a time difference between lastlogoff and now
	if( plr->m_timeLogoff > 0 && plr->GetUInt32Value(UNIT_FIELD_LEVEL) < plr->GetUInt32Value(PLAYER_FIELD_MAX_LEVEL))	// if timelogoff = 0 then it's the first login
	{
		uint32 currenttime = (uint32)UNIXTIME;
		uint32 timediff = currenttime - plr->m_timeLogoff;

		//Calculate rest bonus
		if( timediff > 0 )
			plr->AddCalculatedRestXP(timediff);
	}

	sHookInterface.OnFullLogin(plr);

	if(info->m_Group)
		info->m_Group->Update();

	if(enter_world && !plr->GetMapMgr())
		plr->AddToWorld(true);

	sTracker.CheckPlayerForTracker(plr, true);

	if(plr->GetItemInterface())
		plr->GetItemInterface()->CheckAreaItems();
	objmgr.AddPlayer(plr);
}

bool ChatHandler::HandleRenameCommand(const char * args, WorldSession * m_session)
{
	// prevent buffer overflow
	if(strlen(args) > 100)
		return false;

	char name1[100];
	char name2[100];

	if(sscanf(args, "%s %s", name1, name2) != 2)
		return false;

	if(!sWorld.VerifyName(name2, strlen(name2)))
	{
		RedSystemMessage(m_session, "That name is invalid or contains invalid characters.");
		return true;
	}

	string new_name = name2;
	PlayerInfo * pi = objmgr.GetPlayerInfoByName(name1);
	if(pi == 0)
	{
		RedSystemMessage(m_session, "Player not found with this name.");
		return true;
	}

	if( objmgr.GetPlayerInfoByName(new_name.c_str()) != NULL )
	{
		RedSystemMessage(m_session, "Player found with this name in use already.");
		return true;
	}

	objmgr.RenamePlayerInfo(pi, pi->name, new_name.c_str());

	free(pi->name);
	pi->name = strdup(new_name.c_str());

	// look in world for him
	Player* plr = objmgr.GetPlayer(pi->guid);
	if(plr != 0)
	{
		plr->SetName(new_name);
		BlueSystemMessageToPlr(plr, "%s changed your name to '%s'.", m_session->GetPlayer()->GetName(), new_name.c_str());
		plr->SaveToDB(false);
	}
	else
	{
		CharacterDatabase.WaitExecute("UPDATE characters SET name = '%s' WHERE guid = %u", CharacterDatabase.EscapeString(new_name).c_str(), (uint32)pi->guid);
	}

	GreenSystemMessage(m_session, "Changed name of '%s' to '%s'.", (char*)name1, (char*)name2);
	sWorld.LogGM(m_session, "renamed character %s (GUID: %u) to %s", (char*)name1, pi->guid, (char*)name2);
	sWorld.LogPlayer(m_session, "GM renamed character %s (GUID: %u) to %s", (char*)name1, pi->guid, ((char*)name2));
	return true;
}
