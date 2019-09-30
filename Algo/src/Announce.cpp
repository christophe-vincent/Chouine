#include "Announce.h"

const Card::Value Announce::MARIAGE_CARDS[] =
    {
        Card::DAME, Card::ROI};
const Card::Value TIERCE_CARDS[] =
    {
        Card::VALET, Card::DAME, Card::ROI};
const Card::Value QUARANTE_CARDS[] =
    {
        Card::ROI, Card::DAME, Card::VALET, Card::AS};
const Card::Value CHOUINE_CARDS[] =
    {
        Card::ROI, Card::DAME, Card::VALET, Card::DIX, Card::AS};

const set<Announce::AnnounceType> Announce::ANNOUNCES =
    {MARIAGE, TIERCE, QUARANTE, QUINTE, CHOUINE};

Announce::Announce(Card::Color _color, AnnounceType _type, bool _trump, CardList &_cards) : m_Color(_color), m_Announce(_type), m_IsTrump(_trump), m_Cards(_cards)
{
    //TODO computeScore();
}

Announce::~Announce()
{
}

void Announce::addCard(Card &_card)
{
    m_Cards.add(&_card);
    // increase the score
    //TODO computeScore();
}

void Announce::removeCard(Card &_card)
{
    m_Cards.remove(&_card);
}

/*void Announce::computeScore()
{
   // score depends on the card number
   switch (m_Announce)
   {

   case MARIAGE:
      m_Score = (float)m_Cards.size() / (float)MARIAGE_CARDS.length;
      break;
   case TIERCE:
      m_Score = (float)m_Cards.size() / (float)TIERCE_CARDS.length;
      break;
   case QUARANTE:
      m_Score = (float)m_Cards.size() / (float)QUARANTE_CARDS.length;
      break;
   case QUINTE:
      m_Score = (float)m_Cards.size() / 5.0f;
      break;
   case CHOUINE:
      m_Score = (float)m_Cards.size() / (float)CHOUINE_CARDS.length;
      break;
   }
}*/

bool Announce::isMariage(CardList &_list)
{
    if ((_list.isContainsValue(Card::DAME)) ||
        (_list.isContainsValue(Card::ROI)))
    {
        return true;
    }
    return false;
}

bool Announce::isTierce(CardList &_list)
{
    if ((_list.isContainsValue(Card::VALET)) ||
        (_list.isContainsValue(Card::DAME)) ||
        (_list.isContainsValue(Card::ROI)))
    {
        return true;
    }
    return false;
}

bool Announce::isQuarante(CardList &_list)
{
    if ((_list.isContainsValue(Card::VALET)) ||
        (_list.isContainsValue(Card::DAME)) ||
        (_list.isContainsValue(Card::ROI)) ||
        (_list.isContainsValue(Card::DIX)))
    {
        return true;
    }
    return false;
}

bool Announce::isQuinte(CardList &_list)
{
    int nBrisques = 0;

    for (unsigned int i = 0; i < _list.size(); i++)
    {
        if (_list[i]->getValue() == Card::AS || _list[i]->getValue() == Card::DIX)
        {
            nBrisques++;
        }
    }
    return nBrisques == 5;
}

bool Announce::isChouine(CardList &_list)
{
    bool ret = true;
    Card::Color color;

    if (_list.size() < 5)
        return false;

    // first check that all card have the same color
    color = _list[0]->getColor();
    for (unsigned int i = 1; i < _list.size(); i++)
    {
        if (_list[i]->getColor() != color)
        {
            return false;
        }
    }
    if ((_list.isContainsValue(Card::SEPT)) ||
        (_list.isContainsValue(Card::HUIT)) ||
        (_list.isContainsValue(Card::NEUF)))
    {
        ret = false;
    }
    return ret;
}

set<Announce *> Announce::isAnnounce(CardList &_list, set<Announce *> &_knownAnnounces, bool *_isChouine)
{
    Announce *ann;
    CardList colorList;
    CardList cards;
    set<Card::Color> colorToCheck;
    *_isChouine = false;

    set<Announce *> ret;
    if (_list.size() <= 1)
    {
        return ret;
    }

    if (isChouine(_list))
    {
        ann = new Announce(_list[0]->getColor(),
                           AnnounceType::CHOUINE,
                           _list[0]->isTrump(),
                           _list);
        *_isChouine = true;
        ret.insert(ann);
    }

    if (isQuinte(_list))
    {
        // if quinte already announced, do not accept it
        bool quintePresent = false;
        for (Announce *ca : _knownAnnounces)
        {
            if (ca->getType() == AnnounceType::QUINTE)
            {
                quintePresent = true;
            }
        }
        if (!quintePresent)
        {
            ann = new Announce(Card::BACK, AnnounceType::QUINTE, false, _list);
            ret.insert(ann);
        }
        return ret;
    }

    for (Card::Color color : colorToCheck)
    {
        colorList = _list.getCardsFromList(color);

        if (isQuarante(colorList))
        {
            ann = new Announce(color, AnnounceType::QUARANTE,
                               _list[0]->isTrump(), cards);
            ret.insert(ann);
        }
        else if (isTierce(colorList))
        {
            ann = new Announce(color, AnnounceType::TIERCE,
                               _list[0]->isTrump(), cards);
            ret.insert(ann);
        }
        else if (isMariage(colorList))
        {
            ann = new Announce(color, AnnounceType::MARIAGE,
                               _list[0]->isTrump(), cards);
            ret.insert(ann);
        }
    }

    return ret;
}
