<script lang="ts">
	type Props = {
		iconId?: number;
	};

	let { iconId }: Props = $props();
</script>

{#if iconId !== undefined}
	<span class="icon-wrapper">
		<img
			src="http://localhost:8080/icon/{iconId}"
			alt="Item icon"
			width={32}
			height={32}
		/>
	</span>
{/if}

<style>
	.icon-wrapper {
		display: inline-flex;
		align-items: center;
		justify-content: center;
		vertical-align: middle;
		margin-right: 6px;
		width: 32px;
		height: 32px;
	}

	img {
		display: block;
		image-rendering: pixelated;
	}
</style>
