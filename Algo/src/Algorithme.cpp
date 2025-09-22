#include <iostream>
#include <random>
#include "Algorithme.h"
#include "Joueur.h"
#include "Carte.h"
#include "Annonce.h"

using namespace std;
static std::random_device rd;
static std::mt19937 mt(rd());
static std::uniform_int_distribution<int> dist(0, 4);


Carte* Algorithme::choisirCarte()
{
    if (m_Niveau == NIVEAUX::RANDOM)
    {
        return m_Joueur.carteMain(dist(mt) % m_Joueur.quantiteCartesMain());
    }
    if (m_Niveau >= NIVEAUX::PROBA_ANNONCE)
    {
        return calculScores();
    }
    return m_Joueur.cartes().plusFaible();
}

Carte* Algorithme::calculScores()
{
    Carte* ret = nullptr;
    int plusPetitScore = 99999;
    ListeCartes& cartesMain = m_Joueur.cartes();
    for (Carte* carteMain: cartesMain.cartes())
    {
        int score = 0;
        // pour les cartes yan des points
        score = 10 * carteMain->getPoints();
        for (auto& annonce: carteMain->annonces())
        {
            score += int(2 * 10 * annonce.first->points() * (annonce.second/100.0));
        }
        if (carteMain->atout())
        {
            // si +10 alors +52.8% de gain par rapport à niveau 3
            // +5 => +52.6%
            // +50 => 52.5%
            // +100 => 52.9%
            // +110 => 53.3%
            // +150 => 53.0%
            score += 110;
        }
        if (score < plusPetitScore)
        {
            plusPetitScore = score;
            ret = carteMain;
        }
        carteMain->score(score);
    }
    return ret;
}

///////////////////////////////
Carte* Algorithme::choisirCarte(Carte *_choixAdversaire)
{
    Carte *choix = nullptr;
    // premier à jouer
    if (_choixAdversaire == nullptr)
    {
        choix = choisirCarte();
    } else
    {
        // l'autre joueur a deja joué une carte
        m_Joueur.cartesJouees().ajouter(_choixAdversaire);
        if (m_Niveau >= NIVEAUX::PRISE_BRISQUE)
        {
            if (_choixAdversaire->brisque())
            {
                choix = m_Joueur.cartes().choisirPlusForte(_choixAdversaire);
            }
        }
        if (choix == nullptr)
        {
            choix = choisirCarte();
        }
    }

    return choix;
}

///////////////////////////////
Carte* Algorithme::choisirCartePiocheVide(Carte *_choixAdversaire)
{
    if (_choixAdversaire == nullptr)
    {
        return choisirCarte();
    }

    Carte* carte = m_Joueur.cartes().choisirPlusForte(_choixAdversaire);
    return carte;
}
    