#ifndef _TIPE_ENSEMBLES_H
#define _TIPE_ENSEMBLES_H


#include <stdbool.h>


typedef struct fonction{     
    int *L;                                               
    int **valeurs;                           
} fonction;


typedef struct structurant{ // Disque dans un carré de coté 2r
    bool **forme;                           
    int *origine;   // centre du cercle dans l'image
    int rayon;                           
} structurant;


typedef struct image{
    int hauteur;
    int largeur;
    int **valeurs;
} image;


void ecrire_image(char *nom_fichier, image *img);  // Check
image *recupere_image(char *nom_fichier);  // Check
image *image_depuis_fonction(fonction *f);  // Check


fonction *fonction_nulle(int *L);  // Check
structurant *structurant_vide(int *origine, int rayon);  // Check


void dessine_fonction(fonction *f, fonction *g);  // Check // dessine g sur f
void libere_fonction(fonction *f);  // Check
void libere_structurant(structurant *B);  // Check
void libere_image(image *img);  // Check
fonction *fonction_of_image(image *img);  // Check
fonction *fonction_of_structurant(structurant *B); // Check


bool est_dedans(int i, int j, int *L);  // Check
int sup(fonction *f, structurant *B);  // Check
int inf(fonction *f, structurant *B);  // Check
void translate_B_vers_x(structurant *B, int *x);
//structurant *symetrique_centre(structurant *B);  // Check
structurant *Disque(int *L, int r, int *o);  // Check


fonction *dilatation_fonctionnelle(fonction *f, structurant *B); // Effet : étale les maxima locaux.   // Check
fonction *erosion_fonctionnelle(fonction *f, structurant *B);    // Effet : rétrécit les maxima locaux (opération duale de la dilatation fonctionnelle).   // Check
fonction *ouverture_numerique(fonction *f, structurant *B);      // Effet : supprime les composantes connexes plus petites en taille que le structurant.  // Check
fonction *fermeture_numerique(fonction *f, structurant *B);      // Effet : comble les vallées plus étroites que  l'élément structurant (opération duale de l'ouverture numérique).  // Check
fonction *rehaussement_contraste(fonction *f, double alpha, double beta, fonction *f_inf, fonction *f_sup);  // Effet : Rehausse les contrastes de l'image.
fonction *gradient_morpho(fonction *f, int *origine);  // Effet : Donne une image sur laquelle les évolutions sont plus ou moins colorées selon leur intensité.
fonction *chapeau_haut_de_forme(fonction *f, structurant *B);  // Effet : extrait les pics plus étroits que l'élément structurant.


#endif