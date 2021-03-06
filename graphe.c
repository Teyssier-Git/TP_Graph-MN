/*
  Structures de type graphe
  Structures de donnees de type liste
  (Pas de contrainte sur le nombre de noeuds des  graphes)
*/


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "graphe.h"
#include "fap.h"
#include "file.h"
#include "pile.h"

int print = 0;

void printed() {
    print = 1;
}

void notPrinted() {
    print = 0;
}

psommet_t chercher_sommet (pgraphe_t g, int label)
{
  psommet_t s ;

  s = g ;

  while ((s!= NULL) && (s->label != label))
    {
      s = s->sommet_suivant ;
    }
  return s ;
}

parc_t existence_arc (parc_t l, psommet_t s)
{
  parc_t p = l ;

  while (p != NULL)
    {
      if (p->dest == s)
	return p ;
      p = p->arc_suivant ;
    }
  return p ;

}


void ajouter_arc (psommet_t o, psommet_t d, int distance)
{
  parc_t parc ;

  parc = (parc_t) malloc (sizeof(arc_t)) ;

  if (existence_arc (o->liste_arcs, d) != NULL)
    {
      fprintf(stderr, "ajout d'un arc deja existant\n") ;
      exit (-1) ;
    }

  parc->poids = distance ;
  parc->dest = d ;
  parc->arc_suivant = o->liste_arcs ;
  o->liste_arcs = parc ;
  return ;
}



// ===================================================================

int nombre_sommets (pgraphe_t g)
{
  psommet_t p = g ;
  int nb = 0 ;

  while (p != NULL)
    {
      nb = nb + 1 ;
      p = p->sommet_suivant ;
    }

  return nb ;
}

int nombre_arcs (pgraphe_t g)
{

  psommet_t p = g ;
  int nb_arcs = 0 ;

  while (p != NULL)
    {
      parc_t l = p->liste_arcs ;

      while (l != NULL)
	{
          nb_arcs = nb_arcs + 1 ;
	  l = l->arc_suivant ;
	}

      p = p->sommet_suivant ;
    }
  return nb_arcs ;
}

void init_couleur_sommet (pgraphe_t g)
{
  psommet_t p = g ;

  while (p != NULL)
    {
      p->couleur = 0 ; // couleur indefinie
      p = p->sommet_suivant ; // passer au sommet suivant dans le graphe
    }

  return ;
}

void init_couleur_arc (pgraphe_t g)
{
  psommet_t p = g ;
  parc_t a;

  while (p != NULL)
    {
      a = p->liste_arcs;
      while(a != NULL){
      	a->couleur = 0;
      	a = a->arc_suivant;
      }
      p = p->sommet_suivant ; // passer au sommet suivant dans le graphe
    }

  return ;
}

int colorier_graphe (pgraphe_t g)
{
  /*
    coloriage du graphe g

    datasets
    graphe data/gr_planning
    graphe data/gr_sched1
    graphe data/gr_sched2
  */

  psommet_t p = g ;
  parc_t a ;
  int couleur ;
  int max_couleur = INT_MIN ; // -INFINI

  int change ;

  init_couleur_sommet (g) ;

  while (p != NULL)
    {
      couleur = 1 ; // 1 est la premiere couleur

      // Pour chaque sommet, on essaie de lui affecter la plus petite couleur

      // Choix de la couleur pour le sommet p

      do
	{
	  a = p->liste_arcs ;
	  change = 0 ;

	  while (a != NULL)
	    {
	      if (a->dest->couleur == couleur)
		{
		  couleur = couleur + 1 ;
		  change = 1 ;
		}
	      a = a->arc_suivant ;
	    }

	} while (change == 1) ;

      // couleur du sommet est differente des couleurs de tous les voisins

      p->couleur = couleur ;
      if (couleur > max_couleur)
	max_couleur = couleur ;

      p = p->sommet_suivant ;
    }

  return max_couleur ;
}

void afficher_graphe_largeur (pgraphe_t g, int r)
{
    /*
      afficher les sommets du graphe avec un parcours en largeur
    */

    init_couleur_sommet(g);

    pfile_t sommets_A_Explorer = creer_file();
    psommet_t sommetCourant;
    psommet_t voisinCourant;
    parc_t arcCourant;

    g->couleur = 1;
    enfiler(sommets_A_Explorer, g);

    while(!file_vide(sommets_A_Explorer)){
    	sommetCourant = defiler(sommets_A_Explorer);
    	printf("%d | ", sommetCourant->label);
    	arcCourant = sommetCourant->liste_arcs;
    	while(arcCourant != NULL){
    		voisinCourant = arcCourant->dest;
    		if(voisinCourant->couleur == 0){
    			voisinCourant->couleur = 1;
    			enfiler(sommets_A_Explorer, voisinCourant);
    		}
    		arcCourant = arcCourant->arc_suivant;
    	}
    }

    return ;
}


void afficher_graphe_profondeur (pgraphe_t g, int r)
{
    /*
      afficher les sommets du graphe avec un parcours en profondeur
    */
    init_couleur_sommet(g);

    ppile_t sommets_A_Explorer = creer_pile();
    psommet_t sommetCourant;
    psommet_t voisinCourant;
    parc_t arcCourant;

    g->couleur = 1;
    empiler(sommets_A_Explorer, g);

    while(!pile_vide(sommets_A_Explorer)){
    	sommetCourant = depiler(sommets_A_Explorer);
    	printf("%d | ", sommetCourant->label);
    	arcCourant = sommetCourant->liste_arcs;
    	while(arcCourant != NULL){
    		voisinCourant = arcCourant->dest;
    		if(voisinCourant->couleur == 0){
    			voisinCourant->couleur = 1;
    			empiler(sommets_A_Explorer, voisinCourant);
    		}
    		arcCourant = arcCourant->arc_suivant;
    	}
    }
    return ;
}

void ajouter_chemin_queue(psommet_t s, parc_t ajout) {
    parc_t copie = (parc_t)malloc(sizeof(arc_t));
    copie->poids = ajout->poids;
    copie->dest = ajout->dest;
    copie->arc_suivant = NULL;

    if (s->chemins == NULL) {
        s->chemins = copie;
    } else {
        parc_t cur = s->chemins;

        while (cur->arc_suivant != NULL) {
            cur = cur->arc_suivant;
        }

        cur->arc_suivant = copie;
    }
}

void supprimer_chemins(psommet_t s) {
    parc_t cur = s->chemins;

    while (cur != NULL) {
        parc_t supp = cur;
        cur = cur->arc_suivant;
        free(supp);
    }

    s->chemins = NULL;
}

fap processing_dijkstra(fap file, int r, psommet_t *destination) {
    psommet_t s = NULL;
    int poids;
    file = extraire(file, &s, &poids);

    s->couleur = GREEN;

    if (s->label == r) {
        *destination = s;
        return file;
    }

    parc_t cur = s->liste_arcs;
    while (cur != NULL) {

        // compare weigth and new weigth + arc weigth
        if (cur->dest->poids > s->poids + cur->poids) {

            // change weigth
            cur->dest->poids = s->poids + cur->poids;

            // change ways
            cur->dest->chemins = s->chemins;
            // add new arc
            // supprimer_chemins(cur->dest);
            cur->dest->chemins = s->chemins;
            ajouter_chemin_queue(cur->dest, cur);

        }
        // enfile the new element
        if (cur->dest->couleur != GREEN) {
            file = inserer(file, cur->dest, cur->dest->poids);
        }


        cur = cur->arc_suivant;
    }


    return file;
}

int algo_dijkstra (pgraphe_t g, int r) {
    psommet_t destination = NULL;

    // init, all infinity exept g
    if (g == NULL) {
        if (print) {
                printf("Le graphe g est NULL\n");
        }
        return -1;
    }
    g->chemins = NULL;
    g->poids = 0;
    g->couleur = RED;
    psommet_t current = g->sommet_suivant;


    while (current != NULL) {
        current->chemins = NULL;
        current->poids = INT_MAX;
        current->couleur = RED;

        current = current->sommet_suivant;
    }

    // init file
    fap file = creer_fap_vide();
    file = inserer(file, g, g->poids);

    while (!est_fap_vide(file)) {
        file = processing_dijkstra(file, r, &destination);
    }


    if (destination != NULL) {
        parc_t act = destination->chemins;

        if (print) {
            printf(" %d |", g->label);
            while (act != NULL) {
                // char c;
                // scanf("%c",&c);
                printf(" %d |", act->dest->label);
                act = act->arc_suivant;
            }
            printf("\nPoids = %d\n", destination->poids);
        }
        return destination->poids;
    }
    if (print) {
        printf("Pas de chemins disponibles, sommet inexistant ou inateignable\n");
    }
    return -1;
}





// ======================================================================

int degre_sortant_sommet (pgraphe_t g, psommet_t s)
{
  /*
    Cette fonction retourne le nombre d'arcs sortants
    du sommet n dans le graphe g
  */

  return 0 ;
}

int degre_entrant_sommet (pgraphe_t g, psommet_t s)
{
  /*
    Cette fonction retourne le nombre d'arcs entrants
    dans le noeud n dans le graphe g
  */

  return 0 ;
}

int degre_maximal_graphe (pgraphe_t g)
{
  /*
    Max des degres des sommets du graphe g
  */

  return 0 ;
}


int degre_minimal_graphe (pgraphe_t g)
{
  /*
    Min des degres des sommets du graphe g
  */

  return 0 ;
}


int independant (pgraphe_t g)
{
  /* Les aretes du graphe n'ont pas de sommet en commun */

  return 0 ;
}



int complet (pgraphe_t g)
{
  /* Toutes les paires de sommet du graphe sont jointes par un arc */

  return 0 ;
}

int regulier (pgraphe_t g)
{
  /*
     graphe regulier: tous les sommets ont le meme degre
     g est le ponteur vers le premier sommet du graphe
     renvoie 1 si le graphe est régulier, 0 sinon
  */

  return 0 ;
}




/*
  placer les fonctions de l'examen 2017 juste apres
*/

int elementaire(pgraphe_t g, pchemin_t c){

  init_couleur_sommet(g);

  psommet_t sommetCourant = c->start;
  pchemin_t cheminRestant;

  cheminRestant = c;

  while(cheminRestant != NULL){
  	sommetCourant = cheminRestant->start;
  	if(sommetCourant->couleur == 1) {return 0;}
	sommetCourant->couleur = 1;
  	cheminRestant = cheminRestant->suite;
  }

  return 1;
}

int simple(pgraphe_t g, pchemin_t c){

  init_couleur_arc(g);

  parc_t arcCourant = c->arc;
  pchemin_t cheminRestant;

  cheminRestant = c;

  while(cheminRestant != NULL){
  	arcCourant = cheminRestant->arc;
  	if(arcCourant->couleur == 1) {return 0;}
	arcCourant->couleur = 1;
  	cheminRestant = cheminRestant->suite;
  }

  return 1;
}

int hamiltonien(pgraphe_t g, pchemin_t c){

  init_couleur_sommet(g);
  pchemin_t cheminRestant = c;

  while(cheminRestant != NULL){
	cheminRestant -> start -> couleur = 8;
	cheminRestant = cheminRestant->suite;
  }

  psommet_t sommetCourant = g;
  while(sommetCourant != NULL){
  	if(sommetCourant->couleur != 8) {return 0;}
  	sommetCourant = sommetCourant->sommet_suivant;
  }

  return 1;
}

int eulerien(pgraphe_t g, pchemin_t c){

  init_couleur_arc(g);
  pchemin_t cheminRestant = c;

  while(cheminRestant != NULL){
	cheminRestant -> arc -> couleur = 5;
	cheminRestant = cheminRestant->suite;
  }

  psommet_t sommetCourant = g;
  parc_t arcCourant;

  while (sommetCourant != NULL)
    {
      arcCourant = sommetCourant->liste_arcs;
      while(arcCourant != NULL){
      	if(arcCourant->couleur != 5) {return 0;}
      	arcCourant = arcCourant->arc_suivant;
      }
      sommetCourant = sommetCourant->sommet_suivant ; // passer au sommet suivant dans le graphe
    }
  return 1;
}

/*
void ajoutQueueChemin (pchemin_t chemin, pchemin_t nouveau) {
    if (chemin == NULL) {
        chemin = nouveau;
    } else {
        pchemin_t cur;
        for (cur = chemin; cur->suite != NULL; cur = cur->suite) {
        }

        cur->suite = nouveau;
    }
}

void detruireQueueChemin (pchemin_t chemin) {
    if (chemin == NULL) {
        return;
    }

    if (chemin->suite == NULL) {
        free(chemin->suite);
        chemin = NULL;
        return;
    }

    pchemin_t cur;
    for (cur = chemin; cur->suite->suite != NULL; cur = cur->suite) {
    }

    free(cur->suite);
    cur->suite = NULL;

}

int tailleChemins(pchemin_t c) {
    pchemin_t cur = c;
    int i = 0;
    while (cur != NULL) {
        cur = cur->suite;
        i++;
    }
    return i;
}

int traitement_eulerien(pgraphe_t g, psommet_t actuel, pchemin_t chemin, int n) {
    printf("AHHAH\n");
    if (eulerien(g,chemin)) {
        return 1;
    }
    if (tailleChemins(chemin) >= n) {
        return 0;
    }

    for (parc_t cur = g->liste_arcs; cur != NULL; cur = cur->arc_suivant) {
        pchemin_t nouv = (pchemin_t)malloc(sizeof(chemin_t));
        nouv->start = actuel;
        nouv->arc = cur;
        nouv->suite = NULL;
        ajoutQueueChemin(chemin, nouv);
        if (traitement_eulerien(g,cur->dest, chemin, n)) {
            return 1;
        }
        detruireQueueChemin(chemin);
    }
    return 0;
}

int graphe_eulerien (pgraphe_t g){
    for (psommet_t cur = g; cur != NULL; cur = cur->sommet_suivant) {
        cur->couleur = RED;
    }

    int n = nombre_arcs(g);

    for (psommet_t cur = g; cur != NULL; cur = cur->sommet_suivant) {
        pchemin_t chemin = NULL;
        if (traitement_eulerien(g, g, chemin, n)) {
            return 1;
        }
    }
    return 0;
}*/

int distance(pgraphe_t g, int x, int y){
    pgraphe_t gX = NULL, gY = NULL;

    psommet_t cur = g;
    while (cur != NULL) {
        if (cur->label == x) {
            gX = cur;
        }
        if (cur->label == y) {
            gY = cur;
        }

        cur = cur->sommet_suivant;
    }

    if (gX == NULL || gY == NULL) {
        if (print) {
            printf("Label %d ou %d n'existent pas\n", x, y);
        }
        return -1;
    }

    int n1 = algo_dijkstra(gX,y);
    int n2 = algo_dijkstra(gY,x);

    if (n1 == -1) {
        return n2;
    }

    if (n2 == -1) {
        return n1;
    }
    return ( n1 < n2 ? n1 : n2);
}

int excentricite (pgraphe_t g, int n){
    int max = 0;

    psommet_t cur = g;
    while (cur != NULL) {
        int exc = distance(g, n, cur->label);

        if (exc > max) {
            max = exc;
        }

        cur = cur->sommet_suivant;
    }

    return max;
}

int diametre (pgraphe_t g){
    int max = 0;

    psommet_t cur = g;
    while (cur != NULL) {
        int dia = excentricite(g, cur->label);

        if (dia > max) {
            max = dia;
        }

        cur = cur->sommet_suivant;
    }

    return max;
}
