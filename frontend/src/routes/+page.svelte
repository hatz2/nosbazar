<script lang="ts">
	import BazarWindow from '$lib/components/BazarWindow.svelte';
	import LanguageSelector from '$lib/components/LanguageSelector.svelte';
</script>

<header class="top-bar">
	<div class="spacer"></div>
	<LanguageSelector />
</header>

<BazarWindow></BazarWindow>

<style>
	.top-bar {
		display: flex;
		justify-content: flex-end;
		padding: 0.5rem 1rem;
		background: #ffffff;
		border-bottom: 1px solid #e5e7eb;
	}

	.spacer {
		flex: 1;
	}
</style>
