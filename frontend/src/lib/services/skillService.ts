import { API_BASE } from '$lib/constants';

export interface SkillStaticData {
	vnum: number;
	name: Record<string, string>;
}

const cache = new Map<number, SkillStaticData>();

export const skillService = {
	async getSkillStaticData(vnum: number): Promise<SkillStaticData | null> {
		if (cache.has(vnum)) {
			return cache.get(vnum)!;
		}

		try {
			const response = await fetch(`${API_BASE}/skill/static/${vnum}`);
			if (!response.ok) {
				console.warn(`Failed to fetch static data for skill ${vnum}: ${response.statusText}`);
				return null;
			}
			const data = await response.json();
			cache.set(vnum, data);
			return data;
		} catch (error) {
			console.error(`Error fetching static data for skill ${vnum}:`, error);
			return null;
		}
	},

	getSync(vnum: number): SkillStaticData | null {
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
				await this.getSkillStaticData(vnum);
			})
		);

		return new Set(uniqueVnums);
	},

	clearCache(): void {
		cache.clear();
	}
};
