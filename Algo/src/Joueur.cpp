#include <iostream>
#include <string.h>
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
    // creation de toutes les annnonces possibles
    Annonce *annonce;
    for (unsigned int i=0; i<Carte::NB_COLORS; i++)
    {
        bool atout = false;
        if (Carte::ALL_COLORS[i] == m_Chouine.couleurAtout())
            atout = true;
        annonce = new Annonce(Carte::ALL_COLORS[i], 
                      Annonce::MARIAGE, atout);
        m_Annonces.insert(annonce);
        annonce = new Annonce(Carte::ALL_COLORS[i], 
                      Annonce::TIERCE, atout);
        m_Annonces.insert(annonce);
        annonce = new Annonce(Carte::ALL_COLORS[i], 
                      Annonce::QUARANTE, atout);
        m_Annonces.insert(annonce);
        annonce = new Annonce(Carte::ALL_COLORS[i], 
                      Annonce::CHOUINE, atout);
        m_Annonces.insert(annonce);
    }
    annonce = new Annonce(Annonce::QUINTE);
    m_Annonces.insert(annonce);
}

Joueur::~Joueur()
{
}

// retourne la main du joueur en chaine
string Joueur::nomCartesMain()
{
    return m_Cartes.nomCartes();
}

// carte gagnees
string Joueur::nomCartesGagnees()
{
    return m_CartesGagnees.nomCartes();
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

Status Joueur::ajouterCarte(Carte &_carte)
{
    if (m_Cartes.size() >= MAX_CARDS)
    {
        return Status::TOO_MANY_CARDS;
    }

    m_Cartes.ajouter(&_carte);
    for (Annonce *annonce: m_Annonces)
    {
        annonce->ajouterCarte(_carte);
        if (annonce->cartesManquantes() == 0)
        {
            //printAnnonces();
            //exit(1);
        }
    }
    printAnnonces();

    return Status::OK;
}


Status Joueur::supprimerCarte(Carte *_carte)
{
    m_Cartes.supprimer(_carte);
    for (Annonce *annonce: m_Annonces)
    {
        annonce->supprimerCarte(_carte);
    }
    printAnnonces();
}


// cherche si la carte fait partie d'une annonce
Annonce* Joueur::rechercheAnnonce(Carte &_carte)
{
    Annonce* annoncePlusForte = nullptr;
    for (Annonce *annonce: m_Annonces)
    {
        if (annonce->cartePresente(_carte))
        {
            if (annonce == nullptr)
            {
                annoncePlusForte = annonce;
            }
            else if (annonce > annoncePlusForte)
            {
                annoncePlusForte = annonce;                
            }
        }
    }
    return annoncePlusForte;
}

void Joueur::printAnnonces()
{
    for (Annonce *annonce: m_Annonces)
    {
        char ann;
        switch (annonce->type())
        {
        case Annonce::MARIAGE :
            ann='M';
            break;
        case Annonce::TIERCE :
            ann='T';
            break;
        case Annonce::QUARANTE :
            ann='Q';
            break;
        case Annonce::CHOUINE :
            ann='C';
            break;
        case Annonce::QUINTE :
            ann='5';
            break;        
        default:
            break;
        }
        char couleur[3];
        switch (annonce->couleur())
        {
        case Carte::PIC:
            strcpy(couleur,  "Pi");
            break;
        case Carte::TREFLE:
            strcpy(couleur,  "Tr");
            break;
        case Carte::CARREAU:
            strcpy(couleur,  "Ca");
            break;
        case Carte::COEUR:
            strcpy(couleur,  "Co");
            break;        
        default:
            break;
        }
        printf("%c%s:%u ",
               ann, 
               couleur,
               annonce->cartesManquantes());
    }
    printf("\n");
}

bool Joueur::replaceTrumpCarte(Carte *_newCarte)
{
    int index = -1;
    Carte *card = nullptr;

    card = m_Cartes.getCarteFromList(m_Chouine.couleurAtout(), Carte::SEPT);

    if (card != nullptr)
    {
        // ok, on a le 7 d'atout
        m_Cartes.supprimer(card);
        
        ajouterCarte(*_newCarte);
        return true;
    }

    return false;
}


Carte* Joueur::choisirCarte(Carte *_carteAdversaire)
{
    m_CarteJouee = nullptr;
    if (m_Chouine.piocheVide())
    {
        m_CarteJouee = m_Algo.choisirCartePiocheVide(_carteAdversaire);
    }
    else
    {
        m_CarteJouee = m_Algo.choisirCarte(_carteAdversaire);
    }
    Annonce *annonce = rechercheAnnonce(*_carteAdversaire);
    if (annonce != nullptr)
    {

    }
    return m_CarteJouee;
}


void Joueur::pliGagnant(Carte& _carteAdversaire)
{
    m_CartesGagnees.ajouter(&_carteAdversaire);
    m_CartesGagnees.ajouter(m_CarteJouee);
    supprimerCarte(m_CarteJouee);
    m_CarteJouee = nullptr;
    Carte* pioche = m_Chouine.pioche().piocheCarte();
    if (pioche != nullptr)
    {
        ajouterCarte(*pioche);
    }
}

void Joueur::pliPerdant(Carte& _carteAdversaire)
{
    m_CartesGagneesAdversaire.ajouter(m_CarteJouee);
    m_CartesGagneesAdversaire.ajouter(&_carteAdversaire);
    supprimerCarte(m_CarteJouee);
    m_CarteJouee = nullptr;
    Carte* pioche = m_Chouine.pioche().piocheCarte();
    if (pioche != nullptr)
    {
        ajouterCarte(*pioche);
    }
}

Carte *Joueur::getSmallestTrump()
{
    int i = 0;
    ListeCartes list;

    m_Cartes.getTrumps(list);
    return m_Cartes.plusFaible();
}

int Joueur::trumpNumber()
{
    ListeCartes list;
    m_Cartes.getTrumps(list);
    return list.size();
}

Carte *Joueur::EmptyPickSimulation(Carte &_userChoice)
{
    Carte *playCarte;
    ListeCartes couleurList;

    // we must play a higher card
    m_Cartes.getCouleurSubset(_userChoice.couleur(), couleurList);

    playCarte = couleurList.getHigherCarte(_userChoice);
    if (playCarte == nullptr)
    {
        // play trump if user card is not trump...
        if (!_userChoice.atout())
        {
            ListeCartes trumpList;
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

    if (_otherCarte.gagnante(_card))
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

Carte *Joueur::bruteForceAttack(ListeCartes &_hisCartes)
{
    /*int level = 0;
   bool end = false;
   ListeCartes donorCartes;
   ListeCartes otherCartes;
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