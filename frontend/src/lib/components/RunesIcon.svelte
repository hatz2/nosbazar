<script lang="ts">
	import { type EnrichedSearchResult } from '$lib/types/search';
	import { RUNES_ICON_ID } from '$lib/constants';

	type Props = {
		item: EnrichedSearchResult;
		left?: number;
		top?: number;
		oncontextmenu?: (e: MouseEvent) => void;
	};

	let { item, left = 10, top = 50, oncontextmenu }: Props = $props();

	function handleContextMenu(e: MouseEvent) {
		e.preventDefault();
		oncontextmenu?.(e);
	}
</script>

{#if 'display_runes_count' in item.data && item.data.display_runes_count !== 0}
	<!-- svelte-ignore a11y_no_static_element_interactions -->
	<div style="left: {left}px; top: {top}px" oncontextmenu={handleContextMenu}>
		<img
			src="http://localhost:8080/icon/{RUNES_ICON_ID}"
			alt="Runes icon"
			class="orange-outline"
			draggable="false"
		/>

		<br />
		<span class="white">({item.data.display_runes_count}/21)</span>
	</div>
{/if}

<style>
	.white {
		color: white;
	}

	div {
		position: absolute;
		padding: 0;
	}

	img {
		width: 36px;
		height: 36px;
		cursor: pointer;
	}
</style>
