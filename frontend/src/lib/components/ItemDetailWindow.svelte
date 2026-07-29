<script lang="ts">
	import Draggable from './Draggable.svelte';
	import type { EnrichedSearchResult } from '$lib/types/search';
	import { lang } from '$lib/stores/lang.svelte';
	import { RequiredClass } from '$lib/types/enums';
	import CloseButton from './CloseButton.svelte';
	import ItemSpecialistDetails from './ItemSpecialistDetails.svelte';
	import ItemPetBeadDetails from './ItemPetBeadDetails.svelte';
	import ItemPartnerSpecialistDetails from './ItemPartnerSpecialistDetails.svelte';
	import ItemPartnerBeadDetails from './ItemPartnerBeadDetails.svelte';
	import ItemMountBeadDetails from './ItemMountBeadDetails.svelte';
	import ItemStatsSection from './ItemStatsSection.svelte';
	import ItemShellSection from './ItemShellSection.svelte';
	import {
		formatCellonOptionString,
		formatString,
		get_required_class_string,
		get_required_level,
		get_element,
		get_star_pets
	} from '$lib/utils/format';
	import {
		get_display_icon_id,
		isAccessory,
		itemHasFlagsToDisplay,
		shouldShowDescription,
		shouldShowFlags
	} from '$lib/types/search';
	import { onMount } from 'svelte';
	import { ConstStringKey } from '$lib/types/constStringKeys';
	import { fetchConstString, get_const_string } from '$lib/services/constStringService.svelte';

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
		fetchConstString(ConstStringKey.PartnerSkill);
		fetchConstString(ConstStringKey.PartnerSkillLevel);
		fetchConstString(ConstStringKey.PartnerAttackBonus);
		fetchConstString(ConstStringKey.PartnerDefenceBonus);
		fetchConstString(ConstStringKey.PartnerCritReduction);
		fetchConstString(ConstStringKey.PartnerHpMpBonus);
		fetchConstString(ConstStringKey.PartnerFireResBonus);
		fetchConstString(ConstStringKey.PartnerWaterResBonus);
		fetchConstString(ConstStringKey.PartnerLightResBonus);
		fetchConstString(ConstStringKey.PartnerShadowResBonus);
	});

</script>

<Draggable {left} {top}>
	<div class="window">
		<div class="window-header">
			<CloseButton onclick={onclose} />
		</div>

		<img
			src="http://localhost:8080/icon/{get_display_icon_id(item)}"
			alt="Item icon"
			style="pointer-events: none; position: absolute; top: 10px; left: 10px; max-width: 32px; max-height: 32px;"
		/>
		<!-- Name -->
		<div class="body-text">
			<p class="light-orange">
				{item.item_name[lang.current]}
				{#if 'fairy_level_percent' in item.data}
					<br />{get_const_string(ConstStringKey.Attribute)}: {get_element(item)}<br />
					{formatString(
						get_const_string(ConstStringKey.StrengthensElementBy),
						item.data.fairy_level_percent
					)}
				{/if}
			</p>

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
						{get_const_string(ConstStringKey.Hours)}: {item.data.remaining_time_in_secs}
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

			<ItemSpecialistDetails {item} />

			<ItemPetBeadDetails {item} />

			<ItemPartnerSpecialistDetails {item} />

			<ItemPartnerBeadDetails {item} />

			<ItemMountBeadDetails {item} />

			<!-- Flags section -->
			{#if item.static_data}
				<p class="light-orange">
					{#if itemHasFlagsToDisplay(item.static_data.flags) && shouldShowFlags(item.data)}
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
					{/if}
					{#if 'cannot_trade_after_equip' in item.data && item.data.cannot_trade_after_equip}
						[{get_const_string(ConstStringKey.TradeIsAllowed)}]<br />
						{get_const_string(ConstStringKey.IfEquipedItWontBeTradable)}<br />
					{/if}
					{#if 'remaining_time_in_hours' in item.data && item.data.remaining_time_in_hours > 0}
						<span class="red"
							>{get_const_string(ConstStringKey.RemainingTime)}
							{item.data.remaining_time_in_hours / 24}
							{get_const_string(ConstStringKey.Day)}</span
						>
					{/if}
				</p>
			{/if}

			<!-- Monsters to level up for fairies -->
			{#if 'monster_to_level_up' in item.data}
				<p class="white-color">
					{get_const_string(ConstStringKey.MonstersRequiredForNextFairyLevel)}:
					{item.data.monster_to_level_up}
				</p>
			{/if}

			<ItemStatsSection {item} />

			<!-- Price -->
			{#if 'price' in item.data}
				<p class="red">
					{get_const_string(ConstStringKey.Price)}: {item.data.price.toLocaleString('es-ES')}
				</p>
			{/if}

			<ItemShellSection {item} />

			<!-- Cell options -->
			{#if isAccessory(item.data)}
				{#if item.data.options.length > 0}
					<p class="light-orange">
						{#each item.data.options as option, i (i)}
							{formatCellonOptionString(option)}<br />
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

			{#if shouldShowDescription(item.data)}
				<p class="description">{item.static_data?.description[lang.current]}</p>
			{/if}
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

	.red {
		color: #ff323d;
	}

	.stat-header {
		color: #cbcbcb;
	}

	.bcard-effect {
		color: #ff862c;
	}
</style>
