<script lang="ts">
	import type { EnrichedSearchResult } from '$lib/types/search';
	import { lang } from '$lib/stores/lang.svelte';
	import ItemIcon from './ItemIcon.svelte';

	type Props = {
		results: EnrichedSearchResult[];
	};

	let { results }: Props = $props();

	function formatTime(minutes: number) {
		if (minutes < 60) {
			return `${minutes} Mins`;
		} else if (minutes <= 1440) {
			const hours = Math.round(minutes / 60);
			return `${hours} Hours`;
		} else {
			const days = Math.round(minutes / 1440);
			return `${days} Day(s)`;
		}
	}
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
				<td class="name-cell"><ItemIcon iconId={item.icon_id} />{item.item_name[lang.current] || item.item_name['UK'] || item.item_vnum.toString()}</td>
				<td>{item.amount}</td>
				<td>{item.bazar_price.toLocaleString('en-US')}</td>
				<td>{formatTime(item.minutes_left)}</td>
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

	.name-cell {
		text-align: left;
	}
</style>
