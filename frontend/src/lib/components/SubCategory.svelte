<script lang="ts">
	import * as enums from '$lib/types/enums';
	import DynamicSelect from './DynamicSelect.svelte';
	import { ConstStringKey } from '$lib/types/constStringKeys';
	import { get_const_string } from '$lib/services/constStringService.svelte';

	type Props = {
		value?: number;
		category: enums.BazarCategory;
	};

	let { value = $bindable(0), category }: Props = $props();

	const weaponOptions = $derived([
		{ label: get_const_string(ConstStringKey.All), value: enums.WeaponSubcategory.All },
		{
			label: get_const_string(ConstStringKey.BazarSubcategorySwordsman),
			value: enums.WeaponSubcategory.Swordsman
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryArcher),
			value: enums.WeaponSubcategory.Archer
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryMagician),
			value: enums.WeaponSubcategory.Magician
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryAdventurer),
			value: enums.WeaponSubcategory.Adventurer
		},
		{
			label: get_const_string(ConstStringKey.MartialArtist),
			value: enums.WeaponSubcategory.MartialArtist
		}
	]);

	const armourOptions = $derived([
		{ label: get_const_string(ConstStringKey.All), value: enums.ArmourSubcategory.All },
		{
			label: get_const_string(ConstStringKey.BazarSubcategorySwordsman),
			value: enums.ArmourSubcategory.Swordsman
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryArcher),
			value: enums.ArmourSubcategory.Archer
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryMagician),
			value: enums.ArmourSubcategory.Magician
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryAdventurer),
			value: enums.ArmourSubcategory.Adventurer
		},
		{
			label: get_const_string(ConstStringKey.MartialArtist),
			value: enums.ArmourSubcategory.MartialArtist
		}
	]);

	const equipmentOptions = $derived([
		{ label: get_const_string(ConstStringKey.All), value: enums.EquipmentSubcategory.All },
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryHat),
			value: enums.EquipmentSubcategory.Hat
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryAccessory),
			value: enums.EquipmentSubcategory.Accessory
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryGloves),
			value: enums.EquipmentSubcategory.Gloves
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryShoes),
			value: enums.EquipmentSubcategory.Shoes
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryCostume),
			value: enums.EquipmentSubcategory.Costume
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryCostumeHat),
			value: enums.EquipmentSubcategory.CostumeHat
		},
		{
			label:
				get_const_string(ConstStringKey.Costume) + ' ' + get_const_string(ConstStringKey.Weapon),
			value: enums.EquipmentSubcategory.CostumeWeapon
		},
		{
			label: get_const_string(ConstStringKey.BazarCostumeWings),
			value: enums.EquipmentSubcategory.CostumeWings
		},
		{
			label: get_const_string(ConstStringKey.BazarCostumeFemale),
			value: enums.EquipmentSubcategory.CostumeFemale
		},
		{
			label: get_const_string(ConstStringKey.BazarCostumeHatFemale),
			value: enums.EquipmentSubcategory.CostumeHatFemale
		},
		{
			label: get_const_string(ConstStringKey.BazarCostumeMale),
			value: enums.EquipmentSubcategory.CostumeMale
		},
		{
			label: get_const_string(ConstStringKey.BazarCostumeHatMale),
			value: enums.EquipmentSubcategory.CostumeHatMale
		}
	]);

	const accessoriesOptions = $derived([
		{ label: get_const_string(ConstStringKey.All), value: enums.AccessoriesSubcategory.All },
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryNecklace),
			value: enums.AccessoriesSubcategory.Necklace
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryRing),
			value: enums.AccessoriesSubcategory.Ring
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryBracelet),
			value: enums.AccessoriesSubcategory.Bracelet
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryFairy),
			value: enums.AccessoriesSubcategory.Fairy
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryAmulet),
			value: enums.AccessoriesSubcategory.Amulet
		},
		{ label: get_const_string(ConstStringKey.MiniPet), value: enums.AccessoriesSubcategory.MiniPet }
	]);

	const specialistOptions = $derived([
		{ label: get_const_string(ConstStringKey.All), value: enums.SpecialistSubcategory.All },
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryEmptyCardHolder),
			value: enums.SpecialistSubcategory.EmptyCardHolder
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryCrusader),
			value: enums.SpecialistSubcategory.Crusader
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryBerserker),
			value: enums.SpecialistSubcategory.Berserker
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryWarrior),
			value: enums.SpecialistSubcategory.Warrior
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryNinja),
			value: enums.SpecialistSubcategory.Ninja
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryWildKeeper),
			value: enums.SpecialistSubcategory.WildKeeper
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryAssassin),
			value: enums.SpecialistSubcategory.Assasin
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryDestroyer),
			value: enums.SpecialistSubcategory.Destroyer
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryRanger),
			value: enums.SpecialistSubcategory.Ranger
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryHolyMage),
			value: enums.SpecialistSubcategory.HolyMage
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryDarkGunner),
			value: enums.SpecialistSubcategory.DarkGunner
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryRedMagician),
			value: enums.SpecialistSubcategory.RedMagician
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryBlueMagician),
			value: enums.SpecialistSubcategory.BlueMagician
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryJajamaru),
			value: enums.SpecialistSubcategory.Jajamaru
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryChickenCostume),
			value: enums.SpecialistSubcategory.ChickenCostume
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryPyjama),
			value: enums.SpecialistSubcategory.Pyjama
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryPirate),
			value: enums.SpecialistSubcategory.Pirate
		},
		{
			label: get_const_string(ConstStringKey.Gladiator),
			value: enums.SpecialistSubcategory.Gladiator
		},
		{
			label: get_const_string(ConstStringKey.FireCanonneer),
			value: enums.SpecialistSubcategory.FireCannoneer
		},
		{ label: get_const_string(ConstStringKey.Volcano), value: enums.SpecialistSubcategory.Volcano },
		{
			label: get_const_string(ConstStringKey.BattleMonk),
			value: enums.SpecialistSubcategory.BattleMonk
		},
		{ label: get_const_string(ConstStringKey.Scout), value: enums.SpecialistSubcategory.Scout },
		{
			label: get_const_string(ConstStringKey.TideLord),
			value: enums.SpecialistSubcategory.TideLord
		},
		{
			label: get_const_string(ConstStringKey.DeathReaper),
			value: enums.SpecialistSubcategory.DeathReaper
		},
		{
			label: get_const_string(ConstStringKey.DemonHunter),
			value: enums.SpecialistSubcategory.DemonHunter
		},
		{ label: get_const_string(ConstStringKey.Seer), value: enums.SpecialistSubcategory.Seer },
		{
			label: get_const_string(ConstStringKey.Renegade),
			value: enums.SpecialistSubcategory.Renegade
		},
		{
			label: get_const_string(ConstStringKey.AvengingAngel),
			value: enums.SpecialistSubcategory.AvengingAngel
		},
		{
			label: get_const_string(ConstStringKey.Archmage),
			value: enums.SpecialistSubcategory.Archmage
		},
		{
			label: get_const_string(ConstStringKey.DraconicFist),
			value: enums.SpecialistSubcategory.DragonicFist
		},
		{
			label: get_const_string(ConstStringKey.MysticArts),
			value: enums.SpecialistSubcategory.MysticArts
		},
		{
			label: get_const_string(ConstStringKey.WeddingCostume),
			value: enums.SpecialistSubcategory.WeddingCostume
		},
		{
			label: get_const_string(ConstStringKey.MasterWolf),
			value: enums.SpecialistSubcategory.MasterWolf
		},
		{
			label: get_const_string(ConstStringKey.DemonWarrior),
			value: enums.SpecialistSubcategory.DemonWarrior
		},
		{
			label: get_const_string(ConstStringKey.WaterfallBerserker),
			value: enums.SpecialistSubcategory.WaterfallBerserker
		},
		{
			label: get_const_string(ConstStringKey.Sunchaser),
			value: enums.SpecialistSubcategory.Sunchaser
		},
		{
			label: get_const_string(ConstStringKey.VodooPriest),
			value: enums.SpecialistSubcategory.VoodooPriest
		},
		{
			label: get_const_string(ConstStringKey.FlameDruid),
			value: enums.SpecialistSubcategory.FlameDruid
		},
		{
			label: get_const_string(ConstStringKey.DragonKnight),
			value: enums.SpecialistSubcategory.DragonKnight
		},
		{ label: get_const_string(ConstStringKey.Blaster), value: enums.SpecialistSubcategory.Blaster },
		{ label: get_const_string(ConstStringKey.Gravity), value: enums.SpecialistSubcategory.Gravity },
		{
			label: get_const_string(ConstStringKey.HydraulicFist),
			value: enums.SpecialistSubcategory.HydraulicFist
		},
		{
			label: get_const_string(ConstStringKey.StoneBreaker),
			value: enums.SpecialistSubcategory.StoneBreaker
		},
		{
			label: get_const_string(ConstStringKey.FogHunter),
			value: enums.SpecialistSubcategory.FogHunter
		},
		{
			label: get_const_string(ConstStringKey.FireStorm),
			value: enums.SpecialistSubcategory.FireStorm
		},
		{
			label: get_const_string(ConstStringKey.Thunderer),
			value: enums.SpecialistSubcategory.Thunderer
		},
		{
			label: get_const_string(ConstStringKey.Achilles),
			value: enums.SpecialistSubcategory.Achilles
		},
		{
			label: get_const_string(ConstStringKey.AdmiralYi),
			value: enums.SpecialistSubcategory.AdmiralYi
		},
		{
			label: get_const_string(ConstStringKey.Merlin),
			value: enums.SpecialistSubcategory.Merlin
		},
		{
			label: get_const_string(ConstStringKey.Thor),
			value: enums.SpecialistSubcategory.Thor
		}
	]);

	const petOptions = $derived([
		{ label: get_const_string(ConstStringKey.All), value: enums.PetSubcategory.All },
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryEmptyPetBead),
			value: enums.PetSubcategory.EmptyPetBead
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryPetBead),
			value: enums.PetSubcategory.PetBead
		}
	]);

	const partnerOptions = $derived([
		{ label: get_const_string(ConstStringKey.All), value: enums.PartnerSubcategory.All },
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryEmptyPartnerBead),
			value: enums.PartnerSubcategory.EmptyPartnerBead
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryPartnerBead),
			value: enums.PartnerSubcategory.PartnerBead
		},
		{
			label: get_const_string(ConstStringKey.EmptyCardHolder),
			value: enums.PartnerSubcategory.EmptyCardHolder
		},
		{
			label: get_const_string(ConstStringKey.CloseAttack),
			value: enums.PartnerSubcategory.CloseAttack
		},
		{
			label: get_const_string(ConstStringKey.RemoteAttack),
			value: enums.PartnerSubcategory.RemoteAttack
		},
		{ label: get_const_string(ConstStringKey.PartnerMagic), value: enums.PartnerSubcategory.Magic }
	]);

	const storeMountOptions = $derived([
		{ label: get_const_string(ConstStringKey.All), value: enums.StoreMountSubcategory.All },
		{
			label: get_const_string(ConstStringKey.EmptyMountBead),
			value: enums.StoreMountSubcategory.EmptyMountBead
		},
		{
			label: get_const_string(ConstStringKey.MountBead),
			value: enums.StoreMountSubcategory.MountBead
		}
	]);

	const shellOptions = $derived([
		{ label: get_const_string(ConstStringKey.All), value: enums.ShellSubcategory.All },
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryWeapon),
			value: enums.ShellSubcategory.Weapon
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryClothing),
			value: enums.ShellSubcategory.Clothing
		}
	]);

	const mainItemOptions = $derived([
		{ label: get_const_string(ConstStringKey.All), value: enums.MainItemSubcategory.All },
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryGeneralItems),
			value: enums.MainItemSubcategory.GeneralItems
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryMaterial),
			value: enums.MainItemSubcategory.Material
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryProductionItem),
			value: enums.MainItemSubcategory.ProductionItem
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategorySpecialItems),
			value: enums.MainItemSubcategory.SpecialItems
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryHealingPotion),
			value: enums.MainItemSubcategory.HealingPotion
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryEvent),
			value: enums.MainItemSubcategory.Event
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryTitle),
			value: enums.MainItemSubcategory.Title
		}
	]);

	const consumerItemOptions = $derived([
		{ label: get_const_string(ConstStringKey.All), value: enums.ConsumerItemSubcategory.All },
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryFood),
			value: enums.ConsumerItemSubcategory.Food
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategorySnack),
			value: enums.ConsumerItemSubcategory.Snack
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryMagicItem),
			value: enums.ConsumerItemSubcategory.MagicItem
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryIngredients),
			value: enums.ConsumerItemSubcategory.Ingredients
		},
		{
			label: get_const_string(ConstStringKey.BazarSubcategoryPartnerItem),
			value: enums.ConsumerItemSubcategory.PartnerItem
		},
		{
			label: get_const_string(ConstStringKey.SaleItem),
			value: enums.ConsumerItemSubcategory.SaleItem
		},
		{ label: get_const_string(ConstStringKey.Fish), value: enums.ConsumerItemSubcategory.Fish }
	]);

	const optionsByCategory = $derived({
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
	});

	const options = $derived(optionsByCategory[category]);
</script>

<DynamicSelect bind:value {options} />
