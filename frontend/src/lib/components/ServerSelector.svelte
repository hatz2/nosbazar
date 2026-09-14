<script lang="ts">
	import { onMount } from 'svelte';
	import Dropdown from '$lib/components/Dropdown.svelte';
	import { API_BASE } from '$lib/constants';

	type Server = {
		id: number;
		name: string;
	};

	let { value = $bindable<number>() } = $props();
	let servers: Server[] = $state([]);

	const STORAGE_KEY = 'selected-server';

	function readServerFromStorage(): number | undefined {
		if (typeof localStorage === 'undefined') {
			return undefined;
		}
		const raw = localStorage.getItem(STORAGE_KEY);
		if (raw === null) {
			return undefined;
		}
		const id = parseInt(raw, 10);
		return Number.isNaN(id) ? undefined : id;
	}

	const selected = $derived(servers.find((s) => s.id === value));

	onMount(async () => {
		try {
			const saved = readServerFromStorage();
			const res = await fetch(`${API_BASE}/servers`);
			servers = await res.json();

			if (saved !== undefined && servers.some((s) => s.id === saved)) {
				value = saved;
			} else if (servers.length > 0 && !servers.some((s) => s.id === value)) {
				value = servers[0].id;
			}
		} catch (e) {
			console.error('Failed to fetch servers', e);
		}
	});

	$effect(() => {
		if (servers.length > 0 && !servers.some((s) => s.id === value)) {
			value = servers[0].id;
		}
	});

	$effect(() => {
		if (typeof localStorage !== 'undefined' && value !== undefined) {
			localStorage.setItem(STORAGE_KEY, String(value));
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
