// Deplacement_Robot.cpp - Christopher Plantevin

// les includes (headers et librairies)

#include "Deplacement_Robot.h"
#include "config.h"
#include <Arduino.h>

// Variables internes

    // variables pour le suiveur de ligne
        int a;
        int b;
        int c;

    // variables pour les correction du suiveur de ligne
        int force_correction;

    // Variables pour le deplacement du robot

    // Variables pour la recherche de la ligne

// Fonctions

    // Section | Suiveur de Ligne

        void SuiveurLigne(){

            a = analogRead(A1);
            b = analogRead(A2);
            c = analogRead(A3);

            if(b > SEUIL_LIGNE){
                Avancer();
            } else { 
                if (a > c && a > SEUIL_DETECTION_D){
                    Correction_Droite();  
                } else if (c > a && c > SEUIL_DETECTION_G){
                    Correction_Gauche();
                } else {
                // rechercher la ligne de suivit au cas le robot perd la ligne 
                // [PAS ENCORE IMPLEMENTE | RETOURNE RIEN]
                    Recherche_Ligne();
                }
            }
        }

        void Correction_Droite(){
            force_correction = 1 - b / 2;
            MOTOR_SetSpeed(LEFT, 0.1);
            MOTOR_SetSpeed(RIGHT, 0.4);

        }

        void Correction_Gauche(){
            force_correction = 1 - b / 2;
            MOTOR_SetSpeed(LEFT, 0.4);
            MOTOR_SetSpeed(RIGHT, 0.1);
    
       }   






    


// Section | Deplacement du Robot

    void Avancer(distance){

        MOTOR_SetSpeed(LEFT, 0.2);
        MOTOR_SetSpeed(RIGHT, 0.2);    
        
    }

    void Reculer(distance){
    
        MOTOR_SetSpeed(LEFT, -0.2);
        MOTOR_SetSpeed(RIGHT, -0.2);
    
    }


    void Tourner(angle){


    }


// Section | Recherche de la ligne



    void Recherche_Ligne(){

    }