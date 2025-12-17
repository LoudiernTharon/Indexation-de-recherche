#ifndef TP4_H
#define TP4_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* A. Structures de donnees */

typedef struct Position {
    int numeroLigne;
    int ordre;
    int numeroPhrase;
    struct Position *suivant;
} T_Position;

typedef struct Noeud {
    char *mot;
    int nbOccurences;
    T_Position *listePositions;
    struct Noeud *filsGauche;
    struct Noeud *filsDroit;
} T_Noeud;

typedef struct Index {
    T_Noeud *racine;
    int nbMotsDistincts;
    int nbMotsTotal;
} T_Index;

/* B. Fonctions de base */

/* Question B.1 */
T_Position *ajouterPosition(T_Position *listeP, int ligne, int ordre, int phrase);

/* Question B.2 */
int ajouterOccurence(T_Index *index, char *mot, int ligne, int ordre, int phrase);

/* Question B.3 */
int indexerFichier(T_Index *index, char *filename);

/* Question B.4 */
void afficherIndex(T_Index index);

/* Question B.5 */
T_Noeud *rechercherMot(T_Index index, char *mot);

/* Question B.6 */
void afficherOccurencesMot(T_Index index, char *mot);

/* Question B.7 */
void construireTexte(T_Index index, char *filename);

/* Fonctions auxiliaires */
void initialiserIndex(T_Index *index);
void libererPositions(T_Position *liste);
void libererNoeud(T_Noeud *noeud);

void libererIndex(T_Index *index);
void afficherNoeudInfixe(T_Noeud *noeud);
char *convertirMinuscules(char *mot);
void collecterMotsPhrase(T_Noeud *noeud, int numeroPhrase, T_MotOrdre **listeMots);
void collecterPhrasesRecursif(T_Noeud *noeud, T_Phrase **phrases, int *nbPhrases, int *capacitePhrases);

#endif
