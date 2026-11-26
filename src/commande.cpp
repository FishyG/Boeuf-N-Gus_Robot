#include "..\include\commande.h"
#include <LibRobus.h>

static int chef_actuel;

int trouver_chef(int commande) {

    for (int i = 0; i <= NOMBRE_CHEFS; i++) {
        if (commande-1 == i ) {
            chef_actuel = i;
        }
    }
    return chef_actuel;
}
static int trouver_taille(int taille_max){
	int taille=0;
	for ( int i=0; i< taille_max; i++){
		if (liste_chefs[chef_actuel][i] != 0){
			taille++;
		}
	}
	return taille;
}

void extraire_ingredients(int chef_actuel, int ingredients[8]) {
    int taille_actuelle = trouver_taille(INGREDIENTS_MAX);
    for (int i = 0; i < taille_actuelle; i++) {
        ingredients[i] = liste_chefs[chef_actuel][taille_actuelle - 1 - i];
    }
    for(int i =0; i<taille_actuelle;i++){
		Serial.print(i + 1);
        Serial.print(". ");
        Serial.println(ingredients[i]);
    }
}

int lireCommandeIR() {
    uint32_t message = REMOTE_read();

    if (message == 0 || message == 0xFFFFFFFF) {
        return -1;
    }

    uint8_t adresse     = (message >> 24) & 0xFF;
    uint8_t adresseInv  = (message >> 16) & 0xFF;
    uint8_t commande    = (message >> 8) & 0xFF;
    uint8_t commandeInv = message & 0xFF;

    // Integrite du message
    if (adresse != (uint8_t)~adresseInv || commande != (uint8_t)~commandeInv) {
        Serial.println(" -> message non-integre");
        return -1;
    }

    // Bonne adresse?
    if (adresse != ADRESSE_ATTENDUE) {
        Serial.println(" -> mauvaise adresse");
        return -1;
    }

    return commande;
}
