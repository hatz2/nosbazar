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
		fetchConstString(ConstStringKey.Upgrade0);
		fetchConstString(ConstStringKey.Upgrade1);
		fetchConstString(ConstStringKey.Upgrade2);
		fetchConstString(ConstStringKey.Upgrade3);
		fetchConstString(ConstStringKey.Upgrade4);
		fetchConstString(ConstStringKey.Upgrade5);
		fetchConstString(ConstStringKey.Upgrade6);
		fetchConstString(ConstStringKey.Upgrade7);
		fetchConstString(ConstStringKey.Upgrade8);
		fetchConstString(ConstStringKey.Upgrade9);
		fetchConstString(ConstStringKey.Upgrade10);
		fetchConstString(ConstStringKey.Upgrade11);
		fetchConstString(ConstStringKey.Upgrade12);
		fetchConstString(ConstStringKey.Upgrade13);
		fetchConstString(ConstStringKey.Upgrade14);
		fetchConstString(ConstStringKey.Upgrade15);
	});

	const upgradeLevelOptions = $derived([
		{ label: get_const_string(ConstStringKey.All), value: 0 },
		{ label: get_const_string(ConstStringKey.Upgrade0), value: 1 },
		{ label: get_const_string(ConstStringKey.Upgrade1), value: 2 },
		{ label: get_const_string(ConstStringKey.Upgrade2), value: 3 },
		{ label: get_const_string(ConstStringKey.Upgrade3), value: 4 },
		{ label: get_const_string(ConstStringKey.Upgrade4), value: 5 },
		{ label: get_const_string(ConstStringKey.Upgrade5), value: 6 },
		{ label: get_const_string(ConstStringKey.Upgrade6), value: 7 },
		{ label: get_const_string(ConstStringKey.Upgrade7), value: 8 },
		{ label: get_const_string(ConstStringKey.Upgrade8), value: 9 },
		{ label: get_const_string(ConstStringKey.Upgrade9), value: 10 },
		{ label: get_const_string(ConstStringKey.Upgrade10), value: 11 },
		{ label: get_const_string(ConstStringKey.Upgrade11), value: 12 },
		{ label: get_const_string(ConstStringKey.Upgrade12), value: 13 },
		{ label: get_const_string(ConstStringKey.Upgrade13), value: 14 }
	]);

	const specialistOptions = $derived([
		{ label: get_const_string(ConstStringKey.All), value: 0 },
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
	]);

	const petOptions = $derived([
		{ label: get_const_string(ConstStringKey.All), value: 0 },
		{ label: '+0', value: 1 },
		{ label: '+1', value: 2 },
		{ label: '+2', value: 3 },
		{ label: '+3', value: 4 },
		{ label: '+4', value: 5 },
		{ label: '+5', value: 6 },
		{ label: '+6', value: 7 }
	]);

	const optionsByCategory = $derived({
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
	});

	const options = $derived(optionsByCategory[category]);
</script>

<DynamicSelect bind:value {options} />
