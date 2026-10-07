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

2·x₁ + 1·x₂ + 1·x₃ + 3·x₄ + 1·x₅ + 2·x₆ ≤ 6**

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