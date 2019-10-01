#include <iostream>
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

void CarteList::add(Carte *_card)
{
    //cout << _card->nom() << endl;
    if ((_card->isTrump()) && (_card->getValeur() == Carte::SEPT))
    {
        m_isTrumpSeven = true;
    }
    m_Cartes.push_back(_card);
}

void CarteList::remove(Carte *_card)
{
    auto it = find(m_Cartes.begin(), m_Cartes.end(), _card);
    if (it != m_Cartes.end())
    {
        m_Cartes.erase(it);
        if ((_card->isTrump()) && (_card->getValeur() == Carte::SEPT))
        {
            m_isTrumpSeven = false;
        }
    }
}

void CarteList::shuffle()
{
    random_shuffle(m_Cartes.begin(), m_Cartes.end());
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
        if ((*it)->isTrump())
        {
            _list.add(*it);
        }
        it++;
    }
}

Carte *CarteList::getSmallest()
{
    Carte *smallest;
    auto it = m_Cartes.begin();
    smallest = *it;
    while (it != m_Cartes.end())
    {
        if ((*it)->getPoints() < smallest->getPoints())
        {
            smallest = *it;
        }
        it++;
    }
    return *it;
}

void CarteList::getCouleurSubset(Carte::Couleur _couleur, CarteList &_list)
{
    auto it = m_Cartes.begin();
    while (it != m_Cartes.end())
    {
        if ((*it)->getCouleur() == _couleur)
        {
            _list.add(*it);
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
            ret.add(*it);
        }
    }
    return ret;
}

CarteList CarteList::getCartesFromList(Carte::Couleur _couleur)
{
    CarteList ret;

    for (auto it = m_Cartes.begin(); it != m_Cartes.end(); it++)
    {
        if ((*it)->getCouleur() == _couleur)
        {
            ret.add(*it);
        }
    }
    return ret;
}

Carte *CarteList::getCarteFromList(Carte::Couleur _couleur, Carte::Valeur _value)
{
    Carte *ret = nullptr;

    for (auto it = m_Cartes.begin(); it != m_Cartes.end(); it++)
    {
        if (((*it)->getValeur() == _value) && ((*it)->getCouleur() == _couleur))
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
    while ((it != m_Cartes.end()) && (!(*it)->isBetter(_card)))
        it++;

    if ((it != m_Cartes.end()) && (!(*it)->isBetter(_card)))
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
            if (((*ann)->couleur() == _card.getCouleur()) && ((*ann)->type() == *it))
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
                _card.getCouleur(),
                *it,
                _card.isTrump(),
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
            if (((*ann)->couleur() == _card.getCouleur()) && ((*ann)->type() == *it))
            {
                // great the annouce is already known
                (*ann)->supprimerCarte(_card);
                //TODO _card.removeProbableAnnonce(ann);
            }
        }
    }
}

// retourne la liste des cartes en chaine de caractères
std::string CarteList::cartes()
{
    string cartes;
    for (auto it = m_Cartes.begin(); it != m_Cartes.end(); it++)
    {
        cartes += (*it)->nom() + " ";
    }
    return cartes;

}