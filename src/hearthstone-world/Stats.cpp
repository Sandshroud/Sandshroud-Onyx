/***
 * Demonstrike Core
 */

#include "StdAfx.h"

uint32 getConColor(uint16 AttackerLvl, uint16 VictimLvl)
{

#define PLAYER_LEVEL_CAP 70
	const uint32 grayLevel[PLAYER_LEVEL_CAP+1] = {0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,13,14,15,16,17,18,19,20,21,22,22,23,24,25,26,27,28,29,30,31,31,32,33,34,35,35,36,37,38,39,39,40,41,42,43,43,44,45,46,47,47,48,49,50,51,51,52,53,54,55,55};
	if(AttackerLvl + 5 <= VictimLvl)
	{
		if(AttackerLvl + 10 <= VictimLvl)
		{
			return 5;
		}
		return 4;
	}
	else
	{
		switch(VictimLvl - AttackerLvl)
		{
		case 4:
		case 3:
			return 3;
			break;
		case 2:
		case 1:
		case 0:
		case -1:
		case -2:
			return 2;
			break;
		default:
			// More adv formula for grey/green lvls:
			if(AttackerLvl <= 6)
			{
				return 1; //All others are green.
			}
			else
			{
				if(AttackerLvl > PLAYER_LEVEL_CAP)
					return 1;//gm
				if(AttackerLvl<PLAYER_LEVEL_CAP && VictimLvl <= grayLevel[AttackerLvl])
					return 0;
				else
					return 1;
			}
		}
	}
#undef PLAYER_LEVEL_CAP
}

uint32 CalculateXpToGive(Unit* pVictim, Unit* pAttacker)
{
	if(pVictim->IsPlayer())
		return 0;

	if( TO_CREATURE(pVictim)->IsTotem())
		return 0;

	CreatureInfo *victimI;
	victimI = TO_CREATURE(pVictim)->GetCreatureInfo();

	if(victimI)
		if(victimI->Type == CRITTER)
			return 0;
	uint32 VictimLvl = pVictim->getLevel();
	uint32 AttackerLvl = pAttacker->getLevel();

	if( pAttacker->IsPet() && TO_PET(pAttacker)->GetPetOwner() )
	{
		// based on: http://www.wowwiki.com/Talk:Formulas:Mob_XP#Hunter.27s_pet_XP (2008/01/12)
		uint32 ownerLvl = TO_PET( pAttacker )->GetPetOwner()->getLevel();
		VictimLvl += ownerLvl - AttackerLvl;
		AttackerLvl = ownerLvl;
	}
	else if( (int32)VictimLvl - (int32)AttackerLvl > 10 ) //not wowwikilike but more balanced
		return 0;

	// Partha: this screws things up for pets and groups

	float zd = 5;
	float g = 5;

	// get zero diff
	// get grey diff

	if(AttackerLvl >= 100)
	{
		zd = 23;
		g = 19;
	}
	else if(AttackerLvl >= 90)
	{
		zd = 22;
		g = 18;
	}
	else if(AttackerLvl >= 80)
	{
		zd = 21;
		g = 17;
	}
	else if(AttackerLvl >= 75)
	{
		zd = 20;
		g = 16;
	}
	else if(AttackerLvl >= 70)
	{
		zd = 19;
		g = 15;
	}
	else if(AttackerLvl >= 65)
	{
		zd = 18;
		g = 14;
	}
	else if(AttackerLvl >= 60)
	{
		zd = 17;
		g = 13;
	}
	else if(AttackerLvl >= 55)
	{
		zd = 16;
		g = 12;
	}
	else if(AttackerLvl >= 50)
	{
		zd = 15;
		g = 11;
	}
	else if(AttackerLvl >= 45)
	{
		zd = 14;
		g = 10;
	}
	else if(AttackerLvl >= 40)
	{
		zd = 13;
		g = 9;
	}
	else if(AttackerLvl >= 30)
	{
		zd = 12;
		g = 8;
	}
	else if(AttackerLvl >= 20)
	{
		zd = 11;
		g = 7;
	}
	else if(AttackerLvl >= 16)
	{
		zd = 9;
		g = 6;
	}
	else if(AttackerLvl >= 12)
	{
		zd = 8;
		g = 6;
	}
	else if(AttackerLvl >= 10)
	{
		zd = 7;
		g = 6;
	}
	else if(AttackerLvl >= 8)
	{
		zd = 6;
		g = 5;
	}
	else
	{
		zd = 5;
		g = 5;
	}

	float xp = 0.0f;
	float fVictim = float(VictimLvl);
	float fAttacker = float(AttackerLvl);

	if(VictimLvl == AttackerLvl)
		xp = float( ((fVictim * 5.0f) + 45.0f) );
	else if(VictimLvl > AttackerLvl)
	{
		float j = 1.0f + (0.25f * (fVictim - fAttacker));
		xp = float( ((AttackerLvl * 5.0f) + 45.0f) * j );
	}
	else
	{
		if((AttackerLvl - VictimLvl) < g)
		{
			float j = (1.0f - float((fAttacker - fVictim) / zd));
			xp = (AttackerLvl * 5.0f + 45.0f) * j;
		}
	}

	// multiply by global XP rate
	if(xp == 0.0f)
		return 0;

	xp *= sWorld.getRate(RATE_XP);

	// elite boss multiplier
	if(victimI)
	{
		switch(victimI->Rank)
		{
		case 0: // normal mob
			break;
		case 1: // elite
			xp *= 2.0f;
			break;
		case 2: // rare elite
			xp *= 2.0f;
			break;
		case 3: // world boss
			xp *= 2.5f;
			break;
		default:	// rare or higher
			//			xp *= 7.0f;
			break;
		}
	}
	if( xp < 0 )//probably caused incredible wrong exp
		xp = 0;

	return (uint32)xp;
}

/*
Author: pionere

Calculate the stat increase. Using 3rd grade polynome.

Parameter level The level the character reached.
Parameter a3 The factor for x^3.
Parameter a2 The factor for x^2.
Parameter a1 The factor for x^1.
Parameter a0 The constant factor for the polynome.
Return stat gain
*/
uint32 CalculateStat(uint16 level, float a3, float a2, float a1, float a0)
{
	int result1 = (int)(a3*level*level*level +
		a2*level*level +
		a1*level +
		a0);

	int result2 = (int)(a3*(level-1)*(level-1)*(level-1) +
		a2*(level-1)*(level-1) +
		a1*(level-1) +
		a0);

	//get difference
	int diff = result1-result2;
	return diff;
}

#define StatCalculationMacroAction(_stat, stat1float1, stat1float2, stat1float3, stat1float4, stat2float1, stat2float2, stat2float3, stat2float4,\
	stat3float1, stat3float2, stat3float3, stat3float4, stat4float1, stat4float2, stat4float3, stat4float4, stat5float1, stat5float2, stat5float3, stat5float4)\
	switch(_stat)\
	{\
	case STAT_STRENGTH:	{ gain = CalculateStat(level, stat1float1, stat1float2, stat1float3, stat1float4); } break;\
	case STAT_AGILITY:	{ gain = CalculateStat(level, stat2float1, stat2float2, stat2float3, stat2float4); } break;\
	case STAT_STAMINA:	{ gain = CalculateStat(level, stat3float1, stat3float2, stat3float3, stat3float4); } break;\
	case STAT_INTELLECT:{ gain = CalculateStat(level, stat4float1, stat4float2, stat4float3, stat4float4); } break;\
	case STAT_SPIRIT:	{ gain = CalculateStat(level, stat5float1, stat5float2, stat5float3, stat5float4); } break;\
	}

uint32 CalcStatForLevel(uint16 level, uint8 playerrace, uint8 playerclass, uint8 Stat)
{
	uint32 gain = 0;
	switch(playerclass)
	{
	case WARRIOR:
		{
			StatCalculationMacroAction(Stat,
				0.000039f, 0.006902f, 1.080040f, -1.051701f,
				0.000022f, 0.004600f, 0.655333f, -0.600356f,
				0.000059f, 0.004044f, 1.040000f, -1.488504f,
				0.000002f, 0.001003f, 0.100890f, -0.076055f,
				0.000006f, 0.002031f, 0.278360f, -0.340077f);
		}break;
	case PALADIN:
		{
			StatCalculationMacroAction(Stat,
				0.000037f, 0.005455f, 0.940039f, -1.000090f,
				0.000020f, 0.003007f, 0.505215f, -0.500642f,
				0.000038f, 0.005145f, 0.871006f, -0.832029f,
				0.000023f, 0.003345f, 0.560050f, -0.562058f,
				0.000032f, 0.003025f, 0.615890f, -0.640307f);
		}break;
	case HUNTER:
		{
			StatCalculationMacroAction(Stat,
				0.000022f, 0.001800f, 0.407867f, -0.550889f,
				0.000040f, 0.007416f, 1.125108f, -1.003045f,
				0.000031f, 0.004480f, 0.780040f, -0.800471f,
				0.000020f, 0.003007f, 0.505215f, -0.500642f,
				0.000017f, 0.003803f, 0.536846f, -0.490026f);
		}break;
	case ROGUE:
		{
			StatCalculationMacroAction(Stat,
				0.000025f, 0.004170f, 0.654096f, -0.601491f,
				0.000038f, 0.007834f, 1.191028f, -1.203940f,
				0.000032f, 0.003025f, 0.615890f, -0.640307f,
				0.000008f, 0.001001f, 0.163190f, -0.064280f,
				0.000024f, 0.000981f, 0.364935f, -0.570900f);
		}break;
	case PRIEST:
		{
			StatCalculationMacroAction(Stat,
				0.000008f, 0.001001f, 0.163190f, -0.064280f,
				0.000022f, 0.000022f, 0.260756f, -0.494000f,
				0.000024f, 0.000981f, 0.364935f, -0.570900f,
				0.000039f, 0.006981f, 1.090090f, -1.006070f,
				0.000040f, 0.007416f, 1.125108f, -1.003045f);
		}break;
	case SHAMAN:
		{
			StatCalculationMacroAction(Stat,
				0.000035f, 0.003641f, 0.734310f, -0.800626f,
				0.000022f, 0.001800f, 0.407867f, -0.550889f,
				0.000020f, 0.006030f, 0.809570f, -0.809220f,
				0.000031f, 0.004480f, 0.780040f, -0.800471f,
				0.000038f, 0.005145f, 0.871006f, -0.832029f);
		}break;
	case MAGE:
		{
			StatCalculationMacroAction(Stat,
				0.000002f, 0.001003f, 0.100890f, -0.076055f,
				0.000008f, 0.001001f, 0.163190f, -0.064280f,
				0.000006f, 0.002031f, 0.278360f, -0.340077f,
				0.000040f, 0.007416f, 1.125108f, -1.003045f,
				0.000039f, 0.006981f, 1.090090f, -1.006070f);
		}break;
	case WARLOCK:
		{
			StatCalculationMacroAction(Stat,
				0.000006f, 0.002031f, 0.278360f, -0.340077f,
				0.000024f, 0.000981f, 0.364935f, -0.570900f,
				0.000021f, 0.003009f, 0.486493f, -0.400003f,
				0.000059f, 0.004044f, 1.040000f, -1.488504f,
				0.000040f, 0.006404f, 1.038791f, -1.039076f);
		}break;
	case DRUID:
		{
			StatCalculationMacroAction(Stat,
				0.000021f, 0.003009f, 0.486493f, -0.400003f,
				0.000041f, 0.000440f, 0.512076f, -1.000317f,
				0.000023f, 0.003345f, 0.560050f, -0.562058f,
				0.000038f, 0.005145f, 0.871006f, -0.832029f,
				0.000059f, 0.004044f, 1.040000f, -1.488504f);
		}break;
	}

	return gain;
}

uint32 CalculateDamage( Unit* pAttacker, Unit* pVictim, uint32 weapon_damage_type, SpellEntry* ability ) // spellid is used only for 2-3 spells, that have AP bonus
{
	//TODO: Some awesome formula to determine how much damage to deal
	//consider this is melee damage
	// weapon_damage_type: 0 = melee, 1 = offhand(dualwield), 2 = ranged

	// Attack Power increases your base damage-per-second (DPS) by 1 for every 14 attack power.
	// (c) wowwiki

	//type of this UNIT_FIELD_ATTACK_POWER_MODS is unknown, not even uint32 disabled for now.

	uint32 offset;
	Item* it = NULLITEM;

	if(pAttacker->disarmed && pAttacker->IsPlayer())
	{
		offset=UNIT_FIELD_MINDAMAGE;
		it = TO_PLAYER(pAttacker)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
	}
	else if( weapon_damage_type == MELEE )
		offset = UNIT_FIELD_MINDAMAGE;
	else if( weapon_damage_type == OFFHAND )
		offset = UNIT_FIELD_MINOFFHANDDAMAGE;
	else  // weapon_damage_type == RANGED
		offset = UNIT_FIELD_MINRANGEDDAMAGE;

	float min_damage = pAttacker->GetFloatValue(offset);
	float max_damage = pAttacker->GetFloatValue(offset+1);
	if(it)
	{
		min_damage -= it->GetProto()->Damage[0].Min;
		max_damage -= it->GetProto()->Damage[0].Max;
	}

	float ap = 0;
	float bonus;
	float wspeed;
	float appbonuspct = 1.0f;
	Item* BonusItem = NULLITEM;
	if( pAttacker->IsPlayer() && weapon_damage_type == MELEE )
	{
		BonusItem = TO_PLAYER(pAttacker)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
	}
	else if(pAttacker->IsPlayer() && weapon_damage_type == OFFHAND )
	{
		BonusItem = TO_PLAYER(pAttacker)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_OFFHAND);
	}
	else if(pAttacker->IsPlayer() && weapon_damage_type == RANGED )
	{
		BonusItem = TO_PLAYER(pAttacker)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED);
	}

	if( pAttacker->IsPlayer() && BonusItem )
	{
		appbonuspct = TO_PLAYER(pAttacker)->m_WeaponSubClassDamagePct[ BonusItem->GetProto()->SubClass  ];
	}

	if(offset == UNIT_FIELD_MINRANGEDDAMAGE)
	{
		//starting from base attack power then we apply mods on it
		//ap += pAttacker->GetRAP();
		ap += pVictim->RAPvModifier;

		if(!pVictim->IsPlayer() && TO_CREATURE(pVictim)->GetCreatureInfo())
		{
			uint32 creatType = TO_CREATURE(pVictim)->GetCreatureInfo()->Type;
			ap += (float)pAttacker->CreatureRangedAttackPowerMod[creatType];

			if(pAttacker->IsPlayer())
			{
				min_damage = (min_damage + TO_PLAYER(pAttacker)->IncreaseDamageByType[creatType]) * (1 + TO_PLAYER(pAttacker)->IncreaseDamageByTypePCT[creatType]);
				max_damage = (max_damage + TO_PLAYER(pAttacker)->IncreaseDamageByType[creatType]) * (1 + TO_PLAYER(pAttacker)->IncreaseDamageByTypePCT[creatType]);
			}
		}

		if(pAttacker->IsPlayer())
		{
			if(!pAttacker->disarmed)
			{
				Item* it = TO_PLAYER(pAttacker)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED);
				if(it)
				{
					wspeed = (float)it->GetProto()->Delay;
				}
				else
					wspeed = 2000;
			}
			else
				wspeed = (float)pAttacker->GetUInt32Value(UNIT_FIELD_RANGEDATTACKTIME);
		}
		else
		{
			wspeed = (float)pAttacker->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME);
		}

		//ranged weapon normalization.
		if(pAttacker->IsPlayer() && ability)
		{
			if(ability->Effect[0] == SPELL_EFFECT_DUMMYMELEE || ability->Effect[1] == SPELL_EFFECT_DUMMYMELEE || ability->Effect[2] == SPELL_EFFECT_DUMMYMELEE)
			{
				wspeed = 2800;
			}
		}

		//Weapon speed constant in feral forms
		if(pAttacker->IsPlayer())
		{
			if(TO_PLAYER(pAttacker)->IsInFeralForm())
			{
				uint8 ss = TO_PLAYER(pAttacker)->GetShapeShift();

				if(ss == FORM_CAT)
					wspeed = 1000.0;
				else if(ss == FORM_DIREBEAR || ss == FORM_BEAR)
					wspeed = 2500.0;
			}
		}

		bonus = (wspeed-pAttacker->GetUInt32Value(UNIT_FIELD_RANGEDATTACKTIME))/14000.0f*ap;
		min_damage += bonus;
		max_damage += bonus;
	}
	else
	{
		//MinD = AP(28AS-(WS/7))-MaxD
		//starting from base attack power then we apply mods on it
		//ap += pAttacker->GetAP();
		ap += pVictim->APvModifier;

		if(!pVictim->IsPlayer() && TO_CREATURE(pVictim)->GetCreatureInfo())
		{
			uint32 creatType = TO_CREATURE(pVictim)->GetCreatureInfo()->Type;
			ap += (float)pAttacker->CreatureAttackPowerMod[creatType];

			if(pAttacker->IsPlayer())
			{
				min_damage = (min_damage + TO_PLAYER(pAttacker)->IncreaseDamageByType[creatType]) * (1 + TO_PLAYER(pAttacker)->IncreaseDamageByTypePCT[creatType]);
				max_damage = (max_damage + TO_PLAYER(pAttacker)->IncreaseDamageByType[creatType]) * (1 + TO_PLAYER(pAttacker)->IncreaseDamageByTypePCT[creatType]);
			}
		}

		if(pAttacker->IsPlayer())
		{
			if(!pAttacker->disarmed)
			{
				Item* it = TO_PLAYER(pAttacker)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);

				if(it)
					wspeed = (float)it->GetProto()->Delay;
				else
					wspeed = 2000;
			}
			else
				wspeed = (float)pAttacker->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME);

			if(ability && ability->SpellFamilyFlags)
			{
				int32 apall = pAttacker->GetAP();
				/* this spell modifier doesn't exist. also need to clear up how the AP is used here
				int32 apb=0;
				SM_FIValue(pAttacker->SM[SMT_ATTACK_POWER_BONUS][1],&apb,ability->SpellFamilyFlags);

				if(apb)
					ap += apall*((float)apb/100);
				else*/
					ap = float(apall);
			}
		}
		else
		{
			wspeed = (float)pAttacker->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME);
		}

		//Normalized weapon damage checks.
		if(pAttacker->IsPlayer() && ability)
		{
			if(ability->Effect[0] == SPELL_EFFECT_DUMMYMELEE || ability->Effect[1] == SPELL_EFFECT_DUMMYMELEE || ability->Effect[2] == SPELL_EFFECT_DUMMYMELEE)
			{
				it = TO_PLAYER(pAttacker)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);

				if(it)
				{
					if(it->GetProto()->Class == ITEM_CLASS_WEAPON) //weapon
					{
						if(it->GetProto()->InventoryType == INVTYPE_2HWEAPON) wspeed = 3300;
						else if(it->GetProto()->SubClass == 15) wspeed = 1700;
						else wspeed = 2400;
					}
				}
			}
		}

		//Weapon speed constant in feral forms
		if(pAttacker->IsPlayer())
		{
			if(TO_PLAYER(pAttacker)->IsInFeralForm())
			{
				uint8 ss = TO_PLAYER(pAttacker)->GetShapeShift();

				if(ss == FORM_CAT)
					wspeed = 1000.0;
				else if(ss == FORM_DIREBEAR || ss == FORM_BEAR)
					wspeed = 2500.0;
			}
		}

		if (offset == UNIT_FIELD_MINDAMAGE)
			bonus = (wspeed-pAttacker->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME))/14000.0f*ap;
		else if( offset == UNIT_FIELD_MINOFFHANDDAMAGE )
			bonus = (wspeed-pAttacker->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME + 1)) / 14000.0f*ap;
		else
			bonus = (wspeed-pAttacker->GetUInt32Value(UNIT_FIELD_RANGEDATTACKTIME))/14000.0f*ap;
		min_damage += bonus;
		max_damage += bonus;
	}

	float diff = fabs(max_damage - min_damage);
	float result = min_damage;

	if(diff >= 1)
		result += float(RandomDouble(diff));

	if(result >= 0)
	{
		return FL2UINT(result * appbonuspct);
	}

	return 0;
}

bool isEven (int num)
{
	if ((num%2)==0)
	{
		return true;
	}

	return false;
}
