#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include "ListeCartes.h"
#include "Annonce.h"

using namespace std;

CarteList::CarteList() : m_isTrumpSeven(false)
{
}

CarteList::CarteList(Carte *_card) : m_isTrumpSeven(false)
{
    m_Cartes.push_back(_card);
}

void CarteList::ajouter(Carte *_card)
{
    if (_card == nullptr) return;
    //cout << _card->nom() << endl;
    if ((_card->atout()) && (_card->getValeur() == Carte::SEPT))
    {
        m_isTrumpSeven = true;
    }
    m_Cartes.push_back(_card);
}

void CarteList::supprimer(Carte *_card)
{
    if (_card == nullptr) return;
    auto it = find(m_Cartes.begin(), m_Cartes.end(), _card);
    if (it != m_Cartes.end())
    {
        m_Cartes.erase(it);
        if ((_card->atout()) && (_card->getValeur() == Carte::SEPT))
        {
            m_isTrumpSeven = false;
        }
    }
}

void CarteList::shuffle()
{
    auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 mt(seed);
    
    std::shuffle(m_Cartes.begin(), m_Cartes.end(), mt);
}

// return first card and remove it from list (like we we pick a card)
Carte *CarteList::piocheCarte()
{
    if (m_Cartes.size() == 0)
    {
        return nullptr;
    }
    Carte *ret = m_Cartes.back();
    m_Cartes.pop_back();
    return ret;
}

Carte *CarteList::searchCarte(Carte::Valeur _value)
{
    Carte *ret = nullptr;

    auto it = m_Cartes.begin();
    while ((it != m_Cartes.end()) && (ret == nullptr))
    {
        if ((*it)->getValeur() == _value)
        {
            ret = *it;
        }
        it++;
    }
    return ret;
}

void CarteList::getTrumps(CarteList &_list)
{
    auto it = m_Cartes.begin();
    while (it != m_Cartes.end())
    {
        if ((*it)->atout())
        {
            _list.ajouter(*it);
        }
        it++;
    }
}

Carte *CarteList::plusFaible()
{
    Carte *plusFaible;
    auto it = m_Cartes.begin();
    plusFaible = *it;
    while (it != m_Cartes.end())
    {
        if ((*it)->getPoints() < plusFaible->getPoints())
        {
            plusFaible = *it;
        }
        it++;
    }
    return plusFaible;
}


Carte* CarteList::choisirPlusForte(Carte* _carte)
{
    Carte* carte = nullptr; 

    for (auto it=m_Cartes.begin(); it!=m_Cartes.end(); ++it)
    {
        if (_carte->compare(**it))
        {
            if (carte == nullptr)
            {
                carte = *it;
            }
            else
            {
                if ((*it)->compare(*carte))
                {
                    // la carte choisie precedement est plus forte
                    carte = *it;
                }
            }            
        }
    }
    if (carte == nullptr)
    {
        carte = plusFaible();
    }
    return carte;
}


void CarteList::getCouleurSubset(Carte::Couleur _couleur, CarteList &_list)
{
    auto it = m_Cartes.begin();
    while (it != m_Cartes.end())
    {
        if ((*it)->couleur() == _couleur)
        {
            _list.ajouter(*it);
        }
        it++;
    }
}

bool CarteList::isContainsValeur(Carte::Valeur _value)
{
    bool ret = false;

    for (auto it = m_Cartes.begin(); it != m_Cartes.end(); it++)
    {
        if ((*it)->getValeur() == _value)
        {
            ret = true;
            break;
        }
    }

    return ret;
}

bool CarteList::isContainsCarte(Carte &_value)
{
    bool ret = false;

    for (auto it = m_Cartes.begin(); it != m_Cartes.end(); it++)
    {
        if ((**it) == _value)
        {
            ret = true;
            break;
        }
    }

    return ret;
}

CarteList CarteList::getCartesFromList(Carte::Valeur _value)
{
    CarteList ret;

    for (auto it = m_Cartes.begin(); it != m_Cartes.end(); it++)
    {
        if ((*it)->getValeur() == _value)
        {
            ret.ajouter(*it);
        }
    }
    return ret;
}

CarteList CarteList::getCartesFromList(Carte::Couleur _couleur)
{
    CarteList ret;

    for (auto it = m_Cartes.begin(); it != m_Cartes.end(); it++)
    {
        if ((*it)->couleur() == _couleur)
        {
            ret.ajouter(*it);
        }
    }
    return ret;
}

Carte *CarteList::getCarteFromList(Carte::Couleur _couleur, Carte::Valeur _value)
{
    Carte *ret = nullptr;

    for (auto it = m_Cartes.begin(); it != m_Cartes.end(); it++)
    {
        if (((*it)->getValeur() == _value) && ((*it)->couleur() == _couleur))
        {
            ret = *it;
        }
    }

    return ret;
}

int CarteList::NumberOfValeur(Carte::Valeur _value)
{
    int ret = 0;

    for (auto it = m_Cartes.begin(); it != m_Cartes.end(); it++)
    {
        if ((*it)->getValeur() == _value)
        {
            ret++;
        }
    }

    return ret;
}

Carte *CarteList::getHigherValeur(Carte::Valeur _value)
{
    Carte *card = nullptr;
    auto it = m_Cartes.begin();
    while ((it != m_Cartes.end()) && (_value > (*it)->getValeur()))
        it++;

    if ((it != m_Cartes.end()) && (_value > (*it)->getValeur()))
    {
        // we found the card !
        card = *it;
    }
    return card;
}

Carte *CarteList::getHigherCarte(Carte &_card)
{
    Carte *card = nullptr;
    auto it = m_Cartes.begin();
    while ((it != m_Cartes.end()) && (!(*it)->compare(_card)))
        it++;

    if ((it != m_Cartes.end()) && (!(*it)->compare(_card)))
    {
        // we found the card !
        card = *it;
    }
    return card;
}

int CarteList::getPoints()
{
    int points = 0;

    for (auto it : m_Cartes)
    {
        points += it->getPoints();
    }
    return points;
}

void CarteList::newCarteStatistics(Carte &_card)
{
    set<Annonce *> cardAnnonces;
    bool create = true;

    if ((_card.getValeur() == Carte::SEPT) || (_card.getValeur() == Carte::HUIT) || (_card.getValeur() == Carte::NEUF))
    {
        return; // these cards are not used in announces
    }

    // check all possible announces for this card

    for (auto it = Annonce::ANNONCES.begin(); it != Annonce::ANNONCES.end(); it++)
    {
        // search in the list if this announce has already statistics
        create = true;
        for (auto ann = m_AnnoncesStats.begin(); ann != m_AnnoncesStats.end(); ann++)
        {
            if (((*ann)->couleur() == _card.couleur()) && ((*ann)->type() == *it))
            {
                // great the annouce is already known
                (*ann)->ajouterCarte(_card);
                //TODO _card->addProbableAnnonce(ann);
                create = false;
            }
        }

        if (create)
        {
            // announce has not been created, create a new one
            CarteList cl = CarteList(&_card);
            Annonce *announce = new Annonce(
                _card.couleur(),
                *it,
                _card.atout(),
                cl);
            m_AnnoncesStats.push_back(announce);
            //TODO _card->addProbableAnnonce(announce);
        }
    }
}

void CarteList::removeCarteStatistics(Carte &_card)
{
    if ((_card.getValeur() == Carte::SEPT) || (_card.getValeur() == Carte::HUIT) || (_card.getValeur() == Carte::NEUF))
    {
        return; // these cards are not used in announces
    }

    // check all possible announces for this card
    for (auto it = Annonce::ANNONCES.begin(); it != Annonce::ANNONCES.end(); it++)
    {
        // search in the list if this announce has already statistics
        for (auto ann = m_AnnoncesStats.begin(); ann != m_AnnoncesStats.end(); ann++)
        {
            if (((*ann)->couleur() == _card.couleur()) && ((*ann)->type() == *it))
            {
                // great the annouce is already known
                (*ann)->supprimerCarte(_card);
                //TODO _card.removeProbableAnnonce(ann);
            }
        }
    }
}

// retourne la liste des cartes en chaine de caractères
std::string CarteList::nomCartes()
{
    string cartes;
    for (auto it = m_Cartes.begin(); it != m_Cartes.end(); it++)
    {
        cartes += (*it)->nom() + " ";
    }
    return cartes;
}