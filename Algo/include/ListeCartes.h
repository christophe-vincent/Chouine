/*
Copyright 2025,2026 Christophe Vincent
This file is part of La Chouine.
La Chouine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or any later version.
La Chouine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>
*/
#ifndef _CARDLIST_H
#define _CARDLIST_H
#include <vector>
#include "Carte.h"

class Annonce;

class ListeCartes
{
public:
    ListeCartes();
    ListeCartes(Carte *_card);

    void ajouter(Carte *_card);
    void supprimer(Carte *_card);
    void shuffle(std::vector<std::string> _order);
    Carte *piocheCarte();
    Carte *searchCarte(Carte::Valeur _value);
    Carte *searchCarte(Carte::Valeur _value, Carte::Couleur _couleur);

    // retourne la position d'une carte dans la liste
    unsigned int position(Carte* _carte);

    // échange une carte avec celle de la liste
    Carte *echangerCarte(Carte* _carte, unsigned int position);

    void getTrumps(ListeCartes &_list);
    Carte *plusFaible(bool _sansAtout=true);
    Carte* choisirPlusForte(Carte* _carte);
    void getCouleurSubset(Carte::Couleur _couleur, ListeCartes &_list);
    bool isContainsValeur(Carte::Valeur _value);
    bool rechercheCarte(Carte &_value);
    ListeCartes getCartesFromList(Carte::Valeur _value);
    ListeCartes cartesMemeCouleur(Carte::Couleur _couleur);
    Carte *getCarteFromList(Carte::Couleur _couleur, Carte::Valeur _value);
    int NumberOfValeur(Carte::Valeur _value);
    Carte *getHigherValeur(Carte::Valeur _value);
    Carte *getHigherCarte(Carte &_value);
    int getPoints();

    std::vector<Carte*>& cartes() { return m_Cartes; }
    void setCartes(std::vector<Carte*> _cartes) {m_Cartes = _cartes;}
    Carte* septAtout() { return m_SeptAtout; }
    unsigned int size() { return (unsigned int)m_Cartes.size(); }
    Carte *getLastCarte() { return m_Cartes.front(); }
    Carte *operator[](size_t _idx) { return m_Cartes[_idx]; }
    unsigned int nombreCartes() { return m_Cartes.size();};
    std::string nomCartes();

private:
    void scoreAnnonces();
    void miseAJourAnnonces();

private:
    std::vector<Carte *> m_Cartes;
    Carte* m_SeptAtout;
};

#endif