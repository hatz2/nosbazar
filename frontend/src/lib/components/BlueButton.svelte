<script lang="ts">
	let { text, onclick} = $props();

	// TODO: Proper style for this button
</script>

<button class="blue-btn" {onclick}>
	{text}
</button>

<style>
	/* .blue-btn {
		color: #dcecff;
		background: linear-gradient(#5aa8d8 0%, #5aa8d8 10%, #3475be 25%, #3475be 85%, #588dc9 100%);
		border: 1px solid #0e2752;
		border-radius: 8px;
		padding: 7px 15px;
		text-shadow:
			2px 0 #1052c6,
			-2px 0 #1052c6,
			0 2px #1052c6,
			0 -2px #1052c6,
			1px 1px #1052c6,
			-1px -1px #1052c6,
			1px -1px #1052c6,
			-1px 1px #1052c6;
	}

	.blue-btn:hover {
		background: linear-gradient(#75acde 0%, #75acde 10%, #5090cb 25%, #5090cb 85%, #7dcdf8 100%);
	}

	.blue-btn:active {
		background: linear-gradient(#296fbe 0%, #296fbe 90%, #4ba2d8 100%);
	} */
</style>
