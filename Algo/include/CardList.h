#ifndef _CARDLIST_H
#define _CARDLIST_H
#include <vector>
#include "Card.h"

class Announce;
using namespace std;

class CardList
{
public:
    CardList();
    CardList(Card *_card);

    void add(Card *_card);
    void remove(Card *_card);
    void shuffle();
    Card *pickCard();
    Card *searchCard(Card::Value _value);
    void getTrumps(CardList &_list);
    Card *getSmallest();
    void getColorSubset(Card::Color _color, CardList &_list);
    bool isContainsValue(Card::Value _value);
    bool isContainsCard(Card &_value);
    CardList getCardsFromList(Card::Value _value);
    CardList getCardsFromList(Card::Color _color);
    Card *getCardFromList(Card::Color _color, Card::Value _value);
    int NumberOfValue(Card::Value _value);
    Card *getHigherValue(Card::Value _value);
    Card *getHigherCard(Card &_value);
    int getPoints();

    void newCardStatistics(Card &_card);
    void removeCardStatistics(Card &_card);

    //set<Card*> getCards() { return m_Cards; }
    bool isTrumpSeven() { return m_isTrumpSeven; }
    unsigned int size() { return (unsigned int)m_Cards.size(); }
    Card *getLastCard() { return *m_Cards.rbegin(); }
    Card *operator[](size_t _idx) { return m_Cards[_idx]; }

private:
    vector<Card *> m_Cards;
    vector<Announce *> m_AnnouncesStats;
    bool m_isTrumpSeven;
};

#endif