#include <iostream>
#include "Joueur.h"
#include "Chouine.h"
#include "Algorithme.h"

const int Joueur::MAX_CARDS = 5;

using namespace std;

Joueur::Joueur(Chouine& _chouine, int _niveau):
m_Chouine(_chouine), m_Algo(_niveau, *this), m_Niveau(_niveau)
{
    m_10Der = 0;
    m_LatestAnnonce = nullptr;
    m_Change7Trump = false;
    m_IsChouine = false;
    m_CarteJouee = nullptr;
}

Joueur::~Joueur()
{
}

// retourne la main du joueur en chaine
string Joueur::nomCartesMain()
{
    return m_Cartes.cartes();
}

// carte gagnees
string Joueur::nomCartesGagnees()
{
    return m_CartesGagnees.cartes();
}


Carte *Joueur::getCarte(unsigned int _index)
{
    if (_index >= m_Cartes.size())
    {
        return nullptr;
    }
    return m_Cartes[_index];
}

void Joueur::ajouterCartesGagneesAdversaire(Carte *_c1, Carte *_c2)
{
    m_CartesGagneesAdversaire.ajouter(_c1);
    m_CartesGagneesAdversaire.ajouter(_c2);
}

bool Joueur::hasChange7Trump()
{
    if (m_Change7Trump)
    {
        m_Change7Trump = true;
        return true;
    }
    return false;
}

Status Joueur::addCarte(Carte &_card)
{
    if (m_Cartes.size() >= MAX_CARDS)
    {
        return Status::TOO_MANY_CARDS;
    }

    m_Cartes.newCarteStatistics(_card);

    if (m_Cartes.size() == 0)
    {
        m_Cartes.ajouter(&_card);
        return Status::OK;
    }

    m_Cartes.ajouter(&_card);

    return Status::OK;
}

bool Joueur::replaceTrumpCarte(Carte *_newCarte)
{
    int index = -1;
    Carte *card = nullptr;

    card = m_Cartes.getCarteFromList(m_Chouine.couleurAtout(), Carte::SEPT);

    if (card != nullptr)
    {
        // ok, the user has the trump 7
        m_Cartes.supprimer(card);
        m_Cartes.removeCarteStatistics(*_newCarte);

        // now insert the new card
        addCarte(*_newCarte);
        return true;
    }

    return false;
}

Annonce *Joueur::newAnnonce(set<int> _list)
{
    set<Annonce *> announces;
    CarteList cards;

    // first find cards
    for (auto it = _list.begin(); it != _list.end(); it++)
    {
        cards.ajouter(m_Cartes[*it]);
    }

    announces = Annonce::annonces(cards, m_Annonces, &m_IsChouine);
    if (announces.empty())
    {
        return nullptr;
    }
    else
    {
        m_Annonces.insert(*announces.begin());
        return *announces.begin();
    }
}

Carte* Joueur::choisirCarte(Carte *_carteAdversaire)
{
    if (m_Chouine.piocheVide())
    {
        m_CarteJouee = m_Algo.choisirCartePiocheVide(_carteAdversaire);
        return m_CarteJouee;
    }
    else
    {
        m_CarteJouee = m_Algo.choisirCarte(_carteAdversaire);
        return m_CarteJouee;
    }    
    return nullptr;
}


void Joueur::pliGagnant(Carte& _carteAdversaire)
{
    m_CartesGagnees.ajouter(&_carteAdversaire);
    m_CartesGagnees.ajouter(m_CarteJouee);
    m_Cartes.supprimer(m_CarteJouee);
    m_CarteJouee = nullptr;
    Carte* pioche = m_Chouine.pioche().piocheCarte();
    m_Cartes.ajouter(pioche);    
}

void Joueur::pliPerdant(Carte& _carteAdversaire)
{
    m_CartesGagneesAdversaire.ajouter(m_CarteJouee);
    m_CartesGagneesAdversaire.ajouter(&_carteAdversaire);
    m_Cartes.supprimer(m_CarteJouee);
    m_CarteJouee = nullptr;
    Carte* pioche = m_Chouine.pioche().piocheCarte();
    m_Cartes.ajouter(pioche);
}

Carte *Joueur::getSmallestTrump()
{
    int i = 0;
    CarteList list;

    m_Cartes.getTrumps(list);
    return m_Cartes.plusFaible();
}

int Joueur::trumpNumber()
{
    CarteList list;
    m_Cartes.getTrumps(list);
    return list.size();
}

Carte *Joueur::EmptyPickSimulation(Carte &_userChoice)
{
    Carte *playCarte;
    CarteList couleurList;

    // we must play a higher card
    m_Cartes.getCouleurSubset(_userChoice.couleur(), couleurList);

    playCarte = couleurList.getHigherCarte(_userChoice);
    if (playCarte == nullptr)
    {
        // play trump if user card is not trump...
        if (!_userChoice.atout())
        {
            CarteList trumpList;
            m_Cartes.getCouleurSubset(m_Chouine.couleurAtout(), trumpList);
            playCarte = couleurList.plusFaible();
        }
    }

    return playCarte;
}

// check if the Joueur is allowed to play this card or not
// it is only done when pick is empty : user must play higher card
bool Joueur::isCarteAllowed(Carte &_card, Carte &_otherCarte)
{
    if (m_Cartes.size() == 1)
    {
        // last card...
        return true;
    }

    if (_otherCarte.compare(_card))
    {
        // played card is better than other Joueur : it is allowed
        return true;
    }

    Carte *card = m_Cartes.getHigherCarte(_otherCarte);
    if (card == nullptr)
    {
        // the Joueur does not have higher card, so he can play nay card
        return true;
    }
    else
    {
        return false;
    }
}

Status Joueur::PlayCarte(Carte &_card)
{
    m_Cartes.removeCarteStatistics(_card);
    m_Cartes.supprimer(&_card);
    return Status::OK;
}

Status Joueur::ajouterCartesGagnees(Carte &_card1, Carte &_card2)
{
    if (m_Cartes.size() == 0)
    {
        m_10Der = 10;
    }
    m_CartesGagnees.ajouter(&_card1);
    m_CartesGagnees.ajouter(&_card2);
    return Status::OK;
}

Carte *Joueur::bruteForceAttack(CarteList &_hisCartes)
{
    /*int level = 0;
   bool end = false;
   CarteList donorCartes;
   CarteList otherCartes;
   Node nodes;
   int* cardScore;
   int donorId; */
    vector<int> myCartesIdx(m_Cartes.size());
    vector<int> hisCartesIdx(_hisCartes.size());

    int i = 0;
    std::fill(myCartesIdx.begin(), myCartesIdx.end(), i++);
    i = 0;
    std::fill(hisCartesIdx.begin(), hisCartesIdx.end(), i++);

    return nullptr;
    // TO BE DONE
    //for ()
    /*
   // root level
   nodes = ProcessLevel(null, donorCartes, otherCartes, 0);
   nodesLevels.add((LinkedList<MoveNode>) nodes);

   level++;

   do
   {
      // go to the next level
      // loop on each node of the current level
      nodes = new LinkedList<>();
      for (MoveNode cn : nodesLevels.get(level - 1))
      {
         donorCartes.clear();
         otherCartes.clear();
         if ((cn.getIsWinner() && (cn.getDonorId() == 0))
            || (!cn.getIsWinner() && (cn.getDonorId() != 0)))
         {
            donorCartes.addAll(_myCartes);
            otherCartes.addAll(_hisCartes);
            donorId = 0;
         }
         else
         {
            donorCartes.addAll(_hisCartes);
            otherCartes.addAll(_myCartes);
            donorId = 1;
         }

         // remove all cards played
         node = cn;
         do
         {
            donorCartes.remove(node.getDonorCarte());
            donorCartes.remove(node.getOtherCarte());

            otherCartes.remove(node.getOtherCarte());
            otherCartes.remove(node.getDonorCarte());

            node = node.getParent();
         } while (node != null);

         levelNode = ProcessLevel(cn, donorCartes, otherCartes, donorId);
         nodes.addAll(levelNode);
      }

      nodesLevels.add((LinkedList<MoveNode>) nodes);
      level++;

      if (donorCartes.size() < 2)
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

   cardScore = new int[_myCartes.size()];
   for (int i = 0; i < _myCartes.size(); i++)
   {
      cardScore[i] = 0;
   }
   for (MoveNode cn : nodesLevels.get(0))
   {
      cardScore[_myCartes.indexOf(cn.getDonorCarte())] += cn.getTotalScore();
      //Tracer.print(Tracer.Verbosity.DEBUG, cn.getDonorCarte().displayName());
      //Tracer.println(Tracer.Verbosity.DEBUG, " : " + String.valueOf(cn.getTotalScore()));
   }

   //Tracer.println(Tracer.Verbosity.DEBUG, "");
   int maxScore = Integer.MIN_VALUE;
   Carte chooseCarte = null;
   for (int i = 0; i < _myCartes.size(); i++)
   {
      //Tracer.println(Tracer.Verbosity.DEBUG, _myCartes.get(i).displayName()+" : "+cardScore[i]);
      if (cardScore[i] > maxScore)
      {
         chooseCarte = _myCartes.get(i);
         maxScore = cardScore[i];
      }
   }


   //Tracer.println(Tracer.Verbosity.DEBUG, "Carte to be played = " + chooseCarte.displayName());
   return chooseCarte;
   */
}

/*private LinkedList<MoveNode> ProcessLevel(MoveNode _parentNode,
   LinkedList<Carte> _donorCartes,
   LinkedList<Carte> _otherCartes,
   int _donorId)
{
   LinkedList<MoveNode> nodeLevel = new LinkedList<>();
   MoveNode node;
   boolean isStrongest;
   LinkedList<Carte> allowedCartes;
   boolean lastMove;
   int i;

   lastMove = false;
   if (_donorCartes.size() < 2)
   {
      lastMove = true;
   }

   //Tracer.println(Tracer.Verbosity.DEBUG, "-");
   for (i = 0; i < _donorCartes.size(); i++)
      //for (Carte donorCarte : _donorCartes)
   {
      // remove the cards the other cannot play
      // add all stronger cards in the list
      isStrongest = false;
      for (Carte c : _otherCartes)
      {
         if (_donorCartes.get(i).isBetter(c))
         {
            isStrongest = true; // at least one card is better
         }
      }

      // he must play better card or nay if none of the cards is better
      allowedCartes = new LinkedList<>();
      for (Carte c : _otherCartes)
      {
         if ((_donorCartes.get(i).isBetter(c)) || (!isStrongest))
         {
            allowedCartes.add(c);
         }
      }

      // add childs to the donor node
      for (Carte c : allowedCartes)
      {
         // create a new node
         node = new MoveNode(_parentNode, _donorCartes.get(i), c, _donorId, lastMove);
         nodeLevel.add(node);
      }
   }
   return nodeLevel;
}
*/