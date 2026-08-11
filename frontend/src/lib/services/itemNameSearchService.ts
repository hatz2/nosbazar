import { API_BASE } from '$lib/constants';
import type { LanguageCode } from '$lib/stores/lang.svelte';

export interface ItemNameMatch {
	vnum: number;
	name: string;
	icon_id: number;
}

const cache = new Map<string, ItemNameMatch[]>();

export const itemNameSearchService = {
	/**
	 * Searches item names (case-insensitive) in the given language.
	 * Results are cached per (lang, query) pair.
	 */
	async searchItemNames(
		query: string,
		lang: LanguageCode,
		signal?: AbortSignal
	): Promise<ItemNameMatch[]> {
		const key = `${lang}|${query.toLowerCase()}`;
		const cached = cache.get(key);
		if (cached) {
			return cached;
		}

		const url = `${API_BASE}/items/name-search?q=${encodeURIComponent(query)}&lang=${lang}`;
		try {
			const response = await fetch(url, { signal });
			if (!response.ok) {
				console.warn(`Failed to search item names for "${query}": ${response.statusText}`);
				return [];
			}
			const data = await response.json();
			const matches = (data.matches ?? []) as ItemNameMatch[];
			cache.set(key, matches);
			return matches;
		} catch (error) {
			if (error instanceof DOMException && error.name === 'AbortError') {
				return [];
			}
			console.error(`Error searching item names for "${query}":`, error);
			return [];
		}
	},

	/**
	 * Clears the suggestion cache.
	 */
	clearCache(): void {
		cache.clear();
	}
};
