#include <LibRobus.h>
#include "moteur.h"

#define MOTOR_RIGHT_INVERTED 0

static inline float clamp(float x, float a, float b) {
  if (x < a) return a;
  if (x > b) return b;
  return x;
}

void MOTEUR_Init() {
  BoardInit();
  MOTOR_SetSpeed(LEFT, 0);
  MOTOR_SetSpeed(RIGHT, 0);
}

void MOTEUR_Stop() {
  MOTOR_SetSpeed(LEFT, 0);
  MOTOR_SetSpeed(RIGHT, 0);
}

void MOTEUR_Drive(float base_speed, float correction) {
  float vg = base_speed - correction;
  float vd = base_speed + correction;
  vg = clamp(vg, -1.0f, 1.0f);
  vd = clamp(vd, -1.0f, 1.0f);
  if (MOTOR_RIGHT_INVERTED) vd = -vd;

  MOTOR_SetSpeed(LEFT, vg);
  MOTOR_SetSpeed(RIGHT, vd);
}

void MOTEUR_TourneGauche(float vitesse) {
  MOTOR_SetSpeed(LEFT, -vitesse);
  MOTOR_SetSpeed(RIGHT, vitesse);
}

void MOTEUR_TourneDroite(float vitesse) {
  MOTOR_SetSpeed(LEFT, vitesse);
  MOTOR_SetSpeed(RIGHT, -vitesse);
}
