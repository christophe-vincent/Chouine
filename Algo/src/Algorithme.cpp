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
    if (m_Niveau >= NIVEAUX::PROBA_ANNONCE)
    {
        return calculScores();
    }
    if (m_Niveau >= NIVEAUX::CARTE_PLUS_FAIBLE)
    {
        return m_Joueur.cartes().plusFaible();
    }
    // m_Niveau == NIVEAUX::RANDOM
    return m_Joueur.carteMain(dist(mt) % m_Joueur.quantiteCartesMain());
}

Carte* Algorithme::calculScores()
{
    // Calcule les scores de chaque carte et retourne la carte ayant le plus petit score
    Carte* ret = nullptr;
    int plusPetitScore = 99999;
    ListeCartes& cartesMain = m_Joueur.cartes();
    for (Carte* carteMain: cartesMain.cartes())
    {
        int score = 0;
        // pour les cartes ayant des points
        score = carteMain->getPoints();
        float scoreAnnonces = 0;
        for (auto& annonce: carteMain->annonces())
        {
            // calcule un score pour chaque annonce possible associée à la carte
            int sa = int(0.1 * annonce.first->points() * (annonce.second/100.0));
            if (sa > scoreAnnonces)
            {
                // on prend le plus grand score
                scoreAnnonces = sa;
            }
        }
        score += scoreAnnonces;
        if (carteMain->atout())
        {
            score += 10 + carteMain->getPoints();
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
    