<script lang="ts">
	import { onMount } from 'svelte';
	import { BazarCategory } from '$lib/types/enums';
	import { ConstStringKey } from '$lib/types/constStringKeys';
	import { fetchConstString, get_const_string } from '$lib/services/constStringService.svelte';

	type Props = {
		value?: BazarCategory;
		onchange?: (e: Event) => void;
	};

	let { value = $bindable(BazarCategory.All), onchange }: Props = $props();

	onMount(() => {
		fetchConstString(ConstStringKey.BazarCategoryWeapon);
		fetchConstString(ConstStringKey.BazarCategoryArmour);
		fetchConstString(ConstStringKey.BazarCategoryEquipment);
		fetchConstString(ConstStringKey.BazarCategoryAccessories);
		fetchConstString(ConstStringKey.Specialist);
		fetchConstString(ConstStringKey.BazarCategoryPet);
		fetchConstString(ConstStringKey.BazarCategoryPartner);
		fetchConstString(ConstStringKey.BazarCategoryShell);
		fetchConstString(ConstStringKey.BazarCategoryMainItem);
		fetchConstString(ConstStringKey.BazarCategoryConsumerItem);
		fetchConstString(ConstStringKey.BazarCategoryMiscellaneous);
	});
</script>

<select bind:value {onchange} class="bazar-select">
	<option value={BazarCategory.All} selected>{get_const_string(ConstStringKey.All)}</option>
	<option value={BazarCategory.Weapon}
		>{get_const_string(ConstStringKey.BazarCategoryWeapon)}</option
	>
	<option value={BazarCategory.Armour}
		>{get_const_string(ConstStringKey.BazarCategoryArmour)}</option
	>
	<option value={BazarCategory.Equipment}
		>{get_const_string(ConstStringKey.BazarCategoryEquipment)}</option
	>
	<option value={BazarCategory.Accessories}
		>{get_const_string(ConstStringKey.BazarCategoryAccessories)}</option
	>
	<option value={BazarCategory.Specialist}>{get_const_string(ConstStringKey.Specialist)}</option>
	<option value={BazarCategory.Pet}>{get_const_string(ConstStringKey.BazarCategoryPet)}</option>
	<option value={BazarCategory.Partner}
		>{get_const_string(ConstStringKey.BazarCategoryPartner)}</option
	>
	<option value={BazarCategory.StoreMount}>{get_const_string(ConstStringKey.StoreMount)}</option>
	<option value={BazarCategory.Shell}>{get_const_string(ConstStringKey.BazarCategoryShell)}</option>
	<option value={BazarCategory.MainItem}
		>{get_const_string(ConstStringKey.BazarCategoryMainItem)}</option
	>
	<option value={BazarCategory.ConsumerItem}
		>{get_const_string(ConstStringKey.BazarCategoryConsumerItem)}</option
	>
	<option value={BazarCategory.Miscellaneous}
		>{get_const_string(ConstStringKey.BazarCategoryMiscellaneous)}</option
	>
</select>

<style>
</style>
