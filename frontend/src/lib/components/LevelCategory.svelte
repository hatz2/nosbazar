<script lang="ts">
	import * as enums from '$lib/types/enums';
	import DynamicSelect from './DynamicSelect.svelte';
	import { ConstStringKey } from '$lib/types/constStringKeys';
	import { get_const_string } from '$lib/services/constStringService.svelte';

	type Props = {
		value?: number;
		category: enums.BazarCategory;
	};

	let { value = $bindable(0), category }: Props = $props();

	const equipmentOptions = $derived([
		{ label: get_const_string(ConstStringKey.All), value: 0 },
		{ label: get_const_string(ConstStringKey.BazarLevel1_10), value: 1 },
		{ label: get_const_string(ConstStringKey.BazarLevel11_20), value: 2 },
		{ label: get_const_string(ConstStringKey.BazarLevel21_30), value: 3 },
		{ label: get_const_string(ConstStringKey.BazarLevel31_40), value: 4 },
		{ label: get_const_string(ConstStringKey.BazarLevel41_50), value: 5 },
		{ label: get_const_string(ConstStringKey.BazarLevel51_60), value: 6 },
		{ label: get_const_string(ConstStringKey.BazarLevel61_70), value: 7 },
		{ label: get_const_string(ConstStringKey.BazarLevel71_80), value: 8 },
		{ label: get_const_string(ConstStringKey.BazarLevel81_90), value: 9 },
		{ label: get_const_string(ConstStringKey.BazarLevel91_99), value: 10 },
		{ label: get_const_string(ConstStringKey.ChampionGear), value: 11 },
		{ label: get_const_string(ConstStringKey.ChampionLevel1to10), value: 12 },
		{ label: get_const_string(ConstStringKey.ChampionLevel10to20), value: 13 },
		{ label: get_const_string(ConstStringKey.ChampionLevel20to30), value: 14 },
		{ label: get_const_string(ConstStringKey.ChampionLevel30to40), value: 15 },
		{ label: get_const_string(ConstStringKey.ChampionLevel40to50), value: 16 },
		{ label: get_const_string(ConstStringKey.ChampionLevel50to60), value: 17 },
		{ label: get_const_string(ConstStringKey.ChampionLevel60to70), value: 18 },
		{ label: get_const_string(ConstStringKey.ChampionLevel70to80), value: 19 },
		{ label: get_const_string(ConstStringKey.ChampionLevel80to90), value: 20 },
		{ label: get_const_string(ConstStringKey.ChampionLevel90to100), value: 21 }
	]);

	const normalLevelOptions = $derived([
		{ label: get_const_string(ConstStringKey.All), value: 0 },
		{ label: get_const_string(ConstStringKey.BazarLevel1_10), value: 1 },
		{ label: get_const_string(ConstStringKey.BazarLevel11_20), value: 2 },
		{ label: get_const_string(ConstStringKey.BazarLevel21_30), value: 3 },
		{ label: get_const_string(ConstStringKey.BazarLevel31_40), value: 4 },
		{ label: get_const_string(ConstStringKey.BazarLevel41_50), value: 5 },
		{ label: get_const_string(ConstStringKey.BazarLevel51_60), value: 6 },
		{ label: get_const_string(ConstStringKey.BazarLevel61_70), value: 7 },
		{ label: get_const_string(ConstStringKey.BazarLevel71_80), value: 8 },
		{ label: get_const_string(ConstStringKey.BazarLevel81_90), value: 9 },
		{ label: get_const_string(ConstStringKey.BazarLevel91_99), value: 10 }
	]);

	const optionsByCategory = $derived({
		[enums.BazarCategory.All]: [],
		[enums.BazarCategory.Weapon]: equipmentOptions,
		[enums.BazarCategory.Armour]: equipmentOptions,
		[enums.BazarCategory.Equipment]: equipmentOptions,
		[enums.BazarCategory.Accessories]: equipmentOptions,
		[enums.BazarCategory.Specialist]: normalLevelOptions,
		[enums.BazarCategory.Pet]: normalLevelOptions,
		[enums.BazarCategory.Partner]: normalLevelOptions,
		[enums.BazarCategory.StoreMount]: [],
		[enums.BazarCategory.Shell]: normalLevelOptions,
		[enums.BazarCategory.MainItem]: [],
		[enums.BazarCategory.ConsumerItem]: [],
		[enums.BazarCategory.Miscellaneous]: []
	});

	const options = $derived(optionsByCategory[category]);
</script>

<DynamicSelect bind:value {options} />
