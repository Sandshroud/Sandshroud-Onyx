/***
 * Demonstrike Core
 */

#include "StdAfx.h"

bool IsDamagingSpell(SpellEntry *sp);

void ApplySingleSpellFixes(SpellEntry *sp)
{
	if(sp->School & 64)
		sp->School = SCHOOL_ARCANE;
	else if(sp->School & 32)
		sp->School = SCHOOL_SHADOW;
	else if(sp->School & 16)
		sp->School = SCHOOL_FROST;
	else if(sp->School & 8)
		sp->School = SCHOOL_NATURE;
	else if(sp->School & 4)
		sp->School = SCHOOL_FIRE;
	else if(sp->School & 2)
		sp->School = SCHOOL_HOLY;
	else if(sp->School & 1)
		sp->School = SCHOOL_NORMAL;
	else if(sp->School == NULL)
		sp->School = SCHOOL_NORMAL;
	else
		printf("UNKNOWN SCHOOL %u in spell %u\n", sp->School, sp->Id);

	// New 2.4.3 summon types
	for(uint8 i = 0; i<3; i++)
	{
		if(!sp->EffectMiscValueB[i])
			continue;

		switch(sp->EffectMiscValueB[i])
		{
			case DBC_SUMMON_TYPE_POSSESSED: sp->Effect[i] = SPELL_EFFECT_SUMMON_POSSESSED; break;
			case DBC_SUMMON_TYPE_GUARDIAN: sp->Effect[i] = SPELL_EFFECT_SUMMON_GUARDIAN; break;
			case DBC_SUMMON_TYPE_WILD: sp->Effect[i] = SPELL_EFFECT_SUMMON_WILD; break;
			case DBC_SUMMON_TYPE_DEMON: sp->Effect[i] = SPELL_EFFECT_SUMMON_DEMON; break;
			case DBC_SUMMON_TYPE_TOTEM_1: sp->Effect[i] = SPELL_EFFECT_SUMMON_TOTEM_SLOT1; break;
			case DBC_SUMMON_TYPE_TOTEM_2: sp->Effect[i] = SPELL_EFFECT_SUMMON_TOTEM_SLOT2; break;
			case DBC_SUMMON_TYPE_TOTEM_4: sp->Effect[i] = SPELL_EFFECT_SUMMON_TOTEM_SLOT4; break;
			case DBC_SUMMON_TYPE_CRITTER: sp->Effect[i] = SPELL_EFFECT_SUMMON_CRITTER; break;
			case DBC_SUMMON_TYPE_TOTEM_3: sp->Effect[i] = SPELL_EFFECT_SUMMON_TOTEM_SLOT3; break;
			case DBC_SUMMON_TYPE_CREATE_TOTEM: sp->Effect[i] = SPELL_EFFECT_SUMMON_TOTEM; break;
		}
	}

	// correct caster aura state
	if( sp->CasterAuraState != 0 )
	{
		switch( sp->CasterAuraState )
		{
		case 1:
			sp->CasterAuraState = AURASTATE_FLAG_DODGE_BLOCK;
			break;

		case 2:
			sp->CasterAuraState = AURASTATE_FLAG_HEALTH20;
			break;

		case 3:
			sp->CasterAuraState = AURASTATE_FLAG_BERSERK;
			break;

		case 5:
			sp->CasterAuraState = AURASTATE_FLAG_JUDGEMENT;
			break;

		case 7:
			sp->CasterAuraState = AURASTATE_FLAG_PARRY;
			break;

		case 10:
			sp->CasterAuraState = AURASTATE_FLAG_VICTORIOUS;
			break;

		case 11:
			sp->CasterAuraState = AURASTATE_FLAG_CRITICAL;
			break;

		case 13:
			sp->CasterAuraState = AURASTATE_FLAG_HEALTH35;
			break;

		case 14:
			sp->CasterAuraState = AURASTATE_FLAG_IMMOLATE;
			break;

		case 15:
			sp->CasterAuraState = AURASTATE_FLAG_REJUVENATE;
			break;

		case 16:
			sp->CasterAuraState = AURASTATE_FLAG_POISON;
			break;

		default:
			Log.Error("AuraState", "Spell %u (%s) has unknown caster aura state %u\n", sp->Id, sp->Name, sp->CasterAuraState);
			break;
		}
	}

	if( sp->TargetAuraState != 0 )
	{
		switch( sp->TargetAuraState )
		{
		case 1:
			sp->TargetAuraState = AURASTATE_FLAG_DODGE_BLOCK;
			break;

		case 2:
			sp->TargetAuraState = AURASTATE_FLAG_HEALTH20;
			break;

		case 3:
			sp->TargetAuraState = AURASTATE_FLAG_BERSERK;
			break;

		case 5:
			sp->TargetAuraState = AURASTATE_FLAG_JUDGEMENT;
			break;

		case 7:
			sp->TargetAuraState = AURASTATE_FLAG_PARRY;
			break;

		case 10:
			sp->TargetAuraState = AURASTATE_FLAG_VICTORIOUS;
			break;

		case 11:
			sp->TargetAuraState = AURASTATE_FLAG_CRITICAL;
			break;

		case 13:
			sp->TargetAuraState = AURASTATE_FLAG_HEALTH35;
			break;

		case 14:
			sp->TargetAuraState = AURASTATE_FLAG_IMMOLATE;
			break;

		case 15:
			sp->TargetAuraState = AURASTATE_FLAG_REJUVENATE;
			break;

		case 16:
			sp->TargetAuraState = AURASTATE_FLAG_POISON;
			break;

		default:
			Log.Error("AuraState", "Spell %u (%s) has unknown target aura state %u\n", sp->Id, sp->Name, sp->TargetAuraState);
			break;
		}
	}

	// apply on shapeshift change
	if( sp->NameHash == SPELL_HASH_TRACK_HUMANOIDS )
		sp->apply_on_shapeshift_change = true;

	if( sp->NameHash == SPELL_HASH_BLOOD_FURY || sp->NameHash == SPELL_HASH_SHADOWSTEP )
		sp->always_apply = true;

	//there are some spells that change the "damage" value of 1 effect to another : devastate = bonus first then damage
	//this is a total bullshit so remove it when spell system supports effect overwriting
	for( uint32 col1_swap = 0; col1_swap < 2 ; col1_swap++ )
	{
		for( uint32 col2_swap = col1_swap ; col2_swap < 3 ; col2_swap++ )
		{
			if( sp->Effect[col1_swap] == SPELL_EFFECT_WEAPON_PERCENT_DAMAGE && sp->Effect[col2_swap] == SPELL_EFFECT_DUMMYMELEE )
			{
				uint32 temp;
				float ftemp;
				temp = sp->Effect[col1_swap];			sp->Effect[col1_swap] = sp->Effect[col2_swap] ;						sp->Effect[col2_swap] = temp;
				temp = sp->EffectDieSides[col1_swap];	sp->EffectDieSides[col1_swap] = sp->EffectDieSides[col2_swap] ;		sp->EffectDieSides[col2_swap] = temp;
				temp = sp->EffectBaseDice[col1_swap];	sp->EffectBaseDice[col1_swap] = sp->EffectBaseDice[col2_swap] ;		sp->EffectBaseDice[col2_swap] = temp;
				ftemp = sp->EffectDicePerLevel[col1_swap];			sp->EffectDicePerLevel[col1_swap] = sp->EffectDicePerLevel[col2_swap] ;				sp->EffectDicePerLevel[col2_swap] = ftemp;
				ftemp = sp->EffectRealPointsPerLevel[col1_swap];	sp->EffectRealPointsPerLevel[col1_swap] = sp->EffectRealPointsPerLevel[col2_swap] ;	sp->EffectRealPointsPerLevel[col2_swap] = ftemp;
				temp = sp->EffectBasePoints[col1_swap];	sp->EffectBasePoints[col1_swap] = sp->EffectBasePoints[col2_swap] ;	sp->EffectBasePoints[col2_swap] = temp;
				temp = sp->EffectMechanic[col1_swap];	sp->EffectMechanic[col1_swap] = sp->EffectMechanic[col2_swap] ;	sp->EffectMechanic[col2_swap] = temp;
				temp = sp->EffectImplicitTargetA[col1_swap];	sp->EffectImplicitTargetA[col1_swap] = sp->EffectImplicitTargetA[col2_swap] ;	sp->EffectImplicitTargetA[col2_swap] = temp;
				temp = sp->EffectImplicitTargetB[col1_swap];	sp->EffectImplicitTargetB[col1_swap] = sp->EffectImplicitTargetB[col2_swap] ;	sp->EffectImplicitTargetB[col2_swap] = temp;
				temp = sp->EffectRadiusIndex[col1_swap];	sp->EffectRadiusIndex[col1_swap] = sp->EffectRadiusIndex[col2_swap] ;	sp->EffectRadiusIndex[col2_swap] = temp;
				temp = sp->EffectApplyAuraName[col1_swap];	sp->EffectApplyAuraName[col1_swap] = sp->EffectApplyAuraName[col2_swap] ;	sp->EffectApplyAuraName[col2_swap] = temp;
				temp = sp->EffectAmplitude[col1_swap];		sp->EffectAmplitude[col1_swap] = sp->EffectAmplitude[col2_swap] ;	sp->EffectAmplitude[col2_swap] = temp;
				ftemp = sp->EffectMultipleValue[col1_swap];		sp->EffectMultipleValue[col1_swap] = sp->EffectMultipleValue[col2_swap] ;	sp->EffectMultipleValue[col2_swap] = ftemp;
				temp = sp->EffectChainTarget[col1_swap];	sp->EffectChainTarget[col1_swap] = sp->EffectChainTarget[col2_swap] ;	sp->EffectChainTarget[col2_swap] = temp;
				temp = sp->EffectSpellGroupRelation[col1_swap];	sp->EffectSpellGroupRelation[col1_swap] = sp->EffectSpellGroupRelation[col2_swap] ;	sp->EffectSpellGroupRelation[col2_swap] = temp;
				temp = sp->EffectMiscValue[col1_swap];		sp->EffectMiscValue[col1_swap] = sp->EffectMiscValue[col2_swap] ;	sp->EffectMiscValue[col2_swap] = temp;
				temp = sp->EffectTriggerSpell[col1_swap];	sp->EffectTriggerSpell[col1_swap] = sp->EffectTriggerSpell[col2_swap] ;	sp->EffectTriggerSpell[col2_swap] = temp;
				ftemp = sp->EffectPointsPerComboPoint[col1_swap];	sp->EffectPointsPerComboPoint[col1_swap] = sp->EffectPointsPerComboPoint[col2_swap] ;	sp->EffectPointsPerComboPoint[col2_swap] = ftemp;
			}
		}
	}

	for(uint32 b=0;b<3;++b)
	{
		if(sp->EffectTriggerSpell[b] != 0 && dbcSpell.LookupEntryForced(sp->EffectTriggerSpell[b]) == NULL)
		{
			/* proc spell referencing non-existant spell. create a dummy spell for use w/ it. */
			CreateDummySpell(sp->EffectTriggerSpell[b]);
		}
		/** Load teaching spells (used for hunters when learning pets wild abilities) */
		if(sp->Effect[b]==SPELL_EFFECT_LEARN_SPELL && sp->EffectImplicitTargetA[b]==EFF_TARGET_PET)
		{
			map<uint32,uint32>::iterator itr = sWorld.TeachingSpellMap.find(sp->EffectTriggerSpell[b]);
			if(itr == sWorld.TeachingSpellMap.end())
				sWorld.TeachingSpellMap.insert(make_pair(sp->EffectTriggerSpell[b],sp->Id));
		}

		if( sp->Attributes & ATTRIBUTES_ONLY_OUTDOORS && sp->EffectApplyAuraName[b] == SPELL_AURA_MOUNTED )
		{
			sp->Attributes &= ~ATTRIBUTES_ONLY_OUTDOORS;
		}
	}

	if(!strcmp(sp->Name, "Hearthstone") || !strcmp(sp->Name, "Stuck") || !strcmp(sp->Name, "Astral Recall"))
		sp->self_cast_only = true;

	// parse rank text
	if( !sscanf( sp->Rank, "Rank %d", (unsigned int*)&sp->RankNumber) )
		sp->RankNumber = 0;

	//seal of light 
	if( sp->NameHash == SPELL_HASH_SEAL_OF_LIGHT )			
		sp->procChance = 45;	/* this will do */

	//seal of command
	else if( sp->NameHash == SPELL_HASH_SEAL_OF_COMMAND )		
		sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC;

	//judgement of command
	else if( sp->NameHash == SPELL_HASH_JUDGEMENT_OF_COMMAND )		
		sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC;

	//Rogue: Posion time fix for 2.3
	if( strstr( sp->Name, "Crippling Poison") && sp->Effect[0] == 54 ) //I, II
		sp->EffectBasePoints[0] = 3599;
	if( strstr( sp->Name, "Mind-numbing Poison") && sp->Effect[0] == 54 ) //I,II,III
		sp->EffectBasePoints[0] = 3599;
	if( strstr( sp->Name, "Instant Poison") && sp->Effect[0] == 54 ) //I,II,III,IV,V,VI,VII	
		sp->EffectBasePoints[0] = 3599;
	if( strstr( sp->Name, "Deadly Poison") && sp->Effect[0] == 54 ) //I,II,III,IV,V,VI,VII
		sp->EffectBasePoints[0] = 3599;
	if( strstr( sp->Name, "Wound Poison") && sp->Effect[0] == 54 ) //I,II,III,IV,V
		sp->EffectBasePoints[0] = 3599;
	if( strstr( sp->Name, "Anesthetic Poison") && sp->Effect[0] == 54 ) //I
		sp->EffectBasePoints[0] = 3599;

	if( strstr( sp->Name, "Sharpen Blade") && sp->Effect[0] == 54 ) //All BS stones
		sp->EffectBasePoints[0] = 3599;

	uint32 type = 0;

	//these mostly do not mix so we can use else 
	// look for seal, etc in name
	if( strstr( sp->Name, "Seal"))
		type |= SPELL_TYPE_SEAL;
	else if( strstr( sp->Name, "Blessing"))
		type |= SPELL_TYPE_BLESSING;
	else if( strstr( sp->Name, "Curse"))
		type |= SPELL_TYPE_CURSE;
	else if( strstr( sp->Name, "Aspect"))
		type |= SPELL_TYPE_ASPECT;
	else if( strstr( sp->Name, "Sting") || strstr( sp->Name, "sting"))
		type |= SPELL_TYPE_STING;
	// don't break armor items!
	else if(strcmp(sp->Name, "Armor") && strstr( sp->Name, "Armor") || strstr( sp->Name, "Demon Skin"))
		type |= SPELL_TYPE_ARMOR;
	else if( strstr( sp->Name, "Aura"))
		type |= SPELL_TYPE_AURA;
	else if( strstr( sp->Name, "Track")==sp->Name)
		type |= SPELL_TYPE_TRACK;
	else if( sp->NameHash == SPELL_HASH_GIFT_OF_THE_WILD || sp->NameHash == SPELL_HASH_MARK_OF_THE_WILD )
		type |= SPELL_TYPE_MARK_GIFT;
	else if( sp->NameHash == SPELL_HASH_IMMOLATION_TRAP || sp->NameHash == SPELL_HASH_FREEZING_TRAP || sp->NameHash == SPELL_HASH_FROST_TRAP || sp->NameHash == SPELL_HASH_EXPLOSIVE_TRAP || sp->NameHash == SPELL_HASH_SNAKE_TRAP )
		type |= SPELL_TYPE_HUNTER_TRAP;
	else if( sp->NameHash == SPELL_HASH_ARCANE_INTELLECT || sp->NameHash == SPELL_HASH_ARCANE_BRILLIANCE )
		type |= SPELL_TYPE_MAGE_INTEL;
	else if( sp->NameHash == SPELL_HASH_AMPLIFY_MAGIC || sp->NameHash == SPELL_HASH_DAMPEN_MAGIC )
		type |= SPELL_TYPE_MAGE_MAGI;
	else if( sp->NameHash == SPELL_HASH_FIRE_WARD || sp->NameHash == SPELL_HASH_FROST_WARD )
		type |= SPELL_TYPE_MAGE_WARDS;
	else if( sp->NameHash == SPELL_HASH_SHADOW_PROTECTION || sp->NameHash == SPELL_HASH_PRAYER_OF_SHADOW_PROTECTION )
		type |= SPELL_TYPE_PRIEST_SH_PPROT;
	else if( sp->NameHash == SPELL_HASH_WATER_SHIELD || sp->NameHash == SPELL_HASH_EARTH_SHIELD || sp->NameHash == SPELL_HASH_LIGHTNING_SHIELD )
		type |= SPELL_TYPE_SHIELD;
	else if( sp->NameHash == SPELL_HASH_POWER_WORD__FORTITUDE || sp->NameHash == SPELL_HASH_PRAYER_OF_FORTITUDE )
		type |= SPELL_TYPE_FORTITUDE;
	else if( sp->NameHash == SPELL_HASH_DIVINE_SPIRIT || sp->NameHash == SPELL_HASH_PRAYER_OF_SPIRIT )
		type |= SPELL_TYPE_SPIRIT;
	else if( strstr( sp->Name, "Immolate") || strstr( sp->Name, "Conflagrate"))
		type |= SPELL_TYPE_WARLOCK_IMMOLATE;
	else if( strstr( sp->Name, "Amplify Magic") || strstr( sp->Name, "Dampen Magic"))
		type |= SPELL_TYPE_MAGE_AMPL_DUMP;
	else if( strstr( sp->Description, "Battle Elixir"))
		type |= SPELL_TYPE_ELIXIR_BATTLE;
	else if( strstr( sp->Description, "Guardian Elixir"))
		type |= SPELL_TYPE_ELIXIR_GUARDIAN;
	else if( strstr( sp->Description, "Battle and Guardian elixir"))
		type |= SPELL_TYPE_ELIXIR_FLASK;
	else if( sp->NameHash == SPELL_HASH_HUNTER_S_MARK )		// hunter's mark
		type |= SPELL_TYPE_HUNTER_MARK;
	else if( sp->NameHash == SPELL_HASH_COMMANDING_SHOUT || sp->NameHash == SPELL_HASH_BATTLE_SHOUT )
		type |= SPELL_TYPE_WARRIOR_SHOUT;
	else if( strstr( sp->Description, "Finishing move")==sp->Description)
		sp->c_is_flags |= SPELL_FLAG_IS_FINISHING_MOVE;
	if( IsDamagingSpell( sp ) )
		sp->c_is_flags |= SPELL_FLAG_IS_DAMAGING;
	if( IsHealingSpell( sp ) )
		sp->c_is_flags |= SPELL_FLAG_IS_HEALING;
	if( IsTargetingStealthed( sp ) )
		sp->c_is_flags |= SPELL_FLAG_IS_TARGETINGSTEALTHED;
	if( IsFlyingSpell(sp) )
		sp->c_is_flags |= SPELL_FLAG_IS_FLYING;
		
	for(uint32 b = 0; b < 3; ++b)
	{
		if( sp->Effect[b] == SPELL_EFFECT_PERSISTENT_AREA_AURA )
		{
			/*if( sp->EffectImplicitTargetB[b] != 0 )
			printf("%s %s - A: %u, B: %u\n", sp->Name, sp->Rank, sp->EffectImplicitTargetA[b], sp->EffectImplicitTargetB[b]);*/

			sp->EffectImplicitTargetA[b] = EFF_TARGET_SELF;
			sp->EffectImplicitTargetB[b] = 0;
		}
	}

	//stupid spell ranking problem
	if(sp->spellLevel==0)
	{
		uint32 new_level=0;
		if( strstr( sp->Name, "Apprentice "))
			new_level = 1;
		else if( strstr( sp->Name, "Journeyman "))
			new_level = 2;
		else if( strstr( sp->Name, "Expert "))
			new_level = 3;
		else if( strstr( sp->Name, "Artisan "))
			new_level = 4;
		else if( strstr( sp->Name, "Master "))
			new_level = 5;
		if(new_level!=0)
		{
			uint32 teachspell=0;
			if(sp->Effect[0]==SPELL_EFFECT_LEARN_SPELL)
				teachspell = sp->EffectTriggerSpell[0];
			else if(sp->Effect[1]==SPELL_EFFECT_LEARN_SPELL)
				teachspell = sp->EffectTriggerSpell[1];
			else if(sp->Effect[2]==SPELL_EFFECT_LEARN_SPELL)
				teachspell = sp->EffectTriggerSpell[2];
			if(teachspell)
			{
				SpellEntry *spellInfo;
				spellInfo = dbcSpell.LookupEntryForced(teachspell);
				spellInfo->spellLevel = new_level;
				sp->spellLevel = new_level;
			}
		}
	}

	// find diminishing status
	sp->DiminishStatus = GetDiminishingGroup(sp->NameHash);
	sp->buffIndexType = 0;
	switch(sp->NameHash)
	{
	case SPELL_HASH_HUNTER_S_MARK:		// Hunter's mark
		sp->buffIndexType = SPELL_TYPE_INDEX_MARK;
		break;

	case SPELL_HASH_POLYMORPH:			// Polymorph
	case SPELL_HASH_POLYMORPH__CHICKEN:	// Polymorph: Chicken
	case SPELL_HASH_POLYMORPH__PIG:		// Polymorph: Pig
	case SPELL_HASH_POLYMORPH__SHEEP:	// Polymorph: Sheep
	case SPELL_HASH_POLYMORPH__TURTLE:	// Polymorph: Turtle
		sp->buffIndexType = SPELL_TYPE_INDEX_POLYMORPH;
		break;

	case SPELL_HASH_FEAR:				// Fear
		sp->buffIndexType = SPELL_TYPE_INDEX_FEAR;
		break;

	case SPELL_HASH_SAP:				// Sap
		sp->buffIndexType = SPELL_TYPE_INDEX_SAP;
		break;

	case SPELL_HASH_SCARE_BEAST:		// Scare Beast
		sp->buffIndexType = SPELL_TYPE_INDEX_SCARE_BEAST;
		break;

	case SPELL_HASH_HIBERNATE:			// Hibernate
		sp->buffIndexType = SPELL_TYPE_INDEX_HIBERNATE;
		break;

	case SPELL_HASH_CYCLONE:			// Cyclone
		sp->buffIndexType = SPELL_TYPE_INDEX_CYCLONE;
		break;

	case SPELL_HASH_BANISH:				// Banish
		sp->buffIndexType = SPELL_TYPE_INDEX_BANISH;
		break;

	//case SPELL_HASH_JUDGEMENT_OF_VENGEANCE:
	case SPELL_HASH_JUDGEMENT_OF_THE_CRUSADER:
	case SPELL_HASH_JUDGEMENT_OF_LIGHT:
	case SPELL_HASH_JUDGEMENT_OF_WISDOM:
	case SPELL_HASH_JUDGEMENT_OF_JUSTICE:
		sp->buffIndexType = SPELL_TYPE_INDEX_JUDGEMENT;
		break;
	}

	// PROCS PERMINUTE
	switch( sp->NameHash )
	{
	case SPELL_HASH_MAGTHERIDON_MELEE_TRINKET:
		sp->ProcsPerMinute = 1.5f;
		break; // dragonspine trophy
	case SPELL_HASH_ROMULO_S_POISON:
		sp->ProcsPerMinute = 1.5f;
		break; // romulo's
	case SPELL_HASH_BLACK_TEMPLE_MELEE_TRINKET:
		sp->ProcsPerMinute = 1.0f;
		break; // madness of the betrayer
	case SPELL_HASH_BAND_OF_THE_ETERNAL_CHAMPION:
		sp->proc_interval = 30;
		break;
	case SPELL_HASH_BAND_OF_THE_ETERNAL_DEFENDER:
		sp->proc_interval = 30;
		break;
	case SPELL_HASH_BAND_OF_THE_ETERNAL_RESTORER:
		sp->proc_interval = 30;
		break;
	case SPELL_HASH_BAND_OF_THE_ETERNAL_SAGE:
		sp->proc_interval = 30;
		break;
	}

	// HACK FIX: Break roots/fear on damage.. this needs to be fixed properly!
	if(!(sp->AuraInterruptFlags & AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN))
	{
		for(uint8 z = 0; z < 3; ++z) {
			if(sp->EffectApplyAuraName[z] == SPELL_AURA_MOD_FEAR ||
				sp->EffectApplyAuraName[z] == SPELL_AURA_MOD_ROOT)
			{
				sp->AuraInterruptFlags |= 0x200;
				break;
			}
		}
	}

	for(uint8 z = 0;z < 3; ++z)
	{
		if( ( sp->Effect[z] == SPELL_EFFECT_SCHOOL_DAMAGE && sp->Spell_Dmg_Type == SPELL_DMG_TYPE_MELEE ) || sp->Effect[z] == SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL || sp->Effect[z] == SPELL_EFFECT_WEAPON_DAMAGE || sp->Effect[z] == SPELL_EFFECT_WEAPON_PERCENT_DAMAGE || sp->Effect[z] == SPELL_EFFECT_DUMMYMELEE )
			sp->is_melee_spell = true;
		if( ( sp->Effect[z] == SPELL_EFFECT_SCHOOL_DAMAGE && sp->Spell_Dmg_Type == SPELL_DMG_TYPE_RANGED ) )
		{
			//sLog.outString( "Ranged Spell: %u [%s]" , sp->Id , sp->Name );
			sp->is_ranged_spell = true;
		}

		switch(sp->EffectImplicitTargetA[z])
		{
			case 6:
			case 22:
			case 24:
			case 25:
				sp->c_is_flags |= SPELL_FLAG_CAN_BE_REFLECTED;
				break;
		}

		if( sp->Effect[z] == SPELL_EFFECT_DISPEL )
			sp->c_is_flags |= SPELL_FLAG_IS_DISPEL_SPELL;

		if( sp->Effect[z] == SPELL_EFFECT_HEAL )
			sp->c_is_flags |= SPELL_FLAG_IS_HEALING_SPELL;

		if( sp->Effect[z] == SPELL_EFFECT_ENERGIZE )
			sp->c_is_flags |= SPELL_FLAG_IS_HEALING_MANA_SPELL;
	}

	// set extra properties
	sp->buffType = type;
	uint32 effect = 0, result = 0;

	uint32 pr = sp->procFlags;
	for(uint32 y=0;y < 3; y++)
	{
		// get the effect number from the spell
		effect = sp->Effect[0]; // spelleffect[0] = 64 // 2.0.1

		if(effect==SPELL_EFFECT_APPLY_AURA)
		{
			uint32 aura = sp->EffectApplyAuraName[y]; // 58+30+3 = 91
			if( aura == SPELL_AURA_PROC_TRIGGER_SPELL ||
				aura == SPELL_AURA_PROC_TRIGGER_DAMAGE
				)//search for spellid in sp->Descriptionription
			{
				const char *p=sp->Description;
				while((p=strstr(p,"$")))
				{
					p++;
					//got $  -> check if spell
					if(*p>='0' && *p <='9')
					{//woot this is spell id
						
						result=atoi(p);
					}
				}
				pr=0;

				uint32 len = (uint32)strlen(sp->Description);
				for(uint32 i = 0; i < len; ++i)
					sp->Description[i] = tolower(sp->Description[i]);
				//dirty code for procs, if any1 got any better idea-> u are welcome
				//139944 --- some magic number, it will trigger on all hits etc
					//for seems to be smth like custom check
				if( strstr( sp->Description,"your ranged criticals"))
					pr|=PROC_ON_RANGED_CRIT_ATTACK;
				if( strstr( sp->Description,"chance on hit"))
					pr|=PROC_ON_MELEE_ATTACK;
				if( strstr( sp->Description,"takes damage"))
					pr|=PROC_ON_ANY_DAMAGE_VICTIM;
				if( strstr( sp->Description,"attackers when hit"))
					pr|=PROC_ON_MELEE_ATTACK_VICTIM;
				if( strstr( sp->Description,"character strikes an enemy"))
					pr|=PROC_ON_MELEE_ATTACK;
				if( strstr( sp->Description,"strike you with a melee attack"))
					pr|=PROC_ON_MELEE_ATTACK_VICTIM;
				if( strstr( sp->Description,"target casts a spell"))
					pr|=PROC_ON_CAST_SPELL;
				if( strstr( sp->Description,"your harmful spells land"))
					pr|=PROC_ON_CAST_SPELL;
				if( strstr( sp->Description,"on spell critical hit"))
					pr|=PROC_ON_SPELL_CRIT_HIT;
				if( strstr( sp->Description,"spell critical strikes"))
					pr|=PROC_ON_SPELL_CRIT_HIT;
				if( strstr( sp->Description,"being able to resurrect"))
					pr|=PROC_ON_DIE;
				if( strstr( sp->Description,"any damage caused"))
					pr|=PROC_ON_ANY_DAMAGE_VICTIM;
				if( strstr( sp->Description,"the next melee attack against the caster"))
					pr|=PROC_ON_MELEE_ATTACK_VICTIM;
				if( strstr( sp->Description,"when successfully hit"))
					pr|=PROC_ON_MELEE_ATTACK ;
				if( strstr( sp->Description,"an enemy on hit"))
					pr|=PROC_ON_MELEE_ATTACK;
				if( strstr( sp->Description,"when it hits"))
					pr|=PROC_ON_MELEE_ATTACK;
				if( strstr( sp->Description,"when successfully hit"))
					pr|=PROC_ON_MELEE_ATTACK;
				if( strstr( sp->Description,"on a successful hit"))
					pr|=PROC_ON_MELEE_ATTACK;
				if( strstr( sp->Description,"damage to attacker on hit"))
					pr|=PROC_ON_MELEE_ATTACK_VICTIM;
				if( strstr( sp->Description,"on a hit"))
					pr|=PROC_ON_MELEE_ATTACK;
				if( strstr( sp->Description,"strikes you with a melee attack"))
					pr|=PROC_ON_MELEE_ATTACK_VICTIM;
				if( strstr( sp->Description,"when caster takes damage"))
					pr|=PROC_ON_ANY_DAMAGE_VICTIM;
				if( strstr( sp->Description,"when the caster is using melee attacks"))
					pr|=PROC_ON_MELEE_ATTACK;
				if( strstr( sp->Description,"when struck in combat") || strstr(sp->Description,"When struck in combat"))
					pr|=PROC_ON_MELEE_ATTACK_VICTIM;
				if( strstr( sp->Description,"successful melee attack"))
					pr|=PROC_ON_MELEE_ATTACK;
				if( strstr( sp->Description,"chance per attack"))
					pr|=PROC_ON_MELEE_ATTACK;
				if( strstr( sp->Description,"chance per hit"))
					pr|=PROC_ON_MELEE_ATTACK;
				if( strstr( sp->Description,"that strikes a party member"))
					pr|=PROC_ON_MELEE_ATTACK_VICTIM;
				if( strstr( sp->Description,"when hit by a melee attack"))
					pr|=PROC_ON_MELEE_ATTACK_VICTIM;
				if( strstr( sp->Description,"landing a melee critical strike"))
					pr|=PROC_ON_CRIT_ATTACK;
				if( strstr( sp->Description,"your critical strikes"))
					pr|=PROC_ON_CRIT_ATTACK;
				if( strstr( sp->Description,"whenever you deal ranged damage"))
					pr|=PROC_ON_RANGED_ATTACK;
//					if( strstr( sp->Description,"whenever you deal melee damage"))
				if( strstr( sp->Description,"you deal melee damage"))
					pr|=PROC_ON_MELEE_ATTACK;
				if( strstr( sp->Description,"your melee attacks"))
					pr|=PROC_ON_MELEE_ATTACK;
				if( strstr( sp->Description,"damage with your Sword"))
					pr|=PROC_ON_MELEE_ATTACK;
				if( strstr( sp->Description,"when struck in melee combat"))
					pr|=PROC_ON_MELEE_ATTACK_VICTIM;
				if( strstr( sp->Description,"any successful spell cast against the priest"))
					pr|=PROC_ON_SPELL_HIT_VICTIM;
				if( strstr( sp->Description,"the next melee attack on the caster"))
					pr|=PROC_ON_MELEE_ATTACK_VICTIM;
				if( strstr( sp->Description,"striking melee or ranged attackers"))
					pr|=PROC_ON_MELEE_ATTACK_VICTIM|PROC_ON_RANGED_ATTACK_VICTIM;
				if( strstr( sp->Description,"when damaging an enemy in melee"))
					pr|=PROC_ON_MELEE_ATTACK;
				if( strstr( sp->Description,"victim of a critical strike"))
					pr|=PROC_ON_CRIT_HIT_VICTIM;
				if( strstr( sp->Description,"on successful melee or ranged attack"))
					pr|=PROC_ON_MELEE_ATTACK|PROC_ON_RANGED_ATTACK;
				if( strstr( sp->Description,"enemy that strikes you in melee"))
					pr|=PROC_ON_MELEE_ATTACK_VICTIM;
				if( strstr( sp->Description,"after getting a critical strike"))
					pr|=PROC_ON_CRIT_ATTACK;
				if( strstr( sp->Description,"whenever damage is dealt to you"))
					pr|=PROC_ON_ANY_DAMAGE_VICTIM;
				if( strstr( sp->Description,"when ranged or melee damage is dealt"))
					pr|=PROC_ON_MELEE_ATTACK|PROC_ON_RANGED_ATTACK;
				if( strstr( sp->Description,"damaging melee attacks"))
					pr|=PROC_ON_MELEE_ATTACK;
				if( strstr( sp->Description,"on melee or ranged attack"))
					pr|=PROC_ON_MELEE_ATTACK|PROC_ON_RANGED_ATTACK;
				if( strstr( sp->Description,"on a melee swing"))
					pr|=PROC_ON_MELEE_ATTACK;
				if( strstr( sp->Description,"Chance on melee"))
					pr|=PROC_ON_MELEE_ATTACK;
				if( strstr( sp->Description,"spell criticals against you"))
					pr|=PROC_ON_SPELL_CRIT_HIT_VICTIM;
				if( strstr( sp->Description,"after being struck by a melee or ranged critical hit"))
					pr|=PROC_ON_CRIT_HIT_VICTIM;
//					if( strstr( sp->Description,"on a critical hit"))
				if( strstr( sp->Description,"critical hit"))
					pr|=PROC_ON_CRIT_ATTACK;
				if( strstr( sp->Description,"strikes the caster"))
					pr|=PROC_ON_MELEE_ATTACK_VICTIM;
				if( strstr( sp->Description,"a spell, melee or ranged attack hits the caster"))
					pr|=PROC_ON_ANY_DAMAGE_VICTIM;
				if( strstr( sp->Description,"after dealing a critical strike"))
					pr|=PROC_ON_CRIT_ATTACK;
				if( strstr( sp->Description,"each melee or ranged damage hit against the priest"))
					pr|=PROC_ON_MELEE_ATTACK_VICTIM|PROC_ON_RANGED_ATTACK_VICTIM;				
				if( strstr( sp->Description, "a chance to deal additional"))
					pr|=PROC_ON_MELEE_ATTACK;
				if( strstr( sp->Description, "chance to get an extra attack"))
					pr|=PROC_ON_MELEE_ATTACK;
				if( strstr( sp->Description, "melee attacks has"))
					pr|=PROC_ON_MELEE_ATTACK;
				if( strstr( sp->Description, "any damage spell hits a target"))
					pr|=PROC_ON_CAST_SPELL;
				if( strstr( sp->Description, "giving each melee attack a chance"))
					pr|=PROC_ON_MELEE_ATTACK;
				if( strstr( sp->Description, "damage when hit"))
					pr|=PROC_ON_ANY_DAMAGE_VICTIM; //myabe melee damage ?
				if( strstr( sp->Description, "gives your"))
				{
					if( strstr( sp->Description, "melee"))
						pr|=PROC_ON_MELEE_ATTACK;
					else if( strstr( sp->Description,"sinister strike, backstab, gouge and shiv"))
						pr|=PROC_ON_CAST_SPELL;
					else if( strstr( sp->Description,"chance to daze the target"))
						pr|=PROC_ON_CAST_SPELL;
					else if( strstr( sp->Description,"finishing moves"))
						pr|=PROC_ON_CAST_SPELL;
					//we should find that specific spell (or group) on what we will trigger
					else pr|=PROC_ON_CAST_SPECIFIC_SPELL;
				}
				if( strstr( sp->Description, "chance to add an additional combo") && strstr(sp->Description, "critical") )
					pr|=PROC_ON_CRIT_ATTACK;
				else if( strstr( sp->Description, "chance to add an additional combo"))
					pr|=PROC_ON_CAST_SPELL;
				if( strstr( sp->Description, "victim of a melee or ranged critical strike"))
					pr|=PROC_ON_CRIT_HIT_VICTIM;
				if( strstr( sp->Description, "getting a critical effect from"))
					pr|=PROC_ON_SPELL_CRIT_HIT_VICTIM;
				if( strstr( sp->Description, "damaging attack is taken"))
					pr|=PROC_ON_ANY_DAMAGE_VICTIM;
				if( strstr( sp->Description, "struck by a Stun or Immobilize"))
					pr|=PROC_ON_SPELL_HIT_VICTIM;
				if( strstr( sp->Description, "melee critical strike"))
					pr|=PROC_ON_CRIT_ATTACK;
				if( strstr( sp->Name, "Bloodthirst"))
					pr|=PROC_ON_MELEE_ATTACK | PROC_TARGET_SELF;
				if( strstr( sp->Description, "experience or honor"))
					pr|=PROC_ON_GAIN_EXPIERIENCE;
				if( strstr( sp->Description,"your next offensive ability"))
					pr|=PROC_ON_CAST_SPELL;
				if( strstr( sp->Description,"hit by a melee or ranged attack"))
					pr|=PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM;
				if( strstr( sp->Description,"enemy strikes the caster"))
					pr|=PROC_ON_MELEE_ATTACK_VICTIM;
				if( strstr( sp->Description,"melee and ranged attacks against you"))
					pr|=PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM;
				if( strstr( sp->Description,"when a block occurs"))
					pr|=PROC_ON_BLOCK_VICTIM;
				if( strstr( sp->Description,"dealing a critical strike from a weapon swing, spell, or ability"))
					pr|=PROC_ON_CRIT_ATTACK|PROC_ON_SPELL_CRIT_HIT;
				if( strstr( sp->Description,"dealing a critical strike from a weapon swing, spell, or ability"))
					pr|=PROC_ON_CRIT_ATTACK|PROC_ON_SPELL_CRIT_HIT;
				if( strstr( sp->Description,"shadow bolt critical strikes increase shadow damage"))
					pr|=PROC_ON_SPELL_CRIT_HIT;
				if( strstr( sp->Description,"next offensive ability"))
					pr|=PROC_ON_CAST_SPELL;
				if( strstr( sp->Description,"after being hit with a shadow or fire spell"))
					pr|=PROC_ON_SPELL_LAND_VICTIM;
				if( strstr( sp->Description,"giving each melee attack"))
					pr|=PROC_ON_MELEE_ATTACK;
				if( strstr( sp->Description,"each strike has"))
					pr|=PROC_ON_MELEE_ATTACK;		
				if( strstr( sp->Description,"your Fire damage spell hits"))
					pr|=PROC_ON_CAST_SPELL;		//this happens only on hit ;)
				if( strstr( sp->Description,"corruption, curse of agony, siphon life and seed of corruption spells also cause"))
					pr|=PROC_ON_CAST_SPELL;
				if( strstr( sp->Description,"pain, mind flay and vampiric touch spells also cause"))
					pr|=PROC_ON_CAST_SPELL;
				if( strstr( sp->Description,"shadow damage spells have"))
					pr|=PROC_ON_CAST_SPELL;
				if( strstr( sp->Description,"on successful spellcast"))
					pr|=PROC_ON_CAST_SPELL;
				if( strstr( sp->Description,"your spell criticals have"))
					pr|=PROC_ON_SPELL_CRIT_HIT | PROC_ON_SPELL_CRIT_HIT_VICTIM;
				if( strstr( sp->Description,"after dodging their attack"))
				{
					pr|=PROC_ON_DODGE_VICTIM;
					if( strstr( sp->Description,"add a combo point"))
						pr|=PROC_TARGET_SELF;
				}
				if( strstr( sp->Description,"Your Shadow Word: Pain, Mind Flay and Vampiric Touch spells also cause the target"))
					pr|=PROC_ON_CAST_SPELL;
				if( strstr( sp->Description,"chance on spell hit"))
					pr|=PROC_ON_CAST_SPELL;
				if( strstr( sp->Description,"your spell casts have"))
					pr|=PROC_ON_CAST_SPELL;
				if( strstr( sp->Description,"chance on spell cast"))
					pr|=PROC_ON_CAST_SPELL;
				if( strstr( sp->Description,"your melee and ranged attacks"))
					pr|=PROC_ON_MELEE_ATTACK|PROC_ON_RANGED_ATTACK;
				//////////////////////////////////////////////////
				//proc dmg flags
				//////////////////////////////////////////////////
				if( strstr( sp->Description,"each attack blocked"))
					pr|=PROC_ON_BLOCK_VICTIM;
				if( strstr( sp->Description,"into flame, causing an additional"))
					pr|=PROC_ON_MELEE_ATTACK;
				if( strstr( sp->Description,"victim of a critical melee strike"))
					pr|=PROC_ON_CRIT_HIT_VICTIM;
				if( strstr( sp->Description,"damage to melee attackers"))
					pr|=PROC_ON_MELEE_ATTACK;
				if( strstr( sp->Description,"target blocks a melee attack"))
					pr|=PROC_ON_BLOCK_VICTIM;
				if( strstr( sp->Description,"ranged and melee attacks to deal"))
					pr|=PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM;
				if( strstr( sp->Description,"damage on hit"))
					pr|=PROC_ON_ANY_DAMAGE_VICTIM;
				if( strstr( sp->Description,"chance on hit"))
					pr|=PROC_ON_MELEE_ATTACK;
				if( strstr( sp->Description,"after being hit by any damaging attack"))
					pr|=PROC_ON_ANY_DAMAGE_VICTIM;
				if( strstr( sp->Description,"striking melee or ranged attackers"))
					pr|=PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM;
				if( strstr( sp->Description,"damage to attackers when hit"))
					pr|=PROC_ON_MELEE_ATTACK_VICTIM;
				if( strstr( sp->Description,"striking melee attackers"))
					pr|=PROC_ON_MELEE_ATTACK_VICTIM;
				if( strstr( sp->Description,"whenever the caster takes damage"))
					pr|=PROC_ON_ANY_DAMAGE_VICTIM;
				if( strstr( sp->Description,"damage on every attack"))
					pr|=PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
				if( strstr( sp->Description,"chance to reflect Fire spells"))
					pr|=PROC_ON_SPELL_HIT_VICTIM;
				if( strstr( sp->Description,"hunter takes on the aspects of a hawk"))
					pr|=PROC_TARGET_SELF | PROC_ON_RANGED_ATTACK;
				if( strstr( sp->Description,"successful auto shot attacks"))
					pr|=PROC_ON_AUTO_SHOT_HIT;
				if( strstr( sp->Description,"after getting a critical effect from your"))
					pr=PROC_ON_SPELL_CRIT_HIT;
			}//end "if procspellaura"
			//dirty fix to remove auras that should expire on event and they are not
		}//end "if aura"
	}//end "for each effect"
	sp->procFlags = pr;

	if( strstr( sp->Description, "Must remain seated"))
	{
		sp->RecoveryTime = 1000;
		sp->CategoryRecoveryTime = 1000;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// procintervals
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//omg lighning shield trigger spell id's are all wrong ?
	//if you are bored you could make thiese by hand but i guess we might find other spells with this problem..and this way it's safe
	if( strstr( sp->Name, "Lightning Shield" ) && sp->EffectTriggerSpell[0] )
	{
		//check if we can find in the desription
		char *startofid = strstr(sp->Description, "for $");
		if( startofid )
		{
			startofid += strlen("for $");
			sp->EffectTriggerSpell[0] = atoi( startofid ); //get new lightning shield trigger id
		}
		sp->proc_interval = 3000; //few seconds
	}
	//mage ignite talent should proc only on some chances
	else if( strstr( sp->Name, "Ignite") && sp->Id>=11119 && sp->Id<=12848 && sp->EffectApplyAuraName[0] == 4 )
	{
		//check if we can find in the desription
		char *startofid=strstr(sp->Description, "an additional ");
		if(startofid)
		{
			startofid += strlen("an additional ");
			sp->EffectBasePoints[0]=atoi(startofid); //get new value. This is actually level*8 ;)
		}
		sp->Effect[0] = 6; //aura
		sp->EffectApplyAuraName[0] = 42; //force him to use procspell effect
		sp->EffectTriggerSpell[0] = 12654; //evil , but this is good for us :D
		sp->procFlags = PROC_ON_SPELL_CRIT_HIT; //add procflag here since this was not processed with the others !
	}
	// Winter's Chill handled by frost school
	else if( strstr( sp->Name, "Winter's Chill"))
	{
		sp->School = 4;
	}
	// Blackout handled by Shadow school
	else if( strstr( sp->Name, "Blackout"))
	{
		sp->School = 5;
	}
	//Improved Aspect of the Hawk
	else if( strstr( sp->Name, "Improved Aspect of the Hawk"))
		sp->EffectSpellGroupRelation[1] = 0x00100000;
	//more triggered spell ids are wrong. I think blizz is trying to outsmart us :S
	else if( strstr( sp->Name, "Nature's Guardian"))
	{
		sp->EffectTriggerSpell[0] = 31616;
		sp->proc_interval = 5000;
	}
	//Chain Heal all ranks %50 heal value (49 + 1)
	else if( strstr( sp->Name, "Chain Heal"))
	{
		sp->EffectDieSides[0] = 49;
	}
	//this starts to be an issue for trigger spell id : Deep Wounds
	else if( strstr( sp->Name, "Deep Wounds") && sp->EffectTriggerSpell[0])
	{
		//check if we can find in the desription
		char *startofid=strstr(sp->Description, "over $");
		if(startofid)
		{
			startofid += strlen("over $");
			sp->EffectTriggerSpell[0] = atoi(startofid);
		}
	}
	else if( strstr( sp->Name, "Holy Shock"))
	{
		//check if we can find in the desription
		char *startofid=strstr(sp->Description, "causing $");
		if(startofid)
		{
			startofid += strlen("causing $");
			sp->EffectTriggerSpell[0] = atoi(startofid);
		}
		//check if we can find in the desription
		startofid=strstr(sp->Description, " or $");
		if(startofid)
		{
			startofid += strlen(" or $");
			sp->EffectTriggerSpell[1]=atoi(startofid);
		}
	}
	else if( strstr( sp->Name, "Touch of Weakness"))
	{
		//check if we can find in the desription
		char *startofid=strstr(sp->Description, "cause $");
		if(startofid)
		{
			startofid += strlen("cause $");
			sp->EffectTriggerSpell[0] = atoi(startofid);
			sp->EffectTriggerSpell[1]=sp->EffectTriggerSpell[0]; //later versions of this spell changed to eff[1] the aura
			sp->procFlags = uint32(PROC_ON_MELEE_ATTACK_VICTIM);
		}
	}
	else if( strstr( sp->Name, "Firestone Passive"))
	{
		//Enchants the main hand weapon with fire, granting each attack a chance to deal $17809s1 additional fire damage.
		//check if we can find in the desription
		char * startofid=strstr(sp->Description, "to deal $");
		if(startofid)
		{
			startofid += strlen("to deal $");
			sp->EffectTriggerSpell[0] = atoi(startofid);
			sp->EffectApplyAuraName[0] = 42;
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->procChance = 50;
		}
	}
	//some procs trigger at intervals
	else if( strstr( sp->Name, "Water Shield"))
	{
		sp->proc_interval = 3000; //few seconds
		sp->procFlags |= PROC_TARGET_SELF;
	}
	else if( strstr( sp->Name, "Earth Shield"))
		sp->proc_interval = 3000; //few seconds
	else if( strstr( sp->Name, "Shadowguard"))
		sp->proc_interval = 3000; //few seconds
	else if( strstr( sp->Name, "Poison Shield"))
		sp->proc_interval = 3000; //few seconds
	else if( strstr( sp->Name, "Infused Mushroom"))
		sp->proc_interval = 10000; //10 seconds
	else if( strstr( sp->Name, "Aviana's Purpose"))
		sp->proc_interval = 10000; //10 seconds
	//don't change to sp->NameHash since we are searching only a protion of the name
 	else if( strstr( sp->Name, "Crippling Poison"))
	{
		sp->SpellFamilyFlags.Flags |= 16384; //some of them do have the flags but i's hard to write down those some from 130 spells
		sp->c_is_flags |= SPELL_FLAG_IS_POISON;
	}
	else if( strstr( sp->Name, "Mind-Numbing Poison"))
	{
		sp->SpellFamilyFlags.Flags |= 32768; //some of them do have the flags but i's hard to write down those some from 130 spells
		sp->c_is_flags |= SPELL_FLAG_IS_POISON;
	}
	else if( strstr( sp->Name, "Instant Poison"))
	{
		sp->SpellFamilyFlags.Flags |= 8192; //some of them do have the flags but i's hard to write down those some from 130 spells
		sp->c_is_flags |= SPELL_FLAG_IS_POISON;
	}
	else if( strstr( sp->Name, "Deadly Poison"))
	{
		sp->SpellFamilyFlags.Flags |= 65536; //some of them do have the flags but i's hard to write down those some from 130 spells
		sp->c_is_flags |= SPELL_FLAG_IS_POISON;
	}
	else if( strstr( sp->Name, "Wound Poison"))
	{
		sp->SpellFamilyFlags.Flags |= 268435456; //some of them do have the flags but i's hard to write down those some from 130 spells
		sp->c_is_flags |= SPELL_FLAG_IS_POISON;
	}
	else if( strstr( sp->Name, "Scorpid Poison") )
	{
		// groups?
		sp->c_is_flags |= SPELL_FLAG_IS_POISON;
	}

	//warlock - shadow bolt
	if( sp->NameHash == SPELL_HASH_SHADOW_BOLT )
		sp->SpellFamilyFlags.Flags |= 1; //some of them do have the flags but i's hard to write down those some from 130 spells

	//mage Ice Floes affects these spells : Cone of Cold,Cold Snap,Ice Barrier,Ice Block
	if( sp->NameHash == SPELL_HASH_CONE_OF_COLD || sp->NameHash == SPELL_HASH_COLD_SNAP || sp->NameHash == SPELL_HASH_ICE_BARRIER || sp->NameHash == SPELL_HASH_ICE_BLOCK )
		sp->EffectSpellGroupRelation[0] = 0x00200000;

	//Set Silencing spells mech.
			// Set default mechanics if we don't already have one
	if( !sp->MechanicsType )
	{
		//Set Silencing spells mechanic.
		if( sp->EffectApplyAuraName[0] == 27 ||
			sp->EffectApplyAuraName[1] == 27 ||
			sp->EffectApplyAuraName[2] == 27 )
			sp->MechanicsType = MECHANIC_SILENCED;

		//Set Stunning spells mechanic.
		if( sp->EffectApplyAuraName[0] == 12 ||
			sp->EffectApplyAuraName[1] == 12 ||
			sp->EffectApplyAuraName[2] == 12 )
			sp->MechanicsType = MECHANIC_STUNNED;

		//Set Fearing spells mechanic
		if( sp->EffectApplyAuraName[0] == 7 ||
			sp->EffectApplyAuraName[1] == 7 ||
			sp->EffectApplyAuraName[2] == 7 )
			sp->MechanicsType = MECHANIC_FLEEING;
	}

	// Sap, Gouge, Blehhhh
	if( sp->NameHash == SPELL_HASH_GOUGE ||
		sp->NameHash == SPELL_HASH_SAP ||
		sp->NameHash == SPELL_HASH_REPENTANCE ||
		sp->NameHash == SPELL_HASH_MAIM ||
		sp->NameHash == SPELL_HASH_GOBLIN_ROCKET_HELMET ||
		sp->NameHash == SPELL_HASH_RECKLESS_CHARGE)
		sp->MechanicsType = MECHANIC_INCAPACIPATED;

	if( sp->proc_interval != 0 )
		sp->procFlags |= PROC_REMOVEONUSE;

	// Seal of Command - Proc Chance
	if( sp->NameHash == SPELL_HASH_SEAL_OF_COMMAND )
	{
		sp->procChance = 25;
		sp->School = SCHOOL_HOLY; //the procspells of the original seal of command have fizical school instead of holy
		sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC; //heh, crazy spell uses melee/ranged/magic dmg type for 1 spell. Now which one is correct ?
	}

	//Seal of Jusice - Proc Chance
	if( sp->NameHash == SPELL_HASH_SEAL_OF_JUSTICE )
		sp->procChance = 25;

	/* Decapitate */
	if( sp->NameHash == SPELL_HASH_DECAPITATE )
		sp->procChance = 30;

	if( sp->NameHash == SPELL_HASH_DIVINE_SHIELD || sp->NameHash == SPELL_HASH_DIVINE_PROTECTION || sp->NameHash == SPELL_HASH_BLESSING_OF_PROTECTION )
		sp->MechanicsType = 25;

	/* hackfix for this - FIX ME LATER - Burlex */
	if( sp->NameHash == SPELL_HASH_SEAL_FATE )
		sp->procFlags = 0;

	if(
		((sp->Attributes & ATTRIBUTES_TRIGGER_COOLDOWN) && (sp->AttributesEx & ATTRIBUTESEX_NOT_BREAK_STEALTH)) //rogue cold blood
		|| ((sp->Attributes & ATTRIBUTES_TRIGGER_COOLDOWN) && (!sp->AttributesEx || sp->AttributesEx & ATTRIBUTESEX_REMAIN_OOC))
		)
	{
		sp->c_is_flags |= SPELL_FLAG_IS_REQUIRECOOLDOWNUPDATE;
	}

	///	SPELLS CAN CRIT ///
	sp->spell_can_crit = true; // - except in special cases noted in this section

	//////////////////////////////////////////////////////
	// CLASS-SPECIFIC SPELL FIXES						//
	//////////////////////////////////////////////////////

	/* Note: when applying spell hackfixes, please follow a template */

	//////////////////////////////////////////
	// WARRIOR								//
	//////////////////////////////////////////

	// Insert warrior spell fixes here

	//////////////////////////////////////////
	// PALADIN								//
	//////////////////////////////////////////

	// Insert paladin spell fixes here
			
	// Seal of Righteousness - cannot crit
	if( sp->NameHash == SPELL_HASH_SEAL_OF_RIGHTEOUSNESS )
		sp->spell_can_crit = false;

	//////////////////////////////////////////
	// HUNTER								//
	//////////////////////////////////////////

	// Insert hunter spell fixes here
	//Hunter - Go for the Throat
	if(sp->Id == 34950)
		sp->procFlags = PROC_ON_RANGED_CRIT_ATTACK;

	if(sp->Id == 34954)
		sp->procFlags = PROC_ON_RANGED_CRIT_ATTACK;

	if(sp->Id == 34952)
		sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;

	if(sp->Id == 34953)
		sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;

	// come to think of it... anything *castable* requiring a ranged weapon is a ranged spell -.-
	// Note that talents etc also come under this, however it does not matter
	// if they get flagged as ranged spells because is_ranged_spell is only used for
	// differentiating between resistable and physically avoidable spells.
	if( sp->EquippedItemClass == 2 && sp->EquippedItemSubClass & 262156 ) // 4 + 8 + 262144 ( becomes item classes 2, 3 and 18 which correspond to bow, gun and crossbow respectively)
	{
		sp->is_ranged_spell = true;
	}

	//////////////////////////////////////////
	// ROGUE								//
	//////////////////////////////////////////

	// Insert rogue spell fixes here

	//////////////////////////////////////////
	// PRIEST								//
	//////////////////////////////////////////

	// Insert priest spell fixes here

	//////////////////////////////////////////
	// SHAMAN								//
	//////////////////////////////////////////

	// Insert shaman spell fixes here

	// Lightning Shield - cannot crit
	if( sp->NameHash == SPELL_HASH_LIGHTNING_SHIELD ) // not a mistake, the correct proc spell for lightning shield is also called lightning shield
		sp->spell_can_crit = false;

	//////////////////////////////////////////
	// MAGE								//
	//////////////////////////////////////////

	// Insert mage spell fixes here

	// Winter's chill
	if(sp->Id == 12579)
		sp->c_is_flags |= SPELL_FLAG_CANNOT_PROC_ON_SELF;


	//////////////////////////////////////////
	// WARLOCK								//
	//////////////////////////////////////////

	// Insert warlock spell fixes here

	//////////////////////////////////////////
	// DRUID								//
	//////////////////////////////////////////

	// Insert druid spell fixes here

}
