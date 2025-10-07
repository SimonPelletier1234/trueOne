#include <Arduino.h>
#include <LibRobus.h>
#include "moteur.h"
#include "capteur.h"
#include "pid.h"

// --- Réglages déplacement ---
static const float VITESSE_BASE       = 0.25f;
static const long  DISTANCE_TICKS_MAX = 25000;

// --- Réaction obstacles ---
static const float VITESSE_TOURNE = 0.20f;
static const int   TOURNE_MS      = 900;   // ~90°
static const float VITESSE_RECULE = 0.10f;
static const int   RECULE_MS      = 200;

// --- Anti-glitch capteurs (N lectures identiques d’affilée) ---
static const int DETECT_N      = 3;
static const int DETECT_DT_MS  = 5;

// Helpers
static void reculer_ms(int ms) {
  MOTOR_SetSpeed(LEFT,  -VITESSE_RECULE);
  MOTOR_SetSpeed(RIGHT, -VITESSE_RECULE);
  delay(ms);
  MOTEUR_Stop();
}

static int CAPTEUR_DetecterMur_Stable() {
  int identiques = 0;
  int dernier = Find_Mur();
  for (int i = 0; i < DETECT_N; i++) {
    int etat = Find_Mur();
    if (etat == dernier) {
      identiques++;
    } else {
      identiques = 0;
      dernier = etat;
    }
    delay(DETECT_DT_MS);
  }
  return (identiques >= DETECT_N) ? dernier : 0;
}

void setup() {
  Serial.begin(9600);
  MOTEUR_Init();
  CAPTEUR_Init();
  delay(300);
}

void loop() {
  // Avance droit jusqu’à la distance cible ou détection d’obstacle
  PID_AvanceDroit(VITESSE_BASE, DISTANCE_TICKS_MAX);

  MOTEUR_Stop();
  delay(500);

  int mur = CAPTEUR_DetecterMur_Stable();

  if (mur == 0) {
    delay(300); // repart tout droit
    return;
  }

  if (mur == 1) {                 // mur à gauche → tourne à droite
    delay(300);
    MOTEUR_TourneDroite(VITESSE_TOURNE);
    delay(TOURNE_MS);
    MOTEUR_Stop();
  } else if (mur == 2) {          // mur à droite → tourne à gauche
    delay(300);
    MOTEUR_TourneGauche(VITESSE_TOURNE);
    delay(TOURNE_MS);
    MOTEUR_Stop();
  } else if (mur == 3) {          // murs des deux côtés → recule puis tourne
    delay(300);
    reculer_ms(RECULE_MS);
    MOTEUR_TourneGauche(VITESSE_TOURNE);
    delay(TOURNE_MS + 250);
    MOTEUR_Stop();
  }

  delay(500);
}
