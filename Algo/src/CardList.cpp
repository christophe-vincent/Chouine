#include <algorithm>
#include "CardList.h"
#include "Announce.h"

CardList::CardList():
   m_isTrumpSeven(false)
{
}

CardList::CardList(Card* _card) :
   m_isTrumpSeven(false)
{
   m_Cards.push_back(_card);
}

void CardList::add(Card* _card)
{
   if ((_card->isTrump()) && (_card->getValue() == Card::SEPT))
   {
      m_isTrumpSeven = true;
   }
   m_Cards.push_back(_card);
}


void CardList::remove(Card* _card)
{
   auto it = find(m_Cards.begin(), m_Cards.end(), _card);
   if (it != m_Cards.end())
   {
      m_Cards.erase(it);
      if ((_card->isTrump()) && (_card->getValue() == Card::SEPT))
      {
         m_isTrumpSeven = false;
      }
   }
}

void CardList::shuffle()
{
   random_shuffle(m_Cards.begin(), m_Cards.end());
}

// return first card and remove it from list (like we we pick a card)
Card* CardList::pickCard()
{
   if (m_Cards.size() == 0)
   {
      return nullptr;
   }
   Card *ret = *m_Cards.begin();
   m_Cards.pop_back();
   return ret;
}


Card* CardList::searchCard(Card::Value _value)
{
   Card* ret = nullptr;

   auto it = m_Cards.begin();
   while ((it != m_Cards.end()) && (ret == nullptr))
   {
      if ((*it)->getValue() == _value)
      {
         ret = *it;
      }
      it++;
   }
   return ret;
}

void CardList::getTrumps(CardList &_list)
{
   auto it = m_Cards.begin();
   while (it != m_Cards.end())
   {
      if ((*it)->isTrump())
      {
         _list.add(*it);
      }
      it++;
   }
}

Card * CardList::getSmallest()
{
   Card* smallest;
   auto it = m_Cards.begin();
   smallest = *it;
   while (it != m_Cards.end())
   {
      if ((*it)->getPoints() < smallest->getPoints())
      {
         smallest = *it;
      }
      it++;
   }
   return *it;
}

void CardList::getColorSubset(Card::Color _color, CardList & _list)
{
   auto it = m_Cards.begin();
   while (it != m_Cards.end())
   {
      if ((*it)->getColor() == _color)
      {
         _list.add(*it);
      }
      it++;
   }
}


bool CardList::isContainsValue(Card::Value _value)
{
   bool ret = false;

   for (auto it = m_Cards.begin(); it != m_Cards.end(); it++)
   {
      if ((*it)->getValue() == _value)
      {
         ret = true;
         break;
      }
   }

   return ret;
}

bool CardList::isContainsCard(Card &_value)
{
   bool ret = false;

   for (auto it = m_Cards.begin(); it != m_Cards.end(); it++)
   {
      if ( (**it) == _value)
      {
         ret = true;
         break;
      }
   }

   return ret;
}

CardList CardList::getCardsFromList(Card::Value _value)
{
   CardList ret;

   for (auto it = m_Cards.begin(); it != m_Cards.end(); it++)
   {
      if ((*it)->getValue() == _value)
      {
         ret.add(*it);
      }
   }
   return ret;
}

CardList CardList::getCardsFromList(Card::Color _color)
{
   CardList ret;

   for (auto it = m_Cards.begin(); it != m_Cards.end(); it++)
   {
      if ((*it)->getColor() == _color)
      {
         ret.add(*it);
      }
   }
   return ret;
}

Card* CardList::getCardFromList(Card::Color _color, Card::Value _value)
{
   Card* ret = nullptr;

   for (auto it = m_Cards.begin(); it != m_Cards.end(); it++)
   {
      if ( ((*it)->getValue() == _value) && ((*it)->getColor() == _color) )
      {
         ret = *it;
      }
   }

   return ret;
}

int CardList::NumberOfValue(Card::Value _value)
{
   int ret = 0;

   for (auto it = m_Cards.begin(); it != m_Cards.end(); it++)
   {
      if ((*it)->getValue() == _value)
      {
         ret++;
      }
   }

   return ret;
}

Card* CardList::getHigherValue(Card::Value _value)
{
   Card* card = nullptr;
   auto it = m_Cards.begin();
   while ((it != m_Cards.end()) && (_value > (*it)->getValue())) it++;

   if ((it != m_Cards.end()) && (_value > (*it)->getValue()))
   {
      // we found the card !
      card = *it;
   }
   return card;
}

Card* CardList::getHigherCard(Card& _card)
{
   Card* card = nullptr;
   auto it = m_Cards.begin();
   while ((it != m_Cards.end()) && (!(*it)->isBetter(_card))) it++;

   if ((it != m_Cards.end()) && (!(*it)->isBetter(_card)))
   {
      // we found the card !
      card = *it;
   }
   return card;
}

int CardList::getPoints()
{
   int points = 0;

   for (auto it : m_Cards)
   {
      points += it->getPoints();
   }
   return points;
}

void CardList::newCardStatistics(Card& _card)
{
   set<Announce*> cardAnnounces;
   bool create = true;

   if ((_card.getValue() == Card::SEPT)
      || (_card.getValue() == Card::HUIT)
      || (_card.getValue() == Card::NEUF))
   {
      return; // these cards are not used in announces
   }

   // check all possible announces for this card

   for (auto it = Announce::ANNOUNCES.begin(); it != Announce::ANNOUNCES.end(); it++)
   {
      // search in the list if this announce has already statistics
      create = true;
      for (auto ann=m_AnnouncesStats.begin(); ann!=m_AnnouncesStats.end(); ann++)
      {
         if ( ((*ann)->getColor() == _card.getColor())
            && ((*ann)->getType() == *it))
         {
            // great the annouce is already known
            (*ann)->addCard(_card);
            //TODO _card->addProbableAnnounce(ann);
            create = false;
         }
      }

      if (create)
      {
         // announce has not been created, create a new one
         Announce* announce = new Announce(_card.getColor(), *it, _card.isTrump(), CardList(&_card));
         m_AnnouncesStats.push_back(announce);
         //TODO _card->addProbableAnnounce(announce);
      }
   }
}

void CardList::removeCardStatistics(Card& _card)
{
   if ((_card.getValue() == Card::SEPT)
      || (_card.getValue() == Card::HUIT)
      || (_card.getValue() == Card::NEUF))
   {
      return; // these cards are not used in announces
   }

   // check all possible announces for this card
   for (auto it = Announce::ANNOUNCES.begin(); it != Announce::ANNOUNCES.end(); it++)
   {
      // search in the list if this announce has already statistics
      for (auto ann = m_AnnouncesStats.begin(); ann != m_AnnouncesStats.end(); ann++)
      {
         if (((*ann)->getColor() == _card.getColor())
            && ((*ann)->getType() == *it))
         {
            // great the annouce is already known
            (*ann)->removeCard(_card);
            //TODO _card.removeProbableAnnounce(ann);
         }
      }
   }
}