// Available languages based on backend LangManager
export type LanguageCode = 'ES' | 'UK' | 'FR' | 'DE' | 'TR' | 'IT' | 'RU' | 'PL' | 'CZ';

export const language = $state<LanguageCode>('UK');
