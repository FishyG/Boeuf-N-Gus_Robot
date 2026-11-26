/*
Projet: main.cpp robo swag
Equipe: P2
Auteurs: Jessy, RAJOUTER VOS CNOMS ICI
Description: Code epreuve du combattant
Date: Aujourd'hui lmao
*/

/* ****************************************************************************
Inclure les librairies de functions que vous voulez utiliser
**************************************************************************** */

#include <LibRobus.h>  // Pour utiliser RobUS
#include "../include/commande.h"
int recette_vide[INGREDIENTS_MAX];

void setup() {
    BoardInit(); // Balls XD
}

void loop(){
    int commande = lireCommandeIR();
    if (commande>0){
        Serial.print("commande recu: ");
        Serial.println(commande);
    }

    int chef_actuel = trouver_chef(commande);
	extraire_ingredients(chef_actuel, recette_vide);
    
}
