export interface ShellEffect {
	grade: number;
	vnum: number;
	value: number;
	upgrade: number;
}

interface WeaponData {
	vnum: number;
	rare: number;
	upgrade: number;
	fixed_level: boolean;
	required_level: number;
	min_dmg: number;
	max_dmg: number;
	price: number;
	shell_count: number;
	shells: ShellEffect[];
}

export interface MeleeWeaponData extends WeaponData {
	hit_rate: number;
	critical_rate: number;
	critical_dmg: number;
}

export interface RangedWeaponData extends WeaponData {
	hit_rate: number;
	critical_rate: number;
	critical_dmg: number;
	ammo: number;
	max_ammo: number;
}

export interface MagicWeaponData extends WeaponData {
	concentration: number;
}

export interface ArmourData {
	vnum: number;
	rare: number;
	upgrade: number;
	fixed_level: boolean;
	required_level: number;
	melee_defence: number;
	ranged_defence: number;
	magic_defence: number;
	dodge: number;
	price: number;
	shells: ShellEffect[];
}

interface EquipmentData {
	vnum: number;
	required_level: number;
	melee_defence: number;
	ranged_defence: number;
	magic_defence: number;
	dodge: number;
	price: number;
}

export interface ResistancesData extends EquipmentData {
	fire_res: number;
	water_res: number;
	light_res: number;
	shadow_res: number;
	sum_level: number;
}

export interface EquipmentHatMaskData extends EquipmentData {}

export interface CellonOption {
	vnum: number;
	level: number;
	value: number;
}

export interface AccessoryData {
	vnum: number;
	required_level: number;
	max_option_level: number;
	max_option_count: number;
	price: number;
}

export interface SpecialistData {
	icon_id: number;
	contains_sp: boolean;
	vnum: number;
	job_level: number;
	level_percentage: number;
	upgrade_level: number;
	attack_points: number;
	defence_points: number;
	element_points: number;
	hp_points: number;
	remaining_points: number;
	perfection_level: number;
	fire_res: number;
	water_res: number;
	light_res: number;
	shadow_res: number;
	attack_perf: number;
	defence_perf: number;
	element_perf: number;
	hp_perf: number;
	fire_res_perf: number;
	water_res_perf: number;
	light_res_perf: number;
	shadow_res_perf: number;
}

export type NoData = Record<string, never>;

export type SearchResultData =
	| MeleeWeaponData
	| RangedWeaponData
	| MagicWeaponData
	| ArmourData
	| ResistancesData
	| EquipmentHatMaskData
	| AccessoryData
	| SpecialistData
	| NoData;

export type SearchResult = {
	owner_name: string;
	item_vnum: number;
	amount: number;
	is_package: boolean;
	bazar_price: number;
	minutes_left: number;
	data: SearchResultData;
};

import type { ItemStaticData, ItemFlagsData } from '$lib/services/itemService';
import type { LanguageCode } from '$lib/stores/lang.svelte';

export type EnrichedSearchResult = SearchResult & {
	item_name: Record<string, string>;
	icon_id?: number;
	static_data: ItemStaticData | null;
};

// ---- Type guards for the SearchResultData union ----

export function isMeleeWeapon(data: SearchResultData): data is MeleeWeaponData {
	return 'min_dmg' in data && 'hit_rate' in data && !('ammo' in data);
}

export function isRangedWeapon(data: SearchResultData): data is RangedWeaponData {
	return 'min_dmg' in data && 'hit_rate' in data && 'ammo' in data;
}

export function isMagicWeapon(data: SearchResultData): data is MagicWeaponData {
	return 'min_dmg' in data && 'concentration' in data;
}

export function isArmour(data: SearchResultData): data is ArmourData {
	return 'melee_defence' in data && !('fire_res' in data);
}

export function isResistances(data: SearchResultData): data is ResistancesData {
	return 'melee_defence' in data && 'fire_res' in data;
}

export function isHatMask(data: SearchResultData): data is EquipmentHatMaskData {
	if (!('melee_defence' in data)) return false;
	return (
		!('max_dmg' in data) &&
		!('fire_res' in data) &&
		!('max_option_level' in data) &&
		!('icon_id' in data)
	);
}

export function isAccessory(data: SearchResultData): data is AccessoryData {
	return 'max_option_level' in data;
}

export function isSpecialist(data: SearchResultData): data is SpecialistData {
	return 'icon_id' in data && 'contains_sp' in data;
}

export function isNoData(data: SearchResultData): data is NoData {
	return Object.keys(data).length === 0;
}

// ---- Formatting types ----

export interface FormattedSectionField {
	label: string;
	value: string | number;
	cssClass: string;
}

export interface FormattedSection {
	name: string;
	fields: FormattedSectionField[];
}

function fPrice(price: number): string {
	return price.toLocaleString('en-US');
}

function buildUpgradeSection(shells: ShellEffect[]): FormattedSectionField[] {
	const fields: FormattedSectionField[] = [];

	for (const shell of shells) {
		fields.push({ label: `Shell ${shell.grade}`, value: `${shell.value}`, cssClass: 'stat-shell' });
	}

	return fields;
}

// ---- Formatters per type ----

function formatMelee(data: MeleeWeaponData): FormattedSection[] {
	const sections: FormattedSection[] = [
		{
			name: 'damage',
			fields: [
				{ label: 'Damage', value: `${data.min_dmg}-${data.max_dmg}`, cssClass: 'stat-damage' },
				{ label: 'Hit Rate', value: data.hit_rate, cssClass: 'stat-hitrate' },
				{ label: 'Critical Rate', value: data.critical_rate, cssClass: 'stat-crit' },
				{ label: 'Critical Dmg', value: data.critical_dmg, cssClass: 'stat-crit' }
			]
		}
	];

	if (data.shells.length > 0) {
		sections.push({ name: 'shells', fields: buildUpgradeSection(data.shells) });
	}

	sections.push({
		name: 'other',
		fields: [
			{ label: 'Level', value: data.required_level, cssClass: 'stat-level' },
			{ label: 'Price (item)', value: fPrice(data.price), cssClass: 'stat-price' }
		]
	});

	return sections;
}

function formatRanged(data: RangedWeaponData): FormattedSection[] {
	return [
		{
			name: 'damage',
			fields: [
				{ label: 'Damage', value: `${data.min_dmg}-${data.max_dmg}`, cssClass: 'stat-damage' },
				{ label: 'Hit Rate', value: data.hit_rate, cssClass: 'stat-hitrate' },
				{ label: 'Critical Rate', value: data.critical_rate, cssClass: 'stat-crit' },
				{ label: 'Critical Dmg', value: data.critical_dmg, cssClass: 'stat-crit' }
			]
		},
		{
			name: 'ammo',
			fields: [{ label: 'Ammo', value: `${data.ammo}/${data.max_ammo}`, cssClass: 'stat-ammo' }]
		}
	] as FormattedSection[];
}

function formatMagic(data: MagicWeaponData): FormattedSection[] {
	return [
		{
			name: 'damage',
			fields: [
				{ label: 'Damage', value: `${data.min_dmg}-${data.max_dmg}`, cssClass: 'stat-damage' },
				{ label: 'Concentration', value: data.concentration, cssClass: 'stat-magic' }
			]
		}
	] as FormattedSection[];
}

function formatArmour(data: ArmourData): FormattedSection[] {
	const sections: FormattedSection[] = [
		{
			name: 'defense',
			fields: [
				{ label: 'Melee Defence', value: data.melee_defence, cssClass: 'stat-defense-melee' },
				{ label: 'Ranged Defence', value: data.ranged_defence, cssClass: 'stat-defense-ranged' },
				{ label: 'Magic Defence', value: data.magic_defence, cssClass: 'stat-defense-magic' },
				{ label: 'Dodge', value: data.dodge, cssClass: 'stat-dodge' }
			]
		}
	];

	if (data.shells.length > 0) {
		sections.push({ name: 'shells', fields: buildUpgradeSection(data.shells) });
	}

	sections.push({
		name: 'other',
		fields: [
			{ label: 'Level', value: data.required_level, cssClass: 'stat-level' },
			{ label: 'Price (item)', value: fPrice(data.price), cssClass: 'stat-price' }
		]
	});

	return sections;
}

function formatResistances(data: ResistancesData): FormattedSection[] {
	const sections: FormattedSection[] = [
		{
			name: 'defense',
			fields: [
				{ label: 'Melee Defence', value: data.melee_defence, cssClass: 'stat-defense-melee' },
				{ label: 'Ranged Defence', value: data.ranged_defence, cssClass: 'stat-defense-ranged' },
				{ label: 'Magic Defence', value: data.magic_defence, cssClass: 'stat-defense-magic' },
				{ label: 'Dodge', value: data.dodge, cssClass: 'stat-dodge' }
			]
		},
		{
			name: 'resistances',
			fields: [
				{ label: 'Fire Res', value: data.fire_res, cssClass: 'stat-res-fire' },
				{ label: 'Water Res', value: data.water_res, cssClass: 'stat-res-water' },
				{ label: 'Light Res', value: data.light_res, cssClass: 'stat-res-light' },
				{ label: 'Shadow Res', value: data.shadow_res, cssClass: 'stat-res-shadow' },
				{ label: 'Total Resist', value: data.sum_level, cssClass: 'stat-res-total' }
			]
		}
	];

	sections.push({
		name: 'other',
		fields: [
			{ label: 'Level', value: data.required_level, cssClass: 'stat-level' },
			{ label: 'Price (item)', value: fPrice(data.price), cssClass: 'stat-price' }
		]
	});

	return sections;
}

function formatHatMask(data: EquipmentHatMaskData): FormattedSection[] {
	return [
		{
			name: 'defense',
			fields: [
				{ label: 'Melee Defence', value: data.melee_defence, cssClass: 'stat-defense-melee' },
				{ label: 'Ranged Defence', value: data.ranged_defence, cssClass: 'stat-defense-ranged' },
				{ label: 'Magic Defence', value: data.magic_defence, cssClass: 'stat-defense-magic' },
				{ label: 'Dodge', value: data.dodge, cssClass: 'stat-dodge' }
			]
		},
		{
			name: 'other',
			fields: [
				{ label: 'Level', value: data.required_level, cssClass: 'stat-level' },
				{ label: 'Price (item)', value: fPrice(data.price), cssClass: 'stat-price' }
			]
		}
	] as FormattedSection[];
}

function formatAccessory(data: AccessoryData): FormattedSection[] {
	return [
		{
			name: 'options',
			fields: [
				{ label: 'Max Option Level', value: data.max_option_level, cssClass: 'stat-accessory' },
				{ label: 'Max Option Count', value: data.max_option_count, cssClass: 'stat-accessory' }
			]
		},
		{
			name: 'other',
			fields: [
				{ label: 'Level', value: data.required_level, cssClass: 'stat-level' },
				{ label: 'Price (item)', value: fPrice(data.price), cssClass: 'stat-price' }
			]
		}
	] as FormattedSection[];
}

function formatSpecialist(data: SpecialistData): FormattedSection[] {
	return [
		{
			name: 'points',
			fields: [
				{ label: 'Attack Pts', value: data.attack_points, cssClass: 'stat-specialist-offense' },
				{ label: 'Defence Pts', value: data.defence_points, cssClass: 'stat-specialist-defense' },
				{ label: 'Element Pts', value: data.element_points, cssClass: 'stat-specialist-element' },
				{ label: 'HP Pts', value: data.hp_points, cssClass: 'stat-specialist-hp' },
				{
					label: 'Remaining Pts',
					value: data.remaining_points,
					cssClass: 'stat-specialist-remaining'
				}
			]
		},
		{
			name: 'resistances',
			fields: [
				{ label: 'Fire Res', value: data.fire_res, cssClass: 'stat-res-fire' },
				{ label: 'Water Res', value: data.water_res, cssClass: 'stat-res-water' },
				{ label: 'Light Res', value: data.light_res, cssClass: 'stat-res-light' },
				{ label: 'Shadow Res', value: data.shadow_res, cssClass: 'stat-res-shadow' }
			]
		},
		{
			name: 'other',
			fields: [
				{ label: 'Job Level', value: data.job_level, cssClass: 'stat-level' },
				{ label: 'Perfection Lv', value: data.perfection_level, cssClass: 'stat-rare' }
			]
		}
	] as FormattedSection[];
}

// ---- Main formatting function ----

export function formatItemData(data: SearchResultData): FormattedSection[] {
	if (isMeleeWeapon(data)) return formatMelee(data);
	if (isRangedWeapon(data)) return formatRanged(data);
	if (isMagicWeapon(data)) return formatMagic(data);
	if (isArmour(data)) return formatArmour(data);
	if (isResistances(data)) return formatResistances(data);
	if (isHatMask(data)) return formatHatMask(data);
	if (isAccessory(data)) return formatAccessory(data);
	if (isSpecialist(data)) return formatSpecialist(data);
	return [];
}

// ---- Static data formatting ----

const classNames = ['Adventurer', 'Swordsman', 'Archer', 'Mage', 'Martial Artist'];

const flagLabels: [keyof ItemFlagsData, string][] = [
	['no_selling', 'No Selling'],
	['no_dropping', 'No Dropping'],
	['no_trading', 'No Trading'],
	['show_warning_on_use', 'Warning on Use'],
	['female_can_wear', 'Female Only'],
	['male_can_wear', 'Male Only'],
	['is_champion_equip', 'Champion Only'],
	['is_limited', 'Limited']
];

export function formatStaticData(
	static_data: ItemStaticData | null,
	langCode: LanguageCode
): FormattedSection[] {
	if (!static_data) return [];

	const sections: FormattedSection[] = [];

	// Required class
	const canUse =
		classNames.filter((_, i) => static_data.required_class & (1 << i)).join(', ') || 'None';

	sections.push({
		name: 'requirements',
		fields: [{ label: '', value: canUse + ' only', cssClass: 'stat-level' }]
	});

	// Flags (only true)
	const flagFields = flagLabels
		.filter(([key]) => static_data.flags[key])
		.map(([, label]) => ({ label, value: '⚠', cssClass: 'stat-flag' }));

	if (flagFields.length > 0) {
		sections.push({ name: 'flags', fields: flagFields });
	}

	// Description
	// const desc = static_data.description?.[langCode] || static_data.description?.['UK'] || '';
	// if (desc) {
	// 	sections.push({
	// 		name: 'description',
	// 		fields: [{ label: 'Description', value: desc, cssClass: 'stat-desc' }]
	// 	});
	// }

	return sections;
}
