#ifndef _ANNONCE_H_
#define _ANNONCE_H_

#include <set>
#include <vector>
#include <string>
#include "ListeCartes.h"
#include "Carte.h"
using namespace std;


class Annonce
{
public:
    enum TypeAnnonce
    {
        MARIAGE  = 0,
        TIERCE   = 1,
        QUARANTE = 2,
        QUINTE   = 3,
        CHOUINE  = 4 
    };
    
    static const std::array<Carte::Valeur, 2> CARTES_MARIAGE;
    static const std::array<Carte::Valeur, 3> CARTES_TIERCE;
    static const std::array<Carte::Valeur, 4> CARTES_QUARANTE;
    static const std::array<Carte::Valeur, 5> CARTES_CHOUINE;
    static const TypeAnnonce ANNONCES[];
    static const set<int> POINTS_ANNONCES;

    Annonce(Carte::Couleur _couleur,
            TypeAnnonce _type,
            bool _atout);

    ~Annonce();

    // retourne ne nombre de points de l'annonce
    int points() { return m_Points;}

    bool operator > (Annonce & _annonce)
    {
         return this->m_Points < _annonce.points();
    }

    // calcule le pourcetage de cartes données présentes dans l'annonce
    // 50 pour un mariage signifie qu'il n'y a par exemple qu'un roi ou une dame
    int calculeScore(ListeCartes &_cartesMain, ListeCartes &_cartesJouees);

    void ajouterCarte(Carte &_carte);
    void supprimerCarte(Carte *_carte);
    bool carteDansAnnonce(Carte &_carte);
    vector<Carte*>& cartes() { return m_Cartes.cartes(); }

    static bool mariage(ListeCartes &_list);
    static bool tierce(ListeCartes &_list);
    static bool quarante(ListeCartes &_list);
    static bool quinte(ListeCartes &_list);
    static bool chouine(ListeCartes &_list);

    Carte::Couleur couleur() { return m_Couleur; }
    TypeAnnonce type() { return m_Annonce; }
    unsigned int cartesManquantes();

    string to_string();

private:
    //void computeScore();
    ListeCartes m_Cartes;
    Carte::Couleur m_Couleur;
    TypeAnnonce m_Annonce;
    int m_Points;
    bool m_Atout;
    float m_Score;
};

#endif