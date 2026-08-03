<script lang="ts">
	let { text, onclick, disabled = false } = $props();
</script>

<button class="blue-btn" {onclick} {disabled}>
	{text}
</button>

<style>
	.blue-btn {
		color: #dcecff;
		background: linear-gradient(#5aa8d8 0%, #5aa8d8 10%, #3475be 25%, #3475be 85%, #588dc9 100%);
		border: 1px solid #0e2752;
		border-radius: 6px;
		padding: 5px 0px;
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
		background: linear-gradient(#588dc9 0%, #3475be 15%, #3475be 75%, #5aa8d8 90%, #7dcdf8 100%);
	}

	.blue-btn:disabled {
		color: #848aa2;
		background: linear-gradient(#588dc9 0%, #3475be 15%, #3475be 75%, #5aa8d8 90%, #7dcdf8 100%);
	}
</style>
