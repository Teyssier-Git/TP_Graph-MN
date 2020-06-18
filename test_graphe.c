#include <stdio.h>
#include <stdlib.h>

#include "graphe.h"
#include "fap.h"
#include "pile.h"
#include "file.h"

/**
 * Construit un chemin avec des sommets de ayant comme label le tableau d'entier
 * fournis
 * Effet de bord : affiche le chemin créé
*/
/* chemin_t constructeurChemins(int tab[], int taille) {
   if (taille == 0) {
        return NULL;
    }

    chemin_t chem = (chemin_t)malloc(sizeof(arc_t));
    chem->arc_suivant = NULL;

    psommet_t premierSommet = (psommet_t)malloc(sizeof(sommet_t));
    premierSommet->label = tab[0];

    chem->dest = premierSommet;

    chemin_t cur = chem;

    printf(" %d ", tab[0]);

    for (int i=1; i<taille; i++, cur = cur->arc_suivant) {
        chemin_t tempChem = (chemin_t)malloc(sizeof(arc_t));
        tempChem->arc_suivant = NULL;

        psommet_t tempSomm = (psommet_t)malloc(sizeof(sommet_t));
        tempSomm->label = tab[i];
        printf("| %d ", tab[i]);

        tempChem->dest = tempSomm;

        cur->arc_suivant = tempChem;
    }

    printf("\n");
    return chem; 
}*/

int main (int argc, char **argv)
{
  pgraphe_t g ;
  int nc ;

  if (argc != 2)
    {
      fprintf (stderr, "erreur parametre \n") ;
      exit (-1) ;
    }

  /*
    la fonction lire_graphe alloue le graphe (matrice,...)
    et lit les donnees du fichier passe en parametre
  */


  lire_graphe (argv [1], &g) ;

  /*
    la fonction ecrire_graphe affiche le graphe a l'ecran
  */

  printf ("nombre de sommets du graphe %d nombre arcs %d \n", nombre_sommets (g), nombre_arcs (g)) ;
  fflush (stdout) ;

  ecrire_graphe (g) ;

  nc = colorier_graphe (g) ;

  printf ("nombre chromatique graphe = %d\n", nc) ;

  ecrire_graphe_colorie (g) ;

  printf("\n=== test algo_dijkstra ===\n\n");
  int n;
  scanf("%d", &n);

  algo_dijkstra(g,n);

  printf("\n=== test elementaire ===\n\n");

  //int tab[5] = {0,1,2,3,4};
  //chemin_t testChem = constructeurChemins(tab,5);
  //printf("Elementaire : %d\n", elementaire(g,testChem));

  //int tab1[5] = {0,4,2,3,4};
  //testChem = constructeurChemins(tab1,5);
  //printf("Elementaire : %d\n", elementaire(g,testChem));

  printf("\n=== test eulerien ===\n\n");

  printf("\n=== test distance ===\n\n");
  int x,y;
  printf("premier sommet : ");
  scanf("%d", &x);
  printf("deuxième sommet : ");
  scanf("%d", &y);
  int res = distance(g,x,y);
  printf("Distance entre %d et %d = %d\n", x, y, res);

  printf("\n=== test excentricite ==\n\n");
  scanf("%d", &n);
  int exc = excentricite(g,n);
  printf("Excentricite de %d = %d\n", n, exc);

  printf("\n=== test diametre ==\n\n");
  int dia = diametre(g);
  printf("Diametre = %d\n", dia);
}
