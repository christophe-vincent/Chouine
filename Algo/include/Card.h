#pragma once
#ifndef _CARD_H_
#define _CARD_H_
#include <set>
#include <string>
using namespace std;

enum class Status
{
    OK,
    TOO_MANY_CARDS
};

class Card
{
public:
    enum Color
    {
        COEUR,
        PIC,
        TREFLE,
        CARREAU,
        BACK,
        UNDEF_COLOR
    };

    static const Color ALL_COLORS[];
    static const int NB_COLORS;

    enum Value
    {
        SEPT,
        HUIT,
        NEUF,
        VALET,
        DAME,
        ROI,
        DIX,
        AS
    };

    static const Value ALL_VALUES[];
    static const int NB_VALUES;

    Card(Color _color, Value _value, bool _trump);
    ~Card();

    bool operator==(Card &_card);

    Color getColor() { return m_Color; }
    Value getValue() { return m_Value; }
    bool isTrump() { return m_Trump; }
    void setTrump(bool _trump) { m_Trump = _trump; }
    //Announce* getAnnounce()                            { return m_Announce; }
    //void setAnnounce(Announce *_ann)                   { m_Announce = _ann; }
    //set<Announce*> getProbableAnnounces()              { return m_ProbableAnnounce; }
    //set<Announce::AnnounceType> getAllAnnounceTypes()  { return m_Announcetypes; }
    int getScore() { return m_Score; }
    void setScore(int _score) { m_Score = _score; }

    int getPoints();
    //void addProbableAnnounce(Announce* _announce);
    //void removeProbableAnnounce(Announce* _announce);

    // true if given card is better
    bool isBetter(Card &_card);
    std::string colorName();
    std::string displayName();

private:
    Color m_Color;
    Value m_Value;
    bool m_Trump;
    //Announce *m_Announce;
    //set<Announce*> m_ProbableAnnounce;
    //set<Announce::AnnounceType> m_Announcetypes;
    int m_Score;
};

#endif