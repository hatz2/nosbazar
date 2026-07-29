<script lang="ts">
	import type { EnrichedSearchResult } from '$lib/types/search';
	import { ConstStringKey } from '$lib/types/constStringKeys';
	import { get_const_string } from '$lib/services/constStringService.svelte';
	import { isResistances } from '$lib/types/search';

	type Props = {
		item: EnrichedSearchResult;
	};

	let { item }: Props = $props();
</script>

<!-- Damage section -->
<p class="light-orange">
	{#if 'min_dmg' in item.data && 'max_dmg' in item.data}
		{get_const_string(ConstStringKey.Damage)}: {item.data.min_dmg}~{item.data.max_dmg}<br />
		{#if 'hit_rate' in item.data}
			{get_const_string(ConstStringKey.HitRate)}: {item.data.hit_rate}<br />
		{:else if 'concentration' in item.data}
			{get_const_string(ConstStringKey.Concentration)}: {item.data.concentration}<br />
		{/if}
		{#if 'critical_rate' in item.data && 'critical_dmg' in item.data}
			{item.data.critical_rate}% {get_const_string(ConstStringKey.CriticalChance)}
			{item.data.critical_dmg}% {get_const_string(ConstStringKey.Critical)}<br />
		{/if}
	{/if}

	<!-- Armour stats -->
	{#if 'melee_defence' in item.data}
		{get_const_string(ConstStringKey.Melee)}
		{get_const_string(ConstStringKey.Defence)}: {item.data.melee_defence}<br />
		{get_const_string(ConstStringKey.Ranged)}
		{get_const_string(ConstStringKey.Defence)}: {item.data.ranged_defence}<br />
		{get_const_string(ConstStringKey.Magic)}
		{get_const_string(ConstStringKey.Defence)}: {item.data.magic_defence}<br />
		{get_const_string(ConstStringKey.Dodge)}: {item.data.dodge}
	{/if}
</p>

<!-- Resistances for gloves/shoes -->
{#if isResistances(item.data)}
	<p class="light-orange">
		{get_const_string(ConstStringKey.Fire)} Element Resistance: {item.data.fire_res}%<br />
		{get_const_string(ConstStringKey.Water)} Element Resistance: {item.data.water_res}%<br />
		{get_const_string(ConstStringKey.Light)} Element Resistance: {item.data.light_res}%<br />
		{get_const_string(ConstStringKey.Shadow)} Element Resistance: {item.data.shadow_res}%
	</p>
{/if}

<style>
	.light-orange {
		color: #f5c478;
	}
</style>
