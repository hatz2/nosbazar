<script lang="ts">
	import { BazarCategory } from '$lib/types/enums';

	type Props = {
		value?: BazarCategory;
		onchange?: (e: Event) => void;
	};

	let { value = $bindable(BazarCategory.All), onchange }: Props = $props();
</script>

<select bind:value {onchange}>
	<option value={BazarCategory.All} selected>All</option>
	<option value={BazarCategory.Weapon}>Weapon</option>
	<option value={BazarCategory.Armour}>Armour</option>
	<option value={BazarCategory.Equipment}>Equipment</option>
	<option value={BazarCategory.Accessories}>Accessories</option>
	<option value={BazarCategory.Specialist}>Specialist</option>
	<option value={BazarCategory.Pet}>Pet</option>
	<option value={BazarCategory.Partner}>Partner</option>
	<option value={BazarCategory.StoreMount}>Store mount</option>
	<option value={BazarCategory.Shell}>Shell</option>
	<option value={BazarCategory.MainItem}>Main item</option>
	<option value={BazarCategory.ConsumerItem}>Consumer item</option>
	<option value={BazarCategory.Miscellaneous}>Miscellaneous</option>
</select>

<style>
</style>
