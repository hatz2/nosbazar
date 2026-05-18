<script lang="ts">
	import * as enums from '$lib/types/enums';
	import DynamicSelect from './DynamicSelect.svelte';

	type Props = {
		value?: number;
		category: enums.BazarCategory;
	};

	let { value = $bindable(0), category }: Props = $props();

	const weaponOptions = [
		{ label: 'All', value: enums.WeaponSubcategory.All },
		{ label: 'Swordsman', value: enums.WeaponSubcategory.Swordsman },
		{ label: 'Archer', value: enums.WeaponSubcategory.Archer },
		{ label: 'Magician', value: enums.WeaponSubcategory.Magician },
		{ label: 'Adventurer', value: enums.WeaponSubcategory.Adventurer },
		{ label: 'Martial Artist', value: enums.WeaponSubcategory.MartialArtist }
	];

	const armourOptions = [
		{ label: 'All', value: enums.ArmourSubcategory.All },
		{ label: 'Swordsman', value: enums.ArmourSubcategory.Swordsman },
		{ label: 'Archer', value: enums.ArmourSubcategory.Archer },
		{ label: 'Magician', value: enums.ArmourSubcategory.Magician },
		{ label: 'Adventurer', value: enums.ArmourSubcategory.Adventurer },
		{ label: 'Martial Artist', value: enums.ArmourSubcategory.MartialArtist }
	];

	const equipmentOptions = [
		{ label: 'All', value: enums.EquipmentSubcategory.All },
		{ label: 'Hat', value: enums.EquipmentSubcategory.Hat },
		{ label: 'Accessory', value: enums.EquipmentSubcategory.Accessory },
		{ label: 'Gloves', value: enums.EquipmentSubcategory.Gloves },
		{ label: 'Shoes', value: enums.EquipmentSubcategory.Shoes },
		{ label: 'Costume', value: enums.EquipmentSubcategory.Costume },
		{ label: 'Costume Hat', value: enums.EquipmentSubcategory.CostumeHat },
		{ label: 'Costume Weapon', value: enums.EquipmentSubcategory.CostumeWeapon },
		{ label: 'Costume Wings', value: enums.EquipmentSubcategory.CostumeWings },
		{ label: 'Costume (f)', value: enums.EquipmentSubcategory.CostumeFemale },
		{ label: 'Costume Hat (f)', value: enums.EquipmentSubcategory.CostumeHatFemale },
		{ label: 'Costume (m)', value: enums.EquipmentSubcategory.CostumeMale },
		{ label: 'Costume Hat (m)', value: enums.EquipmentSubcategory.CostumeHatMale }
	];

	const accessoriesOptions = [
		{ label: 'All', value: enums.AccessoriesSubcategory.All },
		{ label: 'Necklace', value: enums.AccessoriesSubcategory.Necklace },
		{ label: 'Ring', value: enums.AccessoriesSubcategory.Ring },
		{ label: 'Bracelet', value: enums.AccessoriesSubcategory.Bracelet },
		{ label: 'Fairy', value: enums.AccessoriesSubcategory.Fairy },
		{ label: 'Amulet', value: enums.AccessoriesSubcategory.Amulet },
		{ label: 'Mini Pet', value: enums.AccessoriesSubcategory.MiniPet }
	];

	const specialistOptions = [
		{ label: 'All', value: enums.SpecialistSubcategory.All },
		{ label: 'Empty card holder', value: enums.SpecialistSubcategory.EmptyCardHolder },
		{ label: 'Crusader', value: enums.SpecialistSubcategory.Crusader },
		{ label: 'Berserker', value: enums.SpecialistSubcategory.Berserker },
		{ label: 'Warrior', value: enums.SpecialistSubcategory.Warrior },
		{ label: 'Ninja', value: enums.SpecialistSubcategory.Ninja },
		{ label: 'Wild Keeper', value: enums.SpecialistSubcategory.WildKeeper },
		{ label: 'Assasin', value: enums.SpecialistSubcategory.Assasin },
		{ label: 'Destroyer', value: enums.SpecialistSubcategory.Destroyer },
		{ label: 'Ranger', value: enums.SpecialistSubcategory.Ranger },
		{ label: 'Holy Mage', value: enums.SpecialistSubcategory.HolyMage },
		{ label: 'Dark Gunner', value: enums.SpecialistSubcategory.DarkGunner },
		{ label: 'Red Magician', value: enums.SpecialistSubcategory.RedMagician },
		{ label: 'Blue Magician', value: enums.SpecialistSubcategory.BlueMagician },
		{ label: 'Jajamaru', value: enums.SpecialistSubcategory.Jajamaru },
		{ label: 'Chicken Costume', value: enums.SpecialistSubcategory.ChickenCostume },
		{ label: 'Pyjama', value: enums.SpecialistSubcategory.Pyjama },
		{ label: 'Pirate', value: enums.SpecialistSubcategory.Pirate },
		{ label: 'Gladiator', value: enums.SpecialistSubcategory.Gladiator },
		{ label: 'Fire Cannoneer', value: enums.SpecialistSubcategory.FireCannoneer },
		{ label: 'Volcano', value: enums.SpecialistSubcategory.Volcano },
		{ label: 'Battle Monk', value: enums.SpecialistSubcategory.BattleMonk },
		{ label: 'Scout', value: enums.SpecialistSubcategory.Scout },
		{ label: 'Tide Lord', value: enums.SpecialistSubcategory.TideLord },
		{ label: 'Death Reaper', value: enums.SpecialistSubcategory.DeathReaper },
		{ label: 'Demon Hunter', value: enums.SpecialistSubcategory.DemonHunter },
		{ label: 'Seer', value: enums.SpecialistSubcategory.Seer },
		{ label: 'Renegade', value: enums.SpecialistSubcategory.Renegade },
		{ label: 'Avenging Angel', value: enums.SpecialistSubcategory.AvengingAngel },
		{ label: 'Archmage', value: enums.SpecialistSubcategory.Archmage },
		{ label: 'Draconic Fist', value: enums.SpecialistSubcategory.DragonicFist },
		{ label: 'Mystic Arts', value: enums.SpecialistSubcategory.MysticArts },
		{ label: 'Wedding Costume', value: enums.SpecialistSubcategory.WeddingCostume },
		{ label: 'Master Wolf', value: enums.SpecialistSubcategory.MasterWolf },
		{ label: 'Demon Warrior', value: enums.SpecialistSubcategory.DemonWarrior },
		{ label: 'Waterfall Berserker', value: enums.SpecialistSubcategory.WaterfallBerserker },
		{ label: 'Sunchaser', value: enums.SpecialistSubcategory.Sunchaser },
		{ label: 'Voodoo Priest', value: enums.SpecialistSubcategory.VoodooPriest },
		{ label: 'Flame Druid', value: enums.SpecialistSubcategory.FlameDruid },
		{ label: 'Dragon Knight', value: enums.SpecialistSubcategory.DragonKnight },
		{ label: 'Blaster', value: enums.SpecialistSubcategory.Blaster },
		{ label: 'Gravity', value: enums.SpecialistSubcategory.Gravity },
		{ label: 'Hydraulic Fist', value: enums.SpecialistSubcategory.HydraulicFist },
		{ label: 'Stone Breaker', value: enums.SpecialistSubcategory.StoneBreaker },
		{ label: 'FogHunter', value: enums.SpecialistSubcategory.FogHunter },
		{ label: 'Fire Storm', value: enums.SpecialistSubcategory.FireStorm },
		{ label: 'Thunderer', value: enums.SpecialistSubcategory.Thunderer }
	];

	const petOptions = [
		{ label: 'All', value: enums.PetSubcategory.All },
		{ label: 'Empty Pet Bead', value: enums.PetSubcategory.EmptyPetBead },
		{ label: 'Pet Bead', value: enums.PetSubcategory.PetBead }
	];

	const partnerOptions = [
		{ label: 'All', value: enums.PartnerSubcategory.All },
		{ label: 'Partner Bead', value: enums.PartnerSubcategory.PartnerBead },
		{ label: 'Empty Card Holder', value: enums.PartnerSubcategory.EmptyCardHolder },
		{ label: 'Close Attack', value: enums.PartnerSubcategory.CloseAttack },
		{ label: 'Remote Attack', value: enums.PartnerSubcategory.RemoteAttack },
		{ label: 'Magic', value: enums.PartnerSubcategory.Magic }
	];

	const storeMountOptions = [
		{ label: 'All', value: enums.StoreMountSubcategory.All },
		{ label: 'Empty Mount Bead', value: enums.StoreMountSubcategory.EmptyMountBead },
		{ label: 'Mount Bead', value: enums.StoreMountSubcategory.MountBead }
	];

	const shellOptions = [
		{ label: 'All', value: enums.ShellSubcategory.All },
		{ label: 'Weapon', value: enums.ShellSubcategory.Weapon },
		{ label: 'Clothing', value: enums.ShellSubcategory.Clothing }
	];

	const mainItemOptions = [
		{ label: 'All', value: enums.MainItemSubcategory.All },
		{ label: 'General items', value: enums.MainItemSubcategory.GeneralItems },
		{ label: 'Material', value: enums.MainItemSubcategory.Material },
		{ label: 'Production item', value: enums.MainItemSubcategory.ProductionItem },
		{ label: 'Special items', value: enums.MainItemSubcategory.SpecialItems },
		{ label: 'Healing potion', value: enums.MainItemSubcategory.HealingPotion },
		{ label: 'Event', value: enums.MainItemSubcategory.Event },
		{ label: 'Title', value: enums.MainItemSubcategory.Title }
	];

	const consumerItemOptions = [
		{ label: 'All', value: enums.ConsumerItemSubcategory.All },
		{ label: 'Food', value: enums.ConsumerItemSubcategory.Food },
		{ label: 'Snack', value: enums.ConsumerItemSubcategory.Snack },
		{ label: 'Magic item', value: enums.ConsumerItemSubcategory.MagicItem },
		{ label: 'Ingredients', value: enums.ConsumerItemSubcategory.Ingredients },
		{ label: 'Partner item', value: enums.ConsumerItemSubcategory.PartnerItem },
		{ label: 'Sale item', value: enums.ConsumerItemSubcategory.SaleItem },
		{ label: 'Fish', value: enums.ConsumerItemSubcategory.Fish }
	];

	const optionsByCategory = {
		[enums.BazarCategory.All]: [],
		[enums.BazarCategory.Weapon]: weaponOptions,
		[enums.BazarCategory.Armour]: armourOptions,
		[enums.BazarCategory.Equipment]: equipmentOptions,
		[enums.BazarCategory.Accessories]: accessoriesOptions,
		[enums.BazarCategory.Specialist]: specialistOptions,
		[enums.BazarCategory.Pet]: petOptions,
		[enums.BazarCategory.Partner]: partnerOptions,
		[enums.BazarCategory.StoreMount]: storeMountOptions,
		[enums.BazarCategory.Shell]: shellOptions,
		[enums.BazarCategory.MainItem]: mainItemOptions,
		[enums.BazarCategory.ConsumerItem]: consumerItemOptions,
		[enums.BazarCategory.Miscellaneous]: []
	};

	const options = $derived(optionsByCategory[category]);
</script>

<DynamicSelect bind:value {options} />
