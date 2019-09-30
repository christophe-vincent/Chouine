#pragma once
#ifndef _CARTE_H_
#define _CARTE_H_
#include <set>
#include <string>
using namespace std;

enum class Status
{
    OK,
    TOO_MANY_CARDS
};

class Carte
{
public:
    enum Couleur
    {
        COEUR,
        PIC,
        TREFLE,
        CARREAU,
        BACK,
        UNDEF_COLOR
    };

    static const Couleur ALL_COLORS[];
    static const int NB_COLORS;

    enum Valeur
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

    static const Valeur ALL_VALUES[];
    static const int NB_VALUES;

    Carte(Couleur _couleur, Valeur _value, bool _trump);
    ~Carte();

    bool operator==(Carte &_card);

    Couleur getCouleur() { return m_Couleur; }
    Valeur getValeur() { return m_Valeur; }
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
    bool isBetter(Carte &_card);
    std::string couleurName();
    std::string displayName();

private:
    Couleur m_Couleur;
    Valeur m_Valeur;
    bool m_Trump;
    //Announce *m_Announce;
    //set<Announce*> m_ProbableAnnounce;
    //set<Announce::AnnounceType> m_Announcetypes;
    int m_Score;
};

#endif