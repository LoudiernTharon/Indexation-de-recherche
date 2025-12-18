#include "src/tp4.h"

void afficherMenu() {
    printf("\n========== MENU PRINCIPAL ==========\n");
    printf("1. Charger un fichier\n");
    printf("2. Caracteristiques de l'index\n");
    printf("3. Afficher index\n");
    printf("4. Rechercher un mot\n");
    printf("5. Afficher les occurrences d'un mot\n");
    printf("6. Construire le texte a partir de l'index\n");
    printf("7. Quitter\n");
    printf("====================================\n");
    printf("Votre choix : ");
}

int main() {
    T_Index index;
    initialiserIndex(&index);
    char choix[10];
    char filename[256];
    char mot[256];
    int nbMots;
    T_Noeud *noeud;
    
    while (1) {
        afficherMenu();
        scanf("%s", choix);
        
        switch (choix[0]) {
            case '1':
                printf("Nom du fichier a charger : ");
                scanf("%s", filename);
                nbMots = indexerFichier(&index, filename);
                if (nbMots > 0) {
                    printf("Fichier charge avec succes. %d mots lus.\n", nbMots);
                } else {
                    printf("Erreur lors du chargement du fichier.\n");
                }
                break;
                
            case '2':
                printf("\n--- CARACTERISTIQUES DE L'INDEX ---\n");
                printf("Nombre de mots distincts : %d\n", index.nbMotsDistincts);
                printf("Nombre total de mots : %d\n", index.nbMotsTotal);
                break;
                
            case '3':
                afficherIndex(index);
                break;
                
            case '4':
                printf("Mot a rechercher : ");
                scanf("%s", mot);
                noeud = rechercherMot(index, mot);
                if (noeud != NULL) {
                    printf("\nMot trouve : %s (%d occurrence(s))\n", noeud->mot, noeud->nbOccurences);
                    printf("Positions : ");
                    T_Position *pos = noeud->listePositions;
                    while (pos != NULL) {
                        printf("L%d-M%d-P%d", pos->numeroLigne, pos->ordre, pos->numeroPhrase);
                        if (pos->suivant != NULL) {
                            printf(", ");
                        }
                        pos = pos->suivant;
                    }
                    printf("\n");
                } else {
                    printf("Le mot '%s' n'existe pas dans l'index.\n", mot);
                }
                break;
                
            case '5':
                printf("Mot a rechercher : ");
                scanf("%s", mot);
                afficherOccurencesMot(index, mot);
                break;
                
            case '6':
                printf("Nom du fichier de sortie : ");
                scanf("%s", filename);
                construireTexte(index, filename);
                printf("Texte reconstruit et enregistre dans '%s'.\n", filename);
                break;
                
            case '7':
                libererIndex(&index);
                printf("Memoire liberee. Au revoir!\n");
                return 0;
                
            default:
                printf("Choix invalide. Veuillez reessayer.\n");
        }
    }
    
    return 0;
}
