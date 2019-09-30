#include <random>
#include <string>
#include "Chouine.h"

Chouine *Chouine::m_Instance = nullptr;

Chouine *Chouine::getInstance()
{
    if (m_Instance == nullptr)
    {
        m_Instance = new Chouine();
    }
    return m_Instance;
}

Chouine::Chouine()
{
    for (int i = 0; i < 2; i++)
    {
        m_PlayerLevel[i] = 10;
        m_Players[i] = nullptr;
    }
    m_isChouine = false;
    m_PlayedCard = nullptr;
}

Chouine::~Chouine()
{
    if (m_Players[0] != nullptr)
    {
        delete m_Players[0];
    }
    if (m_Players[1] != nullptr)
    {
        delete m_Players[1];
    }
}

CardId Chouine::getPlayerCard(int _player, int _card)
{
    if (_player > 1)
        return NONE;
    Card *c = m_Players[_player]->getCard(_card);
    if (c == nullptr)
        return NONE;
}

void Chouine::newGame()
{
    set<Card *> cards;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, 3);

    m_Trump = Card::ALL_COLORS[dist(mt)];

    m_StartPlayer = 0;
    int otherPlayer = 1;
    if (dist(mt) >= Card::NB_COLORS / 2)
    {
        m_StartPlayer = 1;
        otherPlayer = 0;
    }

    // add all cards to pick
    unsigned int id = 0;
    for (auto c = 0; c < Card::NB_COLORS; c++)
    {
        for (auto v = 0; v < Card::NB_VALUES; v++)
        {
            m_Pick.add(new Card(
                Card::ALL_COLORS[c],
                Card::ALL_VALUES[v],
                m_Trump == Card::ALL_COLORS[c]));
        }
    }

    m_Pick.shuffle();

    // set level
    m_Players[0] = new Player();
    m_Players[1] = new Player();
    m_Players[0]->setLevel(m_PlayerLevel[0]);
    m_Players[1]->setLevel(m_PlayerLevel[1]);

    m_PlayedCard = nullptr;

    // create card game
    for (int i = 0; i < Card::NB_COLORS; i++)
    {
        for (int j = 0; j < Card::NB_VALUES; j++)
        {
            cards.insert(new Card(Card::ALL_COLORS[i], Card::ALL_VALUES[j], false));
        }
    }

    for (int i = 0; i < Player::MAX_CARDS; i++)
    {
        Card *card = m_Pick.pickCard();
        if (card != nullptr)
            m_Players[m_StartPlayer]->addCard(*card);

        card = m_Pick.pickCard();
        if (card != nullptr)
            m_Players[otherPlayer]->addCard(*card);
    }
}

void Chouine::setPlayerLevel(int _player, int _level)
{
    if (_player > 1)
    {
        return;
    }
    m_PlayerLevel[_player] = _level;
    m_Players[_player]->setLevel(_level);
}

Card::Color Chouine::getTrumpColor()
{
    if (m_Pick.size() > 0)
    {
        return m_Pick.getLastCard()->getColor();
    }
    else
    {
        return Card::UNDEF_COLOR;
    }
}

int Chouine::getPlayerChoice(int _player)
{
    if (_player > 1)
    {
        return -1;
    }
    Card *playedCard;
    int ret = -1;

    if (m_StartPlayer == _player)
    {
        // this player start to play, so we choose a card the first
        ret = m_Players[_player]->SimulateMove(nullptr, m_Pick.size());
        playedCard = m_Players[_player]->getCard(ret);
    }
    else
    {
        // other user has already played
        if (m_PlayedCard == nullptr)
        {
            return -1; // error this should not happen
        }
        ret = m_Players[_player]->SimulateMove(m_PlayedCard, m_Pick.size());
        playedCard = m_Players[_player]->getCard(ret);
    }

    return ret;
}

bool Chouine::setPlayerChoice(int _player, int _choice)
{
    if (_player > 1)
    {
        return false;
    }
    Card *card;
    card = m_Players[_player]->getCard(_choice);
    if (card == nullptr)
    {
        return false;
    }
    // check if user can play this card
    if ((m_Pick.size() == 0) && (m_StartPlayer != _player))
    {
        if (!m_Players[_player]->isCardAllowed(*card, *m_PlayedCard))
        {
            return false;
        }
    }

    // TODO : lancer la r�solution de la done ici

    return true;
}

/*
string Chouine::hasChange7Trump(int _player)
{
   if (_player > 1)
   {
      return nullptr;
   }
   if (m_Players[_player].hasChange7Trump())
   {
      Card cardSeven = new Card(m_Pick.getTrumpColor(), CardValue.SEPT, true);
      return cardSeven.displayName();
   }
   else
   {
      return nullptr;
   }
}


string Chouine::change7Trump(int _player)
{
   if (_player > 1)
   {
      return nullptr;
   }
   Card cardSeven = new Card(m_Pick.getTrumpColor(), CardValue.SEPT, true);

   if (!m_Players[_player].replaceTrumpCard(cardSeven.displayName(), m_Pick.getTrumpCard()))
   {
      return nullptr;
   }
   m_Pick.replaceTrumpCard(cardSeven);
   return cardSeven.displayName();
}

// return the player index that win
int Chouine::Play()
{
   int winner;
   bool ret = false;

   if (m_Players[0].CardLeft() == 0)
   {
      return -1;
   }

   // get strongest card
   winner = m_Donor;
   if (m_PlayerChoices[m_Donor].isBetter(m_PlayerChoices[(m_Donor + 1) % 2]))
   {
      winner = (m_Donor + 1) % 2;
   }

   // indicate to the player that card as played
   m_Players[0].PlayMove(m_PlayerChoices[0]);
   m_Players[1].PlayMove(m_PlayerChoices[1]);

   m_Players[winner].WinCards(m_PlayerChoices[m_Donor], m_PlayerChoices[(m_Donor + 1) % 2]);
   m_Players[(winner + 1) % 2].enemyWinCards(m_PlayerChoices[m_Donor], m_PlayerChoices[(m_Donor + 1) % 2]);

   //Tracer.print(Tracer.Verbosity.INFO, ">>> " + m_Players[m_Donor].name() + ": ");
   //Tracer.print(Tracer.Verbosity.INFO, m_PlayerChoices[m_Donor].displayName() + " - ");
   //Tracer.print(Tracer.Verbosity.INFO, m_Players[(m_Donor + 1) % 2].name() + ": ");
   //Tracer.print(Tracer.Verbosity.INFO, m_PlayerChoices[(m_Donor + 1) % 2].displayName() + " - ");
   //Tracer.print(Tracer.Verbosity.INFO, "      GAGANT : " + m_Players[winner].name());
   //Tracer.println(Tracer.Verbosity.INFO, "");
   //Tracer.println(Tracer.Verbosity.INFO, "");

   if (m_Pick.CardLeft() > 0)
   {
      m_Players[winner].addCard(m_Pick.pickCard());
      m_Players[(winner + 1) % 2].addCard(m_Pick.pickCard());
   }

   if (m_Players[0].CardLeft() > 0)
   {
      m_Players[0].displayCards();
      m_Players[1].displayCards();
   }

   m_PlayerChoices[0] = nullptr;
   m_PlayerChoices[1] = nullptr;

   m_Donor = winner;

   return winner;
}

bool Chouine::isGameOver()
{
   if (m_Players[0].getIsChouine() || m_Players[1].getIsChouine())
   {
      return true;
   }

   if (m_Players[0].CardLeft() == 0)
   {
      // gameover
      return true;
   }
   else
   {
      return false;
   }
}


int Chouine::getPlayerAnnouncesPoints(int _player)
{
   if (_player > 1)
   {
      return 0;
   }
   int points = 0;
   set<Announce*> announces;

   announces = getPlayerAnnouces(_player);
   if (!announces.empty())
   {
      for (Announce an : announces)
      {
         points += an.getPoints();
      }
   }
   return points;
}

int Chouine::getPlayerTotalPoints(int _player)
{
   if (_player > 1)
   {
      return 0;
   }

   int points;
   set<Announce*> announces;

   points = getPlayerPoints(_player);
   if (getPlayer10Der(_player) > 0)
   {
      points += getPlayer10Der(_player);
   }
   announces = getPlayerAnnouces(_player);
   if (!announces.empty())
   {
      for (Announce an : announces)
      {
         points += an.getPoints();
      }
   }
   return points;
}

void Chouine::testBruteForce()
{
   LinkedList<Card> donorCards = new LinkedList<>();
   LinkedList<Card> otherCards = new LinkedList<>();

   donorCards.add(m_Players[0].m_Cards.get(0));
   donorCards.add(m_Players[0].m_Cards.get(1));
   donorCards.add(m_Players[0].m_Cards.get(2));
   donorCards.add(m_Players[0].m_Cards.get(3));
   //donorCards.add(m_Players[0].m_Cards.get(4));

   otherCards.add(m_Players[1].m_Cards.get(0));
   otherCards.add(m_Players[1].m_Cards.get(1));
   otherCards.add(m_Players[1].m_Cards.get(2));
   otherCards.add(m_Players[1].m_Cards.get(3));
   //otherCards.add(m_Players[1].m_Cards.get(4));

   m_Players[0].bruteForceAttack(donorCards, otherCards);
}
*/