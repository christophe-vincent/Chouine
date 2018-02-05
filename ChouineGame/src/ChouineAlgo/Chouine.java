/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ChouineAlgo;

import Cards.Announce;
import Cards.Pick;
import Cards.Card;
import Cards.Card.CardValue;
import Tools.Tracer;
import java.security.NoSuchAlgorithmException;
import java.util.LinkedList;
import java.util.List;

/**
 *
 * @author vincentc
 */
public class Chouine
{

    private Player[] m_Players;
    private int[] m_PlayerAlgo;
    private int[] m_PlayerLevel;
    private Card[] m_PlayerChoices;
    private Pick m_Pick;
    private int m_Donor;
    private boolean m_isChouine;

    public Chouine()
    {
        new Tracer(Tracer.Verbosity.NONE);
        
        m_PlayerAlgo = new int[2];
        m_PlayerLevel = new int [2];
        for (int i=0; i<2; i++)
        {
            m_PlayerAlgo[i] = 0;
            m_PlayerLevel[i] = 10;
        }
        m_Players = null;
        m_isChouine = false;
    }

    public void setVerbosity(Tracer.Verbosity _verb)
    {
        Tracer.SetVerbosity(_verb);
    }
    
    public void NewGame(String _player1Name, String _player2Name) throws NoSuchAlgorithmException
    {
       NewGame(_player1Name, _player2Name, null);
    }

    public void NewGame(String _player1Name, String _player2Name, LinkedList<Integer> _order) throws NoSuchAlgorithmException
    {
        LinkedList<Card> cards;

        cards = new LinkedList<>();
        m_Pick = new Pick();

        m_Players = new Player[2];
        m_Players[0] = Player.createPlayer(m_PlayerAlgo[0], this, 0, _player1Name);
        m_Players[1] = Player.createPlayer(m_PlayerAlgo[1], this, 1, _player2Name);
        
        // set level
        m_Players[0].setLevel(m_PlayerLevel[0]);
        m_Players[1].setLevel(m_PlayerLevel[1]);
        
        m_PlayerChoices = new Card[2];
        m_PlayerChoices[0] = null;
        m_PlayerChoices[1] = null;

        // create card game
        for (Card.Color color : Card.ALL_COLORS)
        {
            for (Card.CardValue value : Card.ALL_VALUES)
            {
                cards.add(new Card(color, value, false));
            }
        }
        
        m_Players[0].setUnplayedCards(cards);
        m_Players[1].setUnplayedCards(cards);
        
        m_Pick.addCards(cards, _order);

        Tracer.println(Tracer.Verbosity.INFO, "Atout : " + m_Pick.getTrumpCard().displayName());
    }

    public String getPlayerName(int _player)
    {
        if (_player > 1)
        {
            return "";
        }
        return m_Players[_player].name();
    }

    public void setPlayerLevel(int _player, int _level)
    {
        if (_player > 1)
        {
            return;
        }
        m_PlayerLevel[_player] = _level;
        if (m_Players != null)
        {
           m_Players[_player].setLevel(_level);
        }
    }
    
    public void setPlayerAlgo(int _player, int _algoId)
    {
        if (_player > 1)
        {
            return;
        }
        m_PlayerAlgo[_player] = _algoId;
    }

    public int getPlayerLevel(int _player)
    {
        if (_player > 1)
        {
            return 0;
        }
        return m_Players[_player].getLevel();
    }

    public void DistributeCards(boolean _random)
    {
        int receiver = 1;
        m_Donor = 0;

        if ((_random) && ((int) Math.round((Math.random())) == 0))
        {
           // TEST : comment two next lines to always have the same behavior
           m_Donor = 1;
           receiver = 0;
        }

        // give 5 cards to players
        for (int i = 0; i < Player.MAX_CARDS; i++)
        {
            m_Players[m_Donor].addCard(m_Pick.pickCard());
            
            m_Players[receiver].addCard(m_Pick.pickCard());
        }

        m_Players[0].displayCards();
        m_Players[1].displayCards();
    }

    public List<Cards.Card> GetPickList()
    {
        return m_Pick.GetCardsList();
    }

    public List<Cards.Card> GetPlayerCardList(int _player)
    {
        if (_player > 1)
        {
            return null;
        }
        return m_Players[_player].GetCardsList();
    }

    public List<Cards.Card> GetPlayerWinCardList(int _player)
    {
        if (_player > 1)
        {
            return null;
        }
        return m_Players[_player].GetWinCardsList();
    }

    public Card.Color GetTrumpColor()
    {
        return m_Pick.getTrumpColor();
    }

    public Card GetTrumpCard()
    {
        return m_Pick.getTrumpCard();
    }

    public Card getPlayerChoice(int _player, boolean _simulate)
    {
        if (_player > 1)
        {
            return null;
        }
        Card playedCard;

        if (m_Donor == _player)
        {
            // this player start to play, so we choose a card the first
            playedCard = m_Players[_player].SimulateMove(null, m_Pick.CardLeft(), _simulate);
        } else
        {
            // other user has already played
            Card c = m_PlayerChoices[(_player + 1) % 2];
            if (c == null)
            {
                return null; // error this should not happen
            } else
            {
                playedCard = m_Players[_player].SimulateMove(c, m_Pick.CardLeft(), _simulate);
            }
        }

        if (!_simulate)
        {
            m_PlayerChoices[_player] = playedCard;
        }

        return playedCard;
    }

    public Announce getPlayerLatestAnnounce(int _player)
    {
        if (_player > 1)
        {
            return null;
        }
        return m_Players[_player].getLatestAnnounce();
    }
    
    public String hasChange7Trump(int _player)
    {
        if (_player > 1)
        {
            return null;
        }
        if (m_Players[_player].hasChange7Trump())
        {
            Card cardSeven = new Card(m_Pick.getTrumpColor(), CardValue.SEPT, true);
            return cardSeven.displayName();
        }
        else
        {
            return null;
        }
    }

    public int donnor()
    {
        return m_Donor;
    }

    public boolean setPlayerChoice(int _player, String _choice)
    {
        if (_player > 1)
        {
            return false;
        }
        Card card;
        card = m_Players[_player].GetCard(_choice);
        // check if user can play this card
        if ((m_Pick.CardLeft() == 0) && (m_Donor != _player))
        {
            if (!m_Players[_player].isCardAllowed(card, m_PlayerChoices[m_Donor]))
            {
                return false;
            }
        }
        m_PlayerChoices[_player] = card;
        return true;
    }

    public String change7Trump(int _player)
    {
        if (_player > 1)
        {
            return null;
        }
        Card cardSeven = new Card(m_Pick.getTrumpColor(), CardValue.SEPT, true);

        if (!m_Players[_player].replaceTrumpCard(cardSeven.displayName(), m_Pick.getTrumpCard()))
        {
            return null;
        }
        m_Pick.replaceTrumpCard(cardSeven);
        return cardSeven.displayName();
    }

    public Announce setPlayerAnnounce(int _player, List<String> _cardNames)
    {
        if (_player > 1)
        {
            return null;
        }

        return m_Players[_player].newAnnouce(_cardNames);
    }

    // return the player index that win
    public int Play()
    {
        int winner;
        boolean ret = false;

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
        m_Players[(winner+1)%2].enemyWinCards(m_PlayerChoices[m_Donor], m_PlayerChoices[(m_Donor + 1) % 2]);

        Tracer.print(Tracer.Verbosity.INFO, ">>> " + m_Players[m_Donor].name() + ": ");
        Tracer.print(Tracer.Verbosity.INFO, m_PlayerChoices[m_Donor].displayName() + " - ");
        Tracer.print(Tracer.Verbosity.INFO, m_Players[(m_Donor + 1) % 2].name() + ": ");
        Tracer.print(Tracer.Verbosity.INFO, m_PlayerChoices[(m_Donor + 1) % 2].displayName() + " - ");
        Tracer.print(Tracer.Verbosity.INFO, "      GAGANT : " + m_Players[winner].name());
        Tracer.println(Tracer.Verbosity.INFO, "");
        Tracer.println(Tracer.Verbosity.INFO, "");

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

        m_PlayerChoices[0] = null;
        m_PlayerChoices[1] = null;

        m_Donor = winner;

        return winner;
    }

    public boolean isGameOver()
    {
        if (m_Players[0].getIsChouine() || m_Players[1].getIsChouine())
        {
            return true;
        }
        
        if (m_Players[0].CardLeft() == 0)
        {
            // gameover
            return true;
        } else
        {
            return false;
        }
    }

    public int getPlayerPoints(int _player)
    {
        if (_player > 1)
        {
            return -1;
        }
        return m_Players[_player].Points();
    }

    public int getPlayer10Der(int _player)
    {
        if (_player > 1)
        {
            return -1;
        }
        return m_Players[_player].get10Der();
    }

    public List<Announce> getPlayerAnnouces(int _player)
    {
        if (_player > 1)
        {
            return new LinkedList();
        }

        return m_Players[_player].getAnnouces();
    }

    public int getPlayerAnnouncesPoints(int _player)
    {
        if (_player > 1)
        {
            return 0;
        }
        int points = 0;
        List<Announce> announces;

        announces = getPlayerAnnouces(_player);
        if (!announces.isEmpty())
        {
            for (Announce an : announces)
            {
                points += an.getPoints();
            }
        }
        return points;
    }

    public int getPlayerTotalPoints(int _player)
    {
        if (_player > 1)
        {
            return 0;
        }

        int points;
        List<Announce> announces;

        points = getPlayerPoints(_player);
        if (getPlayer10Der(_player) > 0)
        {
            points += getPlayer10Der(_player);
        }
        announces = getPlayerAnnouces(_player);
        if (!announces.isEmpty())
        {
            for (Announce an : announces)
            {
                points += an.getPoints();
            }
        }
        return points;
    }
    
    public void testBruteForce()
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
}
