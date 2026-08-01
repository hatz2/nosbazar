<script lang="ts">
	import type { Snippet } from 'svelte';
	import Draggable from './Draggable.svelte';
	import CloseButton from './CloseButton.svelte';

	type Props = {
		left?: number;
		top?: number;
		height?: number;
		iconId?: number;
		onclose?: () => void;
		children: Snippet;
	};

	let { left = 0, top = 0, height, iconId, onclose, children }: Props = $props();
</script>

<Draggable {left} {top}>
	<div class="window" style:height={height ? `${height}px` : undefined}>
		<div class="window-header">
			<CloseButton onclick={onclose} />
		</div>

		{#if iconId !== undefined}
			<img src="http://localhost:8080/icon/{iconId}" alt="Item icon" class="window-icon" />
		{/if}

		{@render children?.()}
	</div>
</Draggable>

<style>
	.window {
		border: 1px solid gray;
		background: rgba(27, 27, 27, 0.9);
		padding: 0.5rem;
		width: 300px;
		text-align: center;
		position: relative;
		box-sizing: border-box;
		overflow-y: auto;
	}

	.window-header {
		position: absolute;
		top: 0.5rem;
		right: 0.5rem;
	}

	.window-icon {
		position: absolute;
		top: 10px;
		left: 10px;
		width: 36px;
		height: 36px;
		pointer-events: none;
	}
</style>
