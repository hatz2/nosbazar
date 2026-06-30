<script lang="ts">
	import { onMount } from 'svelte';
	import { SortFilter } from '$lib/types/enums';
	import { ConstStringKey } from '$lib/types/constStringKeys';
	import { fetchConstString, get_const_string } from '$lib/services/constStringService.svelte';

	type Props = {
		value?: number;
	};

	let { value = $bindable(0) }: Props = $props();

	onMount(() => {
		fetchConstString(ConstStringKey.SortByPriceAscending);
		fetchConstString(ConstStringKey.SortByPriceDescending);
		fetchConstString(ConstStringKey.SortByAmountAscending);
		fetchConstString(ConstStringKey.SortByAmountDescending);
	});
</script>

<select bind:value>
	<option value={SortFilter.PriceAscending} selected>{get_const_string(ConstStringKey.SortByPriceAscending)}</option>
	<option value={SortFilter.PriceDescending}>{get_const_string(ConstStringKey.SortByPriceDescending)}</option>
	<option value={SortFilter.AmountAscending}>{get_const_string(ConstStringKey.SortByAmountAscending)}</option>
	<option value={SortFilter.AmountDescending}>{get_const_string(ConstStringKey.SortByAmountDescending)}</option>
</select>
