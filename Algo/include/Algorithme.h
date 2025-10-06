#ifndef _ALGORITHME_H_
#define _ALGORITHME_H_

class Joueur;
class Carte;

class Algorithme
{
public:
enum NIVEAUX
    {
        RANDOM            = 0,  // aléatoire
        PRISE_BRISQUE     = 1,  // Prend les brisques de l'adversaire
        CARTE_PLUS_FAIBLE = 2,  // Carte la plus faible
        VERIFIE_ANNONCE   = 3,  // vérifie si une annonce doit être faite avant de jouer une carte
        ECHANGE_7_ATOUT   = 4,  // prendre la carte d'atout avec le 7 d'atout
        PROBA_ANNONCE     = 5   // mémoriser les cartes jouées pour faire des annonces
    };

    Algorithme(int _niveau, Joueur& _joueur):
     m_Niveau(_niveau), m_Joueur(_joueur) {};

    Carte* choisirCarte();

    // Calcule le score de chaque carte en fonction des annonces possibles et retourne la carte avec le score le plus bas
    Carte* calculScores();
    Carte* choisirCarte(Carte *_choixAdversaire);
    Carte* choisirCartePiocheVide(Carte *_choixAdversaire);
    

private:
    int m_Niveau;
    Joueur &m_Joueur;

};

#endif