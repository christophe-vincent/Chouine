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


Carte* Algorithme::choisirCarteAlgo(Carte *_choixAdversaire)
{
    if (m_Niveau >= NIVEAUX::PROBA_ANNONCE)
    {
        return calculScores(_choixAdversaire);
    }
    if (m_Niveau >= NIVEAUX::CARTE_PLUS_FAIBLE)
    {
        return m_Joueur.cartes().plusFaible();
    }
    // m_Niveau == NIVEAUX::RANDOM
    return m_Joueur.carteMain(dist(mt) % m_Joueur.quantiteCartesMain());
}

Carte* Algorithme::calculScores(Carte *_choixAdversaire)
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

        // introduire la carte adverse pour attribuer des points supplementaires si l'on peut conserver une carte à points
        if (_choixAdversaire != nullptr && m_Niveau >= NIVEAUX::SAUVER_BRISQUES && _choixAdversaire->gagnante(*carteMain))
        {
            // si la carte est à points, vérifions si elle peut être sauvée
            if (carteMain->getPoints() > 0 && !carteMain->atout())
            {
                // cette carte peut être sauvée
                score -= 2 * carteMain->getPoints();
                score -= 2 * _choixAdversaire->getPoints();
            }
        }
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
        choix = choisirCarteAlgo();
    } else
    {
        // l'autre joueur a deja joué une carte
        m_Joueur.cartesJouees().ajouter(_choixAdversaire);
        if (m_Niveau >= NIVEAUX::PRISE_BRISQUE)
        {
            if (_choixAdversaire->brisque())
            {
                choix = m_Joueur.cartes().choisirPlusForte(_choixAdversaire);
                if (choix->getPoints() > 10 && choix->atout() == true)
                {
                    // ne peut pas prendre la brisque avec l'as d'atout (gain tres faible voire nul)
                    choix = nullptr;
                } else if (choix->gagnante(*_choixAdversaire) == true)
                {
                    // on ne peut pas gagner, on joue la plus faible, gain pas évident
                    choix = choisirCarteAlgo(_choixAdversaire);
                }
            }
        }
        if (choix == nullptr)
        {
            choix = choisirCarteAlgo(_choixAdversaire);
        }
    }

    return choix;
}

///////////////////////////////
Carte* Algorithme::choisirCartePiocheVide(Carte *_choixAdversaire)
{
    if (_choixAdversaire == nullptr)
    {
        return choisirCarte(_choixAdversaire);
    }

    // choisir une carte de la même couleur si disponible
    ListeCartes& cartesMain = m_Joueur.cartes();
    Carte *choix = nullptr;
    Carte *choix_meme_couleur = nullptr;
    for (Carte* carteMain: cartesMain.cartes())
    {
        if (carteMain->couleur() == _choixAdversaire->couleur())
        {
            if (choix_meme_couleur == nullptr)
            {
                choix_meme_couleur = carteMain;
            } else
            {
                // on garde la carte de même couleur la plus faible
                if (carteMain->getPoints() < choix_meme_couleur->getPoints())
                {
                    choix_meme_couleur = carteMain;
                }
            }
            if ( carteMain->getPoints() > _choixAdversaire->getPoints())
            {
                // nous avons une carte de la même couleur et + forte
                if (choix == nullptr)
                {
                    choix = carteMain;
                } else
                {
                    // on choisi la carte ayant la + petite valeur
                    if (carteMain->getPoints() < choix->getPoints())
                    {
                        choix = carteMain;
                    }
                }
            }
        }
    }
    if (choix != nullptr)
    {
        return choix;
    }
    if (choix_meme_couleur != nullptr)
    {
        return choix_meme_couleur;
    }

    choix = m_Joueur.cartes().choisirPlusForte(_choixAdversaire);

    return choix;
}
    