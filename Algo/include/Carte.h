#pragma once
#ifndef _CARTE_H_
#define _CARTE_H_
#include <set>
#include <string>
#include <array>
#include <map>

class Annonce;

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

    static const std::array<std::string, 4> NOM_COULEURS;

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

    bool operator==(const Carte &_card) const;
    //bool operator>(const Carte &_card) const;
    //bool operator<(const Carte &_card) const;

    // retourne la couleur
    Couleur couleur() const { return m_Couleur; }

    // retourne la valeur
    Valeur valeur() const { return m_Valeur; }

    // vrai si la carte est un atout
    bool atout() const { return m_Atout; }

    // ecriture de l'atout
    void atout(bool _trump) { m_Atout = _trump; }

    // score getter/setter
    const int score() const & { return m_Score; }
    void score(int _score) { m_Score = _score; }

    bool gagnante(const Carte &_card) const;

    // retourne les annonces associées à cette carte
    std::map<Annonce*, int>& annonces() { return m_Annonces; }

    // retourne vrai si la carte est une brisque (10 ou As)
    bool brisque();

    // retourne le nombre de points de la carte
    int getPoints();

    // convertit vers une chaine de chaine de caracteres
    static std::string couleurToStr(Couleur _couleur);
    std::string couleurCarteToStr() const;
    std::string carteToStr();
    std::string annonceToStr();

private:
    Couleur m_Couleur;
    Valeur m_Valeur;
    bool m_Atout;
    std::map<Annonce*, int> m_Annonces;
    int m_Score;

   
};

#endif