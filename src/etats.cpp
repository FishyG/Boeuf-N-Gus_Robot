#include "etats.h"

#include <Arduino.h>

#include "arret_robot.h"
#include "config.h"
#include "couleur.h"
// TODO(Christopher) : #include "suiveur_ligne.h"
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


static const Commande COMMANDE_DEMO = {{ING_PAIN_SOUS, ING_OIGNON, ING_VIANDE, ING_TOMATE, ING_PAIN}, 5};

static void changer_etat(Etat nouvel) {
    LOG(String("-> ") + NOMS_ETATS[nouvel]);
    etat = nouvel;
    t_entree = millis();
}

static Ingredient prochain_ingredient() {
    return nb_pris < commande.nb ? commande.liste[nb_pris] : ING_AUCUN;
}

void etats_init() {
    // Chaque module s'initialise lui-meme, dans son propre fichier.
    couleur_init();
    arret_init();
    // TODO(Christopher) : suiveur_init();
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
            if (millis() - t_entree >= DELAI_COMMANDE_DEMO) {  // TEMPORAIRE
                commande = COMMANDE_DEMO;                       // TEMPORAIRE
                nb_pris = 0;
                changer_etat(DECISION);
            }
            break;

        case DECISION:
            switch (arret_decider(prochain_ingredient())) {
                case ACTION_PRENDRE: changer_etat(PRISE_INGREDIENT); break;
                case ACTION_LIVRER:  changer_etat(COMMANDE_PRETE);   break;
                case ACTION_AVANCER: sens = SENS_AVANT;   changer_etat(DEPLACEMENT); break;
                case ACTION_RECULER: sens = SENS_ARRIERE; changer_etat(DEPLACEMENT); break;
                case ACTION_ERREUR:  changer_etat(ERREUR); break;
            }
            break;

        case DEPLACEMENT:
            // TODO(Christian)   : del_afficher(DEL_EN_ROUTE);
            // TODO(Christopher) : suiveur_suivre(sens, sens == SENS_AVANT ? VITESSE_BASE : VITESSE_RECUL);
#if STUB_COULEUR
            couleur_stub_sens(sens);  // le faux capteur "roule" dans le meme sens
#endif
            if (arret_nouvelle_station(couleur_lire())) {
                // TODO(Christopher) : suiveur_arreter();
                arret_demarrer();
                LOG_VAL("Station", arret_position());
                changer_etat(ARRET_STATION);
            }
            break;

        case ARRET_STATION:
            if (arret_termine()) changer_etat(DECISION);
            break;

        case PRISE_INGREDIENT:
            // TODO(Christian) : del_afficher(DEL_PRISE);
            // TODO(Lyevan)    : if (!ingredients_prendre(prochain_ingredient())) { changer_etat(ERREUR); break; }
            nb_pris++;
            changer_etat(DECISION);  // le suivant est peut-etre a la meme station
            break;

        case COMMANDE_PRETE:
            // TODO(Christian) : del_afficher(DEL_PRET);
            // A DECIDER : attendre le capteur IR "burger ramasse" plutot qu'une duree.
            if (millis() - t_entree >= DUREE_AFFICHAGE_PRET) changer_etat(ATTENTE_COMMANDE);
            break;

        case ERREUR:
            // TODO(Christopher) : suiveur_arreter();
            // TODO(Christian)   : del_afficher(DEL_ERREUR);
            // A DECIDER : comment sortir d'une erreur.
            break;
    }
}
