// Définir les constantes
#define INGREDIENTS_MAX 8
#define NOMBRE_CHEFS 6

// Définir les ingrédients
#define PAIN 3
#define PAIN_SOUS 4
#define FROMAGE 5
#define VIANDE 6
#define TOMATE 7
#define OIGNON 8
#define CORNICHON 9
#define SALADE 10

// Définir les recettes pour le projet
#define PATRICK [PAIN, FROMAGE, VIANDE, TOMATE, VIANDE, TOMATE, PAIN_SOUS]
#define BOB [PAIN, FROMAGE, VIANDE, TOMATE, OIGNON, CORNICHON, SALADE, PAIN_SOUS]
#define CRAB [PAIN, VIANDE, OIGNON, SALADE, TOMATE, CORNICHON, SALADE, PAIN_SOUS]
#define PLANKTON [PAIN, CORNICHON, VIANDE, SALADE, CORNICHON, TOMATE, CORNICHON, PAIN_SOUS]
#define CARLOS [PAIN, FROMAGE, OIGNON, TOMATE, TOMATE, OIGNON, SALADE, PAIN_SOUS]
#define SANDY [PAIN, FROMAGE, VIANDE, FROMAGE, VIANDE, FROMAGE, VIANDE, PAIN_SOUS]

// Structure de recette pour le projet
struct Chef {
    char nom[20];   // Nom du personnage sur la carte
    int recette[8]; // Recette du burger sur la carte
    char tag_1[20]; // Première carte de ce personnage (tag RFID)
    char tag_2[20]; // Le deuxième tag RFID de ce personnage (tag RFID)
};