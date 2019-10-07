#include "Algorithme.h"
#include "Joueur.h"

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
        return m_Joueur.cartes().plusFaible();
    }
    return nullptr;
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
    