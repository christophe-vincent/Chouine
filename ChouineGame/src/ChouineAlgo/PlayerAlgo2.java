/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ChouineAlgo;

import Cards.Announce;
import Cards.Card;
import Tools.Tracer;
import java.util.List;

/**
 *
 * @author vincentc
 */
public class PlayerAlgo2 extends Player
{

    private static final int TRUMP_SCORE = 5;

    public PlayerAlgo2(Chouine _game, int _id, String _name)
    {
        super(_game, _id, _name);
    }

    @Override
    public Card SimulateMove(Card _enemyChoice, int _pickLeft, boolean _simulate)
    {
        Card playCard = null;
        int score;
        int maxScore;
        
        if ((_enemyChoice != null) && (_pickLeft == 0))
        {
            playCard = EmptyPickSimulation(_enemyChoice);
            return playCard;
        }

        for (Card c : m_Cards)
        {
            // score based on card value
            score = -c.getPoints();
            if (c.isTrump())
            {
                score -= TRUMP_SCORE;
            }

            //extra score if an announce can be done on this card
            for (Announce ann : c.getProbableAnnounces())
            {
                score -= (ann.getScore()*0.0f) * ann.getPoints();
            }
            c.setScore(score);
        }

        // for each card, compute a score
        if (_enemyChoice != null)
        {
            for (Card c : m_Cards)
            {
                score = c.getScore();
                if (_enemyChoice.isBetter(c))
                {
                    // our card wins
                    score += 2*_enemyChoice.getPoints();
                    score += 2 * c.getPoints();
                } else
                {
                    // our card loose
                    score -= _enemyChoice.getPoints();
                }
                c.setScore(score);
            }
        }

        // choose the card the the better score
        maxScore = Integer.MIN_VALUE;
        for (Card c : m_Cards)
        {
            if ((c.getScore() > maxScore)
                    && (playCard == null))
            {
                playCard = c;
            }
        }
        
        return playCard;
    }
}
