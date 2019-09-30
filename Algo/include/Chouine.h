/**
*
* @author vincentc
*/
#ifndef _CHOUINE_H_
#define _CHOUINE_H
#include <array>

//#include "Card.h"
//#include "Announce.h"
#include "Player.h"
//#include "Pick.h"

class Player;

enum CardId
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
    static Chouine *getInstance();

    void newGame();
    void setPlayerLevel(int _player, int _level);

    CardId getPlayerCard(int _player, int _card);

    int getPlayerChoice(int _player);
    bool setPlayerChoice(int _player, int _choice);

    /*string hasChange7Trump(int _player);
   string change7Trump(int _player);
   int Play();
   bool isGameOver();
   int getPlayerAnnouncesPoints(int _player);
   int getPlayerTotalPoints(int _player);
   void testBruteForce();
   
   
   int getPlayerLevel(int _player)
   {
      if (_player > 1) return 0;
      return m_Players[_player].getLevel();
   }   
   set<Card*> GetPickList() { return m_Pick.GetCards(); }
   set<Card*> GetPlayerCardList(int _player)
   {
      if (_player > 1) return ;
      return m_Players[_player].GetCards();
   }

   set<Card*> GetPlayerWinCardList(int _player)
   {
      if (_player > 1) return nullptr;
      return m_Players[_player].GetWinCards();
   }
   */
    Card::Color getTrumpColor();
    /*  Card* GetTrumpCard() { return m_Pick.getTrumpCard(); }
   Announce* getPlayerLatestAnnounce(int _player)
   {
      if (_player > 1) return nullptr;
      return m_Players[_player].getLatestAnnounce();
   }
   
   int donnor() { return m_Donor; }
   Announce setPlayerAnnounce(int _player, set<string> _cardNames)
   {
      if (_player > 1) return nullptr;
      return m_Players[_player].newAnnouce(_cardNames);
   }
   
   int getPlayerPoints(int _player)
   {
      if (_player > 1) return -1;
      return m_Players[_player].Points();
   }

   int getPlayer10Der(int _player)
   {
      if (_player > 1) return -1;
      return m_Players[_player].get10Der();
   }

   set<Announce*> getPlayerAnnouces(int _player)
   {
      if (_player > 1) return nullptr;
      return m_Players[_player].getAnnouces();
   }*/

private:
    Chouine();
    ~Chouine();
    Card::Color m_Trump;
    static Chouine *m_Instance;
    array<Player *, 2> m_Players;
    array<int, 2> m_PlayerLevel;
    Card *m_PlayedCard;
    CardList m_Pick;
    int m_StartPlayer;
    bool m_isChouine;
};
#endif