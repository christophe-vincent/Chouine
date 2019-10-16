#include <iostream>
#include "Algorithme.h"
#include "Joueur.h"

using namespace std;

Carte* Algorithme::choisirCarte()
{
    return m_Joueur.cartes().plusFaible();
}

Carte* Algorithme::choisirCarte(Carte *_choixAdversaire)
{
    Carte *choix = nullptr;
    if (_choixAdversaire == nullptr)
    {
        return choisirCarte();
    }
    if (m_Niveau > 0)
    {
        if (_choixAdversaire->brisque())
        {
            choix = m_Joueur.cartes().choisirPlusForte(_choixAdversaire);
        }
    }
    if (choix == nullptr)
    {
        choix = m_Joueur.cartes().plusFaible();
    }
    return choix;
}

Carte* Algorithme::choisirCartePiocheVide()
{
    return m_Joueur.cartes().plusFaible();
}

Carte* Algorithme::choisirCartePiocheVide(Carte *_choixAdversaire)
{
    if (_choixAdversaire == nullptr)
    {
        return choisirCartePiocheVide();
    }

    Carte* carte = m_Joueur.cartes().choisirPlusForte(_choixAdversaire);
    return carte;
}
    