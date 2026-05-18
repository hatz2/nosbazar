<script lang="ts">
	import * as enums from '$lib/types/enums';
	import DynamicSelect from './DynamicSelect.svelte';

	type Props = {
		value?: number;
		category: enums.BazarCategory;
	};

	let { value = $bindable(0), category }: Props = $props();

	const rarityOptions = [
		{ label: 'All', value: 0 },
		{ label: '[0]', value: 1 },
		{ label: '[1] Useful', value: 2 },
		{ label: '[2] Good', value: 3 },
		{ label: '[3] High quality', value: 4 },
		{ label: '[4] Excellent', value: 5 },
		{ label: '[5] Ancient', value: 6 },
		{ label: '[6] Mysterious', value: 7 },
		{ label: '[7] Legendary', value: 8 },
		{ label: '[8] Phenomenal', value: 9 }
	];

	const specialistOptions = [
		{ label: 'All', value: 0 },
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
	];

	const optionsByCategory = {
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
	};

	const options = $derived(optionsByCategory[category]);
</script>

<DynamicSelect bind:value {options} />
