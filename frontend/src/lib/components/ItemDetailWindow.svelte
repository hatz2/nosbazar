<script lang="ts">
	import Draggable from './Draggable.svelte';
	import type { EnrichedSearchResult } from '$lib/types/search';
	import { lang } from '$lib/stores/lang.svelte';
	import { RequiredClass } from '$lib/types/enums';
	import CloseButton from './CloseButton.svelte';
	import { isAccessory, isResistances, itemHasFlagsToDisplay } from '$lib/types/search';

	type Props = {
		item: EnrichedSearchResult;
		left?: number;
		top?: number;
		onclose?: () => void;
	};

	let { item, left = 0, top = 0, onclose }: Props = $props();

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
				result += 'Adventurer';
			}
			if (required_class & RequiredClass.Swordsman) {
				result += 'Swordsman';
			}
			if (required_class & RequiredClass.Archer) {
				result += 'Archer';
			}
			if (required_class & RequiredClass.Mage) {
				result += 'Mage';
			}
			if (required_class & RequiredClass.Martial) {
				result += 'Martial Artist';
			}
		} else {
			if (required_class & RequiredClass.Adventurer) {
				result += 'Adventurer, ';
			}
			if (required_class & RequiredClass.Swordsman) {
				result += 'Swordsman, ';
			}
			if (required_class & RequiredClass.Archer) {
				result += 'Archer, ';
			}
			if (required_class & RequiredClass.Mage) {
				result += 'Mage, ';
			}
			if (required_class & RequiredClass.Martial) {
				result += 'Martial Artist';
			}
		}

		result += ' only';

		return result;
	}

	function get_required_level(item: EnrichedSearchResult): string {
		let result: string = '';

		if ('required_level' in item.data) {
			if (item.static_data?.flags.is_champion_equip) {
				result = 'Required Champion Level: ';
			} else {
				result = 'Required Level: ';
			}

			result += item.data.required_level + ' Lv';
		}

		return result;
	}
</script>

<Draggable {left} {top}>
	<div class="window">
		<div class="window-header">
			<CloseButton onclick={onclose} />
		</div>
		<!-- Name -->
		<div class="name-row">
			<img
				src="http://localhost:8080/icon/{item.icon_id}"
				alt="Item icon"
				style="pointer-events: none;"
			/>
			<p class="light-orange name-center">{item.item_name[lang.current]}</p>
		</div>

		<!-- Gender specific item -->
		{#if item.static_data}
			{#if item.static_data.flags.male_can_wear}
				<p class="gender-desc">Only for females</p>
			{:else if item.static_data.flags.female_can_wear}
				<p class="gender-desc">Only for males</p>
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
			{#if 'rare' in item.data}
				Rarity level: {item.data.rare}<br />
			{/if}
			{#if 'sum_level' in item.data}
				<span class="light-orange">Combine Times: {item.data.sum_level}</span><br />
			{/if}
			{#if isAccessory(item.data)}
				<span class="light-orange">Maximum Option Level: {item.data.max_option_level}</span><br />
				<span class="light-orange"
					>Maximum Option Piece Number: {item.data.options.length}/{item.data
						.max_option_count}</span
				><br />
			{/if}
		</p>

		<!-- Flags section -->
		{#if item.static_data}
			{#if itemHasFlagsToDisplay(item.static_data.flags)}
				<p class="light-orange">
					{#if item.static_data.flags.no_dropping}
						[Drop] False<br />
					{/if}
					{#if item.static_data.flags.no_selling}
						[Sell] False<br />
					{/if}
					{#if item.static_data.flags.no_trading}
						[Trade] False<br />
					{/if}
				</p>
			{/if}
		{/if}

		<!-- Damage section -->
		<p class="light-orange">
			<!-- Dmg stats -->
			{#if 'min_dmg' in item.data && 'max_dmg' in item.data}
				Damage: {item.data.min_dmg}~{item.data.max_dmg}<br />
				{#if 'hit_rate' in item.data}
					Hit rate: {item.data.hit_rate}<br />
				{:else if 'concentration' in item.data}
					Concentration: {item.data.concentration}<br />
				{/if}
				{#if 'critical_rate' in item.data && 'critical_dmg' in item.data}
					{item.data.critical_rate}% Chance of {item.data.critical_dmg}% Critical<br />
				{/if}
			{/if}

			<!-- Armour stats -->
			{#if 'melee_defence' in item.data}
				Melee Defence: {item.data.melee_defence}<br />
				Ranged Defence: {item.data.ranged_defence}<br />
				Magic Defence: {item.data.magic_defence}<br />
				Dodge: {item.data.dodge}
			{/if}
		</p>

		<!-- Resistances for gloves/shoes -->
		{#if isResistances(item.data)}
			<p class="light-orange">
				Fire Element Resistance: {item.data.fire_res}%<br />
				Water Element Resistance: {item.data.water_res}%<br />
				Light Element Resistance: {item.data.light_res}%<br />
				Shadow Element Resistance: {item.data.shadow_res}%
			</p>
		{/if}

		<!-- Price -->
		{#if 'price' in item.data}
			<p class="price">Price: {item.data.price.toLocaleString('es-ES')}</p>
		{/if}

		<!-- Cell options -->
		{#if isAccessory(item.data)}
			{#if item.data.options.length > 0}
				<p class="light-orange">
					{#each item.data.options as option, i (i)}
						{option.level} Lv {option.vnum} {option.value}<br />
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
</Draggable>

<style>
	.window {
		border: 1px solid gray;
		/* background: white; */
		background: rgba(27, 27, 27, 0.9);
		padding: 0.5rem;
		min-width: 300px;
		max-width: 300px;
		text-align: center;
		position: relative;
	}

	.window-header {
		position: absolute;
		top: 0.5rem;
		right: 0.5rem;
	}

	.gender-desc {
		color: #ffffff;
	}

	.name-row {
		display: grid;
		grid-template-columns: 1fr;
		grid-template-rows: 1fr;
		align-items: center;
	}

	.name-row img {
		grid-column: 1;
		grid-row: 1;
		justify-self: start;
	}

	.name-row .name-center {
		grid-column: 1;
		grid-row: 1;
		justify-self: center;
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
