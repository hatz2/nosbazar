<script lang="ts">
	import favicon from '$lib/assets/favicon.svg';

	let { children } = $props();
</script>

<svelte:head>
	<link rel="icon" href={favicon} />
</svelte:head>

{@render children()}

<style global>
	:global(body) {
		font-family: Tahoma, Arial, sans-serif;
		font-size: 10pt;
		-webkit-font-smoothing: none;
		font-smooth: never;
		font-style: normal;
		font-weight: normal;
		font-variant: normal;
	}
</style>
