#include "Player.h"
#include "PlayerAlgo.h"
#include "Chouine.h"

const int Player::MAX_CARDS = 5;

Player::Player()
{
    m_10Der = 0;
    m_Level = 10; // max level
    m_LatestAnnounce = nullptr;
    m_Change7Trump = false;
    m_IsChouine = false;
}

Player::~Player()
{
}

Card *Player::getCard(unsigned int _index)
{
    if (_index >= m_Cards.size())
    {
        return nullptr;
    }
    return m_Cards[_index];
}

void Player::enemyWinCards(Card *_c1, Card *_c2)
{
    m_EnemyWinCards.add(_c1);
    m_EnemyWinCards.add(_c2);
}

bool Player::hasChange7Trump()
{
    if (m_Change7Trump)
    {
        m_Change7Trump = true;
        return true;
    }
    return false;
}

Status Player::addCard(Card &_card)
{
    if (m_Cards.size() >= MAX_CARDS)
    {
        return Status::TOO_MANY_CARDS;
    }

    m_Cards.newCardStatistics(_card);

    if (m_Cards.size() == 0)
    {
        m_Cards.add(&_card);
        return Status::OK;
    }

    m_Cards.add(&_card);

    return Status::OK;
}

bool Player::replaceTrumpCard(Card *_newCard)
{
    int index = -1;
    Card *card = nullptr;
    Chouine *chouine = Chouine::getInstance();

    card = m_Cards.getCardFromList(chouine->getTrumpColor(), Card::SEPT);

    if (card != nullptr)
    {
        // ok, the user has the trump 7
        m_Cards.remove(card);
        m_Cards.removeCardStatistics(*_newCard);

        // now insert the new card
        addCard(*_newCard);
        return true;
    }

    return false;
}

Announce *Player::newAnnounce(set<int> _list)
{
    set<Announce *> announces;
    CardList cards;

    // first find cards
    for (auto it = _list.begin(); it != _list.end(); it++)
    {
        cards.add(m_Cards[*it]);
    }

    announces = Announce::isAnnounce(cards, m_Announces, &m_IsChouine);
    if (announces.empty())
    {
        return nullptr;
    }
    else
    {
        m_Announces.insert(*announces.begin());
        return *announces.begin();
    }
}

int Player::SimulateMove(Card *_enemyChoice, int _pickLeft)
{
    return -1;
}

Card *Player::getSmallestTrump()
{
    int i = 0;
    CardList list;

    m_Cards.getTrumps(list);
    return m_Cards.getSmallest();
}

int Player::trumpNumber()
{
    CardList list;
    m_Cards.getTrumps(list);
    return list.size();
}

Card *Player::EmptyPickSimulation(CardList &_cards)
{
    Card *playCard;

    if (m_Level > 5)
    {
        // TODO
        playCard = bruteForceAttack(_cards);
    }
    else
    {
        playCard = m_Cards.getSmallest();
    }

    return playCard;
}

Card *Player::EmptyPickSimulation(Card &_userChoice)
{
    Card *playCard;
    CardList colorList;

    // we must play a higher card
    m_Cards.getColorSubset(_userChoice.getColor(), colorList);

    playCard = colorList.getHigherCard(_userChoice);
    if (playCard == nullptr)
    {
        // play trump if user card is not trump...
        if (!_userChoice.isTrump())
        {
            CardList trumpList;
            m_Cards.getColorSubset(Chouine::getInstance()->getTrumpColor(), trumpList);
            playCard = colorList.getSmallest();
        }
    }

    return playCard;
}

// check if the player is allowed to play this card or not
// it is only done when pick is empty : user must play higher card
bool Player::isCardAllowed(Card &_card, Card &_otherCard)
{
    if (m_Cards.size() == 1)
    {
        // last card...
        return true;
    }

    if (_otherCard.isBetter(_card))
    {
        // played card is better than other player : it is allowed
        return true;
    }

    Card *card = m_Cards.getHigherCard(_otherCard);
    if (card == nullptr)
    {
        // the player does not have higher card, so he can play nay card
        return true;
    }
    else
    {
        return false;
    }
}

Status Player::PlayCard(Card &_card)
{
    m_Cards.removeCardStatistics(_card);
    m_Cards.remove(&_card);
    return Status::OK;
}

Status Player::WinCards(Card &_card1, Card &_card2)
{
    if (m_Cards.size() == 0)
    {
        m_10Der = 10;
    }
    m_WinCards.add(&_card1);
    m_WinCards.add(&_card2);
    return Status::OK;
}

Card *Player::bruteForceAttack(CardList &_hisCards)
{
    /*int level = 0;
   bool end = false;
   CardList donorCards;
   CardList otherCards;
   Node nodes;
   int* cardScore;
   int donorId; */
    vector<int> myCardsIdx(m_Cards.size());
    vector<int> hisCardsIdx(_hisCards.size());

    int i = 0;
    std::fill(myCardsIdx.begin(), myCardsIdx.end(), i++);
    i = 0;
    std::fill(hisCardsIdx.begin(), hisCardsIdx.end(), i++);

    return nullptr;
    // TO BE DONE
    //for ()
    /*
   // root level
   nodes = ProcessLevel(null, donorCards, otherCards, 0);
   nodesLevels.add((LinkedList<MoveNode>) nodes);

   level++;

   do
   {
      // go to the next level
      // loop on each node of the current level
      nodes = new LinkedList<>();
      for (MoveNode cn : nodesLevels.get(level - 1))
      {
         donorCards.clear();
         otherCards.clear();
         if ((cn.getIsWinner() && (cn.getDonorId() == 0))
            || (!cn.getIsWinner() && (cn.getDonorId() != 0)))
         {
            donorCards.addAll(_myCards);
            otherCards.addAll(_hisCards);
            donorId = 0;
         }
         else
         {
            donorCards.addAll(_hisCards);
            otherCards.addAll(_myCards);
            donorId = 1;
         }

         // remove all cards played
         node = cn;
         do
         {
            donorCards.remove(node.getDonorCard());
            donorCards.remove(node.getOtherCard());

            otherCards.remove(node.getOtherCard());
            otherCards.remove(node.getDonorCard());

            node = node.getParent();
         } while (node != null);

         levelNode = ProcessLevel(cn, donorCards, otherCards, donorId);
         nodes.addAll(levelNode);
      }

      nodesLevels.add((LinkedList<MoveNode>) nodes);
      level++;

      if (donorCards.size() < 2)
      {
         end = true;
      }
   } while (!end);


   // compute scores
   // for each nodes of the last level
   for (MoveNode cn : nodesLevels.get(level - 1))
   {
      node = cn;

      // goes up to the root
      do
      {
         node.getParent().addScore(node.getScore());
         node = node.getParent();
      } while (node.getParent() != null);
      node.addScore(node.getScore());
   }

   cardScore = new int[_myCards.size()];
   for (int i = 0; i < _myCards.size(); i++)
   {
      cardScore[i] = 0;
   }
   for (MoveNode cn : nodesLevels.get(0))
   {
      cardScore[_myCards.indexOf(cn.getDonorCard())] += cn.getTotalScore();
      //Tracer.print(Tracer.Verbosity.DEBUG, cn.getDonorCard().displayName());
      //Tracer.println(Tracer.Verbosity.DEBUG, " : " + String.valueOf(cn.getTotalScore()));
   }

   //Tracer.println(Tracer.Verbosity.DEBUG, "");
   int maxScore = Integer.MIN_VALUE;
   Card chooseCard = null;
   for (int i = 0; i < _myCards.size(); i++)
   {
      //Tracer.println(Tracer.Verbosity.DEBUG, _myCards.get(i).displayName()+" : "+cardScore[i]);
      if (cardScore[i] > maxScore)
      {
         chooseCard = _myCards.get(i);
         maxScore = cardScore[i];
      }
   }


   //Tracer.println(Tracer.Verbosity.DEBUG, "Card to be played = " + chooseCard.displayName());
   return chooseCard;
   */
}

/*private LinkedList<MoveNode> ProcessLevel(MoveNode _parentNode,
   LinkedList<Card> _donorCards,
   LinkedList<Card> _otherCards,
   int _donorId)
{
   LinkedList<MoveNode> nodeLevel = new LinkedList<>();
   MoveNode node;
   boolean isStrongest;
   LinkedList<Card> allowedCards;
   boolean lastMove;
   int i;

   lastMove = false;
   if (_donorCards.size() < 2)
   {
      lastMove = true;
   }

   //Tracer.println(Tracer.Verbosity.DEBUG, "-");
   for (i = 0; i < _donorCards.size(); i++)
      //for (Card donorCard : _donorCards)
   {
      // remove the cards the other cannot play
      // add all stronger cards in the list
      isStrongest = false;
      for (Card c : _otherCards)
      {
         if (_donorCards.get(i).isBetter(c))
         {
            isStrongest = true; // at least one card is better
         }
      }

      // he must play better card or nay if none of the cards is better
      allowedCards = new LinkedList<>();
      for (Card c : _otherCards)
      {
         if ((_donorCards.get(i).isBetter(c)) || (!isStrongest))
         {
            allowedCards.add(c);
         }
      }

      // add childs to the donor node
      for (Card c : allowedCards)
      {
         // create a new node
         node = new MoveNode(_parentNode, _donorCards.get(i), c, _donorId, lastMove);
         nodeLevel.add(node);
      }
   }
   return nodeLevel;
}
*/