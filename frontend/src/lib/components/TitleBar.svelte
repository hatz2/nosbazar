<script lang="ts">
	let { title, children } = $props();
</script>

<div class="title-bar">
	<span>{title}</span>
	{@render children?.()}
</div>

<style>
	.title-bar {
		display: flex;
		justify-content: space-between;
		align-items: center;
		font-size: 18px;
		color: white;
		background: linear-gradient(
			#474847 0%,
			#757875 10%,
			#2b2d31 25%,
			#2b2d31 50%,
			black 55%,
			black 100%
		);
		border: 1px solid #7c7c7c;
		border-radius: 5px 5px 5px 5px;
		box-shadow: 0 0 0px 1px black;
		padding: 4px 7px;
		text-shadow:
			2px 0 #2a2c30,
			-2px 0 #2a2c30,
			0 2px #2a2c30,
			0 -2px #2a2c30,
			1px 1px #2a2c30,
			-1px -1px #2a2c30,
			1px -1px #2a2c30,
			-1px 1px #2a2c30;
	}
</style>
