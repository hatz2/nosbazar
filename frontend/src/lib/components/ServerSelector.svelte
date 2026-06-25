<script lang="ts">
	import { onMount } from 'svelte';

	let { value = $bindable<number>() } = $props();
	let servers: { id: number; name: string }[] = $state([]);

	onMount(async () => {
		try {
			const res = await fetch('http://localhost:8080/servers');
			servers = await res.json();
		} catch (e) {
			console.error('Failed to fetch servers', e);
		}
	});

	$effect(() => {
		if (servers.length > 0 && !servers.some(s => s.id === value)) {
			value = servers[0].id;
		}
	});
</script>

<div class="server-selector">
	<label for="server-select">Server: </label>
	<select id="server-select" bind:value>
		{#each servers as server (server.id)}
			<option value={server.id}>
				{server.name}
			</option>
		{/each}
	</select>
</div>

<style>
	.server-selector {
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
