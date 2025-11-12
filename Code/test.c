#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include "Morpho_maths_numerique.h"


int main(void){

    double alpha = 0.33;
    double beta = 0.33;

    image *img = recupere_image("singe.pgm");
    int L[2] = {img->hauteur, img->largeur};
    int origine[2] = {img->hauteur/2, img->largeur/2};
    fonction *f = fonction_of_image(img);
    structurant *D = Disque(L, 3, origine);

    fonction *f_inf = ouverture_numerique(f, D);
    fonction *f_sup = fermeture_numerique(f, D);

    fonction *f_ero = erosion_fonctionnelle(f, D);
    fonction *f_dil = dilatation_fonctionnelle(f, D);

    ecrire_image("singe_dilate_3.pgm", image_depuis_fonction(dilatation_fonctionnelle(f, D)));
    ecrire_image("singe_erode_3.pgm", image_depuis_fonction(erosion_fonctionnelle(f, D)));
    ecrire_image("singe_ouvert_3.pgm", image_depuis_fonction(ouverture_numerique(f, D)));
    ecrire_image("singe_ferme_3.pgm", image_depuis_fonction(fermeture_numerique(f, D)));
    ecrire_image("singe_rehausse_equi_ouv_fer.pgm", image_depuis_fonction(rehaussement_contraste(f, alpha, beta, f_inf, f_sup)));
    ecrire_image("singe_equi_ero_dil.pgm", image_depuis_fonction(rehaussement_contraste(f, alpha, beta, f_ero, f_dil)));
    ecrire_image("singe_gradient.pgm", image_depuis_fonction(gradient_morpho(f, origine)));
    ecrire_image("singe_chapeau_hdf.pgm", image_depuis_fonction(chapeau_haut_de_forme(f, D)));
    
    libere_structurant(D);
    libere_fonction(f);
    libere_fonction(f_inf);
    libere_fonction(f_sup);
    libere_fonction(f_ero);
    libere_fonction(f_dil);
    libere_image(img);

    return 0;
}