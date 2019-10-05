#include <iostream>
#include <random>
#include <string>
#include <chrono>
#include "Chouine.h"

using namespace std;

Chouine::Chouine(int _niveauJoueur1, int _niveauJoueur2):
m_Joueur1(*this, _niveauJoueur1), m_Joueur2(*this, _niveauJoueur2)
{
    m_Joueurs[0] = &m_Joueur1;
    m_Joueurs[1] = &m_Joueur2;
    m_isChouine = false;
    m_CarteJouee = nullptr;
}

Chouine::~Chouine()
{
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
    auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 mt(seed);
    std::uniform_int_distribution<int> dist(0, 3);

    m_GagnantPli = JOUEUR_1;
    int otherJoueur = 1;
    if (dist(mt) >= Carte::NB_COLORS / 2)
    {
        m_GagnantPli = JOUEUR_2;
        otherJoueur = 0;
    }

    // ajoute les cartes à la pioche
    unsigned int id = 0;
    for (auto c = 0; c < Carte::NB_COLORS; c++)
    {
        for (auto v = 0; v < Carte::NB_VALUES; v++)
        {
            m_Pioche.add(new Carte(
                Carte::ALL_COLORS[c],
                Carte::ALL_VALUES[v],
                m_Atout == Carte::ALL_COLORS[c]));
        }
    }

    m_Pioche.shuffle();
    m_Atout = m_Pioche.getLastCarte()->couleur();
    //cout << "Pioche : " << m_Pioche.cartes() << endl;

    m_CarteJouee = nullptr;

    for (int i = 0; i < Joueur::MAX_CARDS; i++)
    {
        Carte *card = m_Pioche.piocheCarte();
        if (card != nullptr)
            m_Joueurs[m_GagnantPli]->addCarte(*card);

        card = m_Pioche.piocheCarte();
        if (card != nullptr)
            m_Joueurs[otherJoueur]->addCarte(*card);
    }
}

bool Chouine::piocheVide()
{
    bool ret;
    (m_Pioche.size() > 0) ? ret = false : ret = true;
    return ret;
}

string Chouine::choixJoueur(Chouine::JOUEUR _player)
{
    string ret("erreur");
    if (_player > 1)
    {
        return ret;
    }
    Carte *carte = nullptr;

    if (m_GagnantPli == _player)
    {
        // ce joueur joue le premier
        carte = m_Joueurs[_player]->choisirCarte(nullptr);
        m_CarteJouee = carte;
    }
    else
    {
        // l'autre joueur a déjà joué
        if (m_CarteJouee == nullptr)
        {
            return ret; // hum...
        }
        carte = m_Joueurs[_player]->choisirCarte(m_CarteJouee);
    }

    if (carte)
    {
        ret = carte->nom();
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
    if ((m_Pioche.size() == 0) && (m_GagnantPli != _player))
    {
        if (!m_Joueurs[_player]->isCarteAllowed(*card, *m_CarteJouee))
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
      Carte cardSeven = new Carte(m_Pioche.getTrumpCouleur(), CarteValue.SEPT, true);
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
   Carte cardSeven = new Carte(m_Pioche.getTrumpCouleur(), CarteValue.SEPT, true);

   if (!m_Joueurs[_player].replaceTrumpCarte(cardSeven.displayName(), m_Pioche.getTrumpCarte()))
   {
      return nullptr;
   }
   m_Pioche.replaceTrumpCarte(cardSeven);
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

   if (m_Pioche.CarteLeft() > 0)
   {
      m_Joueurs[winner].addCarte(m_Pioche.pickCarte());
      m_Joueurs[(winner + 1) % 2].addCarte(m_Pioche.pickCarte());
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