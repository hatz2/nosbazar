<script lang="ts" generics="T">
	import type { Snippet } from 'svelte';

	type Props<T> = {
		options: T[];
		value?: T;
		onselect?: (option: T) => void;
		buttonId?: string;
		getKey?: (option: T) => string | number;
		children: Snippet<[T]>;
	};

	let {
		options,
		value,
		onselect,
		buttonId,
		getKey = (option: T) => String(option),
		children
	}: Props<T> = $props();

	let container = $state<HTMLDivElement | null>(null);
	let open = $state(false);

	const selected = $derived(options.find((option) => option === value) ?? options[0]);

	$effect(() => {
		if (!open) return;
		const onPointerDown = (event: PointerEvent) => {
			if (container && !container.contains(event.target as Node)) {
				open = false;
			}
		};
		const onKeyDown = (event: KeyboardEvent) => {
			if (event.key === 'Escape') {
				open = false;
			}
		};
		window.addEventListener('pointerdown', onPointerDown);
		window.addEventListener('keydown', onKeyDown);
		return () => {
			window.removeEventListener('pointerdown', onPointerDown);
			window.removeEventListener('keydown', onKeyDown);
		};
	});

	function select(option: T) {
		onselect?.(option);
		open = false;
	}
</script>

<div class="dropdown" bind:this={container}>
	<button
		id={buttonId}
		class="dropdown-toggle"
		type="button"
		aria-haspopup="listbox"
		aria-expanded={open}
		onclick={() => (open = !open)}
	>
		{#if selected}
			{@render children(selected)}
		{/if}
		<span class="caret" aria-hidden="true">▾</span>
	</button>

	{#if open}
		<ul class="dropdown-menu" role="listbox">
			{#each options as option (getKey(option))}
				<li role="none">
					<button
						type="button"
						role="option"
						class="dropdown-option"
						class:selected={option === selected}
						aria-selected={option === selected}
						onclick={() => select(option)}
					>
						{@render children(option)}
					</button>
				</li>
			{/each}
		</ul>
	{/if}
</div>

<style>
	.dropdown {
		position: relative;
	}

	.dropdown-toggle {
		display: inline-flex;
		align-items: center;
		gap: 0.4rem;
		padding: 0.15rem 0.4rem;
		background: #fff;
		border: 1px solid #d1d5db;
		border-radius: 0.25rem;
		cursor: pointer;
		font-family: inherit;
		font-size: inherit;
	}

	.dropdown-toggle:hover {
		border-color: #9ca3af;
	}

	.caret {
		font-size: 1.5em;
		color: #6b7280;
	}

	.dropdown-menu {
		position: absolute;
		top: calc(100% + 4px);
		right: 0;
		z-index: 10;
		min-width: 0;
		width: 100%;
		list-style: none;
		margin: 0;
		padding: 0.25rem;
		background: #f3f4f6;
		border: 1px solid #d1d5db;
		border-radius: 0.25rem;
		box-shadow: 0 4px 12px rgba(0, 0, 0, 0.15);
	}

	.dropdown-option {
		display: flex;
		align-items: center;
		gap: 0.4rem;
		width: 100%;
		padding: 0.3rem 0.5rem;
		background: transparent;
		border: none;
		border-radius: 0.25rem;
		cursor: pointer;
		font-family: inherit;
		font-size: inherit;
		text-align: left;
		white-space: nowrap;
		overflow: hidden;
		text-overflow: ellipsis;
	}

	.dropdown-option:hover {
		background: #e5e7eb;
	}

	.dropdown-option.selected {
		background: #dbeafe;
	}
</style>
