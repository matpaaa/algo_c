/*
 * campagne.c - Mini-projet B2 : Recherche operationnelle et reseaux sociaux
 * Probleme : choisir des publications (sac a dos 0/1) pour maximiser
 *            l'engagement sans depasser la capacite en heures.
 * Auteurs  : James / Mathy
 * Compilation : gcc -Wall -Wextra -std=c99 -o campagne campagne.c
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PUBS 20
#define TAILLE_NOM 32

typedef struct {
    char nom[TAILLE_NOM];
    int temps;
    int engagement;
    double ratio;
} Publication;

typedef struct {
    Publication *pubs;
    int nb;
    int capacite;
} Probleme;

typedef struct {
    int choix[MAX_PUBS];
    int temps_total;
    int engagement_total;
    long nb_examinees;   /* compteur de selections examinees (Partie 5) */
} Solution;

/* Alloue le probleme et son tableau de publications */

Probleme *creer_probleme(int capacite)
{
    Probleme *p = malloc(sizeof(Probleme));
    if (p == NULL) {
        return NULL;
    }
    p->pubs = malloc(MAX_PUBS * sizeof(Publication));
    if (p->pubs == NULL) {
        free(p);
        return NULL;
    }
    p->nb = 0;
    p->capacite = capacite;
    return p;
}


/* Libere le tableau, puis la structure */

void liberer_probleme(Probleme *p)
{
    if (p != NULL) {
        free(p->pubs);
        free(p);
    }
}


/* Ajoute une publication a la fin du tableau */

void ajouter_publication(Probleme *p, const char *nom, int temps, int engagement)
{
    if (p->nb >= MAX_PUBS) {
        printf("Erreur : trop de publications\n");
        return;
    }
    strncpy(p->pubs[p->nb].nom, nom, TAILLE_NOM - 1);
    p->pubs[p->nb].nom[TAILLE_NOM - 1] = '\0';
    p->pubs[p->nb].temps = temps;
    p->pubs[p->nb].engagement = engagement;
    p->pubs[p->nb].ratio = 0.0;
    p->nb++;
}


/* Ratio = engagement / temps (conversion en double sinon division entiere) */

void calculer_ratios(Probleme *p)
{
    int i;
    for (i = 0; i < p->nb; i++) {
        p->pubs[i].ratio = (double)p->pubs[i].engagement / p->pubs[i].temps;
    }
}


/* Affiche le tableau des publications */

void afficher_publications(const Probleme *p)
{
    int i;
    printf("\n%-3s %-14s %-6s %-11s %-8s\n", "N", "Publication", "Temps", "Engagement", "Ratio");
    for (i = 0; i < p->nb; i++) {
        printf("%-3d %-14s %-6d %-11d %-8.2f\n", i, p->pubs[i].nom, p->pubs[i].temps, p->pubs[i].engagement, p->pubs[i].ratio);
    }
    printf("Capacite : %d h\n", p->capacite);
}

/* Met tout a zero */
void initialiser_solution(Solution *s)
{
    int i;
    for (i = 0; i < MAX_PUBS; i++) {
        s->choix[i] = 0;
    }
    s->temps_total = 0;
    s->engagement_total = 0;
    s->nb_examinees = 0;
}

/* Calcule le temps total et l'engagement total de la selection */
void evaluer_solution(const Probleme *p, Solution *s)
{
    int i;
    s->temps_total = 0;
    s->engagement_total = 0;
    for (i = 0; i < p->nb; i++) {
        if (s->choix[i] == 1) {
            s->temps_total += p->pubs[i].temps;
            s->engagement_total += p->pubs[i].engagement;
        }
    }
}

/* Renvoie 1 si le temps total respecte la capacite, 0 sinon */
int est_realisable(const Probleme *p, const Solution *s)
{
    if (s->temps_total <= p->capacite) {
        return 1;
    }
    return 0;
}

/* Affiche une selection */
void afficher_solution(const Probleme *p, const Solution *s, const char *titre)
{
    int i;
    printf("\n--- %s ---\n", titre);
    printf("Publications : ");
    for (i = 0; i < p->nb; i++) {
        if (s->choix[i] == 1) {
            printf("%s ; ", p->pubs[i].nom);
        }
    }
    printf("\nTemps : %d h / %d h\n", s->temps_total, p->capacite);
    printf("Engagement : %d\n", s->engagement_total);
    if (est_realisable(p, s)) {
        printf("Selection realisable\n");
    } else {
        printf("Selection NON realisable\n");
    }
}

/* ---------- PARTIE 4 ---------- */

/* Comparaison pour qsort : ratio decroissant.
   a et b sont des adresses de cases du tableau de pointeurs */
int comparer_ratio(const void *a, const void *b)
{
    const Publication *pa = *(const Publication **)a;
    const Publication *pb = *(const Publication **)b;
    if (pb->ratio > pa->ratio) {
        return 1;
    }
    if (pb->ratio < pa->ratio) {
        return -1;
    }
    return (pa > pb) - (pa < pb);   /* egalite : on garde l'ordre d'origine */
}

/* Glouton A : engagement decroissant */
int comparer_engagement(const void *a, const void *b)
{
    const Publication *pa = *(const Publication **)a;
    const Publication *pb = *(const Publication **)b;
    return pb->engagement - pa->engagement;
}

/* Glouton B : temps croissant */
int comparer_temps(const void *a, const void *b)
{
    const Publication *pa = *(const Publication **)a;
    const Publication *pb = *(const Publication **)b;
    if (pa->temps != pb->temps) {
        return pa->temps - pb->temps;
    }
    return (pa > pb) - (pa < pb);   /* egalite : on garde l'ordre d'origine */
}

/* Glouton : on prend d'abord les meilleurs ratios engagement/heure */
void glouton(const Probleme *p, Solution *s, int (*comparer)(const void *, const void *))
{
    const Publication *ordre[MAX_PUBS];
    int i, k, temps_restant;

    /* On remplit le tableau de pointeurs (on ne touche pas a p->pubs) */
    for (i = 0; i < p->nb; i++) {
        ordre[i] = &p->pubs[i];
    }
    qsort(ordre, p->nb, sizeof(ordre[0]), comparer);

    initialiser_solution(s);
    temps_restant = p->capacite;
    for (k = 0; k < p->nb; k++) {
        if (ordre[k]->temps <= temps_restant) {
            i = ordre[k] - p->pubs;   /* indice d'origine */
            s->choix[i] = 1;
            temps_restant -= ordre[k]->temps;
        }
    }
    evaluer_solution(p, s);
}

/* ---------- PARTIE 5 ---------- */

/* Exploration exhaustive : pour chaque publication, on teste "prise" puis "non prise".
   courante et meilleure sont passees par adresse : les appels recursifs
   modifient les memes structures (sinon on travaillerait sur des copies). */
void explorer(const Probleme *p, int i, Solution *courante, Solution *meilleure)
{
    long compteur;

    if (i == p->nb) {                      /* selection complete */
        meilleure->nb_examinees++;
        evaluer_solution(p, courante);
        if (est_realisable(p, courante) &&
            courante->engagement_total > meilleure->engagement_total) {
            compteur = meilleure->nb_examinees;   /* on garde le compteur */
            *meilleure = *courante;               /* copie de la structure */
            meilleure->nb_examinees = compteur;
        }
        return;
    }
    courante->choix[i] = 1;                /* branche : je la prends */
    explorer(p, i + 1, courante, meilleure);
    courante->choix[i] = 0;                /* j'annule ce choix */
    explorer(p, i + 1, courante, meilleure);   /* branche : je ne la prends pas */
}

/* Bonus : affiche toutes les selections qui atteignent l'engagement maximal.
   Chaque nombre de 0 a 2^nb - 1 represente une selection (ses bits = les choix). */
void afficher_optimales(const Probleme *p, int engagement_max)
{
    int masque, i, nb_trouvees = 0;
    Solution s;

    for (masque = 0; masque < (1 << p->nb); masque++) {
        initialiser_solution(&s);
        for (i = 0; i < p->nb; i++) {
            s.choix[i] = (masque >> i) & 1;
        }
        evaluer_solution(p, &s);
        if (est_realisable(p, &s) && s.engagement_total == engagement_max) {
            nb_trouvees++;
            printf("\nOptimale n.%d", nb_trouvees);
            afficher_solution(p, &s, "Solution optimale");
        }
    }
    printf("\nNombre de solutions optimales : %d\n", nb_trouvees);
}

/* ---------- MAIN ---------- */

int main(int argc, char *argv[])
{
    int i, v, capacite = 6;
    Solution test, saisie, g, courante, meilleure;
    Probleme *p;

    if (argc > 1) {                        /* bonus : capacite en argument */
        capacite = atoi(argv[1]);
    }
    p = creer_probleme(capacite);
    if (p == NULL) {
        return 1;
    }
    ajouter_publication(p, "Video courte", 2, 80);
    ajouter_publication(p, "Meme", 1, 35);
    ajouter_publication(p, "Story", 1, 25);
    ajouter_publication(p, "Interview", 3, 100);
    ajouter_publication(p, "Sondage", 1, 30);
    ajouter_publication(p, "Tutoriel", 2, 70);
    calculer_ratios(p);
    afficher_publications(p);

    /* Test : Video courte + Interview + Sondage */
    initialiser_solution(&test);
    test.choix[0] = 1;
    test.choix[3] = 1;
    test.choix[4] = 1;
    evaluer_solution(p, &test);
    afficher_solution(p, &test, "Test Video + Interview + Sondage");

    /* Selection saisie par l'utilisateur */
    initialiser_solution(&saisie);
    printf("\nVotre selection (0 ou 1) :\n");
    for (i = 0; i < p->nb; i++) {
        printf("%s ? ", p->pubs[i].nom);
        v = 0;
        if (scanf("%d", &v) != 1) {
            v = 0;
        }
        saisie.choix[i] = (v == 1);   /* tout ce qui n'est pas 1 vaut 0 */
    }
    evaluer_solution(p, &saisie);
    afficher_solution(p, &saisie, "Votre selection");

    glouton(p, &g, comparer_engagement);
    afficher_solution(p, &g, "Glouton A (engagement)");
    glouton(p, &g, comparer_temps);
    afficher_solution(p, &g, "Glouton B (temps)");
    glouton(p, &g, comparer_ratio);
    afficher_solution(p, &g, "Glouton C (ratio)");

    /* Partie 5 : exploration exhaustive */
    initialiser_solution(&courante);
    initialiser_solution(&meilleure);
    explorer(p, 0, &courante, &meilleure);
    afficher_solution(p, &meilleure, "Exhaustif (optimum)");
    printf("Selections examinees : %ld\n", meilleure.nb_examinees);
    afficher_optimales(p, meilleure.engagement_total);

    liberer_probleme(p);
    return 0;
}
