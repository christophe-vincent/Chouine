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
        m_JoueurLevel[i] = 10;
        m_Joueurs[i] = nullptr;
    }
    m_isChouine = false;
    m_PlayedCarte = nullptr;
}

Chouine::~Chouine()
{
    if (m_Joueurs[0] != nullptr)
    {
        delete m_Joueurs[0];
    }
    if (m_Joueurs[1] != nullptr)
    {
        delete m_Joueurs[1];
    }
}

CarteId Chouine::getJoueurCarte(int _player, int _card)
{
    if (_player > 1)
        return NONE;
    Carte *c = m_Joueurs[_player]->getCarte(_card);
    if (c == nullptr)
        return NONE;
}

void Chouine::newGame()
{
    set<Carte *> cards;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, 3);

    m_Trump = Carte::ALL_COLORS[dist(mt)];

    m_StartJoueur = 0;
    int otherJoueur = 1;
    if (dist(mt) >= Carte::NB_COLORS / 2)
    {
        m_StartJoueur = 1;
        otherJoueur = 0;
    }

    // add all cards to pick
    unsigned int id = 0;
    for (auto c = 0; c < Carte::NB_COLORS; c++)
    {
        for (auto v = 0; v < Carte::NB_VALUES; v++)
        {
            m_Pick.add(new Carte(
                Carte::ALL_COLORS[c],
                Carte::ALL_VALUES[v],
                m_Trump == Carte::ALL_COLORS[c]));
        }
    }

    m_Pick.shuffle();

    // set level
    m_Joueurs[0] = new Joueur();
    m_Joueurs[1] = new Joueur();
    m_Joueurs[0]->setLevel(m_JoueurLevel[0]);
    m_Joueurs[1]->setLevel(m_JoueurLevel[1]);

    m_PlayedCarte = nullptr;

    // create card game
    for (int i = 0; i < Carte::NB_COLORS; i++)
    {
        for (int j = 0; j < Carte::NB_VALUES; j++)
        {
            cards.insert(new Carte(Carte::ALL_COLORS[i], Carte::ALL_VALUES[j], false));
        }
    }

    for (int i = 0; i < Joueur::MAX_CARDS; i++)
    {
        Carte *card = m_Pick.pickCarte();
        if (card != nullptr)
            m_Joueurs[m_StartJoueur]->addCarte(*card);

        card = m_Pick.pickCarte();
        if (card != nullptr)
            m_Joueurs[otherJoueur]->addCarte(*card);
    }
}

void Chouine::setJoueurLevel(int _player, int _level)
{
    if (_player > 1)
    {
        return;
    }
    m_JoueurLevel[_player] = _level;
    m_Joueurs[_player]->setLevel(_level);
}

Carte::Couleur Chouine::getTrumpCouleur()
{
    if (m_Pick.size() > 0)
    {
        return m_Pick.getLastCarte()->getCouleur();
    }
    else
    {
        return Carte::UNDEF_COLOR;
    }
}

int Chouine::getJoueurChoice(int _player)
{
    if (_player > 1)
    {
        return -1;
    }
    Carte *playedCarte;
    int ret = -1;

    if (m_StartJoueur == _player)
    {
        // this player start to play, so we choose a card the first
        ret = m_Joueurs[_player]->SimulateMove(nullptr, m_Pick.size());
        playedCarte = m_Joueurs[_player]->getCarte(ret);
    }
    else
    {
        // other user has already played
        if (m_PlayedCarte == nullptr)
        {
            return -1; // error this should not happen
        }
        ret = m_Joueurs[_player]->SimulateMove(m_PlayedCarte, m_Pick.size());
        playedCarte = m_Joueurs[_player]->getCarte(ret);
    }

    return ret;
}

bool Chouine::setJoueurChoice(int _player, int _choice)
{
    if (_player > 1)
    {
        return false;
    }
    Carte *card;
    card = m_Joueurs[_player]->getCarte(_choice);
    if (card == nullptr)
    {
        return false;
    }
    // check if user can play this card
    if ((m_Pick.size() == 0) && (m_StartJoueur != _player))
    {
        if (!m_Joueurs[_player]->isCarteAllowed(*card, *m_PlayedCarte))
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
   if (m_Joueurs[_player].hasChange7Trump())
   {
      Carte cardSeven = new Carte(m_Pick.getTrumpCouleur(), CarteValue.SEPT, true);
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
   Carte cardSeven = new Carte(m_Pick.getTrumpCouleur(), CarteValue.SEPT, true);

   if (!m_Joueurs[_player].replaceTrumpCarte(cardSeven.displayName(), m_Pick.getTrumpCarte()))
   {
      return nullptr;
   }
   m_Pick.replaceTrumpCarte(cardSeven);
   return cardSeven.displayName();
}

// return the player index that win
int Chouine::Play()
{
   int winner;
   bool ret = false;

   if (m_Joueurs[0].CarteLeft() == 0)
   {
      return -1;
   }

   // get strongest card
   winner = m_Donor;
   if (m_JoueurChoices[m_Donor].isBetter(m_JoueurChoices[(m_Donor + 1) % 2]))
   {
      winner = (m_Donor + 1) % 2;
   }

   // indicate to the player that card as played
   m_Joueurs[0].PlayMove(m_JoueurChoices[0]);
   m_Joueurs[1].PlayMove(m_JoueurChoices[1]);

   m_Joueurs[winner].WinCartes(m_JoueurChoices[m_Donor], m_JoueurChoices[(m_Donor + 1) % 2]);
   m_Joueurs[(winner + 1) % 2].enemyWinCartes(m_JoueurChoices[m_Donor], m_JoueurChoices[(m_Donor + 1) % 2]);

   //Tracer.print(Tracer.Verbosity.INFO, ">>> " + m_Joueurs[m_Donor].name() + ": ");
   //Tracer.print(Tracer.Verbosity.INFO, m_JoueurChoices[m_Donor].displayName() + " - ");
   //Tracer.print(Tracer.Verbosity.INFO, m_Joueurs[(m_Donor + 1) % 2].name() + ": ");
   //Tracer.print(Tracer.Verbosity.INFO, m_JoueurChoices[(m_Donor + 1) % 2].displayName() + " - ");
   //Tracer.print(Tracer.Verbosity.INFO, "      GAGANT : " + m_Joueurs[winner].name());
   //Tracer.println(Tracer.Verbosity.INFO, "");
   //Tracer.println(Tracer.Verbosity.INFO, "");

   if (m_Pick.CarteLeft() > 0)
   {
      m_Joueurs[winner].addCarte(m_Pick.pickCarte());
      m_Joueurs[(winner + 1) % 2].addCarte(m_Pick.pickCarte());
   }

   if (m_Joueurs[0].CarteLeft() > 0)
   {
      m_Joueurs[0].displayCartes();
      m_Joueurs[1].displayCartes();
   }

   m_JoueurChoices[0] = nullptr;
   m_JoueurChoices[1] = nullptr;

   m_Donor = winner;

   return winner;
}

bool Chouine::isGameOver()
{
   if (m_Joueurs[0].getIsChouine() || m_Joueurs[1].getIsChouine())
   {
      return true;
   }

   if (m_Joueurs[0].CarteLeft() == 0)
   {
      // gameover
      return true;
   }
   else
   {
      return false;
   }
}


int Chouine::getJoueurAnnouncesPoints(int _player)
{
   if (_player > 1)
   {
      return 0;
   }
   int points = 0;
   set<Announce*> announces;

   announces = getJoueurAnnouces(_player);
   if (!announces.empty())
   {
      for (Announce an : announces)
      {
         points += an.getPoints();
      }
   }
   return points;
}

int Chouine::getJoueurTotalPoints(int _player)
{
   if (_player > 1)
   {
      return 0;
   }

   int points;
   set<Announce*> announces;

   points = getJoueurPoints(_player);
   if (getJoueur10Der(_player) > 0)
   {
      points += getJoueur10Der(_player);
   }
   announces = getJoueurAnnouces(_player);
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
   LinkedList<Carte> donorCartes = new LinkedList<>();
   LinkedList<Carte> otherCartes = new LinkedList<>();

   donorCartes.add(m_Joueurs[0].m_Cartes.get(0));
   donorCartes.add(m_Joueurs[0].m_Cartes.get(1));
   donorCartes.add(m_Joueurs[0].m_Cartes.get(2));
   donorCartes.add(m_Joueurs[0].m_Cartes.get(3));
   //donorCartes.add(m_Joueurs[0].m_Cartes.get(4));

   otherCartes.add(m_Joueurs[1].m_Cartes.get(0));
   otherCartes.add(m_Joueurs[1].m_Cartes.get(1));
   otherCartes.add(m_Joueurs[1].m_Cartes.get(2));
   otherCartes.add(m_Joueurs[1].m_Cartes.get(3));
   //otherCartes.add(m_Joueurs[1].m_Cartes.get(4));

   m_Joueurs[0].bruteForceAttack(donorCartes, otherCartes);
}
*/