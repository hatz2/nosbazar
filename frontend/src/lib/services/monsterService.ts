import { API_BASE } from '$lib/constants';

export interface MonsterStaticData {
	vnum: number;
	name: Record<string, string>;
	icon_id: number;
}

const cache = new Map<number, MonsterStaticData>();

export const monsterService = {
	async getMonsterStaticData(vnum: number): Promise<MonsterStaticData | null> {
		if (cache.has(vnum)) {
			return cache.get(vnum)!;
		}

		try {
			const response = await fetch(`${API_BASE}/monster/static/${vnum}`);
			if (!response.ok) {
				console.warn(`Failed to fetch static data for monster ${vnum}: ${response.statusText}`);
				return null;
			}
			const data = await response.json();
			cache.set(vnum, data);
			return data;
		} catch (error) {
			console.error(`Error fetching static data for monster ${vnum}:`, error);
			return null;
		}
	},

	getSync(vnum: number): MonsterStaticData | null {
		return cache.get(vnum) || null;
	},

	async fetchMany(vnums: number[]): Promise<Set<number>> {
		const uniqueVnums = Array.from(new Set(vnums));
		const missingVnums = uniqueVnums.filter((vnum) => !cache.has(vnum));

		if (missingVnums.length === 0) {
			return new Set(uniqueVnums);
		}

		await Promise.all(
			missingVnums.map(async (vnum) => {
				await this.getMonsterStaticData(vnum);
			})
		);

		return new Set(uniqueVnums);
	},

	clearCache(): void {
		cache.clear();
	}
};
