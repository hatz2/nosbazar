export interface ItemBuffEntry {
	vnum: number;
	bcard_display: Record<string, string>;
}

export interface ItemFlagsData {
	no_selling: boolean;
	no_dropping: boolean;
	no_trading: boolean;
	miniland_item: boolean;
	miniland_item_2: boolean;
	show_warning_on_use: boolean;
	is_time_space_reward_box: boolean;
	show_description_on_hover: boolean;
	follow_mouse_on_use: boolean;
	show_something_on_hover: boolean;
	can_be_colored: boolean;
	female_can_wear: boolean;
	male_can_wear: boolean;
	play_sound_on_pickup: boolean;
	use_reputation_as_price: boolean;
	is_champion_equip: boolean;
	is_limited: boolean;
}

export interface ItemStaticData {
	vnum: number;
	price: number;
	name: Record<string, string>;
	inventory_tab: number;
	item_type: number;
	item_subtype: number;
	equip_slot: number;
	icon_id: number;
	visual_change_id: number;
	attack_type: number;
	required_class: number;
	flags: ItemFlagsData;
	buffs: ItemBuffEntry[];
	description: Record<string, string>;
}

export function isSingleGenderItem(item_data: ItemStaticData): boolean {
	if (item_data.flags.female_can_wear && !item_data.flags.male_can_wear) {
		return true;
	}

	if (!item_data.flags.female_can_wear && item_data.flags.male_can_wear) {
		return true;
	}

	return false;
}

const cache = new Map<number, ItemStaticData>();

export const itemService = {
	/**
	 * Gets static data for a single item. Uses cache if available.
	 */
	async getItemStaticData(vnum: number): Promise<ItemStaticData | null> {
		if (cache.has(vnum)) {
			return cache.get(vnum)!;
		}

		try {
			const response = await fetch(`http://localhost:8080/items/static/${vnum}`);
			if (!response.ok) {
				console.warn(`Failed to fetch static data for item ${vnum}: ${response.statusText}`);
				return null;
			}
			const data = await response.json();
			console.log(data);
			cache.set(vnum, data);
			return data;
		} catch (error) {
			console.error(`Error fetching static data for item ${vnum}:`, error);
			return null;
		}
	},

	/**
	 * Synchronously retrieves data from cache. Returns null if not present.
	 */
	getSync(vnum: number): ItemStaticData | null {
		return cache.get(vnum) || null;
	},

	/**
	 * Ensures all provided vnums are in the cache.
	 * Returns a list of vnums that were successfully fetched or are already cached.
	 */
	async fetchMany(vnums: number[]): Promise<Set<number>> {
		const uniqueVnums = Array.from(new Set(vnums));
		const missingVnums = uniqueVnums.filter((vnum) => !cache.has(vnum));

		if (missingVnums.length === 0) {
			return new Set(uniqueVnums);
		}

		await Promise.all(
			missingVnums.map(async (vnum) => {
				await this.getItemStaticData(vnum);
			})
		);

		return new Set(uniqueVnums);
	},

	/**
	 * Clears the cache.
	 */
	clearCache(): void {
		cache.clear();
	}
};
