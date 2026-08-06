<script lang="ts">
	import { BazarCategory } from '$lib/types/enums';
	import {
		type SearchResult,
		type EnrichedSearchResult,
		isSpecialist,
		isPetBeadItem,
		isPartnerBeadItem,
		isPartnerSpecialistItem,
		isMountBeadItem
	} from '$lib/types/search';
	import { itemService } from '$lib/services/itemService';
	import { monsterService } from '$lib/services/monsterService';
	import { skillService } from '$lib/services/skillService';
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
	import { onMount } from 'svelte';
	import { ConstStringKey } from '$lib/types/constStringKeys';
	import { fetchConstString, get_const_string } from '$lib/services/constStringService.svelte';
	import { BAZAR_PAGES_PER_SEARCH } from '$lib/constants';

	type OpenWindow = {
		item: EnrichedSearchResult;
		left: number;
		top: number;
		id: number;
	};

	type Anchor = 'left' | 'center' | 'right';

	const ANCHOR_STORAGE_KEY = 'bazar-anchor';

	function readAnchorFromStorage(): Anchor {
		if (typeof localStorage === 'undefined') {
			return 'center';
		}
		const value = localStorage.getItem(ANCHOR_STORAGE_KEY);
		return value === 'left' || value === 'right' || value === 'center' ? value : 'center';
	}

	let { server = $bindable(1) } = $props();
	let category = $state(BazarCategory.All);
	let subCategory = $state(0);
	let level = $state(0);
	let rarityLevel = $state(0);
	let upgradeLevel = $state(0);
	let order = $state(0);
	let visualIndex = $state(1);
	let resultsPageIndex = $state(0);
	let isCooldown = $state(false);
	let isSearching = $state(false);
	let anchor = $state<Anchor>(readAnchorFromStorage());

	$effect(() => {
		localStorage.setItem(ANCHOR_STORAGE_KEY, anchor);
	});
	let results = $state<EnrichedSearchResult[]>([]);
	let openWindows = $state<OpenWindow[]>([]);

	function getPageIndex() {
		return Math.floor((visualIndex - 1) / BAZAR_PAGES_PER_SEARCH);
	}

	// Reset category-dependent filters when category changes
	$effect(() => {
		void category;
		subCategory = 0;
		level = 0;
		rarityLevel = 0;
		upgradeLevel = 0;
		visualIndex = 1;
	});

	let nextWindowId = $state(0);

	onMount(() => {
		fetchConstString(ConstStringKey.Name);
		fetchConstString(ConstStringKey.Level);
		fetchConstString(ConstStringKey.RarityLevel);
		fetchConstString(ConstStringKey.UpgradeLevel);
		fetchConstString(ConstStringKey.SortBy);
		fetchConstString(ConstStringKey.NosBazar);
		fetchConstString(ConstStringKey.Category);
	});

	async function do_search(force = false) {
		const index = getPageIndex();
		if (!force && index === resultsPageIndex) {
			return;
		}
		if (isCooldown) {
			return;
		}
		isCooldown = true;
		isSearching = true;
		results = [];
		try {
			const response = await fetch('http://localhost:8080/search', {
				method: 'POST',
				headers: { 'Content-Type': 'application/json' },
				body: JSON.stringify({
					server,
					filters: {
						index,
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
			console.log(data);
			const rawItems = (data.items ?? []) as SearchResult[];

			if (rawItems.length > 0) {
				const allVnums = rawItems.map((item: SearchResult) => item.item_vnum);
				const petVnums: number[] = [];
				const skillVnums: number[] = [];
				for (const item of rawItems) {
					if (isSpecialist(item.data) && item.data.contains_sp) {
						allVnums.push(item.data.vnum);
					}
					if (isPetBeadItem(item.data) && item.data.has_pet_inside) {
						petVnums.push(item.data.pet_vnum);
					}
					if (isPartnerBeadItem(item.data) && item.data.has_partner_inside) {
						petVnums.push(item.data.partner_vnum);
					}
					if (isPartnerSpecialistItem(item.data) && item.data.has_partner_sp_inside) {
						allVnums.push(item.data.partner_sp_vnum);
						for (const skill of item.data.skills) {
							skillVnums.push(skill.vnum);
						}
					}
					if (isMountBeadItem(item.data) && item.data.has_mount_inside) {
						allVnums.push(item.data.mount_vnum);
					}
				}
				await Promise.all([
					itemService.fetchMany(allVnums),
					monsterService.fetchMany(petVnums),
					skillService.fetchMany(skillVnums)
				]);

				results = rawItems.map((item: SearchResult): EnrichedSearchResult => {
					const staticData = itemService.getSync(item.item_vnum);
					const containsData =
						(isSpecialist(item.data) && item.data.contains_sp) ||
						(isPartnerSpecialistItem(item.data) && item.data.has_partner_sp_inside) ||
						(isPetBeadItem(item.data) && item.data.has_pet_inside);
					const containedData = containsData
						? (itemService.getSync(
								isPartnerSpecialistItem(item.data) ? item.data.partner_sp_vnum : item.data.vnum
							) ?? undefined)
						: undefined;
					const containedMonsterData =
						isPetBeadItem(item.data) && item.data.has_pet_inside
							? (monsterService.getSync(item.data.pet_vnum) ?? undefined)
							: isPartnerBeadItem(item.data) && item.data.has_partner_inside
								? (monsterService.getSync(item.data.partner_vnum) ?? undefined)
								: undefined;
					const containedMountItemData =
						isMountBeadItem(item.data) && item.data.has_mount_inside
							? (itemService.getSync(item.data.mount_vnum) ?? undefined)
							: undefined;
					return {
						...item,
						item_name: staticData?.name || { UK: item.item_vnum.toString() },
						static_data: staticData,
						contained_item_static_data: containedData,
						contained_monster_static_data: containedMonsterData,
						contained_mount_item_static_data: containedMountItemData
					};
				});
			} else {
				results = [];
			}
			resultsPageIndex = index;
		} catch (e) {
			console.error('Search failed', e);
			results = [];
		} finally {
			isSearching = false;
			setTimeout(() => {
				isCooldown = false;
			}, 3000);
		}
	}

	async function on_search_clicked() {
		visualIndex = 1;
		await do_search(true);
	}

	function go_next_page() {
		const newVisual = visualIndex + 1;
		const newIndex = Math.floor((newVisual - 1) / BAZAR_PAGES_PER_SEARCH);
		if (newIndex !== resultsPageIndex && isCooldown) {
			return;
		}
		visualIndex = newVisual;
		if (newIndex !== resultsPageIndex) {
			do_search();
		}
	}

	function go_previous_page() {
		if (visualIndex <= 1) {
			return;
		}
		const newVisual = visualIndex - 1;
		const newIndex = Math.floor((newVisual - 1) / BAZAR_PAGES_PER_SEARCH);
		if (newIndex !== resultsPageIndex && isCooldown) {
			return;
		}
		visualIndex = newVisual;
		if (newIndex !== resultsPageIndex) {
			do_search();
		}
	}
</script>

<!-- <Draggable left={200} top={200}> -->
<div
	class="window-host"
	class:anchor-left={anchor === 'left'}
	class:anchor-right={anchor === 'right'}
>
	<div class="content">
		<TitleBar title={get_const_string(ConstStringKey.NosBazar)}>
			<div class="anchor-group">
				<button
					class="anchor-btn"
					class:active={anchor === 'left'}
					title="Left"
					onclick={() => (anchor = 'left')}>◀</button
				>
				<button
					class="anchor-btn"
					class:active={anchor === 'center'}
					title="Center"
					onclick={() => (anchor = 'center')}>◉</button
				>
				<button
					class="anchor-btn"
					class:active={anchor === 'right'}
					title="Right"
					onclick={() => (anchor = 'right')}>▶</button
				>
			</div>
		</TitleBar>

		<div class="content-body">
			<Toolbar>
				<span class="category_label">{get_const_string(ConstStringKey.Name)}</span>
				<span class="category_label">{get_const_string(ConstStringKey.Category)}</span>
				<span></span>
				<span></span>

				<input type="text" />
				<Category bind:value={category}></Category>
				<SubCategory bind:value={subCategory} {category}></SubCategory>
				<BlueButton
					text={get_const_string(ConstStringKey.Search)}
					onclick={on_search_clicked}
					disabled={isCooldown}
				></BlueButton>

				<span class="category_label">{get_const_string(ConstStringKey.Level)}</span>
				<span class="category_label">{get_const_string(ConstStringKey.RarityLevel)}</span>
				<span class="category_label">{get_const_string(ConstStringKey.UpgradeLevel)}</span>
				<span class="category_label">{get_const_string(ConstStringKey.SortBy)}</span>

				<LevelCategory bind:value={level} {category}></LevelCategory>
				<RarityLevelCategory bind:value={rarityLevel} {category}></RarityLevelCategory>
				<UpgradeLevelCategory bind:value={upgradeLevel} {category}></UpgradeLevelCategory>
				<SortByFilter bind:value={order}></SortByFilter>
			</Toolbar>

			<ResultsTable
				{results}
				{visualIndex}
				{isSearching}
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

			<div class="pagination gray-panel">
				<button onclick={go_previous_page}>◀</button>
				<input
					type="number"
					bind:value={visualIndex}
					min="1"
					onchange={() => {
						if (getPageIndex() !== resultsPageIndex && isCooldown) {
							visualIndex = resultsPageIndex * BAZAR_PAGES_PER_SEARCH + 1;
							return;
						}
						if (getPageIndex() !== resultsPageIndex) {
							do_search();
						}
					}}
					class="page-input"
				/>
				<button onclick={go_next_page}>▶</button>
			</div>
		</div>
	</div>
</div>

{#each openWindows as window (window.id)}
	<ItemDetailWindow
		item={window.item}
		bind:left={window.left}
		bind:top={window.top}
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
	.window-host {
		display: flex;
		width: 100%;
		justify-content: center;
	}

	.window-host.anchor-left {
		justify-content: flex-start;
	}

	.window-host.anchor-right {
		justify-content: flex-end;
	}

	.content {
		padding-top: 0;
		max-width: 725px;
		background-color: #373d42;
		box-shadow: 0 0 10px 2px #373d42;
		margin: 0 10px;
	}

	.content-body {
		padding: 5px;
	}

	.pagination {
		display: flex;
		justify-content: center;
		align-items: center;
		gap: 12px;
		padding: 8px 0;
	}

	.pagination .page-input {
		width: 50px;
		text-align: center;
	}

	.category_label {
		color: white;
		text-shadow:
			-1px -1px 0 black,
			1px -1px 0 black,
			-1px 1px 0 black,
			1px 1px 0 black;
	}

	input {
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

	/* Chrome, Edge, Safari, Opera */
	input[type='number']::-webkit-outer-spin-button,
	input[type='number']::-webkit-inner-spin-button {
		-webkit-appearance: none;
		margin: 0;
	}

	/* Firefox */
	input[type='number'] {
		appearance: textfield;
		-moz-appearance: textfield;
	}

	button {
		background-color: transparent;
		border: none;
		color: #6ebce2;
		text-shadow:
			2px 2px 2px #001d41,
			-2px 2px 2px #001d41;
		font-size: x-large;
	}

	button:hover {
		background-color: transparent;
		border: none;
		color: #a5d6ee;
	}

	button:disabled {
		color: #3e667b;
	}

	button:active {
		color: #487891;
	}

	.anchor-group {
		display: flex;
		gap: 4px;
	}

	.anchor-btn {
		font-size: 14px;
		line-height: 1;
		padding: 1px 4px;
		border: 1px solid transparent;
		border-radius: 3px;
	}

	.anchor-btn:hover {
		border: 1px solid #7c7c7c;
		/* border-radius: 3px; */
	}

	.anchor-btn.active {
		color: #dcecff;
		border: 1px solid #6ebce2;
	}
</style>
