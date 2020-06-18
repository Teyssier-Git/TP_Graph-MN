#include <stdlib.h>

#include "file.h"

pfile_t creer_file ()
{
  pfile_t f=(pfile_t)malloc(sizeof(file_t));
  f->tete=0;
  f->queue=0;
  f->vide=1;
  return f;
}

int detruire_file (pfile_t f)
{
  free(f);
  return 0 ;
}

int file_vide (pfile_t f)
{
  return f->vide;
}

int file_pleine (pfile_t f)
  {
  return (f->vide!=1 && f->tete==f->queue ? 1 : 0) ;
}

psommet_t defiler (pfile_t f)
  {
      psommet_t p=f->Tab[f->tete];
      f->tete = (f->tete+1)%MAX_FILE_SIZE;
      if (f->tete==f->queue) {
          f->vide=1;
      }
      return p;
}

int enfiler (pfile_t f, psommet_t p)
{
  f->Tab[f->queue]=p;
  f->queue = (f->queue+1)%MAX_FILE_SIZE;
  if (f->vide==1) {
      f->vide=0;
  }
  return 0 ;
}
