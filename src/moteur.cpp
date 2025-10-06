#include <LibRobus.h>
#include "moteur.h"

// Si ta roue droite est câblée à l’envers, mets 1 :
#define MOTOR_RIGHT_INVERTED 0

static inline float clamp(float x, float a, float b) {
  if (x < a) return a;
  if (x > b) return b;
  return x;
}

void MOTEUR_Init() {
  BoardInit();
  MOTOR_SetSpeed(LEFT,  0.0f);
  MOTOR_SetSpeed(RIGHT, 0.0f);
}

void MOTEUR_Stop() {
  MOTOR_SetSpeed(LEFT,  0.0f);
  MOTOR_SetSpeed(RIGHT, 0.0f);
}

void MOTEUR_Drive(float base_speed, float correction) {
  // vitesses avant clamp
  float vg = base_speed - correction;
  float vd = base_speed + correction;

  // borne dans [-1, 1]
  vg = clamp(vg, -1.0f, 1.0f);
  vd = clamp(vd, -1.0f, 1.0f);

  // inversion éventuelle de la droite
  if (MOTOR_RIGHT_INVERTED) vd = -vd;

  MOTOR_SetSpeed(LEFT,  vg);
  MOTOR_SetSpeed(RIGHT, vd);
}
