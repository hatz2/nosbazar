<script lang="ts">
	let { children } = $props();
</script>

<div class="container">
	<div class="toolbar">
		{@render children?.()}
	</div>
</div>

<style>
	.container {
		padding: 10px;
	}

	.toolbar {
		display: grid;
		grid-template-columns: 1fr 1fr 1fr 1fr;
		column-gap: 10px;
		row-gap: 5px;
		/* gap: 10px; */
		/* padding: 10px; */
	}

	.toolbar div {
		border: solid 1px red;
		display: flex;
		flex-direction: column;
	}
</style>
