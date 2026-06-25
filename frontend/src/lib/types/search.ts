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

export interface EquipmentHatMaskData extends EquipmentData {
}

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

export type EnrichedSearchResult = SearchResult & {
	item_name: Record<string, string>;
	icon_id?: number;
};
