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
        INCONNU = 0,
        MARIAGE = 20,
        TIERCE = 30,
        QUARANTE = 40,
        QUINTE = 50,
        CHOUINE = 1000
    };

    static const Carte::Valeur CARTES_MARIAGE[];
    static const Carte::Valeur CARTES_TIERCE[];
    static const Carte::Valeur CARTES_QUARANTE[];
    static const Carte::Valeur CARTES_CHOUINE[];
    static const set<TypeAnnonce> ANNONCES;

    Annonce(Carte::Couleur _couleur,
             TypeAnnonce _type,
             bool _atout,
             CarteList &_cartes);
    ~Annonce();

    void ajouterCarte(Carte &_carte);
    void supprimerCarte(Carte &_carte);

    static bool mariage(CarteList &_list);
    static bool tierce(CarteList &_list);
    static bool quarante(CarteList &_list);
    static bool quinte(CarteList &_list);
    static bool chouine(CarteList &_list);
    static set<Annonce *> annonces(CarteList &_list,
                                   set<Annonce *> &_annoncesConnues,
                                   bool *_isChouine);

    Carte::Couleur couleur() { return m_Couleur; }
    TypeAnnonce type() { return m_Annonce; }

private:
    //void computeScore();

    CarteList m_Cartes;
    Carte::Couleur m_Couleur;
    TypeAnnonce m_Annonce;
    int m_Points;
    bool m_IsTrump;
    float m_Score;
};

#endif