<script lang="ts">
	import type { EnrichedSearchResult } from '$lib/types/search';
	import { lang } from '$lib/stores/lang.svelte';
	import { ConstStringKey } from '$lib/types/constStringKeys';
	import { get_const_string } from '$lib/services/constStringService.svelte';
	import { get_star_pets } from '$lib/utils/format';
	import { isPetBeadItem } from '$lib/types/search';

	type Props = {
		item: EnrichedSearchResult;
	};

	let { item }: Props = $props();
</script>

{#if isPetBeadItem(item.data)}
	<p class="light-orange">
		{#if item.data.has_pet_inside}
			{item.contained_monster_static_data?.name[lang.current]}
			{get_const_string(ConstStringKey.InCustody)}<br />
			{get_const_string(ConstStringKey.Level)}: {item.data.level}<br />
			{get_const_string(ConstStringKey.ExperiencePoints)}: {item.data.level_percentage}%<br />
			{get_const_string(ConstStringKey.Rating)}: {get_star_pets(item)}<br />
			{get_const_string(ConstStringKey.AttackLevel)}: {item.data.attack_level}<br />
			{get_const_string(ConstStringKey.DefenceStat)}
			{get_const_string(ConstStringKey.Level)}: {item.data.defence_level}<br />
		{:else}
			{get_const_string(ConstStringKey.NotUsed)}<br />
		{/if}
	</p>
{/if}

<style>
	.light-orange {
		color: #f5c478;
	}
</style>
