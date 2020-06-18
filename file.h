#ifndef __FILE_H__
#define __FILE_H__

#include "graphe.h"
#define MAX_FILE_SIZE       32


typedef struct {
  int tete ;
  int queue ;
  int vide;
  psommet_t Tab [MAX_FILE_SIZE] ;
} file_t, *pfile_t ;

pfile_t creer_file () ;

int detruire_file (pfile_t f) ;

int file_vide (pfile_t f) ;

int file_pleine (pfile_t f) ;

psommet_t defiler (pfile_t f)  ;

int enfiler (pfile_t f, psommet_t p) ;

#endif
