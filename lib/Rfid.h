#include <Arduino.h>

// Définir les ingrédients---------------------------------------------------------------------
#define PAIN 3
#define PAIN_SOUS 4
#define FROMAGE 5
#define VIANDE 6
#define TOMATE 7
#define OIGNON 8
#define CORNICHON 9
#define SALADE 10

// Définir les recettes pour le projet---------------------------------------------------------
#define PATRICK [PAIN, FROMAGE, VIANDE, TOMATE, VIANDE, TOMATE, PAIN_SOUS]
#define BOB [PAIN, FROMAGE, VIANDE, TOMATE, OIGNON, CORNICHON, SALADE, PAIN_SOUS]
#define CRAB [PAIN, VIANDE, OIGNON, SALADE, TOMATE, CORNICHON, SALADE, PAIN_SOUS]
#define PLANKTON [PAIN, CORNICHON, VIANDE, SALADE, CORNICHON, TOMATE, CORNICHON, PAIN_SOUS]
#define CARLOS [PAIN, FROMAGE, OIGNON, TOMATE, TOMATE, OIGNON, SALADE, PAIN_SOUS]
#define SANDY [PAIN, FROMAGE, VIANDE, FROMAGE, VIANDE, FROMAGE, VIANDE, PAIN_SOUS]

// Structure de recette pour le projet--------------------------------------------------------
struct Chef {
    char nom[20];   // Nom du personnage sur la carte
    int recette[8]; // Recette du burger sur la carte
    char tag_1[20]; // Première carte de ce personnage (tag RFID)
    char tag_2[20]; // Le deuxième tag RFID de ce personnage (tag RFID)
};


// Fonctions du module RFID --------------------------------------------------------------------
// Fonction permettant de configurer le module RFID - OBLIGATOIRE
// À mettre dans le setup() du programme principal
void setupRfid();

// Fonction permettant de lire le tag RFID et de le stocker dans le tableau id_tag
// Paramètre : id_tag - tableau de 20 caractères pour stocker le tag lu (la fonction ne
// le retourne pas, mais la donnée est modifiée dans la mémoire)
bool lireRfid(char id_tag[20]);

// Compare le tag lu avec celui entre parametre et retourne 1 
// si les tags correspondent, sinon retourne 0
int verifierTag(char id_tag[20], char tag_attendu[20]);