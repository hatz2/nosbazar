<script lang="ts">
	import BazarWindow from '$lib/components/BazarWindow.svelte';
	import Footer from '$lib/components/Footer.svelte';
	import Header from '$lib/components/Header.svelte';

	let server = $state(1);
</script>

<Header bind:server></Header>

<BazarWindow bind:server></BazarWindow>

<Footer></Footer>
