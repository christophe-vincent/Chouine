#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include "Annonce.h"
#include "ListeCartes.h"
#include "Annonce.h"

using namespace std;
static std::random_device rd;
static std::mt19937 mt(rd());

ListeCartes::ListeCartes() : m_isTrumpSeven(false)
{
}

ListeCartes::ListeCartes(Carte *_card) : m_isTrumpSeven(false)
{
    m_Cartes.push_back(_card);
}

///////////////////////////////
void ListeCartes::ajouter(Carte *_carte)
{
    if (_carte == nullptr) return;
    // si la carte est deja dans la liste, rien à faire
    for (Carte* carte : m_Cartes)
    {
        if (_carte == carte)
        {
            return;
        }
    }
    //cout << _carte->nom() << endl;
    if ((_carte->atout()) && (_carte->valeur() == Carte::SEPT))
    {
        m_isTrumpSeven = true;
    }
    m_Cartes.push_back(_carte);
}

///////////////////////////////
void ListeCartes::supprimer(Carte *_card)
{
    if (_card == nullptr) return;
    auto it = find(m_Cartes.begin(), m_Cartes.end(), _card);
    if (it != m_Cartes.end())
    {
        m_Cartes.erase(it);
        if ((_card->atout()) && (_card->valeur() == Carte::SEPT))
        {
            m_isTrumpSeven = false;
        }
    }
}

///////////////////////////////
void ListeCartes::shuffle()
{
    std::shuffle(m_Cartes.begin(), m_Cartes.end(), mt);
}

///////////////////////////////
// return first card and remove it from list (like we we pick a card)
Carte *ListeCartes::piocheCarte()
{
    if (m_Cartes.size() == 0)
    {
        return nullptr;
    }
    Carte *ret = m_Cartes.back();
    m_Cartes.pop_back();
    return ret;
}

///////////////////////////////
Carte *ListeCartes::searchCarte(Carte::Valeur _value)
{
    Carte *ret = nullptr;

    auto it = m_Cartes.begin();
    while ((it != m_Cartes.end()) && (ret == nullptr))
    {
        if ((*it)->valeur() == _value)
        {
            ret = *it;
        }
        it++;
    }
    return ret;
}


///////////////////////////////
void ListeCartes::getTrumps(ListeCartes &_list)
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

///////////////////////////////
Carte *ListeCartes::plusFaible(bool _sansAtout)
{
    Carte *plusFaible;
    Carte *plusFaibleHorsAtout = nullptr;
    auto it = m_Cartes.begin();
    plusFaible = *it;
    while (it != m_Cartes.end())
    {
        if ( (plusFaibleHorsAtout == nullptr) && (!(*it)->atout()) )
        {
            // init de la carte la plus faible sans jouer d'atout
            plusFaibleHorsAtout = *it;
        }
        else
        {
            it++;
            continue;
        }
        if (!(*it)->atout() && ((*it)->getPoints() < plusFaibleHorsAtout->getPoints()))
        {
            //cout << (*it)->nom() << " < " << plusFaibleHorsAtout->nom() << endl;
            plusFaibleHorsAtout = *it;
        }
        if ((*it)->getPoints() < plusFaibleHorsAtout->getPoints())
        {
            plusFaible = *it;
        }
        it++;
    }
    if ((_sansAtout) && (plusFaibleHorsAtout != nullptr) )
    {
        return plusFaibleHorsAtout;
    }
    return plusFaible;
}


///////////////////////////////
Carte* ListeCartes::choisirPlusForte(Carte* _carte)
{
    Carte* carte = nullptr; 

    for (auto it=m_Cartes.begin(); it!=m_Cartes.end(); ++it)
    {
        if ((*it)->getPoints() > _carte->getPoints())
        {
            if (carte == nullptr)
            {
                carte = *it;
            }
            else
            {
                if ((*it)->getPoints() < _carte->getPoints())
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

////////////////////////////////
// remplit la liste _list avec les cartes de la couleur _couleur
void ListeCartes::getCouleurSubset(Carte::Couleur _couleur, ListeCartes &_list)
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

////////////////////////////////
// 
bool ListeCartes::isContainsValeur(Carte::Valeur _value)
{
    bool ret = false;

    for (auto it = m_Cartes.begin(); it != m_Cartes.end(); it++)
    {
        if ((*it)->valeur() == _value)
        {
            ret = true;
            break;
        }
    }

    return ret;
}

bool ListeCartes::rechercheCarte(Carte &_value)
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

ListeCartes ListeCartes::getCartesFromList(Carte::Valeur _value)
{
    ListeCartes ret;

    for (auto it = m_Cartes.begin(); it != m_Cartes.end(); it++)
    {
        if ((*it)->valeur() == _value)
        {
            ret.ajouter(*it);
        }
    }
    return ret;
}

ListeCartes ListeCartes::cartesMemeCouleur(Carte::Couleur _couleur)
{
    ListeCartes ret;

    for (auto it = m_Cartes.begin(); it != m_Cartes.end(); it++)
    {
        if ((*it)->couleur() == _couleur)
        {
            ret.ajouter(*it);
        }
    }
    return ret;
}

Carte *ListeCartes::getCarteFromList(Carte::Couleur _couleur, Carte::Valeur _value)
{
    Carte *ret = nullptr;

    for (auto it = m_Cartes.begin(); it != m_Cartes.end(); it++)
    {
        if (((*it)->valeur() == _value) && ((*it)->couleur() == _couleur))
        {
            ret = *it;
        }
    }

    return ret;
}

int ListeCartes::NumberOfValeur(Carte::Valeur _value)
{
    int ret = 0;

    for (auto it = m_Cartes.begin(); it != m_Cartes.end(); it++)
    {
        if ((*it)->valeur() == _value)
        {
            ret++;
        }
    }

    return ret;
}

Carte *ListeCartes::getHigherValeur(Carte::Valeur _value)
{
    Carte *card = nullptr;
    auto it = m_Cartes.begin();
    while ((it != m_Cartes.end()) && (_value > (*it)->valeur()))
        it++;

    if ((it != m_Cartes.end()) && (_value > (*it)->valeur()))
    {
        // we found the card !
        card = *it;
    }
    return card;
}

Carte *ListeCartes::getHigherCarte(Carte &_carte)
{
    Carte *card = nullptr;
    auto it = m_Cartes.begin();
    while ((it != m_Cartes.end()) && (!(_carte.getPoints() > (*it)->getPoints())))
        it++;

    if ((it != m_Cartes.end()) && (! (_carte.getPoints() > (*it)->getPoints())))
    {
        // we found the card !
        card = *it;
    }
    return card;
}

int ListeCartes::getPoints()
{
    int points = 0;

    for (auto it : m_Cartes)
    {
        points += it->getPoints();
    }
    return points;
}


///////////////////////////////
// retourne la liste des cartes en chaine de caractères
std::string ListeCartes::nomCartes()
{
    string cartes;
    for (auto it = m_Cartes.begin(); it != m_Cartes.end(); it++)
    {
        if (cartes.length() > 0) {
            cartes += " ";
        }
       cartes += (*it)->carteToStr();
    }
    return cartes;
}

///////////////////////////////
// Calcule le score de chacune des annonces
void ListeCartes::scoreAnnonces()
{
    // boucle sur les couleurs
    for (unsigned int i=0; i<4; i++)
    {
        ListeCartes cartesCouleur = cartesMemeCouleur(Carte::ALL_COLORS[i]);
        // boucle sur toutes les annonces
       /* for (auto it=Annonce::ANNONCES.begin();
            it!=Annonce::ANNONCES.end(); ++it )
        {
            

        }*/
    }
}


void ListeCartes::miseAJourAnnonces()
{
    Annonce *ann;
    ListeCartes couleurList;
    ListeCartes cartes;
    set<Carte::Couleur> couleurToCheck;
/*
    if (Annonce::chouine(*this))
    {
        ann = new Annonce(_list[0]->couleur(),
                          TypeAnnonce::CHOUINE,
                          _list[0]->atout(),
                          _list);
        *_isChouine = true;
        ret.insert(ann);
    }

    if (quinte(_list))
    {
        // if quinte already announced, do not accept it
        bool quintePresent = false;
        for (Annonce *ca : _annoncesConnues)
        {
            if (ca->type() == TypeAnnonce::QUINTE)
            {
                quintePresent = true;
            }
        }
        if (!quintePresent)
        {
            ann = new Annonce(Carte::BACK, TypeAnnonce::QUINTE, false, _list);
            ret.insert(ann);
        }
        return ret;
    }

    for (Carte::Couleur couleur : couleurToCheck)
    {
        couleurList = _list.cartesMemeCouleur(couleur);

        if (quarante(couleurList))
        {
            ann = new Annonce(couleur, TypeAnnonce::QUARANTE,
                              _list[0]->atout(), cartes);
            ret.insert(ann);
        }
        else if (tierce(couleurList))
        {
            ann = new Annonce(couleur, TypeAnnonce::TIERCE,
                              _list[0]->atout(), cartes);
            ret.insert(ann);
        }
        else if (mariage(couleurList))
        {
            ann = new Annonce(couleur, TypeAnnonce::MARIAGE,
                              _list[0]->atout(), cartes);
            ret.insert(ann);
        }
    }
    */
}
