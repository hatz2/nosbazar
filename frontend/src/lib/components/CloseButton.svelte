<script lang="ts">
	import closeBtn from '$lib/assets/close-btn.webp';
	import closeBtnClicked from '$lib/assets/close-btn-clicked.webp';

	let pressed = $state(false);
	let { onclick }: { onclick?: () => void } = $props();
</script>

<button
	onmousedown={() => (pressed = true)}
	onmouseup={() => (pressed = false)}
	onmouseleave={() => (pressed = false)}
	{onclick}
	aria-label="Close"
>
	<img
		src={pressed ? closeBtnClicked : closeBtn}
		style="pointer-events: none;"
		alt="Close button"
	/>
</button>

<style>
	button {
		background: none;
		border: none;
		padding: 0;
		cursor: pointer;
		display: inline-flex;
	}
</style>
