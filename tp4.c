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

/* Question B.6 */
typedef struct {
    int numeroPhrase;
    char **mots;
    int *ordres;
    int taille;
    int capacite;
} Phrase;

void ajouterMotPhrase(Phrase *phrase, char *mot, int ordre) {
    if (phrase->taille >= phrase->capacite) {
        phrase->capacite *= 2;
        phrase->mots = realloc(phrase->mots, phrase->capacite * sizeof(char*));
        phrase->ordres = realloc(phrase->ordres, phrase->capacite * sizeof(int));
    }
    phrase->mots[phrase->taille] = mot;
    phrase->ordres[phrase->taille] = ordre;
    phrase->taille++;
}

void collecterPhrasesRecursif(T_Noeud *noeud, Phrase **phrases, int *nbPhrases, int *capacitePhrases) {
    if (noeud == NULL) {
        return;
    }
    
    collecterPhrasesRecursif(noeud->filsGauche, phrases, nbPhrases, capacitePhrases);
    
    T_Position *pos = noeud->listePositions;
    while (pos != NULL) {
        int numPhrase = pos->numeroPhrase;
        int i;
        int trouve = 0;
        
        for (i = 0; i < *nbPhrases; i++) {
            if (phrases[i]->numeroPhrase == numPhrase) {
                ajouterMotPhrase(phrases[i], noeud->mot, pos->ordre);
                trouve = 1;
                break;
            }
        }
        
        if (!trouve) {
            if (*nbPhrases >= *capacitePhrases) {
                *capacitePhrases *= 2;
                *phrases = realloc(*phrases, (*capacitePhrases) * sizeof(Phrase*));
            }
            
            phrases[*nbPhrases] = malloc(sizeof(Phrase));
            phrases[*nbPhrases]->numeroPhrase = numPhrase;
            phrases[*nbPhrases]->capacite = 10;
            phrases[*nbPhrases]->taille = 0;
            phrases[*nbPhrases]->mots = malloc(10 * sizeof(char*));
            phrases[*nbPhrases]->ordres = malloc(10 * sizeof(int));
            ajouterMotPhrase(phrases[*nbPhrases], noeud->mot, pos->ordre);
            (*nbPhrases)++;
        }
        
        pos = pos->suivant;
    }
    
    collecterPhrasesRecursif(noeud->filsDroit, phrases, nbPhrases, capacitePhrases);
}

int comparerOrdres(const void *a, const void *b) {
    int *ordreA = (int*)a;
    int *ordreB = (int*)b;
    return *ordreA - *ordreB;
}

void afficherOccurencesMot(T_Index index, char *mot) {
    T_Noeud *noeud = rechercherMot(index, mot);
    
    if (noeud == NULL) {
        printf("Le mot '%s' n'existe pas dans l'index.\n", mot);
        return;
    }
    
    int capacitePhrases = 10;
    int nbPhrases = 0;
    Phrase **phrases = malloc(capacitePhrases * sizeof(Phrase*));
    
    collecterPhrasesRecursif(index.racine, phrases, &nbPhrases, &capacitePhrases);
    
    T_Position *pos = noeud->listePositions;
    while (pos != NULL) {
        int numPhrase = pos->numeroPhrase;
        int i;
        
        for (i = 0; i < nbPhrases; i++) {
            if (phrases[i]->numeroPhrase == numPhrase) {
                int j;
                int *indices = malloc(phrases[i]->taille * sizeof(int));
                for (j = 0; j < phrases[i]->taille; j++) {
                    indices[j] = j;
                }
                
                int k;
                for (j = 0; j < phrases[i]->taille - 1; j++) {
                    for (k = j + 1; k < phrases[i]->taille; k++) {
                        if (phrases[i]->ordres[indices[j]] > phrases[i]->ordres[indices[k]]) {
                            int temp = indices[j];
                            indices[j] = indices[k];
                            indices[k] = temp;
                        }
                    }
                }
                
                printf("Phrase %d : ", numPhrase);
                for (j = 0; j < phrases[i]->taille; j++) {
                    printf("%s", phrases[i]->mots[indices[j]]);
                    if (j < phrases[i]->taille - 1) {
                        printf(" ");
                    }
                }
                printf(".\n");
                
                free(indices);
                break;
            }
        }
        
        pos = pos->suivant;
    }
    
    int i;
    for (i = 0; i < nbPhrases; i++) {
        free(phrases[i]->mots);
        free(phrases[i]->ordres);
        free(phrases[i]);
    }
    free(phrases);
}

/* Question B.7 */
void construireTexte(T_Index index, char *filename) {
    int capacitePhrases = 10;
    int nbPhrases = 0;
    Phrase **phrases = malloc(capacitePhrases * sizeof(Phrase*));
    
    collecterPhrasesRecursif(index.racine, phrases, &nbPhrases, &capacitePhrases);
    
    int i;
    for (i = 0; i < nbPhrases - 1; i++) {
        int j;
        for (j = i + 1; j < nbPhrases; j++) {
            if (phrases[i]->numeroPhrase > phrases[j]->numeroPhrase) {
                Phrase *temp = phrases[i];
                phrases[i] = phrases[j];
                phrases[j] = temp;
            }
        }
    }
    
    FILE *fichier = fopen(filename, "w");
    if (fichier == NULL) {
        for (i = 0; i < nbPhrases; i++) {
            free(phrases[i]->mots);
            free(phrases[i]->ordres);
            free(phrases[i]);
        }
        free(phrases);
        return;
    }
    
    for (i = 0; i < nbPhrases; i++) {
        int *indices = malloc(phrases[i]->taille * sizeof(int));
        int j;
        for (j = 0; j < phrases[i]->taille; j++) {
            indices[j] = j;
        }
        
        int k;
        for (j = 0; j < phrases[i]->taille - 1; j++) {
            for (k = j + 1; k < phrases[i]->taille; k++) {
                if (phrases[i]->ordres[indices[j]] > phrases[i]->ordres[indices[k]]) {
                    int temp = indices[j];
                    indices[j] = indices[k];
                    indices[k] = temp;
                }
            }
        }
        
        for (j = 0; j < phrases[i]->taille; j++) {
            if (j == 0) {
                fprintf(fichier, "%c%s", toupper(phrases[i]->mots[indices[j]][0]), 
                        phrases[i]->mots[indices[j]] + 1);
            } else {
                fprintf(fichier, " %s", phrases[i]->mots[indices[j]]);
            }
        }
        fprintf(fichier, ".");
        
        if (i < nbPhrases - 1) {
            fprintf(fichier, " ");
        }
        
        free(indices);
    }
    
    fclose(fichier);
    
    for (i = 0; i < nbPhrases; i++) {
        free(phrases[i]->mots);
        free(phrases[i]->ordres);
        free(phrases[i]);
    }
    free(phrases);
}

void libererPositions(T_Position *liste) {
    while (liste != NULL) {
        T_Position *temp = liste;
        liste = liste->suivant;
        free(temp);
    }
}

void libererNoeud(T_Noeud *noeud) {
    if (noeud == NULL) {
        return;
    }
    
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
