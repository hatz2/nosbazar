<script lang="ts">
	import { BazarCategory } from '$lib/types/enums';
	import BlueButton from './BlueButton.svelte';
	import Category from './Category.svelte';
	import Draggable from './Draggable.svelte';
	import LevelCategory from './LevelCategory.svelte';
	import SubCategory from './SubCategory.svelte';
	import TitleBar from './TitleBar.svelte';

	let category = $state(BazarCategory.All);
</script>

<Draggable left={200} top={200}>
	<TitleBar title="NosBazaar"></TitleBar>

	<div class="content">
		Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi in velit sit amet augue bibendum
		convallis. Nam nec imperdiet elit, id sollicitudin quam. Praesent sit amet enim gravida, rutrum
		velit at, cursus sapien. Nunc a varius tortor. Ut molestie feugiat ante pharetra cursus. Nulla
		placerat magna finibus dolor ullamcorper suscipit. Morbi nunc diam, aliquet et finibus ut,
		sodales ut nunc. In elit eros, lobortis quis turpis sed, gravida lacinia urna. Phasellus laoreet
		lobortis luctus. Proin risus dolor, posuere quis porta hendrerit, condimentum non leo. Maecenas
		rutrum convallis ultrices. Maecenas pretium ullamcorper suscipit. Class aptent taciti sociosqu
		ad litora torquent per conubia nostra, per inceptos himenaeos.
		<BlueButton
			text="Test"
			onclick={() => {
				console.log('Clicked');
			}}
		/>

		<Category bind:value={category}></Category>
		<SubCategory {category}></SubCategory>
		<LevelCategory {category}></LevelCategory>
	</div>
</Draggable>

<style>
	.content {
		margin-top: 0;
		padding-top: 0;
		border: 1px solid gray;
		max-width: 800px;
	}
</style>
