#ifndef _PLAYER_H_
#define _PLAYER_H_
#include <string>
#include <set>
#include <map>
#include "Card.h"
#include "CardList.h"
#include "Announce.h"

using namespace std;

class Chouine;

class Node
{
private:
   Node *m_Parent;
   // pair : card idx play 1, card idx player 2
   map< pair<int, int>, Node*> m_Nodes;
   int m_Points;
};

class Player
{
public:
	Player();
	virtual ~Player();

   static const int MAX_CARDS;

   void setLevel(int _level) { m_Level = _level; }
   int  getLevel() { return m_Level; }
   CardList& getCards() { return m_Cards; }
   CardList& getWinCards() { return m_WinCards; }
   Announce* getLatestAnnounce() { return m_LatestAnnounce; }
   bool getIsChouine() { return m_IsChouine; }
   int CardLeft() { return m_Cards.size(); }
   int getPoints() { return m_WinCards.getPoints(); }
   int get10Der() { return m_10Der; }
   set<Announce*> getAnnouces() { return m_Announces; }

   Card* getCard(unsigned int _index);
   void enemyWinCards(Card* _c1, Card* _c2);
   bool hasChange7Trump();
   Status addCard(Card& _card);
   Announce* newAnnounce(set<int> _list);
   int SimulateMove(Card* _enemyChoice, int _pickLeft);
   Card* getSmallestTrump();
   int trumpNumber();
   bool replaceTrumpCard(Card* _newCard);
   bool isCardAllowed(Card& _card, Card& _otherCard);
   Status PlayCard(Card& _card);
   Status WinCards(Card& _card1, Card& _card2);
   Card* bruteForceAttack(CardList &_hisCards);

protected:
   Card* EmptyPickSimulation(CardList& _cards);
   Card* EmptyPickSimulation(Card& _userChoice);

   bool m_IsChouine;
   bool m_Change7Trump;

private:
	CardList m_Cards;      // 5 cards to be played
   CardList m_WinCards;   // cards win
   CardList m_EnemyWinCards;
	set<Announce*> m_Announces;
	Announce* m_LatestAnnounce;
	int m_Level;
	int m_10Der;
};

#endif