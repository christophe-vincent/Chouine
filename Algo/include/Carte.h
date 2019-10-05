#pragma once
#ifndef _CARTE_H_
#define _CARTE_H_
#include <set>
#include <string>
#include <array>
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

    static const std::array<string, 4> NOM_COULEURS;

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

    static std::string nomCouleur(Couleur _couleur);
    Couleur couleur() { return m_Couleur; }
    Valeur getValeur() { return m_Valeur; }
    bool atout() { return m_Atout; }
    void atout(bool _trump) { m_Atout = _trump; }
    //Announce* getAnnounce()                            { return m_Announce; }
    //void setAnnounce(Announce *_ann)                   { m_Announce = _ann; }
    //set<Announce*> getProbableAnnounces()              { return m_ProbableAnnounce; }
    //set<Announce::AnnounceType> getAllAnnounceTypes()  { return m_Announcetypes; }
    int getScore() { return m_Score; }
    void setScore(int _score) { m_Score = _score; }

    int getPoints();
    //void addProbableAnnounce(Announce* _announce);
    //void removeProbableAnnounce(Announce* _announce);

    // vrai si la carte en argument est plus forte
    bool compare(Carte &_card);
    std::string nomCouleur();
    std::string nom();

private:
    Couleur m_Couleur;
    Valeur m_Valeur;
    bool m_Atout;
    //Announce *m_Announce;
    //set<Announce*> m_ProbableAnnounce;
    //set<Announce::AnnounceType> m_Announcetypes;
    int m_Score;
};

#endif