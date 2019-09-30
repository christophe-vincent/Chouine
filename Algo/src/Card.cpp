#include "Card.h"
using namespace std;

const Card::Color Card::ALL_COLORS[] =
    {
        CARREAU,
        COEUR,
        PIC,
        TREFLE};
const int Card::NB_COLORS = 4;

const Card::Value Card::ALL_VALUES[] =
    {
        SEPT,
        HUIT,
        NEUF,
        VALET,
        DAME,
        ROI,
        DIX,
        AS};
const int Card::NB_VALUES = 8;

Card::Card(Color _color, Value _value, bool _trump) : m_Color(_color), m_Value(_value), m_Trump(_trump)
{
    //m_Announce = nullptr;
    m_Score = 0;

    // set the announces the card may be involved
    /*	switch (m_Value)
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

Card::~Card()
{
}

bool Card::operator==(Card &_card)
{
    if ((m_Value == _card.getValue()) && (m_Color == _card.getColor()))
    {
        return true;
    }
    return false;
}

int Card::getPoints()
{
    int points;

    switch (m_Value)
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

/*void Card::addProbableAnnounce(Announce *_announce)
{
    if (m_ProbableAnnounce.find(_announce) == m_ProbableAnnounce.end)
    {
        m_ProbableAnnounce.insert(_announce);
    }
}

void Card::removeProbableAnnounce(Announce *_announce)
{
   auto it = m_ProbableAnnounce.find(_announce);
    if (it != m_ProbableAnnounce.end)
    {
        m_ProbableAnnounce.erase(it);
    }
}*/

bool Card::isBetter(Card &_card)
{
    bool ret;

    if (m_Color == _card.getColor())
    {
        // same color for both
        if (m_Value > _card.getValue())
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

string Card::colorName()
{
    string name = "";
    switch (m_Color)
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

string Card::displayName()
{
    string name;

    switch (m_Value)
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

    name += colorName();

    return name;
}