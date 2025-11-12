#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include "Morpho_maths_numerique.h"


void ecrire_image(char *nom_fichier, image *img){
    FILE *fichier = fopen(nom_fichier, "w");
    if(fichier == NULL){
        printf("Erreur ! Fichier non ouvert.");
        return;
    }
    fprintf(fichier, "P2\n%d %d\n", img->largeur, img->hauteur);
    fprintf(fichier, "%d\n", 255);
    for(int i = 0; i < img->hauteur; i++){
        for(int j = 0; j < img->largeur; j++){
            fprintf(fichier, "%d ", img->valeurs[i][j]);
        }
        fprintf(fichier, "\n");
    }
    fclose(fichier);
}

image *recupere_image(char *nom_fichier){
    FILE *fichier = fopen(nom_fichier, "r");
    if(fichier == NULL){
        printf("Erreur ! Fichier non ouvert.\n");
        return NULL;
    }
    char *mode = malloc(3 * sizeof(char));
    if (mode == NULL) {
        printf("Erreur d'allocation pour le mode.\n");
        fclose(fichier);
        return NULL;
    }
    
    int hauteur, largeur, max;
    fscanf(fichier, "%s %d %d %d", mode, &hauteur, &largeur, &max);
    printf("%s -> Mode: %s, Largeur: %d, Hauteur: %d, Max: %d\n", nom_fichier, mode, largeur, hauteur, max);

    
    image *img = malloc(sizeof(image));
    if (img == NULL) {
        printf("Erreur d'allocation pour l'image.\n");
        free(mode);
        fclose(fichier);
        return NULL;
    }
    
    img->hauteur = hauteur;
    img->largeur = largeur;
    img->valeurs = malloc(hauteur * sizeof(int*));
    if (img->valeurs == NULL) {
        printf("Erreur d'allocation pour les lignes.\n");
        free(mode);
        free(img);
        fclose(fichier);
        return NULL;
    }
    
    for(int i = 0; i < hauteur; i++){
        img->valeurs[i] = malloc(largeur * sizeof(int));
        if (img->valeurs[i] == NULL) {
            printf("Erreur d'allocation pour la colonne %d.\n", i);
            for (int k = 0; k < i; k++) free(img->valeurs[k]);
            free(img->valeurs);
            free(img);
            free(mode);
            fclose(fichier);
            return NULL;
        }
    }
    
    for(int i = 0; i < img->hauteur; i++){
        for(int j = 0; j < img->largeur; j++){
            fscanf(fichier, "%d", &(img->valeurs[i][j]));
        }
    }

    free(mode);
    fclose(fichier);
    return img;
}

image *image_depuis_fonction(fonction *f){
    image *img = malloc(sizeof(image));
    img->hauteur = f->L[0];
    img->largeur = f->L[1];
    img->valeurs = malloc(img->hauteur * sizeof(int *));
    for(int i = 0; i < img->hauteur; i++){
        img->valeurs[i] = malloc(img->largeur * sizeof(int));
        for(int j = 0; j < img->largeur; j++){
            img->valeurs[i][j] = f->valeurs[i][j];
        }
    }
    return img;
}

fonction *fonction_nulle(int *L){
    fonction *f_0 = malloc(sizeof(fonction));
    f_0->L = L;
    f_0->valeurs = malloc(L[0]*sizeof(int*));
    for(int i = 0; i < L[0]; i++){
        f_0->valeurs[i] = malloc(L[1]*sizeof(int));
        for(int j = 0; j < L[1]; j++){
            f_0->valeurs[i][j] = 0;
        }
    }
    return f_0;
}

structurant *structurant_vide(int *origine, int rayon){
    structurant *ret = malloc(sizeof(structurant));
    ret->rayon = rayon;
    ret->forme = malloc(2*rayon*sizeof(bool*));
    for(int i = 0; i < 2*rayon; i++){
        ret->forme[i] = calloc(2*rayon, sizeof(bool));
    }
    ret->origine = origine;
    return ret;
}

void dessine_fonction(fonction *f, fonction *g){
    for(int i = 0; i < f->L[0]; i++){
        for(int j = 0; j < f->L[1]; j++){
            if(g->valeurs[i][j] > 0){
                f->valeurs[i][j] = g->valeurs[i][j];
            }
        }
    }
    return;
}

void libere_fonction(fonction *f){
    for(int i = 0; i < f->L[0]; i++){
        free(f->valeurs[i]);
    }
    free(f->valeurs);
    free(f);
    return;
}

void libere_structurant(structurant *B){
    for(int i = 0; i < 2*B->rayon; i++){
        free(B->forme[i]);
    }
    free(B->forme);
    free(B);
    return;
}

void libere_image(image *img){
    for(int i = 0; i < img->hauteur; i++){
        free(img->valeurs[i]);
    }
    free(img->valeurs);
    free(img);
    return;
}

fonction *fonction_of_image(image *img){
    int *L = malloc(2*sizeof(int));
    L[0] = img->hauteur; L[1] = img->largeur;
    fonction *C = fonction_nulle(L);
    for(int i = 0; i < L[0]; i++){
        for(int j = 0; j < L[1]; j++){
            C->valeurs[i][j] = img->valeurs[i][j];
        }
    }
    return C;
}

fonction *fonction_of_structurant(structurant *B){
    int *L = malloc(2*sizeof(int));
    L[0] = 2*B->rayon;
    L[1] = 2*B->rayon;
    fonction *f = fonction_nulle(L);
    f->L = L;
    for(int i = 0; i < 2*B->rayon; i++){
        for(int j = 0; j < 2*B->rayon; j++){
            f->valeurs[i][j] = 255*B->forme[i][j];
        }
    }
    return f;
}

bool est_dedans(int i, int j, int *L){
    return 0 <= i && i < L[0] && 0 <= j && j < L[1];
}

int sup(fonction *f, structurant *B){
    int s = 0;
    int o[2] = {B->origine[0], B->origine[1]};
    int r = B->rayon;
    for(int i = o[0] - r; i < o[0] + r; i++){
        for(int j = o[1] - r; j < o[1] + r; j++){
            if(est_dedans(i, j, f->L) && B->forme[i + r - o[0]][j + r - o[1]] && s <= f->valeurs[i][j]){
                s = f->valeurs[i][j];
            }
        }
    }
    return s;
}

int inf(fonction *f, structurant *B){
    int s = 255;
    int o[2] = {B->origine[0], B->origine[1]};
    int r = B->rayon;
    for(int i = o[0] - r; i < o[0] + r; i++){
        for(int j = o[1] - r; j < o[1] + r; j++){
            if(est_dedans(i, j, f->L) && B->forme[i + r - o[0]][j + r - o[1]] && s >= f->valeurs[i][j]){
                s = f->valeurs[i][j];
            }
        }
    }
    return s;
}

void translate_B_vers_x(structurant *B, int *x){
    B->origine = x;
    return;
}

/*structurant *symetrique_centre(structurant *B){
    structurant *S_B = structurant_vide(B->L, B->origine, B->rayon);
    for(int i = 0; i < B->L[0]; i++){
        for(int j = 0; j < B->L[1]; j++){
            int reflexion_i = 2*B->origine[0]-i;
            int reflexion_j = 2*B->origine[1]-j;
            if(0 <= reflexion_i && reflexion_i < B->L[0] && 0 <= reflexion_j && reflexion_j < B->L[1]){
                S_B->forme[reflexion_i][reflexion_j] = B->forme[i][j]; 
            }
        }
    }
    return S_B;
}*/

/*structurant *symetrique_centre(structurant *B){
    int o[2] = {B->origine[0], B->origine[1]};
    int r = B->rayon;
    structurant *S_B = structurant_vide(B->origine, B->rayon);
    for(int i = o[0] - r; i < o[0] + r; i++){
        for(int j = o[1] - r; j< o[1] + r; j++){
            int reflexion_i = 2*B->origine[0]-i;
            int reflexion_j = 2*B->origine[1]-j;
            if(0 <= reflexion_i && reflexion_i < B->L[0] && 0 <= reflexion_j && reflexion_j < B->L[1]){
                S_B->forme[reflexion_i][reflexion_j] = B->forme[i][j]; 
            }
        }
    }
    return S_B;
}*/

structurant *Disque(int *L, int r, int *o){
    structurant *B = structurant_vide(o, r);
    for(int i = o[0] - r; i < o[0] + r; i++){
        for(int j = o[1] - r; j< o[1] + r; j++){
            int dist_2 = (i-o[0]) * (i-o[0]) + (j-o[1]) * (j-o[1]);
            if(est_dedans(i, j, L)){
                if(dist_2 <= r*r){
                    B->forme[i + r - o[0]][j + r - o[1]] = true;
                }
            }
        }
    }
    return B;
}  

fonction *dilatation_fonctionnelle(fonction *f, structurant *B){
    fonction *f_d = fonction_nulle(f->L);
    int *x = malloc(2*sizeof(int));
    for(int i = 0; i < f->L[0]; i++){
        x[0] = i;
        for(int j = 0; j < f->L[1]; j++){
            x[1] = j;
            translate_B_vers_x(B, x);
            f_d->valeurs[i][j] = sup(f, B);
        }
    }
    free(x);
    return f_d;
}

fonction *erosion_fonctionnelle(fonction *f, structurant *B){
    fonction *f_e = fonction_nulle(f->L);
    int *x = malloc(2*sizeof(int));
    for(int i = 0; i < f->L[0]; i++){
        x[0] = i;
        for(int j = 0; j < f->L[1]; j++){
            x[1] = j;
            translate_B_vers_x(B, x);
            f_e->valeurs[i][j] = inf(f, B);
        }
    }
    free(x);
    return f_e;
}

/*fonction *ouverture_numerique(fonction *f, structurant *B){
    structurant *S_B = symetrique_centre(B);
    return dilatation_fonctionnelle(erosion_fonctionnelle(f, B), S_B);
}

fonction *fermeture_numerique(fonction *f, structurant *B){
    structurant *S_B = symetrique_centre(B);
    return erosion_fonctionnelle(dilatation_fonctionnelle(f, B), S_B);
}*/

fonction *ouverture_numerique(fonction *f, structurant *B){
    return dilatation_fonctionnelle(erosion_fonctionnelle(f, B), B);
}

fonction *fermeture_numerique(fonction *f, structurant *B){
    return erosion_fonctionnelle(dilatation_fonctionnelle(f, B), B);
}

fonction *rehaussement_contraste(fonction *f, double alpha, double beta, fonction *f_inf, fonction *f_sup){
    fonction *g = fonction_nulle(f->L);
    for(int i = 0; i < f->L[0]; i++){
        for(int j = 0; j < f->L[1]; j++){
            int Delta_f_i_j = f_sup->valeurs[i][j] - f_inf->valeurs[i][j];
            if(f_inf->valeurs[i][j] <= f->valeurs[i][j] && f->valeurs[i][j] <= f_inf->valeurs[i][j] + alpha*Delta_f_i_j){
                g->valeurs[i][j] = f_inf->valeurs[i][j];
            }else if(f_inf->valeurs[i][j] + alpha*Delta_f_i_j <= f->valeurs[i][j] && f->valeurs[i][j] <= f_sup->valeurs[i][j] - beta*Delta_f_i_j){
                g->valeurs[i][j] = f->valeurs[i][j];
            }else{
                g->valeurs[i][j] = f_sup->valeurs[i][j];
            }
        }
    }
    return g;
}

fonction *gradient_morpho(fonction *f, int *origine){
    fonction *g = fonction_nulle(f->L);
    structurant *B = Disque(f->L, 1, origine);
    fonction *D = dilatation_fonctionnelle(f, B);
    fonction *E = erosion_fonctionnelle(f, B);
    for(int i = 0; i < f->L[0]; i++){
        for(int j = 0; j < f->L[1]; j++){
            g->valeurs[i][j] = D->valeurs[i][j] - E->valeurs[i][j];
        }
    }
    return g;
}

fonction *chapeau_haut_de_forme(fonction *f, structurant *B){
    fonction *g = fonction_nulle(f->L);
    fonction *ouv = ouverture_numerique(f, B);
    for(int i = 0; i < f->L[0]; i++){
        for(int j = 0; j < f->L[1]; j++){
            if(f->valeurs[i][j] - ouv->valeurs[i][j] <= 0){
                g->valeurs[i][j] = 0;
            }
            else{
                g->valeurs[i][j] = f->valeurs[i][j] - ouv->valeurs[i][j];
            }
        }
    }
    return g;
}
