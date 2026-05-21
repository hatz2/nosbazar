<script lang="ts">
	import { BazarCategory } from '$lib/types/enums';
	import BlueButton from './BlueButton.svelte';
	import Category from './Category.svelte';
	// import Draggable from './Draggable.svelte';
	import LevelCategory from './LevelCategory.svelte';
	import RarityLevelCategory from './RarityLevelCategory.svelte';
	import SubCategory from './SubCategory.svelte';
	import TitleBar from './TitleBar.svelte';
	import Toolbar from './Toolbar.svelte';
	import UpgradeLevelCategory from './UpgradeLevelCategory.svelte';
	import ResultsTable from './ResultsTable.svelte';

	let category = $state(BazarCategory.All);
</script>

<!-- <Draggable left={200} top={200}> -->
<div class="content">
	<TitleBar title="NosBazaar"></TitleBar>
	<Toolbar>
		<span>Name</span>
		<span>Category</span>
		<span>Subcategory</span>
		<span></span>

		<input type="text" />
		<Category bind:value={category}></Category>
		<SubCategory {category}></SubCategory>
		<BlueButton text="Search" onclick={() => {}}></BlueButton>

		<span>Level</span>
		<span>Rarity Level</span>
		<span>Upgrade Level</span>
		<span></span>

		<LevelCategory {category}></LevelCategory>
		<RarityLevelCategory {category}></RarityLevelCategory>
		<UpgradeLevelCategory {category}></UpgradeLevelCategory>
	</Toolbar>

	<ResultsTable></ResultsTable>
</div>

<!-- </Draggable> -->

<style>
	.content {
		margin-top: 0;
		padding-top: 0;
		border: 1px solid gray;
		max-width: 800px;
	}
</style>
