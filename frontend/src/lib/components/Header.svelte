<script lang="ts">
	import LanguageSelector from '$lib/components/LanguageSelector.svelte';
	import ServerSelector from '$lib/components/ServerSelector.svelte';

	let { server = $bindable() } = $props<{
		server?: number;
	}>();
</script>

<header class="top-bar">
	<div class="spacer"></div>
	<ServerSelector bind:value={server} />
	<LanguageSelector />
</header>

<style>
	.top-bar {
		display: flex;
		justify-content: flex-start;
		gap: 1rem;
		padding: 0.5rem 1rem;
		border-bottom: 1px solid #e5e7eb;
	}

	.spacer {
		flex: 1;
	}
</style>
