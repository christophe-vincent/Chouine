#include "Carte.h"
using namespace std;

const Carte::Couleur Carte::ALL_COLORS[] =
    {
        CARREAU,
        COEUR,
        PIC,
        TREFLE};
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

Carte::Carte(Couleur _couleur, Valeur _value, bool _trump) : m_Couleur(_couleur), m_Valeur(_value), m_Trump(_trump)
{
    //m_Announce = nullptr;
    m_Score = 0;

    // set the announces the card may be involved
    /*	switch (m_Valeur)
    {
    case AS:
        m_Announcetypes.insert(Announce::CHOUINE);
        m_Announcetypes.insert(Announce::QUARANTE);
        m_Announcetypes.insert(Announce::QUINTE);
        break;
    case DIX:
        m_Announcetypes.insert(Announce::CHOUINE);
        m_Announcetypes.insert(Announce::QUINTE);
        break;
    case ROI:
        m_Announcetypes.insert(Announce::CHOUINE);
        m_Announcetypes.insert(Announce::QUARANTE);
        m_Announcetypes.insert(Announce::TIERCE);
        m_Announcetypes.insert(Announce::MARIAGE);
        break;
    case DAME:
        m_Announcetypes.insert(Announce::CHOUINE);
        m_Announcetypes.insert(Announce::QUARANTE);
        m_Announcetypes.insert(Announce::TIERCE);
        m_Announcetypes.insert(Announce::MARIAGE);
        break;
    case VALET:
        m_Announcetypes.insert(Announce::CHOUINE);
        m_Announcetypes.insert(Announce::QUARANTE);
        m_Announcetypes.insert(Announce::TIERCE);
        break;
    }*/
}

Carte::~Carte()
{
}

bool Carte::operator==(Carte &_card)
{
    if ((m_Valeur == _card.getValeur()) && (m_Couleur == _card.getCouleur()))
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

/*void Carte::addProbableAnnounce(Announce *_announce)
{
    if (m_ProbableAnnounce.find(_announce) == m_ProbableAnnounce.end)
    {
        m_ProbableAnnounce.insert(_announce);
    }
}

void Carte::removeProbableAnnounce(Announce *_announce)
{
   auto it = m_ProbableAnnounce.find(_announce);
    if (it != m_ProbableAnnounce.end)
    {
        m_ProbableAnnounce.erase(it);
    }
}*/

bool Carte::isBetter(Carte &_card)
{
    bool ret;

    if (m_Couleur == _card.getCouleur())
    {
        // same couleur for both
        if (m_Valeur > _card.getValeur())
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
        if (_card.isTrump())
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

string Carte::couleurName()
{
    string name = "";
    switch (m_Couleur)
    {
    case COEUR:
        name += "Coeur";
        break;
    case CARREAU:
        name += "Carreau";
        break;
    case TREFLE:
        name += "Tr�fle";
        break;
    case PIC:
        name += "Pic";
        break;
    default:
        name += "ERROR";
    }
    return name;
}

string Carte::displayName()
{
    string name;

    switch (m_Valeur)
    {
    case SEPT:
        name = "7 ";
        break;
    case HUIT:
        name = "8 ";
        break;
    case NEUF:
        name = "9 ";
        break;
    case DIX:
        name = "10 ";
        break;
    case VALET:
        name = "V ";
        break;
    case DAME:
        name = "D ";
        break;
    case ROI:
        name = "R ";
        break;
    case AS:
        name = "As ";
        break;
    default:
        name = "ERROR";
        break;
    }

    name += couleurName();

    return name;
}