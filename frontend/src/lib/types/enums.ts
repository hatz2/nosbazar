enum BazarCategory {
	All = 0,
	Weapon = 1,
	Armour = 2,
	Equipment = 3,
	Accessories = 4,
	Specialist = 5,
	Pet = 6,
	Partner = 7,
	StoreMount = 12,
	Shell = 8,
	MainItem = 9,
	ConsumerItem = 10,
	Miscellaneous = 11
}

enum WeaponSubcategory {
	All = 0,
	Swordsman = 1,
	Archer = 2,
	Magician = 3,
	Adventurer = 4,
	MartialArtist = 5
}

enum ArmourSubcategory {
	All = 0,
	Swordsman = 1,
	Archer = 2,
	Magician = 3,
	Adventurer = 4,
	MartialArtist = 5
}

enum EquipmentSubcategory {
	All = 0,
	Hat = 1,
	Accessory = 2,
	Gloves = 3,
	Shoes = 4,
	Costume = 5,
	CostumeHat = 6,
	CostumeWeapon = 7,
	CostumeWings = 8,
	CostumeFemale = 9,
	CostumeHatFemale = 10,
	CostumeMale = 11,
	CostumeHatMale = 12
}

enum AccessoriesSubcategory {
	All = 0,
	Necklace = 1,
	Ring = 2,
	Bracelet = 3,
	Fairy = 4,
	Amulet = 5,
	MiniPet = 6
}

enum SpecialistSubcategory {
	All = 0,
	EmptyCardHolder = 1,
	Crusader = 2,
	Berserker = 3,
	Warrior = 4,
	Ninja = 5,
	WildKeeper = 6,
	Assasin = 7,
	Destroyer = 8,
	Ranger = 9,
	HolyMage = 10,
	DarkGunner = 11,
	RedMagician = 12,
	BlueMagician = 13,
	Jajamaru = 14,
	ChickenCostume = 15,
	Pyjama = 16,
	Pirate = 17,
	Gladiator = 18,
	FireCannoneer = 19,
	Volcano = 20,
	BattleMonk = 21,
	Scout = 22,
	TideLord = 23,
	DeathReaper = 24,
	DemonHunter = 25,
	Seer = 26,
	Renegade = 27,
	AvengingAngel = 28,
	Archmage = 29,
	DragonicFist = 30,
	MysticArts = 32,
	WeddingCostume = 33,
	MasterWolf = 34,
	DemonWarrior = 35,
	WaterfallBerserker = 40,
	Sunchaser = 41,
	VoodooPriest = 42,
	FlameDruid = 43,
	DragonKnight = 46,
	Blaster = 47,
	Gravity = 48,
	HydraulicFist = 49,
	StoneBreaker = 52,
	FogHunter = 53,
	FireStorm = 54,
	Thunderer = 55,
	Achilles = 56,
	AdmiralYi = 57,
	Merlin = 58,
	Thor = 59,
}

enum PetSubcategory {
	All = 0,
	EmptyPetBead = 1,
	PetBead = 2
}

enum PartnerSubcategory {
	All = 0,
	EmptyPartnerBead = 1,
	PartnerBead = 2,
	EmptyCardHolder = 3,
	CloseAttack = 4,
	RemoteAttack = 5,
	Magic = 6
}

enum StoreMountSubcategory {
	All = 0,
	EmptyMountBead = 1,
	MountBead = 2
}

enum ShellSubcategory {
	All = 0,
	Weapon = 1,
	Clothing = 2
}

enum MainItemSubcategory {
	All = 0,
	GeneralItems = 1,
	Material = 2,
	ProductionItem = 3,
	SpecialItems = 4,
	HealingPotion = 5,
	Event = 6,
	Title = 7
}

enum ConsumerItemSubcategory {
	All = 0,
	Food = 1,
	Snack = 2,
	MagicItem = 3,
	Ingredients = 4,
	PartnerItem = 5,
	SaleItem = 6,
	Fish = 7
}

enum SortFilter {
	PriceAscending = 0,
	PriceDescending = 1,
	AmountAscending = 2,
	AmountDescending = 3
}

enum RequiredClass {
	All = 0,
	Adventurer = 1,
	Swordsman = 2,
	Archer = 4,
	Mage = 8,
	Martial = 16,
	AllButAdventurer = 30
}

enum Element {
	NoElement = 0,
	Fire = 1,
	Water = 2,
	Light = 3,
	Shadow = 4
}

const PartnerSkillLevels: Record<number, string> = {
	1: 'F',
	2: 'E',
	3: 'D',
	4: 'C',
	5: 'B',
	6: 'A',
	7: 'S'
};

enum WeaponShellOption {
	EnhancedDmg = 1,
	IncreasedDmg = 2,
	MinorBleeding = 3,
	Bleeding = 4,
	HeavyBleeding = 5,
	Blackout = 6,
	Freeze = 7,
	DeadlyBlackout = 8,
	IncreasedDmgToPlants = 9,
	IncreasedDmgToAnimals = 10,
	IncreasedDmgToMonsters = 11,
	IncreasedDmgToUndeads = 12,
	IncreasedDmgToKovolts = 13,
	IncreasedDmgToMapBosses = 14,
	IncreasedCritChance = 15,
	IncreasedCritDmg = 16,
	UndisturbedCasting = 17,
	IncreasedFireElement = 18,
	IncreasedWaterElement = 19,
	IncreasedLightElement = 20,
	IncreasedShadowElement = 21,
	IncreasedAllElements = 22,
	ReducedMpConsumption = 23,
	HpRecoveryPerKill = 24,
	MpRecoveryPerKill = 25,
	IncreasedSlDmgStat = 26,
	IncreasedSlDefenseStat = 27,
	IncreasedSlPropertyStat = 28,
	IncreasedSlEnergyStat = 29,
	IncreasedOverallSlStat = 30,
	GainMoreGold = 31,
	IncreasedCombatExp = 32,
	IncreasedJobExp = 33,
	IncreasedPvpDmg = 34,
	ReducedPvpDefence = 35,
	ReducedPvpFireResistance = 36,
	ReducedPvpWaterResistance = 37,
	ReducedPvpLightResistance = 38,
	ReducedPvpShadowResistance = 39,
	ReducedPvpAllResistances = 40,
	HitEverytimeInPvp = 41,
	PercentDmgAt15InPvp = 42,
	ReducedPvpManaPerHit = 43,
	IgnorePvpFireResistance = 44,
	IgnorePvpWaterResistance = 45,
	IgnorePvpLightResistance = 46,
	IgnorePvpShadowResistance = 47,
	SpRecoveryPerKill = 48,
	IncreasedAccuracy = 49,
	IncreasedConcentration = 50,
}

enum ArmorShellOption {
	EnhancedMeleeDefense = 1,
	EnhancedLongRangeDefense = 2,
	EnhancedMagicDefense = 3,
	IncreasedOverallDefence = 4,
	ReducedMinorBleedingChance = 5,
	ReducedBleedingChance = 6,
	ReducedAllBleedingChance = 7,
	ReducedBlackoutChance = 8,
	ReducedAllBlackoutChance = 9,
	ReducedHandOfDeathChance = 10,
	ReducedFreezeChance = 11,
	ReducedBlindChance = 12,
	ReducedBindChance = 13,
	ReducedWeakenDefenceChance = 14,
	ReducedShockChance = 15,
	ReducedParalysisChance = 16,
	ReducedAllNegativeEffectsChance = 17,
	IncreasedHpRecoveryWhileResting = 18,
	IncreasedNaturalHpRecovery = 19,
	IncreasedMpRecoveryWhileResting = 20,
	IncreasedNaturalMpRecovery = 21,
	RecoverDmgAsHp = 22,
	ReducedCritChance = 23,
	IncreasedFireResistance = 24,
	IncreasedWaterResistance = 25,
	IncreasedLightResistance = 26,
	IncreasedShadowResistance = 27,
	IncreasedAllResistances = 28,
	ReducedDignityLoss = 29,
	ReducedProductionPointConsumption = 30,
	MoreMinigameRewards = 31,
	IncreasedItemRecovery = 32,
	IncreasedPvpOverallDefence = 33,
	DodgePvpMelee = 34,
	DodgePvpRanged = 35,
	DodgePvpMagic = 36,
	DodgePvpAll = 37,
	ManaDamageProtection = 38,
	ImmuneToPvpFireDamage = 39,
	ImmuneToPvpWaterDamage = 40,
	ImmuneToPvpLightDamage = 41,
	ImmuneToPvpShadowDamage = 42,
}

export {
	BazarCategory,
	WeaponSubcategory,
	ArmourSubcategory,
	EquipmentSubcategory,
	AccessoriesSubcategory,
	SpecialistSubcategory,
	PetSubcategory,
	PartnerSubcategory,
	StoreMountSubcategory,
	ShellSubcategory,
	MainItemSubcategory,
	ConsumerItemSubcategory,
	SortFilter,
	RequiredClass,
	Element,
	PartnerSkillLevels,
	WeaponShellOption,
	ArmorShellOption
};
