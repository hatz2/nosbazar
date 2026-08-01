<script lang="ts">
	import { onMount } from 'svelte';
	import DetailWindow from './DetailWindow.svelte';
	import { type RuneEffect } from '$lib/types/search';
	import { lang, type LanguageCode } from '$lib/stores/lang.svelte';
	import { fetchBcardString } from '$lib/services/bcardService';
	import { RUNES_ICON_ID } from '$lib/constants';
	import { get_const_string } from '$lib/services/constStringService.svelte';
	import { ConstStringKey } from '$lib/types/constStringKeys';

	type Props = {
		runes: RuneEffect[];
		display_runes_count: number;
		left?: number;
		top?: number;
		onclose?: () => void;
	};

	let { runes, display_runes_count, left = 0, top = 0, onclose }: Props = $props();

	let rune_datas = $state<RuneDisplay[]>([]);

	interface RuneDisplay {
		rune: RuneEffect;
		translation: Record<LanguageCode, string>;
	}

	async function loadRuneDisplay(rune: RuneEffect): Promise<RuneDisplay> {
		return {
			rune,
			translation: await fetchBcardString(
				rune.bcard_vnum,
				rune.bcard_sub,
				rune.bcard_value_1,
				rune.bcard_value_2
			)
		};
	}

	onMount(async () => {
		rune_datas = await Promise.all(runes.map(loadRuneDisplay));
	});

	let rune_skills = $derived(rune_datas.filter((d) => d.rune.bcard_value_2 === 0));
	let rune_powers = $derived(rune_datas.filter((d) => d.rune.bcard_value_2 !== 0));
</script>

<DetailWindow {left} {top} {onclose} iconId={RUNES_ICON_ID}>
	<div class="body-text">
		<p style="color: #F8E2B3">
			{get_const_string(ConstStringKey.CarveRunes)} ({display_runes_count}/21)
		</p>

		<p class="header">
			{get_const_string(ConstStringKey.RuneSkills)}
		</p>

		<p class="bcard-effect">
			{#each rune_skills as rune_data, i (i)}
				{rune_data.translation[lang.current]}<br />
			{/each}
		</p>

		{#if rune_powers.length > 0}
			<p class="header">{get_const_string(ConstStringKey.RunePowers)}</p>

			{#each rune_powers as rune_data, i (i)}
				<p style="color: white">
					{rune_data.translation[lang.current]}
				</p>
			{/each}
		{/if}
	</div>
</DetailWindow>

<style>
	.body-text {
		margin-left: 40px;
		margin-right: 40px;
	}

	.bcard-effect {
		color: #ff862c;
	}

	.header {
		color: #cbcbcb;
	}
</style>
