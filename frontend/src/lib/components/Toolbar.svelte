<script lang="ts">
	let { children } = $props();
</script>

<div class="toolbar gray-panel">
	{@render children?.()}
</div>

<style>
	.toolbar {
		display: grid;
		grid-template-columns: 1fr 1fr 1fr 1fr;
		column-gap: 10px;
		row-gap: 5px;
		padding: 10px;
		margin: 0 0 5px 0;
	}
</style>
