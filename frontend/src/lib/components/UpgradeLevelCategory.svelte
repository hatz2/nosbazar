<script lang="ts">
	import * as enums from '$lib/types/enums';
	import DynamicSelect from './DynamicSelect.svelte';

	type Props = {
		value?: number;
		category: enums.BazarCategory;
	};

	let { value = $bindable(0), category }: Props = $props();

	const upgradeLevelOptions = [
		{ label: 'All', value: 0 },
		{ label: '0', value: 1 },
		{ label: '1', value: 2 },
		{ label: '2', value: 3 },
		{ label: '3', value: 4 },
		{ label: '4', value: 5 },
		{ label: '5', value: 6 },
		{ label: '6', value: 7 },
		{ label: '7', value: 8 },
		{ label: '8', value: 9 },
		{ label: '9', value: 10 },
		{ label: '10', value: 11 },
		{ label: '11', value: 12 },
		{ label: '12', value: 13 },
		{ label: '13', value: 14 }
	];

	const specialistOptions = [
		{ label: 'All', value: 0 },
		{ label: '+0', value: 1 },
		{ label: '+1', value: 2 },
		{ label: '+2', value: 3 },
		{ label: '+3', value: 4 },
		{ label: '+4', value: 5 },
		{ label: '+5', value: 6 },
		{ label: '+6', value: 7 },
		{ label: '+7', value: 8 },
		{ label: '+8', value: 9 },
		{ label: '+9', value: 10 },
		{ label: '+10', value: 11 },
		{ label: '+11', value: 12 },
		{ label: '+12', value: 13 },
		{ label: '+13', value: 14 },
		{ label: '+14', value: 15 },
		{ label: '+15', value: 16 },
		{ label: '+16', value: 17 },
		{ label: '+17', value: 18 },
		{ label: '+18', value: 19 },
		{ label: '+19', value: 20 },
		{ label: '+20', value: 21 }
	];

	const petOptions = [
		{ label: 'All', value: 0 },
		{ label: '+0', value: 1 },
		{ label: '+1', value: 2 },
		{ label: '+2', value: 3 },
		{ label: '+3', value: 4 },
		{ label: '+4', value: 5 },
		{ label: '+5', value: 6 },
		{ label: '+6', value: 7 }
	];

	const optionsByCategory = {
		[enums.BazarCategory.All]: [],
		[enums.BazarCategory.Weapon]: upgradeLevelOptions,
		[enums.BazarCategory.Armour]: upgradeLevelOptions,
		[enums.BazarCategory.Equipment]: [],
		[enums.BazarCategory.Accessories]: [],
		[enums.BazarCategory.Specialist]: specialistOptions,
		[enums.BazarCategory.Pet]: petOptions,
		[enums.BazarCategory.Partner]: [],
		[enums.BazarCategory.StoreMount]: [],
		[enums.BazarCategory.Shell]: [],
		[enums.BazarCategory.MainItem]: [],
		[enums.BazarCategory.ConsumerItem]: [],
		[enums.BazarCategory.Miscellaneous]: []
	};

	const options = $derived(optionsByCategory[category]);
</script>

<DynamicSelect bind:value {options} />
