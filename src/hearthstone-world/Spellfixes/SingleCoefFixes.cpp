/***
 * Demonstrike Core
 */

#include "StdAfx.h"

void ApplyCoeffSpellFixes(SpellEntry *sp)
{
	switch(sp->Id)
	{
	case 116: // Frostbolt
		{
			sp->SP_coef_override = float(0.814300f);
		}break;
	case 120: // Cone of Cold
		{
			sp->SP_coef_override = float(0.135700f);
		}break;
	case 133: // Fireball
		{
			sp->SP_coef_override = float(1.000000f);
		}break;
	case 172: // Corruption
		{
			sp->SPOT_coef_override = float(0.156000f);
		}break;
	case 331: // Healing Wave
		{
			sp->SP_coef_override = float(0.857100f);
		}break;
	case 339: // Entangling Roots
		{
			sp->SPOT_coef_override = float(0.100000f);
		}break;
	case 348: // Immolate
		{
			sp->SP_coef_override = float(0.200000f);
			sp->SPOT_coef_override = float(0.200000f);
		}break;
	case 403: // Lightning Bolt
		{
			sp->SP_coef_override = float(0.714300f);
		}break;
	case 585: // Smite
		{
			sp->SP_coef_override = float(0.714000f);
		}break;
	case 603: // Curse of Doom
		{
			sp->SPOT_coef_override = float(2.000000f);
		}break;
	case 686: // Shadow Bolt
		{
			sp->SP_coef_override = float(0.857100f);
		}break;
	case 703: // Garrote
		{
			sp->APOT_coef_override = float(0.030000f);
		}break;
	case 755: // Health Funnel
		{
			sp->SPOT_coef_override = float(0.285700f);
		}break;
	case 774: // Rejuvenation
		{
			sp->SPOT_coef_override = float(0.160000f);
		}break;
	case 879: // Exorcism
		{
			sp->AP_coef_override = float(0.150000f);
			sp->SP_coef_override = float(0.150000f);
		}break;
	case 974: // Earth Shield
		{
			sp->SP_coef_override = float(0.285700f);
		}break;
	case 980: // Curse of Agony
		{
			sp->SPOT_coef_override = float(0.100000f);
		}break;
	case 1120: // Drain Soul
		{
			sp->SPOT_coef_override = float(0.428600f);
		}break;
	case 1454: // Life Tap
		{
			sp->SP_coef_override = float(0.800000f);
		}break;
	case 1463: // Mana Shield
		{
			sp->SP_coef_override = float(0.805300f);
		}break;
	case 1495: // Mongoose Bite
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 1822: // Rake
		{
			sp->APOT_coef_override = float(0.020000f);
		}break;
	case 1949: // Hellfire
		{
			sp->SPOT_coef_override = float(0.094600f);
		}break;
	case 1978: // Serpent Sting
		{
			sp->APOT_coef_override = float(0.020000f);
		}break;
	case 2120: // Flamestrike
		{
			sp->SP_coef_override = float(0.235700f);
			sp->SPOT_coef_override = float(0.122000f);
		}break;
	case 2812: // Holy Wrath
		{
			sp->AP_coef_override = float(0.070000f);
			sp->SP_coef_override = float(0.070000f);
		}break;
	case 3044: // Arcane Shot
		{
			sp->AP_coef_override = float(0.150000f);
		}break;
	case 3606: // Attack
		{
			sp->SP_coef_override = float(0.166700f);
		}break;
	case 5176: // Wrath
		{
			sp->SP_coef_override = float(0.571400f);
		}break;
	case 5185: // Healing Touch
		{
			sp->SP_coef_override = float(1.000000f);
		}break;
	case 5570: // Insect Swarm
		{
			sp->SPOT_coef_override = float(0.127000f);
		}break;
	case 5672: // Healing Stream
		{
			sp->SPOT_coef_override = float(0.045000f);
		}break;
	case 5857: // Hellfire Effect
		{
			sp->SP_coef_override = float(0.142800f);
		}break;
	case 6229: // Shadow Ward
		{
			sp->SP_coef_override = float(0.300000f);
		}break;
	case 6350: // Attack
		{
			sp->SP_coef_override = float(0.166700f);
		}break;
	case 6351: // Attack
		{
			sp->SP_coef_override = float(0.166700f);
		}break;
	case 6352: // Attack
		{
			sp->SP_coef_override = float(0.166700f);
		}break;
	case 6353: // Soul Fire
		{
			sp->SP_coef_override = float(1.150000f);
		}break;
	case 7268: // Arcane Missile
		{
			sp->SP_coef_override = float(0.285700f);
		}break;
	case 7269: // Arcane Missiles
		{
			sp->SP_coef_override = float(0.285700f);
		}break;
	case 7270: // Arcane Missiles
		{
			sp->SP_coef_override = float(0.285700f);
		}break;
	case 8026: // Flametongue Weapon Proc
		{
			sp->SP_coef_override = float(0.100000f);
		}break;
	case 8028: // Flametongue Weapon Proc
		{
			sp->SP_coef_override = float(0.100000f);
		}break;
	case 8029: // Flametongue Weapon Proc
		{
			sp->SP_coef_override = float(0.100000f);
		}break;
	case 8034: // Frostbrand Attack
		{
			sp->SP_coef_override = float(0.100000f);
		}break;
	case 8042: // Earth Shock
		{
			sp->SP_coef_override = float(0.385800f);
		}break;
	case 8050: // Flame Shock
		{
			sp->SP_coef_override = float(0.214200f);
			sp->SPOT_coef_override = float(0.100000f);
		}break;
	case 8056: // Frost Shock
		{
			sp->SP_coef_override = float(0.385800f);
		}break;
	case 8188: // Magma Totem Passive
		{
			sp->SP_coef_override = float(0.100000f);
		}break;
	case 8418: // Arcane Missiles
		{
			sp->SP_coef_override = float(0.285700f);
		}break;
	case 8419: // Arcane Missiles
		{
			sp->SP_coef_override = float(0.285700f);
		}break;
	case 8443: // Fire Nova
		{
			sp->SP_coef_override = float(0.214200f);
		}break;
	case 8504: // Fire Nova
		{
			sp->SP_coef_override = float(0.214200f);
		}break;
	case 8505: // Fire Nova
		{
			sp->SP_coef_override = float(0.214200f);
		}break;
	case 8921: // Moonfire
		{
			sp->SP_coef_override = float(0.151500f);
			sp->SPOT_coef_override = float(0.130000f);
		}break;
	case 8936: // Regrowth
		{
			sp->SP_coef_override = float(0.300000f);
			sp->SPOT_coef_override = float(0.100000f);
		}break;
	case 9007: // Pounce Bleed
		{
			sp->APOT_coef_override = float(0.030000f);
		}break;
	case 10273: // Arcane Missiles
		{
			sp->SP_coef_override = float(0.285700f);
		}break;
	case 10274: // Arcane Missiles
		{
			sp->SP_coef_override = float(0.285700f);
		}break;
	case 10435: // Attack
		{
			sp->SP_coef_override = float(0.166700f);
		}break;
	case 10436: // Attack
		{
			sp->SP_coef_override = float(0.166700f);
		}break;
	case 10445: // Flametongue Weapon Proc
		{
			sp->SP_coef_override = float(0.100000f);
		}break;
	case 10582: // Magma Totem Passive
		{
			sp->SP_coef_override = float(0.100000f);
		}break;
	case 10583: // Magma Totem Passive
		{
			sp->SP_coef_override = float(0.100000f);
		}break;
	case 10584: // Magma Totem Passive
		{
			sp->SP_coef_override = float(0.100000f);
		}break;
	case 11113: // Blast Wave
		{
			sp->SP_coef_override = float(0.135700f);
		}break;
	case 11310: // Fire Nova
		{
			sp->SP_coef_override = float(0.214200f);
		}break;
	case 11311: // Fire Nova
		{
			sp->SP_coef_override = float(0.214200f);
		}break;
	case 11681: // Hellfire Effect
		{
			sp->SP_coef_override = float(0.142800f);
		}break;
	case 11682: // Hellfire Effect
		{
			sp->SP_coef_override = float(0.142800f);
		}break;
	case 13797: // Immolation Trap Effect
		{
			sp->APOT_coef_override = float(0.020000f);
		}break;
	case 13812: // Explosive Trap Effect
		{
			sp->AP_coef_override = float(0.100000f);
			sp->APOT_coef_override = float(0.100000f);
		}break;
	case 14914: // Holy Fire
		{
			sp->SP_coef_override = float(0.571100f);
			sp->SPOT_coef_override = float(0.024000f);
		}break;
	case 15237: // Holy Nova
		{
			sp->SP_coef_override = float(0.160600f);
		}break;
	case 15407: // Mind Flay
		{
			sp->SPOT_coef_override = float(0.190000f);
		}break;
	case 16343: // Flametongue Weapon Proc
		{
			sp->SP_coef_override = float(0.100000f);
		}break;
	case 16344: // Flametongue Weapon Proc
		{
			sp->SP_coef_override = float(0.100000f);
		}break;
	case 18220: // Dark Pact
		{
			sp->SP_coef_override = float(0.960000f);
		}break;
	case 20167: // Seal of Light
		{
			sp->AP_coef_override = float(0.160000f);
			sp->SP_coef_override = float(0.250000f);
		}break;
	case 20424: // Seal of Command
		{
			sp->AP_coef_override = float(0.160000f);
			sp->SP_coef_override = float(0.250000f);
		}break;
	case 20925: // Holy Shield
		{
			sp->AP_coef_override = float(0.056000f);
			sp->SP_coef_override = float(0.090000f);
		}break;
	case 23455: // Holy Nova
		{
			sp->SP_coef_override = float(0.160600f);
		}break;
	case 23458: // Holy Nova
		{
			sp->SP_coef_override = float(0.160600f);
		}break;
	case 23459: // Holy Nova
		{
			sp->SP_coef_override = float(0.160600f);
		}break;
	case 24275: // Hammer of Wrath
		{
			sp->AP_coef_override = float(0.150000f);
			sp->SP_coef_override = float(0.150000f);
		}break;
	case 25329: // Holy Nova
		{
			sp->SP_coef_override = float(0.160600f);
		}break;
	case 25346: // Arcane Missiles
		{
			sp->SP_coef_override = float(0.285700f);
		}break;
	case 25488: // Flametongue Weapon Proc
		{
			sp->SP_coef_override = float(0.100000f);
		}break;
	case 25530: // Attack
		{
			sp->SP_coef_override = float(0.166700f);
		}break;
	case 25538: // Fire Nova
		{
			sp->SP_coef_override = float(0.214200f);
		}break;
	case 25539: // Fire Nova
		{
			sp->SP_coef_override = float(0.214200f);
		}break;
	case 25551: // Magma Totem Passive
		{
			sp->SP_coef_override = float(0.100000f);
		}break;
	case 25742: // Seal of Righteousness
		{
			sp->AP_coef_override = float(0.039000f);
			sp->SP_coef_override = float(0.070000f);
		}break;
	case 26363: // Lightning Shield
		{
			sp->SP_coef_override = float(0.330000f);
		}break;
	case 26364: // Lightning Shield
		{
			sp->SP_coef_override = float(0.330000f);
		}break;
	case 26365: // Lightning Shield
		{
			sp->SP_coef_override = float(0.330000f);
		}break;
	case 26366: // Lightning Shield
		{
			sp->SP_coef_override = float(0.330000f);
		}break;
	case 26367: // Lightning Shield
		{
			sp->SP_coef_override = float(0.330000f);
		}break;
	case 26369: // Lightning Shield
		{
			sp->SP_coef_override = float(0.330000f);
		}break;
	case 26370: // Lightning Shield
		{
			sp->SP_coef_override = float(0.330000f);
		}break;
	case 26371: // Lightning Shield
		{
			sp->SP_coef_override = float(0.330000f);
		}break;
	case 26372: // Lightning Shield
		{
			sp->SP_coef_override = float(0.330000f);
		}break;
	case 26573: // Consecration
		{
			sp->APOT_coef_override = float(0.040000f);
			sp->SPOT_coef_override = float(0.040000f);
		}break;
	case 27076: // Arcane Missiles
		{
			sp->SP_coef_override = float(0.285700f);
		}break;
	case 27214: // Hellfire Effect
		{
			sp->SP_coef_override = float(0.142800f);
		}break;
	case 27243: // Seed of Corruption
		{
			sp->SP_coef_override = float(0.220000f);
			sp->SPOT_coef_override = float(0.250000f);
		}break;
	case 27803: // Holy Nova
		{
			sp->SP_coef_override = float(0.160600f);
		}break;
	case 27804: // Holy Nova
		{
			sp->SP_coef_override = float(0.160600f);
		}break;
	case 27805: // Holy Nova
		{
			sp->SP_coef_override = float(0.160600f);
		}break;
	case 30108: // Unstable Affliction
		{
			sp->SPOT_coef_override = float(0.240000f);
		}break;
	case 30455: // Ice Lance
		{
			sp->SP_coef_override = float(0.142900f);
		}break;
	case 31117: // Unstable Affliction
		{
			sp->SP_coef_override = float(1.800000f);
		}break;
	case 31661: // Dragon's Breath
		{
			sp->SP_coef_override = float(0.135700f);
		}break;
	case 31803: // Holy Vengeance
		{
			sp->APOT_coef_override = float(0.150000f);
			sp->SPOT_coef_override = float(0.013000f);
		}break;
	case 31893: // Seal of Blood
		{
			sp->AP_coef_override = float(0.160000f);
			sp->SP_coef_override = float(0.250000f);
		}break;
	case 31935: // Avenger's Shield
		{
			sp->AP_coef_override = float(0.070000f);
			sp->SP_coef_override = float(0.070000f);
		}break;
	case 32221: // Seal of Blood
		{
			sp->AP_coef_override = float(0.160000f);
			sp->SP_coef_override = float(0.250000f);
		}break;
	case 33745: // Lacerate
		{
			sp->AP_coef_override = float(0.010000f);
			sp->APOT_coef_override = float(0.010000f);
		}break;
	case 33763: // Lifebloom
		{
			sp->SP_coef_override = float(0.342900f);
			sp->SPOT_coef_override = float(0.074200f);
		}break;
	case 34433: // Shadowfiend
		{
			sp->SP_coef_override = float(0.650000f);
		}break;
	case 34914: // Vampiric Touch
		{
			sp->SPOT_coef_override = float(0.400000f);
		}break;
	case 38700: // Arcane Missiles
		{
			sp->SP_coef_override = float(0.285700f);
		}break;
	case 38703: // Arcane Missiles
		{
			sp->SP_coef_override = float(0.285700f);
		}break;
	case 42198: // Blizzard
		{
			sp->SP_coef_override = float(0.095200f);
		}break;
	case 42208: // Blizzard
		{
			sp->SP_coef_override = float(0.095200f);
		}break;
	case 42209: // Blizzard
		{
			sp->SP_coef_override = float(0.095200f);
		}break;
	case 42210: // Blizzard
		{
			sp->SP_coef_override = float(0.095200f);
		}break;
	case 42211: // Blizzard
		{
			sp->SP_coef_override = float(0.095200f);
		}break;
	case 42212: // Blizzard
		{
			sp->SP_coef_override = float(0.095200f);
		}break;
	case 42213: // Blizzard
		{
			sp->SP_coef_override = float(0.095200f);
		}break;
	case 42218: // Rain of Fire
		{
			sp->SP_coef_override = float(0.952000f);
		}break;
	case 42223: // Rain of Fire
		{
			sp->SP_coef_override = float(0.952000f);
		}break;
	case 42224: // Rain of Fire
		{
			sp->SP_coef_override = float(0.952000f);
		}break;
	case 42225: // Rain of Fire
		{
			sp->SP_coef_override = float(0.952000f);
		}break;
	case 42226: // Rain of Fire
		{
			sp->SP_coef_override = float(0.952000f);
		}break;
	case 42231: // Hurricane
		{
			sp->SP_coef_override = float(0.128980f);
		}break;
	case 44203: // Tranquility
		{
			sp->SP_coef_override = float(0.182500f);
		}break;
	case 44459: // Living Bomb
		{
			sp->SP_coef_override = float(0.400000f);
		}break;
	case 44461: // Living Bomb
		{
			sp->SP_coef_override = float(0.400000f);
		}break;
	}
}
