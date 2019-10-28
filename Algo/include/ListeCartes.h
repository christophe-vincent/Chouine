#ifndef _CARDLIST_H
#define _CARDLIST_H
#include <vector>
#include "Carte.h"

class Annonce;
using namespace std;

class ListeCartes
{
private:
public:
    ListeCartes();
    ListeCartes(Carte *_card);

    void ajouter(Carte *_card);
    void supprimer(Carte *_card);
    void shuffle();
    Carte *piocheCarte();
    Carte *searchCarte(Carte::Valeur _value);
    void getTrumps(ListeCartes &_list);
    Carte *plusFaible(bool _sansAtout=true);
    Carte* choisirPlusForte(Carte* _carte);
    void getCouleurSubset(Carte::Couleur _couleur, ListeCartes &_list);
    bool isContainsValeur(Carte::Valeur _value);
    bool isContainsCarte(Carte &_value);
    ListeCartes getCartesFromList(Carte::Valeur _value);
    ListeCartes cartesMemeCouleur(Carte::Couleur _couleur);
    Carte *getCarteFromList(Carte::Couleur _couleur, Carte::Valeur _value);
    int NumberOfValeur(Carte::Valeur _value);
    Carte *getHigherValeur(Carte::Valeur _value);
    Carte *getHigherCarte(Carte &_value);
    int getPoints();

    vector<Carte*>& cartes() { return m_Cartes; }
    bool isTrumpSeven() { return m_isTrumpSeven; }
    unsigned int size() { return (unsigned int)m_Cartes.size(); }
    Carte *getLastCarte() { return m_Cartes.front(); }
    Carte *operator[](size_t _idx) { return m_Cartes[_idx]; }
    unsigned int nombreCartes() { return m_Cartes.size();};
    std::string nomCartes();

private:
    void scoreAnnonces();
    void miseAJourAnnonces();

private:
    vector<Carte *> m_Cartes;
    bool m_isTrumpSeven;
};

#endif