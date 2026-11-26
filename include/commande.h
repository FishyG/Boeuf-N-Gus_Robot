#pragma once
#include "..\include\ingredients.h"
#include <arduino.h>

const uint8_t ADRESSE_ATTENDUE = 0x17;

// ======= Module commande =======
// Role : Reconnaitre chaque commandes a partir de l'adresse RFID et lister les ingredients

// Recoit le id scanne et modifie chef_actuel pour qu'il contienne la structure du chef
/* UID_scanne est l'id de la carte qui vient d'etre scanne
chefs[] est la liste de toute les structures de chef
chef_actuel est le chef qui a ete scanne */
void trouver_chef(const char *UID_scanne, const struct Chef chefs[],
                 struct Chef *chef_actuel);

// Recoit le chef actuel et remplit le tableau d'ingredients (taille 8)
/* chef_actuel est la structure qui contient le chef scanne
ingredients est la liste d'igredient (du bas au dessus) de la recette en cours */
void extraire_ingredients(const struct Chef *chef_actuel, int ingredients[8]);
