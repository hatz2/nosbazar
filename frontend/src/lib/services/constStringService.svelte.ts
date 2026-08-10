import type { LanguageCode } from '$lib/stores/lang.svelte';
import type { ConstStringKey } from '$lib/types/constStringKeys';
import { lang } from '$lib/stores/lang.svelte';
import { API_BASE } from '$lib/constants';

const ALL_LANGS: LanguageCode[] = ['ES', 'UK', 'FR', 'DE', 'TR', 'IT', 'PL', 'CZ'];
const STORAGE_KEY = 'nosbazar-conststrings-v1';

const cache: Record<number, Record<LanguageCode, string> | undefined> = $state({});
const inFlight: Record<number, Promise<void> | undefined> = {};

function emptyTranslations(): Record<LanguageCode, string> {
	const result = {} as Record<LanguageCode, string>;
	for (const lang of ALL_LANGS) {
		result[lang] = '';
	}
	return result;
}

function loadFromStorage(): void {
	if (typeof localStorage === 'undefined') return;
	try {
		const raw = localStorage.getItem(STORAGE_KEY);
		if (!raw) return;
		const stored = JSON.parse(raw) as Record<string, Record<string, string>>;
		for (const [key, translations] of Object.entries(stored)) {
			cache[Number(key)] = { ...emptyTranslations(), ...translations };
		}
	} catch (error) {
		console.warn('Failed to load cached const strings:', error);
	}
}

function saveToStorage(): void {
	if (typeof localStorage === 'undefined') return;
	try {
		const stored: Record<string, Record<string, string>> = {};
		for (const [key, translations] of Object.entries(cache)) {
			if (translations !== undefined) {
				stored[key] = translations as Record<string, string>;
			}
		}
		localStorage.setItem(STORAGE_KEY, JSON.stringify(stored));
	} catch (error) {
		console.warn('Failed to save const strings to cache:', error);
	}
}

loadFromStorage();

export async function fetchConstString(id: ConstStringKey): Promise<void> {
	if (cache[id] !== undefined) return;
	if (inFlight[id]) return inFlight[id];

	const promise = (async () => {
		try {
			const response = await fetch(`${API_BASE}/conststring/${id}`);
			if (!response.ok) {
				console.warn(`Failed to fetch const string ${id}: ${response.statusText}`);
				cache[id] = emptyTranslations();
				return;
			}

			const data = (await response.json()) as Record<string, string>;
			cache[id] = { ...emptyTranslations(), ...data };
			saveToStorage();
		} catch (error) {
			console.error(`Error fetching const string ${id}:`, error);
			cache[id] = emptyTranslations();
		} finally {
			delete inFlight[id];
		}
	})();

	inFlight[id] = promise;
	return promise;
}

export function get_const_string(key: ConstStringKey): string {
	if (cache[key] === undefined) {
		fetchConstString(key);
		return '';
	}

	return cache[key]?.[lang.current] ?? '';
}