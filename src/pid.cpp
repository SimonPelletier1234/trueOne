#include <LibRobus.h>
#include "moteur.h"
#include "pid.h"
#include "capteur.h"

static const float KP = 0.12f;
static const float KI = 0.0008f;
static const float KD = 0.02f;
static const float SORTIE_MAX   = 0.25;
static const float INTEGRAL_MAX = 2000.0f;

static inline float clamp(float x, float a, float b) {
  if (x < a) return a;
  if (x > b) return b;
  return x;
}

void PID_AvanceDroit(float vitesse_base, long distance_ticks) {
  ENCODER_Reset(LEFT);
  ENCODER_Reset(RIGHT);

  float e = 0, ePrev = 0, sumE = 0;
  unsigned long tPrev = micros();

  while (true) {
    long L = ENCODER_Read(LEFT);
    long R = ENCODER_Read(RIGHT);
    long mean = (L + R) / 2;
    if (mean >= distance_ticks) break;

    // Stop si un IR voit un mur
    if (Find_Mur() != 0) break;

    e = float(L - R);
    unsigned long tNow = micros();
    float dt = (tNow - tPrev) / 1e6f;
    if (dt < 1e-4f) dt = 1e-4f;
    tPrev = tNow;

    sumE += e * dt;
    sumE = clamp(sumE, -INTEGRAL_MAX, INTEGRAL_MAX);

    float d = (e - ePrev) / dt;
    ePrev = e;

    float corr = KP * e + KI * sumE + KD * d;
    corr = clamp(corr, -SORTIE_MAX, SORTIE_MAX);

    MOTEUR_Drive(vitesse_base, corr);
    delay(5);
  }

  MOTEUR_Stop();
}
