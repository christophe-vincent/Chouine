#include "Algorithme.h"
#include "Joueur.h"

Carte* Algorithme::choisirCarte()
{
    if (m_Niveau == 0)
    {
        return m_Joueur.cartes().plusFaible();
    }
    return nullptr;
}

Carte* Algorithme::choisirCarte(Carte &_choixAdversaire)
{
    if (m_Niveau == 0)
    {
        return m_Joueur.cartes().plusFaible();
    }
    return nullptr;
}

Carte* Algorithme::choisirCartePiocheVide()
{
    return nullptr;
}

Carte* Algorithme::choisirCartePiocheVide(Carte &_choixAdversaire)
{
    return nullptr;
}
    