#include <iostream>
#include "Carte.h"
#include "Annonce.h"
using namespace std;

const std::array<string, 4> Carte::NOM_COULEURS =
{
    "coeur", "pic", "trèfle", "carreau"
};

const Carte::Couleur Carte::ALL_COLORS[] =
    {
        COEUR,
        PIC,
        TREFLE,
        CARREAU};
const int Carte::NB_COLORS = 4;

const Carte::Valeur Carte::ALL_VALUES[] =
    {
        SEPT,
        HUIT,
        NEUF,
        VALET,
        DAME,
        ROI,
        DIX,
        AS};
const int Carte::NB_VALUES = 8;

Carte::Carte(Couleur _couleur, Valeur _value, bool _trump) : m_Couleur(_couleur), m_Valeur(_value), m_Atout(_trump)
{
    m_Score = 0;
}

Carte::~Carte()
{
}

bool Carte::operator == (const Carte &_card) const
{
    if ((m_Valeur == _card.valeur()) && (m_Couleur == _card.couleur()))
    {
        return true;
    }
    return false;
}

// bool Carte::operator > (const Carte  &_card) const
// {
//     return !gagnante(_card);
// }

// bool Carte::operator < (const Carte  &_card) const
// {
//     return gagnante(_card);
// }


bool Carte::brisque()
{
    if ( (m_Valeur == AS) || (m_Valeur == DIX) )
    {
        return true;
    }
    return false;
}

int Carte::getPoints()
{
    int points;

    switch (m_Valeur)
    {
    case SEPT:
    case HUIT:
    case NEUF:
        points = 0;
        break;
    case VALET:
        points = 2;
        break;
    case DAME:
        points = 3;
        break;
    case ROI:
        points = 4;
        break;
    case DIX:
        points = 10;
        break;
    case AS:
        points = 11;
        break;
    default:
        points = 0;
    }
    return points;
}

// Vrai si _carte est supérieure
bool Carte::gagnante(const Carte &_card) const
{
    bool ret;

    if (m_Couleur == _card.couleur())
    {
        // meme couleur
        if (m_Valeur > _card.valeur())
        {
            ret = false;
        }
        else
        {
            ret = true;
        }
    }
    else
    {
        if (_card.atout())
        {
            ret = true;
        }
        else
        {
            ret = false;
        }
    }
    return ret;
}

std::string Carte::couleurToStr(Couleur _couleur)
{
    if (_couleur < NOM_COULEURS.size())
    {
        return NOM_COULEURS[_couleur];
    }
    return "erreur";
}


string Carte::couleurCarteToStr() const
{
    string couleur = "";
    switch (m_Couleur)
    {
    case COEUR:
        couleur += "COE";
        break;
    case CARREAU:
        couleur += "CAR";
        break;
    case TREFLE:
        couleur += "TRE";
        break;
    case PIC:
        couleur += "PIC";
        break;
    default:
        couleur += "E";
    }
    return couleur;
}

string Carte::carteToStr()
{
    string valeur;

    switch (m_Valeur)
    {
    case SEPT:
        valeur = " 7_";
        break;
    case HUIT:
        valeur = " 8_";
        break;
    case NEUF:
        valeur = " 9_";
        break;
    case DIX:
        valeur = "10_";
        break;
    case VALET:
        valeur = "Va_";
        break;
    case DAME:
        valeur = "Da_";
        break;
    case ROI:
        valeur = "Ro_";
        break;
    case AS:
        valeur = "As_";
        break;
    default:
        valeur = "E_";
        break;
    }

    valeur += couleurCarteToStr();
    if (m_Atout)
    {
        valeur += "*";
    }

    return valeur;
}

std::string Carte::annonceToStr()
{
    std::string str = "";
    for (const auto& pair : m_Annonces)
    {
        str += pair.first->to_string() + ":" + to_string(pair.second) + " ";
    }
    return str;
}