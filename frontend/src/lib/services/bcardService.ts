import type { LanguageCode } from '$lib/stores/lang.svelte';

const ALL_LANGS: LanguageCode[] = ['ES', 'UK', 'FR', 'DE', 'TR', 'IT', 'PL', 'CZ'];
const cache = new Map<string, Record<LanguageCode, string>>();

function emptyTranslations(): Record<LanguageCode, string> {
	const result = {} as Record<LanguageCode, string>;
	for (const lang of ALL_LANGS) {
		result[lang] = '';
	}
	return result;
}

export async function fetchBcardString(
	vnum: number,
	sub: number,
	val1: number,
	val2: number
): Promise<Record<LanguageCode, string>> {
	const key = `${vnum}-${sub}-${val1}-${val2}`;

	if (cache.has(key)) {
		return cache.get(key)!;
	}

	try {
		const response = await fetch(
			`http://localhost:8080/bcard/string?vnum=${vnum}&sub=${sub}&val1=${val1}&val2=${val2}`
		);
		if (!response.ok) {
			console.warn(`Failed to fetch bcard string ${key}: ${response.statusText}`);
			const empty = emptyTranslations();
			cache.set(key, empty);
			return empty;
		}

		const data = (await response.json()) as Record<string, string>;
		const result: Record<LanguageCode, string> = { ...emptyTranslations(), ...data };
		cache.set(key, result);
		return result;
	} catch (error) {
		console.error(`Error fetching bcard string ${key}:`, error);
		const empty = emptyTranslations();
		cache.set(key, empty);
		return empty;
	}
}
