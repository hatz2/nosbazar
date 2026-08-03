<script lang="ts">
	import type { Snippet } from 'svelte';

	let { tooltipText, children } = $props<{
		tooltipText: Snippet;
		children: Snippet;
	}>();
</script>

<div class="tooltip">
	{@render children()}

	<span class="tooltip-content">
		{@render tooltipText()}
	</span>
</div>

<style>
	.tooltip {
		position: relative;
		display: inline-block;
	}

	.tooltip-content {
		text-align: center;
		position: absolute;
		left: 0;
		bottom: calc(100% + 0px);

		background: rgba(10, 10, 10, 0.9);
		color: white;
		padding: 6px 10px;
		white-space: nowrap;

		opacity: 0;
		pointer-events: none;
		transition: opacity 0.15s;
	}

	.tooltip:hover .tooltip-content {
		opacity: 1;
	}
</style>
