import type { ItemFlagsData, ItemStaticData } from '$lib/services/itemService';
import type { MonsterStaticData } from '$lib/services/monsterService';
import { Element } from './enums';

export type SearchResult = {
	owner_name: string;
	item_vnum: number;
	amount: number;
	is_package: boolean;
	bazar_price: number;
	minutes_left: number;
	data: SearchResultData;
};

export type EnrichedSearchResult = SearchResult & {
	item_name: Record<string, string>;
	static_data: ItemStaticData | null;
	contained_item_static_data?: ItemStaticData;
	contained_monster_static_data?: MonsterStaticData;
};

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

interface AmuletData {
	vnum: number;
	required_level: number;
	price: number;
	remaining_time_in_secs: number;
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
	options: CellonOption[];
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
	price: number;
}

export interface FairyData {
	vnum: number;
	element_type: Element;
	fairy_level_percent: number;
	cannot_trade_after_equip: boolean;
	monsters_to_level_up: number;
}

export interface MiniPetData {
	vnum: number;
	required_level: number;
	remaining_time_in_hours: number;
	cannot_trade_after_equip: boolean;
	price: number;
}

export interface PetBeadData {
	vnum: number;
	has_pet_inside: boolean;
	pet_vnum: number;
	level: number;
	level_percentage: number;
	attack_level: number;
	defence_level: number;
	stars: number;
	price: number;
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
	| AmuletData
	| FairyData
	| MiniPetData
	| PetBeadData
	| NoData;

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

export function itemHasFlagsToDisplay(flags: ItemFlagsData): boolean {
	return flags.no_dropping || flags.no_selling || flags.no_trading;
}

export function isPetBeadItem(data: SearchResultData): data is PetBeadData {
	return 'has_pet_inside' in data;
}

export function get_display_icon_id(item: EnrichedSearchResult): number {
	if (item.item_vnum === 285 && item.contained_monster_static_data) {
		return item.contained_monster_static_data.icon_id;
	}
	return item.contained_item_static_data?.icon_id ?? item.static_data?.icon_id ?? 0;
}
