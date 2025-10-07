#include <LibRobus.h>
#include "moteur.h"

#define MOTOR_RIGHT_INVERTED 0  // Mettre à 1 si le moteur droit tourne à l’envers

// Garde la valeur entre a et b
static inline float limiter(float valeur, float min, float max) {
  if (valeur < min) return min;
  if (valeur > max) return max;
  return valeur;
}

void MOTEUR_Init(void) {
  BoardInit();
  MOTOR_SetSpeed(LEFT, 0);
  MOTOR_SetSpeed(RIGHT, 0);
}

void MOTEUR_Stop(void) {
  MOTOR_SetSpeed(LEFT, 0);
  MOTOR_SetSpeed(RIGHT, 0);
}

void MOTEUR_Drive(float base_speed, float correction) {
  float vitesseGauche = base_speed - correction;
  float vitesseDroite = base_speed + correction;

  // Empêche les vitesses de dépasser la plage [-1, 1]
  vitesseGauche = limiter(vitesseGauche, -1.0f, 1.0f);
  vitesseDroite = limiter(vitesseDroite, -1.0f, 1.0f);

  if (MOTOR_RIGHT_INVERTED)
    vitesseDroite = -vitesseDroite;

  MOTOR_SetSpeed(LEFT, vitesseGauche);
  MOTOR_SetSpeed(RIGHT, vitesseDroite);
}

void MOTEUR_TourneGauche(float vitesse) {
  MOTOR_SetSpeed(LEFT, -vitesse);
  MOTOR_SetSpeed(RIGHT, vitesse);
}

void MOTEUR_TourneDroite(float vitesse) {
  MOTOR_SetSpeed(LEFT, vitesse);
  MOTOR_SetSpeed(RIGHT, -vitesse);
}
