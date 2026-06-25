<script lang="ts">
	import { SortFilter } from '$lib/types/enums';

	type Props = {
		value?: number;
	};

	let { value = $bindable(0) }: Props = $props();
</script>

<select bind:value>
	<option value={SortFilter.PriceAscending} selected>Price (ascending)</option>
	<option value={SortFilter.PriceDescending}>Price (descending)</option>
	<option value={SortFilter.AmountAscending}>Amount (ascending)</option>
	<option value={SortFilter.AmountDescending}>Amount (descending)</option>
</select>
