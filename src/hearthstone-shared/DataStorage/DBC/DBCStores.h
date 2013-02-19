/***
 * Demonstrike Core
 */

#pragma once

#include "DataStore.h"
#include "../../Timer.h"

#pragma pack(push,1)

struct AreaTable
{
	uint32 AreaId;
	uint32 mapId;
	uint32 ZoneId;
	uint32 explorationFlag;
	uint32 AreaFlags;
//	uint32 unk[5];
	uint32 level;
	char* name;
//	uint32 nameAlt[15];
//	uint32 nameFlags;
	uint32 category;
	uint32 m_liquidTypeID[4];
	uint32 m_minElevation;
	uint32 m_ambient_multiplier;
};

struct AreaTriggerEntry
{
	uint32 id;		// 0 m_ID
	uint32 mapid;	// 1 m_ContinentID
	float x;		// 2 m_x
	float y;		// 3 m_y
	float z;		// 4 m_z
	float radius;	// 5 m_radius
	float box_x;	// 6 m_box_length
	float box_y;	// 7 m_box_width
	float box_z;	// 8 m_box_heigh
	float box_o;	// 9 m_box_yaw
};

struct AuctionHouseDBC
{
	uint32 id;
	uint32 faction;
	uint32 fee;
	uint32 tax;
//	char* name[16];
//	char* nameFlags;
};

struct BankSlotPrice
{
	uint32 Id;
	uint32 Price;
};

struct CharTitlesEntry
{
	uint32 Id;
	char* name;
	uint32 bit_index;
};

struct ChatChannelDBC
{
	uint32 id;
	uint32 flags;
	const char* pattern;
};

struct CharRaceEntry
{
	uint32 race_id;
	uint32 team_id;
	uint32 cinematic_id;
	char* name1;
};

struct CharClassEntry
{
	uint32 class_id;
//	uint32 unk1;
	uint32 power_type;
//	uint32 unk2;
	char* name;
//	uint32 namealt[15];
//	uint32 nameflags;
//	char* string1[16];									// 22-37
//	char* stringflag;									// 38
//	char* string2[16];									// 39-54
//	char* string2flag;									// 55
//	uin32 unused;										// 56
	uint32 spellfamily;									// 57
//	uin32 unused2;										// 58
};

struct CreatureDisplayInfo
{
	uint32 ID;		//id
	//uint32 unk1;	//ModelData column2?
	//uint32 unk2;	//ExtraDisplayInfo column 18?
	uint32 ExtendedDisplayInfoID;
	float Scale;
	//uint32 unk5;
	//uint32 unk6;
	//uint32 unk7;
	//uint32 unk8;
	//uint32 unk9;
	//uint32 unk10;
	//uint32 unk11;
	//uint32 unk12;
	//uint32 unk13;
};

struct CreatureBoundData
{
	uint32 Entry; // Display ID
	float Low[3];
	float High[3];
	float BoundRadius;
};

struct CreatureFamilyEntry
{
	uint32 ID;											// 1
	float minsize;										// 2
	uint32 minlevel;									// 3
	float maxsize;										// 4
	uint32 maxlevel;									// 5
	uint32 skilline;									// 6
	uint32 tameable;									// 7 second skill line - 270 Generic
	uint32 petdietflags;								// 8
	char* name;											// 11
	//char*   NameAlt[15];								// 12-26
														// 27 string flags, unused
														// 28 m_iconFile unused
};

struct CreatureSpellDataEntry
{
	uint32 id;
	uint32 Spells[3];
	uint32 PHSpell;
	uint32 Cooldowns[3];
	uint32 PH;
};

struct gtFloat
{
	float val;
};

struct CombatRatingDBC
{
	float val;
};

struct DurabilityCostsEntry
{
	uint32 itemlevel;
	uint32 modifier[29];
};

struct DurabilityQualityEntry
{
	uint32 id;
	float quality_modifier;
};

struct ItemSetEntry
{
	uint32 id;						//1
//	char* name;						//2
//	uint32 unused_shit[15];			//3 - 17
//	uint32 flag;					//18 constant
	uint32 itemid[10];				//19 - 28
//	uint32 more_unused_shit[7];		//29 - 35
	uint32 SpellID[8];				//36 - 43
	uint32 itemscount[8];			//44 - 51
	uint32 RequiredSkillID;			//52
	uint32 RequiredSkillAmt;		//53
};

struct Lock
{
	uint32 Id;
	uint32 locktype[8]; //0 - no lock, 1 - item needed for lock, 2 - min lockping skill needed
	uint32 lockmisc[8]; //if type is 1 here is a item to unlock, else is unknow for now
	uint32 minlockskill[8]; //min skill in lockpiking to unlock.
	//uint32 unk[8]; //unknown
};

struct BattleMasterListEntry
{
	uint32 entry;		// 0
	int32 mapids[8];	// 1-8 Map ids
	uint32 maptype;		// 9 Map flags
	uint32 minLevel;	// 10
	uint32 maxLevel;	// 11
	uint32 maxMembers;	// 12 Maximum members allowed to queue.
//	uint32 unk2[2];		// 13-14
	char *name;			// 15 name
//	char* shit[15];		// 16-30 shitty shit.
//	char* shit2;		// 31 string flags
//	uint32 unkshit;		// 32
};

struct emoteentry
{
	uint32 Id;
//	uint32 name;
	uint32 textid;
/*	uint32 textid2;
	uint32 textid3;
	uint32 textid4;
	uint32 unk1;
	uint32 textid5;
	uint32 unk2;
	uint32 textid6;
	uint32 unk3;
	uint32 unk4;
	uint32 unk5;
	uint32 unk6;
	uint32 unk7;
	uint32 unk8;
	uint32 unk9;
	uint32 unk10;
	uint32 unk11;*/
};

struct skilllinespell //SkillLineAbility.dbc
{
	uint32 Id;					//1
	uint32 skilline;			//2
	uint32 spell;				//3
	uint32 racemask;			// 4 m_raceMask
	uint32 classmask;			// 5 m_classMask
	uint32 racemaskNot;			// 6 m_excludeRace
	uint32 classmaskNot;		// 7 m_excludeClass
	uint32 req_skill_value;		// 8 m_minSkillLineRank
	uint32 next;				//9
    uint32 learnOnGetSkill;
	uint32 max_value;
	uint32 min_value;
    //uint32 unk10;
    //uint32 unk11;
    uint32 reqTP;
};

struct EnchantEntry
{
	uint32 Id;			//1
	uint32 type[3];		//3-5
	int32 min[3];		//6-8 //for compat, in practice min==max
	int32 max[3];		//9-11
	uint32 spell[3];	//12-14
	char* Name;			//15
	//char* NameAlt[15]	//16-31
	//uint32 NameFlags;	//32
	uint32 visual;		//33
	uint32 EnchantGroups; //34
	uint32 GemEntry;	//35
	uint32 EnchantmentCondition;		//36 Gem Related
};

struct GemPropertyEntry{
	uint32 Entry;
	uint32 EnchantmentID;
	uint32 unk1;//bool
	uint32 unk2;//bool
	uint32 SocketMask;
};

struct skilllineentry //SkillLine.dbc
{
	uint32 id;
	uint32 type;
	uint32 skillCostID;
	char* Name;
	//uint32 Description;
	//char* idk;
	//uint32 unk2;
};

struct SummonPropertiesEntry
{
	uint32 Id;
	uint32 controltype;
	uint32 factionId;
	uint32 type;
	uint32 slot;
	uint32 unk2;
};

struct ClassFamilyMask
{
	uint64 Flags;

	ClassFamilyMask() : Flags(0) {}
	explicit ClassFamilyMask(uint64 familyFlags) : Flags(familyFlags) {}

	bool Empty() const { return Flags == 0; }
	bool operator!() const { return Empty(); }
	operator void const*() const { return Empty() ? NULL : this; } // for allow normal use in if(mask)

	bool IsFitToFamilyMask(uint64 familyFlags) const
	{
		return ((Flags & familyFlags) == 0 ? true : false);
	}

	bool IsFitToFamilyMask(ClassFamilyMask const& mask) const
	{
		return ((Flags & mask.Flags) == 0 ? true : false);
	}

	uint64 operator& (uint64 mask) const                    // possible will removed at finish convertion code use IsFitToFamilyMask
	{
		return ((Flags & mask) == 0 ? true : false);
	}

	ClassFamilyMask& operator|= (ClassFamilyMask const& mask)
	{
		Flags |= mask.Flags;
		return *this;
	}
};

// Struct for the entry in Spell.dbc
struct SpellEntry
{
	uint32 Id;							//0
	uint32 Category;					//1
    uint32 CastUI;						//2 not used
	uint32 DispelType;					//3
	uint32 MechanicsType;				//4
	uint32 Attributes;					//5
	uint32 AttributesEx;				//6
	uint32 Flags3;						//7
	uint32 Flags4;						//8
	uint32 Flags5;						//9
	uint32 Flags6;						//10
	uint32 Flags7;						//11
	uint32 RequiredShapeShift;			//12 // Flags BitMask for shapeshift spells
	uint32 NotAllowedShapeShift;		//13 // Flags BitMask for which shapeshift forms this spell can NOT be used in.
	uint32 Targets;						//14 - N / M
	uint32 TargetCreatureType;			//15
	uint32 RequiresSpellFocus;			//16
	uint32 FacingCasterFlags;			//17
	uint32 CasterAuraState;				//18
	uint32 TargetAuraState;				//19
	uint32 CasterAuraStateNot;			//20
	uint32 TargetAuraStateNot;			//21
	uint32 CastingTimeIndex;			//22
	uint32 RecoveryTime;				//23
	uint32 CategoryRecoveryTime;		//24
	uint32 InterruptFlags;				//25
	uint32 AuraInterruptFlags;			//26
	uint32 ChannelInterruptFlags;		//27
	uint32 procFlags;					//28
	uint32 procChance;					//29
	uint32 procCharges;					//30
	uint32 maxLevel;					//31
	uint32 baseLevel;					//32
	uint32 spellLevel;					//33
	uint32 DurationIndex;				//34
	uint32 powerType;					//35
	uint32 manaCost;					//36
	uint32 manaCostPerlevel;			//37
	uint32 manaPerSecond;				//38
	uint32 manaPerSecondPerLevel;		//39
	uint32 rangeIndex;					//40
	float  speed;						//41
	uint32 modalNextSpell;				//42
	uint32 maxstack;					//43
	uint32 Totem[2];					//44 - 45
	uint32 Reagent[8];					//46 - 53
	uint32 ReagentCount[8];				//54 - 61
	int32 EquippedItemClass;			//62
	uint32 EquippedItemSubClass;		//63
	uint32 RequiredItemFlags;			//64
	uint32 Effect[3];					//65 - 67
	uint32 EffectDieSides[3];			//68 - 70
	uint32 EffectBaseDice[3];			//71 - 73
    float  EffectDicePerLevel[3];		//74 - 76
	float  EffectRealPointsPerLevel[3];	//77 - 79
	int32  EffectBasePoints[3];			//80 - 82
	int32  EffectMechanic[3];			//83 - 85 Related to SpellMechanic.dbc
	uint32 EffectImplicitTargetA[3];	//86 - 88
	uint32 EffectImplicitTargetB[3];	//89 - 91
	uint32 EffectRadiusIndex[3];		//92 - 94
	uint32 EffectApplyAuraName[3];		//95 - 97
	uint32 EffectAmplitude[3];			//98 - 100
	float  EffectMultipleValue[3];		//101 - 103 This value is the $ value from description
	uint32 EffectChainTarget[3];		//104 - 106
	uint32 EffectSpellGroupRelation[3];	//107 - 109
	uint32 EffectMiscValue[3];			//110 - 112
	uint32 EffectMiscValueB[3];			//113 - 115
	uint32 EffectTriggerSpell[3];		//116 - 118
	float  EffectPointsPerComboPoint[3];//119 - 121
	uint32 SpellVisual[2];				//122 - 123
	uint32 SpellIconID;					//124
	uint32 activeIconID;				//125
	uint32 spellPriority;				//126
	char* Name;							//127
	//char* NameAlt[15];				//128-142 not used
	//uint32 NameFlags;					//143 not used
	char * Rank;						//144
	//char * RankAlt[15];				//145-159 not used
	//uint32 RankFlags;					//160 not used
	char * Description;					//161
	//char * DescriptionAlt[15];		//162-176 not used
	//uint32 DescriptionFlags;			//177 not used
	char * BuffDescription;				//178
	//char * BuffDescription[15];		//179-193 not used
	//uint32 buffdescflags;				//194 not used
	uint32 ManaCostPercentage;			//195
	uint32 StartRecoveryCategory;		//196
	uint32 StartRecoveryTime;			//197
	uint32 MaxTargetLevel;				//198
	uint32 SpellFamilyName;				//199
    ClassFamilyMask SpellFamilyFlags;	//200 - 201 m_spellClassMask
	uint32 MaxTargets;					//202
	uint32 Spell_Dmg_Type;				//203   dmg_class Integer	  0=None, 1=Magic, 2=Melee, 3=Ranged
	uint32 PreventionType;				//204   0,1,2 related to Spell_Dmg_Type I think
	int32 StanceBarOrder;				//205   related to paladin aura's 
	float dmg_multiplier[3];			//206 - 208   if the name is correct I dono
	uint32 MinFactionID;				//209   only one spellid:6994 has this value = 369 UNUSED
	uint32 MinReputation;				//210   only one spellid:6994 has this value = 4 UNUSED
	uint32 RequiredAuraVision;			//211  3 spells 1 or 2   
	uint32 TotemCategory[2];			//212-213
	int32 AreaId;						//214 
	uint32 School;						//215

	/// CUSTOM: these fields are used for the modifications made in the world.cpp
	uint32 DiminishStatus;				//!!! DMS
	uint32 proc_interval;				//!!! CUSTOM, Time(In MS) between proc's.
	float ProcsPerMinute;				//!!! CUSTOM, number of procs per minute
	uint32 buffIndexType;				//!!! CUSTOM, Tells us what type of buff it is, so we can limit the amount of them.
	uint32 c_is_flags;					//!!! CUSTOM, store spell checks in a static way : isdamageind,ishealing
	uint32 buffType;					//!!! CUSTOM, these are related to creating a item through a spell
	uint32 RankNumber;					//!!! CUSTOM, this protects players from having >1 rank of a spell
	uint32 NameHash;					//!!! CUSTOM, related to custom spells, summon spell quest related spells
	float base_range_or_radius;			//!!! CUSTOM, needed for aoe spells most of the time
	float base_range_or_radius_sqr;		//!!! CUSTOM, needed for aoe spells most of the time
	float base_range_or_radius_friendly;//!!! CUSTOM, needed for aoe spells most of the time
	float base_range_or_radius_sqr_friendly;//!!! CUSTOM, needed for aoe spells most of the time
	bool is_melee_spell;				//!!! CUSTOM, Obvious.
	bool is_ranged_spell;				//!!! CUSTOM, Obvious.
	bool spell_can_crit;				//!!! CUSTOM, Obvious.
	uint32 trnr_req_clsmsk;				//!!! CUSTOM, Required class mask to learn at a trainer.

	/* Crow:
	Custom: The amount of threat the spell will generate.
	This is loaded from a DB table, and if it isn't there, threat is always damage. */
	uint32 ThreatForSpell;
	float cone_width; // love me or hate me, all "In a cone in front of the caster" spells don't necessarily mean "in front"
	//Spell Coefficient
	bool isAOE;							//!!! CUSTOM, Obvious.
	float AP_coef_override;				//!!! CUSTOM, Additional coef from ap
	float APOT_coef_override;			//!!! CUSTOM, Additional dot/hot coef from ap
	float SP_coef_override;				//!!! CUSTOM, Additional coef from sp
	float SPOT_coef_override;			//!!! CUSTOM, Additional dot/hot coef from sp
	bool self_cast_only;				//!!! CUSTOM, Obvious.
	bool apply_on_shapeshift_change;	//!!! CUSTOM, Obvious.
	bool always_apply;					//!!! CUSTOM, Obvious.
	uint32 auraimmune_flag;				//!!! CUSTOM, this var keeps aura effects in binary format.
	bool Unique;						//!!! CUSTOM, Is this a unique effect? ex: Mortal Strike -50% healing.

	uint32 area_aura_update_interval;
	uint32 skilline;
	/* Crow:
	SpellId used to send log to client for this spell
	This is overwritten sometimes with proc's */
	uint32 logsId;
	uint32 AdditionalAura;
	uint32 forced_creature_target;
	uint32 AreaAuraTarget;

	//poisons type...
	uint32 poison_type;					//!!! CUSTOM, Type of poison it is.

	//backattack
	bool AllowBackAttack;				//!!! CUSTOM, Obvious.

	// Crow: The following are customs made by me, mostly duplicate fields for handling more information.
	uint32 procflags2; // We get two now, hurray. One does not take the place of the other.

	// Queries/Commands:
	bool IsFitToFamilyMask(uint64 familyFlags) const
	{
		return SpellFamilyFlags.IsFitToFamilyMask(familyFlags);
	}

	bool IsFitToFamily(uint32 family, uint64 familyFlags) const
	{
		return (SpellFamilyName == family && IsFitToFamilyMask(familyFlags));
	}

	bool IsFitToFamilyMask(ClassFamilyMask const& mask) const
	{
		return SpellFamilyFlags.IsFitToFamilyMask(mask);
	}

	bool IsFitToFamily(uint32 family, ClassFamilyMask const& mask) const
	{
		return (SpellFamilyName == family && IsFitToFamilyMask(mask));
	}

	bool IsChannelSpell() { return ((AttributesEx & (0x04|0x40)) ? true : (ChannelInterruptFlags != 0 ? true : false)); }
};

struct ItemExtendedCostEntry
{
	uint32 costid;
	uint32 honor;
	uint32 arena;
	uint32 item[5];
	uint32 count[5];
	uint32 personalrating;
};

struct TalentEntry
{
	uint32 TalentID;
	uint32 TalentTree;
	uint32 Row;
	uint32 Col;
	uint32 RankID[5];
	//uint32 unk0[4];
	uint32 DependsOn;
	//uint32 unk1[2];
	uint32 DependsOnRank;
	//uint32 unk2[4];

	uint32 BitCounter[5];
};

struct TalentTabEntry
{
	uint32	TalentTabID;
	uint32  ClassMask;
	uint32  TabPage;
};

struct Trainerspell
{
	uint32 Id;
	uint32 skilline1;
	uint32 skilline2;
	uint32 skilline3;
	uint32 maxlvl;
	uint32 charclass;
};

struct SpellCastTime
{
	uint32 ID;
	uint32 CastTime;
//	uint32 unk1;
//	uint32 unk2;
};

struct SpellRadius
{
	uint32 ID;
	float radiusHostile;
//	float unk;
	float radiusFriend;
};

struct SpellRange
{
	uint32 ID;
	float minRange;
	float maxRange;
};

struct SpellDuration
{
	uint32 ID;
	int32 Duration1;
	int32 Duration2;
	int32 Duration3;
};

struct RandomProps
{
	uint32 ID;
	char *rpname;
	uint32 spells[3];
//	uint32 unk[2];
//	uint32 name[16];
//	uint32 RankFlags;

};

struct WorldMapOverlayEntry
{
	uint32 AreaReference;
	uint32 AreaTableID;
};

struct WMOAreaTableEntry
{
	uint32 Id;				// 0 index
	int32 rootId;			// 1 used in root WMO
	int32 adtId;			// 2 used in adt file
	int32 groupId;			// 3 used in group WMO
	uint32 Flags;			// 9 used for indoor/outdoor determination
	uint32 areaId;			// 10 link to AreaTableEntry.ID
};

struct FactionDBC
{
	uint32 ID;
	int32  RepListId;
	uint32 baseRepMask[4];
	uint32 baseRepClassMask[4];
	int32  baseRepValue[4];
	uint32 reputationFlags[4];
	uint32 parentFaction;
	char*  Name;
//	uint32 names[15];
//	uint32 nameFlags;
//	uint32 Description[16];
//	uint32 descriptionFlags;
};

struct FactionTemplateDBC
{
	uint32 ID;
	uint32 Faction;
	uint32 FactionFlags;
	uint32 FactionMask;
	uint32 FriendlyMask;
	uint32 HostileMask;
	uint32 EnemyFactions[4];
	uint32 FriendlyFactions[4];
};

struct DBCTaxiNode
{
	uint32 id;
	uint32 mapid;
	float x;
	float y;
	float z;
//	char* name[15];
//	uint32 nameflags;
	uint32 horde_mount;
	uint32 alliance_mount;
};

struct DBCTaxiPath
{
	uint32 id;
	uint32 from;
	uint32 to;
	uint32 price;
};

struct DBCTaxiPathNode
{
	uint32 id;
	uint32 path;
	uint32 seq;
	uint32 mapid;
	float x;
	float y;
	float z;
	uint32 flag;
	uint32 waittime;
//	uint32 unk[2];
};

struct MapEntry
{
	uint32 id;
//	char* name_internal;
	uint32 map_type;
//	uint32 flags;
//	char* real_name[16];
//	uint32 linked_zone;		// common zone for instance and continent map
//	char* hordeIntro[16];		// text for PvP Zones
//	uint32 hordeflags;
//	char* allianceIntro[16];	// text for PvP Zones
//	uint32 allianceflags;
	uint32 multimap_id;		// seems to be 0 for all test maps.
//	uint32 unk;				// 1/1.25
//	int32 parent_map;		// map_id of parent map
//	float start_x;			// enter x coordinate (if exist single entry)
//	float start_y;			// enter y coordinate (if exist single entry)
//	uint32 unk;				// -1
	uint32 addon;			// 0-original maps, 1-tbc addon, 2-wotlk addon

	bool israid() { return map_type == 2; }
};

struct ItemRandomSuffixEntry
{
	uint32 id;
	char *name;
	uint32 enchantments[3];
	uint32 prefixes[3];
};

struct SpellShapeshiftForm
{
	uint32 id;												// 0 id
	//uint32 buttonPosition;                                // 1 unused
	//char*  Name[16];                                      // 2-17 unused
	//uint32 NameFlags;                                     // 18 unused
	uint32 flags1;                                          // 19
	int32  creatureType;                                    // 20 <= 0 humanoid, other normal creature types
	//uint32 unk1;                                          // 21 unused
	uint32 attackSpeed;                                     // 22
	//uint32 modelID;                                       // 23 unused
	//uint32 unk2;                                          // 24 unused
	//uint32 unk3;                                          // 25 unused
	//uint32 unk4;                                          // 26 unused
	uint32 spells[8];
};

struct QuestXP
{
	uint32 questLevel;	// 0
	uint32 xpIndex[8];	// 1-9
	//unk				// 10
};

#pragma pack(pop)

HEARTHSTONE_INLINE float GetDBCScale(CreatureDisplayInfo *Scale)
{
	if(Scale && Scale->Scale)
		return Scale->Scale;

	return 1.0f; // It's 1 a large percent of the time anyway...
}

HEARTHSTONE_INLINE float GetDBCRadius(SpellRadius *radius)
{
	if(radius)
	{
		if(radius->radiusHostile)
			return radius->radiusHostile;
		else
			return radius->radiusFriend;
	}

	return 0.0f;
}

HEARTHSTONE_INLINE uint32 GetDBCCastTime(SpellCastTime *time)
{
	if(time && time->CastTime)
		return time->CastTime;
	return 0;
}

HEARTHSTONE_INLINE float GetDBCMaxRange(SpellRange *range)
{
	if(range)
		return range->maxRange;
	return 0.0f;
}

HEARTHSTONE_INLINE float GetDBCMinRange(SpellRange *range)
{
	if(range)
		return range->minRange;
	return 0.0f;
}

HEARTHSTONE_INLINE int32 GetDBCDuration(SpellDuration *dur)
{
	if(dur && dur->Duration1)
		return dur->Duration1;

	return -1;
}

HEARTHSTONE_INLINE float GetDBCFriendlyRadius(SpellRadius *radius)
{
	if(radius == NULL)
		return 0.0f;

	if(radius->radiusFriend)
		return radius->radiusFriend;

	return GetDBCRadius(radius);
}

HEARTHSTONE_INLINE float GetDBCFriendlyMaxRange(SpellRange *range)
{
	if(range == NULL)
		return 0.0f;
	return GetDBCMaxRange(range);
}

HEARTHSTONE_INLINE float GetDBCFriendlyMinRange(SpellRange *range)
{
	if(range == NULL)
		return 0.0f;
	return GetDBCMinRange(range);
}

#define SAFE_DBC_CODE_RETURNS			/* undefine this to make out of range/nulls return null. */

template<class T>
class SERVER_DECL DBCStorage
{
	T * m_heapBlock;
	T * m_firstEntry;

	T ** m_entries;
	uint32 m_max;
	uint32 m_numrows;
	uint32 m_stringlength;
	char * m_stringData;

public:

	class iterator
	{
	private:
		T* p;
	public:
		iterator(T* ip = 0) : p(ip) { };
		iterator& operator++() { ++p; return *this; };
		bool operator != (const iterator &i) { return (p != i.p); };
		bool operator == (const iterator &i) { return (p == i.p); };
		T* operator*() { return p; };
	};

	iterator begin() { return iterator(&m_heapBlock[0]); }
	iterator end() { return iterator(&m_heapBlock[m_numrows]); }

	DBCStorage()
	{
		m_heapBlock = NULL;
		m_entries = NULL;
		m_firstEntry = NULL;
		m_max = 0;
		m_numrows = 0;
		m_stringlength=0;
		m_stringData = NULL;
	}

	~DBCStorage()
	{
		Cleanup();
	}

	void Cleanup()
	{
		if(m_heapBlock)
		{
			delete[] m_heapBlock;
			//free(m_heapBlock);
			m_heapBlock = NULL;
		}
		if(m_entries)
		{
			delete[] m_entries;
			//free(m_entries);
			m_entries = NULL;
		}
		if( m_stringData != NULL )
		{
			delete[] m_stringData;
			//free(m_stringData);
			m_stringData = NULL;
		}
	}

	bool Load(const char * filename, const char * format, bool load_indexed, bool load_strings)
	{
		uint32 rows;
		uint32 cols;
		uint32 useless_shit;
		uint32 string_length;
		uint32 header;
		uint32 i;
		long pos;

		FILE * f = fopen(filename, "rb");
		if(f == NULL)
			return false;

		/* read the number of rows, and allocate our block on the heap */
		fread(&header,4,1,f);
		fread(&rows, 4, 1, f);
		fread(&cols, 4, 1, f);
		fread(&useless_shit, 4, 1, f);
		fread(&string_length, 4, 1, f);
		pos = ftell(f);

		EndianConvert(&header);
		EndianConvert(&rows);
		EndianConvert(&cols);
		EndianConvert(&useless_shit);
		EndianConvert(&string_length);

		if( load_strings )
		{
			fseek( f, 20 + ( rows * cols * 4 ), SEEK_SET );
			m_stringData = new char[string_length];
			//m_stringData = (char*)malloc(string_length);
			m_stringlength = string_length;
			fread( m_stringData, string_length, 1, f );
		}

		fseek(f, pos, SEEK_SET);

		m_heapBlock = new T[rows];
		//m_heapBlock = (T*)malloc(rows * sizeof(T));
		ASSERT(m_heapBlock);

		/* read the data for each row */
		for(i = 0; i < rows; ++i)
		{
			memset(&m_heapBlock[i], 0, sizeof(T));
			ReadEntry(f, &m_heapBlock[i], format, cols, filename);

			if(load_indexed)
			{
				/* all the time the first field in the dbc is our unique entry */
				if(*(uint32*)&m_heapBlock[i] > m_max)
					m_max = *(uint32*)&m_heapBlock[i];
			}
		}

		if(load_indexed)
		{
			m_entries = new T*[(m_max+1)];
			//m_entries = (T**)malloc(sizeof(T*) * (m_max+1));
			ASSERT(m_entries);

			memset(m_entries, 0, (sizeof(T*) * (m_max+1)));
			for(i = 0; i < rows; ++i)
			{
				if(m_firstEntry == NULL)
					m_firstEntry = &m_heapBlock[i];

				m_entries[*(uint32*)&m_heapBlock[i]] = &m_heapBlock[i];
			}
		}

		m_numrows = rows;

		fclose(f);
		return true;
	}

	void ReadEntry(FILE * f, T * dest, const char * format, uint32 cols, const char * filename)
	{
		const char * t = format;
		uint32 * dest_ptr = (uint32*)dest;
		uint32 c = 0;
		uint32 val;
		size_t len = strlen(format);
		if(len != cols)
		{
			printf("!!! possible invalid format in file %s (us: %u, them: %u)\n", filename, (uint32)len, cols);
			printf("!!! Core will pause for 10 seconds\n");
#if PLATFORM == PLATFORM_WIN
			Sleep(10000);
#else
			usleep(10000*1000);
#endif
			return;
		}

		while(*t != 0)
		{
			if((++c) > cols)
			{
				++t;
				printf("!!! Read buffer overflow in DBC reading of file %s\n", filename);
				printf("!!! Core will pause for 10 seconds\n");
#if PLATFORM == PLATFORM_WIN
				Sleep(10000);
#else
				usleep(10000*1000);
#endif
				break;
			}

			fread(&val, 4, 1, f);
			if(*t == 'x')
			{
				++t;
				continue;		// skip!
			}
			EndianConvert(&val);
			if(*t == 's')
			{
				char ** new_ptr = (char**)dest_ptr;
				static const char * null_str = "";
				char * ptr;
				if( val < m_stringlength )
					ptr = m_stringData + val;
				else
					ptr = (char*)null_str;

				*new_ptr = ptr;
				new_ptr++;
				dest_ptr = (uint32*)new_ptr;
			}
			else
			{
				*dest_ptr = val;
				dest_ptr++;
			}

			++t;
		}
	}

	HEARTHSTONE_INLINE uint32 GetNumRows()
	{
		return m_numrows;
	}

	HEARTHSTONE_INLINE uint32 GetMaxRow()
	{
		return m_max;
	}

	T * LookupEntryForced(uint32 i)
	{
		if(m_entries)
		{
			if(i > m_max)
				return NULL;
			else
				return m_entries[i];
		}
		else
		{
			if(i >= m_numrows)
				return NULL;
			else
				return &m_heapBlock[i];
		}
	}

	T * CreateCopy(T * obj)
	{
		T * oCopy = (T*)malloc(sizeof(T));
		ASSERT(oCopy);
		memcpy(oCopy,obj,sizeof(T));
		return oCopy;
	}

	void SetRow(uint32 i, T * t)
	{
		if(i < m_max && m_entries)
			m_entries[i] = t;
	}

	T * LookupEntry(uint32 i)
	{
		return LookupEntryForced(i);
/*		if(m_entries)
		{
			if(i > m_max)
//				return NULL;
				return m_firstEntry;
			else
				return m_entries[i];
		}
		else
		{
			if(i >= m_numrows)
//				return NULL;
				return &m_heapBlock[0];
			else
				return &m_heapBlock[i];
		}*/
	}

	T * LookupRow(uint32 i)
	{
		if(i >= m_numrows)
			return &m_heapBlock[0];
		else
			return &m_heapBlock[i];
	}
};

extern SERVER_DECL DBCStorage<BattleMasterListEntry> dbcBattleMasterList;
extern SERVER_DECL DBCStorage<GemPropertyEntry> dbcGemProperty;
extern SERVER_DECL DBCStorage<ItemSetEntry> dbcItemSet;
extern SERVER_DECL DBCStorage<Lock> dbcLock;
extern SERVER_DECL DBCStorage<SpellEntry> dbcSpell;
extern SERVER_DECL DBCStorage<SpellDuration> dbcSpellDuration;
extern SERVER_DECL DBCStorage<SpellRange> dbcSpellRange;
extern SERVER_DECL DBCStorage<SpellShapeshiftForm> dbcSpellShapeshiftForm;
extern SERVER_DECL DBCStorage<emoteentry> dbcEmoteEntry;
extern SERVER_DECL DBCStorage<SpellRadius> dbcSpellRadius;
extern SERVER_DECL DBCStorage<SpellCastTime> dbcSpellCastTime;
extern SERVER_DECL DBCStorage<AreaTable> dbcArea;
extern SERVER_DECL DBCStorage<AreaTriggerEntry> dbcAreaTrigger;
extern SERVER_DECL DBCStorage<FactionTemplateDBC> dbcFactionTemplate;
extern SERVER_DECL DBCStorage<FactionDBC> dbcFaction;
extern SERVER_DECL DBCStorage<EnchantEntry> dbcEnchant;
extern SERVER_DECL DBCStorage<RandomProps> dbcRandomProps;
extern SERVER_DECL DBCStorage<skilllinespell> dbcSkillLineSpell;
extern SERVER_DECL DBCStorage<skilllineentry> dbcSkillLine;
extern SERVER_DECL DBCStorage<DBCTaxiNode> dbcTaxiNode;
extern SERVER_DECL DBCStorage<DBCTaxiPath> dbcTaxiPath;
extern SERVER_DECL DBCStorage<DBCTaxiPathNode> dbcTaxiPathNode;
extern SERVER_DECL DBCStorage<AuctionHouseDBC> dbcAuctionHouse;
extern SERVER_DECL DBCStorage<TalentEntry> dbcTalent;
extern SERVER_DECL DBCStorage<TalentTabEntry> dbcTalentTab;
extern SERVER_DECL DBCStorage<CreatureBoundData> dbcCreatureBoundData;
extern SERVER_DECL DBCStorage<CreatureDisplayInfo> dbcCreatureDisplayInfo;
extern SERVER_DECL DBCStorage<CreatureSpellDataEntry> dbcCreatureSpellData;
extern SERVER_DECL DBCStorage<CreatureFamilyEntry> dbcCreatureFamily;
extern SERVER_DECL DBCStorage<CharClassEntry> dbcCharClass;
extern SERVER_DECL DBCStorage<CharRaceEntry> dbcCharRace;
extern SERVER_DECL DBCStorage<MapEntry> dbcMap;
extern SERVER_DECL DBCStorage<ItemExtendedCostEntry> dbcItemExtendedCost;
extern SERVER_DECL DBCStorage<ItemRandomSuffixEntry> dbcItemRandomSuffix;
extern SERVER_DECL DBCStorage<CombatRatingDBC> dbcCombatRating;
extern SERVER_DECL DBCStorage<ChatChannelDBC> dbcChatChannels;
extern SERVER_DECL DBCStorage<DurabilityCostsEntry> dbcDurabilityCosts;
extern SERVER_DECL DBCStorage<DurabilityQualityEntry> dbcDurabilityQuality;
extern SERVER_DECL DBCStorage<BankSlotPrice> dbcBankSlotPrices;
extern SERVER_DECL DBCStorage<BankSlotPrice> dbcStableSlotPrices; //uses same structure as Bank
extern SERVER_DECL DBCStorage<gtFloat> dbcMeleeCrit;
extern SERVER_DECL DBCStorage<gtFloat> dbcMeleeCritBase;
extern SERVER_DECL DBCStorage<gtFloat> dbcSpellCrit;
extern SERVER_DECL DBCStorage<gtFloat> dbcSpellCritBase;
extern SERVER_DECL DBCStorage<gtFloat> dbcManaRegen;
extern SERVER_DECL DBCStorage<gtFloat> dbcManaRegenBase;
extern SERVER_DECL DBCStorage<gtFloat> dbcHPRegen;
extern SERVER_DECL DBCStorage<gtFloat> dbcHPRegenBase;
extern SERVER_DECL DBCStorage<WorldMapOverlayEntry> dbcWorldMapOverlay;
extern SERVER_DECL DBCStorage<WMOAreaTableEntry> dbcWMOAreaTable;
extern SERVER_DECL DBCStorage<SummonPropertiesEntry> dbcSummonProps;
extern SERVER_DECL DBCStorage<QuestXP> dbcQuestXP;

bool LoadRSDBCs(const char* datapath);
bool LoadDBCs(const char* datapath);

HEARTHSTONE_INLINE WMOAreaTableEntry* GetWorldMapOverlayEntry( int32 adtid, int32 rootid, int32 groupid)
{
	DBCStorage<WMOAreaTableEntry>::iterator itr;
	if(dbcWMOAreaTable.begin() != dbcWMOAreaTable.end()) // NO DATERS
	{
		WMOAreaTableEntry* WMOentry = NULL;
		for(itr = dbcWMOAreaTable.begin(); itr != dbcWMOAreaTable.end(); ++itr)
		{
			WMOentry = (*itr);
			if(WMOentry->adtId == adtid && WMOentry->rootId == rootid && WMOentry->groupId == groupid)
				return WMOentry;
		}
	}
#ifdef SAFE_DBC_CODE_RETURNS
	return dbcWMOAreaTable.LookupRow(1);
#else
	return NULL;
#endif
}
