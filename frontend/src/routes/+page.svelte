<script lang="ts">
	import BazarWindow from '$lib/components/BazarWindow.svelte';
	import LanguageSelector from '$lib/components/LanguageSelector.svelte';
	import ServerSelector from '$lib/components/ServerSelector.svelte';

	let server = $state(1);
</script>

<header class="top-bar">
	<div class="spacer"></div>
	<ServerSelector bind:value={server} />
	<LanguageSelector />
</header>

<BazarWindow bind:server></BazarWindow>

<style>
	.top-bar {
		display: flex;
		justify-content: flex-start;
		padding: 0.5rem 1rem;
		/* background: #ffffff; */
		border-bottom: 1px solid #e5e7eb;
	}

	.spacer {
		flex: 1;
	}
</style>
