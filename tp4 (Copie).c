#include "tp4.h"

void initialiserIndex(T_Index *index) {
    index->racine = NULL;
    index->nbMotsDistincts = 0;
    index->nbMotsTotal = 0;
}

char *convertirMinuscules(char *mot) {
    char *resultat = malloc(strlen(mot) + 1);
    int i;
    for (i = 0; mot[i]; i++) {
        resultat[i] = tolower(mot[i]);
    }
    resultat[i] = '\0';
    return resultat;
}

/* Question B.1 */
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

/* Question B.2 */
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

/* Question B.3 */
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
        if (isalpha(c)) {
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

/* Question B.4 */
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

/* Question B.5 */
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

/* Question B.6 - Version claire et optimisée */

// Structure pour stocker un mot avec son ordre dans la phrase
typedef struct MotOrdre {
    char *mot;
    int ordre;
    struct MotOrdre *suivant;
} T_MotOrdre;

// Fonction auxiliaire : ajouter un mot dans une liste triée par ordre
T_MotOrdre *ajouterMotOrdre(T_MotOrdre *liste, char *mot, int ordre) {
    T_MotOrdre *nouveau = malloc(sizeof(T_MotOrdre));
    nouveau->mot = mot;
    nouveau->ordre = ordre;
    nouveau->suivant = NULL;
    
    // Liste vide ou insertion en tête
    if (liste == NULL || ordre < liste->ordre) {
        nouveau->suivant = liste;
        return nouveau;
    }
    
    // Insertion triée
    T_MotOrdre *courant = liste;
    while (courant->suivant != NULL && courant->suivant->ordre < ordre) {
        courant = courant->suivant;
    }
    
    nouveau->suivant = courant->suivant;
    courant->suivant = nouveau;
    return liste;
}

// Fonction auxiliaire : collecter tous les mots d'une phrase donnée
void collecterMotsPhrase(T_Noeud *noeud, int numeroPhrase, T_MotOrdre **listeMots) {
    if (noeud == NULL) {
        return;
    }
    
    // Parcours infixe de l'arbre
    collecterMotsPhrase(noeud->filsGauche, numeroPhrase, listeMots);
    
    // Vérifier si ce noeud contient des occurrences dans la phrase recherchée
    T_Position *pos = noeud->listePositions;
    while (pos != NULL) {
        if (pos->numeroPhrase == numeroPhrase) {
            // Ajouter le mot avec son ordre dans la liste triée
            *listeMots = ajouterMotOrdre(*listeMots, noeud->mot, pos->ordre);
        }
        pos = pos->suivant;
    }
    
    collecterMotsPhrase(noeud->filsDroit, numeroPhrase, listeMots);
}

// Fonction auxiliaire : afficher une phrase à partir d'une liste de mots triés
void afficherPhrase(T_MotOrdre *listeMots, int numeroPhrase) {
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

// Fonction auxiliaire : libérer une liste de mots ordonnés
void libererMotsOrdres(T_MotOrdre *liste) {
    while (liste != NULL) {
        T_MotOrdre *temp = liste;
        liste = liste->suivant;
        free(temp);
    }
}

// Fonction principale B.6
void afficherOccurencesMot(T_Index index, char *mot) {
    // 1. Rechercher le mot dans l'index
    T_Noeud *noeud = rechercherMot(index, mot);
    
    if (noeud == NULL) {
        printf("Le mot '%s' n'existe pas dans l'index.\n", mot);
        return;
    }
    
    // 2. Afficher les informations du mot (comme afficherNoeudInfixe)
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
    
    // 3. Pour chaque occurrence, afficher la phrase complète
    pos = noeud->listePositions;
    while (pos != NULL) {
        int numeroPhrase = pos->numeroPhrase;
        
        // Collecter tous les mots de cette phrase (liste triée par ordre)
        T_MotOrdre *listeMots = NULL;
        collecterMotsPhrase(index.racine, numeroPhrase, &listeMots);
        
        // Afficher la phrase reconstituée
        afficherPhrase(listeMots, numeroPhrase);
        
        // Libérer la mémoire
        libererMotsOrdres(listeMots);
        
        pos = pos->suivant;
    }
}

/* Question B.7 - Version optimisée réutilisant B6 */

// Trouver le nombre max de phrases dans l'index
int compterPhrasesMax(T_Noeud *noeud, int *maxPhrase) {
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

// Compter les phrases couvertes par un mot
int compterPhrasesMot(T_Noeud *noeud) {
    if (noeud == NULL) return 0;
    
    int count = 0;
    int dernierPhrase = -1;
    T_Position *pos = noeud->listePositions;
    
    while (pos != NULL) {
        if (pos->numeroPhrase != dernierPhrase) {
            count++;
            dernierPhrase = pos->numeroPhrase;
        }
        pos = pos->suivant;
    }
    return count;
}

void construireTexte(T_Index index, char *filename) {
    // 1. Trouver le nombre total de phrases
    int nbPhrasesTotal = 0;
    compterPhrasesMax(index.racine, &nbPhrasesTotal);
    
    printf("Nombre total de phrases : %d\n", nbPhrasesTotal);
    
    FILE *fichier = fopen(filename, "w");
    if (fichier == NULL) return;
    
    // 2. Reconstruire chaque phrase en réutilisant collecterMotsPhrase de B6
    for (int i = 1; i <= nbPhrasesTotal; i++) {
        T_MotOrdre *listeMots = NULL;
        collecterMotsPhrase(index.racine, i, &listeMots);
        
        // Écrire la phrase
        T_MotOrdre *courant = listeMots;
        int premier = 1;
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