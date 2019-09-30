#ifndef _CARDLIST_H
#define _CARDLIST_H
#include <vector>
#include "Carte.h"

class Annonce;
using namespace std;

class CarteList
{
public:
    CarteList();
    CarteList(Carte *_card);

    void add(Carte *_card);
    void remove(Carte *_card);
    void shuffle();
    Carte *pickCarte();
    Carte *searchCarte(Carte::Valeur _value);
    void getTrumps(CarteList &_list);
    Carte *getSmallest();
    void getCouleurSubset(Carte::Couleur _couleur, CarteList &_list);
    bool isContainsValeur(Carte::Valeur _value);
    bool isContainsCarte(Carte &_value);
    CarteList getCartesFromList(Carte::Valeur _value);
    CarteList getCartesFromList(Carte::Couleur _couleur);
    Carte *getCarteFromList(Carte::Couleur _couleur, Carte::Valeur _value);
    int NumberOfValeur(Carte::Valeur _value);
    Carte *getHigherValeur(Carte::Valeur _value);
    Carte *getHigherCarte(Carte &_value);
    int getPoints();

    void newCarteStatistics(Carte &_card);
    void removeCarteStatistics(Carte &_card);

    //set<Carte*> getCartes() { return m_Cartes; }
    bool isTrumpSeven() { return m_isTrumpSeven; }
    unsigned int size() { return (unsigned int)m_Cartes.size(); }
    Carte *getLastCarte() { return *m_Cartes.rbegin(); }
    Carte *operator[](size_t _idx) { return m_Cartes[_idx]; }

private:
    vector<Carte *> m_Cartes;
    vector<Annonce *> m_AnnoncesStats;
    bool m_isTrumpSeven;
};

#endif