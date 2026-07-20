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
	let pageIndex = $state(0);
	let isCooldown = $state(false);
	let results = $state<EnrichedSearchResult[]>([]);
	let openWindows = $state<OpenWindow[]>([]);

	// Reset category-dependent filters when category changes
	$effect(() => {
		void category;
		subCategory = 0;
		level = 0;
		rarityLevel = 0;
		upgradeLevel = 0;
		pageIndex = 0;
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

	async function do_search() {
		if (isCooldown) return;
		isCooldown = true;
		try {
			const response = await fetch('http://localhost:8080/search', {
				method: 'POST',
				headers: { 'Content-Type': 'application/json' },
				body: JSON.stringify({
					server,
					filters: {
						index: pageIndex,
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
			pageIndex = data.page_index ?? pageIndex;
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
								isPartnerSpecialistItem(item.data)
									? item.data.partner_sp_vnum
									: item.data.vnum
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
		} catch (e) {
			console.error('Search failed', e);
			results = [];
		} finally {
			setTimeout(() => {
				isCooldown = false;
			}, 4000);
		}
	}

	async function on_search_clicked() {
		pageIndex = 0;
		await do_search();
	}

	function go_next_page() {
		pageIndex++;
		do_search();
	}

	function go_previous_page() {
		if (pageIndex > 0) pageIndex--;
		do_search();
	}
</script>

<!-- <Draggable left={200} top={200}> -->
<div class="content">
	<TitleBar title={get_const_string(ConstStringKey.NosBazar)}></TitleBar>
	<Toolbar>
		<span>{get_const_string(ConstStringKey.Name)}</span>
		<span>{get_const_string(ConstStringKey.Category)}</span>
		<span></span>
		<span></span>

		<input type="text" />
		<Category bind:value={category}></Category>
		<SubCategory bind:value={subCategory} {category}></SubCategory>
		<BlueButton text="Search" onclick={on_search_clicked} disabled={isCooldown}></BlueButton>

		<span>{get_const_string(ConstStringKey.Level)}</span>
		<span>{get_const_string(ConstStringKey.RarityLevel)}</span>
		<span>{get_const_string(ConstStringKey.UpgradeLevel)}</span>
		<span>{get_const_string(ConstStringKey.SortBy)}</span>

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

	<div class="pagination">
		<BlueButton text="< Previous" onclick={go_previous_page} disabled={isCooldown} />
		<input
			type="number"
			bind:value={pageIndex}
			min="0"
			onchange={() => do_search()}
			class="page-input"
			disabled={isCooldown}
		/>
		<BlueButton text="Next >" onclick={go_next_page} disabled={isCooldown} />
	</div>
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
</style>
