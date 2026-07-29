import { get_const_string } from '$lib/services/constStringService.svelte';
import {
	ArmorShellOptionsConstStrings,
	CellonOptionsConstStrings,
	ConstStringKey,
	WeaponShellOptionsConstStrings,
	ShellQualifierKeys
} from '$lib/types/constStringKeys';
import type { CellonOption, ShellEffect, ShellOptionData, EnrichedSearchResult } from '$lib/types/search';
import { WeaponShellOption, ArmorShellOption, RequiredClass, Element } from '$lib/types/enums';

export function formatString(template: string, ...args: (string | number)[]): string {
	const clean = template.replace(/<NEW_TYPE><\d+(,\d+)*>/g, '');
	let argIndex = 0;
	let result = '';
	for (let i = 0; i < clean.length; i++) {
		if (clean[i] === '%' && i + 1 < clean.length) {
			if (clean[i + 1] === 's' || clean[i + 1] === 'd') {
				result += String(args[argIndex++] ?? '%s');
				i++;
			} else if (clean[i + 1] === '%') {
				result += '%';
				i++;
			} else {
				result += clean[i];
			}
		} else {
			result += clean[i];
		}
	}
	return result;
}

export function formatCellonOptionString(cellonOption: CellonOption): string {
	const template = `${cellonOption.level}${get_const_string(ConstStringKey.Lv)} ${get_const_string(CellonOptionsConstStrings[cellonOption.vnum])}`;
	if (template.includes('%s')) {
		return formatString(template, cellonOption.value);
	}
	return `${template} ${cellonOption.value}`;
}

export const ShellGradeColor: Record<number, string> = {
	1: '#FFCC00',
	2: '#FFFF00',
	3: '#99FF66',
	4: '#CCFF99',
	5: '#CC9AFF',
	6: '#CC9AFF',
	7: '#CC9AFF',
	8: '#CC9AFF',
	9: '#FF7AB2',
	10: '#FF7AB2',
	11: '#FF7AB2',
	12: '#FF7AB2',
	13: '#FFCC00',
	14: '#FFFF00',
	15: '#99FF66',
	16: '#CCFF99',
	17: '#CC9AFF',
	18: '#CC9AFF',
	19: '#CC9AFF',
	20: '#CC9AFF',
	21: '#FF7AB2',
	22: '#FF7AB2',
	23: '#FF7AB2',
	24: '#FF7AB2',
};

const ShellGradeLabel: Record<number, string> = {
	1: 'C',
	2: 'B',
	3: 'A',
	4: 'S',
	5: 'C',
	6: 'B',
	7: 'A',
	8: 'S',
	9: 'C',
	10: 'B',
	11: 'A',
	12: 'S',
	13: 'C',
	14: 'B',
	15: 'A',
	16: 'S',
	17: 'C',
	18: 'B',
	19: 'A',
	20: 'S',
	21: 'C',
	22: 'B',
	23: 'A',
	24: 'S',
};

export function formatShellOptionString(shellOption: ShellOptionData): string {
	const isArmor = shellOption.grade > 12;
	const constStrKey = isArmor
		? ArmorShellOptionsConstStrings[shellOption.vnum]
		: WeaponShellOptionsConstStrings[shellOption.vnum];
	if (constStrKey === undefined) {
		return '';
	}
	let template = get_const_string(constStrKey);
	if (template.includes('%s')) {
		template = formatString(template, shellOption.value);
	}
	let result = '';
	const qualifierKey = isArmor ? undefined : ShellQualifierKeys[shellOption.vnum];
	if (qualifierKey !== undefined) {
		result += get_const_string(qualifierKey) + ' ';
	}
	const gradeLabel = ShellGradeLabel[shellOption.grade];
	if (gradeLabel) {
		result += gradeLabel + '-';
	}
	result += template;
	return result;
}

const WeaponShellUpgradeBaseValue: Partial<Record<number, number>> = {
	[WeaponShellOption.EnhancedDmg]: 80,
	[WeaponShellOption.IncreasedDmg]: 2,
	[WeaponShellOption.MinorBleeding]: 1,
	[WeaponShellOption.Bleeding]: 1,
	[WeaponShellOption.HeavyBleeding]: 1,
	[WeaponShellOption.Blackout]: 1,
	[WeaponShellOption.Freeze]: 1,
	[WeaponShellOption.DeadlyBlackout]: 1,
	[WeaponShellOption.IncreasedDmgToPlants]: 2,
	[WeaponShellOption.IncreasedDmgToAnimals]: 2,
	[WeaponShellOption.IncreasedDmgToMonsters]: 2,
	[WeaponShellOption.IncreasedDmgToUndeads]: 2,
	[WeaponShellOption.IncreasedDmgToKovolts]: 2,
	[WeaponShellOption.IncreasedDmgToMapBosses]: 2,
	[WeaponShellOption.IncreasedCritChance]: 1,
	[WeaponShellOption.IncreasedCritDmg]: 10,
	[WeaponShellOption.UndisturbedCasting]: 1000,
	[WeaponShellOption.IncreasedFireElement]: 80,
	[WeaponShellOption.IncreasedWaterElement]: 80,
	[WeaponShellOption.IncreasedLightElement]: 80,
	[WeaponShellOption.IncreasedShadowElement]: 80,
	[WeaponShellOption.IncreasedAllElements]: 80,
	[WeaponShellOption.ReducedMpConsumption]: 2,
	[WeaponShellOption.HpRecoveryPerKill]: 2,
	[WeaponShellOption.MpRecoveryPerKill]: 2,
	[WeaponShellOption.IncreasedSlDmgStat]: 1,
	[WeaponShellOption.IncreasedSlDefenseStat]: 1,
	[WeaponShellOption.IncreasedSlPropertyStat]: 1,
	[WeaponShellOption.IncreasedSlEnergyStat]: 1,
	[WeaponShellOption.IncreasedOverallSlStat]: 1,
	[WeaponShellOption.GainMoreGold]: 2,
	[WeaponShellOption.IncreasedCombatExp]: 2,
	[WeaponShellOption.IncreasedJobExp]: 2,
	[WeaponShellOption.IncreasedPvpDmg]: 2,
	[WeaponShellOption.ReducedPvpDefence]: 2,
	[WeaponShellOption.ReducedPvpFireResistance]: 2,
	[WeaponShellOption.ReducedPvpWaterResistance]: 2,
	[WeaponShellOption.ReducedPvpLightResistance]: 2,
	[WeaponShellOption.ReducedPvpShadowResistance]: 2,
	[WeaponShellOption.ReducedPvpAllResistances]: 2,
	[WeaponShellOption.HitEverytimeInPvp]: 2,
	[WeaponShellOption.PercentDmgAt15InPvp]: 2,
	[WeaponShellOption.ReducedPvpManaPerHit]: 2,
	[WeaponShellOption.IgnorePvpFireResistance]: 2,
	[WeaponShellOption.IgnorePvpWaterResistance]: 2,
	[WeaponShellOption.IgnorePvpLightResistance]: 2,
	[WeaponShellOption.IgnorePvpShadowResistance]: 2,
	[WeaponShellOption.SpRecoveryPerKill]: 2,
	[WeaponShellOption.IncreasedAccuracy]: 2,
	[WeaponShellOption.IncreasedConcentration]: 2,
};

const ArmorShellUpgradeBaseValue: Partial<Record<number, number>> = {
	[ArmorShellOption.EnhancedMeleeDefense]: 100,
	[ArmorShellOption.EnhancedLongRangeDefense]: 100,
	[ArmorShellOption.EnhancedMagicDefense]: 100,
	[ArmorShellOption.IncreasedOverallDefence]: 4,
	[ArmorShellOption.ReducedMinorBleedingChance]: 4,
	[ArmorShellOption.ReducedBleedingChance]: 4,
	[ArmorShellOption.ReducedAllBleedingChance]: 4,
	[ArmorShellOption.ReducedBlackoutChance]: 4,
	[ArmorShellOption.ReducedAllBlackoutChance]: 4,
	[ArmorShellOption.ReducedHandOfDeathChance]: 4,
	[ArmorShellOption.ReducedFreezeChance]: 4,
	[ArmorShellOption.ReducedBlindChance]: 4,
	[ArmorShellOption.ReducedBindChance]: 4,
	[ArmorShellOption.ReducedWeakenDefenceChance]: 4,
	[ArmorShellOption.ReducedShockChance]: 4,
	[ArmorShellOption.ReducedParalysisChance]: 4,
	[ArmorShellOption.ReducedAllNegativeEffectsChance]: 4,
	[ArmorShellOption.IncreasedHpRecoveryWhileResting]: 4,
	[ArmorShellOption.IncreasedNaturalHpRecovery]: 4,
	[ArmorShellOption.IncreasedMpRecoveryWhileResting]: 4,
	[ArmorShellOption.IncreasedNaturalMpRecovery]: 4,
	[ArmorShellOption.RecoverDmgAsHp]: 4,
	[ArmorShellOption.ReducedCritChance]: 4,
	[ArmorShellOption.IncreasedFireResistance]: 4,
	[ArmorShellOption.IncreasedWaterResistance]: 4,
	[ArmorShellOption.IncreasedLightResistance]: 4,
	[ArmorShellOption.IncreasedShadowResistance]: 4,
	[ArmorShellOption.IncreasedAllResistances]: 4,
	[ArmorShellOption.ReducedDignityLoss]: 4,
	[ArmorShellOption.ReducedProductionPointConsumption]: 4,
	[ArmorShellOption.MoreMinigameRewards]: 4,
	[ArmorShellOption.IncreasedItemRecovery]: 4,
	[ArmorShellOption.IncreasedPvpOverallDefence]: 4,
	[ArmorShellOption.DodgePvpMelee]: 4,
	[ArmorShellOption.DodgePvpRanged]: 4,
	[ArmorShellOption.DodgePvpMagic]: 4,
	[ArmorShellOption.DodgePvpAll]: 4,
	[ArmorShellOption.ManaDamageProtection]: 4,
	[ArmorShellOption.ImmuneToPvpFireDamage]: 4,
	[ArmorShellOption.ImmuneToPvpWaterDamage]: 4,
	[ArmorShellOption.ImmuneToPvpLightDamage]: 4,
	[ArmorShellOption.ImmuneToPvpShadowDamage]: 4,
};

function getShellUpgradeBonus(grade: number, vnum: number, upgrade: number): number {
	const map = grade > 12 ? ArmorShellUpgradeBaseValue : WeaponShellUpgradeBaseValue;
	const baseValue = map[vnum] ?? 10;
	return upgrade * baseValue;
}

export function formatAppliedShellEffectString(
	shellEffect: ShellEffect
): string {
	const base = formatShellOptionString(shellEffect);
	if (shellEffect.upgrade === 0) {
		return base;
	}
	const absUpgrade = Math.abs(shellEffect.upgrade);
	const bonus = getShellUpgradeBonus(shellEffect.grade, shellEffect.vnum, absUpgrade);
	const text = `++ ${base}(+${bonus})`;
	if (shellEffect.upgrade < 0) {
		return `[ ${text} ]`;
	}
	return text;
}

export function get_required_class_string(required_class: number | null): string {
	let result: string = '';

	if (required_class === null) {
		return result;
	}

	if (required_class == RequiredClass.All) {
		return result;
	}

	const single_class =
		required_class == RequiredClass.Adventurer ||
		required_class == RequiredClass.Swordsman ||
		required_class == RequiredClass.Archer ||
		required_class == RequiredClass.Mage ||
		required_class == RequiredClass.Martial;

	if (single_class) {
		if (required_class & RequiredClass.Adventurer) {
			result += get_const_string(ConstStringKey.Adventurer);
		}
		if (required_class & RequiredClass.Swordsman) {
			result += get_const_string(ConstStringKey.Swordsman);
		}
		if (required_class & RequiredClass.Archer) {
			result += get_const_string(ConstStringKey.Archer);
		}
		if (required_class & RequiredClass.Mage) {
			result += get_const_string(ConstStringKey.Mage);
		}
		if (required_class & RequiredClass.Martial) {
			result += get_const_string(ConstStringKey.MartialArtist);
		}
	} else {
		if (required_class & RequiredClass.Adventurer) {
			result += `${get_const_string(ConstStringKey.Adventurer)}, `;
		}
		if (required_class & RequiredClass.Swordsman) {
			result += `${get_const_string(ConstStringKey.Swordsman)}, `;
		}
		if (required_class & RequiredClass.Archer) {
			result += `${get_const_string(ConstStringKey.Archer)}, `;
		}
		if (required_class & RequiredClass.Mage) {
			result += `${get_const_string(ConstStringKey.Mage)}, `;
		}
		if (required_class & RequiredClass.Martial) {
			result += get_const_string(ConstStringKey.MartialArtist);
		}
	}

	result += ` ${get_const_string(ConstStringKey.Only)}`;

	return result;
}

export function get_required_level(item: EnrichedSearchResult): string {
	let result: string = '';

	if ('required_level' in item.data) {
		if (item.static_data?.flags.is_champion_equip) {
			result = `${get_const_string(ConstStringKey.Required)}${get_const_string(ConstStringKey.ChampionLevel)}: `;
		} else {
			result = `${get_const_string(ConstStringKey.Required)}${get_const_string(ConstStringKey.Level)}: `;
		}

		result += `${item.data.required_level}${get_const_string(ConstStringKey.Lv)}`;
	}

	return result;
}

export function get_element(item: EnrichedSearchResult): string {
	if ('element_type' in item.data) {
		switch (item.data.element_type) {
			case Element.NoElement:
				return get_const_string(ConstStringKey.FairyNoElement);
			case Element.Fire:
				return get_const_string(ConstStringKey.FairyFireElement);
			case Element.Water:
				return get_const_string(ConstStringKey.FairyWaterElement);
			case Element.Light:
				return get_const_string(ConstStringKey.FairyLightElement);
			case Element.Shadow:
				return get_const_string(ConstStringKey.FairyShadowElement);
		}
	}
	return '';
}

export function get_star_pets(item: EnrichedSearchResult): string {
	if ('stars' in item.data) {
		switch (item.data.stars) {
			case 1:
				return get_const_string(ConstStringKey.StarPets1);
			case 2:
				return get_const_string(ConstStringKey.StarPets2);
			case 3:
				return get_const_string(ConstStringKey.StarPets3);
			case 4:
				return get_const_string(ConstStringKey.StarPets4);
			case 5:
				return get_const_string(ConstStringKey.StarPets5);
			case 6:
				return get_const_string(ConstStringKey.StarPets6);
		}
	}

	return '';
}
