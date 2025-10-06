#include <LibRobus.h>
#include "moteur.h"
#include "pid.h"

// Réglages PID
static const float KP = 0.12f;
static const float KI = 0.0008f;
static const float KD = 0.02f;
static const float SORTIE_MAX = 0.35f;
static const float INTEGRAL_MAX = 2000.0f;

static inline float clamp(float x, float a, float b) {
  if (x < a) return a;
  if (x > b) return b;
  return x;
}

void PID_AvanceDroit(float vitesse_base, long distance_ticks) {
  ENCODER_Reset(LEFT);
  ENCODER_Reset(RIGHT);

  float erreur = 0, erreurPrec = 0, sommeErreurs = 0;
  unsigned long tPrec = micros();

  while (true) {
    long L = ENCODER_Read(LEFT);
    long R = ENCODER_Read(RIGHT);
    long moyenne = (L + R) / 2;
    if (moyenne >= distance_ticks) break;

    erreur = (float)(L - R);
    unsigned long tNow = micros();
    float dt = (tNow - tPrec) / 1e6f;
    if (dt < 1e-4f) dt = 1e-4f;
    tPrec = tNow;

    sommeErreurs += erreur * dt;
    sommeErreurs = clamp(sommeErreurs, -INTEGRAL_MAX, INTEGRAL_MAX);

    float deriv = (erreur - erreurPrec) / dt;
    erreurPrec = erreur;

    float correction = KP * erreur + KI * sommeErreurs + KD * deriv;
    correction = clamp(correction, -SORTIE_MAX, SORTIE_MAX);

    MOTEUR_Drive(vitesse_base, correction);
    delay(5);
  }

  MOTEUR_Stop();
}
