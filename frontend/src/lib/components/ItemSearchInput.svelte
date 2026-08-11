<script lang="ts">
	import ItemIcon from './ItemIcon.svelte';
	import { lang } from '$lib/stores/lang.svelte';
	import { itemNameSearchService, type ItemNameMatch } from '$lib/services/itemNameSearchService';

	type Props = {
		query?: string;
		onsearch?: () => void;
	};

	const SUGGESTION_MIN_LENGTH = 2;
	const SUGGESTION_DEBOUNCE_MS = 250;

	let { query = $bindable(''), onsearch = () => {} }: Props = $props();
	let suggestions = $state<ItemNameMatch[]>([]);
	let activeIndex = $state(-1);

	let skipNextSuggestionsRun = false;

	$effect(() => {
		const text = query.trim();
		const currentLang = lang.current;

		if (skipNextSuggestionsRun) {
			skipNextSuggestionsRun = false;
			return;
		}

		suggestions = [];
		activeIndex = -1;

		if (text.length < SUGGESTION_MIN_LENGTH) {
			return;
		}

		const controller = new AbortController();
		const timer = setTimeout(async () => {
			try {
				const matches = await itemNameSearchService.searchItemNames(
					text,
					currentLang,
					controller.signal
				);
				if (!controller.signal.aborted && text === query.trim()) {
					suggestions = matches;
				}
			} catch {
				// request aborted or failed; keep the list empty
			}
		}, SUGGESTION_DEBOUNCE_MS);

		return () => {
			clearTimeout(timer);
			controller.abort();
		};
	});

	function select_suggestion(match: ItemNameMatch) {
		skipNextSuggestionsRun = true;
		query = match.name;
		suggestions = [];
		activeIndex = -1;
	}

	function on_keydown(e: KeyboardEvent) {
		if (e.key === 'ArrowDown') {
			e.preventDefault();
			activeIndex = Math.min(activeIndex + 1, suggestions.length - 1);
		} else if (e.key === 'ArrowUp') {
			e.preventDefault();
			activeIndex = Math.max(activeIndex - 1, 0);
		} else if (e.key === 'Enter') {
			if (activeIndex >= 0 && suggestions[activeIndex]) {
				select_suggestion(suggestions[activeIndex]);
			} else {
				onsearch();
			}
		} else if (e.key === 'Escape') {
			suggestions = [];
			activeIndex = -1;
		}
	}

	function on_window_click(e: MouseEvent) {
		if (e.target instanceof Element && !e.target.closest('.item-search-input')) {
			suggestions = [];
			activeIndex = -1;
		}
	}
</script>

<div class="item-search-input">
	<input
		type="text"
		bind:value={query}
		spellcheck="false"
		autocomplete="off"
		onkeydown={on_keydown}
	/>
	{#if suggestions.length > 0}
		<ul class="suggestion-list" role="listbox">
			{#each suggestions as match, index (match.vnum)}
				<li
					role="option"
					aria-selected={index === activeIndex}
					class:active={index === activeIndex}
					onmouseenter={() => (activeIndex = index)}
					onkeydown={(e) => {
						if (e.key === 'Enter' || e.key === ' ') {
							e.preventDefault();
							select_suggestion(match);
						}
					}}
					onclick={() => select_suggestion(match)}
				>
					<ItemIcon iconId={match.icon_id} />
					<span class="suggestion-name">{match.name}</span>
				</li>
			{/each}
		</ul>
	{/if}
</div>

<svelte:window onclick={on_window_click} />

<style>
	.item-search-input {
		position: relative;
	}

	input {
		width: 100%;
		box-sizing: border-box;
		background-color: #111517;
		border: 1px solid #606467;
		border-radius: 4px;
		color: white;
		padding: 5px;
		caret-color: #6888dd;
	}

	input:focus {
		outline: none;
		background-color: #111517;
		border: 1px solid #606467;
		border-radius: 4px;
		color: white;
	}

	.suggestion-list {
		position: absolute;
		top: 100%;
		left: 0;
		right: 0;
		z-index: 100;
		margin: 2px 0 0;
		padding: 0;
		list-style: none;
		background-color: #111517;
		border: 1px solid #606467;
		border-radius: 4px;
		max-height: 240px;
		overflow-y: auto;
	}

	.suggestion-list li {
		display: flex;
		align-items: center;
		gap: 6px;
		padding: 3px 6px;
		cursor: pointer;
		white-space: nowrap;
		overflow: hidden;
	}

	.suggestion-list li:hover,
	.suggestion-list li.active {
		background-color: #21272c;
	}

	.suggestion-name {
		min-width: 0;
		overflow: hidden;
		text-overflow: ellipsis;
		color: white;
	}
</style>
