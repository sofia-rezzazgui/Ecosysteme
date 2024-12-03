#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ecosys.h" 

#define NB_PROIES 20
#define NB_PREDATEURS 20
#define SIZE_X 20
#define SIZE_Y 50

int main(void) {
    srand(time(NULL)); 

    Animal *liste_proie = NULL;
    Animal *liste_predateur = NULL;

    // Création des proies
    for (int i = 0; i < NB_PROIES; i++) {
        int x = rand() % SIZE_X; 
        int y = rand() % SIZE_Y;
        Animal *proie = creer_animal(x, y, 100.0); 
        proie->suivant = liste_proie; 
        liste_proie = proie;
    }

    // Création des prédateurs
    for (int i = 0; i < NB_PREDATEURS; i++) {
        int x = rand() % SIZE_X; 
        int y = rand() % SIZE_Y;
        Animal *predateur = creer_animal(x, y, 100.0); 
        predateur->suivant = liste_predateur; 
        liste_predateur = predateur;
    }

    
    printf("Nombre de proies: %d\n", compte_animal_it(liste_proie)); 
    printf("Nombre de prédateurs: %d\n", compte_animal_it(liste_predateur)); 
    afficher_ecosys(liste_proie, liste_predateur);
    
    
// test des focntions lire et ecrire fichier :
 const char *nom_fichier = "ecosystem.txt";
        ecrire_ecosys(nom_fichier, liste_predateur, liste_proie);
        lire_ecosys(nom_fichier,&liste_proie,&liste_predateur);
const char *nom_fichier2 = "lire_ecosystem.txt";
lire_ecosys(nom_fichier2,&liste_proie,&liste_predateur);
ecrire_ecosys(nom_fichier2, liste_predateur, liste_proie);

    // Libération de la mémoire
    liberer_liste_animaux(liste_proie);
    liberer_liste_animaux(liste_predateur);

    return 0;
}

