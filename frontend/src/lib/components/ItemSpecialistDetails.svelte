<script lang="ts">
	import type { EnrichedSearchResult } from '$lib/types/search';
	import { lang } from '$lib/stores/lang.svelte';
	import { ConstStringKey } from '$lib/types/constStringKeys';
	import { get_const_string } from '$lib/services/constStringService.svelte';
	import { formatString } from '$lib/utils/format';
	import { isSpecialist } from '$lib/types/search';

	type Props = {
		item: EnrichedSearchResult;
	};

	let { item }: Props = $props();
</script>

{#if isSpecialist(item.data)}
	{#if item.data.contains_sp}
		<p class="light-orange">
			{item.contained_item_static_data?.name[lang.current]}
			{get_const_string(ConstStringKey.InCustody)}<br />
			{get_const_string(ConstStringKey.Level)}: {item.data.job_level}<br />
			{get_const_string(ConstStringKey.ExperiencePoints)}: {item.data.level_percentage}%
		</p>
		<p class="light-orange">
			{formatString(
				get_const_string(ConstStringKey.UpgradingGrade),
				item.data.upgrade_level
			)}<br />
			{formatString(
				get_const_string(ConstStringKey.LevelOfPerfection),
				item.data.perfection_level
			)}<br />
			{formatString(
				get_const_string(ConstStringKey.SpAttackStat),
				item.data.attack_points,
				item.data.attack_perf
			)}<br />
			{formatString(
				get_const_string(ConstStringKey.SpDefStat),
				item.data.defence_points,
				item.data.defence_perf
			)}<br />
			{formatString(
				get_const_string(ConstStringKey.SpEleStat),
				item.data.element_points,
				item.data.element_perf
			)}<br />
			{formatString(
				get_const_string(ConstStringKey.SpHpMpStat),
				item.data.hp_points,
				item.data.hp_perf
			)}<br />
			{formatString(
				get_const_string(ConstStringKey.SpFireResStat),
				item.data.fire_res,
				item.data.fire_res_perf
			)}<br />
			{formatString(
				get_const_string(ConstStringKey.SpWaterResStat),
				item.data.water_res,
				item.data.water_res_perf
			)}<br />
			{formatString(
				get_const_string(ConstStringKey.SpLightResStat),
				item.data.light_res,
				item.data.light_res_perf
			)}<br />
			{formatString(
				get_const_string(ConstStringKey.SpShadowResStat),
				item.data.shadow_res,
				item.data.shadow_res_perf
			)}<br />
		</p>
		<p class="light-orange">
			{formatString(
				get_const_string(ConstStringKey.RemainingPoints),
				item.data.remaining_points
			)}
		</p>
	{:else}
		<p class="light-orange">
			{get_const_string(ConstStringKey.NotUsed)}
		</p>
	{/if}
{/if}

<style>
	.light-orange {
		color: #f5c478;
	}
</style>
