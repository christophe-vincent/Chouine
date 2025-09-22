#include <iostream>
#include <string.h>
#include "Joueur.h"
#include "Chouine.h"
#include "Carte.h"
#include "Algorithme.h"
#include "Annonce.h"

const int Joueur::MAX_CARDS = 5;

using namespace std;

Joueur::Joueur(Chouine& _chouine, int _niveau, int _id):
m_Chouine(_chouine), m_Algo(_niveau, *this), m_Niveau(_niveau)
{
    m_Id = _id;
    m_10Der = 0;
    m_PointsAnnonces = 0;
    m_LatestAnnonce = nullptr;
    m_Change7Trump = false;
    m_IsChouine = false;
    m_CarteJouee = nullptr;
}

Joueur::~Joueur()
{
}

int Joueur::points()
{
    return m_CartesGagnees.getPoints() + m_10Der + m_PointsAnnonces;
}


Carte *Joueur::carteMain(unsigned int _index)
{
    if (_index >= m_CartesMain.size())
    {
        return nullptr;
    }
    return m_CartesMain[_index];
}


Status Joueur::ajouterCarteMain(Carte &_carte)
{
    if (m_CartesMain.size() >= MAX_CARDS)
    {
        return Status::TOO_MANY_CARDS;
    }

    m_CartesMain.ajouter(&_carte);

    return Status::OK;
}


void Joueur::supprimerCarte(Carte *_carte)
{
    m_CartesMain.supprimer(_carte);
}


// cherche si la carte fait partie d'une annonce en main
Annonce* Joueur::rechercheAnnonce(Carte &_carte)
{
    if (_carte.valeur() == Carte::SEPT ||
        _carte.valeur() == Carte::HUIT ||
        _carte.valeur() == Carte::NEUF)
    {
        // les 7,8,9 ne font jamais partie d'une annonce
        return nullptr;
    }

    Annonce* plusForteAnnonce = nullptr;
    for (const auto& pair : _carte.annonces())
    {
        Annonce* annonce = pair.first;
        int score = pair.second;
        if (score == 100)
        {
            // l'annonce est complète
            if (annonce > plusForteAnnonce)
            {
                plusForteAnnonce = annonce;
            }
        }
    }
    return plusForteAnnonce;
}


bool Joueur::prendreCarteAtout(Carte *_newCarte)
{
    int index = -1;
    Carte *card = nullptr;

    card = m_CartesMain.getCarteFromList(m_Chouine.couleurAtout(), Carte::SEPT);

    if (card != nullptr)
    {
        // ok, on a le 7 d'atout
        m_CartesMain.supprimer(card);
        
        ajouterCarteMain(*_newCarte);
        return true;
    }

    return false;
}


Carte* Joueur::choisirCarte(Carte *_carteAdversaire, string& _annonce)
{
    // recalculer les annonces
    if (m_Niveau >= Algorithme::VERIFIE_ANNONCE)
    {
        m_Annonces.clear();
        for (Carte* carteMain: m_CartesMain.cartes())
        {
            carteMain->annonces().clear();
        }
        for (auto* annonce: m_Chouine.getAnnonces()) 
        {
            // le score indique si une annonce est présente, partiellement presente ou absente de la main
            int score = annonce->calculeScore(m_CartesMain, m_CartesJouees);
            if (score > 0)
            {
                // associons cette annonce au joueur
                m_Annonces[annonce] = score;
                //cout << "Annonce dans la main: " << annonce->to_string() << " Score=" << score << endl;

                // ajoutons l'annonce et son score à chaque carte concernée
                for (Carte* carteMain: m_CartesMain.cartes())
                {
                    //cout << "annonce: " << annonce->to_string() << " carte: " << carteMain->carteToStr();
                    //cout << " Present ? " << annonce->carteDansAnnonce(*carteMain) << endl;
                    if (annonce->carteDansAnnonce(*carteMain))
                    {
                        // ajoutons cette annonce dans la carte
                        carteMain->annonces()[annonce] = score;
                    }
                }
            }
        }
    }

    // for (Carte* carteMain: m_CartesMain.cartes())
    // {
    //     if (carteMain->annonces().size() > 0)
    //     {
    //         cout << carteMain->carteToStr() + " -> " << carteMain->annonceToStr() << endl;
    //     }
    // }

    m_CarteJouee = nullptr;
    if (m_Chouine.piocheVide())
    {
        // on doit donner la bonne couleur
        m_CarteJouee = m_Algo.choisirCartePiocheVide(_carteAdversaire);
    }
    else
    {
        m_CarteJouee = m_Algo.choisirCarte(_carteAdversaire);
    }

    _annonce = "";
    if (m_Niveau >= Algorithme::VERIFIE_ANNONCE)
    {
        // vérifier si la carte jouée fait partie d'une annonce
        Annonce* annonce = rechercheAnnonce(*m_CarteJouee);
        if (annonce != nullptr)
        {
            _annonce = annonce->to_string();
            m_PointsAnnonces += annonce->points();
        }
    }
    return m_CarteJouee;
}


void Joueur::finPli(bool _gagnant, Carte& _carteAdversaire)
{
    if (_gagnant)
    {
        m_CartesGagnees.ajouter(&_carteAdversaire);
        m_CartesGagnees.ajouter(m_CarteJouee);
    }
    if (_carteAdversaire.getPoints() > 0 && !m_CartesJouees.rechercheCarte(_carteAdversaire))
    {
        m_CartesJouees.ajouter(&_carteAdversaire);
    }
    if (m_CarteJouee->getPoints() > 0)
    {
        m_CartesJouees.ajouter(m_CarteJouee);
    }
    supprimerCarte(m_CarteJouee);
    m_CarteJouee = nullptr;
    Carte* pioche = m_Chouine.pioche().piocheCarte();
    if (pioche != nullptr)
    {
        ajouterCarteMain(*pioche);
    }
}



Carte *Joueur::EmptyPickSimulation(Carte &_userChoice)
{
    Carte *playCarte;
    ListeCartes couleurList;

    // we must play a higher card
    m_CartesMain.getCouleurSubset(_userChoice.couleur(), couleurList);

    playCarte = couleurList.getHigherCarte(_userChoice);
    if (playCarte == nullptr)
    {
        // play trump if user card is not trump...
        if (!_userChoice.atout())
        {
            ListeCartes trumpList;
            m_CartesMain.getCouleurSubset(m_Chouine.couleurAtout(), trumpList);
            playCarte = couleurList.plusFaible();
        }
    }

    return playCarte;
}

// check if the Joueur is allowed to play this card or not
// it is only done when pick is empty : user must play higher card
bool Joueur::carteAurotisee(Carte &_card, Carte &_otherCarte)
{
    if (m_CartesMain.size() == 1)
    {
        // last card...
        return true;
    }

    if (_otherCarte.gagnante(_card))
    {
        // played card is better than other Joueur : it is allowed
        return true;
    }

    Carte *card = m_CartesMain.getHigherCarte(_otherCarte);
    if (card == nullptr)
    {
        // the Joueur does not have higher card, so he can play nay card
        return true;
    }
    else
    {
        return false;
    }
}


// retourne la main du joueur en string
string Joueur::cartesMainToStr()
{
    return m_CartesMain.nomCartes();
}

// carte gagnees
string Joueur::cartesGagneesToStr()
{
    return m_CartesGagnees.nomCartes();
}