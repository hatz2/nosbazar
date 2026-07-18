<script lang="ts">
	import Draggable from './Draggable.svelte';
	import type { EnrichedSearchResult } from '$lib/types/search';
	import { lang } from '$lib/stores/lang.svelte';
	import { RequiredClass } from '$lib/types/enums';
	import CloseButton from './CloseButton.svelte';
	import { formatCellonOptionString, formatString } from '$lib/utils/format';
	import {
		get_display_icon_id,
		isAccessory,
		isResistances,
		isSpecialist,
		itemHasFlagsToDisplay
	} from '$lib/types/search';
	import { onMount } from 'svelte';
	import { ConstStringKey } from '$lib/types/constStringKeys';
	import { fetchConstString, get_const_string } from '$lib/services/constStringService.svelte';
	import { Element } from '$lib/types/enums';

	type Props = {
		item: EnrichedSearchResult;
		left?: number;
		top?: number;
		onclose?: () => void;
	};

	let { item, left = 0, top = 0, onclose }: Props = $props();

	onMount(() => {
		fetchConstString(ConstStringKey.Lv);
		fetchConstString(ConstStringKey.Level);
		fetchConstString(ConstStringKey.Required);
		fetchConstString(ConstStringKey.Times);
		fetchConstString(ConstStringKey.Damage);
		fetchConstString(ConstStringKey.HitRate);
		fetchConstString(ConstStringKey.Concentration);
		fetchConstString(ConstStringKey.Critical);
		fetchConstString(ConstStringKey.Price);
		fetchConstString(ConstStringKey.Maximum);
		fetchConstString(ConstStringKey.Magic);
		fetchConstString(ConstStringKey.Defence);
		fetchConstString(ConstStringKey.Dodge);
		fetchConstString(ConstStringKey.Melee);
		fetchConstString(ConstStringKey.Ranged);
		fetchConstString(ConstStringKey.Fire);
		fetchConstString(ConstStringKey.Water);
		fetchConstString(ConstStringKey.Light);
		fetchConstString(ConstStringKey.Shadow);
		fetchConstString(ConstStringKey.RarityLevel);
		fetchConstString(ConstStringKey.OnlyForMales);
		fetchConstString(ConstStringKey.OnlyForFemales);
		fetchConstString(ConstStringKey.ChampionLevel);
		fetchConstString(ConstStringKey.CriticalChance);
		fetchConstString(ConstStringKey.Adventurer);
		fetchConstString(ConstStringKey.Swordsman);
		fetchConstString(ConstStringKey.Mage);
		fetchConstString(ConstStringKey.Archer);
		fetchConstString(ConstStringKey.MartialArtist);
		fetchConstString(ConstStringKey.Only);
		fetchConstString(ConstStringKey.Option);
		fetchConstString(ConstStringKey.Piece);
		fetchConstString(ConstStringKey.Sell);
		fetchConstString(ConstStringKey.Trade);
		fetchConstString(ConstStringKey.CellonLevel);
		fetchConstString(ConstStringKey.Drop);
		fetchConstString(ConstStringKey.False);
		fetchConstString(ConstStringKey.Number);
		fetchConstString(ConstStringKey.CellonHpMaxIncrease);
		fetchConstString(ConstStringKey.CellonMpMaxIncrease);
		fetchConstString(ConstStringKey.CellonHpRecovery);
		fetchConstString(ConstStringKey.CellonMpRecovery);
		fetchConstString(ConstStringKey.CellonCritDmgReduction);
		fetchConstString(ConstStringKey.CellonMpConsumptionReduction);
	});

	function get_required_class_string(required_class: number | null): string {
		let result: string = '';

		console.log(required_class);

		if (required_class === null) {
			return result;
		}

		if (required_class == RequiredClass.All) {
			return result;
		}

		// Single class result
		const single_class =
			required_class == RequiredClass.Adventurer ||
			required_class == RequiredClass.Swordsman ||
			required_class == RequiredClass.Archer ||
			required_class == RequiredClass.Mage ||
			required_class == RequiredClass.Martial;

		if (single_class) {
			if (required_class & RequiredClass.Adventurer) {
				result += get_const_string(ConstStringKey.Adventurer);
			}
			if (required_class & RequiredClass.Swordsman) {
				result += get_const_string(ConstStringKey.Swordsman);
			}
			if (required_class & RequiredClass.Archer) {
				result += get_const_string(ConstStringKey.Archer);
			}
			if (required_class & RequiredClass.Mage) {
				result += get_const_string(ConstStringKey.Mage);
			}
			if (required_class & RequiredClass.Martial) {
				result += get_const_string(ConstStringKey.MartialArtist);
			}
		} else {
			if (required_class & RequiredClass.Adventurer) {
				result += `${get_const_string(ConstStringKey.Adventurer)}, `;
			}
			if (required_class & RequiredClass.Swordsman) {
				result += `${get_const_string(ConstStringKey.Swordsman)}, `;
			}
			if (required_class & RequiredClass.Archer) {
				result += `${get_const_string(ConstStringKey.Archer)}, `;
			}
			if (required_class & RequiredClass.Mage) {
				result += `${get_const_string(ConstStringKey.Mage)}, `;
			}
			if (required_class & RequiredClass.Martial) {
				result += get_const_string(ConstStringKey.MartialArtist);
			}
		}

		result += ` ${get_const_string(ConstStringKey.Only)}`;

		return result;
	}

	function get_required_level(item: EnrichedSearchResult): string {
		let result: string = '';

		if ('required_level' in item.data) {
			if (item.static_data?.flags.is_champion_equip) {
				result = `${get_const_string(ConstStringKey.Required)} ${get_const_string(ConstStringKey.ChampionLevel)}: `;
			} else {
				result = `${get_const_string(ConstStringKey.Required)} ${get_const_string(ConstStringKey.Level)}: `;
			}

			result += `${item.data.required_level} ${get_const_string(ConstStringKey.Lv)}`;
		}

		return result;
	}

	function get_element(item: EnrichedSearchResult): string {
		if ('element_type' in item.data) {
			switch (item.data.element_type) {
				case Element.NoElement:
					return get_const_string(ConstStringKey.NoElement);
				case Element.Fire:
					return get_const_string(ConstStringKey.Fire);
				case Element.Water:
					return get_const_string(ConstStringKey.Water);
				case Element.Light:
					return get_const_string(ConstStringKey.Light);
				case Element.Shadow:
					return get_const_string(ConstStringKey.Shadow);
			}
		}
		return '';
	}
</script>

<Draggable {left} {top}>
	<div class="window">
		<div class="window-header">
			<CloseButton onclick={onclose} />
		</div>

		<img
			src="http://localhost:8080/icon/{get_display_icon_id(item)}"
			alt="Item icon"
			style="pointer-events: none; position: absolute; top: 10px; left: 10px;"
		/>
		<!-- Name -->
		<div class="body-text">
			<p class="light-orange">
				{item.item_name[lang.current]}
				{#if 'element_type' in item.data}
					<br />{get_const_string(ConstStringKey.Attribute)}: {get_element(item)}<br />
					{formatString(
						get_const_string(ConstStringKey.StrengthensElementBy),
						item.data.fairy_level_percent
					)}
				{/if}
			</p>

			{#if 'monster_to_level_up' in item.data}
				<p class="white-color">
					{get_const_string(ConstStringKey.MonstersRequiredForNextFairyLevel)}:
					{item.data.monster_to_level_up}
				</p>
			{/if}

			<!-- Gender specific item -->
			{#if item.static_data}
				{#if item.static_data.flags.male_can_wear}
					<p class="white-color">{get_const_string(ConstStringKey.OnlyForFemales)}</p>
				{:else if item.static_data.flags.female_can_wear}
					<p class="white-color">{get_const_string(ConstStringKey.OnlyForMales)}</p>
				{/if}
			{/if}

			<!-- Header -->
			<p class="stat-header">
				{#if item.static_data?.required_class != RequiredClass.All && item.static_data?.required_class != RequiredClass.AllButAdventurer}
					{get_required_class_string(item.static_data?.required_class || null)}<br />
				{/if}
				{#if 'required_level' in item.data}
					{get_required_level(item)}<br />
				{/if}
				{#if 'remaining_time_in_secs' in item.data}
					<span class="light-orange">
						{get_const_string(ConstStringKey.AmuletRemaining)}
						{get_const_string(ConstStringKey.AmuletHours)}: {item.data.remaining_time_in_secs}
						{get_const_string(ConstStringKey.Seconds)}
					</span>
				{/if}
				{#if 'rare' in item.data}
					{get_const_string(ConstStringKey.RarityLevel)}: {item.data.rare}<br />
				{/if}
				{#if 'sum_level' in item.data}
					<span class="light-orange"
						>{get_const_string(ConstStringKey.Times)}: {item.data.sum_level}</span
					><br />
				{/if}
				{#if isAccessory(item.data)}
					<span class="light-orange"
						>{get_const_string(ConstStringKey.Maximum)}
						{get_const_string(ConstStringKey.Option)}
						{get_const_string(ConstStringKey.CellonLevel)}: {item.data.max_option_level}</span
					><br />
					<span class="light-orange"
						>{get_const_string(ConstStringKey.Maximum)}
						{get_const_string(ConstStringKey.Option)}
						{get_const_string(ConstStringKey.Piece)}
						{get_const_string(ConstStringKey.Number)}: {item.data.options.length}/{item.data
							.max_option_count}</span
					><br />
				{/if}
			</p>

			<!-- Specialist info -->
			{#if isSpecialist(item.data)}
				<p class="light-orange">
					<!-- TODO: Add SP name -->
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
			{/if}

			<!-- Flags section -->
			{#if item.static_data}
				{#if itemHasFlagsToDisplay(item.static_data.flags)}
					<p class="light-orange">
						{#if item.static_data.flags.no_dropping}
							[{get_const_string(ConstStringKey.Drop)}] {get_const_string(ConstStringKey.False)}<br
							/>
						{/if}
						{#if item.static_data.flags.no_selling}
							[{get_const_string(ConstStringKey.Sell)}] {get_const_string(ConstStringKey.False)}<br
							/>
						{/if}
						{#if item.static_data.flags.no_trading}
							[{get_const_string(ConstStringKey.Trade)}] {get_const_string(ConstStringKey.False)}<br
							/>
						{/if}
					</p>
				{/if}
			{/if}

			<!-- Damage section -->
			<p class="light-orange">
				<!-- Dmg stats -->
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

			<!-- Price -->
			{#if 'price' in item.data}
				<p class="price">
					{get_const_string(ConstStringKey.Price)}: {item.data.price.toLocaleString('es-ES')}
				</p>
			{/if}

			<!-- Cell options -->
			{#if isAccessory(item.data)}
				{#if item.data.options.length > 0}
					<p class="light-orange">
						{#each item.data.options as option, i (i)}
							{formatCellonOptionString(option)}<br />
							<!-- {option.level}{get_const_string(ConstStringKey.Lv)}
						{get_const_string(CellonOptionsConstStrings[option.vnum])}
						{option.value}<br /> -->
						{/each}
					</p>
				{/if}
			{/if}

			<!-- Orange bcard bonuses -->
			<p class="bcard-effect">
				{#each item.static_data?.buffs as buff, i (i)}
					{buff.bcard_display[lang.current]}<br />
				{/each}
			</p>

			<p class="description">{item.static_data?.description[lang.current]}</p>
		</div>
	</div>
</Draggable>

<style>
	.window {
		border: 1px solid gray;
		background: rgba(27, 27, 27, 0.9);
		padding: 0.5rem;
		min-width: 300px;
		max-width: 300px;
		text-align: center;
		position: relative;
	}

	.body-text {
		margin-left: 40px;
		margin-right: 40px;
	}

	.window-header {
		position: absolute;
		top: 0.5rem;
		right: 0.5rem;
	}

	.white-color {
		color: #ffffff;
	}

	.description {
		color: #edecfd;
		white-space: pre-line;
	}

	.light-orange {
		color: #f5c478;
	}

	.price {
		color: #ff323d;
	}

	.stat-header {
		color: #cbcbcb;
	}

	.bcard-effect {
		color: #ff862c;
	}
</style>
