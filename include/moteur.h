#ifndef MOTEUR_H
#define MOTEUR_H

#include <Arduino.h>

// Initialise les moteurs (met les vitesses à 0)
void MOTEUR_Init(void);

// Arrête complètement les moteurs
void MOTEUR_Stop(void);

// Fait avancer ou reculer le robot
// base_speed = vitesse de base (entre -1 et 1)
// correction = ajustement gauche/droite (par exemple issu d’un PID)
void MOTEUR_Drive(float base_speed, float correction);

// Fait tourner le robot sur place
void MOTEUR_TourneGauche(float vitesse);
void MOTEUR_TourneDroite(float vitesse);

#endif
