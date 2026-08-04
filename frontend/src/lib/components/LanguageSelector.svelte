<script lang="ts">
	import 'flag-icons/css/flag-icons.min.css';
	import Dropdown from '$lib/components/Dropdown.svelte';
	import { lang, type LanguageCode } from '$lib/stores/lang.svelte';

	type Language = {
		code: LanguageCode;
		flagClass: string;
		name: string;
	};

	const languages: Language[] = [
		{ code: 'ES', flagClass: 'es', name: 'Spanish' },
		{ code: 'UK', flagClass: 'gb', name: 'English' },
		{ code: 'FR', flagClass: 'fr', name: 'French' },
		{ code: 'DE', flagClass: 'de', name: 'German' },
		{ code: 'TR', flagClass: 'tr', name: 'Turkish' },
		{ code: 'IT', flagClass: 'it', name: 'Italian' },
		{ code: 'PL', flagClass: 'pl', name: 'Polish' },
		{ code: 'CZ', flagClass: 'cz', name: 'Czech' }
	];

	const current = $derived(languages.find((l) => l.code === lang.current) ?? languages[0]);

	// Sync to localStorage when language changes
	$effect(() => {
		localStorage.setItem('user-lang', lang.current);
	});
</script>

<div class="lang-selector">
	<label for="lang-select">Language: </label>
	<Dropdown
		options={languages}
		value={current}
		onselect={(language) => (lang.current = language.code)}
		buttonId="lang-select"
		getKey={(language) => language.code}
	>
		{#snippet children(language)}
			<span class={`fi fi-${language.flagClass}`}></span>
			{language.name}
		{/snippet}
	</Dropdown>
</div>

<style>
	.lang-selector {
		display: inline-flex;
		align-items: center;
		gap: 0.5rem;
		font-size: 0.875rem;
		padding: 0.25rem 0.5rem;
		background: rgba(10, 10, 10, 0.4);
		border-radius: 0.25rem;
		border: 1px solid #3a3a3a;
	}

	label {
		font-weight: 500;
		color: #c8c8c8;
	}

	:global(.fi) {
		font-size: 1.25rem;
	}
</style>
