import type { LanguageCode } from '$lib/stores/lang.svelte';
import type { ConstStringKey } from '$lib/types/constStringKeys';
import { lang } from '$lib/stores/lang.svelte';
import { API_BASE } from '$lib/constants';

const ALL_LANGS: LanguageCode[] = ['ES', 'UK', 'FR', 'DE', 'TR', 'IT', 'PL', 'CZ'];
const STORAGE_KEY = 'nosbazar-conststrings';

type StoredCache = {
	version?: string;
	translations: Record<string, Record<string, string>>;
};

let cache = $state<Record<number, Record<LanguageCode, string> | undefined>>({});
const inFlight: Record<number, Promise<void> | undefined> = {};
let storedVersion: string | undefined;
let knownVersion = $state<string | undefined>();
let generation = 0;

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
		const stored = JSON.parse(raw) as StoredCache;
		storedVersion = stored.version;
		knownVersion = stored.version;
		for (const [key, translations] of Object.entries(stored.translations ?? {})) {
			cache[Number(key)] = { ...emptyTranslations(), ...translations };
		}
	} catch (error) {
		console.warn('Failed to load cached const strings:', error);
	}
}

function saveToStorage(): void {
	if (typeof localStorage === 'undefined') return;
	try {
		const stored: StoredCache = { version: knownVersion, translations: {} };
		for (const [key, translations] of Object.entries(cache)) {
			if (translations !== undefined) {
				stored.translations[key] = translations as Record<string, string>;
			}
		}
		localStorage.setItem(STORAGE_KEY, JSON.stringify(stored));
	} catch (error) {
		console.warn('Failed to save const strings to cache:', error);
	}
}

function clearCache(): void {
	generation += 1;
	cache = {};
}

async function checkVersion(): Promise<void> {
	try {
		const response = await fetch(`${API_BASE}/conststring/version`);
		if (!response.ok) return;
		const data = (await response.json()) as { version?: string };
		const serverVersion = data.version;

		knownVersion = serverVersion;
		if (storedVersion !== serverVersion) {
			storedVersion = serverVersion;
			clearCache();
			saveToStorage();
		}
	} catch (error) {
		console.error('Error checking const string version:', error);
	}
}

loadFromStorage();
checkVersion();

export async function fetchConstString(id: ConstStringKey): Promise<void> {
	if (cache[id] !== undefined) return;
	if (inFlight[id]) return inFlight[id];

	const requestGeneration = generation;
	const promise = (async () => {
		try {
			const response = await fetch(`${API_BASE}/conststring/${id}`);
			if (!response.ok) {
				console.warn(`Failed to fetch const string ${id}: ${response.statusText}`);
				if (requestGeneration === generation) {
					cache[id] = emptyTranslations();
				}
				return;
			}

			const data = (await response.json()) as Record<string, string>;
			if (requestGeneration === generation) {
				cache[id] = { ...emptyTranslations(), ...data };
				saveToStorage();
			}
		} catch (error) {
			console.error(`Error fetching const string ${id}:`, error);
			if (requestGeneration === generation) {
				cache[id] = emptyTranslations();
			}
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