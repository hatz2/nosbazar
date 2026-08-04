<script lang="ts">
	import { onMount } from 'svelte';
	import Dropdown from '$lib/components/Dropdown.svelte';

	type Server = {
		id: number;
		name: string;
	};

	let { value = $bindable<number>() } = $props();
	let servers: Server[] = $state([]);

	const selected = $derived(servers.find((s) => s.id === value));

	onMount(async () => {
		try {
			const res = await fetch('http://localhost:8080/servers');
			servers = await res.json();
		} catch (e) {
			console.error('Failed to fetch servers', e);
		}
	});

	$effect(() => {
		if (servers.length > 0 && !servers.some((s) => s.id === value)) {
			value = servers[0].id;
		}
	});
</script>

<div class="server-selector">
	<label for="server-select">Server: </label>
	<Dropdown
		options={servers}
		value={selected}
		onselect={(server) => (value = server.id)}
		buttonId="server-select"
		getKey={(server) => server.id}
	>
		{#snippet children(server)}
			{server.name}
		{/snippet}
	</Dropdown>
</div>

<style>
	.server-selector {
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
</style>
