<script lang="ts">
	import BazarWindow from '$lib/components/BazarWindow.svelte';
	import Header from '$lib/components/Header.svelte';

	let server = $state(1);
</script>

<Header bind:server></Header>

<BazarWindow bind:server></BazarWindow>
