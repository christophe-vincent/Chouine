/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Cards;

import Tools.Tracer;
import Tools.Tracer;
import java.util.LinkedList;
import java.util.List;

/**
 *
 * @author vincentc
 */
public class CardList extends LinkedList<Card>
{
    private List<Announce> m_AnnouncesStats;
    private boolean m_isTrumpSeven;
    
    public CardList()
    {
        m_AnnouncesStats = new LinkedList();
        m_isTrumpSeven = false;
    }
    
    @Override
    public boolean add(Card _card)
    {
        if ((_card.isTrump()) && (_card.cardValue() == Card.CardValue.SEPT))
        {
            m_isTrumpSeven = true;
        }
        return super.add(_card);
    }
    
        
    public boolean remove(Card _card)
    {
        if ((_card.isTrump()) && (_card.cardValue() == Card.CardValue.SEPT))
        {
            m_isTrumpSeven = false;
        }
        return super.remove(_card);
    }
    
    public boolean isTrumpSeven()
    {
        return m_isTrumpSeven;
    }
    
    public Card searchCard(Card.CardValue _value)
    {
        Card ret = null;

        for (Card c : this)
        {
            if (c.cardValue() == _value)
            {
                ret = c;
            }
        }

        return ret;
    }
    
    public CardList getColorSubset(Card.Color _color)
    {
        CardList select = new CardList();

        for (Card c : this)
        {
            if (c.color() == _color)
            {
                select.add(c);
            }
        }

        return select;
    }
    
    
    public void newCardStatistics(Card _card)
    {
        List<Announce> cardAnnounces = new LinkedList();
        boolean create = true;
        
        if ((_card.cardValue() == Card.CardValue.SEPT) 
                || (_card.cardValue() == Card.CardValue.HUIT) 
                || (_card.cardValue() == Card.CardValue.NEUF))
        {
            return; // these cards are not used in announces
        }
        
        // check all possible announces for this card
        for (Announce.AnnounceType at : _card.getAllAnnounceTypes())
        {
            // search in the list if this announce has already statistics
            create = true;
            for (Announce ann : m_AnnouncesStats)
            {
                if ((ann.getColor() == _card.color()) 
                        && (ann.getType() == at))
                {
                    // great the annouce is already known
                    ann.addCard(_card);
                    _card.addProbableAnnounce(ann);
                    create = false;
                }
            }
            
            if (create)
            {
                // announce has not been created, create a new one
                Announce announce = new Announce(_card.color(), at, _card.isTrump(), _card);
                m_AnnouncesStats.add(announce);
                _card.addProbableAnnounce(announce);
            }
        }
    }
    
    public void removeCardStatistics(Card _card)
    {
        if ((_card.cardValue() == Card.CardValue.SEPT) 
                || (_card.cardValue() == Card.CardValue.HUIT) 
                || (_card.cardValue() == Card.CardValue.NEUF))
        {
            return; // these cards are not used in announces
        }
        
        // check all possible announces for this card
        for (Announce.AnnounceType at : _card.getAllAnnounceTypes())
        {
            // search in the list if this announce has already statistics
            for (Announce ann : m_AnnouncesStats)
            {
                if ((ann.getColor() == _card.color()) 
                        && (ann.getType() == at))
                {
                    // great the annouce is already known
                    ann.removeCard(_card);
                    _card.removeProbableAnnounce(ann);
                }
            }
        }
    }
}

