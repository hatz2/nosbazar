<script lang="ts">
	import type { EnrichedSearchResult } from '$lib/types/search';
	import { lang } from '$lib/stores/lang.svelte';
	import { ConstStringKey } from '$lib/types/constStringKeys';
	import { get_const_string } from '$lib/services/constStringService.svelte';
	import { formatString, get_element } from '$lib/utils/format';
	import { isPartnerSpecialistItem } from '$lib/types/search';
	import { skillService } from '$lib/services/skillService';
	import { PartnerSkillLevels } from '$lib/types/enums';

	type Props = {
		item: EnrichedSearchResult;
	};

	let { item }: Props = $props();
</script>

{#if isPartnerSpecialistItem(item.data)}
	{#if item.data.has_partner_sp_inside}
		<p class="light-orange">
			{item.contained_item_static_data?.name[lang.current]}
			{get_const_string(ConstStringKey.InCustody)}<br />
			{get_element(item)}<br />
		</p>

		<p class="light-orange">
			{formatString(
				get_const_string(ConstStringKey.PartnerUpgradeLevel),
				item.data.upgrade_level
			)}<br />
			{formatString(
				get_const_string(ConstStringKey.PartnerAttackBonus),
				20 * item.data.attack_bonus
			)} (+ {item.data.attack_bonus})<br />
			{formatString(
				get_const_string(ConstStringKey.PartnerDefenceBonus),
				10 * item.data.defence_bonus
			)} (+ {item.data.defence_bonus})<br />
			{formatString(
				get_const_string(ConstStringKey.PartnerCritReduction),
				item.data.crit_reduction_bonus
			)} (+ {item.data.crit_reduction_bonus})<br />
			{formatString(
				get_const_string(ConstStringKey.PartnerHpMpBonus),
				300 * item.data.hp_mp_bonus
			)} (+
			{item.data.hp_mp_bonus})<br />

			{formatString(
				get_const_string(ConstStringKey.PartnerFireResBonus),
				item.data.fire_res_bonus
			)} (+
			{item.data.fire_res_bonus})<br />

			{formatString(
				get_const_string(ConstStringKey.PartnerWaterResBonus),
				item.data.water_res_bonus
			)} (+
			{item.data.water_res_bonus})<br />

			{formatString(
				get_const_string(ConstStringKey.PartnerLightResBonus),
				item.data.light_res_bonus
			)} (+
			{item.data.light_res_bonus})<br />

			{formatString(
				get_const_string(ConstStringKey.PartnerShadowResBonus),
				item.data.shadow_res_bonus
			)} (+
			{item.data.shadow_res_bonus})<br />
		</p>

		<p class="light-orange">
			{get_const_string(ConstStringKey.PartnerSkill)}<br />
			{#each item.data.skills as skill, i (i)}
				{skillService.getSync(skill.vnum)?.name[lang.current] ?? `Skill ${skill.vnum}`} : {PartnerSkillLevels[
					skill.grade
				] ?? skill.grade}
				{get_const_string(ConstStringKey.PartnerSkillLevel)}<br />
			{/each}
		</p>
	{:else}
		<p class="light-orange">
			{get_const_string(ConstStringKey.NotUsed)}<br />
		</p>
	{/if}
{/if}

<style>
	.light-orange {
		color: #f5c478;
	}
</style>
