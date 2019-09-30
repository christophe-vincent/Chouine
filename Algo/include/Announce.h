#ifndef _ANNOUNCE_H_
#define _ANNOUNCE_H_

#include <set>
#include <vector>
#include "Card.h"
#include "CardList.h"
using namespace std;

class Announce
{
public:
    enum AnnounceType
    {
        NONE = 0,
        MARIAGE = 20,
        TIERCE = 30,
        QUARANTE = 40,
        QUINTE = 50,
        CHOUINE = 1000
    };

    static const int CARD_POINT = 10;
    static const Card::Value MARIAGE_CARDS[];
    static const Card::Value TIERCE_CARDS[];
    static const Card::Value QUARANTE_CARDS[];
    static const Card::Value CHOUINE_CARDS[];
    static const set<AnnounceType> ANNOUNCES;

    Announce(Card::Color _color,
             AnnounceType _type,
             bool _trump,
             CardList &_cards);
    ~Announce();

    void addCard(Card &_card);
    void removeCard(Card &_card);

    static bool isMariage(CardList &_list);
    static bool isTierce(CardList &_list);
    static bool isQuarante(CardList &_list);
    static bool isQuinte(CardList &_list);
    static bool isChouine(CardList &_list);
    static set<Announce *> isAnnounce(CardList &_list,
                                      set<Announce *> &_knownAnnounces,
                                      bool *_isChouine);

    Card::Color getColor() { return m_Color; }
    AnnounceType getType() { return m_Announce; }

private:
    //void computeScore();

    CardList m_Cards;
    Card::Color m_Color;
    AnnounceType m_Announce;
    int m_Points;
    bool m_IsTrump;
    float m_Score;
};

#endif