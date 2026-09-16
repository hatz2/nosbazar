<script lang="ts">
	import favicon from '$lib/assets/favicon.png';
	import '../app.css';

	let { children } = $props();
</script>

<svelte:head>
	<link rel="icon" href={favicon} />
	<title>NosBazar</title>
</svelte:head>

<div class="page">
	{@render children()}
</div>

<style>
	.page {
		display: flex;
		flex-direction: column;
		min-height: 100vh;
	}
</style>
