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
#include "Deplacement_Robot.h" // Pour utiliser les fonctions de deplacement du robot
#include "Commandes_Robot.h" // Pour utiliser les fonctions de commandes du robot

void setup() {
    BoardInit(); // Balls XD

    Initialisation(); // Initialiser les commandes du robot
    Attente_depart(); // Attendre que le robot soit activé par l'utilisateur

}

void loop() {

    Verif_Commandes(); // verification si une commande est appelee
    SuiveurLigne(); // Appel de la fonction SuiveurLigne pour suivre la ligne

    
}
