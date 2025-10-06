#pragma once

// Initialise RobUS et met les moteurs à 0
void MOTEUR_Init();

// Stoppe immédiatement les deux moteurs
void MOTEUR_Stop();

// Fait avancer autour d'une vitesse de base, avec une correction (-1..1)
//  - base_speed : 0..1 (ex. 0.35)
//  - correction : typiquement issue d’un PID, bornée ~[-0.35 ; +0.35]
void MOTEUR_Drive(float base_speed, float correction);
