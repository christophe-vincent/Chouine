/**
*
* @author vincentc
*/
#ifndef _CHOUINE_H_
#define _CHOUINE_H
#include <array>

//#include "Carte.h"
//#include "Announce.h"
#include "Joueur.h"
//#include "Pick.h"

class Joueur;

enum CarteId
{
    COEUR_7,
    COEUR_8,
    COEUR_9,
    COEUR_VALET,
    COEUR_DAME,
    COEUR_ROI,
    COEUR_10,
    COEUR_AS,
    PIC_7,
    PIC_8,
    PIC_9,
    PIC_VALET,
    PIC_DAME,
    PIC_ROI,
    PIC_10,
    PIC_AS,
    TREFLE_7,
    TREFLE_8,
    TREFLE_9,
    TREFLE_VALET,
    TREFLE_DAME,
    TREFLE_ROI,
    TREFLE_10,
    TREFLE_AS,
    CARREAU_7,
    CARREAU_8,
    CARREAU_9,
    CARREAU_VALET,
    CARREAU_DAME,
    CARREAU_ROI,
    CARREAU_10,
    CARREAU_AS,
    NONE
};

class Chouine
{
public:
    enum JOUEUR
    {
        JOUEUR_1 = 0,
        JOUEUR_2 = 1
    };

public:
    Chouine(int _niveauJoueur1, int _niveauJoueur2);
    ~Chouine();
    
    void newGame();

    Joueur& joueur(JOUEUR _id) { return *m_Joueurs[_id];};
    JOUEUR gagnantPli() { return m_GagnantPli;};
    CarteList& pioche() { return m_Pioche; }
    
    CarteId getJoueurCarte(int _player, int _card);
    bool piocheVide();

    string choixJoueur(JOUEUR _player);
    bool setJoueurChoice(int _player, int _choice);
    JOUEUR finPli();
    bool finPartie();

    int pointsJoueur(JOUEUR _joueur);


    /*string hasChange7Trump(int _player);
   string change7Trump(int _player);
   int Play();
   bool isGameOver();
   int getJoueurAnnouncesPoints(int _player);
   int getJoueurTotalPoints(int _player);
   void testBruteForce();
   
   
   int getJoueurLevel(int _player)
   {
      if (_player > 1) return 0;
      return m_Joueurs[_player].getLevel();
   }   
   set<Carte*> GetPickList() { return m_Pick.GetCartes(); }
   set<Carte*> GetJoueurCarteList(int _player)
   {
      if (_player > 1) return ;
      return m_Joueurs[_player].GetCartes();
   }

   set<Carte*> GetJoueurWinCarteList(int _player)
   {
      if (_player > 1) return nullptr;
      return m_Joueurs[_player].GetWinCartes();
   }
   */
    Carte::Couleur couleurAtout() { return m_Atout; };
    std::string atout() { return Carte::nomCouleur(m_Atout);}
    /*  Carte* GetTrumpCarte() { return m_Pick.getTrumpCarte(); }
   Announce* getJoueurLatestAnnounce(int _player)
   {
      if (_player > 1) return nullptr;
      return m_Joueurs[_player].getLatestAnnounce();
   }
   
   int donnor() { return m_Donor; }
   Announce setJoueurAnnounce(int _player, set<string> _cardNames)
   {
      if (_player > 1) return nullptr;
      return m_Joueurs[_player].newAnnouce(_cardNames);
   }
   
   int getJoueurPoints(int _player)
   {
      if (_player > 1) return -1;
      return m_Joueurs[_player].Points();
   }

   int getJoueur10Der(int _player)
   {
      if (_player > 1) return -1;
      return m_Joueurs[_player].get10Der();
   }

   set<Announce*> getJoueurAnnouces(int _player)
   {
      if (_player > 1) return nullptr;
      return m_Joueurs[_player].getAnnouces();
   }*/

private:
    Carte::Couleur m_Atout;
    static Chouine *m_Instance;
    Joueur m_Joueur1;
    Joueur m_Joueur2;
    array<Joueur *, 2> m_Joueurs;
    array<int, 2> m_JoueurLevel;
    CarteList m_Pioche;
    JOUEUR m_GagnantPli;
    bool m_isChouine;
};
#endif