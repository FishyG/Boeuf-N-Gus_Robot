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
#include "commande.cpp"


void setup() {
    BoardInit(); // Balls XD
}

void loop(){
    int commande = lireCommandeIR();
    if (commande>0){
        Serial.print("commande recu: ");
        Serial.println(commande);
    }
    
}
