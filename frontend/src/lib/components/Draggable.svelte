<script lang="ts">
	let { left = 0, top = 0, children } = $props();

	let moving = false;
	let startX = 0;
	let startY = 0;
	let originLeft = 0;
	let originTop = 0;

	function onPointerDown(e: PointerEvent) {
		moving = true;

		startX = e.clientX;
		startY = e.clientY;

		originLeft = left;
		originTop = top;
	}

	function onPointerMove(e: PointerEvent) {
		if (moving) {
			left = originLeft + (e.clientX - startX);
			top = originTop + (e.clientY - startY);
		}
	}

	function onPointerUp() {
		moving = false;
	}
</script>

<!-- svelte-ignore a11y_no_static_element_interactions -->
<div onpointerdown={onPointerDown} style="left: {left}px; top: {top}px;" class="draggable">
	{@render children?.()}
</div>

<svelte:window on:pointerup={onPointerUp} on:pointermove={onPointerMove} />

<style>
	.draggable {
		user-select: none;
		/* cursor: move; */
		/* border: solid 1px gray; */
		position: absolute;
	}
</style>
