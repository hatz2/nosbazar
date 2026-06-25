// Available languages based on backend LangManager
export type LanguageCode = 'ES' | 'UK' | 'FR' | 'DE' | 'TR' | 'IT' | 'RU' | 'PL' | 'CZ';

const savedLang =
	typeof localStorage !== 'undefined'
		? (localStorage.getItem('user-lang') as LanguageCode)
		: undefined;

export const lang = $state({
	current: savedLang || ('UK' as LanguageCode)
});
