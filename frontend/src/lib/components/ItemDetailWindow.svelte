<script lang="ts">
	import Draggable from './Draggable.svelte';
	import { formatItemData } from '$lib/types/search';
	import type { EnrichedSearchResult, FormattedSection } from '$lib/types/search';

	type Props = {
		item: EnrichedSearchResult;
		left?: number;
		top?: number;
	};

	let { item, left = 0, top = 0 }: Props = $props();

	const sections: FormattedSection[] = formatItemData(item.data);
</script>

<Draggable {left} {top}>
	<div class="window">
		<p><strong>VNum:</strong> {item.item_vnum}</p>
		<p><strong>Name:</strong> {item.item_name['UK']}</p>
		<p><strong>Amount:</strong> {item.amount}</p>
		<p><strong>Price:</strong> {item.bazar_price.toLocaleString('en-US')}</p>
		<p><strong>Seller:</strong> {item.owner_name}</p>

		{#each sections as section, i (i)}
			<hr />
			{#each section.fields as field, j (j)}
				<p class={field.cssClass}>
					<strong>{field.label}:</strong>
					{field.value}
				</p>
			{/each}
		{/each}
	</div>
</Draggable>

<style>
	.window {
		border: 1px solid gray;
		background: white;
		padding: 0.5rem;
		min-width: 200px;
	}

	hr {
		border: none;
		border-top: 1px solid #ddd;
		margin: 0.4rem 0;
	}

	.stat-damage {
		color: #e74c3c;
	}
	.stat-hitrate {
		color: #2ecc71;
	}
	.stat-crit {
		color: #f39c12;
	}
	.stat-magic {
		color: #8e44ad;
	}
	.stat-ammo {
		color: #3498db;
	}
	.stat-shell {
		color: #e67e22;
	}

	.stat-defense-melee {
		color: #3498db;
	}
	.stat-defense-ranged {
		color: #2ecc71;
	}
	.stat-defense-magic {
		color: #8e44ad;
	}
	.stat-dodge {
		color: #f1c40f;
	}

	.stat-res-fire {
		color: #e74c3c;
	}
	.stat-res-water {
		color: #3498db;
	}
	.stat-res-light {
		color: #f1c40f;
	}
	.stat-res-shadow {
		color: #8e44ad;
	}
	.stat-res-total {
		color: #2c3e50;
		font-weight: bold;
	}

	.stat-level {
		color: #95a5a6;
	}
	.stat-price {
		color: #27ae60;
	}
	.stat-accessory {
		color: #d35400;
	}

	.stat-rare {
		color: #e67e22;
	}

	.stat-specialist-offense {
		color: #e74c3c;
	}
	.stat-specialist-defense {
		color: #3498db;
	}
	.stat-specialist-element {
		color: #8e44ad;
	}
	.stat-specialist-hp {
		color: #2ecc71;
	}
	.stat-specialist-remaining {
		color: #f1c40f;
		font-weight: bold;
	}
</style>
