#include <LibRobus.h>
#include "moteur.h"
#include "capteur.h"
#include "pid.h"

// Gains PID et limites
static const float KP = 0.12f;
static const float KI = 0.0008f;
static const float KD = 0.02f;
static const float SORTIE_MAX   = 0.35f;     // limite de la correction envoyée aux moteurs
static const float INTEGRAL_MAX = 2000.0f;   // évite l’emballement de l’intégrale

// Contraint x à [minVal, maxVal]
static inline float limiter(float x, float minVal, float maxVal) {
  if (x < minVal) return minVal;
  if (x > maxVal) return maxVal;
  return x;
}

void PID_AvanceDroit(float vitesse_base, long distance_ticks) {
  // Remise à zéro des encodeurs
  ENCODER_Reset(LEFT);
  ENCODER_Reset(RIGHT);

  float erreur = 0.0f;
  float erreurPrecedente = 0.0f;
  float sommeErreur = 0.0f;

  unsigned long tempsPrecedent_us = micros();

  while (true) {
    // Avancement atteint ?
    long ticksGauche = ENCODER_Read(LEFT);
    long ticksDroite = ENCODER_Read(RIGHT);
    long ticksMoyens = (ticksGauche + ticksDroite) / 2;
    if (ticksMoyens >= distance_ticks) break;

    // Arrêt sécurité si un IR voit un mur (adapter si ta fonction s’appelle CAPTEUR_DetecterMur())
    if (Find_Mur() != 0) break;

    // Erreur = différence de ticks (si gauche avance plus que droite → corriger)
    erreur = (float)(ticksGauche - ticksDroite);

    // Delta temps (en secondes)
    unsigned long t_now = micros();
    float dt = (t_now - tempsPrecedent_us) / 1e6f;
    if (dt < 1e-4f) dt = 1e-4f;  // évite la division par ~0
    tempsPrecedent_us = t_now;

    // Intégrale limitée
    sommeErreur += erreur * dt;
    sommeErreur = limiter(sommeErreur, -INTEGRAL_MAX, INTEGRAL_MAX);

    // Dérivée
    float derivee = (erreur - erreurPrecedente) / dt;
    erreurPrecedente = erreur;

    // Sortie PID limitée
    float correction = KP * erreur + KI * sommeErreur + KD * derivee;
    correction = limiter(correction, -SORTIE_MAX, SORTIE_MAX);

    // Applique la correction gauche/droite autour de la vitesse de base
    MOTEUR_Drive(vitesse_base, correction);

    delay(5); // petite pause pour laisser le temps au système
  }

  MOTEUR_Stop();
}
