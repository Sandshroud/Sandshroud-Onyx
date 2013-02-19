/***
 * Demonstrike Core
 */

#include "StdAfx.h"

void ApplyNormalFixes()
{
	SetupSpellTargets();

	SpellEntry *sp;
	uint32 cnt = dbcSpell.GetNumRows();
	Log.Notice("World", "Processing %u spells...", cnt);

	map<uint32, uint32> talentSpells;
	map<uint32,uint32>::iterator talentSpellIterator;
	unsigned int i,j;
	for(i = 0; i < dbcTalent.GetNumRows(); ++i)
	{
		TalentEntry * tal = dbcTalent.LookupRow(i);
		for(j = 0; j < 5; ++j)
			if(tal->RankID[j] != 0)
				talentSpells.insert(make_pair(tal->RankID[j], tal->TalentTree));
	}

	for(uint32 x = 0; x < cnt; x++)
	{
		sp = dbcSpell.LookupRow(x);
		if(sp == NULL)
			continue;

		SetSingleSpellDefaults(sp);
		ApplySingleSpellFixes(sp);
		ApplyCoeffSpellFixes(sp);
	}
}

// Kroze: Some commented stuff.

// NEW SCHOOLS AS OF 2.4.0:
/* (bitwise)
SCHOOL_NORMAL = 1,
SCHOOL_HOLY   = 2,
SCHOOL_FIRE   = 4,
SCHOOL_NATURE = 8,
SCHOOL_FROST  = 16,
SCHOOL_SHADOW = 32,
SCHOOL_ARCANE = 64

//where do i use this ?

AURASTATE_FLAG_DODGE_BLOCK			= 1,		//1
AURASTATE_FLAG_HEALTH20				= 2,		//2
AURASTATE_FLAG_BERSERK				= 4,		//3
AURASTATE_FLAG_JUDGEMENT			= 16,		//5
AURASTATE_FLAG_PARRY				= 64,		//7
AURASTATE_FLAG_LASTKILLWITHHONOR	= 512,		//10
AURASTATE_FLAG_CRITICAL				= 1024,		//11
AURASTATE_FLAG_HEALTH35				= 4096,		//13
AURASTATE_FLAG_IMMOLATE				= 8192,		//14
AURASTATE_FLAG_REJUVENATE			= 16384,	//15
AURASTATE_FLAG_POISON				= 32768,	//16
*/

uint32 GetTriggerSpellFromDescription(std::string delimiter, std::string desc)
{
	std::string token;

	// find the delimiter.
	size_t i = desc.find(delimiter);
	if (i == string::npos)
		return 0;

	// find the first character of the spell id.
	size_t j = desc.find_first_not_of(delimiter, i);
	if (j == string::npos)
		return 0;

	// find the end of the spell id.
	size_t k = desc.find("s1", j);
	if (k == string::npos)
		return 0;

	// get our token
	token = desc.substr(j, k - j);

	// convert to int
	uint32 id = 0;
	std::istringstream iss(token);
	iss >> id;

	// and return our spell id
	return id;
}

SpellEntry* CreateDummySpell(uint32 id)
{
	std::string name = "Dummy Trigger";
	SpellEntry* sp = new SpellEntry();
	memset(sp, 0, sizeof(SpellEntry*));
	sp->Id = id;
	sp->Attributes = 384;
	sp->AttributesEx = 268435456;
	sp->Flags3 = 4;
	sp->Name = ((char*)name.c_str());
	sp->Rank = ((char*)" ");
	sp->Description = ((char*)" ");
	sp->CastingTimeIndex = 1;
	sp->procChance = 75;
	sp->rangeIndex = 13;
	sp->spellLevel = 0;
	sp->EquippedItemClass = uint32(-1);
	sp->Effect[0] = SPELL_EFFECT_DUMMY;
	sp->EffectImplicitTargetA[0] = 25;
	sp->NameHash = crc32((const unsigned char*)name.c_str(), (unsigned int)name.length());
	sp->dmg_multiplier[0] = 1.0f;
	sp->StanceBarOrder = -1;
	dbcSpell.SetRow(id, sp);
	sWorld.dummyspells.push_back(sp);
	return sp;
}

uint32 GetSpellClass(SpellEntry *sp)
{
	switch(sp->skilline)
	{
	case SKILL_ARMS:
	case SKILL_FURY:
	case SKILL_PROTECTION:
		return WARRIOR;
	case SKILL_HOLY2:
	case SKILL_PROTECTION2:
	case SKILL_RETRIBUTION:
		return PALADIN;
	case SKILL_BEAST_MASTERY:
	case SKILL_SURVIVAL:
	case SKILL_MARKSMANSHIP:
		return HUNTER;
	case SKILL_ASSASSINATION:
	case SKILL_COMBAT:
	case SKILL_SUBTLETY:
		return ROGUE;
	case SKILL_DISCIPLINE:
	case SKILL_HOLY:
	case SKILL_SHADOW:
		return PRIEST;
	case SKILL_ENHANCEMENT:
	case SKILL_RESTORATION:
	case SKILL_ELEMENTAL_COMBAT:
		return SHAMAN;
	case SKILL_FROST:
	case SKILL_FIRE:
	case SKILL_ARCANE:
		return MAGE;
	case SKILL_AFFLICTION:
	case SKILL_DEMONOLOGY:
	case SKILL_DESTRUCTION:
		return WARLOCK;
	case SKILL_RESTORATION2:
	case SKILL_BALANCE:
	case SKILL_FERAL_COMBAT:
		return DRUID;
	}

	return 0;
}

uint32 fill( uint32* arr, ... ) // fills array 'arr' with integers in arguments and returns its new size. Last argument must be 0!
{
	va_list vl;
	uint32 i;
	va_start( vl, arr );
	for( i = 0; i < 100; i++ ){
		arr[i] = va_arg( vl, uint32 );
		if(arr[i] == 0)
			break;
	}
	va_end( vl );
	return i;
}

// Generates SpellNameHashes.h
void GenerateNameHashesFile()
{
	const uint32 fieldSize = 81;
	const char* prefix = "SPELL_HASH_";
	uint32 prefixLen = uint32(strlen(prefix));
	DBCFile dbc;

	if( !dbc.open( "DBC/Spell.dbc" ) )
	{
		Log.Error("World", "Cannot find file ./DBC/Spell.dbc" );
		return;
	}

	uint32 cnt = (uint32)dbc.getRecordCount();
	uint32 namehash = 0;
	FILE * f = fopen("SpellNameHashes.h", "w");
	char spaces[fieldSize], namearray[fieldSize];
	strcpy(namearray, prefix);
	char* name = &namearray[prefixLen];
	for(int i = 0;i < fieldSize-1; ++i)
		spaces[i] = ' ';

	std::set<uint32> namehashes;

	spaces[fieldSize-1] = 0;
	uint32 nameTextLen = 0, nameLen = 0;
	for(uint32 x = 0; x < cnt; x++)
	{
		const char* nametext = dbc.getRecord(x).getString(136);
		nameTextLen = (unsigned int)strlen(nametext);
		strncpy(name, nametext, fieldSize-prefixLen-2);	// Cut it to fit in field size
		name[fieldSize-prefixLen-2] = 0; // in case nametext is too long and strncpy didn't copy the null
		nameLen = (unsigned int)strlen(name);
		for(uint32 i = 0;i<nameLen;++i)
		{
			if(name[i] >= 'a' && name[i] <= 'z')
				name[i] = toupper(name[i]);
			else if(!(name[i] >= '0' && name[i] <= '9') &&
				!(name[i] >= 'A' && name[i] <= 'Z'))
				name[i] = '_';
		}

		namehash = crc32((const unsigned char*)nametext, nameTextLen);

		if(namehashes.find(namehash) != namehashes.end())
			continue; // Skip namehashes we've already done.

		int32 numSpaces = fieldSize-prefixLen-nameLen-1;
		if(numSpaces < 0)
			fprintf(f, "WTF");

		spaces[numSpaces] = 0;
		fprintf(f, "#define %s%s0x%08X\n", namearray, spaces, namehash);
		spaces[numSpaces] = ' ';
		namehashes.insert(namehash);
	}
	fclose(f);
}

// Copies effect number 'fromEffect' in 'fromSpell' to effect number 'toEffect' in 'toSpell'
void CopyEffect(SpellEntry *fromSpell, uint8 fromEffect, SpellEntry *toSpell, uint8 toEffect)
{
	if(!fromSpell || !toSpell || fromEffect > 2 || toEffect > 2)
		return;

	uint32 *from = fromSpell->Effect;
	uint32 *to = toSpell->Effect;
	// Copy 20 values starting at Effect
	for(uint8 index = 0;index < 20;index++)
	{
		to[index * 3 + toEffect] = from[index * 3 + fromEffect];
	}
}

void SetSingleSpellDefaults(SpellEntry *sp)
{
	/// Custom defaults
	sp->forced_creature_target = 0;
	sp->AdditionalAura = 0;
	sp->poison_type = 0;
	sp->self_cast_only = false;
	sp->Unique = false;
	sp->apply_on_shapeshift_change = false;
	sp->always_apply = false;
	sp->proc_interval = 0; //trigger at each event
	sp->ProcsPerMinute = 0;
	sp->c_is_flags = 0;
	sp->isAOE = false;
	sp->AP_coef_override = 0.0f;
	sp->APOT_coef_override = 0.0f;
	sp->SP_coef_override = 0.0f;
	sp->SPOT_coef_override = 0.0f;
	sp->auraimmune_flag = 0;
	sp->AllowBackAttack = false;
	sp->procflags2 = 0.0f;
	sp->cone_width = 0.0f;
	sp->area_aura_update_interval = 2000;
	sp->trnr_req_clsmsk = 0;

	float radius = 0.0f;
	if(sp->EffectRadiusIndex[0] != 0)
		radius = ::GetDBCRadius(dbcSpellRadius.LookupEntry(sp->EffectRadiusIndex[0]));
	if( sp->EffectRadiusIndex[1] != 0 )
		radius = std::max(radius,::GetDBCRadius(dbcSpellRadius.LookupEntry(sp->EffectRadiusIndex[1])));
	if( sp->EffectRadiusIndex[2] != 0 )
		radius = std::max(::GetDBCRadius(dbcSpellRadius.LookupEntry(sp->EffectRadiusIndex[2])),radius);
	radius = std::max(GetDBCMaxRange(dbcSpellRange.LookupEntry(sp->rangeIndex)), radius);

	sp->base_range_or_radius = radius;
	sp->base_range_or_radius_sqr = radius*radius;

	radius = 0.0f;
	if(sp->EffectRadiusIndex[0] != 0)
		radius = ::GetDBCFriendlyRadius(dbcSpellRadius.LookupEntry(sp->EffectRadiusIndex[0]));
	if( sp->EffectRadiusIndex[1] != 0 )
		radius = std::max(radius, ::GetDBCFriendlyRadius(dbcSpellRadius.LookupEntry(sp->EffectRadiusIndex[1])));
	if( sp->EffectRadiusIndex[2] != 0 )
		radius = std::max(::GetDBCFriendlyRadius(dbcSpellRadius.LookupEntry(sp->EffectRadiusIndex[2])), radius);
	radius = std::max(GetDBCFriendlyMaxRange(dbcSpellRange.LookupEntry(sp->rangeIndex)), radius);

	sp->base_range_or_radius_friendly = radius;
	sp->base_range_or_radius_sqr_friendly = radius*radius;

	// hash the name
	//!!!!!!! representing all strings on 32 bits is dangerous. There is a chance to get same hash for a lot of strings ;)
	sp->NameHash = crc32((const unsigned char*)sp->Name, (unsigned int)strlen(sp->Name)); //need these set before we start processing spells
}

extern uint32 implicitTargetFlags[150];

void SetupSpellTargets()
{
	memset(implicitTargetFlags, SPELL_TARGET_NONE, sizeof(uint32)*150);

	implicitTargetFlags[0] = (SPELL_TARGET_REQUIRE_ITEM | SPELL_TARGET_REQUIRE_GAMEOBJECT);
	implicitTargetFlags[1] = (SPELL_TARGET_OBJECT_SELF);
	implicitTargetFlags[3] = (SPELL_TARGET_REQUIRE_FRIENDLY);
	implicitTargetFlags[4] = (SPELL_TARGET_AREA_SELF | SPELL_TARGET_REQUIRE_FRIENDLY);
	implicitTargetFlags[5] = (SPELL_TARGET_OBJECT_CURPET);
	implicitTargetFlags[6] = (SPELL_TARGET_REQUIRE_ATTACKABLE);
	implicitTargetFlags[7] = (SPELL_TARGET_OBJECT_SCRIPTED);
	implicitTargetFlags[8] = (SPELL_TARGET_AREA | SPELL_TARGET_REQUIRE_ATTACKABLE);
	implicitTargetFlags[15] = (SPELL_TARGET_AREA_SELF | SPELL_TARGET_REQUIRE_ATTACKABLE);
	implicitTargetFlags[16] = (SPELL_TARGET_AREA | SPELL_TARGET_REQUIRE_ATTACKABLE);
	//implicitTargetFlags[17] = (SPELL_TARGET_AREA);
	implicitTargetFlags[18] = (SPELL_TARGET_AREA_SELF | SPELL_TARGET_NO_OBJECT);
	implicitTargetFlags[20] = (SPELL_TARGET_AREA_PARTY);
	implicitTargetFlags[21] = (SPELL_TARGET_REQUIRE_FRIENDLY);
	implicitTargetFlags[22] = (SPELL_TARGET_AREA_SELF);
	implicitTargetFlags[23] = (SPELL_TARGET_REQUIRE_GAMEOBJECT);
	implicitTargetFlags[24] = (SPELL_TARGET_AREA_CONE | SPELL_TARGET_REQUIRE_ATTACKABLE);
	implicitTargetFlags[25] = (SPELL_TARGET_ANY_OBJECT);
	implicitTargetFlags[26] = (SPELL_TARGET_REQUIRE_GAMEOBJECT | SPELL_TARGET_REQUIRE_ITEM);
	implicitTargetFlags[27] = (SPELL_TARGET_OBJECT_PETOWNER);
	implicitTargetFlags[28] = (SPELL_TARGET_AREA | SPELL_TARGET_REQUIRE_ATTACKABLE);
	implicitTargetFlags[29] = (SPELL_TARGET_OBJECT_SELF | SPELL_TARGET_AREA_PARTY | SPELL_TARGET_AREA_SELF);
	implicitTargetFlags[30] = (SPELL_TARGET_REQUIRE_FRIENDLY);
	implicitTargetFlags[31] = (SPELL_TARGET_REQUIRE_FRIENDLY | SPELL_TARGET_AREA);
	//implicitTargetFlags[32] = (SPELL_TARGET_OBJECT_SELF);
	implicitTargetFlags[33] = (SPELL_TARGET_AREA_SELF | SPELL_TARGET_AREA_PARTY);
	implicitTargetFlags[35] = (SPELL_TARGET_AREA_PARTY);
	implicitTargetFlags[36] = (SPELL_TARGET_OBJECT_SCRIPTED);
	implicitTargetFlags[37] = (SPELL_TARGET_AREA_SELF | SPELL_TARGET_AREA_PARTY | SPELL_TARGET_AREA_RAID);
	implicitTargetFlags[39] = (SPELL_TARGET_OBJECT_SELF);
	implicitTargetFlags[40] = (SPELL_TARGET_OBJECT_SCRIPTED);
	implicitTargetFlags[41] = (SPELL_TARGET_OBJECT_SELF);
	implicitTargetFlags[42] = (SPELL_TARGET_OBJECT_SELF);
	implicitTargetFlags[43] = (SPELL_TARGET_OBJECT_SELF);
	implicitTargetFlags[44] = (SPELL_TARGET_OBJECT_SELF);
	implicitTargetFlags[45] = (SPELL_TARGET_AREA_CHAIN | SPELL_TARGET_REQUIRE_FRIENDLY);
	implicitTargetFlags[46] = (SPELL_TARGET_OBJECT_SELF);
	implicitTargetFlags[47] = (SPELL_TARGET_AREA_SELF | SPELL_TARGET_NO_OBJECT); //dont fill target map for this (fucks up some spell visuals)
	implicitTargetFlags[48] = (SPELL_TARGET_OBJECT_SELF);
	implicitTargetFlags[49] = (SPELL_TARGET_OBJECT_SELF);
	implicitTargetFlags[50] = (SPELL_TARGET_OBJECT_SELF);
	implicitTargetFlags[52] = (SPELL_TARGET_AREA);
	implicitTargetFlags[53] = (SPELL_TARGET_AREA_CURTARGET | SPELL_TARGET_REQUIRE_ATTACKABLE);
	implicitTargetFlags[54] = (SPELL_TARGET_AREA_CONE | SPELL_TARGET_REQUIRE_ATTACKABLE);
	implicitTargetFlags[56] = (SPELL_TARGET_AREA_SELF | SPELL_TARGET_AREA_RAID); //used by commanding shout] = (targets raid now
	implicitTargetFlags[57] = (SPELL_TARGET_REQUIRE_FRIENDLY | SPELL_TARGET_AREA_PARTY);
	implicitTargetFlags[61] = (SPELL_TARGET_AREA_SELF | SPELL_TARGET_AREA_RAID | SPELL_TARGET_OBJECT_TARCLASS | SPELL_TARGET_REQUIRE_FRIENDLY);
	implicitTargetFlags[63] = (SPELL_TARGET_OBJECT_SELF);
	implicitTargetFlags[64] = (SPELL_TARGET_OBJECT_SELF);
	implicitTargetFlags[65] = (SPELL_TARGET_OBJECT_SELF);
	implicitTargetFlags[66] = (SPELL_TARGET_OBJECT_SELF);
	implicitTargetFlags[67] = (SPELL_TARGET_OBJECT_SELF);
	implicitTargetFlags[69] = (SPELL_TARGET_OBJECT_SELF);
	implicitTargetFlags[72] = (SPELL_TARGET_AREA_RANDOM);
	implicitTargetFlags[73] = (SPELL_TARGET_OBJECT_SELF);
	implicitTargetFlags[76] = (SPELL_TARGET_REQUIRE_ATTACKABLE);
	implicitTargetFlags[77] = (SPELL_TARGET_REQUIRE_ATTACKABLE);
	implicitTargetFlags[86] = (SPELL_TARGET_AREA_RANDOM);
	implicitTargetFlags[87] = (SPELL_TARGET_AREA);
	implicitTargetFlags[89] = (SPELL_TARGET_AREA);
	implicitTargetFlags[90] = (SPELL_TARGET_OBJECT_CURCRITTER);
	implicitTargetFlags[104] = (SPELL_TARGET_REQUIRE_ATTACKABLE | SPELL_TARGET_AREA_CONE);
	implicitTargetFlags[149] = SPELL_TARGET_NOT_IMPLEMENTED;
}

void GenerateSpellCoeffFile()
{
	SpellEntry *sp;
	FILE *file = fopen("SpellPowerCoeff.cpp", "w");
	fprintf(file,  "	{\n");

	QueryResult* resultx = WorldDatabase.Query("SELECT * FROM spell_coef_override");
	if(resultx != NULL)
	{
		do
		{
			Field* f = resultx->Fetch();
			uint32 spellid = f[0].GetUInt32();
			sp = dbcSpell.LookupEntry(spellid);
			if(!spellid || !sp)
				continue;

			float APcoef = f[1].GetFloat();
			float APOTcoef = f[2].GetFloat();
			float SPcoef = f[3].GetFloat();
			float SPOTcoef = f[4].GetFloat();
			if(!APcoef && !APOTcoef && !SPcoef && !SPOTcoef)
				continue;

			fprintf(file,  "	case %u: // %s", spellid, sp->Name);
			if(sp->RankNumber)
				fprintf(file,  " - %s\n", sp->Rank);
			else
				fprintf(file,  "\n");

			fprintf(file,  "		{\n");
			if(APcoef)
				fprintf(file,  "			sp->AP_coef_override = float(%04ff);\n", APcoef);
			if(APOTcoef)
				fprintf(file,  "			sp->APOT_coef_override = float(%04ff);\n", APOTcoef);
			if(SPcoef)
				fprintf(file,  "			sp->SP_coef_override = float(%04ff);\n", SPcoef);
			if(SPOTcoef)
				fprintf(file,  "			sp->SPOT_coef_override = float(%04ff);\n", SPOTcoef);
			fprintf(file,  "		}break;\n");

		}
		while(resultx->NextRow());
		delete resultx;
	}
	fprintf(file,  "	}\n");
	fclose(file);
}

/************************************************************************/
/* IsDamagingSpell, this function seems slow, its only used rarely	  */
/************************************************************************/
bool IsDamagingSpell(SpellEntry *sp)
{
	switch (sp->Effect[0])
	{
		case SPELL_EFFECT_SCHOOL_DAMAGE:
		case SPELL_EFFECT_ENVIRONMENTAL_DAMAGE:
		case SPELL_EFFECT_HEALTH_LEECH:
		case SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL:
		case SPELL_EFFECT_ADD_EXTRA_ATTACKS:
		case SPELL_EFFECT_WEAPON_PERCENT_DAMAGE:
		case SPELL_EFFECT_POWER_BURN:
		case SPELL_EFFECT_ATTACK:
			return true;
	}
	switch (sp->Effect[1])
	{
		case SPELL_EFFECT_SCHOOL_DAMAGE:
		case SPELL_EFFECT_ENVIRONMENTAL_DAMAGE:
		case SPELL_EFFECT_HEALTH_LEECH:
		case SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL:
		case SPELL_EFFECT_ADD_EXTRA_ATTACKS:
		case SPELL_EFFECT_WEAPON_PERCENT_DAMAGE:
		case SPELL_EFFECT_POWER_BURN:
		case SPELL_EFFECT_ATTACK:
			return true;
	}
	switch (sp->Effect[2])
	{
		case SPELL_EFFECT_SCHOOL_DAMAGE:
		case SPELL_EFFECT_ENVIRONMENTAL_DAMAGE:
		case SPELL_EFFECT_HEALTH_LEECH:
		case SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL:
		case SPELL_EFFECT_ADD_EXTRA_ATTACKS:
		case SPELL_EFFECT_WEAPON_PERCENT_DAMAGE:
		case SPELL_EFFECT_POWER_BURN:
		case SPELL_EFFECT_ATTACK:
			return true;
	}
	if( sp->Effect[0]==SPELL_EFFECT_APPLY_AURA ||
		sp->Effect[0]==SPELL_EFFECT_APPLY_AREA_AURA)
	{
		switch (sp->EffectApplyAuraName[0])
		{
			case 3://SPELL_AURA_PERIODIC_DAMAGE:
			case 43://SPELL_AURA_PROC_TRIGGER_DAMAGE:
			case 89://SPELL_AURA_PERIODIC_DAMAGE_PERCENT:
			case 162://SPELL_AURA_POWER_BURN:
				return true;
		}
	}
	if( sp->Effect[1]==SPELL_EFFECT_APPLY_AURA ||
		sp->Effect[1]==SPELL_EFFECT_APPLY_AREA_AURA)
	{
		switch (sp->EffectApplyAuraName[1])
		{
			case 3://SPELL_AURA_PERIODIC_DAMAGE:
			case 43://SPELL_AURA_PROC_TRIGGER_DAMAGE:
			case 89://SPELL_AURA_PERIODIC_DAMAGE_PERCENT:
			case 162://SPELL_AURA_POWER_BURN:
				return true;
		}
	}
	if( sp->Effect[2]==SPELL_EFFECT_APPLY_AURA ||
		sp->Effect[2]==SPELL_EFFECT_APPLY_AREA_AURA)
	{
		switch (sp->EffectApplyAuraName[2])
		{
			case 3://SPELL_AURA_PERIODIC_DAMAGE:
			case 43://SPELL_AURA_PROC_TRIGGER_DAMAGE:
			case 89://SPELL_AURA_PERIODIC_DAMAGE_PERCENT:
			case 162://SPELL_AURA_POWER_BURN:
				return true;
		}
	}
	return false;
}
