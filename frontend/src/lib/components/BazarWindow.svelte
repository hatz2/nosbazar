<script lang="ts">
	import { BazarCategory } from '$lib/types/enums';
	import type { SearchResult, EnrichedSearchResult } from '$lib/types/search';
	import { itemService } from '$lib/services/itemService';
	import BlueButton from './BlueButton.svelte';
	import Category from './Category.svelte';
	// import Draggable from './Draggable.svelte';
	import LevelCategory from './LevelCategory.svelte';
	import RarityLevelCategory from './RarityLevelCategory.svelte';
	import SubCategory from './SubCategory.svelte';
	import TitleBar from './TitleBar.svelte';
	import Toolbar from './Toolbar.svelte';
	import UpgradeLevelCategory from './UpgradeLevelCategory.svelte';
	import ResultsTable from './ResultsTable.svelte';
	import ItemDetailWindow from './ItemDetailWindow.svelte';
	import SortByFilter from './SortByFilter.svelte';

	type OpenWindow = {
		item: EnrichedSearchResult;
		left: number;
		top: number;
		id: number;
	};

	let { server = $bindable(1) } = $props();
	let category = $state(BazarCategory.All);
	let subCategory = $state(0);
	let level = $state(0);
	let rarityLevel = $state(0);
	let upgradeLevel = $state(0);
	let order = $state(0);
	let results = $state<EnrichedSearchResult[]>([]);
	let openWindows = $state<OpenWindow[]>([]);

	// Reset category-dependent filters when category changes
	$effect(() => {
		void category;
		subCategory = 0;
		level = 0;
		rarityLevel = 0;
		upgradeLevel = 0;
	});

	let nextWindowId = $state(0);

	async function on_search_clicked() {
		try {
			const response = await fetch('http://localhost:8080/search', {
				method: 'POST',
				headers: { 'Content-Type': 'application/json' },
				body: JSON.stringify({
					server,
					filters: {
						index: 0,
						category,
						sub_category: subCategory,
						level,
						rare: rarityLevel,
						upgrade: upgradeLevel,
						order,
						vnum: []
					}
				})
			});
			const data = await response.json();
			const rawItems = (data.items ?? []) as SearchResult[];

			if (rawItems.length > 0) {
				const vnums = rawItems.map((item: SearchResult) => item.item_vnum);
				await itemService.fetchMany(vnums);

				// Enrich items with names from the cache synchronously for an atomic update
				results = rawItems.map((item: SearchResult): EnrichedSearchResult => {
					const staticData = itemService.getSync(item.item_vnum);
					return {
						...item,
						item_name: staticData?.name || { UK: item.item_vnum.toString() },
						icon_id: staticData?.icon_id,
						static_data: staticData
					};
				});
			} else {
				results = [];
			}
		} catch (e) {
			console.error('Search failed', e);
			results = [];
		}
	}
</script>

<!-- <Draggable left={200} top={200}> -->
<div class="content">
	<TitleBar title="NosBazaar"></TitleBar>
	<Toolbar>
		<span>Name</span>
		<span>Category</span>
		<span>Subcategory</span>
		<span></span>

		<input type="text" />
		<Category bind:value={category}></Category>
		<SubCategory bind:value={subCategory} {category}></SubCategory>
		<BlueButton text="Search" onclick={on_search_clicked}></BlueButton>

		<span>Level</span>
		<span>Rarity Level</span>
		<span>Upgrade Level</span>
		<span>Sort by</span>

		<LevelCategory bind:value={level} {category}></LevelCategory>
		<RarityLevelCategory bind:value={rarityLevel} {category}></RarityLevelCategory>
		<UpgradeLevelCategory bind:value={upgradeLevel} {category}></UpgradeLevelCategory>
		<SortByFilter bind:value={order}></SortByFilter>
	</Toolbar>

	<ResultsTable
		{results}
		onContextMenu={(item, e) => {
			openWindows = [
				...openWindows,
				{
					item,
					left: e.pageX + 35,
					top: e.pageY - 20,
					id: nextWindowId++
				}
			];
		}}
	/>
</div>

{#each openWindows as window (window.id)}
	<ItemDetailWindow
		item={window.item}
		left={window.left}
		top={window.top}
		onclose={() => {
			openWindows = openWindows.filter((w) => w.id !== window.id);
		}}
	/>
{/each}

<svelte:window
	onkeydown={(e) => {
		if (e.key === 'Escape') {
			openWindows = [];
		}
	}}
/>

<style>
	.content {
		margin-top: 0;
		padding-top: 0;
		border: 1px solid gray;
		max-width: 800px;
	}
</style>
