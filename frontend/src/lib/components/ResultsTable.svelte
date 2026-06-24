<script lang="ts">
	import type { SearchResult } from '$lib/types/search';

	type Props = {
		results: SearchResult[];
	};

	let { results }: Props = $props();
</script>

<table>
	<thead>
		<tr>
			<th>Item name</th>
			<th>Amount</th>
			<th>Price per unit</th>
			<th>Time period</th>
			<th>Seller</th>
		</tr>
	</thead>

	<tbody>
		{#each results as item, i (i)}
			<tr>
				<td>{item.item_vnum}</td>
				<td>{item.amount}</td>
				<td>{item.bazar_price.toLocaleString()}</td>
				<td>{item.minutes_left} min</td>
				<td>{item.owner_name}</td>
			</tr>
		{/each}
	</tbody>

	<tfoot> </tfoot>
</table>

<style>
	table {
		width: 100%;
		border: 1px solid red;
		border-collapse: collapse;
	}

	thead {
		border: 1px solid green;
	}

	td {
		text-align: center;
	}
</style>
