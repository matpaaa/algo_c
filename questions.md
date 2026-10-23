# Partie 1 — Modélisation


## Question 1 — Variables de décision

**Quelles sont les variables de décision du problème ? Combien y en a-t-il et quelles valeurs peuvent-elles prendre ?**

Il y a 6 variables de décision, une par publication, notées x₁ à x₆ :

- x₁ : Vidéo courte
- x₂ : Meme
- x₃ : Story
- x₄ : Interview
- x₅ : Sondage
- x₆ : Tutoriel

Chaque variable est **binaire** : xᵢ ∈ {0, 1}.

- xᵢ = 1 : la publication est produite
- xᵢ = 0 : la publication n'est pas produite

---

## Question 2 — Fonction objectif

**Écrivez la fonction objectif sous forme mathématique.**

 On cherche à maximiser l'engagement total :

Z = 80·x₁ + 35·x₂ + 25·x₃ + 100·x₄ + 30·x₅ + 70·x₆

Autrement dit : max Z = Σ (engagementᵢ · xᵢ), pour i de 1 à 6.

---

## Question 3 — Contrainte de temps

**Écrivez la contrainte de temps.**

**Réponse.** Le temps total des publications choisies ne doit pas dépasser 6 heures :

2·x₁ + 1·x₂ + 1·x₃ + 3·x₄ + 1·x₅ + 2·x₆ ≤ 6

avec xᵢ ∈ {0, 1} pour tout i.

---

## Question 4 — Nombre de sélections

**Combien de sélections différentes peut-on former ? Combien d'entre elles sont réalisables ?**


- Chaque publication est prise ou non : 2 choix par publication, donc **2⁶ = 64 sélections** possibles.
- Parmi elles, **47 sélections sont réalisables** (temps ≤ 6 h), vérifié par calcul. Les 17 autres dépassent 6 h.
- La sélection vide fait partie des 47 : elle est réalisable, mais son engagement est nul.

---

## Question 5 — Une sélection « à la main »

Proposez à la main une sélection qui vous paraît bonne. Quel engagement obtenez-vous ? Êtes-vous certain(e) qu'il n'existe pas mieux ?

Sélection proposée : **Vidéo courte + Interview + Meme**.

- Temps : 2 + 3 + 1 = 6 h (contrainte respectée)
- Engagement : 80 + 100 + 35 = **215**

Cette sélection paraît bonne, mais son score seul ne permet pas de prouver qu'il n'existe pas mieux. L'exploration exhaustive de la partie 5 confirme ensuite qu'elle est optimale.

---

# Partie 5 — Exploration exhaustive

## Question 1 — Nombre de sélections examinées

Combien de sélections complètes votre fonction examine-t-elle ? Ajoutez un compteur pour le vérifier.

La fonction examine **2⁶ = 64 sélections complètes**, car chaque publication peut être prise ou non. Elle examine aussi les sélections qui dépassent la capacité, puis les rejette au moment du contrôle de faisabilité.

Le compteur est déjà présent dans le code : le champ `nb_examinees` de la structure `Solution` est initialisé à zéro, puis incrémenté à chaque feuille de l'arbre, lorsque `i == p->nb` :

```c
meilleure->nb_examinees++;
```

Lorsqu'une meilleure solution est copiée, le compteur est sauvegardé puis restauré pour ne pas être écrasé par la copie de `courante`. À la fin, le programme affiche `Selections examinees : 64`.

## Question 2 — Solutions optimales

Quel est l'engagement maximal ? Quelle(s) sélection(s) l'atteignent ? La solution optimale est-elle unique ?

Pour une capacité de **6 heures**, l'engagement maximal est de **215 points**. Deux sélections l'atteignent :

| Publications | Temps total | Engagement total |
| --- | --- | --- |
| Vidéo courte + Meme + Interview | 2 + 1 + 3 = 6 h | 80 + 35 + 100 = 215 |
| Vidéo courte + Meme + Sondage + Tutoriel | 2 + 1 + 1 + 2 = 6 h | 80 + 35 + 30 + 70 = 215 |

La solution optimale **n'est donc pas unique**. La fonction `explorer` conserve la première solution optimale rencontrée, car elle ne remplace la meilleure solution que si le score est strictement supérieur. La fonction bonus `afficher_optimales` retrouve ensuite les deux sélections.

## Question 3 — Passage par adresse

Pourquoi passe-t-on `courante` et `meilleure` par adresse plutôt que par valeur ?

Le passage par adresse permet aux appels récursifs de travailler sur les mêmes structures. Les changements de choix dans `courante` et la mise à jour de `meilleure` restent ainsi accessibles aux autres appels et à la fonction appelante.

Avec un passage par valeur, chaque appel recevrait une copie : modifier `meilleure` ne mettrait pas à jour la meilleure solution de l'appelant. Il faudrait alors retourner et récupérer explicitement le résultat. Le passage par adresse évite aussi de recopier les structures à chaque appel.

Comme `courante` est partagée, le code remet `courante->choix[i]` à zéro avant d'explorer la branche où la publication n'est pas retenue.

---

# Partie 6 — Analyse

## Question 1 — Comparaison des méthodes

Comparez les heuristiques gloutonnes à la solution exhaustive. Lesquelles trouvent l'optimum ?

Pour la capacité initiale de **6 heures**, les résultats sont :

| Méthode | Publications retenues | Temps | Engagement | Optimum atteint |
| --- | --- | --- | --- | --- |
| Glouton A, engagement décroissant | Vidéo courte + Meme + Interview | 6 h | 215 | Oui |
| Glouton B, temps croissant | Vidéo courte + Meme + Story + Sondage | 5 h | 170 | Non |
| Glouton C, ratio décroissant | Vidéo courte + Meme + Sondage + Tutoriel | 6 h | 215 | Oui |
| Exploration exhaustive | Vidéo courte + Meme + Interview | 6 h | 215 | Oui, garanti |

Les gloutons **A et C trouvent l'optimum**, avec deux sélections différentes. Le glouton B obtient 45 points de moins. Il reste une heure disponible, mais aucune publication restante ne tient dans cette durée. L'exploration exhaustive garantit l'optimum en comparant toutes les sélections réalisables.

## Question 2 — Autres capacités

Relancez votre programme avec une capacité de 4 heures, puis de 7 heures. Les heuristiques trouvent-elles toujours l'optimum ? Qu'en concluez-vous ?

Les exécutions avec `./campagne 4` et `./campagne 7` donnent les engagements suivants :

| Capacité | Glouton A | Glouton B | Glouton C | Optimum exhaustif |
| --- | --- | --- | --- | --- |
| 4 h | 135 | 90 | 145 | 150 |
| 7 h | 250 | 240 | 240 | 250 |

**Avec 4 heures**, l'unique solution optimale est **Vidéo courte + Tutoriel**, soit 4 h et 150 points. A choisit Interview + Meme, B choisit Meme + Story + Sondage et C choisit Vidéo courte + Meme + Sondage. Aucune heuristique ne trouve l'optimum.

Pour C, Meme et Tutoriel ont le même ratio de 35 points par heure. L'ordre initial départage cette égalité en faveur de Meme. Après avoir choisi Vidéo courte puis Meme, il ne reste qu'une heure : le Tutoriel ne peut plus être ajouté. Cela montre qu'un choix local peut empêcher une meilleure combinaison globale.

**Avec 7 heures**, l'unique solution optimale est **Vidéo courte + Interview + Tutoriel**, soit 7 h et 250 points. Seul A la trouve. B et C choisissent Vidéo courte + Meme + Story + Sondage + Tutoriel, pour 7 h et 240 points.

Les heuristiques ne trouvent donc **pas toujours l'optimum**. Leur efficacité dépend des données, de la capacité et parfois du départage des égalités. Elles donnent rapidement une solution réalisable, mais leurs décisions définitives peuvent empêcher d'obtenir la meilleure combinaison. Même utiliser tout le temps disponible ne garantit pas le meilleur score.

## Question 3 — Passage à 60 publications

Si l'agence proposait 60 types de publications, quelle méthode utiliseriez-vous ? Pourquoi ?

Avec 60 publications, une exploration exhaustive examinerait **2⁶⁰**, soit environ **1,15 × 10¹⁸ sélections**. Cette croissance rend le parcours de toutes les combinaisons impraticable.

Pour des temps entiers et une capacité modérée, nous utiliserions la **programmation dynamique**. Un tableau `meilleur[i][t]` donnerait le meilleur engagement obtenu avec les `i` premières publications et au plus `t` heures. Pour chaque publication, on comparerait le résultat sans la prendre avec celui obtenu en la prenant, si elle tient dans le temps disponible.

Cette méthode garantit l'optimum avec une complexité en **O(n × C)**, où `n` est le nombre de publications et `C` la capacité entière. Pour 60 publications et 6 heures, cela représente un tableau de 61 × 7 cases, bien plus petit que l'ensemble des combinaisons. Si la capacité devenait très grande, une heuristique pourrait être préférable pour obtenir rapidement une solution approchée.

Le programme actuel devrait être adapté : `MAX_PUBS` vaut 20 et l'énumération par masques de `afficher_optimales` ne convient pas telle quelle à 60 publications. La programmation dynamique est une proposition d'évolution, elle n'est pas encore implémentée.
