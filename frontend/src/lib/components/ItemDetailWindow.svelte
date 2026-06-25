<script lang="ts">
	import Draggable from './Draggable.svelte';
	import type { EnrichedSearchResult } from '$lib/types/search';

	type Props = {
		item: EnrichedSearchResult;
		left?: number;
		top?: number;
	};

	let { item, left = 0, top = 0 }: Props = $props();
</script>

<Draggable {left} {top}>
	<div class="window">
		<p><strong>VNum:</strong> {item.item_vnum}</p>
		<p><strong>Name:</strong> {item.item_name['UK']}</p>
		<p><strong>Amount:</strong> {item.amount}</p>
		<p><strong>Price:</strong> {item.bazar_price.toLocaleString('en-US')}</p>
		<p><strong>Seller:</strong> {item.owner_name}</p>
		<p>--- Item details placeholder ---</p>
	</div>
</Draggable>

<style>
	.window {
		border: 1px solid gray;
		background: white;
		padding: 0.5rem;
		min-width: 200px;
	}
</style>
