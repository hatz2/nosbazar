<script lang="ts">
	import { onMount } from 'svelte';
	import * as enums from '$lib/types/enums';
	import DynamicSelect from './DynamicSelect.svelte';
	import { ConstStringKey } from '$lib/types/constStringKeys';
	import { fetchConstString, get_const_string } from '$lib/services/constStringService.svelte';

	type Props = {
		value?: number;
		category: enums.BazarCategory;
	};

	let { value = $bindable(0), category }: Props = $props();

	onMount(() => {
		fetchConstString(ConstStringKey.BazarRarity0);
		fetchConstString(ConstStringKey.BazarRarityUseful);
		fetchConstString(ConstStringKey.BazarRarityGood);
		fetchConstString(ConstStringKey.BazarRarityHighQuality);
		fetchConstString(ConstStringKey.BazarRarityExcellent);
		fetchConstString(ConstStringKey.BazarRarityAncient);
		fetchConstString(ConstStringKey.BazarRarityMysterious);
		fetchConstString(ConstStringKey.BazarRarityLegendary);
	});

	const rarityOptions = $derived([
		{ label: get_const_string(ConstStringKey.All), value: 0 },
		{ label: get_const_string(ConstStringKey.BazarRarity0), value: 1 },
		{ label: get_const_string(ConstStringKey.BazarRarityUseful), value: 2 },
		{ label: get_const_string(ConstStringKey.BazarRarityGood), value: 3 },
		{ label: get_const_string(ConstStringKey.BazarRarityHighQuality), value: 4 },
		{ label: get_const_string(ConstStringKey.BazarRarityExcellent), value: 5 },
		{ label: get_const_string(ConstStringKey.BazarRarityAncient), value: 6 },
		{ label: get_const_string(ConstStringKey.BazarRarityMysterious), value: 7 },
		{ label: get_const_string(ConstStringKey.BazarRarityLegendary), value: 8 },
		{ label: get_const_string(ConstStringKey.BazarRarityPhenomenal), value: 9 }
	]);

	const specialistOptions = $derived([
		{ label: get_const_string(ConstStringKey.All), value: 0 },
		{ label: 'Perfection Lv 1-10', value: 1 },
		{ label: 'Perfection Lv 11-20', value: 2 },
		{ label: 'Perfection Lv 21-30', value: 3 },
		{ label: 'Perfection Lv 31-40', value: 4 },
		{ label: 'Perfection Lv 41-50', value: 5 },
		{ label: 'Perfection Lv 51-60', value: 6 },
		{ label: 'Perfection Lv 61-70', value: 7 },
		{ label: 'Perfection Lv 71-80', value: 8 },
		{ label: 'Perfection Lv 81-90', value: 9 },
		{ label: 'Perfection Lv 91-100', value: 10 }
	]);

	const optionsByCategory = $derived({
		[enums.BazarCategory.All]: [],
		[enums.BazarCategory.Weapon]: rarityOptions,
		[enums.BazarCategory.Armour]: rarityOptions,
		[enums.BazarCategory.Equipment]: [],
		[enums.BazarCategory.Accessories]: [],
		[enums.BazarCategory.Specialist]: specialistOptions,
		[enums.BazarCategory.Pet]: [],
		[enums.BazarCategory.Partner]: [],
		[enums.BazarCategory.StoreMount]: [],
		[enums.BazarCategory.Shell]: rarityOptions,
		[enums.BazarCategory.MainItem]: [],
		[enums.BazarCategory.ConsumerItem]: [],
		[enums.BazarCategory.Miscellaneous]: []
	});

	const options = $derived(optionsByCategory[category]);
</script>

<DynamicSelect bind:value {options} />
