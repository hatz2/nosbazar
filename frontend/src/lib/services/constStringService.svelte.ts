import type { LanguageCode } from '$lib/stores/lang.svelte';
import type { ConstStringKey } from '$lib/types/constStringKeys';
import { lang } from '$lib/stores/lang.svelte';

const ALL_LANGS: LanguageCode[] = ['ES', 'UK', 'FR', 'DE', 'TR', 'IT', 'PL', 'CZ'];
const cache: Record<number, Record<LanguageCode, string> | undefined> = $state({});

function emptyTranslations(): Record<LanguageCode, string> {
	const result = {} as Record<LanguageCode, string>;
	for (const lang of ALL_LANGS) {
		result[lang] = '';
	}
	return result;
}

export async function fetchConstString(id: ConstStringKey): Promise<void> {
	if (cache[id] !== undefined) return;

	try {
		const response = await fetch(`http://localhost:8080/conststring/${id}`);
		if (!response.ok) {
			console.warn(`Failed to fetch const string ${id}: ${response.statusText}`);
			cache[id] = emptyTranslations();
			return;
		}

		const data = (await response.json()) as Record<string, string>;
		const result: Record<LanguageCode, string> = { ...emptyTranslations(), ...data };
		cache[id] = result;
	} catch (error) {
		console.error(`Error fetching const string ${id}:`, error);
		cache[id] = emptyTranslations();
	}
}

export function get_const_string(key: ConstStringKey): string {
	if (cache[key] === undefined) {
		fetchConstString(key);
		return '';
	}

	return cache[key]?.[lang.current] ?? '';
}
