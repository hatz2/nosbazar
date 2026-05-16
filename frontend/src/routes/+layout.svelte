<script lang="ts">
	import favicon from '$lib/assets/favicon.svg';

	let { children } = $props();

	import DraggableDiv from '$lib/components/Draggable.svelte';
</script>

<svelte:head>
	<link rel="icon" href={favicon} />
</svelte:head>

{@render children()}

<DraggableDiv left={100} top={100}>
	<p>
		Some content
	</p>
</DraggableDiv>
