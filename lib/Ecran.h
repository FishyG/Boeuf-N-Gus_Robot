#include <Arduino.h>
#include "LiquidCrystal.h"

// Relier les pins du Arduino aux pins du LCD ( Digital )-------------------------------------------
#define PIN_RS 52
#define PIN_EN 50
#define PIN_D4 40
#define PIN_D5 42
#define PIN_D6 44
#define PIN_D7 46

// Matrice d'animation des yeux du robot------------------------------------------------------------
#define OEIL_OUVERT "    .------.    \n   !   &&   !   \n   !   &&   !   \n    '------'    "
#define OEIL_MI_OUVERT "     .---.      \n    !  && !     \n    !  && !     \n     '---'      "
#define OEIL_FERME "       ||       \n       ||       \n       ||       \n       ||       "

// Fonctions du module écran LCD --------------------------------------------------------------------

// Fonction du setup de l'écran LCD
// À mettre obligatoirement dans le setup() du programme principal
void setupLCD(LiquidCrystal &ecran, int &etape, unsigned long &previousMillis);

// Fonction permettant de calculer la taille d'un message
// Paramètre : msg - message dont on veut calculer la taille
// Retourne la taille du message (nombre de caractères)
int tailleMsg(String msg);  

// Fonction permettant d'écrire sur l'écran LCD
// Paramètre : msg - message à écrire sur l'écran
//             ligne - numéro de la ligne sur laquelle écrire le message (0 à 3)
//             ecran - objet LiquidCrystal représentant l'écran LCD
void ecrireEcran(String msg, int ligne, LiquidCrystal &ecran);

// Fonction permettant de faire cligner les yeux du robot
// Paramètre : ecran - objet LiquidCrystal représentant l'écran LCD
// À mettre dans la boucle loop() du programme principal
void oeilCligner(LiquidCrystal &ecran, int &etape, unsigned long &previousMillis);

// Fonction permettant de fermer l'oeil du robot
// Paramètre : ecran - objet LiquidCrystal représentant l'écran LCD
void fermerOeil(LiquidCrystal &ecran);

// Fonction permettant d'ouvrir l'oeil du robot
// Paramètre : ecran - objet LiquidCrystal représentant l'écran LCD
void ouvrirOeil(LiquidCrystal &ecran);



