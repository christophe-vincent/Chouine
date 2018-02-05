 /*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ChouineAlgo;

import Cards.Announce;
import Cards.Card;
import Cards.CardList;
import Cards.MoveNode;
import Tools.Tracer;
import java.util.LinkedList;
import java.util.List;

/**
 *
 * @author vincentc
 */
public class Player
{

    public static final int MAX_CARDS = 5;
    protected int m_ID;
    String m_Name;
    CardList m_Cards;      // 5 cards to be played
    List<Card> m_WinCards; // cards win
    List<Card> m_EnemyWinCards;
    LinkedList<Card> m_UnplayedCards;
    List<Announce> m_Announces;
    Announce m_LatestAnnounce;
    Chouine m_Game;
    protected boolean m_IsChouine;
    int m_Level;
    int m_10Der;
    protected boolean m_Change7Trump;

    static public Player createPlayer(int _algoId, Chouine _game, int _id, String _name)
    {
        Player player = null;
        switch (_algoId)
        {
            case 0:
                player = (Player) new PlayerAlgo1(_game, _id, _name);
                break;
            case 1:
                player = (Player) new PlayerAlgo2(_game, _id, _name);
                break;
        }


        return player;
    }

    protected Player(Chouine _game, int _id, String _name)
    {
        m_ID = _id;
        m_Name = _name;
        m_Cards = new CardList();
        m_WinCards = new LinkedList<>();
        m_Announces = new LinkedList();
        m_EnemyWinCards = new LinkedList();
        m_UnplayedCards = new LinkedList();
        m_Game = _game;
        m_10Der = 0;
        m_Level = 10; // max level
        m_LatestAnnounce = null;
        m_Change7Trump = false;
        m_IsChouine = false;
    }

    public String name()
    {
        return m_Name;
    }

    public void setLevel(int _level)
    {
        m_Level = _level;
    }

    public int getLevel()
    {
        return m_Level;
    }

    public void setUnplayedCards(List<Card> _cards)
    {
        m_UnplayedCards.addAll(_cards);
    }

    public List<Card> GetCardsList()
    {
        return m_Cards;
    }

    public List<Card> GetWinCardsList()
    {
        return m_WinCards;
    }

    public Announce getLatestAnnounce()
    {
        return m_LatestAnnounce;
    }
    
    public boolean getIsChouine()
    {
        return m_IsChouine;
    }

    public void enemyWinCards(Card _c1, Card _c2)
    {
        m_EnemyWinCards.add(_c1);
        m_EnemyWinCards.add(_c2);
        setPlayedCard(_c1);
        setPlayedCard(_c2);
    }

    public void setPlayedCard(Card _c)
    {
        if (m_UnplayedCards.contains(_c))
        {
            m_UnplayedCards.remove(_c);
        }
    }

    public Card GetCard(String _name)
    {
        Card card = null;
        for (Card c : m_Cards)
        {
            if (c.displayName().equals(_name))
            {
                card = c;
            }
        }
        return card;
    }

    public boolean hasChange7Trump()
    {
        if (m_Change7Trump)
        {
            m_Change7Trump = true;
            return true;
        }
        return false;
    }

    public Status.Code addCard(Card _card)
    {
        if (m_Cards.size() >= MAX_CARDS)
        {
            return Status.Code.TOO_MANY_CARDS;
        }

        setPlayedCard(_card);
        m_Cards.newCardStatistics(_card);

        if (m_Cards.isEmpty())
        {
            m_Cards.add(_card);
            return Status.Code.OK;
        }

        // check if card is trump or not
        int index = 0;
        if (m_Game.GetTrumpColor() == _card.color())
        {
            // find index of the first trump card
            while ((index < m_Cards.size()) && (m_Cards.get(index).color() != m_Game.GetTrumpColor()))
            {
                index++;
            }

            // now we have index of first trump card
            while ((index < m_Cards.size()) && (_card.value() > m_Cards.get(index).value()))
            {
                index++;
            }
        } else
        {
            while ((index < m_Cards.size())
                    && (_card.value() > m_Cards.get(index).value())
                    && (m_Cards.get(index).color() != m_Game.GetTrumpColor()))
            {
                index++;
            }
        }
        m_Cards.add(index, _card);

        return Status.Code.OK;
    }

    public boolean replaceTrumpCard(String _sevenName, Card _newCard)
    {
        int index = -1;
        Card card = null;
        // search the trump 7 in the cards
        for (Card c : m_Cards)
        {
            if (c.displayName().equals(_sevenName))
            {
                card = c;
            }
        }

        if (card != null)
        {
            // ok, the user has the trump 7
            m_Cards.remove(card);
            m_Cards.removeCardStatistics(_newCard);

            // now insert the new card
            addCard(_newCard);
            return true;
        }

        Tracer.println(Tracer.Verbosity.DEBUG, "Echange 7 atout non autorisé :" + _sevenName + " / " + _newCard.displayName());
        return false;
    }

    public Announce newAnnouce(List<String> _list)
    {
        List<Announce> announces;
        CardList cards = new CardList();
        Card card;

        // first find cards
        for (String str : _list)
        {
            card = GetCard(str);
            if (card != null)
            {
                cards.add(card);
            }
        }

        announces = Announce.isAnnounce(cards, m_Announces);
        if (announces.isEmpty())
        {
            return null;
        } else
        {
            if (announces.get(0).getType() == Announce.AnnounceType.CHOUINE)
            {
                m_IsChouine = true; // game over !
            }
            m_Announces.add(announces.get(0));
            Tracer.println(Tracer.Verbosity.INFO, "New user announce");
            return announces.get(0);
        }

    }

    public Card SimulateMove(Card _enemyChoice, int _pickLeft, boolean _simulate)
    {
        return null;
    }

    protected Card getSmallestTrump()
    {
        int i = 0;

        while ((i < m_Cards.size()) && (!m_Cards.get(i).isTrump()))
        {
            i++;
        }

        if ((i < m_Cards.size()) && (m_Cards.get(i).isTrump()))
        {
            return m_Cards.get(i);
        } else
        {
            return null;
        }
    }

    protected int trumpNumber()
    {
        int count = 0;
        for (Card c : m_Cards)
        {
            if (c.isTrump())
            {
                count++;
            }
        }
        return count;
    }

    protected Card EmptyPickSimulation()
    {
        Card playCard;
        List<Card> colorList;
        int i;
        
        if (m_Level > 5)
        {
            playCard = bruteForceAttack(m_Cards, m_UnplayedCards);
        }
        else
        {
            playCard = m_Cards.get(0);
        }

        return playCard;
    }

    protected Card EmptyPickSimulation(Card _userChoice)
    {
        Card playCard;
        List<Card> colorList;
        int i;
        playCard = m_Cards.get(0);

        // we must play a higher card
        colorList = Card.getColorCards(_userChoice.color(), m_Cards);
        i = 0;
        while ((i < colorList.size()) && (_userChoice.value() > colorList.get(i).value()))
        {
            i++;
        }

        if ((i < colorList.size()) && (_userChoice.value() < colorList.get(i).value()))
        {
            // we found the card !
            playCard = colorList.get(i);
        } else
        {
            // play trump if user card is not trump...
            if (!_userChoice.isTrump())
            {
                colorList = Card.getColorCards(m_Game.GetTrumpColor(), m_Cards);
                if (colorList.size() > 0)
                {
                    playCard = colorList.get(0);
                }

            }
        }

        return playCard;
    }

    // check if the player is allowed to play this card or not
    // it is only done when pick is empty : user must play higher card
    public boolean isCardAllowed(Card _card, Card _otherCard)
    {
        boolean ret = true;

        if (m_Cards.size() == 1)
        {
            return true;
        }

        if (_card.isTrump() && !_otherCard.isTrump())
        {
            return true;
        }

        if (_card.color() == _otherCard.color())
        {
            if (_card.isBetter(_otherCard))
            {
                for (Card c : m_Cards)
                {
                    if ((c.isTrump()) && _otherCard.isBetter(c))
                    {
                        // yes, he could play this card...
                        ret = false;
                    }
                }
            } else
            {
                ret = true;
            }
        } else
        {
            if (_otherCard.isTrump())
            {
                // check if user could play a better trump
                for (Card c : m_Cards)
                {
                    if ((c.isTrump()) && _otherCard.isBetter(c))
                    {
                        // yes, he could play this card...
                        ret = false;
                    }
                }
            } else if (!_card.isTrump())
            {
                // user did not put the same color...
                // check if user has trump card
                for (Card c : m_Cards)
                {
                    if (c.isTrump())
                    {
                        // yes, he could play this card...
                        ret = false;
                    }
                }

                if (ret == true)
                {
                    // user does not have trump, may be he could play the 
                    // same color the the ohter
                    for (Card c : m_Cards)
                    {
                        if ((c.color() == _otherCard.color()) && _otherCard.isBetter(c))
                        {
                            // yes, he could play this card...
                            ret = false;
                        }
                    }
                }
            }
        }


        return ret;
    }

    public Status.Code PlayMove(Card _card)
    {
        m_Cards.removeCardStatistics(_card);
        if (!m_Cards.remove(_card))
        {
            return Status.Code.BAD_CARD;
        }
        return Status.Code.OK;
    }

    public Status.Code WinCards(Card _card1, Card _card2)
    {
        if (m_Cards.size() == 0)
        {
            m_10Der = 10;
        }
        m_WinCards.add(_card1);
        m_WinCards.add(_card2);
        setPlayedCard(_card1);
        setPlayedCard(_card2);
        return Status.Code.OK;
    }

    public int CardLeft()
    {
        return m_Cards.size();
    }

    public int Points()
    {
        int points = 0;

        for (Card c : m_WinCards)
        {
            points += c.getPoints();
        }
        return points;
    }

    public int get10Der()
    {
        return m_10Der;
    }

    public List<Announce> getAnnouces()
    {
        return m_Announces;
    }

    public void displayCards()
    {
        Tracer.print(Tracer.Verbosity.INFO, m_Name + ": ");

        for (Card c : m_Cards)
        {
            Tracer.print(Tracer.Verbosity.INFO, c.displayName() + " - ");
        }
        Tracer.println(Tracer.Verbosity.INFO, "");
    }

    public Card bruteForceAttack(LinkedList<Card> _myCards, LinkedList<Card> _hisCards)
    {
        int level = 0;
        boolean end = false;
        LinkedList<Card> donorCards;
        LinkedList<Card> otherCards;
        LinkedList<LinkedList<MoveNode>> nodesLevels;
        LinkedList<MoveNode> nodes, levelNode;
        int[] cardScore;
        MoveNode node;
        int donorId;

        nodesLevels = new LinkedList<>();

        // root level
        donorCards = new LinkedList<>();
        donorCards.addAll(_myCards);
        otherCards = new LinkedList<>();
        otherCards.addAll(_hisCards);

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
                } else
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
        for (int i=0; i<_myCards.size(); i++)
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
        for (int i=0; i<_myCards.size(); i++)
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
    }

    private LinkedList<MoveNode> ProcessLevel(MoveNode _parentNode,
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
        for (i=0; i<_donorCards.size(); i++)
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
}
