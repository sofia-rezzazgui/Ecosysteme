#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "ecosys.h"
#include <string.h>
/* PARTIE 1*/
/* Fourni: Part 1, exercice 4, question 2 */
Animal *creer_animal(int x, int y, float energie) {
  Animal *na = (Animal *)malloc(sizeof(Animal));
  assert(na);
  na->x = x;
  na->y = y;
  na->energie = energie;
  na->dir[0] = rand() % 3 - 1;
  na->dir[1] = rand() % 3 - 1;
  na->suivant = NULL;
  return na;
}


/* Fourni: Part 1, exercice 4, question 3 */
Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal) {
  assert(animal);
  assert(!animal->suivant);
  animal->suivant = liste;
  return animal;
}

/* A faire. Part 1, exercice 6, question 2 */
void ajouter_animal(int x, int y, float energie, Animal **liste_animal) {
    Animal *animal = (Animal *)malloc(sizeof(Animal));
    animal->x = x;
    animal->y = y;
    animal->energie = energie;
    animal->dir[0] = rand() % 3 - 1;
    animal->dir[1] = rand() % 3 - 1;
    animal->suivant = *liste_animal;
    *liste_animal = animal;
}


/* A Faire. Part 1, exercice 5, question 5 */
void enlever_animal(Animal **liste, Animal *animal) {
    if (*liste == NULL || animal == NULL) {
        return;
    }

    Animal *courant = *liste;
    Animal *prec = NULL;

    while (courant != NULL && courant != animal) {
        prec = courant;
        courant = courant->suivant;
    }

    if (courant == NULL) {
        return;
    }

    if (prec == NULL) {
        *liste = courant->suivant;
    } else {
        prec->suivant = courant->suivant;
    }

    free(courant);
}
/* A Faire. Part 1, exercice 6, question 7 */
Animal* liberer_liste_animaux(Animal *liste) {
	Animal* courant=liste;
	Animal* suivant;
	while(courant!=NULL){
		suivant=courant->suivant;
		free(courant);
		courant=suivant;
	}
	

  return NULL;
}

/* Fourni: part 1, exercice 4, question 4 */
unsigned int compte_animal_rec(Animal *la) {
  if (!la) return 0;
  return 1 + compte_animal_rec(la->suivant);
}

/* Fourni: part 1, exercice 4, question 4 */
unsigned int compte_animal_it(Animal *la) {
  int cpt=0;
  while (la) {
    ++cpt;
    la=la->suivant;
  }
  return cpt;
}



/* Part 1. Exercice 5, question 1, ATTENTION, ce code est susceptible de contenir des erreurs... */
void afficher_ecosys(Animal *liste_proie, Animal *liste_predateur) {
  unsigned int i, j;
  char ecosys[SIZE_X][SIZE_Y];
  Animal *pa=NULL;

  /* on initialise le tableau */
  for (i = 0; i < SIZE_X; ++i) {
    for (j = 0; j < SIZE_Y; ++j) {
      ecosys[i][j]=' ';
    }
  }

  /* on ajoute les proies */
  pa = liste_proie;
  while (pa) {
    ecosys[pa->x][pa->y] = '*';
    pa=pa->suivant;
  }

  /* on ajoute les predateurs */
  pa = liste_predateur;
  while (pa) {
      if ((ecosys[pa->x][pa->y] == '@') || (ecosys[pa->x][pa->y] == '*')) { /* proies aussi present */
        ecosys[pa->x][pa->y] = '@';
      } else {
        ecosys[pa->x][pa->y] = 'O';
      }
    pa = pa->suivant;
  }

  /* on affiche le tableau */
  printf("+");
  for (j = 0; j < SIZE_Y; ++j) {
    printf("-");
  }  
  printf("+\n");
  for (i = 0; i < SIZE_X; ++i) {
    printf("|");
    for (j = 0; j < SIZE_Y; ++j) {
      putchar(ecosys[i][j]);
    }
    printf("|\n");
  }
  printf("+");
  for (j = 0; j<SIZE_Y; ++j) {
    printf("-");
  }
  printf("+\n");
  int nbproie=compte_animal_it(liste_proie);
  int nbpred=compte_animal_it(liste_predateur);
  
  printf("Nb proies : %5d\tNb predateurs : %5d\n", nbproie, nbpred);

}


void clear_screen() {
  printf("\x1b[2J\x1b[1;1H");  /* code ANSI X3.4 pour effacer l'ecran */
}

/* PARTIE 2*/

/* Part 2. Exercice 4, question 1 */
void bouger_animaux(Animal *la) {
Animal*animal=la;

while (animal != NULL) {
	
	animal->x +=animal->dir[0];
	animal->y +=animal->dir[1];
	if (animal->x < 0) 
		animal->x=SIZE_X - 1;
	if (animal->x >= SIZE_X) 
		animal->x = 0;
        if (animal->y < 0)
        	 animal->y = SIZE_Y - 1;
        if (animal->y >= SIZE_Y)
        	 animal->y = 0;
        
        animal->energie -= 1;
        animal=animal->suivant;
        
        }
}

/* Part 2. Exercice 4, question 3 */
void reproduce(Animal **liste_animal, float p_reproduce) {
    Animal *courant = *liste_animal;
    
    while (courant != NULL) {
        float comp = (float)rand() / RAND_MAX;
        if (comp < p_reproduce) {
            
            Animal *nv_animal = creer_animal(courant->x, courant->y, courant->energie / 2.0);
            if (nv_animal == NULL) {
               
                fprintf(stderr, "Erreur de création d'un nouvel animal\n");
                return;
            }
           *liste_animal = ajouter_en_tete_animal(*liste_animal, nv_animal);
            courant->energie /= 2.0;
        }
        courant = courant->suivant;
    }
}



/* Part 2. Exercice 6, question 1 */
void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y], int temps_repousse_herbe ) {
    Animal *courant =*liste_proie;
     Animal *suivant;
    bouger_animaux(*liste_proie);
    while (courant != NULL) {
    suivant=courant->suivant;
        courant->energie--;
        if(monde[courant->x][courant->y] > 0) {
            courant->energie += monde[courant->x][courant->y];
            monde[courant->x][courant->y] = temps_repousse_herbe;
        }
        if (courant->energie <= 0) {
            enlever_animal(liste_proie, courant);
        }
        
        courant = suivant;
    }
    
    float c = (float)rand() / RAND_MAX;
    reproduce(liste_proie, c);
}

/* Part 2. Exercice 7, question 1 */
Animal *animal_en_XY(Animal *l, int x, int y) {
Animal*courant=l;
while(courant!=NULL){
	if (courant->x==x && courant->y==y){
		return courant;}
	courant=courant->suivant;
	}
		

  return NULL;
} 

/* Part 2. Exercice 7, question 2 */
void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie) {
    Animal *courant = *liste_predateur;
    
    bouger_animaux(*liste_predateur);

    while (courant != NULL) {
        courant->energie--;
        
        // Utilisation de animal_en_XY pour trouver une proie à la même position
        Animal *proie = animal_en_XY(*liste_proie, courant->x, courant->y);
        
        if (proie != NULL) {
            courant->energie += proie->energie;
            enlever_animal(liste_proie, proie);
        }

        if (courant->energie <= 0) {
            Animal *temp = courant;
            courant = courant->suivant; 
            enlever_animal(liste_predateur, temp); 
        } else {
            courant = courant->suivant; 
        }
    }
    
  
    reproduce(liste_predateur,  p_reproduce_predateur);
}



/* Part 2. Exercice 5, question 2 */
void rafraichir_monde(int monde[SIZE_X][SIZE_Y]){
for(int i=0;i<SIZE_X;i++){
	for(int j=0;j<SIZE_Y;j++){
                monde[i][j]++;}
        
}
}


void afficher_liste(Animal *liste) {
    Animal *courant = liste;
    while (courant != NULL) {
        printf("Animal à (%d, %d) avec énergie %.2f\n", courant->x, courant->y, courant->energie);
        courant = courant->suivant;
    }
    printf("Fin de la liste\n\n");
}

void ecrire_ecosys(const char *nom_fichier, Animal *liste_predateur, Animal *liste_proie) {
    FILE *fichier = fopen(nom_fichier,"w");
    if (fichier == NULL) {
        printf("Erreur d'ouverture du fichier");
        return;
    }
    fprintf(fichier,"<proies>\n");
    while (liste_proie != NULL) {
        fprintf(fichier,"x=%d y=%d dir=[%d %d] e=%f\n", 
                liste_proie->x, liste_proie->y, liste_proie->dir[0], liste_proie->dir[1], liste_proie->energie);
        liste_proie = liste_proie->suivant;
    }
    fprintf(fichier,"</proies>\n");

    fprintf(fichier,"<predateurs>\n");
    while (liste_predateur != NULL) {
        fprintf(fichier,"x=%d y=%d dir=[%d %d] e=%f\n", 
                liste_predateur->x, liste_predateur->y, liste_predateur->dir[0], liste_predateur->dir[1], liste_predateur->energie);
        liste_predateur = liste_predateur->suivant;
    }
    fprintf(fichier,"</predateurs>\n");

    fclose(fichier);
}

void lire_ecosys(const char *nom_fichier, Animal **liste_predateur, Animal **liste_proie) {
    FILE *fichier = fopen(nom_fichier,"r");
    if (fichier == NULL) {
        perror("Erreur d'ouverture du fichier");
        return;
    }
    
    char ligne[256];
    while (fgets(ligne,sizeof(ligne),fichier)) {
        if (strncmp(ligne,"<proies>\n",8) == 0) {
            printf("Début de la section des proies\n");
            	while (fgets(ligne, sizeof(ligne), fichier) && strncmp(ligne,"</proies>\n", 9) != 0) {
                	int x, y, dir0, dir1;
                	float energie;
               		sscanf(ligne, "x=%d y=%d dir=[%d %d] e=%f", &x, &y, &dir0, &dir1, &energie);}
               		
         
        } else if (strncmp(ligne,"<predateurs>\n",12) == 0) {
            printf("Début de la section des prédateurs\n");
            	while (fgets(ligne, sizeof(ligne), fichier) && strncmp(ligne,"</proies>\n", 9) != 0) {
                	int x, y, dir0, dir1;
                	float energie;
                	sscanf(ligne, "x=%d y=%d dir=[%d %d] e=%f", &x, &y, &dir0, &dir1, &energie);
    }
	}
	}
    fclose(fichier);
}

