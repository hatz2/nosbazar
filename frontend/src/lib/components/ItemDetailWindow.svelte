<script lang="ts">
	import DetailWindow from './DetailWindow.svelte';
	import type { EnrichedSearchResult } from '$lib/types/search';
	import { lang } from '$lib/stores/lang.svelte';
	import { RequiredClass } from '$lib/types/enums';
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
		RarityLevelColor
	} from '$lib/utils/format';
	import {
		get_display_icon_id,
		isAccessory,
		isArmour,
		itemHasFlagsToDisplay,
		shouldShowDescription,
		shouldShowFlags
	} from '$lib/types/search';
	import { ConstStringKey } from '$lib/types/constStringKeys';
	import { get_const_string } from '$lib/services/constStringService.svelte';
	import RunesIcon from './RunesIcon.svelte';
	import RunesDetailWindow from './RunesDetailWindow.svelte';

	type Props = {
		item: EnrichedSearchResult;
		left?: number;
		top?: number;
		onclose?: () => void;
	};

	let { item, left = $bindable(0), top = $bindable(0), onclose }: Props = $props();

	let runes_window_open = $state(false);
	let runes_window_left = $state(0);
	let runes_window_top = $state(0);

	function onRunesContextMenu(e: MouseEvent) {
		runes_window_left = e.pageX + 15;
		runes_window_top = e.pageY - 20;
		runes_window_open = true;
	}

	function getRarityColor(item: EnrichedSearchResult): string {
		if ('rare' in item.data) {
			return RarityLevelColor[item.data.rare];
		} else {
			return RarityLevelColor[0];
		}
	}
</script>

<DetailWindow bind:left bind:top {onclose} iconId={get_display_icon_id(item)}>
	<RunesIcon {item} oncontextmenu={onRunesContextMenu}></RunesIcon>

	<!-- Name -->
	<div class="body-text">
		<p style="color: {getRarityColor(item)}">
			{item.item_name[lang.current]}
			{#if 'upgrade' in item.data && item.data.upgrade !== 0}
				+{item.data.upgrade}
			{/if}
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
						[{get_const_string(ConstStringKey.Drop)}] {get_const_string(ConstStringKey.False)}<br />
					{/if}
					{#if item.static_data.flags.no_selling}
						[{get_const_string(ConstStringKey.Sell)}] {get_const_string(ConstStringKey.False)}<br />
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

		{#if 'owner_id' in item.data && item.data.owner_id != 0}
			{#if isArmour(item.data)}
				<p class="red">{get_const_string(ConstStringKey.ThisIsSomeoneElseArmour)}</p>
			{:else}
				<p class="red">{get_const_string(ConstStringKey.ThisIsSomeoneElseWeapon)}</p>{/if}
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
</DetailWindow>

{#if runes_window_open && 'runes' in item.data}
	<RunesDetailWindow
		runes={item.data.runes}
		display_runes_count={item.data.display_runes_count}
		left={runes_window_left}
		top={runes_window_top}
		onclose={() => (runes_window_open = false)}
	/>
{/if}

<style>
	.body-text {
		margin-left: 40px;
		margin-right: 40px;
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
