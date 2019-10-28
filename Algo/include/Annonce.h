#ifndef _ANNONCE_H_
#define _ANNONCE_H_

#include <set>
#include <vector>
#include "Carte.h"
#include "ListeCartes.h"
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
    
    static const Carte::Valeur CARTES_MARIAGE[];
    static const Carte::Valeur CARTES_TIERCE[];
    static const Carte::Valeur CARTES_QUARANTE[];
    static const Carte::Valeur CARTES_CHOUINE[];
    static const TypeAnnonce ANNONCES[];
    static const set<int> POINTS_ANNONCES;

    Annonce(Carte::Couleur _couleur, 
            TypeAnnonce _type, 
            bool _atout, 
            ListeCartes &_cartes) : 
            m_Couleur(_couleur), 
            m_Annonce(_type), 
            m_Atout(_atout), 
            m_Cartes(_cartes)
    {
    }

    Annonce(Carte::Couleur _couleur,
            TypeAnnonce _type,
            bool _atout) : 
            m_Couleur(_couleur),
            m_Annonce(_type),
            m_Atout(_atout) {}
    Annonce(TypeAnnonce _type):
            m_Couleur(Carte::UNDEF_COLOR),
            m_Annonce(_type),
            m_Atout(false) {}
    ~Annonce();

    void ajouterCarte(Carte &_carte);
    void supprimerCarte(Carte *_carte);

    static bool mariage(ListeCartes &_list);
    static bool tierce(ListeCartes &_list);
    static bool quarante(ListeCartes &_list);
    static bool quinte(ListeCartes &_list);
    static bool chouine(ListeCartes &_list);

    Carte::Couleur couleur() { return m_Couleur; }
    TypeAnnonce type() { return m_Annonce; }
    unsigned int cartesManquantes();

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