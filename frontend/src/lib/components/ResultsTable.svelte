<script lang="ts">
	import { get_display_icon_id, type EnrichedSearchResult } from '$lib/types/search';
	import { lang } from '$lib/stores/lang.svelte';
	import ItemIcon from './ItemIcon.svelte';
	import { onMount } from 'svelte';
	import { ConstStringKey } from '$lib/types/constStringKeys';
	import { fetchConstString, get_const_string } from '$lib/services/constStringService.svelte';
	import Tooltip from './Tooltip.svelte';
	import TooltipContent from './TooltipContent.svelte';

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
			return `${minutes} ${get_const_string(ConstStringKey.Minutes)}`;
		} else if (minutes <= 1440) {
			const hours = Math.round(minutes / 60);
			return `${hours} ${get_const_string(ConstStringKey.Hours)}`;
		} else {
			const days = Math.round(minutes / 1440);
			return `${days} ${get_const_string(ConstStringKey.DayS)}`;
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
					<td class="name-cell">
						<!-- svelte-ignore a11y_no_static_element_interactions --><span
							oncontextmenu={(e) => {
								e.preventDefault();
								onContextMenu?.(item, e);
							}}
							style="cursor: pointer;"
						>
							<Tooltip>
								<ItemIcon iconId={get_display_icon_id(item)} />
								{#snippet tooltipText()}
									<TooltipContent {item}></TooltipContent>
								{/snippet}
							</Tooltip>
						</span>{item.item_name[lang.current] ||
							item.item_name['UK'] ||
							item.item_vnum.toString()}
					</td>
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
		margin: 5px 0 5px 0;
		color: white;

		border: 1px solid #606467;
		border-radius: 4px;
		border-collapse: separate;
		box-shadow: 0 0 0 1px #111517;
		background-color: #111517;
		border-spacing: 0;
	}

	thead {
		box-shadow: inset 0 -1px 0 #606467;
	}

	thead th {
		font-weight: normal;
	}

	tbody td {
		text-align: center;
		border-bottom: 1px solid #353a40;
		background: linear-gradient(#1f1f1f 0% #030303 99% #141414 100%);
		padding: 3px;
	}

	tbody tr:hover {
		outline: 1px solid white;
		border-radius: 2px;
	}

	tbody tr:last-child td {
		border-bottom: none;
	}

	.name-cell {
		text-align: left;
	}

	p {
		text-align: center;
		font-size: large;
		color: white;
		text-shadow:
			-1px -1px black,
			-1px 1px black,
			1px 1px black,
			1px -1px black;
	}
</style>
