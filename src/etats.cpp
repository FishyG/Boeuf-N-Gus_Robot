#include "etats.h"

#include <Arduino.h>
#include <LibRobus.h>

#include "arret_robot.h"
#include "config.h"
#include "couleur.h"
#include "suiveur_ligne.h"
#include "movements.h"

// TODO(Lyevan)      : #include "commande.h"
// TODO(Lyevan)      : #include "ingredients.h"
// TODO(Christian)   : #include "del_rgb.h"

// L'enum Etat reste ici : aucun autre module n'en a besoin.
enum Etat : uint8_t {
    ATTENTE_COMMANDE,
    DECISION,
    DEPLACEMENT,
    ARRET_STATION,
    PRISE_INGREDIENT,
    COMMANDE_PRETE,
    ERREUR
};
static const char* NOMS_ETATS[] = {"ATTENTE_COMMANDE", "DECISION",       "DEPLACEMENT", "ARRET_STATION",
                                   "PRISE_INGREDIENT", "COMMANDE_PRETE", "ERREUR"};

static Etat etat = ATTENTE_COMMANDE;
static unsigned long t_entree = 0;
static Sens sens = SENS_AVANT;
static Commande commande = {};
static uint8_t nb_pris = 0;

static const Commande COMMANDE_DEMO = {RECETTE_DEMO, NB_RECETTE_DEMO};  // TEMPORAIRE (Lyevan)
static bool demo_faite = false;  // TEMPORAIRE : la demo ne roule qu'une fois

static void changer_etat(Etat nouvel) {
    LOG(String("-> ") + NOMS_ETATS[nouvel]);
    etat = nouvel;
    t_entree = millis();
}

#if LOG_DEBUG
static void log_decision(Ingredient ing, ActionStation action) {
    Serial.print("[decision] position ");
    Serial.print(arret_position());
    Serial.print("  pris ");
    Serial.print(nb_pris);
    Serial.print("/");
    Serial.print(commande.nb);
    Serial.print("  prochain ");
    Serial.print(nom_ingredient(ing));
    Serial.print("  -> ");
    Serial.println(arret_nom_action(action));
}
#endif

static Ingredient prochain_ingredient() {
    return nb_pris < commande.nb ? commande.liste[nb_pris] : ING_AUCUN;
}

void etats_init() {
#if LOG_DEBUG
    Serial.begin(BAUD_DEBUG);
#endif
    // Chaque module s'initialise lui-meme, dans son propre fichier.
    couleur_init();
    arret_init();
    suiveur_init();
    movementsInit();
    // TODO(Lyevan)      : commande_init();
    // TODO(Lyevan)      : ingredients_init();
    // TODO(Christian)   : del_init();
    changer_etat(ATTENTE_COMMANDE);
}

void etats_executer() {
    switch (etat) {
        case ATTENTE_COMMANDE:
            // TODO(Christian) : del_afficher(DEL_ATTENTE);
            // TODO(Lyevan)    : if (commande_disponible()) { commande = commande_lire(); ... }
            if (!demo_faite && millis() - t_entree >= DELAI_COMMANDE_DEMO) {  // TEMPORAIRE
                demo_faite = true;
                commande = COMMANDE_DEMO;
                nb_pris = 0;
                changer_etat(DECISION);
            }
            break;

        case DECISION: {
            ActionStation action = arret_decider(prochain_ingredient());
#if LOG_DEBUG
            log_decision(prochain_ingredient(), action);
#endif
            switch (action) {
                case ACTION_PRENDRE:
                    LOG(String("*** BONNE STATION : prendre ") + nom_ingredient(prochain_ingredient()));
                    AX_BuzzerON(BIP_PRISE_FREQ, BIP_PRISE_DUREE);  // TEMPORAIRE (Christian)
                    changer_etat(PRISE_INGREDIENT);
                    break;
                case ACTION_LIVRER:
                    LOG("*** CLIENT : burger livre");
                    AX_BuzzerON(BIP_LIVRE_FREQ, BIP_LIVRE_DUREE);  // TEMPORAIRE (Christian)
                    changer_etat(COMMANDE_PRETE);
                    break;
                case ACTION_AVANCER:
                    sens = SENS_AVANT;
                    changer_etat(DEPLACEMENT);
                    break;
                case ACTION_RECULER:
                    sens = SENS_ARRIERE;
                    LOG("Marche arriere");
                    changer_etat(DEPLACEMENT);
                    break;
                case ACTION_ERREUR:  changer_etat(ERREUR); break;
            }
            break;
        }

        case DEPLACEMENT: {
            // TODO(Christian)   : del_afficher(DEL_EN_ROUTE);
            suiveur_suivre(sens, sens == SENS_AVANT ? VITESSE_BASE : VITESSE_RECUL);
            if (suiveur_ligne_perdue()) {
                LOG("Ligne perdue");
                changer_etat(ERREUR);
                break;
            }
#if STUB_COULEUR
            couleur_stub_sens(sens);  // le faux capteur "roule" dans le meme sens
#endif
            Couleur couleur = couleur_lire();
            if (arret_nouvelle_station(couleur, sens)) {
                suiveur_arreter();
                LOG(String(">>> ARRET sur ") + nom_couleur(couleur) + ", position " + (int)arret_position());
                arret_demarrer();
                changer_etat(ARRET_STATION);
            }
            break;
        }

        case ARRET_STATION:
            if (arret_termine()) changer_etat(DECISION);
            break;

        case PRISE_INGREDIENT:
            // TODO(Christian) : del_afficher(DEL_PRISE);
            // TODO(Lyevan)    : if (!ingredients_prendre(prochain_ingredient())) { changer_etat(ERREUR); break; }
            if (millis() - t_entree < DUREE_PRISE_DEMO) break;  // TEMPORAIRE : pour bien voir
            nb_pris++;
            changer_etat(DECISION);  // le suivant est peut-etre a la meme station
            break;

        case COMMANDE_PRETE:
            // TODO(Christian) : del_afficher(DEL_PRET);
            // A DECIDER : attendre le capteur IR "burger ramasse" plutot qu'une duree.
            if (millis() - t_entree >= DUREE_AFFICHAGE_PRET) changer_etat(ATTENTE_COMMANDE);
            break;

        case ERREUR:
            suiveur_arreter();
            // TODO(Christian)   : del_afficher(DEL_ERREUR);
            // A DECIDER : comment sortir d'une erreur.
            break;
    }
}
