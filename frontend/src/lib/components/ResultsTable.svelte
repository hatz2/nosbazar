<script lang="ts">
	import type { EnrichedSearchResult } from '$lib/types/search';
	import { lang } from '$lib/stores/lang.svelte';
	import ItemIcon from './ItemIcon.svelte';
	import { onMount } from 'svelte';
	import { ConstStringKey } from '$lib/types/constStringKeys';
	import { fetchConstString, get_const_string } from '$lib/services/constStringService.svelte';

	type Props = {
		results: EnrichedSearchResult[];
		onContextMenu?: (item: EnrichedSearchResult, event: MouseEvent) => void;
	};

	let { results, onContextMenu }: Props = $props();

	onMount(() => {
		fetchConstString(ConstStringKey.Name);
		fetchConstString(ConstStringKey.Price);
		fetchConstString(ConstStringKey.Amount);
		fetchConstString(ConstStringKey.PricePerUnit);
		fetchConstString(ConstStringKey.TimePeriod);
		fetchConstString(ConstStringKey.NoItemHasBeenFound);
	});

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
			<th>{get_const_string(ConstStringKey.Name)}</th>
			<th>{get_const_string(ConstStringKey.Amount)}</th>
			<th>{get_const_string(ConstStringKey.PricePerUnit)}</th>
			<th>{get_const_string(ConstStringKey.TimePeriod)}</th>
			<th>{get_const_string(ConstStringKey.Seller)}</th>
		</tr>
	</thead>

	<tbody>
		{#if results.length > 0}
			{#each results as item, i (i)}
				<tr>
					<td class="name-cell"
						><!-- svelte-ignore a11y_no_static_element_interactions --><span
							oncontextmenu={(e) => {
								e.preventDefault();
								onContextMenu?.(item, e);
							}}
							style="cursor: pointer"><ItemIcon iconId={item.icon_id} /></span
						>{item.item_name[lang.current] || item.item_name['UK'] || item.item_vnum.toString()}</td
					>
					<td>{item.amount}</td>
					<td>{item.bazar_price.toLocaleString('en-US')}</td>
					<td>{formatTime(item.minutes_left)}</td>
					<td>{item.owner_name}</td>
				</tr>
			{/each}
		{/if}
	</tbody>

	<tfoot> </tfoot>
</table>
{#if results.length == 0}
	<p>{get_const_string(ConstStringKey.NoItemHasBeenFound)}</p>
{/if}

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

	p {
		text-align: center;
		font-size: large;
	}
</style>
