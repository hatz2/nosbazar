<script lang="ts">
	import { lang} from '$lib/stores/lang.svelte';

	// Sync to localStorage when language changes
	$effect(() => {
		localStorage.setItem('user-lang', lang.current);
	});
</script>

<div class="lang-selector">
	<label for="lang-select">Language: </label>
	<select id="lang-select" bind:value={lang.current}>
		<option value="ES">Spanish</option>
		<option value="UK">English</option>
		<option value="FR">French</option>
		<option value="DE">German</option>
		<option value="TR">Turkish</option>
		<option value="IT">Italian</option>
		<option value="RU">Russian</option>
		<option value="PL">Polish</option>
		<option value="CZ">Czech</option>
	</select>
</div>

<style>
	.lang-selector {
		display: inline-flex;
		align-items: center;
		gap: 0.5rem;
		font-size: 0.875rem;
		padding: 0.25rem 0.5rem;
		background: #f3f4f6;
		border-radius: 0.25rem;
		border: 1px solid #d1d5db;
	}

	label {
		font-weight: 500;
	}

	select {
		background: transparent;
		border: none;
		outline: none;
		cursor: pointer;
		font-family: inherit;
		font-size: inherit;
	}
</style>
