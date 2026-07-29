<script lang="ts">
	import Draggable from './Draggable.svelte';
	import type { EnrichedSearchResult } from '$lib/types/search';
	import { lang } from '$lib/stores/lang.svelte';
	import { RequiredClass } from '$lib/types/enums';
	import CloseButton from './CloseButton.svelte';
	import {
		formatCellonOptionString,
		formatShellOptionString,
		formatAppliedShellEffectString,
		formatString,
		ShellGradeColor
	} from '$lib/utils/format';
	import {
		get_display_icon_id,
		isAccessory,
		isPetBeadItem,
		isPartnerBeadItem,
		isPartnerSpecialistItem,
		isMountBeadItem,
		isResistances,
		isSpecialist,
		itemHasFlagsToDisplay,
		shouldShowDescription,
		shouldShowFlags,
		isShellData
	} from '$lib/types/search';
	import { skillService } from '$lib/services/skillService';
	import { onMount } from 'svelte';
	import { ConstStringKey } from '$lib/types/constStringKeys';
	import { fetchConstString, get_const_string } from '$lib/services/constStringService.svelte';
	import { Element, PartnerSkillLevels } from '$lib/types/enums';

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
				result = `${get_const_string(ConstStringKey.Required)}${get_const_string(ConstStringKey.ChampionLevel)}: `;
			} else {
				result = `${get_const_string(ConstStringKey.Required)}${get_const_string(ConstStringKey.Level)}: `;
			}

			result += `${item.data.required_level}${get_const_string(ConstStringKey.Lv)}`;
		}

		return result;
	}

	function get_element(item: EnrichedSearchResult): string {
		if ('element_type' in item.data) {
			switch (item.data.element_type) {
				case Element.NoElement:
					return get_const_string(ConstStringKey.FairyNoElement);
				case Element.Fire:
					return get_const_string(ConstStringKey.FairyFireElement);
				case Element.Water:
					return get_const_string(ConstStringKey.FairyWaterElement);
				case Element.Light:
					return get_const_string(ConstStringKey.FairyLightElement);
				case Element.Shadow:
					return get_const_string(ConstStringKey.FairyShadowElement);
			}
		}
		return '';
	}

	function get_star_pets(item: EnrichedSearchResult): string {
		if ('stars' in item.data) {
			switch (item.data.stars) {
				case 1:
					return get_const_string(ConstStringKey.StarPets1);
				case 2:
					return get_const_string(ConstStringKey.StarPets2);
				case 3:
					return get_const_string(ConstStringKey.StarPets3);
				case 4:
					return get_const_string(ConstStringKey.StarPets4);
				case 5:
					return get_const_string(ConstStringKey.StarPets5);
				case 6:
					return get_const_string(ConstStringKey.StarPets6);
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

			<!-- Specialist info -->
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

			<!-- Pet data -->
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

			<!-- Partner specialist data -->
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

			<!-- Partner bead data -->
			{#if isPartnerBeadItem(item.data)}
				<p class="light-orange">
					{#if item.data.has_partner_inside}
						{item.contained_monster_static_data?.name[lang.current]}
						{get_const_string(ConstStringKey.InCustody)}<br />
						{get_const_string(ConstStringKey.Level)}: {item.data.level}<br />
						{get_const_string(ConstStringKey.ExperiencePoints)}: {item.data.level_percentage}%<br />
						{get_const_string(ConstStringKey.AttackLevel)}: {item.data.attack_level}<br />
						{get_const_string(ConstStringKey.DefenceStat)}
						{get_const_string(ConstStringKey.Level)}: {item.data.defence_level}<br />
					{:else}
						{get_const_string(ConstStringKey.NotUsed)}<br />
					{/if}
				</p>
			{/if}

			<!-- Mount data -->
			{#if isMountBeadItem(item.data)}
				<p class="light-orange">
					{#if item.data.has_mount_inside}
						{item.contained_mount_item_static_data?.name[lang.current]}
						{get_const_string(ConstStringKey.InCustody)}<br />
					{:else}
						{get_const_string(ConstStringKey.NotUsed)}<br />
					{/if}
				</p>
			{/if}

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
				<p class="red">
					{get_const_string(ConstStringKey.Price)}: {item.data.price.toLocaleString('es-ES')}
				</p>
			{/if}

			<!-- Shell effects on weapons/armor -->
			{#if 'shells' in item.data && item.data.shells.length > 0}
				<p>
					{#each item.data.shells as shell, i (i)}
						<span style="color: {ShellGradeColor[shell.grade] ?? '#FFFFFF'}">
							{formatAppliedShellEffectString(shell)}
						</span><br />
					{/each}
				</p>
			{/if}

			{#if isShellData(item.data)}
				{#if item.data.shell_options.length > 0}
					<p>
						{#each item.data.shell_options as option, i (i)}
							<span style="color: {ShellGradeColor[option.grade] ?? '#FFFFFF'}">
								{formatShellOptionString(option)}
							</span><br />
						{/each}
					</p>
				{:else}
					<p class="red">
						{get_const_string(ConstStringKey.OptionNotIdentified)}
					</p>
				{/if}
			{/if}

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
