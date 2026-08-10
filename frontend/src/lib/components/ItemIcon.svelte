<script lang="ts">
	import { API_BASE } from '$lib/constants';

	type Props = {
		iconId?: number;
	};

	let { iconId }: Props = $props();
</script>

{#if iconId !== undefined}
	<span class="icon-wrapper">
		<img
			src="{API_BASE}/icon/{iconId}"
			alt="Item icon"
			class="orange-outline"
			draggable="false"
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
		width: 36px;
		height: 36px;
		background-color: #12171b;
		border: 1px solid #6d6e6d;
		box-shadow: 0 0 0 1px #12171b;
		border-radius: 2px;
	}

	img {
		display: block;
		max-width: 36px;
		max-height: 36px;
	}
</style>
