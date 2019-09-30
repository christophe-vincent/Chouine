#ifndef _PLAYER_H_
#define _PLAYER_H_
#include <string>
#include <set>
#include <map>
#include "Carte.h"
#include "ListeCartes.h"
#include "Annonce.h"

using namespace std;

class Chouine;

class Node
{
private:
    Node *m_Parent;
    // pair : card idx play 1, card idx player 2
    map<pair<int, int>, Node *> m_Nodes;
    int m_Points;
};

class Joueur
{
public:
    Joueur();
    virtual ~Joueur();

    static const int MAX_CARDS;

    void setLevel(int _level) { m_Level = _level; }
    int getLevel() { return m_Level; }
    CarteList &getCartes() { return m_Cartes; }
    CarteList &getWinCartes() { return m_WinCartes; }
    Annonce *getLatestAnnonce() { return m_LatestAnnonce; }
    bool getIsChouine() { return m_IsChouine; }
    int CarteLeft() { return m_Cartes.size(); }
    int getPoints() { return m_WinCartes.getPoints(); }
    int get10Der() { return m_10Der; }
    set<Annonce *> getAnnouces() { return m_Annonces; }

    Carte *getCarte(unsigned int _index);
    void enemyWinCartes(Carte *_c1, Carte *_c2);
    bool hasChange7Trump();
    Status addCarte(Carte &_card);
    Annonce *newAnnonce(set<int> _list);
    int SimulateMove(Carte *_enemyChoice, int _pickLeft);
    Carte *getSmallestTrump();
    int trumpNumber();
    bool replaceTrumpCarte(Carte *_newCarte);
    bool isCarteAllowed(Carte &_card, Carte &_otherCarte);
    Status PlayCarte(Carte &_card);
    Status WinCartes(Carte &_card1, Carte &_card2);
    Carte *bruteForceAttack(CarteList &_hisCartes);

protected:
    Carte *EmptyPickSimulation(CarteList &_cards);
    Carte *EmptyPickSimulation(Carte &_userChoice);

    bool m_IsChouine;
    bool m_Change7Trump;

private:
    CarteList m_Cartes;    // 5 cards to be played
    CarteList m_WinCartes; // cards win
    CarteList m_EnemyWinCartes;
    set<Annonce *> m_Annonces;
    Annonce *m_LatestAnnonce;
    int m_Level;
    int m_10Der;
};

#endif