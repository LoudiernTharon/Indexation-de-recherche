# TP4 - Indexation de Texte par ABR

> **NF16 - Algorithmique et Structures de Données**  
> Université de Technologie de Compiègne (UTC)

## Description

Implémentation d'un système d'indexation de texte utilisant un **Arbre Binaire de Recherche (ABR)**. Le programme permet d'indexer un fichier texte, de rechercher des mots, et de reconstruire le texte original à partir de l'index.

## Structure du Projet

```
TP4/
├── src/
│   ├── tp4.h          # Structures et prototypes
│   └── tp4.c          # Implémentation des fonctions
├── textes/            # Fichiers de test
│   ├── test.txt       # Test basique
│   ├── albatroz.txt   # Poème (L'Albatros de Baudelaire)
│   ├── simple.txt     # 2 phrases simples
│   ├── repetitions.txt # Mots répétés
│   ├── multilignes.txt # Phrases sur plusieurs lignes
│   ├── long_phrase.txt # Phrase longue (30+ mots)
│   ├── majuscules.txt  # Casse mixte
│   ├── ponctuation.txt # Ponctuation variée
│   └── mot_unique.txt  # 1 seul mot
├── main.c             # Programme principal (menu)
└── README.md
```

## Compilation

```bash
gcc -I src src/tp4.c main.c -o tp4 -Wall -Wextra
```

## Utilisation

```bash
./tp4
```

### Menu Principal

| Option | Action |
|--------|--------|
| 1 | Charger un fichier texte |
| 2 | Afficher les caractéristiques (nb mots distincts/total) |
| 3 | Afficher l'index complet (ordre alphabétique) |
| 4 | Rechercher un mot dans l'index |
| 5 | Afficher les phrases contenant un mot |
| 6 | Reconstruire le texte dans un fichier |
| 7 | Quitter |

## Tests Rapides

```bash
# Test basique
./tp4
> 1
> textes/test.txt
> 2
> 7

# Test avec poème
./tp4
> 1
> textes/albatroz.txt
> 4
> albatros
> 7
```

## Fonctions Implémentées

| Question | Fonction | Complexité |
|----------|----------|------------|
| B.1 | `ajouterPosition` | O(n) |
| B.2 | `ajouterOccurence` | O(h) |
| B.3 | `indexerFichier` | O(m × h) |
| B.4 | `afficherIndex` | O(n) |
| B.5 | `rechercherMot` | O(h) |
| B.6 | `afficherOccurencesMot` | O(n × p) |
| B.7 | `construireTexte` | O(p × n) |

*h = hauteur de l'arbre, n = nombre de mots, m = mots du fichier, p = nombre de phrases*

## Auteur

Tharon Loudiern - UTC A25
