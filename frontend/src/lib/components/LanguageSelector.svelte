<script lang="ts">
	import flagCZ from '$lib/assets/flag-cz.svg';
	import flagDE from '$lib/assets/flag-de.svg';
	import flagES from '$lib/assets/flag-es.svg';
	import flagFR from '$lib/assets/flag-fr.svg';
	import flagIT from '$lib/assets/flag-it.svg';
	import flagPL from '$lib/assets/flag-pl.svg';
	import flagTR from '$lib/assets/flag-tr.svg';
	import flagUK from '$lib/assets/flag-uk.svg';
	import { lang, type LanguageCode } from '$lib/stores/lang.svelte';

	type LanguageOption = {
		code: LanguageCode;
		name: string;
		flag: string;
	};

	const options: LanguageOption[] = [
		{ code: 'ES', name: 'Spanish', flag: flagES },
		{ code: 'UK', name: 'English', flag: flagUK },
		{ code: 'FR', name: 'French', flag: flagFR },
		{ code: 'DE', name: 'German', flag: flagDE },
		{ code: 'TR', name: 'Turkish', flag: flagTR },
		{ code: 'IT', name: 'Italian', flag: flagIT },
		{ code: 'PL', name: 'Polish', flag: flagPL },
		{ code: 'CZ', name: 'Czech', flag: flagCZ }
	];

	let isOpen = $state(false);
	let container = $state<HTMLDivElement>();

	let current = $derived(options.find((o) => o.code === lang.current) ?? options[1]);

	function toggle() {
		isOpen = !isOpen;
	}

	function select(code: LanguageCode) {
		lang.current = code;
		isOpen = false;
	}

	// Sync to localStorage when language changes
	$effect(() => {
		localStorage.setItem('user-lang', lang.current);
	});

	$effect(() => {
		if (!isOpen) {
			return;
		}
		function onPointerDown(event: PointerEvent) {
			if (container && !container.contains(event.target as Node)) {
				isOpen = false;
			}
		}
		function onKeyDown(event: KeyboardEvent) {
			if (event.key === 'Escape') {
				isOpen = false;
			}
		}
		window.addEventListener('pointerdown', onPointerDown);
		window.addEventListener('keydown', onKeyDown);
		return () => {
			window.removeEventListener('pointerdown', onPointerDown);
			window.removeEventListener('keydown', onKeyDown);
		};
	});
</script>

<div class="lang-selector" bind:this={container}>
	<span>Language: </span>
	<div class="dropdown">
		<button
			class="dropdown-button"
			type="button"
			onclick={toggle}
			aria-haspopup="listbox"
			aria-expanded={isOpen}
		>
			<img src={current.flag} alt="" class="flag" />
			<span>{current.name}</span>
			<span class="caret">▾</span>
		</button>
		{#if isOpen}
			<ul class="dropdown-list" role="listbox">
				{#each options as option (option.code)}
					<li>
						<button
							class="dropdown-option"
							class:selected={option.code === lang.current}
							type="button"
							role="option"
							aria-selected={option.code === lang.current}
							onclick={() => select(option.code)}
						>
							<img src={option.flag} alt="" class="flag" />
							<span>{option.name}</span>
						</button>
					</li>
				{/each}
			</ul>
		{/if}
	</div>
</div>

<style>
	.lang-selector {
		display: inline-flex;
		align-items: center;
		gap: 0.5rem;
		font-size: 0.875rem;
		padding: 0.25rem 0.5rem;
		background: #f3f4f6;
		border-radius: 0.25rem;
		border: 1px solid #d1d5db;
	}

	.lang-selector > span {
		font-weight: 500;
	}

	.dropdown {
		position: relative;
	}

	.dropdown-button {
		display: inline-flex;
		align-items: center;
		gap: 0.35rem;
		background: transparent;
		border: none;
		outline: none;
		cursor: pointer;
		font-family: inherit;
		font-size: inherit;
		color: inherit;
	}

	.flag {
		display: block;
		width: 24px;
		height: 16px;
	}

	.caret {
		font-size: 0.7rem;
	}

	.dropdown-list {
		position: absolute;
		top: calc(100% + 4px);
		right: 0;
		z-index: 50;
		list-style: none;
		margin: 0;
		padding: 0.25rem;
		background: #ffffff;
		border: 1px solid #d1d5db;
		border-radius: 0.25rem;
		box-shadow: 0 4px 12px rgba(0, 0, 0, 0.15);
	}

	.dropdown-option {
		display: flex;
		align-items: center;
		gap: 0.5rem;
		width: 100%;
		padding: 0.3rem 0.5rem;
		background: transparent;
		border: none;
		border-radius: 0.2rem;
		cursor: pointer;
		font-family: inherit;
		font-size: inherit;
		color: inherit;
		text-align: left;
		white-space: nowrap;
	}

	.dropdown-option:hover,
	.dropdown-option.selected {
		background: #e5e7eb;
	}
</style>
