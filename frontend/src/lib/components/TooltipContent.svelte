<script lang="ts">
	import { get_const_string } from '$lib/services/constStringService.svelte';
	import { lang } from '$lib/stores/lang.svelte';
	import { ConstStringKey } from '$lib/types/constStringKeys';
	import { RequiredClass } from '$lib/types/enums';
	import { getCurrentShellLevel, type EnrichedSearchResult } from '$lib/types/search';
	import {
		get_item_type_string,
		get_required_class_string,
		RarityLevelColor
	} from '$lib/utils/format';

	type Props = {
		item: EnrichedSearchResult;
	};

	let { item }: Props = $props();

	function getItemNameColor(item: EnrichedSearchResult): string {
		if ('rare' in item.data && item.data.rare !== 0) {
			return RarityLevelColor[item.data.rare];
		} else {
			return 'white';
		}
	}

	let shell_level = $derived(getCurrentShellLevel(item));

	let item_type_name = $derived(get_item_type_string(item));
</script>

<span>
	<span style="color: {getItemNameColor(item)}"
		>{item.item_name[lang.current]}
		{#if 'upgrade' in item.data && item.data.upgrade !== 0}
			+{item.data.upgrade}
		{/if}
	</span>
	<br />

	{#if 'required_level' in item.data && item.data.required_level}
		<span style="color: white"> </span>
		{item.data.required_level}{#if item.static_data?.flags.is_champion_equip}
			{get_const_string(ConstStringKey.CLv)}
		{:else}
			{get_const_string(ConstStringKey.Lv)}
		{/if}

		{#if item_type_name}
			<span style="color: white">{item_type_name}</span>
		{/if}
		<br />
	{/if}

	{#if 'owner_id' in item.data && item.data.owner_id !== 0}
		<span style="color: #FF5061">{get_const_string(ConstStringKey.Soulbound)}</span>
		<br />
	{/if}

	{#if 'shells' in item.data && item.data.shells.length}
		<span style="color: #FFBB00"
			>{item.data.shells.length} {get_const_string(ConstStringKey.ShellEffects)}</span
		>
		<br />
	{/if}

	{#if 'runes' in item.data && item.data.runes.length}
		<span class="green"
			>{item.data.runes.length} {get_const_string(ConstStringKey.RuneEffects)}</span
		><br />
	{/if}

	{#if shell_level}
		<span class="pink">{get_const_string(ConstStringKey.CurrentShellLevel)} ({shell_level}/8)</span>
		<br />
	{/if}

	{#if 'display_runes_count' in item.data && item.data.display_runes_count}
		<span class="green"
			>{get_const_string(ConstStringKey.CarveRunes)} ({item.data.display_runes_count}/21)</span
		><br />
	{/if}

	{#if item.static_data?.required_class != RequiredClass.All && item.static_data?.required_class != RequiredClass.AllButAdventurer}
		<span style="color: white">
			{get_required_class_string(item.static_data?.required_class || null)}
		</span>
		<br />
	{/if}
</span>

<style>
	.green {
		color: #47c83e;
	}

	.pink {
		color: #f361a6;
	}
</style>
