export interface ItemStaticData {
	name: Record<string, string>;
	icon_id?: number;
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

		// Fetch missing items in parallel
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
