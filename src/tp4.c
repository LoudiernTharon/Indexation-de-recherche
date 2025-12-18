#include "tp4.h"

/* ========== FONCTIONS AUXILIAIRES ========== */

void initialiserIndex(T_Index *index) {
    index->racine = NULL;
    index->nbMotsDistincts = 0;
    index->nbMotsTotal = 0;
}

char *convertirMinuscules(char *mot) {
    char *resultat = malloc(strlen(mot) + 1);
    int i;
    for (i = 0; mot[i]; i++) {
        unsigned char c = (unsigned char)mot[i];
        if (c >= 'A' && c <= 'Z') {
            resultat[i] = c + 32;
        } else {
            resultat[i] = c;
        }
    }
    resultat[i] = '\0';
    return resultat;
}

/* ========== LIBERATION MEMOIRE ========== */

void libererPositions(T_Position *pos) {
    while (pos != NULL) {
        T_Position *temp = pos;
        pos = pos->suivant;
        free(temp);
    }
}

void libererNoeud(T_Noeud *noeud) {
    if (noeud == NULL) return;
    libererNoeud(noeud->filsGauche);
    libererNoeud(noeud->filsDroit);
    free(noeud->mot);
    libererPositions(noeud->listePositions);
    free(noeud);
}

void libererIndex(T_Index *index) {
    libererNoeud(index->racine);
    index->racine = NULL;
    index->nbMotsDistincts = 0;
    index->nbMotsTotal = 0;
}

/* ========== Question B.1 ========== */
T_Position *ajouterPosition(T_Position *listeP, int ligne, int ordre, int phrase) {
    T_Position *nouveau = malloc(sizeof(T_Position));
    nouveau->numeroLigne = ligne;
    nouveau->ordre = ordre;
    nouveau->numeroPhrase = phrase;
    nouveau->suivant = NULL;
    
    if (listeP == NULL) {
        return nouveau;
    }
    
    if (ligne < listeP->numeroLigne || (ligne == listeP->numeroLigne && ordre < listeP->ordre)) {
        nouveau->suivant = listeP;
        return nouveau;
    }
    
    T_Position *courant = listeP;
    while (courant->suivant != NULL) {
        if (ligne < courant->suivant->numeroLigne || 
            (ligne == courant->suivant->numeroLigne && ordre < courant->suivant->ordre)) {
            break;
        }
        courant = courant->suivant;
    }
    
    nouveau->suivant = courant->suivant;
    courant->suivant = nouveau;
    return listeP;
}

/* ========== Question B.2 ========== */
int ajouterOccurence(T_Index *index, char *mot, int ligne, int ordre, int phrase) {
    char *motMinuscule = convertirMinuscules(mot);
    
    if (index->racine == NULL) {
        index->racine = malloc(sizeof(T_Noeud));
        index->racine->mot = motMinuscule;
        index->racine->nbOccurences = 1;
        index->racine->listePositions = ajouterPosition(NULL, ligne, ordre, phrase);
        index->racine->filsGauche = NULL;
        index->racine->filsDroit = NULL;
        index->nbMotsDistincts++;
        index->nbMotsTotal++;
        return 1;
    }
    
    T_Noeud *courant = index->racine;
    T_Noeud *parent = NULL;
    int comparaison;
    
    while (courant != NULL) {
        comparaison = strcmp(motMinuscule, courant->mot);
        
        if (comparaison == 0) {
            courant->listePositions = ajouterPosition(courant->listePositions, ligne, ordre, phrase);
            courant->nbOccurences++;
            index->nbMotsTotal++;
            free(motMinuscule);
            return 1;
        }
        
        parent = courant;
        if (comparaison < 0) {
            courant = courant->filsGauche;
        } else {
            courant = courant->filsDroit;
        }
    }
    
    T_Noeud *nouveau = malloc(sizeof(T_Noeud));
    nouveau->mot = motMinuscule;
    nouveau->nbOccurences = 1;
    nouveau->listePositions = ajouterPosition(NULL, ligne, ordre, phrase);
    nouveau->filsGauche = NULL;
    nouveau->filsDroit = NULL;
    
    if (strcmp(motMinuscule, parent->mot) < 0) {
        parent->filsGauche = nouveau;
    } else {
        parent->filsDroit = nouveau;
    }
    
    index->nbMotsDistincts++;
    index->nbMotsTotal++;
    return 1;
}

/* ========== Question B.3 ========== */

/* Fonction auxiliaire : verifie si c'est une lettre (y compris UTF-8) */
static int estLettre(unsigned char c) {
    if (c >= 'A' && c <= 'Z') return 1;
    if (c >= 'a' && c <= 'z') return 1;
    if (c >= 128) return 1;  /* Caracteres UTF-8 (accents, etc.) */
    return 0;
}

int indexerFichier(T_Index *index, char *filename) {
    FILE *fichier = fopen(filename, "r");
    if (fichier == NULL) {
        return 0;
    }
    
    int numeroLigne = 1;
    int numeroPhrase = 1;
    int ordre = 1;
    int c;
    char mot[256];
    int i = 0;
    
    while ((c = fgetc(fichier)) != EOF) {
        if (estLettre((unsigned char)c)) {
            mot[i++] = c;
        } else {
            if (i > 0) {
                mot[i] = '\0';
                ajouterOccurence(index, mot, numeroLigne, ordre, numeroPhrase);
                ordre++;
                i = 0;
            }
            
            if (c == '.') {
                numeroPhrase++;
                ordre = 1;
            } else if (c == '\n') {
                numeroLigne++;
                ordre = 1;
            }
        }
    }
    
    if (i > 0) {
        mot[i] = '\0';
        ajouterOccurence(index, mot, numeroLigne, ordre, numeroPhrase);
    }
    
    fclose(fichier);
    return index->nbMotsTotal;
}

/* ========== Question B.4 ========== */
void afficherNoeudInfixe(T_Noeud *noeud) {
    if (noeud == NULL) {
        return;
    }
    
    afficherNoeudInfixe(noeud->filsGauche);
    
    printf("%s (%d occurrence(s)) : ", noeud->mot, noeud->nbOccurences);
    T_Position *pos = noeud->listePositions;
    while (pos != NULL) {
        printf("L%d-M%d-P%d", pos->numeroLigne, pos->ordre, pos->numeroPhrase);
        if (pos->suivant != NULL) {
            printf(", ");
        }
        pos = pos->suivant;
    }
    printf("\n");
    
    afficherNoeudInfixe(noeud->filsDroit);
}

void afficherIndex(T_Index index) {
    printf("\n--- INDEX ---\n");
    printf("Nombre de mots distincts : %d\n", index.nbMotsDistincts);
    printf("Nombre total de mots : %d\n\n", index.nbMotsTotal);
    afficherNoeudInfixe(index.racine);
}

/* ========== Question B.5 ========== */
T_Noeud *rechercherMot(T_Index index, char *mot) {
    char *motMinuscule = convertirMinuscules(mot);
    T_Noeud *courant = index.racine;
    
    while (courant != NULL) {
        int comparaison = strcmp(motMinuscule, courant->mot);
        
        if (comparaison == 0) {
            free(motMinuscule);
            return courant;
        }
        
        if (comparaison < 0) {
            courant = courant->filsGauche;
        } else {
            courant = courant->filsDroit;
        }
    }
    
    free(motMinuscule);
    return NULL;
}

/* ========== Question B.6 ========== */

/* Structure locale pour stocker un mot avec sa position */
typedef struct MotOrdre {
    char *mot;
    int ligne;
    int ordre;
    struct MotOrdre *suivant;
} T_MotOrdre;

/* Ajouter un mot dans une liste triee par (ligne, ordre) */
static T_MotOrdre *ajouterMotOrdre(T_MotOrdre *liste, char *mot, int ligne, int ordre) {
    T_MotOrdre *nouveau = malloc(sizeof(T_MotOrdre));
    nouveau->mot = mot;
    nouveau->ligne = ligne;
    nouveau->ordre = ordre;
    nouveau->suivant = NULL;
    
    if (liste == NULL || 
        ligne < liste->ligne || 
        (ligne == liste->ligne && ordre < liste->ordre)) {
        nouveau->suivant = liste;
        return nouveau;
    }
    
    T_MotOrdre *courant = liste;
    while (courant->suivant != NULL) {
        int ligneSuiv = courant->suivant->ligne;
        int ordreSuiv = courant->suivant->ordre;
        if (ligne < ligneSuiv || (ligne == ligneSuiv && ordre < ordreSuiv)) {
            break;
        }
        courant = courant->suivant;
    }
    
    nouveau->suivant = courant->suivant;
    courant->suivant = nouveau;
    return liste;
}

/* Liberer une liste de mots ordonnes */
static void libererMotsOrdres(T_MotOrdre *liste) {
    while (liste != NULL) {
        T_MotOrdre *temp = liste;
        liste = liste->suivant;
        free(temp);
    }
}

/* Collecter tous les mots d'une phrase donnee */
static void collecterMotsPhrase(T_Noeud *noeud, int numeroPhrase, T_MotOrdre **listeMots) {
    if (noeud == NULL) {
        return;
    }
    
    collecterMotsPhrase(noeud->filsGauche, numeroPhrase, listeMots);
    
    T_Position *pos = noeud->listePositions;
    while (pos != NULL) {
        if (pos->numeroPhrase == numeroPhrase) {
            *listeMots = ajouterMotOrdre(*listeMots, noeud->mot, pos->numeroLigne, pos->ordre);
        }
        pos = pos->suivant;
    }
    
    collecterMotsPhrase(noeud->filsDroit, numeroPhrase, listeMots);
}

/* Afficher une phrase a partir d'une liste de mots tries */
static void afficherPhrase(T_MotOrdre *listeMots, int numeroPhrase) {
    printf("Phrase %d : ", numeroPhrase);
    
    T_MotOrdre *courant = listeMots;
    while (courant != NULL) {
        printf("%s", courant->mot);
        if (courant->suivant != NULL) {
            printf(" ");
        }
        courant = courant->suivant;
    }
    
    printf(".\n");
}

void afficherOccurencesMot(T_Index index, char *mot) {
    T_Noeud *noeud = rechercherMot(index, mot);
    
    if (noeud == NULL) {
        printf("Le mot '%s' n'existe pas dans l'index.\n", mot);
        return;
    }
    
    printf("\n--- Occurrences du mot '%s' ---\n", mot);
    printf("%s (%d occurrence(s)) : ", noeud->mot, noeud->nbOccurences);
    
    T_Position *pos = noeud->listePositions;
    while (pos != NULL) {
        printf("L%d-M%d-P%d", pos->numeroLigne, pos->ordre, pos->numeroPhrase);
        if (pos->suivant != NULL) {
            printf(", ");
        }
        pos = pos->suivant;
    }
    printf("\n\n");
    
    pos = noeud->listePositions;
    int dernierePhrase = -1;
    while (pos != NULL) {
        int numPhrase = pos->numeroPhrase;
        
        if (numPhrase != dernierePhrase) {
            T_MotOrdre *listeMots = NULL;
            collecterMotsPhrase(index.racine, numPhrase, &listeMots);
            afficherPhrase(listeMots, numPhrase);
            libererMotsOrdres(listeMots);
            dernierePhrase = numPhrase;
        }
        
        pos = pos->suivant;
    }
}

/* ========== Question B.7 ========== */

/* Trouver le nombre max de phrases dans l'index */
static int compterPhrasesMax(T_Noeud *noeud, int *maxPhrase) {
    if (noeud == NULL) return 0;
    
    compterPhrasesMax(noeud->filsGauche, maxPhrase);
    
    T_Position *pos = noeud->listePositions;
    while (pos != NULL) {
        if (pos->numeroPhrase > *maxPhrase)
            *maxPhrase = pos->numeroPhrase;
        pos = pos->suivant;
    }
    
    compterPhrasesMax(noeud->filsDroit, maxPhrase);
    return *maxPhrase;
}

void construireTexte(T_Index index, char *filename) {
    int nbPhrasesTotal = 0;
    int i;
    compterPhrasesMax(index.racine, &nbPhrasesTotal);
    
    printf("Nombre total de phrases : %d\n", nbPhrasesTotal);
    
    FILE *fichier = fopen(filename, "w");
    if (fichier == NULL) {
        printf("Erreur : impossible de creer le fichier '%s'\n", filename);
        return;
    }
    
    for (i = 1; i <= nbPhrasesTotal; i++) {
        T_MotOrdre *listeMots = NULL;
        T_MotOrdre *courant;
        int premier = 1;
        
        collecterMotsPhrase(index.racine, i, &listeMots);
        
        courant = listeMots;
        while (courant != NULL) {
            if (premier) {
                fprintf(fichier, "%c%s", toupper(courant->mot[0]), courant->mot + 1);
                premier = 0;
            } else {
                fprintf(fichier, " %s", courant->mot);
            }
            courant = courant->suivant;
        }
        fprintf(fichier, ".");
        if (i < nbPhrasesTotal) fprintf(fichier, " ");
        
        libererMotsOrdres(listeMots);
    }
    
    fclose(fichier);
    printf("Document reconstruit dans '%s'\n", filename);
}
