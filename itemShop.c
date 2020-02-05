#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define INVENTORY_CAPACITY 10

typedef struct Item Item;
struct Item {
	char name[64];
	int price;
};

typedef struct Character Character;
struct Character {
	char name[64];
	char status[64];
	int gold;
	Item inventory[INVENTORY_CAPACITY];
	int inventorySize;
	int inventoryCapacity;
};

void displayCharacter(Character);
void displayInventory(Character);
void sortInventory(Character*, int);
void sortInventoryByName(Item*, int);
void sortInventoryByPrice(Item*, int);
void addItem(Character*, Item);
void removeItem(Character*, Item);
void buy(Character*, Character*, Item);
Item createItem(char*, int);

int main() {
	int entry = 0;

	Character player = {
		.name = "Siegfried",
		.status = "Chevalier",
		.gold = 50,
		.inventory = {
			{"Caillou", 1},
			{"Barre a mine", 10}
		},
		.inventorySize = 2,
		.inventoryCapacity = INVENTORY_CAPACITY
	};
	sortInventory(&player, 1);

	Character merchant = {
		.name = "Boris",
		.status = "Marchant",
		.gold = 1000,
		.inventory = {
			{"Potion de vie", 25},
			{"Potion de mana", 35},
			{"Antidote", 15},
			{"Collyre", 10},
			{"Laxatif", 5}
		},
		.inventorySize = 5,
		.inventoryCapacity = INVENTORY_CAPACITY
	};
	sortInventory(&merchant, 2);
	
	do {
		displayCharacter(player);
		printf("\n");

		displayCharacter(merchant);
		printf("\n");

		printf("1 ACHETER 2 AJOUTER 0 QUITTER\n");
		scanf("%d", &entry);
		printf("\n");

		switch (entry) {
			// ACHETER
			case 1:
				do {
					// Aucune marchandise
					if (merchant.inventorySize <= 0) {
						printf("%s n'a rien a vous vendre.\n", merchant.name);
						entry = 0;
					// Plus de place dans l'inventaire
					} else if (player.inventorySize >= player.inventoryCapacity) {
						printf("Vous ne pouvez plus rien transporter.\n");
						entry = 0;
					// Choix d'objet
					} else {
						printf("Votre or : %d PO\n", player.gold);
						displayInventory(merchant);
						printf("\n");
						printf("Quel est votre choix ? (0 QUITTER)\n");
						scanf("%d", &entry);
						if (entry >= 1 && entry <= merchant.inventorySize) {
							if (player.gold < merchant.inventory[entry-1].price) {
								printf("C'est trop cher !\n");
							} else {
								buy(&player, &merchant, merchant.inventory[entry-1]);
							}
						}
					}
					printf("\n");
				} while (entry != 0);
				entry = -1;
				break;
			// AJOUTER
			case 2:
				printf("Nombre d'objet du marchant : %d\n", merchant.inventorySize);
				char newItem_name[64];
				int newItem_price;

				printf("Nouvel objet\n");
				printf("Nom : ");
				scanf("%63s", newItem_name);
				printf("Prix : ");
				scanf("%d", &newItem_price);
				printf("\n");

				Item newItem = createItem(newItem_name, newItem_price);
				addItem(&merchant, newItem);
				displayInventory(merchant);
				printf("\n");
			default:;
		}
	} while (entry != 0);

	return 0;
}

void displayCharacter(Character character) {
	printf("%s %s\n", character.status, character.name);
	printf("Gold : %d PO\n", character.gold);
	printf("Inventaire :\n");
	for (int i=0; i<character.inventorySize; i++) {
		printf(" %d. %s %dPO\n", i+1, character.inventory[i].name, character.inventory[i].price);
	}
}

void displayInventory(Character character) {
	printf("Inventaire de %s : %d/%d\n", character.name, character.inventorySize, character.inventoryCapacity);
	for (int i=0; i<character.inventorySize; i++) {
		printf(" %d. %s %d PO\n", i+1, character.inventory[i].name, character.inventory[i].price);
	}
}

void sortInventory(Character* character, int by) {
	switch (by) {
		case 1:
			sortInventoryByName((*character).inventory, (*character).inventorySize);
			break;
		case 2:
			sortInventoryByPrice((*character).inventory, (*character).inventorySize);
			break;
		default:
			sortInventoryByName((*character).inventory, (*character).inventorySize);
	}
}

void sortInventoryByName(Item* inventory, int inventorySize) {
	Item itemTmp;
	for (int end=inventorySize-1; end>0; end--) {
		for (int i=0; i<end; i++) {
			if (strcmp(inventory[i].name, inventory[i+1].name) > 0) {
				itemTmp = inventory[i];
				inventory[i] = inventory[i+1];
				inventory[i+1] = itemTmp;
			}
		}
	}
}

void sortInventoryByPrice(Item* inventory, int inventorySize) {
	Item itemTmp;
	for (int end=inventorySize-1; end>0; end--) {
		for (int i=0; i<end; i++) {
			if (inventory[i].price < inventory[i+1].price) {
				itemTmp = inventory[i];
				inventory[i] = inventory[i+1];
				inventory[i+1] = itemTmp;
			}
		}
	}
}

void addItem(Character* character, Item item) {
	if ((*character).inventorySize < (*character).inventoryCapacity) {
		(*character).inventory[(*character).inventorySize] = item;
		(*character).inventorySize++;
		sortInventory(character, 1);
	}
}

void removeItem(Character* character, Item item) {
	bool removed = false;
	for (int i=0; i<(*character).inventorySize; i++) {
		if (removed) {
			(*character).inventory[i-1] = (*character).inventory[i];
		} else if ( strcmp((*character).inventory[i].name, item.name) == 0 ) {
			removed = true;
		}
	}
	if (removed) {
		(*character).inventorySize--;
	}
}

void buy(Character* buyer, Character* seller, Item item) {
	(*buyer).gold -= item.price;
	(*seller).gold += item.price;
	addItem(buyer, item);
	removeItem(seller, item);
}

Item createItem(char* name, int price) {
	Item newItem;

	strcpy(newItem.name, name);
	newItem.price = price;

	return newItem;
}