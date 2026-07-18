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
	MysticArts = 31,
	WeddingCostume = 32,
	MasterWolf = 33,
	DemonWarrior = 34,
	WaterfallBerserker = 35,
	Sunchaser = 36,
	VoodooPriest = 37,
	FlameDruid = 38,
	DragonKnight = 39,
	Blaster = 40,
	Gravity = 41,
	HydraulicFist = 42,
	StoneBreaker = 43,
	FogHunter = 44,
	FireStorm = 45,
	Thunderer = 46
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
	Element
};
