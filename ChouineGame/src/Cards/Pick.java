/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Cards;

import ChouineAlgo.Status;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.util.LinkedList;
import java.util.List;

/**
 *
 * @author vincentc
 */
public class Pick {
    LinkedList<Card> m_Cards;
    Card.Color m_TrumpColor;
    
    public Pick()
    {
        m_Cards = new LinkedList<>();
    }
    
    public void addCards(LinkedList<Card> _cards, LinkedList<Integer> _order) throws NoSuchAlgorithmException
    {
        Card.Color trumpColor;
        SecureRandom prng = SecureRandom.getInstance("SHA1PRNG");
        int cardId = 0;
       
        // cards for the pick
        if (_order != null)
        {
            LinkedList<Card> cardList = new LinkedList<>();
            cardList = (LinkedList<Card>) _cards.clone();
            do
            {
                if (_order.size() > 0)
                {
                  cardId = _order.pop();
                }
                
                m_Cards.add(cardList.get(cardId));                
            } while (_order.size() > 0);
            
            // remove the cards already distrubuted
            for (Card c: m_Cards)
            {
                for (Card cd: _cards)
                {
                    if (c.displayName().equals(cd.displayName()))
                    {
                        _cards.remove(cd);
                        break;
                    }
                }
            }
        }
        
        do
        {
            cardId = (int)(prng.nextFloat()* _cards.size());
            m_Cards.add(_cards.get(cardId));
            _cards.remove(cardId);
        } while (_cards.size() > 0);

        
        
        // for each card indicate if it is trump or not
        m_TrumpColor = getTrumpCard().color();
        for (Card c : m_Cards)
        {
            if (c.color() == m_TrumpColor)
            {
                c.setTrump(true);
            }
        }
    }
    
    public void addCards(LinkedList<Card> _cards) throws NoSuchAlgorithmException
    {
        addCards(_cards, null);
    }
    
    public List<Card> GetCardsList()
    {
        return m_Cards;
    }
    
    public int CardLeft()
    {
        return m_Cards.size();
    }
    
    public Card pickCard()
    {
        Card c = m_Cards.getFirst();
        m_Cards.remove(c);
        return c;
    }
    
    public Card getTrumpCard()
    {
        return m_Cards.getLast();
    }
    
    public Card.Color getTrumpColor()
    {
        return m_TrumpColor;
    }
    
    public void replaceTrumpCard(Card _card)
    {
        m_Cards.set(m_Cards.size()-1, _card);
    }
}
