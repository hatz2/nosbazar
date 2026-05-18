<script lang="ts">
	import * as enums from '$lib/types/enums';
	import DynamicSelect from './DynamicSelect.svelte';

	type Props = {
		value?: number;
		category: enums.BazarCategory;
	};

	let { value = $bindable(0), category }: Props = $props();

	const equipmentOptions = [
		{ label: 'All', value: 0 },
		{ label: '1-10', value: 1 },
		{ label: '11-20', value: 2 },
		{ label: '21-30', value: 3 },
		{ label: '31-40', value: 4 },
		{ label: '41-50', value: 5 },
		{ label: '51-60', value: 6 },
		{ label: '61-70', value: 7 },
		{ label: '71-80', value: 8 },
		{ label: '81-90', value: 9 },
		{ label: '91-99', value: 10 },
		{ label: 'Champion gear', value: 11 },
		{ label: 'Champion Lv 1-10', value: 12 },
		{ label: 'Champion Lv 11-20', value: 13 },
		{ label: 'Champion Lv 21-30', value: 14 },
		{ label: 'Champion Lv 31-40', value: 15 },
		{ label: 'Champion Lv 41-50', value: 16 },
		{ label: 'Champion Lv 51-60', value: 17 },
		{ label: 'Champion Lv 61-70', value: 18 },
		{ label: 'Champion Lv 71-80', value: 19 },
		{ label: 'Champion Lv 81-90', value: 20 },
		{ label: 'Champion Lv 91-99', value: 21 }
	];

	const normalLevelOptions = [
		{ label: 'All', value: 0 },
		{ label: '1-10', value: 1 },
		{ label: '11-20', value: 2 },
		{ label: '21-30', value: 3 },
		{ label: '31-40', value: 4 },
		{ label: '41-50', value: 5 },
		{ label: '51-60', value: 6 },
		{ label: '61-70', value: 7 },
		{ label: '71-80', value: 8 },
		{ label: '81-90', value: 9 },
		{ label: '91-99', value: 10 }
	];

	const optionsByCategory = {
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
	};

	const options = $derived(optionsByCategory[category]);
</script>

<DynamicSelect bind:value {options} />
