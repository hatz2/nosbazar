<script lang="ts">
	import Draggable from './Draggable.svelte';
	import { formatItemData, formatStaticData, isResistances } from '$lib/types/search';
	import type { EnrichedSearchResult, FormattedSection } from '$lib/types/search';
	import { lang } from '$lib/stores/lang.svelte';
	import { RequiredClass } from '$lib/types/enums';
	import CloseButton from './CloseButton.svelte';

	type Props = {
		item: EnrichedSearchResult;
		left?: number;
		top?: number;
		onclose?: () => void;
	};

	let { item, left = 0, top = 0, onclose }: Props = $props();

	const sections: FormattedSection[] = formatItemData(item.data);
	const staticSections: FormattedSection[] = formatStaticData(item.static_data, lang.current);

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
			<p class="default-item-name name-center">{item.item_name[lang.current]}</p>
		</div>
		<hr />

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
				Combine Times: {item.data.sum_level}
			{/if}
		</p>

		<!-- Flags section -->
		<p class="dmg-section">
			{#if item.static_data}
				{#if 'no_dropping' in item.static_data.flags && item.static_data.flags.no_dropping}
					[Drop] False<br />
				{/if}
				{#if 'no_selling' in item.static_data.flags && item.static_data.flags.no_selling}
					[Sell] False<br />
				{/if}
				{#if 'no_trading' in item.static_data.flags && item.static_data.flags.no_trading}
					[Trade] False<br />
				{/if}
			{/if}
		</p>

		<!-- Damage section -->
		<p class="dmg-section">
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
			{#if 'melee_defence' in item.data}<br />
				Melee Defence: {item.data.melee_defence}<br />
				Ranged Defence: {item.data.ranged_defence}<br />
				Magic Defence: {item.data.magic_defence}<br />
				Dodge: {item.data.dodge}
			{/if}
		</p>

		<!-- Resistances for gloves/shoes -->
		{#if isResistances(item.data)}
			<!-- <p>Fire Element Resistance: {item.data.fire_res}%</p>
				<p>Water Element Resistance: {item.data.water_res}%</p>
				<p>Light Element Resistance: {item.data.light_res}%</p>
				<p>Shadow Element Resistance: {item.data.shadow_res}%</p> -->
			<p class="dmg-section">
				Fire Element Resistance: {item.data.fire_res}%<br />
				Water Element Resistance: {item.data.water_res}%<br />
				Light Element Resistance: {item.data.light_res}%<br />
				Shadow Element Resistance: {item.data.shadow_res}%
			</p>
		{/if}

		{#if 'price' in item.data}
			<p class="price">Price: {item.data.price.toLocaleString('es-ES')}</p>
		{/if}

		<!-- Orange bcard bonuses -->
		<p class="bcard-effect">
			{#each item.static_data?.buffs as buff, i (i)}
				{buff.bcard_display[lang.current]}<br />
			{/each}
		</p>

		<!-- {#each sections as section, i (i)}
			<hr />
			{#each section.fields as field, j (j)}
				<p class={field.cssClass}>
					<strong>{field.label}:</strong>
					{field.value}
				</p>
			{/each}
		{/each}

		{#each staticSections as section, i (i)}
			<hr />
			{#each section.fields as field, j (j)}
				<p class={field.cssClass}>
					<strong>{field.label}:</strong>
					{field.value}
				</p>
			{/each}
		{/each} -->

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
		max-width: 350px;
		text-align: center;
		position: relative;
	}

	.window-header {
		position: absolute;
		top: 0.5rem;
		right: 0.5rem;
	}

	hr {
		border: none;
		border-top: 1px solid #ddd;
		margin: 0.4rem 0;
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

	.default-item-name {
		color: #f5c478;
	}

	.description {
		color: #edecfd;
		white-space: pre-line;
	}

	.dmg-section {
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

	.stat-damage {
		color: #e74c3c;
	}
	.stat-hitrate {
		color: #2ecc71;
	}
	.stat-crit {
		color: #f39c12;
	}
	.stat-magic {
		color: #8e44ad;
	}
	.stat-ammo {
		color: #3498db;
	}
	.stat-shell {
		color: #e67e22;
	}

	.stat-defense-melee {
		color: #3498db;
	}
	.stat-defense-ranged {
		color: #2ecc71;
	}
	.stat-defense-magic {
		color: #8e44ad;
	}
	.stat-dodge {
		color: #f1c40f;
	}

	.stat-res-fire {
		color: #e74c3c;
	}
	.stat-res-water {
		color: #3498db;
	}
	.stat-res-light {
		color: #f1c40f;
	}
	.stat-res-shadow {
		color: #8e44ad;
	}
	.stat-res-total {
		color: #2c3e50;
		font-weight: bold;
	}

	.stat-level {
		color: #95a5a6;
	}
	.stat-price {
		color: #27ae60;
	}
	.stat-accessory {
		color: #d35400;
	}

	.stat-rare {
		color: #e67e22;
	}

	.stat-specialist-offense {
		color: #e74c3c;
	}
	.stat-specialist-defense {
		color: #3498db;
	}
	.stat-specialist-element {
		color: #8e44ad;
	}
	.stat-specialist-hp {
		color: #2ecc71;
	}
	.stat-specialist-remaining {
		color: #f1c40f;
		font-weight: bold;
	}

	.stat-flag {
		color: #e74c3c;
		font-weight: bold;
	}
	.stat-desc {
		color: #7f8c8d;
		font-style: italic;
		white-space: pre-wrap;
	}
</style>
