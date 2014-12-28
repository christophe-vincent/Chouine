/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ChouineAlgo;

import Cards.Announce;
import Cards.Card;
import Tools.Tracer;
import java.util.LinkedList;
import java.util.List;

/**
 *
 * @author vincentc
 */
public class PlayerAlgo1 extends Player
{

    public PlayerAlgo1(Chouine _game, int _id, String _name)
    {
        super(_game, _id, _name);
    }

    /**
     *
     * @param _userChoice
     * @param _pickLeft
     * @param _simulate
     * @return
     */
    @Override
    public Card SimulateMove(Card _enemyChoice, int _pickLeft, boolean _simulate)
    {
        Card playCard;
        List<Announce> announces;

        if (!_simulate)
        {
            m_LatestAnnounce = null;
        }

        if (m_Cards.isEmpty())
        {
            return null;
        }
        List<Card> colorList;

        playCard = null;
        if (_enemyChoice != null)
        {
            // the other player has already play a card
            if (_pickLeft == 0)
            {
                playCard = EmptyPickSimulation(_enemyChoice);
            }


            if ((_pickLeft != 0) && (m_Level > 0))
            {
                // if we have 10 or AS with sema color than enemy, try to play it
                // select card with same color than other player
                playCard = chooseBetterCard(_enemyChoice);

                if (playCard == null)
                {
                    playCard = chooseSmallest(_enemyChoice);
                }
            }

            if (playCard == null)
            {
                // very basic algo, choose smallest card
                playCard = chooseSmallest(_enemyChoice);
            }
        } else
        {
            if (_pickLeft == 0)
            {
                playCard = EmptyPickSimulation();
            } else
            {
                // basic algo : return a card with less points
                playCard = chooseSmallest();
            }
        }

        // check announces before playing card
        if ((m_Level > 1) && (!_simulate))
        {
            announces = Announce.isAnnounce(m_Cards, m_Announces);
            List<Announce> possibleAnn = new LinkedList<>();

            // check if one of the announces found contains the card to play
            for (Announce ann : announces)
            {
                if (ann.containsCard(playCard))
                {
                    playCard.setAnnounce(ann);
                    //m_Announces.add(ann);
                    possibleAnn.add(ann);
                    Tracer.println(Tracer.Verbosity.INFO, "New announce : " + ann.getName());
                    m_LatestAnnounce = (Announce) ann.clone();
                }
            }

            // choose the announce with the biggest value
            Announce biggestAnn = null;
            int points = 0;
            for (Announce ann : possibleAnn)
            {
                if (ann.getPoints() > points)
                {
                    points = ann.getPoints();
                    biggestAnn = ann;
                }
            }

            if (biggestAnn != null)
            {
                // the announce must be done now !
                playCard.setAnnounce(biggestAnn);
                m_Announces.add(biggestAnn);
                Tracer.println(Tracer.Verbosity.INFO, "New announce : " + biggestAnn.getName());
                m_LatestAnnounce = (Announce) biggestAnn.clone();
                if (biggestAnn.getType() == Announce.AnnounceType.CHOUINE)
                {
                    m_IsChouine = true; // game over !
                }
            }
        }

        // if user has the seven and only 2 cards left in the pick, or 
        // if played card is the Trump 7, then replace
        if ((m_Level > 2) && (!_simulate))
        {
            if (((playCard.cardValue() == Card.CardValue.SEPT)
                    && (playCard.isTrump())
                    || ((m_Game.GetPickList().size() == 2)
                    && (m_Cards.isTrumpSeven())))
                    && (m_Game.GetPickList().size() >= 2))
            {
                m_Game.change7Trump(m_ID);
                m_Change7Trump = true;
                // restart simulation as cards have changed
                playCard = SimulateMove(_enemyChoice, _pickLeft, _simulate);
            }
        }

        return playCard;
    }

    //
    // it chooses a better card than the enemy , and a 10 or AS to save them
    //
    private Card chooseBetterCard(Card _enemyChoice)
    {
        Card card = null;
        List<Card> colorList;

        // get the cards with same color than enemy
        colorList = Card.getColorCards(_enemyChoice.color(), m_Cards);
        if (colorList.size() > 0)
        {
            for (Card c : colorList)
            {
                // if we own a brisque, play it to save it
                if ((c.getPoints() >= 10) && (!c.isTrump()))
                {
                    // we play this card
                    card = c;
                }
            }

            if (card == null)
            {
                // if enemy plays 2, 3 or 4 points, try to win this card
                for (Card c : colorList)
                {
                    if ((c.getPoints() > _enemyChoice.getPoints()) && (!c.isTrump()))
                    {
                        // we play this card
                        card = c;
                    }
                }
            }
        }
        return card;
    }

    private Card chooseSmallest(Card _enemyChoice)
    {
        Card c = null;

        if (m_Level > 3)
        {
            if ((_enemyChoice.getPoints() >= 1) && (_enemyChoice.getPoints() < 10))
            {
                int index = 0;
                while ((c == null)
                        && (index < m_Cards.size()))
                {
                    if (_enemyChoice.isBetter(m_Cards.get(index)))
                    {
                        if (!m_Cards.get(index).isTrump()) // our card is nor trump
                        {
                            c = m_Cards.get(index);
                        } else
                        {
                            // our card is a trump
                            if ((_enemyChoice.isTrump()) && (m_Cards.get(index).getPoints() < 10))
                            {   // we do not use a brisque here, just lttle value cards
                                c = m_Cards.get(index);
                            }
                        }
                    }
                    index++;
                }
            }
            if ((_enemyChoice.getPoints() >= 10))
            {
                int index = 0;
                while ((c == null)
                        && (index < m_Cards.size()))
                {
                    if (_enemyChoice.isBetter(m_Cards.get(index)))
                    {
                        c = m_Cards.get(index);
                    }
                    index++;
                }
            }
        }


        if (c == null)
        {
            c = chooseSmallest();
        }

        return c;
    }

    private Card chooseSmallest()
    {
        Card c = null;

        if (m_Level > 4)
        {
            if (m_Cards.get(0).getPoints() >= 10)
            {
                // use a trump to win the brisque
                c = getSmallestTrump();
                if (c != null)
                {
                    if ((c.getPoints() > 0) || (trumpNumber() < 5))
                    {
                        c = null;
                    }
                }
            }
        }

        if (c == null)
        {
            c = m_Cards.get(0);
        }

        return c;
    }
}
