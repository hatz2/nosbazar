import type { LanguageCode } from '$lib/stores/lang.svelte';
import type { ConstStringKey } from '$lib/types/constStringKeys';
import { lang } from '$lib/stores/lang.svelte';
import { API_BASE } from '$lib/constants';

const ALL_LANGS: LanguageCode[] = ['ES', 'UK', 'FR', 'DE', 'TR', 'IT', 'PL', 'CZ'];
const STORAGE_KEY = 'nosbazar-conststrings';
const MAX_FETCH_ATTEMPTS = 3;
const RETRY_DELAY_MS = 500;

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
			if (ALL_LANGS.some((language) => translations[language])) {
				cache[Number(key)] = { ...emptyTranslations(), ...translations };
			}
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
		const data = (await response.json()) as { version?: unknown } | null;
		const serverVersion = data?.version;
		if (typeof serverVersion !== 'string' || serverVersion.length === 0) return;

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
const initialization = checkVersion();

export async function fetchConstString(id: ConstStringKey): Promise<void> {
	if (cache[id] !== undefined) return;
	if (inFlight[id]) return inFlight[id];

	const promise = (async () => {
		try {
			// Resolve cache invalidation before starting any translation requests.
			await initialization;
			if (cache[id] !== undefined) return;
			const requestGeneration = generation;

			for (let attempt = 0; attempt < MAX_FETCH_ATTEMPTS; attempt += 1) {
				try {
					const response = await fetch(`${API_BASE}/conststring/${id}`);
					if (response.ok) {
						const data = (await response.json()) as Record<string, string>;
						if (requestGeneration === generation) {
							cache[id] = { ...emptyTranslations(), ...data };
							saveToStorage();
						}
						return;
					}

					console.warn(`Failed to fetch const string ${id}: ${response.statusText}`);
					if (response.status < 500 && response.status !== 408 && response.status !== 429) {
						return;
					}
				} catch (error) {
					console.error(`Error fetching const string ${id}:`, error);
				}

				if (attempt + 1 < MAX_FETCH_ATTEMPTS) {
					await new Promise<void>((resolve) => setTimeout(resolve, RETRY_DELAY_MS * 2 ** attempt));
				}
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
